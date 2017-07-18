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

#ifndef QKNXNETIPENDPOINTCONNECTION_P_H
#define QKNXNETIPENDPOINTCONNECTION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qtimer.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipendpointconnection.h>
#include <QtNetwork/qhostaddress.h>

#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpConnectResponse;
class QKnxNetIpConnectionStateResponse;
class QKnxNetIpDeviceConfigurationAcknowledge;
class QKnxNetIpDeviceConfigurationRequest;
class QKnxNetIpDisconnectRequest;
class QKnxNetIpDisconnectResponse;
class QKnxNetIpTunnelingAcknowledge;
class QKnxNetIpTunnelingRequest;
class QUdpSocket;

struct Endpoint final
{
    Endpoint() = default;
    Endpoint(const QHostAddress &addr, quint16 p)
        : address(addr)
        , port(p)
    {}
    explicit Endpoint(const QKnxNetIpHpai &hpai)
        : address(hpai.address())
        , port(hpai.port())
    {}
    explicit Endpoint(const QHostAddress::SpecialAddress &addr)
        : address(addr)
    {}

    Endpoint &operator=(const QKnxNetIpHpai &hpai)
    {
        address = hpai.address(); port = hpai.port();
        return *this;
    }
    operator QKnxNetIpHpai() const { return { address, port }; }

    QHostAddress address { QHostAddress::LocalHost };
    quint16 port { 0 };
};

class Q_KNX_EXPORT QKnxNetIpEndpointConnectionPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpEndpointConnection)

public:
    QKnxNetIpEndpointConnectionPrivate(const QHostAddress &address, quint16 port,
            const QKnxNetIpCri &cri, int sendAttempts, QKnxNetIp::Timeout ackTimeout)
        : m_cri(cri)
        , m_localControlEndpoint { address, port }
        , m_maxCemiRequest(sendAttempts)
        , m_acknowledgeTimeout(ackTimeout)
    {}
    ~QKnxNetIpEndpointConnectionPrivate() override = default;

    void setup();
    void setupTimer();
    void cleanup();

    void sendCemiRequest();
    void sendStateRequest();

    virtual void process(const QKnxCemiFrame &frame);

    // datapoint related processing
    void sendTunnelingRequest(const QKnxCemiFrame &frame);
    virtual void process(const QKnxNetIpTunnelingRequest &);
    virtual void process(const QKnxNetIpTunnelingAcknowledge &);

    void sendDeviceConfigurationRequest(const QKnxCemiFrame &frame);
    virtual void process(const QKnxNetIpDeviceConfigurationRequest &);
    virtual void process(const QKnxNetIpDeviceConfigurationAcknowledge &);

    // endpoint related processing
    virtual void process(const QKnxNetIpConnectResponse &response, const QNetworkDatagram &dg);
    virtual void process(const QKnxNetIpConnectionStateResponse &response);
    virtual void process(const QKnxNetIpDisconnectRequest &request);
    virtual void process(const QKnxNetIpDisconnectResponse &response);

    virtual void processDatagram(QKnxNetIpEndpointConnection::EndpointType, const QNetworkDatagram &);

    void setAndEmitStateChanged(QKnxNetIpEndpointConnection::State newState);
    void setAndEmitErrorOccurred(QKnxNetIpEndpointConnection::Error newError, const QString &message);

private:
    QKnxNetIpCri m_cri;
    Endpoint m_remoteDataEndpoint;
    Endpoint m_remoteControlEndpoint;

    Endpoint m_natEndpoint { QHostAddress::AnyIPv4 };

    Endpoint m_localDataEndpoint { QHostAddress::LocalHost };
    Endpoint m_localControlEndpoint { QHostAddress::LocalHost };

    int m_channelId { -1 };
    quint8 m_sendCount { 0 };
    quint8 m_receiveCount { 0 };

    int m_cemiRequests { 0 };
    int m_maxCemiRequest { 0 };
    int m_acknowledgeTimeout { 0 };
    QByteArray m_lastCemiRequest {};

    int m_stateRequests { 0 };
    const int m_maxStateRequests = { 3 };
    QByteArray m_lastStateRequest {};

    bool m_nat { false };
    quint32 m_heartbeatTimeout { QKnxNetIp::HeartbeatTimeout };
    QVector<quint8> m_supportedVersions { QKnxNetIpFrameHeader::KnxNetIpVersion10 };

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

    QUdpSocket *m_dataEndpoint { nullptr };
    QUdpSocket *m_controlEndpoint { nullptr };
};

QT_END_NAMESPACE

#endif
