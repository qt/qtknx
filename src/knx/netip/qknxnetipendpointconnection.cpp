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
#include "qtcpsocket.h"
#include "qudpsocket.h"

QT_BEGIN_NAMESPACE
/*!
    \class QKnxNetIpEndpointConnection

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpEndpointConnection class serves as base class for derived classes to enable
    the opening and handling of a client connection to a KNXnet/IP server.

    The QKnxNetIpEndpointConnection establishes a communication channel between a
    client and a KNXnet/IP server endpoint. This is used by the client side
    to monitor the state of the communication channel. The IP address of the client must be set
    together with a local port. It is then possible to connect to a chosen KNXnet/IP server on a
    given port. To detect the failure of the channel, the class provides a heartbeat monitor timeout
    that can be modified according to the client needs. If the KNXnet/IP communication
    has to traverse across a network using network address translation (NAT), the client can also make
    the class aware of this.

    \sa {Qt KNXnet/IP Connection Classes}
*/

/*!
    \enum QKnxNetIpEndpointConnection::State

    This enum holds the state of the KNXnet/IP connection.

    \value Disconnected
            Error found establishing the connection, or disconnect response received.
    \value Starting
            Preparing internal parameters for establishing the connection.
    \value Bound
            Internal UDP socket is ready for receiving packets, but the connection is not yet established.
    \value Connecting
            Connection request sent but no answer from the server.
    \value Connected
            Connection response received and connection is established.
    \value Disconnecting
            Disconnect request sent.
*/

/*!
    \enum QKnxNetIpEndpointConnection::Error

    This enum holds the errors that can occur establishing a KNXnet/IP connection.

    \value None
            No errors so far.
    \value State
    \value Network
            Detected errors in the internal UDP socket connection.
    \value NotIPv4
            Host or remote IPs not version 4.
    \value Acknowledge
            No response to connect/disconnect request.
    \value Heartbeat
            State request timeout.
    \value Cemi
            No cEMI frame acknowledge in time.
    \value Unknown
*/
/*!
    \enum QKnxNetIpEndpointConnection::SequenceType

    This enum holds the sequence type.

    \value Send
    \value Receive
*/
/*!
    \enum QKnxNetIpEndpointConnection::EndpointType

    This enum holds the endpoint type.

    \value Data
    \value Control
*/
/*!
   \fn void QKnxNetIpEndpointConnection::connected()

    This signal is emitted after the client connects to a host and the KNXnet/IP connection
    is established.
*/
/*!
   \fn void QKnxNetIpEndpointConnection::disconnected()

    This signal is emitted when the KNXnet/IP connection is lost or client closes
    the connection.
*/
/*!
   \fn void QKnxNetIpEndpointConnection::stateChanged(QKnxNetIpEndpointConnection::State state)

    This signal is emitted when the KNXnet/IP connection state \a state changed.
*/
/*!
   \fn void QKnxNetIpEndpointConnection::errorOccurred(QKnxNetIpEndpointConnection::Error error, QString errorString)

    This signal is emitted when there is an \a error in the KNXnet/IP connection. The
    \a errorString describes the error that occurred.
*/
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

    template <typename T>
    static void clearSocket(T **socket)
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
        processDisconnectResponse(QKnxNetIpDisconnectResponseProxy::builder()
            .setChannelId(m_channelId).setStatus(QKnxNetIp::Error::None).create());
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

namespace QKnxPrivate
{
    static bool isNullOrLocal(const QHostAddress &address)
    {
        return address.isNull() || address == QHostAddress::Any || address.isLoopback();
    }
}

