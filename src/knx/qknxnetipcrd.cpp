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

QKnxNetIp::ConnectionTypeCode QKnxNetIpCRD::connectionTypeCode() const
{
    return QKnxNetIp::ConnectionTypeCode(code());
}

bool QKnxNetIpCRD::isValid() const
{
    switch (connectionTypeCode()) {
        case QKnxNetIp::ConnectionTypeCode::DeviceManagementConnection:
        case QKnxNetIp::ConnectionTypeCode::TunnelConnection :
        case QKnxNetIp::ConnectionTypeCode::RemoteLoggingConnection :
        case QKnxNetIp::ConnectionTypeCode::RemoteConfigurationConnection :
        case QKnxNetIp::ConnectionTypeCode::ObjectServerConnection :
            break;
        default:
            return false;
    }
    return QKnxNetIpStruct::isValid();
}

QT_END_NAMESPACE
