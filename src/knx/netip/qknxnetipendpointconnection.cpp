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

#include "qknxcryptographicengine.h"
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
#include "qknxnetipsessionauthenticate.h"
#include "qknxnetipsecurewrapper.h"
#include "qknxnetipsessionrequest.h"
#include "qknxnetipsessionresponse.h"
#include "qknxnetipsessionstatus.h"
#include "qknxnetiptunnelingacknowledge.h"
#include "qknxnetiptunnelingfeatureget.h"
#include "qknxnetiptunnelingfeatureset.h"
#include "qknxnetiptunnelingfeatureinfo.h"
#include "qknxnetiptunnelingfeatureresponse.h"
#include "qknxnetiptunnelingrequest.h"
#include "qnetworkdatagram.h"
#include "qtcpsocket.h"
#include "qudpsocket.h"

#include "private/qknxnetipsecureconfiguration_p.h"

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
    \value SecureConfig
            An invalid secure configuration was used to establish the connection.
    \value SerialNumber
            An invalid serial number was set for the device.
    \value AuthFailed
            The secure client was not successfully authenticated.
    \value Timeout
            A timeout occurred during secure session handshake.
    \value Close
            The server requested to close the secure session.
    \value Unknown
            An unknown error occurred.
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
    QKnxPrivate::clearTimer(&m_secureTimer);

    Q_Q(QKnxNetIpEndpointConnection);
    m_heartbeatTimer = new QTimer(q);
    m_heartbeatTimer->setSingleShot(true);
    QObject::connect(m_heartbeatTimer, &QTimer::timeout, q, [&]() {
        sendStateRequest();
    });

    m_connectRequestTimer = new QTimer(q);
    m_connectRequestTimer->setSingleShot(true);
    QObject::connect(m_connectRequestTimer, &QTimer::timeout, q, [&]() {
        setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Bound);
        setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Acknowledge,
            QKnxNetIpEndpointConnection::tr("Connect request timeout."));

        Q_Q(QKnxNetIpEndpointConnection);
        q->disconnectFromHost();
    });

    m_connectionStateTimer = new QTimer(q);
    m_connectionStateTimer->setSingleShot(true);
    QObject::connect(m_connectionStateTimer, &QTimer::timeout, q, [&]() {
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
    QObject::connect(m_disconnectRequestTimer, &QTimer::timeout, q, [&] () {
        setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Acknowledge,
            QKnxNetIpEndpointConnection::tr("Disconnect request timeout."));
        processDisconnectResponse(QKnxNetIpDisconnectResponseProxy::builder()
            .setChannelId(m_channelId).setStatus(QKnxNetIp::Error::None).create());
    });

    m_acknowledgeTimer = new QTimer(q);
    m_acknowledgeTimer->setSingleShot(true);
    QObject::connect(m_acknowledgeTimer, &QTimer::timeout, q, [&]() {
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

    m_secureTimer = new QTimer(q);
    m_secureTimer->setSingleShot(true);
}

QKnxNetIp::ServiceType
    QKnxNetIpEndpointConnectionPrivate::processReceivedFrame(const QKnxNetIpFrame &frame)
{
    // TODO: fix the version and validity checks
    // if (!m_supportedVersions.contains(frame.header().protocolVersion())) {
    //     send E_VERSION_NOT_SUPPORTED confirmation frame
    //     send disconnect request
    // } else if (frame.header().protocolVersion() != m_controlEndpointVersion) {
    //     send disconnect request
    // } else {
    // TODO: set the m_dataEndpointVersion once we receive or send the first frame

    auto serviceType = frame.serviceType();
    switch (serviceType) {
    case QKnxNetIp::ServiceType::ConnectResponse:
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

    case QKnxNetIp::ServiceType::TunnelingFeatureInfo:
    case QKnxNetIp::ServiceType::TunnelingFeatureResponse:
        processFeatureFrame(frame);
        break;

    case QKnxNetIp::ServiceType::SecureWrapper: {
        qDebug() << "Received secure wrapper frame:" << frame;

        const QKnxNetIpSecureWrapperProxy proxy(frame);
        if (!proxy.isValid())
            break;

        const auto seqNumber = proxy.sequenceNumber();
        const auto serialNumber = proxy.serialNumber();
        const auto messageTag = proxy.messageTag();

        const auto decData = QKnxCryptographicEngine::decryptSecureWrapperPayload(m_sessionKey,
            proxy.encapsulatedFrame(), seqNumber, serialNumber, messageTag);

        const auto mac = QKnxCryptographicEngine::computeMessageAuthenticationCode(m_sessionKey,
            frame.header(), proxy.secureSessionId(), decData, seqNumber, serialNumber, messageTag);
        const auto decMac = QKnxCryptographicEngine::decryptMessageAuthenticationCode(m_sessionKey,
            proxy.messageAuthenticationCode(), seqNumber, serialNumber, messageTag);

        if (decMac != mac)
            break; // MAC could not be verified, bail out

        return processReceivedFrame(QKnxNetIpFrame::fromBytes(decData));
    }   break;

    case QKnxNetIp::ServiceType::SessionRequest:
        qDebug() << "Unexpectedly received session request frame:" << frame;
        break;

    case QKnxNetIp::ServiceType::SessionResponse: {
        qDebug() << "Received session response frame:" << frame;

        QKnxNetIpSessionResponseProxy proxy(frame);
        if (!proxy.isValid())
            break;

        const auto authHash = QKnxCryptographicEngine::deviceAuthenticationCodeHash(m_secureConfig.
            d->deviceAuthenticationCode);
        const auto xorX_Y = QKnxCryptographicEngine::XOR(m_secureConfig.d->publicKey.bytes(), proxy
            .publicKey());

        auto mac = QKnxCryptographicEngine::computeMessageAuthenticationCode(authHash,
            frame.header(), proxy.secureSessionId(), xorX_Y);
        auto decMac = QKnxCryptographicEngine::decryptMessageAuthenticationCode(authHash,
            proxy.messageAuthenticationCode());

        if (decMac != mac)
            break; // MAC could not be verified, bail out

        m_secureTimer->stop();
        m_secureTimer->disconnect();
        m_sessionId = proxy.secureSessionId();
        m_sessionKey = QKnxCryptographicEngine::sessionKey(m_secureConfig.d->privateKey,
            QKnxSecureKey::fromBytes(QKnxSecureKey::Type::Public, proxy.publicKey()));

        auto secureWrapper = QKnxNetIpSecureWrapperProxy::secureBuilder()
            .setSecureSessionId(m_sessionId)
            .setSequenceNumber(m_sequenceNumber)
            .setSerialNumber(m_serialNumber)
            // .setMessageTag(0x0000) TODO: Do we need an API for this?
            .setEncapsulatedFrame(QKnxNetIpSessionAuthenticateProxy::secureBuilder()
                .setUserId(m_secureConfig.d->userId)
                .create(m_secureConfig.d->userPassword, m_secureConfig.d->publicKey.bytes(), proxy
                    .publicKey()))
            .create(m_sessionKey);

        ++m_sequenceNumber;
        m_waitForAuthentication = true;
        if (m_tcpSocket)
            m_tcpSocket->write(secureWrapper.bytes().toByteArray());

        Q_Q(QKnxNetIpEndpointConnection);
        QObject::connect(m_secureTimer, &QTimer::timeout, q, [&]() {
            m_secureTimer->stop();
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::AuthFailed,
                QKnxNetIpEndpointConnection::tr("Did not receive session status frame."));

            auto secureStatusWrapper = QKnxNetIpSecureWrapperProxy::secureBuilder()
                .setSecureSessionId(m_sessionId)
                .setSequenceNumber(m_sequenceNumber)
                .setSerialNumber(m_serialNumber)
                // .setMessageTag(0x0000) TODO: Do we need an API for this?
                .setEncapsulatedFrame(QKnxNetIpSessionStatusProxy::builder()
                    .setStatus(QKnxNetIp::SecureSessionStatus::Close)
                    .create())
                .create(m_sessionKey);
            if (m_tcpSocket)
                m_tcpSocket->write(secureStatusWrapper.bytes().toByteArray());

            Q_Q(QKnxNetIpEndpointConnection);
            q->disconnectFromHost();
        });
        m_secureTimer->start(QKnxNetIp::SecureSessionAuthenticateTimeout);
    }   break;

    case QKnxNetIp::ServiceType::SessionAuthenticate:
        qDebug() << "Unexpectedly received session authenticate frame:" << frame;
        break;

    case QKnxNetIp::ServiceType::SessionStatus: {
        qDebug() << "Received session status frame:" << frame;

        QKnxNetIpSessionStatusProxy ssp(frame);
        if (!ssp.isValid())
            break;

        bool close { false };
        switch (ssp.status()) {
        case QKnxNetIp::SecureSessionStatus::AuthenticationSuccess: {
            if (m_waitForAuthentication) {
                m_secureTimer->stop();
                m_secureTimer->disconnect();
                m_waitForAuthentication = false;
                auto ep = (m_tcpSocket ? m_routeBack : (m_nat ? m_routeBack : m_localEndpoint));
                auto secureWrapper = QKnxNetIpSecureWrapperProxy::secureBuilder()
                    .setSecureSessionId(m_sessionId)
                    .setSequenceNumber(m_sequenceNumber)
                    .setSerialNumber(m_serialNumber)
                    // .setMessageTag(0x0000) TODO: Do we need an API for this?
                    .setEncapsulatedFrame(QKnxNetIpConnectRequestProxy::builder()
                        .setControlEndpoint(ep)
                        .setDataEndpoint(ep)
                        .setRequestInformation(m_cri)
                        .create())
                    .create(m_sessionKey);

                ++m_sequenceNumber;
                if (m_tcpSocket)
                    m_tcpSocket->write(secureWrapper.bytes().toByteArray());

                if (!m_secureConfig.d->keepAlive)
                    break;

                Q_Q(QKnxNetIpEndpointConnection);
                QObject::connect(m_secureTimer, &QTimer::timeout, q, [&]() {
                    auto secureStatusWrapper = QKnxNetIpSecureWrapperProxy::secureBuilder()
                        .setSecureSessionId(m_sessionId)
                        .setSequenceNumber(m_sequenceNumber)
                        .setSerialNumber(m_serialNumber)
                        // .setMessageTag(0x0000) TODO: Do we need an API for this?
                        .setEncapsulatedFrame(QKnxNetIpSessionStatusProxy::builder()
                            .setStatus(QKnxNetIp::SecureSessionStatus::KeepAlive)
                            .create())
                        .create(m_sessionKey);
                    qDebug() << "Sending keep alive status frame:" << secureStatusWrapper;

                    ++m_sequenceNumber;
                    if (m_tcpSocket)
                        m_tcpSocket->write(secureStatusWrapper.bytes().toByteArray());
                });
                m_secureTimer->setSingleShot(false);
                m_secureTimer->start(QKnxNetIp::Timeout::SecureSessionTimeout - 5000);
            }
        }   break;

        case QKnxNetIp::SecureSessionStatus::KeepAlive:
            // TODO: implement in case we're the server
            qDebug() << "Unexpectedly received keep alive status frame:" << frame;
            break;

        case QKnxNetIp::SecureSessionStatus::AuthenticationFailed:
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::AuthFailed,
                QKnxNetIpEndpointConnection::tr("Secure session authentication failed."));
            close = true;
            break;

        case QKnxNetIp::SecureSessionStatus::Unauthenticated:
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::AuthFailed,
                QKnxNetIpEndpointConnection::tr("Secure session not authenticated."));
            close = true;
            break;

        case QKnxNetIp::SecureSessionStatus::Timeout:
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Timeout,
                QKnxNetIpEndpointConnection::tr("A timeout occurred during secure session handshake."));
            close = true;
            break;

        case QKnxNetIp::SecureSessionStatus::Close:
            setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Timeout,
                QKnxNetIpEndpointConnection::tr("The server requested to close the secure session."));
            close = true;
            break;

        default:
            qDebug() << "Received unknown status frame:" << frame;
            break;
        }

        if (close) {
            Q_Q(QKnxNetIpEndpointConnection);
            q->disconnectFromHost();
        }
    }   break;

    case QKnxNetIp::ServiceType::TimerNotify:
        qDebug() << "Unexpectedly received timer notify frame:" << frame;
        break;

    default:
        break;
    }
    return serviceType;
}

