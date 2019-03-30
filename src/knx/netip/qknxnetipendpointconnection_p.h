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

#ifndef QKNXNETIPENDPOINTCONNECTION_P_H
#define QKNXNETIPENDPOINTCONNECTION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt KNX API.  It exists for the convenience
// of the Qt KNX implementation.  This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qtimer.h>

#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxdevicemanagementframe.h>
#include <QtKnx/qknxlinklayerframe.h>
#include <QtKnx/qknxnetipendpointconnection.h>
#include <QtKnx/qknxnetipsecureconfiguration.h>

#include <QtNetwork/qhostaddress.h>

#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpConnectResponseProxy;
class QKnxNetIpConnectionStateResponseProxy;
class QKnxNetIpDeviceConfigurationAcknowledgeProxy;
class QKnxNetIpDeviceConfigurationRequestProxy;
class QKnxNetIpDisconnectRequestProxy;
class QKnxNetIpDisconnectResponseProxy;
class QKnxNetIpTunnelingAcknowledgeProxy;
class QKnxNetIpTunnelingRequestProxy;
class QUdpSocket;
class QTcpSocket;

struct UserProperties
{
    quint16 port { 0 };
    bool natAware { false };
    QHostAddress address { QHostAddress::LocalHost };
    QKnxByteArray supportedVersions  { QKnxNetIpFrameHeader::KnxNetIpVersion10 };
};

struct Endpoint final
{
    Endpoint() = default;
    Endpoint(const QHostAddress &addr, quint16 p)
        : address(addr)
        , port(p)
    {}
    Endpoint(const QHostAddress &addr, quint16 p, QKnxNetIp::HostProtocol c)
        : address(addr)
        , port(p)
        , hostProtocol(c)
    {}
    explicit Endpoint(const QKnxNetIpHpaiProxy &hpai)
        : address(hpai.hostAddress())
        , port(hpai.port())
    {}
    explicit Endpoint(const QHostAddress::SpecialAddress &addr)
        : address(addr)
    {}

    Endpoint &operator=(const QKnxNetIpHpai &s)
    {
        const QKnxNetIpHpaiProxy hpai(s);
        hostProtocol = hpai.hostProtocol(); address = hpai.hostAddress(); port = hpai.port();
        return *this;
    }
    operator QKnxNetIpHpai() const
    {
        return QKnxNetIpHpaiProxy::builder()
                .setHostAddress(address)
                .setPort(port)
                .setHostProtocol(hostProtocol)
                .create();
    }
    bool isNullOrLocal() const
    {
        return address.isNull() || address == QHostAddress::Any || address.isLoopback() || port == 0;
    }

    QHostAddress address { QHostAddress::Null };
    quint16 port { 0 };
    QKnxNetIp::HostProtocol hostProtocol { QKnxNetIp::HostProtocol::UDP_IPv4 };
};

class Q_KNX_EXPORT QKnxNetIpEndpointConnectionPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpEndpointConnection)

