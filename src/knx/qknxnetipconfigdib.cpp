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

#include "qknxnetipconfigdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QNetworkAddressEntry &addressEntry,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods)
    : QKnxNetIpConfigDIB(addressEntry.ip(), addressEntry.netmask(), gateway, caps, methods)
{}

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QHostAddress &ip, const QHostAddress &subnetMask,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::IpConfiguration)
{
    // Capabilities are limited and at least one assignment method shall be enabled.
    if (caps > 0x07 || (methods < 0x01 || methods > 0x15))
        return;

    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::HostAddress::bytes(ip));
    payload.appendBytes(QKnxUtils::HostAddress::bytes(subnetMask));
    payload.appendBytes(QKnxUtils::HostAddress::bytes(gateway));
    payload.setByte(12, quint8(caps));
    payload.setByte(13, quint8(methods));
    setPayload(payload);
}

QKnxNetIp::DescriptionType QKnxNetIpConfigDIB::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

QHostAddress QKnxNetIpConfigDIB::ipAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef());
}

QHostAddress QKnxNetIpConfigDIB::subnetMask() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef(), 4);
}

QHostAddress QKnxNetIpConfigDIB::defaultGateway() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef(), 8);
}

QKnxNetIpConfigDIB::Capabilities QKnxNetIpConfigDIB::capabilities() const
{
    return QKnxNetIpConfigDIB::Capabilities(payloadRef().byte(12));
}

QKnxNetIpConfigDIB::AssignmentMethods QKnxNetIpConfigDIB::assignmentMethods() const
{
    return QKnxNetIpConfigDIB::AssignmentMethods(payloadRef().byte(13));
}

bool QKnxNetIpConfigDIB::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && size() == 16
        && descriptionType() == QKnxNetIp::DescriptionType::IpConfiguration;
}

QT_END_NAMESPACE
