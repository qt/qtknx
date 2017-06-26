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

#include "qknxnetipclient.h"
#include "qknxnetipclient_p.h"
#include "qknxnetipconnectrequest.h"
#include "qknxnetipconnectresponse.h"
#include "qknxnetipconnectionstaterequest.h"
#include "qknxnetipconnectionstateresponse.h"
#include "qknxnetipdeviceconfigurationacknowledge.h"
#include "qknxnetipdeviceconfigurationrequest.h"
#include "qknxnetipdisconnectrequest.h"
#include "qknxnetipdisconnectresponse.h"
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

void QKnxNetIpClientPrivate::setupTimer()
{
    QKnxPrivate::clearTimer(&m_heartbeatTimer);
    QKnxPrivate::clearTimer(&m_connectRequestTimer);
    QKnxPrivate::clearTimer(&m_connectionStateTimer);
    QKnxPrivate::clearTimer(&m_acknowledgeTimer);

    Q_Q(QKnxNetIpClient);
    m_heartbeatTimer = new QTimer(q);
    m_heartbeatTimer->setSingleShot(true);
    QObject::connect(m_heartbeatTimer, &QTimer::timeout, [&]() {
        sendStateRequest();
    });

    m_connectRequestTimer = new QTimer(q);
    m_connectRequestTimer->setSingleShot(true);
    QObject::connect(m_connectRequestTimer, &QTimer::timeout, [&]() {
        setAndEmitStateChanged(QKnxNetIpClient::State::Bound);
        setAndEmitErrorOccurred(QKnxNetIpClient::Error::Acknowledge,
            QKnxNetIpClient::tr("Connect request timeout."));

        Q_Q(QKnxNetIpClient);
        q->disconnectFromHost();
    });

    m_connectionStateTimer = new QTimer(q);
    m_connectionStateTimer->setSingleShot(true);
    QObject::connect(m_connectionStateTimer, &QTimer::timeout, [&]() {
        m_heartbeatTimer->stop();
        m_connectionStateTimer->stop();
        if (m_stateRequests > m_maxStateRequests) {
            setAndEmitErrorOccurred(QKnxNetIpClient::Error::Heartbeat,
                QKnxNetIpClient::tr("Connection state request timeout."));

            Q_Q(QKnxNetIpClient);
            q->disconnectFromHost();
        } else {
            sendStateRequest();
        }
    });

    m_acknowledgeTimer = new QTimer(q);
    m_acknowledgeTimer->setSingleShot(true);
    QObject::connect(m_acknowledgeTimer, &QTimer::timeout, [&]() {
        if (m_cemiRequests > m_maxCemiRequest) {
            setAndEmitErrorOccurred(QKnxNetIpClient::Error::Cemi, QKnxNetIpClient::tr("Did not "
                "receive acknowledge in time."));

            Q_Q(QKnxNetIpClient);
            q->disconnectFromHost();
        } else {
            sendCemiRequest();
        }
    });
}

void QKnxNetIpClientPrivate::setupSockets()
{
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
                            processDatagram(QKnxNetIpClient::EndpointType::Data, datagram);
                            break;
                        }
                    // }
                }
        }
    });

    using overload = void (QUdpSocket::*)(QUdpSocket::SocketError);
    QObject::connect(m_dataEndpoint,
        static_cast<overload>(&QUdpSocket::error), [&](QUdpSocket::SocketError) {
            setAndEmitErrorOccurred(QKnxNetIpClient::Error::Network, m_dataEndpoint->errorString());

            Q_Q(QKnxNetIpClient);
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
                            processDatagram(QKnxNetIpClient::EndpointType::Control, datagram);
                            break;
                        }
                    // }
                }
        }
    });

    using overload = void (QUdpSocket::*)(QUdpSocket::SocketError);
    QObject::connect(m_controlEndpoint,
        static_cast<overload>(&QUdpSocket::error), [&](QUdpSocket::SocketError) {
            setAndEmitErrorOccurred(QKnxNetIpClient::Error::Network, m_controlEndpoint->errorString());

            Q_Q(QKnxNetIpClient);
            q->disconnectFromHost();
    });
}

void QKnxNetIpClientPrivate::sendCemiRequest()
{
    m_dataEndpoint->writeDatagram(m_lastCemiRequest, m_remoteDataEndpoint.address,
        m_remoteDataEndpoint.port);

    m_cemiRequests++;
    m_acknowledgeTimer->start(m_acknowledgeTimeout);
}