bool QKnxNetIpEndpointConnectionPrivate::initConnection(const QHostAddress &a, quint16 p,
    QKnxNetIp::HostProtocol hp)
{
    if (!QKnxNetIp::isStructType(hp))
        return false;

    if (m_state != QKnxNetIpEndpointConnection::State::Disconnected)
        return false;

    auto isIPv4 = false;
    a.toIPv4Address(&isIPv4);
    if (!isIPv4) {
        setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::NotIPv4,
            QKnxNetIpEndpointConnection::tr("Only IPv4 addresses as remote control endpoint "
                "supported."));
        return false;
    }
    m_remoteControlEndpoint = { a, p, hp };

    isIPv4 = false;
    m_user.address.toIPv4Address(&isIPv4);
    if (!isIPv4) {
        setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::NotIPv4,
            QKnxNetIpEndpointConnection::tr("Only IPv4 addresses as local control endpoint "
                "supported."));
        return false;
    }

    setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Starting);

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

    m_routeBack.hostProtocol = hp;

    m_sessionId = 0;
    m_sequenceNumber = 0;
    m_waitForAuthentication = false;

    setupTimer();

    QKnxPrivate::clearSocket(&m_tcpSocket);
    QKnxPrivate::clearSocket(&m_udpSocket);

    Q_Q(QKnxNetIpEndpointConnection);
    QAbstractSocket *socket = nullptr;
    if (hp == QKnxNetIp::HostProtocol::TCP_IPv4) {
        socket = m_tcpSocket = new QTcpSocket(q_func());
        QObject::connect(m_tcpSocket, &QTcpSocket::readyRead, q, [&]() {
            if (m_tcpSocket->bytesAvailable() < QKnxNetIpFrameHeader::HeaderSize10)
                return;
            m_rxBuffer += QKnxByteArray::fromByteArray(m_tcpSocket->readAll());

            // TODO: AN184 v03 KNXnet-IP Core v2 AS, 2.2.3.2.3.2
            auto header = QKnxNetIpFrameHeader::fromBytes(m_rxBuffer);
            if (header.isValid() && m_rxBuffer.size() < header.totalSize())
                return;

            auto frame = QKnxNetIpFrame::fromBytes(m_rxBuffer);
            m_rxBuffer.remove(0, header.totalSize());
            processReceivedFrame(frame);
        });
    } else if (hp == QKnxNetIp::HostProtocol::UDP_IPv4) {
        socket = m_udpSocket = new QUdpSocket(q_func());
        QObject::connect(m_udpSocket, &QUdpSocket::readyRead, q, [&]() {
            while (m_udpSocket && m_udpSocket->state() == QUdpSocket::BoundState
                && m_udpSocket->hasPendingDatagrams()) {
                    auto tmp = m_udpSocket->receiveDatagram();
                    auto frame = QKnxNetIpFrame::fromBytes(QKnxByteArray::fromByteArray(tmp.data()));
                    if (processReceivedFrame(frame) != QKnxNetIp::ServiceType::ConnectResponse)
                        continue;

                    if (m_nat && m_remoteDataEndpoint.isNullOrLocal())
                        m_remoteDataEndpoint = { tmp.senderAddress(), quint16(tmp.senderPort())};
            }
        });
    } else {
        return false;
    }

    if (socket) {
        QObject::connect(socket, &QAbstractSocket::errorOccurred,
            q, [socket, this](QAbstractSocket::SocketError) {
                setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::Network,
                    socket->errorString());
                Q_Q(QKnxNetIpEndpointConnection);
                q->disconnectFromHost();
        });
    } else {
        return false;
    }

    setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Connecting);
    return true;
}

