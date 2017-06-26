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

#ifndef QKNXNETIPTUNNELCLIENT_H
#define QKNXNETIPTUNNELCLIENT_H

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipclient.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpTunnelClientPrivate;
using QKnxTunnelingFrame = QKnxCemiFrame;

class Q_KNX_EXPORT QKnxNetIpTunnelClient final : public QKnxNetIpClient
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxNetIpTunnelClient)
    Q_DECLARE_PRIVATE(QKnxNetIpTunnelClient)

public:
    QKnxNetIpTunnelClient(QObject *parent = nullptr);
    ~QKnxNetIpTunnelClient() override = default;

    QKnxNetIpTunnelClient(const QHostAddress &localAddress, QObject *parent = nullptr);
    QKnxNetIpTunnelClient(const QHostAddress &localAddress, quint16 localPort,
        QObject *parent = nullptr);
    QKnxNetIpTunnelClient(const QHostAddress &localAddress, quint16 localPort,
        QKnxNetIp::TunnelingLayer layer, QObject *parent = nullptr);

    QKnxAddress individualAddress() const;
    void setIndividualAddress(const QKnxAddress &address);

    void sendTunnelingFrame(const QKnxTunnelingFrame &frame);

Q_SIGNALS:
    void receivedTunnelingFrame(QKnxTunnelingFrame frame);
};

QT_END_NAMESPACE

#endif