void QKnxNetIpClientPrivate::sendStateRequest()
{
    qDebug().noquote().nospace() << "Sending connection state request: 0x" << m_lastStateRequest
        .toHex();

    m_controlEndpoint->writeDatagram(m_lastStateRequest, m_remoteControlEndpoint.address,
        m_remoteControlEndpoint.port);

    m_stateRequests++;
    m_connectionStateTimer->start(QKnxNetIp::ConnectionStateRequestTimeout);
}

void QKnxNetIpClientPrivate::process(const QKnxCemiFrame &)
{}

void QKnxNetIpClientPrivate::process(const QKnxNetIpTunnelingRequest &request)
{
    qDebug() << "Received tunneling request:" << request;

    if (request.channelId() == m_channelId) {
        if (bool counterEquals = (request.sequenceCount() == m_receiveCount)
            || (request.sequenceCount() + 1 == m_receiveCount)) {
                // sequence equals -> acknowledge -> process frame
                // sequence -1 -> acknowledge -> drop frame
                auto ack = QKnxNetIpTunnelingAcknowledge(m_channelId, request.channelId(),
                    QKnxNetIp::Error::None);

                qDebug() << "Sending tunneling acknowledge:" << ack;
                m_dataEndpoint->writeDatagram(ack.bytes<QByteArray>(), m_remoteDataEndpoint
                    .address, m_remoteDataEndpoint.port);

                if (!counterEquals)
                    return;
                m_receiveCount++;
                process(request.cemi());
        }
    } else {
        qDebug() << "Request was ignored due to wrong channel ID. Expected:" << m_channelId <<
            "Current:" << request.channelId();
    }
}

void QKnxNetIpClientPrivate::process(const QKnxNetIpTunnelingAcknowledge &acknowledge)
{
    qDebug() << "Received tunneling acknowledge:" << acknowledge;

    if (acknowledge.channelId() == m_channelId) {
        m_acknowledgeTimer->stop();
        if (acknowledge.status() == QKnxNetIp::Error::None
            && acknowledge.sequenceCount() == m_sendCount) {
                m_sendCount++;
                m_cemiRequests = 0;
        } else {
            sendCemiRequest();
        }
    } else {
        qDebug() << "Acknowledge was ignored due to wrong channel ID. Expected:" << m_channelId <<
            "Current:" << acknowledge.channelId();
    }
}

void QKnxNetIpClientPrivate::sendTunnelingRequest(const QKnxCemiFrame &frame)
{
    m_lastCemiRequest = QKnxNetIpTunnelingRequest(m_channelId, m_sendCount, frame)
        .bytes<QByteArray>();
    qDebug().noquote().nospace() << "Sending tunneling request: 0x" << m_lastCemiRequest.toHex();

    sendCemiRequest();
}

void QKnxNetIpClientPrivate::process(const QKnxNetIpDeviceConfigurationRequest &request)
{
    qDebug() << "Received device configuration request:" << request;

    if (request.channelId() == m_channelId) {
        if (request.sequenceCount() == m_receiveCount) {
                auto ack = QKnxNetIpDeviceConfigurationAcknowledge(m_channelId, request.channelId(),
                    QKnxNetIp::Error::None);

                qDebug() << "Sending device configuration acknowledge:" << ack;
                m_dataEndpoint->writeDatagram(ack.bytes<QByteArray>(), m_remoteDataEndpoint
                    .address, m_remoteDataEndpoint.port);

                m_receiveCount++;
                process(request.cemi());
        }
    } else {
        qDebug() << "Request was ignored due to wrong channel ID. Expected:" << m_channelId <<
            "Current:" << request.channelId();
    }
}

void QKnxNetIpClientPrivate::process(const QKnxNetIpDeviceConfigurationAcknowledge &acknowledge)
{
    qDebug() << "Received device configuration acknowledge:" << acknowledge;

    if (acknowledge.channelId() == m_channelId) {
        m_acknowledgeTimer->stop();
        if (acknowledge.status() == QKnxNetIp::Error::None
            && acknowledge.sequenceCount() == m_sendCount) {
                m_sendCount++;
                m_cemiRequests = 0;
        } else {
            sendCemiRequest();
        }
    } else {
        qDebug() << "Acknowledge was ignored due to wrong channel ID. Expected:" << m_channelId <<
            "Current:" << acknowledge.channelId();
    }
}