void QKnxNetIpEndpointConnectionPrivate::processReceivedFrame(const QHostAddress &address, int port)
{
    const auto frame = QKnxNetIpFrame::fromBytes(m_rxBuffer);
    if (!frame.isValid())
        return;

    // remove already processed KNX frame from buffer
    m_rxBuffer.remove(0, frame.size());

    // TODO: fix the version and validity checks
    // if (!m_supportedVersions.contains(header.protocolVersion())) {
    //     send E_VERSION_NOT_SUPPORTED confirmation frame
    //     send disconnect request
    // } else if (header.protocolVersion() != m_controlEndpointVersion) {
    //     send disconnect request
    // } else {
    // TODO: set the m_dataEndpointVersion once we receive or send the first frame

    switch (frame.serviceType()) {
    case QKnxNetIp::ServiceType::ConnectResponse:
        if (m_nat) { // TODO: Review this for TCP connections
            if (QKnxPrivate::isNullOrLocal(m_remoteDataEndpoint.address)
                || m_remoteDataEndpoint.port == 0) {
                    m_remoteDataEndpoint.port = port;
                    m_remoteDataEndpoint.address = address;
            }
        }
        processConnectResponse(frame);
        break;
    case QKnxNetIp::ServiceType::ConnectionStateResponse:
        processConnectionStateResponse(frame);
        break;
    case QKnxNetIp::ServiceType::DisconnectRequest:
        processDisconnectRequest(frame);
        break;
    case QKnxNetIp::ServiceType::DisconnectResponse:
        processDisconnectResponse(frame);
        break;

    case QKnxNetIp::ServiceType::TunnelingRequest:
        processTunnelingRequest(frame);
        break;
    case QKnxNetIp::ServiceType::TunnelingAcknowledge:
        processTunnelingAcknowledge(frame);
        break;
    case QKnxNetIp::ServiceType::DeviceConfigurationRequest:
        processDeviceConfigurationRequest(frame);
        break;
    case QKnxNetIp::ServiceType::DeviceConfigurationAcknowledge:
        processDeviceConfigurationAcknowledge(frame);
        break;

    default:
        break;
    }
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

    if (m_tcpSocket) {
        QObject::connect(m_tcpSocket, &QIODevice::readyRead, [&]() {
            m_rxBuffer += QKnxByteArray::fromByteArray(m_tcpSocket->readAll());
            int bufferSize = m_rxBuffer.size();
            bool continueProcessingRxBuffer = false;
            do {
                // TODO: AN184 v03 KNXnet-IP Core v2 AS, 2.2.3.2.3.2
                processReceivedFrame(m_remoteControlEndpoint.address,
                                m_remoteControlEndpoint.port);
                continueProcessingRxBuffer = (m_rxBuffer.size() < bufferSize);
                bufferSize = m_rxBuffer.size();
            } while (continueProcessingRxBuffer);
        });

        using overload = void (QTcpSocket::*)(QTcpSocket::SocketError);
        QObject::connect(m_tcpSocket,
            static_cast<overload>(&QTcpSocket::error), [&](QTcpSocket::SocketError) {
                setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Network,
                    m_tcpSocket->errorString());

                Q_Q(QKnxNetIpEndpointConnection);
                q->disconnectFromHost();
        });
    } else if (m_udpSocket) {
        QObject::connect(m_udpSocket, &QUdpSocket::readyRead, [&]() {
            while (m_udpSocket && m_udpSocket->state() == QUdpSocket::BoundState
                && m_udpSocket->hasPendingDatagrams()) {
                auto datagram = m_udpSocket->receiveDatagram();
                m_rxBuffer += QKnxByteArray::fromByteArray(datagram.data());
                processReceivedFrame(datagram.senderAddress(), datagram.senderPort());
            }
        });

        using overload = void (QUdpSocket::*)(QUdpSocket::SocketError);
        QObject::connect(m_udpSocket,
            static_cast<overload>(&QUdpSocket::error), [&](QUdpSocket::SocketError) {
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Network,
                m_udpSocket->errorString());

            Q_Q(QKnxNetIpEndpointConnection);
            q->disconnectFromHost();
        });
    }
}

