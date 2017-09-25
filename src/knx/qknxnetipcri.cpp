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

#include "qknxnetipcri.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCri::QKnxNetIpCri(QKnxNetIp::ConnectionType connectionType)
    : QKnxNetIpConnectionTypeStruct(connectionType)
{}

QKnxNetIpCri::QKnxNetIpCri(QKnxNetIp::TunnelingLayer layer)
    : QKnxNetIpConnectionTypeStruct(QKnxNetIp::ConnectionType::Tunnel)
{
    setTunnelingLayer(layer);
}

QKnxNetIpCri::QKnxNetIpCri(const QKnxNetIpConnectionTypeStruct &other)
    : QKnxNetIpConnectionTypeStruct(other)
{}

QKnxNetIp::ConnectionType QKnxNetIpCri::connectionType() const
{
    return code();
}

void QKnxNetIpCri::setConnectionType(QKnxNetIp::ConnectionType connectionType)
{
    setCode(connectionType);
}

bool QKnxNetIpCri::isValid() const
{
    switch (connectionType()) {
        case QKnxNetIp::ConnectionType::DeviceManagement:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 2;
        case QKnxNetIp::ConnectionType::Tunnel:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 4
                && QKnxNetIp::isTunnelingLayer(tunnelingLayer());
        case QKnxNetIp::ConnectionType::RemoteLogging:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 2;
        case QKnxNetIp::ConnectionType::RemoteConfiguration:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 2;
        case QKnxNetIp::ConnectionType::ObjectServer:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 2;
        default:
            break;
    }
    return false;
}

QKnxNetIp::TunnelingLayer QKnxNetIpCri::tunnelingLayer() const
{
    return QKnxNetIp::TunnelingLayer(payloadRef().byte(0));
}

bool QKnxNetIpCri::setTunnelingLayer(QKnxNetIp::TunnelingLayer layer)
{
    if (connectionType() != QKnxNetIp::ConnectionType::Tunnel
        || (!QKnxNetIp::isTunnelingLayer(layer)))
        return false;

    quint8 payload[2] = { quint8(layer), 0x00 };
    setPayload({ payload, 2 });
    return true;
}

QT_END_NAMESPACE