public:
    QKnxNetIpEndpointConnectionPrivate(const QHostAddress &address, quint16 port,
            const QKnxNetIpCri &cri, int sendAttempts, QKnxNetIp::Timeout ackTimeout)
        : m_cri(cri)
        , m_localEndpoint { address, port }
        , m_maxCemiRequest(sendAttempts)
        , m_acknowledgeTimeout(ackTimeout)
    {}
    ~QKnxNetIpEndpointConnectionPrivate() override = default;

    void setupTimer();
    bool initConnection(const QHostAddress &address, quint16 port, QKnxNetIp::HostProtocol hp);
    void cleanup();

    bool sendCemiRequest();
    void sendStateRequest();

    QKnxNetIp::ServiceType processReceivedFrame(const QKnxNetIpFrame &frame);
    virtual void process(const QKnxLinkLayerFrame &frame);
    virtual void process(const QKnxDeviceManagementFrame &frame);

    // datapoint related processing
    bool sendTunnelingRequest(const QKnxLinkLayerFrame &frame);
    virtual void processTunnelingRequest(const QKnxNetIpFrame &frame);
    virtual void processTunnelingAcknowledge(const QKnxNetIpFrame &frame);

    bool sendDeviceConfigurationRequest(const QKnxDeviceManagementFrame &frame);
    virtual void processDeviceConfigurationRequest(const QKnxNetIpFrame &frame);
    virtual void processDeviceConfigurationAcknowledge(const QKnxNetIpFrame &frame);

    bool sendTunnelingFeatureGet(QKnx::InterfaceFeature feature);
    bool sendTunnelingFeatureSet(QKnx::InterfaceFeature feature, const QKnxByteArray &value);
    virtual void processFeatureFrame(const QKnxNetIpFrame &frame);
    virtual void processTunnelingFeatureFrame(const QKnxNetIpFrame &frame);

    // endpoint related processing
    virtual void processConnectResponse(const QKnxNetIpFrame &frame);
    virtual void processConnectionStateResponse(const QKnxNetIpFrame &frame);
    virtual void processDisconnectRequest(const QKnxNetIpFrame &frame);
    virtual void processDisconnectResponse(const QKnxNetIpFrame &frame);

    void setAndEmitStateChanged(QKnxNetIpEndpointConnection::State newState);
    void setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error newError, const QString &message);

    QKnxNetIpCri cri() const { return m_cri; }
    void updateCri(QKnxNetIp::TunnelLayer layer)
    {
        QKnxNetIpCriProxy proxy(m_cri);
        m_cri = proxy.builder()
            .setTunnelLayer(layer)
            .setIndividualAddress(proxy.individualAddress())
            .create();
    }

    void updateCri(const QKnxAddress &ia)
    {
        QKnxNetIpCriProxy proxy(m_cri);
        if (proxy.connectionType() == QKnxNetIp::ConnectionType::DeviceManagement)
            return;

        if (ia.isValid()) {
            m_cri = QKnxNetIpCriProxy::builder()
                .setTunnelLayer(proxy.tunnelLayer())
                .setIndividualAddress(ia)
                .create();
        } else {
            m_cri = proxy.builder()
                .setTunnelLayer(proxy.tunnelLayer())
                .create();
        }
    }

private:
    QKnxNetIpCri m_cri;
    Endpoint m_remoteDataEndpoint;
    Endpoint m_remoteControlEndpoint;

    Endpoint m_routeBack { QHostAddress::AnyIPv4 };
    Endpoint m_localEndpoint { QHostAddress::LocalHost };

    int m_channelId { -1 };
    quint8 m_sendCount { 0 };
    quint8 m_receiveCount { 0 };

    int m_cemiRequests { 0 };
    const int m_maxCemiRequest { 0 };
    const int m_acknowledgeTimeout { 0 };

    QKnxNetIpFrame m_lastSendCemiRequest {};
    QKnxNetIpFrame m_lastReceivedCemiRequest {};

    int m_stateRequests { 0 };
    const int m_maxStateRequests = { 3 };
    QKnxNetIpFrame m_lastStateRequest {};

    bool m_nat { false };
    quint32 m_heartbeatTimeout { QKnxNetIp::HeartbeatTimeout };
    QKnxByteArray m_supportedVersions { QKnxNetIpFrameHeader::KnxNetIpVersion10 };

    QString m_errorString;
    quint8 m_dataEndpointVersion = QKnxNetIpFrameHeader::KnxNetIpVersion10;
    quint8 m_controlEndpointVersion = QKnxNetIpFrameHeader::KnxNetIpVersion10;

    QKnxNetIpEndpointConnection::Error m_error = QKnxNetIpEndpointConnection::Error::None;
    QKnxNetIpEndpointConnection::State m_state = QKnxNetIpEndpointConnection::State::Disconnected;

    QTimer *m_heartbeatTimer { nullptr };
    QTimer *m_connectRequestTimer { nullptr };
    QTimer *m_connectionStateTimer { nullptr };
    QTimer *m_disconnectRequestTimer { nullptr };
    QTimer *m_acknowledgeTimer { nullptr };
    bool m_waitForAcknowledgement { false };

    QUdpSocket *m_udpSocket { nullptr };
    QTcpSocket *m_tcpSocket { nullptr };
    QKnxByteArray m_rxBuffer;

    UserProperties m_user;

    quint16 m_sessionId { 0 };
    quint48 m_sequenceNumber { 0 };
    bool m_waitForAuthentication { false };

    QKnxByteArray m_sessionKey;
    QTimer *m_secureTimer { nullptr };
    QKnxNetIpSecureConfiguration m_secureConfig;

    // TODO: We need some kind of device configuration class as well.
    QKnxByteArray m_serialNumber { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
};

QT_END_NAMESPACE

#endif
