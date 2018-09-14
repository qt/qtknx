/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxnetiproutingbusy.h"
#include "qknxnetiproutingindication.h"
#include "qknxnetiproutinginterface_p.h"
#include "qknxnetiproutinginterface.h"
#include "qknxnetiproutinglostmessage.h"
#include "qknxnetiproutingsystembroadcast.h"

#ifdef QT_BUILD_INTERNAL
#include "qknxtestingrouter_p.h"
#endif

#include <QtCore/qrandom.h>

QT_BEGIN_NAMESPACE

void QKnxNetIpRoutingInterfacePrivate::errorOccurred(QKnxNetIpRoutingInterface::Error error,
    const QString &errorString)
{
    m_error = error;
    m_errorMessage = errorString;
    changeState(QKnxNetIpRoutingInterface::State::Failure);

    Q_Q(QKnxNetIpRoutingInterface);
    emit q->errorOccurred(m_error, m_errorMessage);
}

void QKnxNetIpRoutingInterfacePrivate::changeState(QKnxNetIpRoutingInterface::State state)
{
    if (m_state == state)
        return;
    m_state = state;

    Q_Q(QKnxNetIpRoutingInterface);
    emit q->stateChanged(m_state);
}

void QKnxNetIpRoutingInterfacePrivate::start()
{
    if (m_state != QKnxNetIpRoutingInterface::State::NotInit
        && m_state != QKnxNetIpRoutingInterface::State::Stop)
        return;

#ifdef QT_BUILD_INTERNAL
    TestingRouter::instance()->setRouterInstance(this);
#endif

    if (!m_iface.isValid()) {
        // Choose first interface available and capable of multicasting
        const auto interfaces = QNetworkInterface::allInterfaces();
        for (const auto &iface : interfaces) {
            if (iface.flags().testFlag(QNetworkInterface::IsRunning)
                && iface.flags().testFlag(QNetworkInterface::CanMulticast)
                && !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
                m_iface = iface;
                break;
            }
        }
        // still no interface valid found
        if (!m_iface.isValid()) {
            errorOccurred(QKnxNetIpRoutingInterface::Error::Network,
                QKnxNetIpRoutingInterface::tr("Could not start routing because there isn't a "
                    "valid interface."));
            return;
        }
    }
    m_ownAddress = m_iface.addressEntries().first().ip();

    m_busyTimer = new QTimer;
    m_busyTimer->setSingleShot(true);

    // while neighbor router busy don't overflow him with messages, timeout until some msec
    QObject::connect(m_busyTimer, &QTimer::timeout, [&]() {
        switch (m_busyStage) {
        case BusyTimerStage::NotInit:
            break;
        case BusyTimerStage::Wait:
            m_busyTimer->setInterval(QRandomGenerator().bounded(m_busyCounter * 50));
            m_busyTimer->start();
            m_busyStage = BusyTimerStage::RandomWait;
            break;
        case BusyTimerStage::RandomWait:
            m_busyTimer->setInterval(m_busyCounter * 100);
            m_busyTimer->start();
            m_busyStage = BusyTimerStage::SlowDuration;
            this->changeState(QKnxNetIpRoutingInterface::State::Routing);
            break;
        case BusyTimerStage::SlowDuration:
            m_busyTimer->setInterval(5);
            m_busyTimer->setSingleShot(false);
            m_busyTimer->start();
            m_busyStage = BusyTimerStage::DecrementBusyCounter;
            break;
        case BusyTimerStage::DecrementBusyCounter:
            if (--m_busyCounter == 0) {
                m_busyTimer->stop();
                m_busyStage = BusyTimerStage::NotInit;
                m_busyTimer->setSingleShot(true);
            }
            break;
        }
    });

    m_socket = new QUdpSocket;
    m_socket->setSocketOption(QUdpSocket::SocketOption::MulticastTtlOption, 60);

    // handle QUdpSocket state changes here
    QObject::connect(m_socket, &QUdpSocket::stateChanged, [&](QUdpSocket::SocketState s) {
        switch (s) {
        case QUdpSocket::BoundState:
            m_socket->setMulticastInterface(m_iface);
            if (m_socket->joinMulticastGroup(m_multicastAddress, m_iface)) {
                changeState(QKnxNetIpRoutingInterface::State::Routing);
            } else {
                errorOccurred(QKnxNetIpRoutingInterface::Error::Network,
                    QKnxNetIpRoutingInterface::tr("Could not join multicast group."));
            }
            break;
        case QUdpSocket::ClosingState:
            if (!m_socket->leaveMulticastGroup(m_multicastAddress, m_iface)) {
                changeState(QKnxNetIpRoutingInterface::State::NotInit);
            } else {
                errorOccurred(QKnxNetIpRoutingInterface::Error::Network,
                    QKnxNetIpRoutingInterface::tr("Could not leave multicast group."));
            }
            break;
        default:
            break;
        }
    });

    // handle frames received by the UDP socket
    QObject::connect(m_socket, &QUdpSocket::readyRead, [&]() {
        // TODO: Review this part, the following members might get cleared unexpectedly
        // when messages come in one after the other and are not contained all in a single
        // datagram.
        m_framesReadCount = 0;
        m_sameKnxDstAddressIndicationCount = 0;
        m_lastIndicationAddress = QKnxAddress();
        while (m_socket && m_socket->state() == QUdpSocket::BoundState
            && m_socket->hasPendingDatagrams()) {

            QNetworkDatagram datagram = m_socket->receiveDatagram();
            if (datagram.senderAddress() == m_ownAddress
                || m_framesReadCount == 10 // incoming queue too big, signal busy
                || m_sameKnxDstAddressIndicationCount == 5) {
                    continue; // discard packet
            }

            auto data = QKnxByteArray::fromByteArray(datagram.data());
            const auto header = QKnxNetIpFrameHeader::fromBytes(data, 0);
            if (!header.isValid() || header.totalSize() != data.size())
                continue; // discard packet

            m_framesReadCount++;
            switch (header.serviceType()) {
            case QKnxNetIp::ServiceType::RoutingIndication:
                processRoutingIndication(QKnxNetIpFrame::fromBytes(data, 0));
                break;
            case QKnxNetIp::ServiceType::RoutingBusy:
                processRoutingBusy(QKnxNetIpFrame::fromBytes(data, 0));
                break;
            case QKnxNetIp::ServiceType::RoutingLostMessage:
                processRoutingLostMessage(QKnxNetIpFrame::fromBytes(data, 0));
                break;
            case QKnxNetIp::ServiceType::RoutingSystemBroadcast:
                processRoutingSystemBroadcast(QKnxNetIpFrame::fromBytes(data, 0));
                break;
            default:
                break;
            }
        }

        if (m_framesReadCount == 10 || m_sameKnxDstAddressIndicationCount == 5) {
            // incoming queue over 10 packets or over 5 packets with
            // individual address destination.
            auto routingBusyNetIpFrame = QKnxNetIpRoutingBusyProxy::builder()
                .setDeviceState(QKnxNetIp::DeviceState::KnxFault)
                .setRoutingBusyWaitTime(m_busyWaitTime)
                .setRoutingBusyControl(0)
                .create();
            sendFrame(routingBusyNetIpFrame);
            flowControlHandling(m_busyWaitTime);
        }
    });

    // handle UDP socket errors
    using overload = void (QUdpSocket::*)(QUdpSocket::SocketError);
    QObject::connect(m_socket,
        static_cast<overload>(&QUdpSocket::error), [&](QUdpSocket::SocketError) {
        errorOccurred(QKnxNetIpRoutingInterface::Error::Network,
            m_socket->errorString());
    });


    // initialize UDP socket and bind
    if (!m_socket->bind(QHostAddress(QHostAddress::AnyIPv4), m_multicastPort,
        (QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))) {
            errorOccurred(QKnxNetIpRoutingInterface::Error::Network,
                QKnxNetIpRoutingInterface::tr("Could not bind endpoint: %1")
                .arg(m_socket->errorString()));
    }
}

