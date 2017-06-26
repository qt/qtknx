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

#include "qknxnetipcrd.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCRD::QKnxNetIpCRD(const QKnxNetIpConnectionTypeStruct &other)
    : QKnxNetIpConnectionTypeStruct(other)
{}

QKnxNetIpCRD::QKnxNetIpCRD(QKnxNetIp::ConnectionType connectionType)
    : QKnxNetIpConnectionTypeStruct(connectionType)
{}

QKnxNetIpCRD::QKnxNetIpCRD(const QKnxAddress &individualAddress)
    : QKnxNetIpConnectionTypeStruct(QKnxNetIp::ConnectionType::Tunnel)
{
    setIndividualAddress(individualAddress);
}

QKnxNetIp::ConnectionType QKnxNetIpCRD::connectionType() const
{
    return QKnxNetIp::ConnectionType(code());
}

void QKnxNetIpCRD::setConnectionType(QKnxNetIp::ConnectionType connectionType)
{
    setCode(connectionType);
}

bool QKnxNetIpCRD::isValid() const
{
    switch (connectionType()) {
        case QKnxNetIp::ConnectionType::DeviceManagement:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 2;
        case QKnxNetIp::ConnectionType::Tunnel: {
            auto address = individualAddress();
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 4
                && address.isValid() && address.type() == QKnxAddress::Type::Individual;
        } break;
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

QKnxAddress QKnxNetIpCRD::individualAddress() const
{
    return QKnxAddress(QKnxAddress::Type::Individual, QKnxUtils::QUint16::fromBytes(payloadRef()));
}

bool QKnxNetIpCRD::setIndividualAddress(const QKnxAddress &address)
{
    if (connectionType() != QKnxNetIp::ConnectionType::Tunnel
        || (!address.isValid()) || (address.type() != QKnxAddress::Type::Individual))
        return false;

    QKnxNetIpPayload load;
    load.setBytes(address.rawData());
    setPayload(load);
    return true;
}

QT_END_NAMESPACE
