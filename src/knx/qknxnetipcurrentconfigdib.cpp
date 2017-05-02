/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipcurrentconfigdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QNetworkAddressEntry &addressEntry,
        const QHostAddress &gateway, const QHostAddress &dhcp, AssignmentMethod method)
    : QKnxNetIpCurrentConfigDIB(addressEntry.ip(), addressEntry.netmask(), gateway, dhcp, method)
{}

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QHostAddress &ip,
    const QHostAddress &subnetMask, const QHostAddress &gateway, const QHostAddress &dhcp,
        AssignmentMethod method)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::CurrentIpConfiguration)
{
    size_t bitcount = std::bitset<8>(quint8(method)).count();
    if (method < AssignmentMethod::Manual || method > AssignmentMethod::AutoIp || bitcount != 1)
        return; // Only one assignment method shall be enabled from within the possible range.

    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::HostAddress::bytes(ip));
    payload.appendBytes(QKnxUtils::HostAddress::bytes(subnetMask));
    payload.appendBytes(QKnxUtils::HostAddress::bytes(gateway));
    payload.appendBytes(QKnxUtils::HostAddress::bytes(dhcp));
    payload.setByte(16, quint8(method));
    payload.setByte(17, quint8(0));
    setPayload(payload);
}

QKnxNetIp::DescriptionType QKnxNetIpCurrentConfigDIB::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

QHostAddress QKnxNetIpCurrentConfigDIB::ipAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef());
}

QHostAddress QKnxNetIpCurrentConfigDIB::subnetMask() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef(), 4);
}

QHostAddress QKnxNetIpCurrentConfigDIB::defaultGateway() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef(), 8);
}

QHostAddress QKnxNetIpCurrentConfigDIB::dhcpOrBootP() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef(), 12);
}

QKnxNetIpCurrentConfigDIB::AssignmentMethod QKnxNetIpCurrentConfigDIB::assignmentMethod() const
{
    return QKnxNetIpCurrentConfigDIB::AssignmentMethod(payloadRef().byte(16));
}

bool QKnxNetIpCurrentConfigDIB::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && size() == 20
        && descriptionType() == QKnxNetIp::DescriptionType::CurrentIpConfiguration;
}

QT_END_NAMESPACE