void QKnxNetIpRoutingInterfacePrivate::restart()
{
    stop();
    start();
}

void QKnxNetIpRoutingInterfacePrivate::stop()
{
    if (m_state == QKnxNetIpRoutingInterface::State::NotInit)
        return;
    cleanup();
}

void QKnxNetIpRoutingInterfacePrivate::cleanup()
{
    if (m_socket) {
        m_socket->disconnect();
        m_socket->deleteLater();
        m_socket = nullptr;
    }

    if (m_busyTimer) {
        (m_busyTimer)->stop();
        (m_busyTimer)->disconnect();
        (m_busyTimer)->deleteLater();
        (m_busyTimer) = nullptr;
    }
    m_busyCounter = 0;
    m_busyStage = BusyTimerStage::NotInit;

    m_errorMessage = QString();
    m_error = QKnxNetIpRoutingInterface::Error::None;
}

void QKnxNetIpRoutingInterfacePrivate::processRoutingIndication(const QKnxNetIpFrame &frame)
{
    QKnxNetIpRoutingIndicationProxy indication(frame);
    if (!indication.isValid()) {
        errorOccurred(QKnxNetIpRoutingInterface::Error::KnxRouting,
            QKnxNetIpRoutingInterface::tr("QKnxNetIp Routing Indication Message is not "
                "correctly formed."));
        return;
    }

    auto cemi = indication.cemi();
    auto currentDstAddress = cemi.destinationAddress();
    if (currentDstAddress.type() == QKnxAddress::Type::Individual
        && m_lastIndicationAddress == currentDstAddress) {
        m_sameKnxDstAddressIndicationCount++;
    } else {
        m_lastIndicationAddress = currentDstAddress;
        m_sameKnxDstAddressIndicationCount = 0;
    }

    Q_Q(QKnxNetIpRoutingInterface);
    emit q->routingIndicationReceived(frame, filterAction(cemi));
}

