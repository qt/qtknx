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

#include "qknxnetipcri.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCRI::QKnxNetIpCRI(QKnxNetIp::ConnectionType connectionType)
    : QKnxNetIpConnectionTypeStruct(connectionType)
{}

QKnxNetIpCRI::QKnxNetIpCRI(QKnxNetIp::TunnelingLayer layer)
    : QKnxNetIpConnectionTypeStruct(QKnxNetIp::ConnectionType::Tunnel)
{
    setTunnelingLayer(layer);
}

QKnxNetIpCRI::QKnxNetIpCRI(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId serviceType)
    : QKnxNetIpCRI(QKnxNetIpServiceFamiliesDIB::connectionTypeFromServiceType(serviceType))
{}

QKnxNetIpCRI::QKnxNetIpCRI(const QKnxNetIpConnectionTypeStruct &other)
    : QKnxNetIpConnectionTypeStruct(other)
{}

QKnxNetIp::ConnectionType QKnxNetIpCRI::connectionType() const
{
    return QKnxNetIp::ConnectionType(code());
}

void QKnxNetIpCRI::setConnectionType(QKnxNetIp::ConnectionType connectionType)
{
    setCode(connectionType);
}

bool QKnxNetIpCRI::isValid() const
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

QKnxNetIp::TunnelingLayer QKnxNetIpCRI::tunnelingLayer() const
{
    return QKnxNetIp::TunnelingLayer(payloadRef().byte(0));
}

bool QKnxNetIpCRI::setTunnelingLayer(QKnxNetIp::TunnelingLayer layer)
{
    if (connectionType() != QKnxNetIp::ConnectionType::Tunnel
        || (!QKnxNetIp::isTunnelingLayer(layer)))
        return false;

    quint8 payload[2] = { quint8(layer), 0x00 };
    setPayload({ payload, 2 });
    return true;
}

QT_END_NAMESPACE
