/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipcrd.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCRD::QKnxNetIpCRD(const QKnxNetIpStruct &other)
    : QKnxNetIpStruct(other)
{}

QKnxNetIp::ConnectionType QKnxNetIpCRD::connectionType() const
{
    return QKnxNetIp::ConnectionType(code());
}

bool QKnxNetIpCRD::isValid() const
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