void QKnxNetIpClientPrivate::sendDeviceConfigurationRequest(const QKnxCemiFrame &frame)
{
    m_lastCemiRequest = QKnxNetIpDeviceConfigurationRequest(m_channelId, m_sendCount, frame)
        .bytes<QByteArray>();
    qDebug().noquote().nospace() << "Sending device configuration request: 0x" << m_lastCemiRequest
        .toHex();
    sendCemiRequest();
}

namespace QKnxPrivate
{
    static bool isNullOrLocal(const QHostAddress &address)
    {
        return address.isNull() || address == QHostAddress::Any || address.isLoopback();
    }
}

void QKnxNetIpClientPrivate::process(const QKnxNetIpConnectResponse &response,
    const QNetworkDatagram &datagram)
{
    qDebug() << "Received connect response:" << response;

    if (m_state == QKnxNetIpClient::State::Connecting) {
        if (response.status() == QKnxNetIp::Error::None) {
            m_connectRequestTimer->stop();
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
                m_nat ? m_natEndpoint : m_localControlEndpoint).bytes<QByteArray>();

            QTimer::singleShot(0, [&]() { sendStateRequest(); });
            setAndEmitStateChanged(QKnxNetIpClient::State::Connected);
        } else {
            setAndEmitErrorOccurred(QKnxNetIpClient::Error::Acknowledge,
                QKnxNetIpClient::tr("Could not connect to remote control endpoint. Error code: "
                    "0x%1").arg(quint8(response.status()), 2, 16, QLatin1Char('0')));

            Q_Q(QKnxNetIpClient);
            q->disconnectFromHost();
        }
    } else {
        qDebug() << "Response was ignored due to current state. Expected:"
            << QKnxNetIpClient::State::Connecting << "Current:" << m_state;
    }
}

void QKnxNetIpClientPrivate::process(const QKnxNetIpConnectionStateResponse &response)
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
        qDebug() << "Response was ignored due to wrong channel ID. Expected:" << m_channelId <<
            "Current:" << response.channelId();
    }
}

void QKnxNetIpClientPrivate::process(const QKnxNetIpDisconnectRequest &request)
{
    qDebug() << "Received disconnect request:" << request;

    if (request.channelId() == m_channelId) {
        auto response = QKnxNetIpDisconnectResponse(m_channelId, QKnxNetIp::Error::None);
        qDebug() << "Sending disconnect response:" << response;
        m_controlEndpoint->writeDatagram(response.bytes<QByteArray>(),
            m_remoteControlEndpoint.address, m_remoteControlEndpoint.port);

        Q_Q(QKnxNetIpClient);
        q->disconnectFromHost();
    } else {
        qDebug() << "Response was ignored due to wrong channel ID. Expected:" << m_channelId <<
            "Current:" << request.channelId();
    }
}

void QKnxNetIpClientPrivate::process(const QKnxNetIpDisconnectResponse &response)
{
    qDebug() << "Received disconnect response:" << response;

    if (response.channelId() == m_channelId) {
        m_dataEndpoint->close();
        m_controlEndpoint->close();

        QKnxPrivate::clearSocket(&m_dataEndpoint);
        QKnxPrivate::clearSocket(&m_controlEndpoint);
        QKnxPrivate::clearTimer(&m_heartbeatTimer);
        QKnxPrivate::clearTimer(&m_connectRequestTimer);
        QKnxPrivate::clearTimer(&m_connectionStateTimer);
        QKnxPrivate::clearTimer(&m_acknowledgeTimer);

        setAndEmitStateChanged(QKnxNetIpClient::State::Disconnected);
    } else {
        qDebug() << "Response was ignored due to wrong channel ID. Expected:" << m_channelId <<
            "Current:" << response.channelId();
    }
}

void QKnxNetIpClientPrivate::processDatagram(QKnxNetIpClient::EndpointType, const QNetworkDatagram &)
{}

void QKnxNetIpClientPrivate::setAndEmitStateChanged(QKnxNetIpClient::State newState)
{
    m_state = newState;

    Q_Q(QKnxNetIpClient);
    emit q->stateChanged(newState);

    if (m_state == QKnxNetIpClient::State::Connected)
        emit q->connected();
    else if (m_state == QKnxNetIpClient::State::Disconnected)
        emit q->disconnected();
}