void QKnxNetIpRoutingInterfacePrivate::processRoutingBusy(const QKnxNetIpFrame &frame)
{
    QKnxNetIpRoutingBusyProxy busyMessage(frame);
    if (!busyMessage.isValid())
        return;

    flowControlHandling(busyMessage.routingBusyWaitTime());

    Q_Q(QKnxNetIpRoutingInterface);
    emit q->routingBusyReceived(frame);
}

void QKnxNetIpRoutingInterfacePrivate::processRoutingLostMessage(const QKnxNetIpFrame &frame)
{
    QKnxNetIpRoutingLostMessageProxy lostMessage(frame);
    if (!lostMessage.isValid()) {
        errorOccurred(QKnxNetIpRoutingInterface::Error::KnxRouting,
            QKnxNetIpRoutingInterface::tr("QKnxNetIp Routing Lost Message is not "
                "correctly formed."));
        return;
    }

    Q_Q(QKnxNetIpRoutingInterface);
    emit q->routingLostCountReceived(frame);
}

void QKnxNetIpRoutingInterfacePrivate::processRoutingSystemBroadcast(const QKnxNetIpFrame &frame)
{
    QKnxNetIpRoutingSystemBroadcastProxy sbc(frame);
    if (sbc.isValid()) {
        Q_Q(QKnxNetIpRoutingInterface);
        emit q->routingSystemBroadcastReceived(frame);
    } else {
        errorOccurred(QKnxNetIpRoutingInterface::Error::KnxRouting,
            QKnxNetIpRoutingInterface::tr("QKnxNetIp Routing System Broadcast is not correctly "
                "formed."));
    }
}

bool QKnxNetIpRoutingInterfacePrivate::sendFrame(const QKnxNetIpFrame &frame)
{
    if (m_state != QKnxNetIpRoutingInterface::State::Routing)
        return true; // no errors, only ignore the frame

    return m_socket->writeDatagram(frame.bytes().toByteArray(),
        m_multicastAddress,
        m_multicastPort) != -1;
}

