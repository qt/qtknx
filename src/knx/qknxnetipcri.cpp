/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipcri.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCRI::QKnxNetIpCRI(const QKnxNetIpConnectionTypeStruct &other)
    : QKnxNetIpConnectionTypeStruct(other)
{}

QKnxNetIp::ConnectionType QKnxNetIpCRI::connectionType() const
{
    return QKnxNetIp::ConnectionType(code());
}

bool QKnxNetIpCRI::isValid() const
{
    switch (connectionType()) {
        case QKnxNetIp::ConnectionType::DeviceManagement:
        case QKnxNetIp::ConnectionType::Tunnel:
        case QKnxNetIp::ConnectionType::RemoteLogging:
        case QKnxNetIp::ConnectionType::RemoteConfiguration:
        case QKnxNetIp::ConnectionType::ObjectServer:
            break;
        default:
            return false;
    }
    return QKnxNetIpConnectionTypeStruct::isValid();
}

QT_END_NAMESPACE