void QKnxNetIpClientPrivate::setAndEmitErrorOccurred(QKnxNetIpClient::Error newError,
    const QString &message)
{
    m_error = newError;
    m_errorString = message;

    Q_Q(QKnxNetIpClient);
    emit q->errorOccurred(m_error, m_errorString);
}


// -- QKnxNetIpClient

QKnxNetIpClient::~QKnxNetIpClient()
{
    disconnectFromHost();
}

QKnxNetIpClient::State QKnxNetIpClient::state() const
{
    Q_D(const QKnxNetIpClient);
    return d->m_state;
}

QString QKnxNetIpClient::errorString() const
{
    Q_D(const QKnxNetIpClient);
    return d->m_errorString;
}

QKnxNetIpClient::Error QKnxNetIpClient::error() const
{
    Q_D(const QKnxNetIpClient);
    return d->m_error;
}

int QKnxNetIpClient::sequenceCount(SequenceType type) const
{
    Q_D(const QKnxNetIpClient);
    return (type == SequenceType::Send ?  d->m_sendCount : d->m_receiveCount);
}

quint8 QKnxNetIpClient::netIpHeaderVersion(EndpointType endpoint) const
{
    Q_D(const QKnxNetIpClient);
    return (endpoint ==  EndpointType::Data ? d->m_dataEndpointVersion : d->m_controlEndpointVersion);
}

quint16 QKnxNetIpClient::localPort() const
{
    Q_D(const QKnxNetIpClient);
    return d->m_localControlEndpoint.port;
}

void QKnxNetIpClient::setLocalPort(quint16 port)
{
    Q_D(QKnxNetIpClient);
    if (d->m_state == QKnxNetIpClient::Disconnected)
        d->m_localControlEndpoint.port = port;
}

QHostAddress QKnxNetIpClient::localAddress() const
{
    Q_D(const QKnxNetIpClient);
    return d->m_localControlEndpoint.address;
}

void QKnxNetIpClient::setLocalAddress(const QHostAddress &address)
{
    Q_D(QKnxNetIpClient);
    if (d->m_state == QKnxNetIpClient::Disconnected)
        d->m_localControlEndpoint.address = address;
}

bool QKnxNetIpClient::natAware() const
{
    Q_D(const QKnxNetIpClient);
    return d->m_nat;
}

void QKnxNetIpClient::setNatAware(bool isAware)
{
    Q_D(QKnxNetIpClient);
    if (d->m_state == QKnxNetIpClient::Disconnected)
        d->m_nat = isAware;
}

quint32 QKnxNetIpClient::heartbeatTimeout() const
{
    Q_D(const QKnxNetIpClient);
    return d->m_heartbeatTimeout;
}

void QKnxNetIpClient::setHeartbeatTimeout(quint32 msec)
{
    if (msec > QKnxNetIp::ConnectionAliveTimeout)
        return;

    Q_D(QKnxNetIpClient);
    d->m_heartbeatTimeout = msec;
    if (d->m_heartbeatTimer)
        d->m_heartbeatTimer->setInterval(msec);
}

QVector<quint8> QKnxNetIpClient::supportedProtocolVersions() const
{
    Q_D(const QKnxNetIpClient);
    return d->m_supportedVersions;
}

void QKnxNetIpClient::setSupportedProtocolVersions(const QVector<quint8> &versions)
{
    Q_D(QKnxNetIpClient);
    if (d->m_state == QKnxNetIpClient::Disconnected)
        d->m_supportedVersions = versions;
}

void QKnxNetIpClient::connectToHost(const QKnxNetIpHpai &controlEndpoint)
{
    connectToHost(controlEndpoint.address(), controlEndpoint.port());
}