void QKnxNetIpEndpointConnectionPrivate::cleanup()
{
    QKnxPrivate::clearTimer(&m_heartbeatTimer);
    QKnxPrivate::clearTimer(&m_connectRequestTimer);
    QKnxPrivate::clearTimer(&m_connectionStateTimer);
    QKnxPrivate::clearTimer(&m_disconnectRequestTimer);
    QKnxPrivate::clearTimer(&m_acknowledgeTimer);

    if (m_udpSocket) {
        m_udpSocket->close();
        QKnxPrivate::clearSocket(&m_udpSocket);
    } else {
        m_tcpSocket->close();
        QKnxPrivate::clearSocket(&m_tcpSocket);
    }

    setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Disconnected);
}

bool QKnxNetIpEndpointConnectionPrivate::sendCemiRequest()
{
    if (!m_waitForAcknowledgement && !m_tcpSocket) {
        m_waitForAcknowledgement = true;
        m_udpSocket->writeDatagram(m_lastSendCemiRequest.bytes().toByteArray(),
                                m_remoteDataEndpoint.address,
                                m_remoteDataEndpoint.port);

        m_cemiRequests++;
        m_acknowledgeTimer->start(m_acknowledgeTimeout);
    } else if (m_tcpSocket) {
        m_tcpSocket->write(m_lastSendCemiRequest.bytes().toByteArray());
        m_waitForAcknowledgement = false;
    }
    return !m_waitForAcknowledgement;
}

void QKnxNetIpEndpointConnectionPrivate::sendStateRequest()
{
    qDebug().noquote().nospace() << "Sending connection state request: 0x" << m_lastStateRequest
        .bytes().toHex();

    if (m_tcpSocket) {
        m_tcpSocket->write(m_lastStateRequest.bytes().toByteArray());
    } else {
        m_udpSocket->writeDatagram(m_lastStateRequest.bytes().toByteArray(),
            m_remoteControlEndpoint.address, m_remoteControlEndpoint.port);
    }

    m_stateRequests++;
    m_connectionStateTimer->start(QKnxNetIp::ConnectionStateRequestTimeout);
}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxLinkLayerFrame &)
{}

void QKnxNetIpEndpointConnectionPrivate::process(const QKnxDeviceManagementFrame &)
{}

void QKnxNetIpEndpointConnectionPrivate::processTunnelingRequest(const QKnxNetIpFrame &frame)
{
    qDebug() << "Received tunneling request:" << frame;

    QKnxNetIpTunnelingRequestProxy request(frame);
    if (m_tcpSocket) {
        process(request.cemi());
        return; // no need to send ACK in TCP connection
    }

    if (frame.channelId() == m_channelId) {
        const bool counterEquals = (frame.sequenceNumber() == m_receiveCount);
        if (counterEquals || (frame.sequenceNumber() + 1 == m_receiveCount)) {
                // sequence equals -> acknowledge -> process frame
                // sequence -1 -> acknowledge -> drop frame
                auto ack = QKnxNetIpTunnelingAcknowledgeProxy::builder()
                    .setChannelId(m_channelId)
                    .setSequenceNumber(m_receiveCount)
                    .setStatus(QKnxNetIp::Error::None)
                    .create();

                qDebug() << "Sending tunneling acknowledge:" << ack;
                m_udpSocket->writeDatagram(ack.bytes().toByteArray(),
                    m_remoteDataEndpoint.address, m_remoteDataEndpoint.port);

                if (!counterEquals)
                    return;
                m_receiveCount++;
                process(request.cemi());
        }
    } else {
        qDebug() << "Request was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << frame.channelId();
    }
}

void QKnxNetIpEndpointConnectionPrivate::processTunnelingAcknowledge(const QKnxNetIpFrame &frame)
{
    qDebug() << "Received tunneling acknowledge:" << frame;

    if (frame.channelId() == m_channelId) {
        m_acknowledgeTimer->stop();
        m_waitForAcknowledgement = false;

        const QKnxNetIpTunnelingAcknowledgeProxy acknowledge(frame);
        if (acknowledge.status() == QKnxNetIp::Error::None
            && acknowledge.sequenceNumber() == m_sendCount) {
                m_sendCount++;
                m_cemiRequests = 0;
        } else {
            sendCemiRequest();
        }
    } else {
        qDebug() << "Acknowledge was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << frame.channelId();
    }
}

