/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipconfigdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QKnxNetIpStruct &other)
    : QKnxNetIpStruct(other)
{}

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QNetworkAddressEntry &addressEntry,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods)
    : QKnxNetIpConfigDIB(addressEntry.ip(), addressEntry.netmask(), gateway, caps, methods)
{}

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QHostAddress &ip, const QHostAddress &subnetMask,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods)
    : QKnxNetIpStruct(quint8(QKnxNetIp::DescriptionTypeCode::IpConfiguration))
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

QKnxNetIp::DescriptionTypeCode QKnxNetIpConfigDIB::descriptionTypeCode() const
{
    return QKnxNetIp::DescriptionTypeCode(code());
}

QHostAddress QKnxNetIpConfigDIB::ipAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(payload().bytes(0, 4));
}

QHostAddress QKnxNetIpConfigDIB::subnetMask() const
{
    return QKnxUtils::HostAddress::fromBytes(payload().bytes(4, 4));
}

QHostAddress QKnxNetIpConfigDIB::defaultGateway() const
{
    return QKnxUtils::HostAddress::fromBytes(payload().bytes(8, 4));
}

QKnxNetIpConfigDIB::Capabilities QKnxNetIpConfigDIB::capabilities() const
{
    return QKnxNetIpConfigDIB::Capabilities(payload().bytes(12, 1)[0]);
}

QKnxNetIpConfigDIB::AssignmentMethods QKnxNetIpConfigDIB::assignmentMethods() const
{
    return QKnxNetIpConfigDIB::AssignmentMethods(payload().bytes(13, 1)[0]);
}

bool QKnxNetIpConfigDIB::isValid() const
{
    return QKnxNetIpStruct::isValid() && size() == 16
        && descriptionTypeCode() == QKnxNetIp::DescriptionTypeCode::IpConfiguration;
}

QT_END_NAMESPACE
