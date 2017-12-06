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

#include "qknxnetipconnectrequest.h"
#include "qknxnetipconnectresponse.h"
#include "qknxnetipconnectionstaterequest.h"
#include "qknxnetipconnectionstateresponse.h"
#include "qknxnetipdeviceconfigurationacknowledge.h"
#include "qknxnetipdeviceconfigurationrequest.h"
#include "qknxnetipdisconnectrequest.h"
#include "qknxnetipdisconnectresponse.h"
#include "qknxnetipendpointconnection.h"
#include "qknxnetipendpointconnection_p.h"
#include "qknxnetiptunnelingacknowledge.h"
#include "qknxnetiptunnelingrequest.h"
#include "qnetworkdatagram.h"
#include "qudpsocket.h"

QT_BEGIN_NAMESPACE

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

    static void clearSocket(QUdpSocket **socket)
    {
        if (*socket) {
            (*socket)->disconnect();
            (*socket)->deleteLater();
            (*socket) = nullptr;
        }
    }
}

void QKnxNetIpEndpointConnectionPrivate::setupTimer()
{
    QKnxPrivate::clearTimer(&m_heartbeatTimer);
    QKnxPrivate::clearTimer(&m_connectRequestTimer);
    QKnxPrivate::clearTimer(&m_connectionStateTimer);
    QKnxPrivate::clearTimer(&m_disconnectRequestTimer);
    QKnxPrivate::clearTimer(&m_acknowledgeTimer);

    Q_Q(QKnxNetIpEndpointConnection);
    m_heartbeatTimer = new QTimer(q);
    m_heartbeatTimer->setSingleShot(true);
    QObject::connect(m_heartbeatTimer, &QTimer::timeout, [&]() {
        sendStateRequest();
    });

    m_connectRequestTimer = new QTimer(q);
    m_connectRequestTimer->setSingleShot(true);
    QObject::connect(m_connectRequestTimer, &QTimer::timeout, [&]() {
        setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Bound);
        setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Acknowledge,
            QKnxNetIpEndpointConnection::tr("Connect request timeout."));

        Q_Q(QKnxNetIpEndpointConnection);
        q->disconnectFromHost();
    });

    m_connectionStateTimer = new QTimer(q);
    m_connectionStateTimer->setSingleShot(true);
    QObject::connect(m_connectionStateTimer, &QTimer::timeout, [&]() {
        m_heartbeatTimer->stop();
        m_connectionStateTimer->stop();
        if (m_stateRequests > m_maxStateRequests) {
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Heartbeat,
                QKnxNetIpEndpointConnection::tr("Connection state request timeout."));

            Q_Q(QKnxNetIpEndpointConnection);
            q->disconnectFromHost();
        } else {
            sendStateRequest();
        }
    });

    m_disconnectRequestTimer = new QTimer(q);
    m_disconnectRequestTimer->setSingleShot(true);
    QObject::connect(m_disconnectRequestTimer, &QTimer::timeout, [&] () {
        setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Acknowledge,
            QKnxNetIpEndpointConnection::tr("Disconnect request timeout."));
        process(QKnxNetIpDisconnectResponse(m_channelId, QKnxNetIp::Error::None));
    });

    m_acknowledgeTimer = new QTimer(q);
    m_acknowledgeTimer->setSingleShot(true);
    QObject::connect(m_acknowledgeTimer, &QTimer::timeout, [&]() {
        if (m_cemiRequests > m_maxCemiRequest) {
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Cemi,
                QKnxNetIpEndpointConnection::tr("Did not receive acknowledge in time."));

            Q_Q(QKnxNetIpEndpointConnection);
            q->disconnectFromHost();
        } else {
            m_waitForAcknowledgement = false;
            sendCemiRequest();
        }
    });
}

