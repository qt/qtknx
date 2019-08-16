/******************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "qknxnetipserverdiscoveryagent.h"
#include "qknxnetipserverdiscoveryagent_p.h"

#include <QtNetwork/qnetworkinterface.h>

QT_BEGIN_NAMESPACE

QKnxNetIpServerDiscoveryAgentPrivate::QKnxNetIpServerDiscoveryAgentPrivate(const QHostAddress &addr,
        quint16 prt)
    : port(prt)
    , address(addr)
{}

namespace QKnxPrivate
{
    static void clearSocket(QUdpSocket **socket)
    {
        if (*socket) {
            (*socket)->disconnect();
            (*socket)->deleteLater();
            (*socket) = nullptr;
        }
    }
}

void QKnxNetIpServerDiscoveryAgentPrivate::setupSocket()
{
    usedPort = port;
    usedAddress = address;
    QKnxPrivate::clearSocket(&socket);

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    socket = new QUdpSocket(q);

    QObject::connect(socket, &QUdpSocket::stateChanged, q, [&](QUdpSocket::SocketState s) {
        Q_Q(QKnxNetIpServerDiscoveryAgent);
        switch (s) {
        case QUdpSocket::BoundState:
            setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Running);
            socket->setSocketOption(QUdpSocket::SocketOption::MulticastTtlOption, ttl);

            if (type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast) {
                QNetworkInterface mni;
                const auto interfaces = QNetworkInterface::allInterfaces();
                for (const auto &iface : interfaces) {
                    if (!iface.flags().testFlag(QNetworkInterface::CanMulticast))
                        continue;

                    const auto entries = iface.addressEntries();
                    for (const auto &entry : entries) {
                        auto ip = entry.ip();
                        if (ip.protocol() != QAbstractSocket::NetworkLayerProtocol::IPv4Protocol)
                            continue;
                        if (ip != address)
                            continue;
                        mni = iface;
                        break;
                    }
                }

                if (mni.isValid())
                    socket->setMulticastInterface(mni);

                if (socket->joinMulticastGroup(multicastAddress, mni)) {
                    usedPort = multicastPort;
                    usedAddress = multicastAddress;
                } else {
                    setAndEmitErrorOccurred(QKnxNetIpServerDiscoveryAgent::Error::Network,
                        QKnxNetIpServerDiscoveryAgent::tr("Could not join multicast group."));
                    q->stop();
                }
            } else {
                usedPort = socket->localPort();
                usedAddress = socket->localAddress();
            }

            if (q->state() == QKnxNetIpServerDiscoveryAgent::State::Running) {
                servers.clear();

                const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> flags(mode);
                if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV1)) {
                    auto frame = QKnxNetIpSearchRequestProxy::builder()
                        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
                            .setHostAddress(nat ? QHostAddress::AnyIPv4 : usedAddress)
                            .setPort(nat ? quint16(0u) : usedPort).create()
                        ).create();
                    socket->writeDatagram(frame.bytes().toByteArray(), multicastAddress,
                        multicastPort);
                }

                if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2)) {
                    auto frame = QKnxNetIpSearchRequestProxy::extendedBuilder()
                        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
                            .setHostAddress(nat ? QHostAddress::AnyIPv4 : usedAddress)
                            .setPort(nat ? quint16(0u) : usedPort).create()
                        )
                        .setExtendedParameters(srps).create();
                    socket->writeDatagram(frame.bytes().toByteArray(), multicastAddress,
                        multicastPort);
                }

                setupAndStartReceiveTimer();
                setupAndStartFrequencyTimer();
            }
            break;
        default:
            break;
        }
    });

    using overload = void (QUdpSocket::*)(QUdpSocket::SocketError);
    QObject::connect(socket,
        static_cast<overload>(&QUdpSocket::error), q, [&](QUdpSocket::SocketError) {
            setAndEmitErrorOccurred(QKnxNetIpServerDiscoveryAgent::Error::Network,
                socket->errorString());

            Q_Q(QKnxNetIpServerDiscoveryAgent);
            q->stop();
    });

    QObject::connect(socket, &QUdpSocket::readyRead, q, [&]() {
        Q_Q(QKnxNetIpServerDiscoveryAgent);
        while (socket->hasPendingDatagrams()) {
            if (q->state() != QKnxNetIpServerDiscoveryAgent::State::Running)
                break;

            auto datagram = socket->receiveDatagram();
            auto data = QKnxByteArray::fromByteArray(datagram.data());
            const auto header = QKnxNetIpFrameHeader::fromBytes(data, 0);
            if (!header.isValid())
                continue;

             if (header.serviceType() != QKnxNetIp::ServiceType::SearchResponse &&
                 header.serviceType() != QKnxNetIp::ServiceType::ExtendedSearchResponse) {
                    continue;
             }

            auto frame = QKnxNetIpFrame::fromBytes(data);
            auto response = QKnxNetIpSearchResponseProxy(frame);
            if (!response.isValid())
                continue;

            const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> flags(mode);
            if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV1)
                && !response.isExtended()) {
                    setAndEmitDeviceDiscovered({
                        (nat ? QKnxNetIpHpaiProxy::builder()
                                    .setHostAddress(datagram.senderAddress())
                                    .setPort(datagram.senderPort()).create()
                            : response.controlEndpoint()
                        ), response.deviceHardware(), response.supportedFamilies(),
                        QNetworkInterface::interfaceFromIndex(datagram.interfaceIndex())
                    });
            }

            if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2)
                && response.isExtended()) {
                    const auto optionalDibs = response.optionalDibs();
                    setAndEmitDeviceDiscovered({
                        (nat ? QKnxNetIpHpaiProxy::builder()
                                    .setHostAddress(datagram.senderAddress())
                                    .setPort(datagram.senderPort()).create()
                            : response.controlEndpoint()
                        ), response.deviceHardware(), response.supportedFamilies(),
                        QNetworkInterface::interfaceFromIndex(datagram.interfaceIndex()),
                           [&optionalDibs]() -> QKnxNetIpDib {
                                for (const auto &dib : qAsConst(optionalDibs)) {
                                    if (dib.code() == QKnxNetIp::DescriptionType::TunnelingInfo)
                                        return dib;
                                }
                                return {};
                            }(),
                            [&optionalDibs]() -> QKnxNetIpDib {
                                for (const auto &dib : qAsConst(optionalDibs)) {
                                    if (dib.code() == QKnxNetIp::DescriptionType::ExtendedDeviceInfo)
                                        return dib;
                                }
                                return {};
                            }()
                    });
            }
        }
    });
}

namespace QKnxPrivate
{
    static void clearTimer(QTimer **timer)
    {
        if (*timer) {
            (*timer)->stop();
            (*timer)->disconnect();
            (*timer)->deleteLater();
            (*timer) = nullptr;
        }
    }
}

void QKnxNetIpServerDiscoveryAgentPrivate::setupAndStartReceiveTimer()
{
    Q_Q(QKnxNetIpServerDiscoveryAgent);

    QKnxPrivate::clearTimer(&receiveTimer);
    if (timeout >= 0) {
        receiveTimer = new QTimer(q);
        receiveTimer->setSingleShot(true);
        receiveTimer->start(timeout);
        QObject::connect(receiveTimer, &QTimer::timeout, q, &QKnxNetIpServerDiscoveryAgent::stop);
    }
}

void QKnxNetIpServerDiscoveryAgentPrivate::setupAndStartFrequencyTimer()
{
    Q_Q(QKnxNetIpServerDiscoveryAgent);

    QKnxPrivate::clearTimer(&frequencyTimer);
    if (frequency > 0) {
        frequencyTimer = new QTimer(q);
        frequencyTimer->setSingleShot(false);
        frequencyTimer->start(60000 / frequency);

        QObject::connect(frequencyTimer, &QTimer::timeout, q, [&]() {
            Q_Q(QKnxNetIpServerDiscoveryAgent);
            if (q->state() == QKnxNetIpServerDiscoveryAgent::State::Running) {
                servers.clear();

                const QFlags<QKnxNetIpServerDiscoveryAgent::DiscoveryMode> flags(mode);
                if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV1)) {
                    auto frame = QKnxNetIpSearchRequestProxy::builder()
                        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
                            .setHostAddress(nat ? QHostAddress::AnyIPv4 : usedAddress)
                            .setPort(nat ? quint16(0u) : usedPort).create()
                        ).create();
                    socket->writeDatagram(frame.bytes().toByteArray(), multicastAddress,
                        multicastPort);
                }

                if (flags.testFlag(QKnxNetIpServerDiscoveryAgent::DiscoveryMode::CoreV2)) {
                    auto frame = QKnxNetIpSearchRequestProxy::extendedBuilder()
                        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
                            .setHostAddress(nat ? QHostAddress::AnyIPv4 : usedAddress)
                            .setPort(nat ? quint16(0u) : usedPort).create()
                        )
                        .setExtendedParameters(srps).create();
                    socket->writeDatagram(frame.bytes().toByteArray(), multicastAddress,
                        multicastPort);
                }
            }
        });
    }
}

void QKnxNetIpServerDiscoveryAgentPrivate::setAndEmitStateChanged(
                                                     QKnxNetIpServerDiscoveryAgent::State newState)
{
    state = newState;

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    emit q->stateChanged(newState);

    if (state == QKnxNetIpServerDiscoveryAgent::State::Running)
        emit q->started();
    else if (state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        emit q->finished();
}

void QKnxNetIpServerDiscoveryAgentPrivate::setAndEmitDeviceDiscovered(
                                                 const QKnxNetIpServerInfo &discoveryInfo)
{
    servers.append(discoveryInfo);

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    emit q->deviceDiscovered(discoveryInfo);
}

void QKnxNetIpServerDiscoveryAgentPrivate::setAndEmitErrorOccurred(
                             QKnxNetIpServerDiscoveryAgent::Error newError, const QString &message)
{
    error = newError;
    errorString = message;

    Q_Q(QKnxNetIpServerDiscoveryAgent);
    emit q->errorOccurred(error, errorString);
}

namespace QKnxPrivate
{
    QString interfaceFromAddress(const QHostAddress &address)
    {
        auto interfaces = QNetworkInterface::allInterfaces();
        for (const auto &interface : qAsConst(interfaces)) {
            auto entries = interface.addressEntries();
            for (const auto &entry : entries) {
                if (entry.ip() == address)
                    return interface.humanReadableName();
            }
        }
        return QLatin1String("Unknown");
    }
}
void QKnxNetIpServerDiscoveryAgentPrivate::start()
{
    if (state != QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        return;

    if (address.isNull())
        address = QHostAddress(QHostAddress::AnyIPv4);

    auto isIPv4 = true;
    address.toIPv4Address(&isIPv4);
    if (isIPv4) {
        setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Starting);
        setupSocket();
        if (type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast) {
            socket->bind(QHostAddress::AnyIPv4, multicastPort, QUdpSocket::ShareAddress
                | QAbstractSocket::ReuseAddressHint);
            qDebug() << "Multicast response using iface:" << QKnxPrivate::interfaceFromAddress(address);
        } else {
            socket->bind(address, port);
            qDebug() << "Unicast response using interface:" << QKnxPrivate::interfaceFromAddress(address);
        }
    } else {
        setAndEmitErrorOccurred(QKnxNetIpServerDiscoveryAgent::Error::NotIPv4,
            QKnxNetIpServerDiscoveryAgent::tr("Only IPv4 local address supported."));
    }
}

void QKnxNetIpServerDiscoveryAgentPrivate::stop()
{
    if (state == QKnxNetIpServerDiscoveryAgent::State::Stopping
            || state == QKnxNetIpServerDiscoveryAgent::State::NotRunning) {
        return;
    }

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Stopping);

    if (socket) {
        if (type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast
            && socket->state() == QUdpSocket::BoundState) {
            socket->leaveMulticastGroup(multicastAddress);
        }
        socket->close();
    }

    QKnxPrivate::clearSocket(&(socket));
    QKnxPrivate::clearTimer(&(receiveTimer));
    QKnxPrivate::clearTimer(&(frequencyTimer));

    setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::NotRunning);
}

QT_END_NAMESPACE