void QKnxNetIpRoutingInterfacePrivate::flowControlHandling(quint16 newBusyWaitTime)
{
    if (m_busyStage == BusyTimerStage::Wait) {
        auto elapsedTime = (m_busyTimer->interval() - m_busyTimer->remainingTime());
        if (elapsedTime >= 10)
            m_busyCounter++;

        if (m_busyTimer->remainingTime() < newBusyWaitTime) {
            m_busyTimer->stop();
            m_busyTimer->setInterval(newBusyWaitTime);
        }
    } else {
        m_busyTimer->stop();
        m_busyTimer->setInterval(newBusyWaitTime);
        m_busyTimer->setSingleShot(true);
        m_busyStage = BusyTimerStage::Wait;
    }
    m_busyTimer->start();
    changeState(QKnxNetIpRoutingInterface::State::NeighborBusy);
}

QKnxNetIpRoutingInterface::FilterAction
    QKnxNetIpRoutingInterfacePrivate::filterAction(const QKnxLinkLayerFrame &frame)
{
    auto dst = frame.destinationAddress();
    auto extCtrlField = frame.extendedControlField();
    auto hopCount = extCtrlField.hopCount();

    if (dst.type() == QKnxAddress::Type::Group) {
        // TODO: review this part
        auto gAdd = QKnxAddress::createGroup(dst.mainOrAreaSection(), dst.middleOrLineSection(), 0);
        bool routingCondition = true;
        if (m_routingMode == QKnxNetIpRoutingInterface::RoutingMode::Filter)
            routingCondition = m_filterTable.contains(gAdd);
        if (m_routingMode == QKnxNetIpRoutingInterface::RoutingMode::Block)
            return QKnxNetIpRoutingInterface::FilterAction::IgnoreTotally;
        if (routingCondition && hopCount > 0 && hopCount <= 7)
            return QKnxNetIpRoutingInterface::FilterAction::RouteDecremented;
        if (routingCondition && hopCount == 0)
            return QKnxNetIpRoutingInterface::FilterAction::IgnoreAcked;
        return QKnxNetIpRoutingInterface::FilterAction::IgnoreTotally;
    }

    // only gets here if destination address is individual
    if (!m_individualAddress.isValid())
        return QKnxNetIpRoutingInterface::FilterAction::IgnoreTotally;

    auto ownAddress = m_individualAddress;
    bool isLineCoupler = ownAddress.middleOrLineSection() != 0;
    if (isLineCoupler) {
        // sub-line to main line routing
        bool notInOwnSubnetwork = (dst.mainOrAreaSection() != ownAddress.mainOrAreaSection())
            || (dst.middleOrLineSection() == ownAddress.middleOrLineSection());
        if (notInOwnSubnetwork) {
            if (dst.subOrDeviceSection() == 0) // address to this line coupler
                return QKnxNetIpRoutingInterface::FilterAction::ForwardLocally;
            if (hopCount > 0 && hopCount <= 7)
                return QKnxNetIpRoutingInterface::FilterAction::RouteDecremented;
            return QKnxNetIpRoutingInterface::FilterAction::IgnoreAcked;
        }
        // no main line to sub-line routing done by a KNXnet/IP RoutingInteface
        return QKnxNetIpRoutingInterface::FilterAction::IgnoreTotally;
    }

    // backbone to main line
    if (dst.mainOrAreaSection() == ownAddress.mainOrAreaSection()) {
        if (dst.middleOrLineSection() == 0 && dst.subOrDeviceSection() == 0)
            // address to area coupler or backbone router.
            return QKnxNetIpRoutingInterface::FilterAction::ForwardLocally;
        if (hopCount > 0 && hopCount <= 7)
            return QKnxNetIpRoutingInterface::FilterAction::RouteDecremented;
        return QKnxNetIpRoutingInterface::FilterAction::IgnoreAcked;
    }

    // main line to backbone routing
    if (hopCount > 0 && hopCount <= 7)
        return QKnxNetIpRoutingInterface::FilterAction::RouteDecremented;
    return QKnxNetIpRoutingInterface::FilterAction::IgnoreAcked;
}

QT_END_NAMESPACE