void QKnxNetIpEndpointConnectionPrivate::setup()
{
    setupTimer();

    m_channelId = -1;

    m_sendCount = 0;
    m_receiveCount = 0;
    m_cemiRequests = 0;
    m_lastSendCemiRequest = {};

    m_stateRequests = 0;
    m_lastStateRequest = {};

    m_nat = m_user.natAware;
    m_supportedVersions = m_user.supportedVersions;

    m_errorString = QString();
    m_error = QKnxNetIpEndpointConnection::Error::None;

    QObject::connect(m_dataEndpoint, &QUdpSocket::readyRead, [&]() {
        while (m_dataEndpoint && m_dataEndpoint->state() == QUdpSocket::BoundState
            && m_dataEndpoint->hasPendingDatagrams()) {
                auto datagram = m_dataEndpoint->receiveDatagram();
                const auto header = QKnxNetIpFrameHeader::fromBytes(datagram.data(), 0);
                if (header.isValid() && header.totalSize() == datagram.data().size()) {
                    // TODO: fix the version and validity checks
                    // if (!m_supportedVersions.contains(header.protocolVersion())) {
                    //     send E_VERSION_NOT_SUPPORTED confirmation frame
                    //     send disconnect request
                    // } else if (header.protocolVersion() != m_dataEndpointVersion {
                    //     send disconnect request
                    // } else {
                    // TODO: set the m_dataEndpointVersion once we receive or send the first frame
                        switch (header.code()) {
                        case QKnxNetIp::ServiceType::TunnelingRequest:
                            process(QKnxNetIpTunnelingRequest::fromBytes(datagram.data(), 0));
                            break;
                        case QKnxNetIp::ServiceType::TunnelingAcknowledge:
                            process(QKnxNetIpTunnelingAcknowledge::fromBytes(datagram.data(), 0));
                            break;
                        case QKnxNetIp::ServiceType::DeviceConfigurationRequest:
                            process(QKnxNetIpDeviceConfigurationRequest::fromBytes(datagram.data(),
                                0));
                            break;
                        case QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge:
                            process(QKnxNetIpDeviceConfigurationAcknowledge::fromBytes(datagram
                                .data(), 0));
                            break;
                        default:
                            processDatagram(QKnxNetIpEndpointConnection::EndpointType::Data, datagram);
                            break;
                        }
                    // }
                }
        }
    });

    using overload = void (QUdpSocket::*)(QUdpSocket::SocketError);
    QObject::connect(m_dataEndpoint,
        static_cast<overload>(&QUdpSocket::error), [&](QUdpSocket::SocketError) {
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Network,
                m_dataEndpoint->errorString());

            Q_Q(QKnxNetIpEndpointConnection);
            q->disconnectFromHost();
    });

    QObject::connect(m_controlEndpoint, &QUdpSocket::readyRead, [&]() {
        while (m_controlEndpoint && m_controlEndpoint->state() == QUdpSocket::BoundState
            && m_controlEndpoint->hasPendingDatagrams()) {
                auto datagram = m_controlEndpoint->receiveDatagram();
                const auto header = QKnxNetIpFrameHeader::fromBytes(datagram.data(), 0);
                if (header.isValid() && header.totalSize() == datagram.data().size()) {
                    // TODO: fix the version and validity checks
                    // if (!m_supportedVersions.contains(header.protocolVersion())) {
                    //     send E_VERSION_NOT_SUPPORTED confirmation frame
                    //     send disconnect request
                    // } else if (header.protocolVersion() != m_controlEndpointVersion) {
                    //     send disconnect request
                    // } else {
                        switch (header.code()) {
                        case QKnxNetIp::ServiceType::ConnectResponse:
                            process(QKnxNetIpConnectResponse::fromBytes(datagram.data(), 0),
                                datagram);
                            break;
                        case QKnxNetIp::ServiceType::ConnectionStateResponse:
                            process(QKnxNetIpConnectionStateResponse::fromBytes(datagram.data(), 0));
                            break;
                        case QKnxNetIp::ServiceType::DisconnectRequest:
                            process(QKnxNetIpDisconnectRequest::fromBytes(datagram.data(), 0));
                            break;
                        case QKnxNetIp::ServiceType::DisconnectResponse:
                            process(QKnxNetIpDisconnectResponse::fromBytes(datagram.data(), 0));
                            break;
                        default:
                            processDatagram(QKnxNetIpEndpointConnection::EndpointType::Control,
                                datagram);
                            break;
                        }
                    // }
                }
        }
    });

    using overload = void (QUdpSocket::*)(QUdpSocket::SocketError);
    QObject::connect(m_controlEndpoint,
        static_cast<overload>(&QUdpSocket::error), [&](QUdpSocket::SocketError) {
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Network,
                m_controlEndpoint->errorString());

            Q_Q(QKnxNetIpEndpointConnection);
            q->disconnectFromHost();
    });
}

