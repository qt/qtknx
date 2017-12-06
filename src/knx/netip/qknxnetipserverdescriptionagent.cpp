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

#include "qknxnetipserverdescriptionagent.h"
#include "qknxnetipserverdescriptionagent_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpServerDescriptionAgent

    \inmodule QtKnx
    \brief The QKnxNetIpServerDescriptionAgent class establishes a point to
    point connection with a KNXnet/IP server and requests its description.

    One must first use the \l QKnxNetIpServerDiscoveryAgent class to choose a
    server, as illustrated by the following code snippet:

    \code
        QKnxNetIpServerDescriptionAgent agent;
        QKnxAddress clientLocalAddress = ...
        agent.setLocalAddress(clientLocalAddress);
        QKnxNetIpServerInfo server = agent.discoveredServers()[0]; // for example
        agent.start(server);
    \endcode

    When the description is received, the class emits a
    \c descriptionReceived(QKnxNetIpServerInfo server) signal containing the
    information received from the server. The description received through this
    point to point connection may be more complete than the one sent during the
    discovery process.
*/

// -- QKnxNetIpServerDescriptionAgentPrivate

QKnxNetIpServerDescriptionAgentPrivate::QKnxNetIpServerDescriptionAgentPrivate(
        const QHostAddress &addr, quint16 prt)
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

