/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipknxaddressesdib.h"

QT_BEGIN_NAMESPACE

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QKnxAddress &address)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::KnxAddresses), 2)
{
    if (address.isValid() && address.type() == QKnxAddress::Type::Individual)
        setData(quint8(DescriptionTypeCode::KnxAddresses), address.rawData<QByteArray>());
}

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QVector<QKnxAddress> &addresses)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::KnxAddresses), addresses.size() * 2)
{
    QByteArray ba;
    for (auto address:qAsConst(addresses))
    if (address.isValid() && address.type() == QKnxAddress::Type::Individual)
        ba += address.rawData<QByteArray>();
    setData(quint8(DescriptionTypeCode::KnxAddresses), ba);
}

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QByteArray &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::KnxAddresses), data)
{}

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::KnxAddresses), data)
{}

QKnxNetIpKnxAddressesDIB QKnxNetIpKnxAddressesDIB::fromRawData(const QByteArray &rawData, qint32 offset)
{
    QKnxNetIpKnxAddressesDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::KnxAddresses), rawData, offset);
    return dib;
}

QKnxNetIpKnxAddressesDIB QKnxNetIpKnxAddressesDIB::fromRawData(const QVector<quint8> &rawData, qint32 offset)
{
    QKnxNetIpKnxAddressesDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::KnxAddresses), rawData, offset);
    return dib;
}

QVector<QKnxAddress> QKnxNetIpKnxAddressesDIB::individualAddresses() const
{
    QVector<QKnxAddress> addresses;
    auto vec = data<QVector<quint8>>();
    for (int i = 0; i < vec.size(); i + 2)
        addresses.append(QKnxAddress(QKnxAddress::Type::Individual, vec.mid(i, 2)));
    return addresses;
}

QT_END_NAMESPACE