void QKnxNetIpEndpointConnectionPrivate::cleanup()
{
    QKnxPrivate::clearTimer(&m_heartbeatTimer);
    QKnxPrivate::clearTimer(&m_connectRequestTimer);
    QKnxPrivate::clearTimer(&m_connectionStateTimer);
    QKnxPrivate::clearTimer(&m_disconnectRequestTimer);
    QKnxPrivate::clearTimer(&m_acknowledgeTimer);

    if (m_dataEndpoint) m_dataEndpoint->close();
    if (m_controlEndpoint) m_controlEndpoint->close();

    QKnxPrivate::clearSocket(&m_dataEndpoint);
    QKnxPrivate::clearSocket(&m_controlEndpoint);

    setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Disconnected);
}

bool QKnxNetIpEndpointConnectionPrivate::sendCemiRequest()
{
    if (!m_waitForAcknowledgement) {
        m_waitForAcknowledgement = true;
        m_dataEndpoint->writeDatagram(m_lastSendCemiRequest, m_remoteDataEndpoint.address,
            m_remoteDataEndpoint.port);
        m_cemiRequests++;
        m_acknowledgeTimer->start(m_acknowledgeTimeout);
    }
    return !m_waitForAcknowledgement;
}

void QKnxNetIpEndpointConnectionPrivate::sendStateRequest()
{
    qDebug().noquote().nospace() << "Sending connection state request: 0x" << m_lastStateRequest
        .toHex();

    m_controlEndpoint->writeDatagram(m_lastStateRequest, m_remoteControlEndpoint.address,
        m_remoteControlEndpoint.port);

    m_stateRequests++;
    m_connectionStateTimer->start(QKnxNetIp::ConnectionStateRequestTimeout);
}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxCemiFrame &)
{}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxNetIpTunnelingRequest &request)
{
    qDebug() << "Received tunneling request:" << request;

    if (request.channelId() == m_channelId) {
        if (bool counterEquals = (request.sequenceCount() == m_receiveCount)
            || (request.sequenceCount() + 1 == m_receiveCount)) {
                // sequence equals -> acknowledge -> process frame
                // sequence -1 -> acknowledge -> drop frame
                auto ack = QKnxNetIpTunnelingAcknowledge(m_channelId, m_receiveCount,
                    QKnxNetIp::Error::None);

                qDebug() << "Sending tunneling acknowledge:" << ack;
                m_dataEndpoint->writeDatagram(ack.bytes(), m_remoteDataEndpoint.address,
                    m_remoteDataEndpoint.port);

                if (!counterEquals)
                    return;
                m_receiveCount++;
                process(request.cemi());
        }
    } else {
        qDebug() << "Request was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << request.channelId();
    }
}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxNetIpTunnelingAcknowledge &acknowledge)
{
    qDebug() << "Received tunneling acknowledge:" << acknowledge;

    if (acknowledge.channelId() == m_channelId) {
        m_acknowledgeTimer->stop();
        m_waitForAcknowledgement = false;
        if (acknowledge.status() == QKnxNetIp::Error::None
            && acknowledge.sequenceCount() == m_sendCount) {
                m_sendCount++;
                m_cemiRequests = 0;
        } else {
            sendCemiRequest();
        }
    } else {
        qDebug() << "Acknowledge was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << acknowledge.channelId();
    }
}

