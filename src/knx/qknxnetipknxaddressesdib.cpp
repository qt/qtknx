/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipknxaddressesdib.h"

QT_BEGIN_NAMESPACE

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QKnxAddress &address)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::KnxAddresses))
{
    if (address.isValid() && address.type() == QKnxAddress::Type::Individual)
        setData(address.rawData<QByteArray>());
}

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QVector<QKnxAddress> &addresses)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::KnxAddresses))
{
    QByteArray ba;
    for (auto address : qAsConst(addresses)) {
        if (address.isValid() && address.type() == QKnxAddress::Type::Individual)
            ba += address.rawData<QByteArray>();
    }
    setData(ba);
}

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QByteArray &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::KnxAddresses), data)
{}

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::KnxAddresses), data)
{}

QKnxNetIpKnxAddressesDIB QKnxNetIpKnxAddressesDIB::fromRawData(const QByteArray &rawData, qint32 offset)
{
    return QKnxNetIpStructure::fromRawData(rawData, offset);
}

QKnxNetIpKnxAddressesDIB QKnxNetIpKnxAddressesDIB::fromRawData(const QVector<quint8> &rawData, qint32 offset)
{
    return QKnxNetIpStructure::fromRawData(rawData, offset);
}

QVector<QKnxAddress> QKnxNetIpKnxAddressesDIB::individualAddresses() const
{
    QVector<QKnxAddress> addresses;
    auto vec = data<QVector<quint8>>();
    for (int i = 0; i < vec.size(); i =+ 2)
        addresses.append(QKnxAddress(QKnxAddress::Type::Individual, vec.mid(i, 2)));
    return addresses;
}

bool QKnxNetIpKnxAddressesDIB::isValid() const
{
    return QKnxNetIpStructure::isValid() && (dataSize() % 2 == 0) // must be even sized
        && dataSize() >= 2 // and store at least one address
        && descriptionTypeCode() == DescriptionTypeCode::KnxAddresses;
}

QT_END_NAMESPACE