bool QKnxNetIpEndpointConnectionPrivate::sendTunnelingRequest(const QKnxLinkLayerFrame &frame)
{
    m_lastSendCemiRequest = QKnxNetIpTunnelingRequestProxy::builder()
        .setChannelId(m_channelId)
        .setSequenceNumber(m_sendCount)
        .setCemi(frame)
        .create();
    qDebug().noquote().nospace() << "Sending tunneling request:" << m_lastSendCemiRequest;

    return sendCemiRequest();
}

void QKnxNetIpEndpointConnectionPrivate::processDeviceConfigurationRequest(const QKnxNetIpFrame &frame)
{
    qDebug() << "Received device configuration request:" << frame;

    QKnxNetIpDeviceConfigurationRequestProxy request(frame);
    if (m_tcpSocket && request.isValid()) {
        process(request.cemi());
        return; // no need to send ACK in TCP connection
    }

    if (frame.channelId() != m_channelId) {
        qDebug() << "Request was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << frame.channelId();
        return;
    }

    if (request.sequenceNumber() != m_receiveCount) {
        qDebug() << "Request was ignored due to wrong sequence number. Expected:" << m_receiveCount
            << "Current:" << request.sequenceNumber();
        return;
    }

    auto ack = QKnxNetIpDeviceConfigurationAcknowledgeProxy::builder()
        .setChannelId(m_channelId)
        .setSequenceNumber(m_receiveCount)
        .setStatus(QKnxNetIp::Error::None)
        .create();

    qDebug() << "Sending device configuration acknowledge:" << ack;
    m_udpSocket->writeDatagram(ack.bytes().toByteArray(),
        m_remoteDataEndpoint.address, m_remoteDataEndpoint.port);

    m_receiveCount++;
    if (m_waitForAcknowledgement)
        m_lastReceivedCemiRequest = frame;
    else
        process(request.cemi());
}

void QKnxNetIpEndpointConnectionPrivate::processDeviceConfigurationAcknowledge(const QKnxNetIpFrame &frame)
{
    qDebug() << "Received device configuration acknowledge:" << frame;

    if (frame.channelId() == m_channelId) {
        m_acknowledgeTimer->stop();
        m_waitForAcknowledgement = false;

        const QKnxNetIpDeviceConfigurationAcknowledgeProxy ack(frame);
        if (ack.status() == QKnxNetIp::Error::None && ack.sequenceNumber() == m_sendCount) {
            m_sendCount++;
            m_cemiRequests = 0;
            if (!m_lastReceivedCemiRequest.isNull()) {
                process(QKnxNetIpDeviceConfigurationRequestProxy(m_lastReceivedCemiRequest).cemi());
                m_lastReceivedCemiRequest = {};
            }
        } else {
            sendCemiRequest();
        }
    } else {
        qDebug() << "Acknowledge was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << frame.channelId();
    }
}

bool QKnxNetIpEndpointConnectionPrivate::sendDeviceConfigurationRequest(const QKnxDeviceManagementFrame &frame)
{
    m_lastSendCemiRequest = QKnxNetIpDeviceConfigurationRequestProxy::builder()
        .setChannelId(m_channelId)
        .setSequenceNumber(m_sendCount)
        .setCemi(frame)
        .create();
    qDebug().noquote().nospace() << "Sending device configuration request:" << m_lastSendCemiRequest;
    return sendCemiRequest();
}