bool QKnxNetIpEndpointConnectionPrivate::sendTunnelingRequest(const QKnxTunnelFrame &frame)
{
    m_lastSendCemiRequest = QKnxNetIpTunnelingRequest(m_channelId, m_sendCount, frame).bytes();
    qDebug().noquote().nospace() << "Sending tunneling request: 0x" << m_lastSendCemiRequest.toHex();

    return sendCemiRequest();
}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxNetIpDeviceConfigurationRequest &request)
{
    qDebug() << "Received device configuration request:" << request;

    if (request.channelId() == m_channelId) {
        if (request.sequenceCount() == m_receiveCount) {
                auto ack = QKnxNetIpDeviceConfigurationAcknowledge(m_channelId, m_receiveCount,
                    QKnxNetIp::Error::None);

                qDebug() << "Sending device configuration acknowledge:" << ack;
                m_dataEndpoint->writeDatagram(ack.bytes(), m_remoteDataEndpoint.address,
                    m_remoteDataEndpoint.port);

                m_receiveCount++;
                if (m_waitForAcknowledgement)
                    m_lastReceivedCemiRequest = request.cemi().bytes();
                else
                    process(request.cemi());
        }
    } else {
        qDebug() << "Request was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << request.channelId();
    }
}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxNetIpDeviceConfigurationAcknowledge &ack)
{
    qDebug() << "Received device configuration acknowledge:" << ack;

    if (ack.channelId() == m_channelId) {
        m_acknowledgeTimer->stop();
        m_waitForAcknowledgement = false;
        if (ack.status() == QKnxNetIp::Error::None
            && ack.sequenceCount() == m_sendCount) {
                m_sendCount++;
                m_cemiRequests = 0;
                if (!m_lastReceivedCemiRequest.isEmpty()) {
                     process(QKnxCemiFrame::fromBytes(m_lastReceivedCemiRequest, 0,
                         m_lastReceivedCemiRequest.size()));
                     m_lastReceivedCemiRequest.clear();
                }
        } else {
            sendCemiRequest();
        }
    } else {
        qDebug() << "Acknowledge was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << ack.channelId();
    }
}

bool QKnxNetIpEndpointConnectionPrivate::sendDeviceConfigurationRequest(const QKnxLocalDeviceManagementFrame &frame)
{
    m_lastSendCemiRequest = QKnxNetIpDeviceConfigurationRequest(m_channelId, m_sendCount, frame).bytes();
    qDebug().noquote().nospace() << "Sending device configuration request: 0x" << m_lastSendCemiRequest
        .toHex();
    return sendCemiRequest();
}

namespace QKnxPrivate
{
    static bool isNullOrLocal(const QHostAddress &address)
    {
        return address.isNull() || address == QHostAddress::Any || address.isLoopback();
    }
}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxNetIpConnectResponse &response,
    const QNetworkDatagram &datagram)
{
    qDebug() << "Received connect response:" << response;

    if (m_state == QKnxNetIpEndpointConnection::State::Connecting) {
        if (response.status() == QKnxNetIp::Error::None) {
            QKnxPrivate::clearTimer(&m_connectRequestTimer);

            m_channelId = response.channelId();
            m_remoteDataEndpoint = response.dataEndpoint();
            if (m_nat) {
                if (QKnxPrivate::isNullOrLocal(m_remoteDataEndpoint.address)
                    || m_remoteDataEndpoint.port == 0) {
                        m_remoteDataEndpoint.port = datagram.senderPort();
                        m_remoteDataEndpoint.address = datagram.senderAddress();
                }
            }

            m_lastStateRequest = QKnxNetIpConnectionStateRequest(m_channelId,
                m_nat ? m_natEndpoint : m_localControlEndpoint).bytes();

            QTimer::singleShot(0, [&]() { sendStateRequest(); });
            setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Connected);
        } else {
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Acknowledge,
                QKnxNetIpEndpointConnection::tr("Could not connect to remote control endpoint. "
                    "Error code: 0x%1").arg(quint8(response.status()), 2, 16, QLatin1Char('0')));

            Q_Q(QKnxNetIpEndpointConnection);
            q->disconnectFromHost();
        }
    } else {
        qDebug() << "Response was ignored due to current state. Expected:"
            << QKnxNetIpEndpointConnection::State::Connecting << "Current:" << m_state;
    }
}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxNetIpConnectionStateResponse &response)
{
    qDebug() << "Received connection state response:" << response;

    if (response.channelId() == m_channelId) {
        if (response.status() == QKnxNetIp::Error::None) {
            m_stateRequests = 0;
            m_connectionStateTimer->stop();
            m_heartbeatTimer->start(m_heartbeatTimeout);
        } else if (!m_connectionStateTimer->isActive()) {
            sendStateRequest();
        }
    } else {
        qDebug() << "Response was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << response.channelId();
    }
}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxNetIpDisconnectRequest &request)
{
    qDebug() << "Received disconnect request:" << request;

    if (request.channelId() == m_channelId) {
        auto response = QKnxNetIpDisconnectResponse(m_channelId, QKnxNetIp::Error::None);
        qDebug() << "Sending disconnect response:" << response;
        m_controlEndpoint->writeDatagram(response.bytes(), m_remoteControlEndpoint.address,
            m_remoteControlEndpoint.port);

        Q_Q(QKnxNetIpEndpointConnection);
        q->disconnectFromHost();
    } else {
        qDebug() << "Response was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << request.channelId();
    }
}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxNetIpDisconnectResponse &response)
{
    qDebug() << "Received disconnect response:" << response;

    if (response.channelId() == m_channelId) {
        cleanup();
    } else {
        qDebug() << "Response was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << response.channelId();
    }
}

