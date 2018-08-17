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

#ifndef QKNXNETIPTUNNEL_H
#define QKNXNETIPTUNNEL_H

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxnetipendpointconnection.h>
#include <QtKnx/qknxlinklayerframe.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpTunnelPrivate;
class Q_KNX_EXPORT QKnxNetIpTunnel final : public QKnxNetIpEndpointConnection
{
    Q_OBJECT
    Q_DISABLE_COPY(QKnxNetIpTunnel)
    Q_DECLARE_PRIVATE(QKnxNetIpTunnel)

public:
    QKnxNetIpTunnel(QObject *parent = nullptr);
    ~QKnxNetIpTunnel() override = default;

    QKnxNetIpTunnel(const QHostAddress &localAddress, QObject *parent = nullptr);
    QKnxNetIpTunnel(const QHostAddress &localAddress, quint16 localPort, QObject *parent = nullptr);
    QKnxNetIpTunnel(const QHostAddress &localAddress, quint16 localPort,
        QKnxNetIp::TunnelLayer layer, QObject *parent = nullptr);

    QKnxAddress individualAddress() const;
    void setIndividualAddress(const QKnxAddress &address);

    QKnxNetIp::TunnelLayer layer() const;
    void setTunnelLayer(QKnxNetIp::TunnelLayer layer);

    bool sendFrame(const QKnxLinkLayerFrame &frame);

    bool sendTunnelingFeatureGet(QKnx::InterfaceFeature feature);
    bool sendTunnelingFeatureSet(QKnx::InterfaceFeature feature, const QKnxByteArray &value);

Q_SIGNALS:
    void frameReceived(QKnxLinkLayerFrame frame);

    void tunnelingFeatureInfoReceived(QKnx::InterfaceFeature feature, QKnxByteArray value);
    void tunnelingFeatureResponseReceived(QKnx::InterfaceFeature feature, QKnx::ReturnCode code,
                                          QKnxByteArray value);
};

QT_END_NAMESPACE

#endif
