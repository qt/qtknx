/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

/*!
    \class QKnxNetIpServerDiscoveryAgent

    \inmodule QtKnx
    \brief The QKnxNetIpServerDiscoveryAgent class discovers KNXnet/IP servers
    in the network that the client is connected to.

    Here is an example on how to use this discovery agent:

    \code
        QKnxNetIpServerDiscoveryAgent agent;
        QKnxAddress clientLocalAddress = ...
        agent.setLocalAddress(clientLocalAddress);
        agent.start();
    \endcode

    To retrieve the list of discovered servers:
    \code
        auto servers = agent.discoveredServers();
    \endcode
*/

// -- QKnxNetIpServerDiscoveryAgentPrivate

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
    socket->setSocketOption(QUdpSocket::SocketOption::MulticastTtlOption, ttl);

    QObject::connect(socket, &QUdpSocket::stateChanged, [&](QUdpSocket::SocketState s) {
        Q_Q(QKnxNetIpServerDiscoveryAgent);
        switch (s) {
        case QUdpSocket::BoundState:
            setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Running);

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

                auto data = QKnxNetIpSearchRequest({
                    (nat ? QHostAddress::AnyIPv4 : usedAddress),
                    (nat ? quint16(0u) : usedPort)
                }).bytes();
                socket->writeDatagram((const char*) data.constData(), data.size(),
                    multicastAddress, multicastPort);

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
        static_cast<overload>(&QUdpSocket::error), [&](QUdpSocket::SocketError) {
            setAndEmitErrorOccurred(QKnxNetIpServerDiscoveryAgent::Error::Network,
                socket->errorString());

            Q_Q(QKnxNetIpServerDiscoveryAgent);
            q->stop();
    });

    QObject::connect(socket, &QUdpSocket::readyRead, [&]() {
        Q_Q(QKnxNetIpServerDiscoveryAgent);
        while (socket->hasPendingDatagrams()) {
            if (q->state() != QKnxNetIpServerDiscoveryAgent::State::Running)
                break;

            auto ba = socket->receiveDatagram().data();
            QKnxByteArray data(ba.constData(), ba.size());
            const auto header = QKnxNetIpFrameHeader::fromBytes(data, 0);
            if (!header.isValid() || header.code() != QKnxNetIp::ServiceType::SearchResponse)
                continue;

            auto response = QKnxNetIpSearchResponse::fromBytes(data, 0);
            if (!response.isValid())
                continue;

            setAndEmitDeviceDiscovered({ response.controlEndpoint(), response.deviceHardware(),
                response.supportedFamilies() });
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

        QObject::connect(receiveTimer, &QTimer::timeout, [&] () {
            Q_Q(QKnxNetIpServerDiscoveryAgent);
            if (q->state() == QKnxNetIpServerDiscoveryAgent::State::Running) {
                servers.clear();

                auto data = QKnxNetIpSearchRequest({
                    (nat ? QHostAddress::AnyIPv4 : address),
                    (nat ? quint16(0u) : port)
                }).bytes();
                socket->writeDatagram((const char*) data.constData(), data.size(),
                    multicastAddress, multicastPort);
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


// -- QKnxNetIpServerDiscoveryAgent

QKnxNetIpServerDiscoveryAgent::QKnxNetIpServerDiscoveryAgent(QObject *parent)
    : QKnxNetIpServerDiscoveryAgent(QHostAddress(QHostAddress::AnyIPv4), 0u, parent)
{}

QKnxNetIpServerDiscoveryAgent::~QKnxNetIpServerDiscoveryAgent()
{
    stop();
}

QKnxNetIpServerDiscoveryAgent::QKnxNetIpServerDiscoveryAgent(const QHostAddress &localAddress,
        QObject *parent)
    : QKnxNetIpServerDiscoveryAgent(localAddress, 0u, parent)
{}

QKnxNetIpServerDiscoveryAgent::QKnxNetIpServerDiscoveryAgent(const QHostAddress &localAddress,
        quint16 port, QObject *parent)
    : QKnxNetIpServerDiscoveryAgent(*new QKnxNetIpServerDiscoveryAgentPrivate(localAddress, port),
        parent)
{}

QKnxNetIpServerDiscoveryAgent::State QKnxNetIpServerDiscoveryAgent::state() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->state;
}

QKnxNetIpServerDiscoveryAgent::Error QKnxNetIpServerDiscoveryAgent::error() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->error;
}

QString QKnxNetIpServerDiscoveryAgent::errorString() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->errorString;
}

QVector<QKnxNetIpServerInfo> QKnxNetIpServerDiscoveryAgent::discoveredServers() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->servers;
}

quint16 QKnxNetIpServerDiscoveryAgent::localPort() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::Running)
        return d->usedPort;
    return d->port;
}

void QKnxNetIpServerDiscoveryAgent::setLocalPort(quint16 port)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        d->port = port;
}