void QKnxNetIpEndpointConnectionPrivate::processDatagram(QKnxNetIpEndpointConnection::EndpointType,
    const QNetworkDatagram &)
{}

void QKnxNetIpEndpointConnectionPrivate::setAndEmitStateChanged(
    QKnxNetIpEndpointConnection::State newState)
{
    m_state = newState;

    Q_Q(QKnxNetIpEndpointConnection);
    emit q->stateChanged(newState);

    if (m_state == QKnxNetIpEndpointConnection::State::Connected)
        emit q->connected();
    else if (m_state == QKnxNetIpEndpointConnection::State::Disconnected)
        emit q->disconnected();
}

void QKnxNetIpEndpointConnectionPrivate::setAndEmitErrorOccurred(
    QKnxNetIpEndpointConnection::Error newError, const QString &message)
{
    m_error = newError;
    m_errorString = message;

    Q_Q(QKnxNetIpEndpointConnection);
    emit q->errorOccurred(m_error, m_errorString);
}


// -- QKnxNetIpClient

QKnxNetIpEndpointConnection::~QKnxNetIpEndpointConnection()
{
    disconnectFromHost();
}

QKnxNetIpEndpointConnection::State QKnxNetIpEndpointConnection::state() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_state;
}

QString QKnxNetIpEndpointConnection::errorString() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_errorString;
}

QKnxNetIpEndpointConnection::Error QKnxNetIpEndpointConnection::error() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_error;
}

int QKnxNetIpEndpointConnection::sequenceCount(SequenceType type) const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return (type == SequenceType::Send ?  d->m_sendCount : d->m_receiveCount);
}

quint8 QKnxNetIpEndpointConnection::netIpHeaderVersion(EndpointType endpoint) const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return (endpoint == EndpointType::Data ? d->m_dataEndpointVersion : d->m_controlEndpointVersion);
}

quint16 QKnxNetIpEndpointConnection::localPort() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_localControlEndpoint.port;
}

void QKnxNetIpEndpointConnection::setLocalPort(quint16 port)
{
    Q_D(QKnxNetIpEndpointConnection);
    d->m_user.port = port;
}

QHostAddress QKnxNetIpEndpointConnection::localAddress() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    if (d->m_state == QKnxNetIpEndpointConnection::Disconnected)
        return d->m_user.address;
    return d->m_localControlEndpoint.address;
}

void QKnxNetIpEndpointConnection::setLocalAddress(const QHostAddress &address)
{
    Q_D(QKnxNetIpEndpointConnection);
    d->m_user.address = address;
}

bool QKnxNetIpEndpointConnection::natAware() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    if (d->m_state == QKnxNetIpEndpointConnection::Disconnected)
        return d->m_user.natAware;
    return d->m_nat;
}

void QKnxNetIpEndpointConnection::setNatAware(bool isAware)
{
    Q_D(QKnxNetIpEndpointConnection);
    d->m_user.natAware = isAware;
}

quint32 QKnxNetIpEndpointConnection::heartbeatTimeout() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_heartbeatTimeout;
}

void QKnxNetIpEndpointConnection::setHeartbeatTimeout(quint32 msec)
{
    if (msec > QKnxNetIp::ConnectionAliveTimeout)
        return;

    Q_D(QKnxNetIpEndpointConnection);
    d->m_heartbeatTimeout = msec;
    if (d->m_heartbeatTimer)
        d->m_heartbeatTimer->setInterval(msec);
}

QVector<quint8> QKnxNetIpEndpointConnection::supportedProtocolVersions() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    if (d->m_state == QKnxNetIpEndpointConnection::Disconnected)
        return d->m_user.supportedVersions;
    return d->m_supportedVersions;
}

void QKnxNetIpEndpointConnection::setSupportedProtocolVersions(const QVector<quint8> &versions)
{
    Q_D(QKnxNetIpEndpointConnection);
    d->m_user.supportedVersions = versions;
}