void QKnxNetIpEndpointConnectionPrivate::processConnectResponse(const QKnxNetIpFrame &frame)
{
    qDebug() << "Received connect response:" << frame;

    QKnxNetIpConnectResponseProxy response(frame);
    if (m_state == QKnxNetIpEndpointConnection::State::Connecting) {
        if (response.status() == QKnxNetIp::Error::None) {
            QKnxPrivate::clearTimer(&m_connectRequestTimer);

            m_channelId = response.channelId();
            m_remoteDataEndpoint = response.dataEndpoint();
            m_lastStateRequest = QKnxNetIpConnectionStateRequestProxy::builder()
                .setChannelId(m_channelId)
                .setControlEndpoint(m_nat ? m_natEndpoint : m_localEndpoint)
                .create();

            QTimer::singleShot(0, [&]() { sendStateRequest(); });
            setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Connected);
        } else {
            auto metaEnum = QMetaEnum::fromType<QKnxNetIp::Error>();
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Acknowledge,
                QKnxNetIpEndpointConnection::tr("Could not connect to remote control endpoint. "
                    "Error code: 0x%1 (%2)").arg(quint8(response.status()), 2, 16, QLatin1Char('0'))
                    .arg(QString::fromLatin1(metaEnum.valueToKey(int(response.status())))));

            Q_Q(QKnxNetIpEndpointConnection);
            q->disconnectFromHost();
        }
    } else {
        qDebug() << "Response was ignored due to current state. Expected:"
            << QKnxNetIpEndpointConnection::State::Connecting << "Current:" << m_state;
    }
}

void QKnxNetIpEndpointConnectionPrivate::processConnectionStateResponse(const QKnxNetIpFrame &frame)
{
    qDebug() << "Received connection state response:" << frame;

    QKnxNetIpConnectionStateResponseProxy response(frame);
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

void QKnxNetIpEndpointConnectionPrivate::processDisconnectRequest(const QKnxNetIpFrame &frame)
{
    qDebug() << "Received disconnect request:" << frame;

    QKnxNetIpDisconnectRequestProxy request(frame);
    if (request.channelId() == m_channelId) {
        auto frame = QKnxNetIpDisconnectResponseProxy::builder()
            .setChannelId(m_channelId)
            .setStatus(QKnxNetIp::Error::None)
            .create();
        qDebug() << "Sending disconnect response:" << frame;
        if (m_tcpSocket) {
            m_tcpSocket->write(frame.bytes().toByteArray());
        } else {
            m_udpSocket->writeDatagram(frame.bytes().toByteArray(),
                                    m_remoteControlEndpoint.address, m_remoteControlEndpoint.port);
        }

        Q_Q(QKnxNetIpEndpointConnection);
        q->disconnectFromHost();
    } else {
        qDebug() << "Response was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << request.channelId();
    }
}

void QKnxNetIpEndpointConnectionPrivate::processDisconnectResponse(const QKnxNetIpFrame &frame)
{
    qDebug() << "Received disconnect response:" << frame;

    QKnxNetIpDisconnectResponseProxy response(frame);
    if (response.channelId() == m_channelId) {
        cleanup();
    } else {
        qDebug() << "Response was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << response.channelId();
    }
}

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
/*!
    Destroys a connection and disconnects from the server.
*/
QKnxNetIpEndpointConnection::~QKnxNetIpEndpointConnection()
{
    disconnectFromHost();
}

/*!
    Returns the state of the connection.
*/
QKnxNetIpEndpointConnection::State QKnxNetIpEndpointConnection::state() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_state;
}

/*!
    Returns an error string describing the last error detected in the connection.
*/
QString QKnxNetIpEndpointConnection::errorString() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_errorString;
}

/*!
    Returns the last error code detected in the connection.
*/
QKnxNetIpEndpointConnection::Error QKnxNetIpEndpointConnection::error() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_error;
}

/*!
    Returns the received or sent sequence count based on \a type.
*/
int QKnxNetIpEndpointConnection::sequenceCount(SequenceType type) const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return (type == SequenceType::Send ?  d->m_sendCount : d->m_receiveCount);
}

/*!
    Returns the KNXnet/IP header version of the data or control connection depending on \a endpoint.
*/
quint8 QKnxNetIpEndpointConnection::netIpHeaderVersion(EndpointType endpoint) const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return (endpoint == EndpointType::Data ? d->m_dataEndpointVersion : d->m_controlEndpointVersion);
}

/*!
    Returns the local port associated with the connection.
*/
quint16 QKnxNetIpEndpointConnection::localPort() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_localEndpoint.port;
}