void QKnxNetIpEndpointConnectionPrivate::cleanup()
{
    QKnxPrivate::clearTimer(&m_heartbeatTimer);
    QKnxPrivate::clearTimer(&m_connectRequestTimer);
    QKnxPrivate::clearTimer(&m_connectionStateTimer);
    QKnxPrivate::clearTimer(&m_disconnectRequestTimer);
    QKnxPrivate::clearTimer(&m_acknowledgeTimer);
    QKnxPrivate::clearTimer(&m_secureTimer);

    if (m_udpSocket) {
        m_udpSocket->close();
        QKnxPrivate::clearSocket(&m_udpSocket);
    } else if (m_tcpSocket) {
        if (m_secureConfig.isValid()) {
            auto secureStatusWrapper = QKnxNetIpSecureWrapperProxy::secureBuilder()
                .setSecureSessionId(m_sessionId)
                .setSequenceNumber(m_sequenceNumber)
                .setSerialNumber(m_serialNumber)
                // .setMessageTag(0x0000) TODO: Do we need an API for this?
                .setEncapsulatedFrame(QKnxNetIpSessionStatusProxy::builder()
                    .setStatus(QKnxNetIp::SecureSessionStatus::Close)
                    .create())
                .create(m_sessionKey);
            ++m_sequenceNumber;
            m_tcpSocket->write(secureStatusWrapper.bytes().toByteArray());
            m_tcpSocket->waitForBytesWritten();
        }
        m_tcpSocket->close();
        QKnxPrivate::clearSocket(&m_tcpSocket);
    }

    setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Disconnected);
}