void QKnxNetIpEndpointConnection::connectToHost(const QKnxNetIpHpai &controlEndpoint)
{
    connectToHost(controlEndpoint.address(), controlEndpoint.port());
}

void QKnxNetIpEndpointConnection::connectToHost(const QHostAddress &address, quint16 port)
{
    Q_D(QKnxNetIpEndpointConnection);

    if (d->m_state != State::Disconnected)
        return;

    auto isIPv4 = false;
    address.toIPv4Address(&isIPv4);
    if (!isIPv4) {
        d->setAndEmitErrorOccurred(Error::NotIPv4, tr("Only IPv4 addresses as remote control "
            "endpoint supported."));
        return;
    }
    d->m_remoteControlEndpoint = Endpoint(address, port);

    isIPv4 = false;
    d->m_user.address.toIPv4Address(&isIPv4);
    if (!isIPv4) {
        d->setAndEmitErrorOccurred(Error::NotIPv4, tr("Only IPv4 addresses as local control "
            "endpoint supported."));
        return;
    }

    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Starting);

    auto socket = new QUdpSocket(this);
    QKnxPrivate::clearSocket(&(d->m_controlEndpoint));
    if (!socket->bind(d->m_user.address, d->m_user.port)) {
        d->setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Network,
            QKnxNetIpEndpointConnection::tr("Could not bind local control endpoint: %1")
                .arg(socket->errorString()));
        QKnxPrivate::clearSocket(&socket);
        d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Disconnected);
        return;
    }
    d->m_controlEndpoint = socket;
    d->m_localControlEndpoint = Endpoint(socket->localAddress(), socket->localPort());

    socket = new QUdpSocket(this);
    QKnxPrivate::clearSocket(&(d->m_dataEndpoint));
    if (!socket->bind(d->m_localControlEndpoint.address, 0)) {
        d->setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Network,
            QKnxNetIpEndpointConnection::tr("Could not bind local data endpoint: %1")
                .arg(socket->errorString()));
        QKnxPrivate::clearSocket(&socket);
        QKnxPrivate::clearSocket(&(d->m_controlEndpoint));
        d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Disconnected);
        return;
    }
    d->m_dataEndpoint = socket;
    d->m_localDataEndpoint = Endpoint(socket->localAddress(), socket->localPort());

    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Bound);

    d->setup();

    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Connecting);

    auto request = QKnxNetIpConnectRequest(d->m_nat ? d->m_natEndpoint : d->m_localControlEndpoint,
        // TODO: Fix this. It cannot work in NAT mode, because the sever does not know the data
        // endpoint address if we pass a NAT endpoint. The most likely solution will be to use a
        // single socket for control and data endpoint...
        d->m_nat ? d->m_natEndpoint : d->m_localDataEndpoint, d->m_cri);
    d->m_controlEndpointVersion = request.header().protocolVersion();

    qDebug() << "Sending connect request:" << request;

    d->m_connectRequestTimer->start(QKnxNetIp::ConnectRequestTimeout);
    d->m_controlEndpoint->writeDatagram(request.bytes(), d->m_remoteControlEndpoint.address,
        d->m_remoteControlEndpoint.port);
}

void QKnxNetIpEndpointConnection::disconnectFromHost()
{
    Q_D(QKnxNetIpEndpointConnection);

    if (d->m_state == State::Disconnecting || d->m_state == State::Disconnected)
        return;

    auto oldState = d->m_state;
    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Disconnecting);

    if (oldState != State::Connected) {
        d->cleanup();
    } else {
        auto request = QKnxNetIpDisconnectRequest(d->m_channelId, {
                (d->m_nat ? d->m_natEndpoint : d->m_localControlEndpoint).operator QKnxNetIpHpai()
            }
        );

        qDebug() << "Sending disconnect request:" << request;
        d->m_controlEndpoint->writeDatagram(request.bytes(), d->m_remoteControlEndpoint.address,
            d->m_remoteControlEndpoint.port);

        d->m_disconnectRequestTimer->start(QKnxNetIp::DisconnectRequestTimeout);
        // Fully disconnected will be handled inside the private cleanup function.
    }
}

QKnxNetIpEndpointConnection::QKnxNetIpEndpointConnection(QKnxNetIpEndpointConnectionPrivate &dd,
        QObject *parent)
    : QObject(dd, parent)
{}

QT_END_NAMESPACE