/*!
    Sets the local port \a port associated with the connection.
*/
void QKnxNetIpEndpointConnection::setLocalPort(quint16 port)
{
    Q_D(QKnxNetIpEndpointConnection);
    d->m_user.port = port;
}

/*!
    Returns the local address associated with the connection.
*/
QHostAddress QKnxNetIpEndpointConnection::localAddress() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    if (d->m_state == QKnxNetIpEndpointConnection::Disconnected)
        return d->m_user.address;
    return d->m_localEndpoint.address;
}

/*!
    Sets the local address \a address associated with the connection.
*/
void QKnxNetIpEndpointConnection::setLocalAddress(const QHostAddress &address)
{
    Q_D(QKnxNetIpEndpointConnection);
    d->m_user.address = address;
}

/*!
    Returns \c true if the connection uses NAT; \c false otherwise.
*/
bool QKnxNetIpEndpointConnection::natAware() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    if (d->m_state == QKnxNetIpEndpointConnection::Disconnected)
        return d->m_user.natAware;
    return d->m_nat;
}

/*!
    Sets the connection to use NAT based on the \a isAware parameter.
*/
void QKnxNetIpEndpointConnection::setNatAware(bool isAware)
{
    Q_D(QKnxNetIpEndpointConnection);
    d->m_user.natAware = isAware;
}

/*!
    Returns the value of the heartbeat timeout.
*/
quint32 QKnxNetIpEndpointConnection::heartbeatTimeout() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_heartbeatTimeout;
}

/*!
    Sets the value of the heartbeat timeout. Every \a msec a state request is sent over the
    connection to keep it alive.
*/
void QKnxNetIpEndpointConnection::setHeartbeatTimeout(quint32 msec)
{
    if (msec > QKnxNetIp::ConnectionAliveTimeout)
        return;

    Q_D(QKnxNetIpEndpointConnection);
    d->m_heartbeatTimeout = msec;
    if (d->m_heartbeatTimer)
        d->m_heartbeatTimer->setInterval(msec);
}

/*!
    Returns a byte array with the supported KNXnet/IP versions.
*/
QKnxByteArray QKnxNetIpEndpointConnection::supportedProtocolVersions() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    if (d->m_state == QKnxNetIpEndpointConnection::Disconnected)
        return d->m_user.supportedVersions;
    return d->m_supportedVersions;
}

/*!
    Sets the connection supported KNXnet/IP versions indicated in \a versions.
*/
void QKnxNetIpEndpointConnection::setSupportedProtocolVersions(const QKnxByteArray &versions)
{
    Q_D(QKnxNetIpEndpointConnection);
    d->m_user.supportedVersions = versions;
}

/*!
    Establishes a connection to the KNXnet/IP control endpoint \a controlEndpoint.
*/
void QKnxNetIpEndpointConnection::connectToHost(const QKnxNetIpHpai &controlEndpoint)
{
    const QKnxNetIpHpaiProxy proxy(controlEndpoint);
    connectToHost(proxy.hostAddress(), proxy.port(), proxy.hostProtocol());
}

/*!
    Establishes a connection to the host with \a address and \a port.
*/
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

    QKnxPrivate::clearSocket(&(d->m_udpSocket));

    d->m_udpSocket = new QUdpSocket(this);
    if (!d->m_udpSocket->bind(d->m_user.address, d->m_user.port)) {
        d->setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Network,
            QKnxNetIpEndpointConnection::tr("Could not bind endpoint: %1")
                .arg(d->m_udpSocket->errorString()));
        QKnxPrivate::clearSocket(&d->m_udpSocket);
        d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Disconnected);
        return;
    }
    d->m_localEndpoint = Endpoint(d->m_udpSocket->localAddress(), d->m_udpSocket->localPort());

    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Bound);

    d->setup();

    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Connecting);

    auto request = QKnxNetIpConnectRequestProxy::builder()
        .setControlEndpoint(d->m_nat ? d->m_natEndpoint : d->m_localEndpoint)
        .setDataEndpoint(d->m_nat ? d->m_natEndpoint : d->m_localEndpoint)
        .setRequestInformation(d->m_cri)
        .create();
    d->m_controlEndpointVersion = request.header().protocolVersion();

    qDebug() << "Sending connect request:" << request;

    d->m_connectRequestTimer->start(QKnxNetIp::ConnectRequestTimeout);

    d->m_udpSocket->writeDatagram(request.bytes().toByteArray(),
        d->m_remoteControlEndpoint.address, d->m_remoteControlEndpoint.port);
}

