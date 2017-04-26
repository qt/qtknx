/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipknxaddressesdib.h"

QT_BEGIN_NAMESPACE

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QKnxNetIpStruct &other)
    : QKnxNetIpStruct(other)
{}

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QKnxAddress &address)
    : QKnxNetIpKnxAddressesDIB(QVector<QKnxAddress> { address })
{
}

QKnxNetIpKnxAddressesDIB::QKnxNetIpKnxAddressesDIB(const QVector<QKnxAddress> &addresses)
    : QKnxNetIpStruct(quint8(QKnxNetIp::DescriptionType::KnxAddresses))
{
    QKnxNetIpPayload payload;
    for (auto address : qAsConst(addresses)) {
        if (address.isValid() && address.type() == QKnxAddress::Type::Individual)
            payload.appendBytes(address.rawData<std::vector<quint8>>());
    }
    setPayload(payload);
}

QKnxNetIp::DescriptionType QKnxNetIpKnxAddressesDIB::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

bool QKnxNetIpKnxAddressesDIB::isValid() const
{
    return QKnxNetIpStruct::isValid() && (size() % 2 == 0) // must be even sized
        && size() >= 2 // and stores the header and at least one address
        && descriptionType() == QKnxNetIp::DescriptionType::KnxAddresses;
}

QT_END_NAMESPACE
