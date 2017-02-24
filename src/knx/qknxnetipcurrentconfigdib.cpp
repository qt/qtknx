/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipcurrentconfigdib.h"

#include <bitset>

QT_BEGIN_NAMESPACE

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QNetworkAddressEntry &addrSubnetMask,
        const QHostAddress &gateway, const QHostAddress &dhcp, AssignmentMethod method)
    : QKnxNetIpCurrentConfigDIB(addrSubnetMask.ip(), addrSubnetMask.netmask(), gateway, dhcp,
        method)
{}

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QHostAddress &ip,
        const QHostAddress &subnetMask, const QHostAddress &gateway, const QHostAddress &dhcp,
        AssignmentMethod method)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::CurrentIpConfiguration), 18)
{
    size_t bitcount = std::bitset<8>(quint8(method)).count();
    if (method < AssignmentMethod::Manual || method > AssignmentMethod::AutoIp || bitcount != 1)
        return; // Only one assignment method shall be enabled from within the possible range.

    QByteArray data(0, Qt::Uninitialized);
    data.insert(0, QKnxUtils::HostAddress::toArray<QByteArray>(ip));
    data.insert(4, QKnxUtils::HostAddress::toArray<QByteArray>(subnetMask));
    data.insert(8, QKnxUtils::HostAddress::toArray<QByteArray>(gateway));
    data.insert(12, QKnxUtils::HostAddress::toArray<QByteArray>(dhcp));
    data[16] = quint8(method);

    setData(quint8(DescriptionTypeCode::CurrentIpConfiguration), data);
}

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QByteArray &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::CurrentIpConfiguration), data)
{
    resizeData(18); // size enforced by 7.5.4.5 IP Current Config DIB
}

QKnxNetIpCurrentConfigDIB::QKnxNetIpCurrentConfigDIB(const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::CurrentIpConfiguration), data)
{
    resizeData(18); // size enforced by 7.5.4.5 IP Current Config DIB
}

QKnxNetIpCurrentConfigDIB QKnxNetIpCurrentConfigDIB::fromRawData(const QByteArray &rawData,
    qint32 offset)
{
    QKnxNetIpCurrentConfigDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::CurrentIpConfiguration), rawData, offset);
    resizeData(18); // size enforced by 7.5.4.5 IP Current Config DIB
    return dib;
}

QKnxNetIpCurrentConfigDIB QKnxNetIpCurrentConfigDIB::fromRawData(const QVector<quint8> &rawData,
    qint32 offset)
{
    QKnxNetIpCurrentConfigDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::CurrentIpConfiguration), rawData, offset);
    resizeData(18); // size enforced by 7.5.4.5 IP Current Config DIB
    return dib;
}

QHostAddress QKnxNetIpCurrentConfigDIB::ipAddress() const
{
    return QKnxUtils::HostAddress::fromArray(data<QVector<quint8>>(0, 4));
}

QHostAddress QKnxNetIpCurrentConfigDIB::subnetMask() const
{
    return QKnxUtils::HostAddress::fromArray(data<QVector<quint8>>(4, 4));
}

QHostAddress QKnxNetIpCurrentConfigDIB::defaultGateway() const
{
    return QKnxUtils::HostAddress::fromArray(data<QVector<quint8>>(8, 4));
}

QHostAddress QKnxNetIpCurrentConfigDIB::dhcpOrBootP() const
{
    return QKnxUtils::HostAddress::fromArray(data<QVector<quint8>>(12, 4));
}

QKnxNetIpCurrentConfigDIB::AssignmentMethod QKnxNetIpCurrentConfigDIB::assignmentMethod() const
{
    return QKnxNetIpCurrentConfigDIB::AssignmentMethod(data<QVector<quint8>>(16, 1)[0]);
}

QT_END_NAMESPACE
