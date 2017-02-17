/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipconfigdib.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QNetworkAddressEntry &addressAndSubnetMask,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods)
    : QKnxNetIpConfigDIB(addressAndSubnetMask.ip(), addressAndSubnetMask.netmask(), gateway, caps,
        methods)
{
}

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QHostAddress &ip, const QHostAddress &subnetMask,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::IpConfiguration), 14)
{
    // Capabilities are limited and at least one assignment method shall be enabled.
    if (caps > 0x07 || (methods < 0x01 || methods > 0x15))
        return;

    QByteArray data(0, Qt::Uninitialized);
    data.insert(0, QKnxNetIpUtils::toArray<QByteArray>(ip));
    data.insert(4, QKnxNetIpUtils::toArray<QByteArray>(subnetMask));
    data.insert(8, QKnxNetIpUtils::toArray<QByteArray>(gateway));

    data[12] = quint8(caps);
    data[13] = quint8(methods);

    setData(quint8(DescriptionTypeCode::IpConfiguration), data);
}

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QByteArray &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::IpConfiguration), data)
{
    resizeData(14); // size enforced by 7.5.4.4 IP Config DIB
}

QKnxNetIpConfigDIB::QKnxNetIpConfigDIB(const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::IpConfiguration), data)
{
    resizeData(14); // size enforced by 7.5.4.4 IP Config DIB
}

QKnxNetIpConfigDIB QKnxNetIpConfigDIB::fromRawData(const QByteArray &rawData, qint32 offset)
{
    QKnxNetIpConfigDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::IpConfiguration), rawData, offset);
    resizeData(14); // size enforced by 7.5.4.4 IP Config DIB
    return dib;
}

QKnxNetIpConfigDIB QKnxNetIpConfigDIB::fromRawData(const QVector<quint8> &rawData, qint32 offset)
{
    QKnxNetIpConfigDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::IpConfiguration), rawData, offset);
    resizeData(14); // size enforced by 7.5.4.4 IP Config DIB
    return dib;
}

QHostAddress QKnxNetIpConfigDIB::ipAddress() const
{
    return QKnxNetIpUtils::fromArray(data<QVector<quint8>>(0, 4));
}

QHostAddress QKnxNetIpConfigDIB::subnetMask() const
{
    return QKnxNetIpUtils::fromArray(data<QVector<quint8>>(4, 4));
}

QHostAddress QKnxNetIpConfigDIB::defaultGateway() const
{
    return QKnxNetIpUtils::fromArray(data<QVector<quint8>>(8, 4));
}

QKnxNetIpConfigDIB::Capabilities QKnxNetIpConfigDIB::capabilities() const
{
    return QKnxNetIpConfigDIB::Capabilities(data<QVector<quint8>>(12, 1)[0]);
}

QKnxNetIpConfigDIB::AssignmentMethods QKnxNetIpConfigDIB::assignmentMethods() const
{
    return QKnxNetIpConfigDIB::AssignmentMethods(data<QVector<quint8>>(13, 1)[0]);
}

QT_END_NAMESPACE
