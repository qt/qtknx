/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPDISCOVERYAGENT_P_H
#define QKNXNETIPDISCOVERYAGENT_P_H

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

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipsearchrequest.h>
#include <QtKnx/qknxnetipsearchresponse.h>
#include <QtKnx/qknxnetipserverdiscoveryagent.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qnetworkdatagram.h>
#include <QtNetwork/qudpsocket.h>

#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpServerDiscoveryAgentPrivate final : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QKnxNetIpServerDiscoveryAgent)

public:
    QKnxNetIpServerDiscoveryAgentPrivate(const QHostAddress &addr, quint16 prt);
    ~QKnxNetIpServerDiscoveryAgentPrivate() override = default;

    void setupSocket();

    void setupAndStartReceiveTimer();
    void setupAndStartFrequencyTimer();

    void setAndEmitStateChanged(QKnxNetIpServerDiscoveryAgent::State newState);
    void setAndEmitDeviceDiscovered(const QKnxNetIpServerDiscoveryInfo &discoveryInfo);
    void setAndEmitErrorOccurred(QKnxNetIpServerDiscoveryAgent::Error e, const QString &message);

private:
    QUdpSocket *socket { nullptr };
    QTimer *receiveTimer { nullptr };
    QTimer *frequencyTimer { nullptr };

    quint16 port { 0 };
    QHostAddress address { QHostAddress::AnyIPv4 };

    const quint16 multicastPort { QKnxNetIp::DefaultPort };
    const QHostAddress multicastAddress { QLatin1String(QKnxNetIp::MulticastAddress) };

    quint8 ttl { 60 };
    bool nat { false };
    int frequency { 0 };
    int timeout { QKnxNetIp::Timeout::SearchTimeout };

    QString errorString;
    QVector<QKnxNetIpServerDiscoveryInfo> servers;

    QKnxNetIpServerDiscoveryAgent::Error error { QKnxNetIpServerDiscoveryAgent::Error::None };
    QKnxNetIpServerDiscoveryAgent::State state { QKnxNetIpServerDiscoveryAgent::State::NotRunning };
    QKnxNetIpServerDiscoveryAgent::ResponseType
        type { QKnxNetIpServerDiscoveryAgent::ResponseType::Multicast };
};

QT_END_NAMESPACE

#endif
