/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipcri.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCRI::QKnxNetIpCRI(QKnxNetIp::ConnectionType connectionType)
    : QKnxNetIpConnectionTypeStruct(connectionType)
{}

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
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 4;
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

QT_END_NAMESPACE