bool QKnxNetIpEndpointConnectionPrivate::sendCemiRequest()
{
    if (m_udpSocket) {
        if (m_waitForAcknowledgement)
            return false; // still waiting for an ACK from an previous request

        m_waitForAcknowledgement = true;
        m_udpSocket->writeDatagram(m_lastSendCemiRequest.bytes().toByteArray(),
            m_remoteDataEndpoint.address,
            m_remoteDataEndpoint.port);

        m_cemiRequests++;
        m_acknowledgeTimer->start(m_acknowledgeTimeout);
        return true;
    }

    if (m_tcpSocket) {
        if (m_secureConfig.isValid()) {
            auto secureFrame = QKnxNetIpSecureWrapperProxy::secureBuilder()
                .setSecureSessionId(m_sessionId)
                .setSequenceNumber(m_sequenceNumber)
                .setSerialNumber(m_serialNumber)
                // .setMessageTag(0x0000) TODO: Do we need an API for this?
                .setEncapsulatedFrame(m_lastSendCemiRequest)
                .create(m_sessionKey);
            ++m_sequenceNumber;
            m_tcpSocket->write(secureFrame.bytes().toByteArray());
        } else {
            m_tcpSocket->write(m_lastSendCemiRequest.bytes().toByteArray());
        }
        return true; // TCP connections do not send an ACK
    }
    return false;
}

