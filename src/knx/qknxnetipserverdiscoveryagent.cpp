/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxnetipserverdiscoveryagent.h"
#include "qknxnetipserverdiscoveryagent_p.h"

QT_BEGIN_NAMESPACE

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
                if (socket->joinMulticastGroup(multicastAddress)) {
                    port = multicastPort;
                    address = multicastAddress;
                } else {
                    setAndEmitErrorOccurred(QKnxNetIpServerDiscoveryAgent::Error::Network,
                        QKnxNetIpServerDiscoveryAgent::tr("Could not join multicast group."));
                    q->stop();
                }
            } else {
                port = socket->localPort();
                address = socket->localAddress();
            }

            if (q->state() == QKnxNetIpServerDiscoveryAgent::State::Running) {
                servers.clear();
                socket->writeDatagram(QKnxNetIpSearchRequest({
                    (nat ? QHostAddress::AnyIPv4 : address),
                    (nat ? quint16(0u) : port)
                }).bytes<QByteArray>(), multicastAddress, multicastPort);

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

            auto datagram = socket->receiveDatagram();
            const auto header = QKnxNetIpFrameHeader::fromBytes(datagram.data(), 0);
            if (!header.isValid() || header.code() != QKnxNetIp::ServiceType::SearchResponse)
                continue;

            auto response = QKnxNetIpSearchResponse::fromBytes(datagram.data(), 0);
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
                socket->writeDatagram(QKnxNetIpSearchRequest({
                    (nat ? QHostAddress::AnyIPv4 : address),
                    (nat ? quint16(0u) : port)
                }).bytes<QByteArray>(), multicastAddress, multicastPort);
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
                                                 const QKnxNetIpServerDiscoveryInfo &discoveryInfo)
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

QVector<QKnxNetIpServerDiscoveryInfo> QKnxNetIpServerDiscoveryAgent::discoveredServers() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
    return d->servers;
}

quint16 QKnxNetIpServerDiscoveryAgent::localPort() const
{
    Q_D(const QKnxNetIpServerDiscoveryAgent);
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
    Sets the timeout for the discovery agent to \a msecs. If \a msecs is -1,
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
    Sets the search frequency used by the discovery agent to send search
    request messages. The default value is 0.

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