void QKnxNetIpClient::connectToHost(const QHostAddress &address, quint16 port)
{
    Q_D(QKnxNetIpClient);

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
    d->m_localControlEndpoint.address.toIPv4Address(&isIPv4);
    if (!isIPv4) {
        d->setAndEmitErrorOccurred(Error::NotIPv4, tr("Only IPv4 addresses as local control "
            "endpoint supported."));
        return;
    }

    d->setAndEmitStateChanged(QKnxNetIpClient::State::Starting);

    auto socket = new QUdpSocket(this);
    QKnxPrivate::clearSocket(&(d->m_controlEndpoint));
    if (!socket->bind(d->m_localControlEndpoint.address, d->m_localControlEndpoint.port)) {
        d->setAndEmitErrorOccurred(QKnxNetIpClient::Error::Network, QKnxNetIpClient::tr("Could "
            "not bind local control endpoint: %1").arg(socket->errorString()));
        QKnxPrivate::clearSocket(&socket);
        d->setAndEmitStateChanged(QKnxNetIpClient::State::Disconnected);
        return;
    }
    d->m_controlEndpoint = socket;
    d->m_localControlEndpoint = Endpoint(socket->localAddress(), socket->localPort());

    socket = new QUdpSocket(this);
    QKnxPrivate::clearSocket(&(d->m_dataEndpoint));
    if (!socket->bind(d->m_localControlEndpoint.address, 0)) {
        d->setAndEmitErrorOccurred(QKnxNetIpClient::Error::Network, QKnxNetIpClient::tr("Could "
            "not bind local data endpoint: %1").arg(socket->errorString()));
        QKnxPrivate::clearSocket(&socket);
        QKnxPrivate::clearSocket(&(d->m_controlEndpoint));
        d->setAndEmitStateChanged(QKnxNetIpClient::State::Disconnected);
        return;
    }
    d->m_dataEndpoint = socket;
    d->m_localDataEndpoint = Endpoint(socket->localAddress(), socket->localPort());

    d->setAndEmitStateChanged(QKnxNetIpClient::State::Bound);

    d->setupTimer();
    d->setupSockets();

    d->setAndEmitStateChanged(QKnxNetIpClient::State::Connecting);

    auto request = QKnxNetIpConnectRequest(d->m_nat ? d->m_natEndpoint : d->m_localControlEndpoint,
        d->m_nat ? d->m_natEndpoint : d->m_localDataEndpoint, d->m_cri);
    d->m_controlEndpointVersion = request.header().protocolVersion();

    qDebug() << "Sending connect request:" << request;
    d->m_controlEndpoint->writeDatagram(request.bytes<QByteArray>(),
        d->m_remoteControlEndpoint.address, d->m_remoteControlEndpoint.port);

    d->m_connectRequestTimer->start(QKnxNetIp::ConnectRequestTimeout);
}

void QKnxNetIpClient::disconnectFromHost()
{
    Q_D(QKnxNetIpClient);

    if (d->m_state == State::Disconnecting || d->m_state == State::Disconnected)
        return;

    auto oldState = d->m_state;
    d->setAndEmitStateChanged(QKnxNetIpClient::State::Disconnecting);

    if (oldState != State::Connected) {
        d->m_dataEndpoint->close();
        d->m_controlEndpoint->close();

        QKnxPrivate::clearSocket(&(d->m_dataEndpoint));
        QKnxPrivate::clearSocket(&(d->m_controlEndpoint));

        QKnxPrivate::clearTimer(&(d->m_heartbeatTimer));
        QKnxPrivate::clearTimer(&(d->m_connectRequestTimer));
        QKnxPrivate::clearTimer(&(d->m_connectionStateTimer));
        QKnxPrivate::clearTimer(&(d->m_acknowledgeTimer));

        d->setAndEmitStateChanged(QKnxNetIpClient::State::Disconnected);
    } else {
        auto request = QKnxNetIpDisconnectRequest(d->m_channelId, {
                (d->m_nat ? d->m_natEndpoint : d->m_localControlEndpoint)
            });

        qDebug() << "Sending disconnect request:" << request;
        d->m_controlEndpoint->writeDatagram(request.bytes<QByteArray>(), d->m_remoteControlEndpoint
            .address, d->m_remoteControlEndpoint.port);

        QTimer::singleShot(QKnxNetIp::DisconnectRequestTimeout, this, [&] () {
             Q_D(QKnxNetIpClient);
             d->process(QKnxNetIpDisconnectResponse(d->m_channelId, QKnxNetIp::Error::None));
        });
        // Fully disconnected will be handled inside the process disconnect response function.
    }
}

QKnxNetIpClient::QKnxNetIpClient(QKnxNetIpClientPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{}

QT_END_NAMESPACE