QHostAddress QKnxNetIpServerDiscoveryAgent::localAddress() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::Running)
        return d->usedAddress;
    return d->address;
}

void QKnxNetIpServerDiscoveryAgent::setLocalAddress(const QHostAddress &address)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        d->address = address;
}

/*!
    Returns the timeout value used by the discovery agent to wait for incoming
    search response messages. The default value is 3000 milliseconds.

    \sa setTimeout
*/
int QKnxNetIpServerDiscoveryAgent::timeout() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->timeout;
}

/*!
    Sets the timeout for the discovery agent to \a msec. If \a msec is -1,
    the agent will not timeout and has to be terminated by calling the \l stop
    function.

    \sa timeout
    \sa searchFrequency
    \sa setSearchFrequency
*/
void QKnxNetIpServerDiscoveryAgent::setTimeout(int msec)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    d->timeout = msec;
    d->setupAndStartReceiveTimer();
}

/*!
    Returns the search frequency used by the discovery agent to send search
    request messages. The default value is 0.

    \sa setSearchFrequency
*/
int QKnxNetIpServerDiscoveryAgent::searchFrequency() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->frequency;
}

/*!
    Sets the search frequency used by the discovery agent to \a timesPerMinute
    to send search request messages. The default value is 0.

    \sa timeout
    \sa setTimeout
    \sa searchFrequency
*/
void QKnxNetIpServerDiscoveryAgent::setSearchFrequency(int timesPerMinute)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    d->frequency = timesPerMinute;
    if (d->frequencyTimer)
        d->frequencyTimer->setInterval(60000 / timesPerMinute);
}

bool QKnxNetIpServerDiscoveryAgent::natAware() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->nat;
}

void QKnxNetIpServerDiscoveryAgent::setNatAware(bool isAware)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        d->nat = isAware;
}

quint8 QKnxNetIpServerDiscoveryAgent::multicastTtl() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->ttl;
}

void QKnxNetIpServerDiscoveryAgent::setMulticastTtl(quint8 ttl)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    d->ttl = ttl;
    if (d->socket)
        d->socket->setSocketOption(QUdpSocket::SocketOption::MulticastTtlOption, ttl);
}

QKnxNetIpServerDiscoveryAgent::ResponseType QKnxNetIpServerDiscoveryAgent::responseType() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->type;
}

void QKnxNetIpServerDiscoveryAgent::setResponseType(QKnxNetIpServerDiscoveryAgent::ResponseType type)
{
    Q_D(QKnxNetIpServerDiscoveryAgent);
    if (d->state == QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        d->type = type;
}

void QKnxNetIpServerDiscoveryAgent::start()
{
    Q_D(QKnxNetIpServerDiscoveryAgent);

    if (d->state != QKnxNetIpServerDiscoveryAgent::State::NotRunning)
        return;

    auto isIPv4 = true;
    d->address.toIPv4Address(&isIPv4);
    if (isIPv4) {
        d->setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Starting);

        d->setupSocket();
        if (d->type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast) {
            d->socket->bind(QHostAddress::AnyIPv4, d->multicastPort, QUdpSocket::ShareAddress
                | QAbstractSocket::ReuseAddressHint);
        } else {
            d->socket->bind(d->address, d->port);
        }
    } else {
        d->setAndEmitErrorOccurred(Error::NotIPv4, tr("Only IPv4 local address supported."));
    }
}

void QKnxNetIpServerDiscoveryAgent::start(int timeout)
{
    d_func()->timeout = timeout;
    start();
}

void QKnxNetIpServerDiscoveryAgent::stop()
{
    Q_D(QKnxNetIpServerDiscoveryAgent);

    if (d->state == State::Stopping || d->state == State::NotRunning)
        return;

    d->setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::Stopping);

    if (d->type == QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast
        && d->socket->state() == QUdpSocket::BoundState) {
            d->socket->leaveMulticastGroup(d->multicastAddress);
    }
    d->socket->close();

    QKnxPrivate::clearSocket(&(d->socket));
    QKnxPrivate::clearTimer(&(d->receiveTimer));
    QKnxPrivate::clearTimer(&(d->frequencyTimer));

    d->setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State::NotRunning);
}

QKnxNetIpServerDiscoveryAgent::QKnxNetIpServerDiscoveryAgent(QKnxNetIpServerDiscoveryAgentPrivate &dd,
        QObject *parent)
    : QObject(dd, parent)
{}

QT_END_NAMESPACE