void QKnxNetIpEndpointConnectionPrivate::sendStateRequest()
{
    qDebug().noquote().nospace() << "Sending connection state request: 0x" << m_lastStateRequest
        .bytes().toHex();

    if (m_tcpSocket) {
        if (m_secureConfig.isValid()) {
            auto secureFrame = QKnxNetIpSecureWrapperProxy::secureBuilder()
                .setSecureSessionId(m_sessionId)
                .setSequenceNumber(m_sequenceNumber)
                .setSerialNumber(m_serialNumber)
                // .setMessageTag(0x0000) TODO: Do we need an API for this?
                .setEncapsulatedFrame(m_lastStateRequest)
                .create(m_sessionKey);
            ++m_sequenceNumber;
            m_tcpSocket->write(secureFrame.bytes().toByteArray());
        } else {
            m_tcpSocket->write(m_lastStateRequest.bytes().toByteArray());
        }
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

bool QKnxNetIpEndpointConnectionPrivate::sendTunnelingFeatureGet(QKnx::InterfaceFeature feature)
{
    m_lastSendCemiRequest = QKnxNetIpTunnelingFeatureGetProxy::builder()
        .setChannelId(m_channelId)
        .setSequenceNumber(m_sendCount)
        .setFeatureIdentifier(feature)
        .create();
    qDebug().noquote() << "Sending tunneling feature get:" << m_lastSendCemiRequest;

    return sendCemiRequest();
}

bool QKnxNetIpEndpointConnectionPrivate::sendTunnelingFeatureSet(QKnx::InterfaceFeature feature,
    const QKnxByteArray &value)
{
    m_lastSendCemiRequest = QKnxNetIpTunnelingFeatureSetProxy::builder()
        .setChannelId(m_channelId)
        .setSequenceNumber(m_sendCount)
        .setFeatureIdentifier(feature)
        .setFeatureValue(value)
        .create();
    qDebug().noquote() << "Sending tunneling feature set:" << m_lastSendCemiRequest;

    return sendCemiRequest();
}

void QKnxNetIpEndpointConnectionPrivate::processFeatureFrame(const QKnxNetIpFrame &frame)
{
    qDebug() << "Received tunneling feature frame:" << frame;

    QKnxNetIpTunnelingFeatureInfoProxy proxy(frame);
    if (m_tcpSocket || proxy.isValid()) {
        processTunnelingFeatureFrame(frame);
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
                processTunnelingFeatureFrame(frame);
        }
    } else {
        qDebug() << "Frame was ignored due to wrong channel ID. Expected:" << m_channelId
            << "Current:" << frame.channelId();
    }
}

void QKnxNetIpEndpointConnectionPrivate::processTunnelingFeatureFrame(const QKnxNetIpFrame &)
{}

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
                .setControlEndpoint(
                        m_tcpSocket ? m_routeBack : (m_nat ? m_routeBack : m_localEndpoint)
                    )
                .create();

            Q_Q(QKnxNetIpEndpointConnection);
            QTimer::singleShot(0, q, [&]() { sendStateRequest(); });
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
        auto responseFrame = QKnxNetIpDisconnectResponseProxy::builder()
            .setChannelId(m_channelId)
            .setStatus(QKnxNetIp::Error::None)
            .create();
        qDebug() << "Sending disconnect response:" << responseFrame;
        if (m_tcpSocket) {
            if (m_secureConfig.isValid()) {
                responseFrame = QKnxNetIpSecureWrapperProxy::secureBuilder()
                    .setSecureSessionId(m_sessionId)
                    .setSequenceNumber(m_sequenceNumber)
                    .setSerialNumber(m_serialNumber)
                    // .setMessageTag(0x0000) TODO: Do we need an API for this?
                    .setEncapsulatedFrame(responseFrame)
                    .create(m_sessionKey);
                ++m_sequenceNumber;
            }
            m_tcpSocket->write(responseFrame.bytes().toByteArray());
        } else {
            m_udpSocket->writeDatagram(responseFrame.bytes().toByteArray(),
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


// -- QKnxNetIpEndpointConnection

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
    Sets the value of the heartbeat timeout. Every \a msec a state request is
    sent over the connection to keep it alive.
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
    Establishes a connection to the host with \a address and \a port via UDP.
*/
void QKnxNetIpEndpointConnection::connectToHost(const QHostAddress &address, quint16 port)
{
    Q_D(QKnxNetIpEndpointConnection);

    if (!d->initConnection(address, port, QKnxNetIp::HostProtocol::UDP_IPv4))
        return;

    if (!d->m_udpSocket->bind(d->m_user.address, d->m_user.port))
        return;
    d->m_localEndpoint = { d->m_udpSocket->localAddress(), d->m_udpSocket->localPort() };

    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Bound);

    auto request = QKnxNetIpConnectRequestProxy::builder()
        .setControlEndpoint(d->m_nat ? d->m_routeBack : d->m_localEndpoint)
        .setDataEndpoint(d->m_nat ? d->m_routeBack : d->m_localEndpoint)
        .setRequestInformation(d->m_cri)
        .create();
    d->m_controlEndpointVersion = request.header().protocolVersion();

    d->setAndEmitStateChanged(QKnxNetIpEndpointConnection::State::Connecting);

    qDebug() << "Sending connect request:" << request;
    d->m_udpSocket->writeDatagram(request.bytes().toByteArray(),
        d->m_remoteControlEndpoint.address, d->m_remoteControlEndpoint.port);

    if (d->m_connectRequestTimer)
        d->m_connectRequestTimer->start(QKnxNetIp::ConnectRequestTimeout);
}

/*!
    \since 5.12

    Establishes a connection to the host with \a address, \a port and \a protocol.
*/
void QKnxNetIpEndpointConnection::connectToHost(const QHostAddress &address, quint16 port,
    QKnxNetIp::HostProtocol protocol)
{
    if (protocol == QKnxNetIp::HostProtocol::UDP_IPv4)
        return connectToHost(address, port);

    Q_D(QKnxNetIpEndpointConnection);
    if (!d->initConnection(address, port, protocol))
        return;

    connect(d->m_tcpSocket, &QTcpSocket::connected, this, [&]() {
        Q_D(QKnxNetIpEndpointConnection);
        d->m_localEndpoint = { d->m_tcpSocket->localAddress(), d->m_tcpSocket->localPort(),
            QKnxNetIp::HostProtocol::TCP_IPv4 };

        auto request = QKnxNetIpConnectRequestProxy::builder()
            .setControlEndpoint(d->m_routeBack)
            .setDataEndpoint(d->m_routeBack)
            .setRequestInformation(d->m_cri)
            .create();
        d->m_controlEndpointVersion = request.header().protocolVersion();

        qDebug() << "Sending connect request:" << request;
        d->m_tcpSocket->write(request.bytes().toByteArray());
        d->m_connectRequestTimer->start(QKnxNetIp::ConnectRequestTimeout);
    });
    d->m_tcpSocket->connectToHost(address, port);
}

/*!
    \since 5.13

    Returns the serial number of the device using this connection. The default
    value is set to \c {0x000000000000}.
*/
QKnxByteArray QKnxNetIpEndpointConnection::serialNumber() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_serialNumber;
}

/*!
    \since 5.13

    Sets the serial number to \a serialNumber of the device using this
    connection.

    \note The serial number must contain exactly 6 bytes and cannot be changed
    while the secure session is established.
*/
void QKnxNetIpEndpointConnection::setSerialNumber(const QKnxByteArray &serialNumber)
{
    Q_D(QKnxNetIpEndpointConnection);
    if (d->m_state == QKnxNetIpEndpointConnection::State::Disconnected)
        d->m_serialNumber = serialNumber;
}

/*!
    \since 5.13

    Returns the secure configuration used to establish the secure session.
*/
QKnxNetIpSecureConfiguration QKnxNetIpEndpointConnection::secureConfiguration() const
{
    Q_D(const QKnxNetIpEndpointConnection);
    return d->m_secureConfig;
}

/*!
    \since 5.13

    Sets a secure configuration to be used to establish secure session
    to \a {config}. The configuration cannot be changed while the secure
    connection is established.
*/
void QKnxNetIpEndpointConnection::setSecureConfiguration(const QKnxNetIpSecureConfiguration &config)
{
    Q_D(QKnxNetIpEndpointConnection);
    if (d->m_state == QKnxNetIpEndpointConnection::State::Disconnected) {
        d->m_secureConfig = config;
        d->updateCri(config.individualAddress());
    }
}

/*!
    \since 5.13

    Establishes a connection to the KNXnet/IP control endpoint \a controlEndpoint.

    \sa setSerialNumber(), setSecureConfiguration()
*/
void QKnxNetIpEndpointConnection::connectToHostEncrypted(const QKnxNetIpHpai &controlEndpoint)
{
    const QKnxNetIpHpaiProxy proxy(controlEndpoint);
    if (proxy.isValid() && proxy.hostProtocol() == QKnxNetIp::HostProtocol::TCP_IPv4)
        connectToHost(proxy.hostAddress(), proxy.port());
}

/*!
    \since 5.13

    Establishes a secure session to the host with \a address and \a port.

    \sa setSerialNumber(), setSecureConfiguration()
*/
void QKnxNetIpEndpointConnection::connectToHostEncrypted(const QHostAddress &address, quint16 port)
{
    Q_D(QKnxNetIpEndpointConnection);
    if (!d->initConnection(address, port, QKnxNetIp::HostProtocol::TCP_IPv4))
        return;

    if (!d->m_secureConfig.isValid())
        return d->setAndEmitErrorOccurred(Error::SecureConfig, tr("Invalid secure configuration."));

    if (d->m_serialNumber.size() != 6)
        return d->setAndEmitErrorOccurred(Error::SerialNumber, tr("Invalid device serial number."));

    connect(d->m_tcpSocket, &QTcpSocket::connected, this, [&]() {
        Q_D(QKnxNetIpEndpointConnection);
        d->m_localEndpoint = Endpoint(d->m_tcpSocket->localAddress(),
            d->m_tcpSocket->localPort(), QKnxNetIp::HostProtocol::TCP_IPv4);

        auto request = QKnxNetIpSessionRequestProxy::builder()
            .setControlEndpoint(d->m_routeBack)
            .setPublicKey(d->m_secureConfig.d->publicKey.bytes())
            .create();
        d->m_controlEndpointVersion = request.header().protocolVersion();

        qDebug() << "Sending secure session request:" << request;
        d->m_tcpSocket->write(request.bytes().toByteArray());

        QObject::connect(d->m_secureTimer, &QTimer::timeout, this, [&]() {
            Q_D(QKnxNetIpEndpointConnection);
            d->m_secureTimer->stop();
            d->setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error::AuthFailed,
                QKnxNetIpEndpointConnection::tr("Could not establish secure session."));
            disconnectFromHost();
        });
        d->m_secureTimer->start(QKnxNetIp::SecureSessionRequestTimeout);
    });
    d->m_tcpSocket->connectToHost(address, port);
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
            .setControlEndpoint(d->m_nat ? d->m_routeBack : d->m_localEndpoint)
            .create();

        qDebug() << "Sending disconnect request:" << frame;
        if (d->m_tcpSocket) {
            if (d->m_secureConfig.isValid()) {
                auto secureFrame = QKnxNetIpSecureWrapperProxy::secureBuilder()
                    .setSecureSessionId(d->m_sessionId)
                    .setSequenceNumber(d->m_sequenceNumber)
                    .setSerialNumber(d->m_serialNumber)
                    // .setMessageTag(0x0000) TODO: Do we need an API for this?
                    .setEncapsulatedFrame(frame)
                    .create(d->m_sessionKey);
                ++(d->m_sequenceNumber);
                d->m_tcpSocket->write(secureFrame.bytes().toByteArray());
            } else {
                d->m_tcpSocket->write(frame.bytes().toByteArray());
            }
        } else {
            d->m_udpSocket->writeDatagram(frame.bytes().toByteArray(),
                d->m_remoteControlEndpoint.address, d->m_remoteControlEndpoint.port);
        }

        d->m_disconnectRequestTimer->start(QKnxNetIp::DisconnectRequestTimeout);
        // Fully disconnected will be handled inside the private cleanup function.
    }
}

/*!
    \internal
*/
QKnxNetIpEndpointConnection::QKnxNetIpEndpointConnection(QKnxNetIpEndpointConnectionPrivate &dd,
        QObject *parent)
    : QObject(dd, parent)
{}

QT_END_NAMESPACE