/*!
    Establishes a connection to the host with \a address, \a port and \a proto.
*/
void QKnxNetIpEndpointConnection::connectToHost(const QHostAddress &address, quint16 port,
    QKnxNetIp::HostProtocol proto)
{
    if (proto == QKnxNetIp::HostProtocol::Unknown)
        return;

    if (proto == QKnxNetIp::HostProtocol::UDP_IPv4) {
        connectToHost(address, port);
        return;
    }

    // establishing TCP connection
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
    d->m_remoteControlEndpoint.code = QKnxNetIp::HostProtocol::TCP_IPv4;

    isIPv4 = false;
    d->m_user.address.toIPv4Address(&isIPv4);
    if (!isIPv4) {
        d->setAndEmitErrorOccurred(Error::NotIPv4, tr("Only IPv4 addresses as local control "
            "endpoint supported."));
        return;
    }

    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Starting);

    QKnxPrivate::clearSocket(&(d->m_tcpSocket));

    d->m_tcpSocket = new QTcpSocket(this);
    d->setup();

    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Connecting);

    d->m_tcpSocket->abort();
    d->m_tcpSocket->connectToHost(address, port);

    d->m_localEndpoint = Endpoint(d->m_tcpSocket->localAddress(),
                                  d->m_tcpSocket->localPort());
    d->m_localEndpoint.code = QKnxNetIp::HostProtocol::TCP_IPv4;
    d->m_natEndpoint.code = QKnxNetIp::HostProtocol::TCP_IPv4;
    d->m_remoteDataEndpoint.code = QKnxNetIp::HostProtocol::TCP_IPv4;

    connect(d->m_tcpSocket, &QTcpSocket::connected, this, [&]() {
        Q_D(QKnxNetIpEndpointConnection);
        auto request = QKnxNetIpConnectRequestProxy::builder()
            .setControlEndpoint(d->m_nat ? d->m_natEndpoint : d->m_localEndpoint)
            .setDataEndpoint(d->m_nat ? d->m_natEndpoint : d->m_localEndpoint)
            .setRequestInformation(d->m_cri)
            .create();
        d->m_controlEndpointVersion = request.header().protocolVersion();

        qDebug() << "Sending connect request:" << request;
        d->m_tcpSocket->write(request.bytes().toByteArray());
    });

    // TODO: Implement connect request timeout.
}

/*!
    Closes an established connection.
*/
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
        auto frame = QKnxNetIpDisconnectRequestProxy::builder()
            .setChannelId(d->m_channelId)
            .setControlEndpoint(d->m_nat ? d->m_natEndpoint : d->m_localEndpoint)
            .create();

        qDebug() << "Sending disconnect request:" << frame;
        if (d->m_tcpSocket) {
            d->m_tcpSocket->write(frame.bytes().toByteArray());
        } else {
            d->m_udpSocket->writeDatagram(frame.bytes().toByteArray(),
                d->m_remoteControlEndpoint.address, d->m_remoteControlEndpoint.port);
        }

        d->m_disconnectRequestTimer->start(QKnxNetIp::DisconnectRequestTimeout);
        // Fully disconnected will be handled inside the private cleanup function.
    }
}

QKnxNetIpEndpointConnection::QKnxNetIpEndpointConnection(QKnxNetIpEndpointConnectionPrivate &dd,
        QObject *parent)
    : QObject(dd, parent)
{}

QT_END_NAMESPACE
