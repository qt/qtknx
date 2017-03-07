/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipcurrentconfigdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QKnxNetIpStruct &other)
    : QKnxNetIpStruct(other)
{}

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QNetworkAddressEntry &addressEntry,
        const QHostAddress &gateway, const QHostAddress &dhcp, AssignmentMethod method)
    : QKnxNetIpCurrentConfigDIB(addressEntry.ip(), addressEntry.netmask(), gateway, dhcp, method)
{}

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QHostAddress &ip,
    const QHostAddress &subnetMask, const QHostAddress &gateway, const QHostAddress &dhcp,
        AssignmentMethod method)
    : QKnxNetIpStruct(quint8(QKnxNetIp::DescriptionTypeCode::CurrentIpConfiguration))
{
    size_t bitcount = std::bitset<8>(quint8(method)).count();
    if (method < AssignmentMethod::Manual || method > AssignmentMethod::AutoIp || bitcount != 1)
        return; // Only one assignment method shall be enabled from within the possible range.

    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::HostAddress::bytes<std::vector<quint8>>(ip));
    payload.appendBytes(QKnxUtils::HostAddress::bytes<std::vector<quint8>>(subnetMask));
    payload.appendBytes(QKnxUtils::HostAddress::bytes<std::vector<quint8>>(gateway));
    payload.appendBytes(QKnxUtils::HostAddress::bytes<std::vector<quint8>>(dhcp));
    payload.setByte(17, quint8(method));
    setPayload(payload);
}

QKnxNetIp::DescriptionTypeCode QKnxNetIpCurrentConfigDIB::descriptionTypeCode() const
{
    return QKnxNetIp::DescriptionTypeCode(code());
}

QHostAddress QKnxNetIpCurrentConfigDIB::ipAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(payload().bytes<std::vector<quint8>>(0, 4));
}

QHostAddress QKnxNetIpCurrentConfigDIB::subnetMask() const
{
    return QKnxUtils::HostAddress::fromBytes(payload().bytes<std::vector<quint8>>(4, 4));
}

QHostAddress QKnxNetIpCurrentConfigDIB::defaultGateway() const
{
    return QKnxUtils::HostAddress::fromBytes(payload().bytes<std::vector<quint8>>(8, 4));
}

QHostAddress QKnxNetIpCurrentConfigDIB::dhcpOrBootP() const
{
    return QKnxUtils::HostAddress::fromBytes(payload().bytes<std::vector<quint8>>(12, 4));
}

QKnxNetIpCurrentConfigDIB::AssignmentMethod QKnxNetIpCurrentConfigDIB::assignmentMethod() const
{
    return QKnxNetIpCurrentConfigDIB::AssignmentMethod(payload().bytes<std::vector<quint8>>(16, 1)[0]);
}

bool QKnxNetIpCurrentConfigDIB::isValid() const
{
    return QKnxNetIpStruct::isValid() && size() == 20
        && descriptionTypeCode() == QKnxNetIp::DescriptionTypeCode::CurrentIpConfiguration;
}

QT_END_NAMESPACE
