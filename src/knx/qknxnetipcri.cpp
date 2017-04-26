/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipcri.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCRI::QKnxNetIpCRI(const QKnxNetIpStruct &other)
    : QKnxNetIpStruct(other)
{}

QKnxNetIp::ConnectionType QKnxNetIpCRI::connectionType() const
{
    return QKnxNetIp::ConnectionType(code());
}

bool QKnxNetIpCRI::isValid() const
{
    switch (connectionType()) {
        case QKnxNetIp::ConnectionType::DeviceManagementConnection:
        case QKnxNetIp::ConnectionType::TunnelConnection :
        case QKnxNetIp::ConnectionType::RemoteLoggingConnection :
        case QKnxNetIp::ConnectionType::RemoteConfigurationConnection :
        case QKnxNetIp::ConnectionType::ObjectServerConnection :
            break;
        default:
            return false;
    }
    return QKnxNetIpStruct::isValid();
}

QT_END_NAMESPACE