void QKnxNetIpServerDescriptionAgentPrivate::setupSocket()
{
    usedPort = port;
    QKnxPrivate::clearSocket(&socket);

    Q_Q(QKnxNetIpServerDescriptionAgent);
    socket = new QUdpSocket(q);
    socket->setSocketOption(QUdpSocket::SocketOption::MulticastTtlOption, ttl);

    QObject::connect(socket, &QUdpSocket::stateChanged, [&](QUdpSocket::SocketState s) {
        Q_Q(QKnxNetIpServerDescriptionAgent);
        switch (s) {
        case QUdpSocket::BoundState:
            setAndEmitStateChanged(QKnxNetIpServerDescriptionAgent::State::Running);

            if (q->state() == QKnxNetIpServerDescriptionAgent::State::Running) {
                m_description = {};
                usedPort = socket->localPort();
                socket->writeDatagram(QKnxNetIpDescriptionRequest({
                    (nat ? QHostAddress::AnyIPv4 : socket->localAddress()),
                    (nat ? quint16(0u) : usedPort)
                }).bytes(), m_server.address(), m_server.port());

                setupAndStartReceiveTimer();
            }
            break;
        default:
            break;
        }
    });

    using overload = void (QUdpSocket::*)(QUdpSocket::SocketError);
    QObject::connect(socket,
        static_cast<overload>(&QUdpSocket::error), [&](QUdpSocket::SocketError) {
            setAndEmitErrorOccurred(QKnxNetIpServerDescriptionAgent::Error::Network,
                socket->errorString());

            Q_Q(QKnxNetIpServerDescriptionAgent);
            q->stop();
    });

    QObject::connect(socket, &QUdpSocket::readyRead, [&]() {
        Q_Q(QKnxNetIpServerDescriptionAgent);
        while (socket->hasPendingDatagrams()) {
            if (q->state() != QKnxNetIpServerDescriptionAgent::State::Running)
                break;

            auto datagram = socket->receiveDatagram();
            const auto header = QKnxNetIpFrameHeader::fromBytes(datagram.data(), 0);
            if (!header.isValid() || header.code() != QKnxNetIp::ServiceType::DescriptionResponse)
                continue;

            auto response = QKnxNetIpDescriptionResponse::fromBytes(datagram.data(), 0);
            if (!response.isValid())
                continue;

            setAndEmitServerDescriptionReceived({ m_server, response.deviceHardware(),
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

void QKnxNetIpServerDescriptionAgentPrivate::setupAndStartReceiveTimer()
{
    Q_Q(QKnxNetIpServerDescriptionAgent);

    QKnxPrivate::clearTimer(&receiveTimer);
    if (timeout >= 0) {
        receiveTimer = new QTimer(q);
        receiveTimer->setSingleShot(true);
        receiveTimer->start(timeout);
        QObject::connect(receiveTimer, &QTimer::timeout, q, &QKnxNetIpServerDescriptionAgent::stop);
    }
}

void QKnxNetIpServerDescriptionAgentPrivate::setAndEmitStateChanged(
                                                   QKnxNetIpServerDescriptionAgent::State newState)
{
    state = newState;

    Q_Q(QKnxNetIpServerDescriptionAgent);
    emit q->stateChanged(newState);

    if (state == QKnxNetIpServerDescriptionAgent::State::Running)
        emit q->started();
    else if (state == QKnxNetIpServerDescriptionAgent::State::NotRunning)
        emit q->finished();
}

void QKnxNetIpServerDescriptionAgentPrivate::setAndEmitServerDescriptionReceived(
                                                            const QKnxNetIpServerInfo &description)
{
    m_description = description;

    Q_Q(QKnxNetIpServerDescriptionAgent);
    emit q->descriptionReceived(description);
}

void QKnxNetIpServerDescriptionAgentPrivate::setAndEmitErrorOccurred(
                             QKnxNetIpServerDescriptionAgent::Error newError, const QString &message)
{
    error = newError;
    errorString = message;

    Q_Q(QKnxNetIpServerDescriptionAgent);
    emit q->errorOccurred(error, errorString);
}


// -- QKnxNetIpServerDescriptionAgent

QKnxNetIpServerDescriptionAgent::QKnxNetIpServerDescriptionAgent(QObject *parent)
    : QKnxNetIpServerDescriptionAgent(QHostAddress(QHostAddress::AnyIPv4), 0u, parent)
{}

QKnxNetIpServerDescriptionAgent::~QKnxNetIpServerDescriptionAgent()
{
    stop();
}

QKnxNetIpServerDescriptionAgent::QKnxNetIpServerDescriptionAgent(const QHostAddress &localAddress,
        QObject *parent)
    : QKnxNetIpServerDescriptionAgent(localAddress, 0u, parent)
{}

QKnxNetIpServerDescriptionAgent::QKnxNetIpServerDescriptionAgent(const QHostAddress &localAddress,
        quint16 port, QObject *parent)
    : QKnxNetIpServerDescriptionAgent(*new QKnxNetIpServerDescriptionAgentPrivate(localAddress, port),
        parent)
{}

QKnxNetIpServerDescriptionAgent::State QKnxNetIpServerDescriptionAgent::state() const
{
    Q_D(const QKnxNetIpServerDescriptionAgent);
    return d->state;
}

QKnxNetIpServerDescriptionAgent::Error QKnxNetIpServerDescriptionAgent::error() const
{
    Q_D(const QKnxNetIpServerDescriptionAgent);
    return d->error;
}

QString QKnxNetIpServerDescriptionAgent::errorString() const
{
    Q_D(const QKnxNetIpServerDescriptionAgent);
    return d->errorString;
}

QKnxNetIpServerInfo QKnxNetIpServerDescriptionAgent::serverDescription() const
{
    Q_D(const QKnxNetIpServerDescriptionAgent);
    return d->m_description;
}

quint16 QKnxNetIpServerDescriptionAgent::localPort() const
{
    Q_D(const QKnxNetIpServerDescriptionAgent);
    if (d->state == QKnxNetIpServerDescriptionAgent::State::Running)
        return d->usedPort;
    return d->port;
}

void QKnxNetIpServerDescriptionAgent::setLocalPort(quint16 port)
{
    Q_D(QKnxNetIpServerDescriptionAgent);
    if (d->state == QKnxNetIpServerDescriptionAgent::State::NotRunning)
        d->port = port;
}

QHostAddress QKnxNetIpServerDescriptionAgent::localAddress() const
{
    Q_D(const QKnxNetIpServerDescriptionAgent);
    return d->address;
}

void QKnxNetIpServerDescriptionAgent::setLocalAddress(const QHostAddress &address)
{
    Q_D(QKnxNetIpServerDescriptionAgent);
    if (d->state == QKnxNetIpServerDescriptionAgent::State::NotRunning)
        d->address = address;
}

/*!
    Returns the timeout value used by the discovery agent to wait for incoming
    search response messages. The default value is 3000 milliseconds.

    \sa setTimeout
*/
int QKnxNetIpServerDescriptionAgent::timeout() const
{
    Q_D(const QKnxNetIpServerDescriptionAgent);
    return d->timeout;
}

/*!
    Sets the timeout for the discovery agent to \a msec. If \a msec is -1,
    the agent will not timeout and has to be terminated by calling the \l stop
    function.

    \sa timeout
*/
void QKnxNetIpServerDescriptionAgent::setTimeout(int msec)
{
    Q_D(QKnxNetIpServerDescriptionAgent);
    d->timeout = msec;
    d->setupAndStartReceiveTimer();
}

bool QKnxNetIpServerDescriptionAgent::natAware() const
{
    Q_D(const QKnxNetIpServerDescriptionAgent);
    return d->nat;
}

void QKnxNetIpServerDescriptionAgent::setNatAware(bool isAware)
{
    Q_D(QKnxNetIpServerDescriptionAgent);
    if (d->state == QKnxNetIpServerDescriptionAgent::State::NotRunning)
        d->nat = isAware;
}

quint8 QKnxNetIpServerDescriptionAgent::multicastTtl() const
{
    Q_D(const QKnxNetIpServerDescriptionAgent);
    return d->ttl;
}

void QKnxNetIpServerDescriptionAgent::setMulticastTtl(quint8 ttl)
{
    Q_D(QKnxNetIpServerDescriptionAgent);
    d->ttl = ttl;
    if (d->socket)
        d->socket->setSocketOption(QUdpSocket::SocketOption::MulticastTtlOption, ttl);
}

void QKnxNetIpServerDescriptionAgent::start(const QKnxNetIpHpai &server)
{
    Q_D(QKnxNetIpServerDescriptionAgent);

    if (d->state != QKnxNetIpServerDescriptionAgent::State::NotRunning)
        return;

    auto isIPv4 = true;
    d->address.toIPv4Address(&isIPv4);
    if (isIPv4) {
        d->setAndEmitStateChanged(QKnxNetIpServerDescriptionAgent::State::Starting);

        d->setupSocket();
        d->m_server = server;
        d->socket->bind(d->address, d->port);
    } else {
        d->setAndEmitErrorOccurred(Error::NotIPv4, tr("Only IPv4 local address supported."));
    }
}

void QKnxNetIpServerDescriptionAgent::start(const QKnxNetIpServerInfo &server)
{
    start(server.controlEndpointAddress(), server.controlEndpointPort());
}

void QKnxNetIpServerDescriptionAgent::start(const QHostAddress &address, quint16 port)
{
    start(QKnxNetIpHpai { address, port });
}

void QKnxNetIpServerDescriptionAgent::stop()
{
    Q_D(QKnxNetIpServerDescriptionAgent);

    if (d->state == State::Stopping || d->state == State::NotRunning)
        return;

    d->setAndEmitStateChanged(QKnxNetIpServerDescriptionAgent::State::Stopping);

    d->socket->close();

    QKnxPrivate::clearSocket(&(d->socket));
    QKnxPrivate::clearTimer(&(d->receiveTimer));

    d->setAndEmitStateChanged(QKnxNetIpServerDescriptionAgent::State::NotRunning);
}

QKnxNetIpServerDescriptionAgent::QKnxNetIpServerDescriptionAgent(
        QKnxNetIpServerDescriptionAgentPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{}

QT_END_NAMESPACE
