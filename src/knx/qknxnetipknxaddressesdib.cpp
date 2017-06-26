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

#include "qknxnetipknxaddressesdib.h"

QT_BEGIN_NAMESPACE

QKnxNetIpKnxAddressesDib::QKnxNetIpKnxAddressesDib(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpKnxAddressesDib::QKnxNetIpKnxAddressesDib(const QKnxAddress &address)
    : QKnxNetIpKnxAddressesDib(QVector<QKnxAddress> { address })
{
}

QKnxNetIpKnxAddressesDib::QKnxNetIpKnxAddressesDib(const QVector<QKnxAddress> &addresses)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::KnxAddresses)
{
    QKnxNetIpPayload payload;
    for (auto address : qAsConst(addresses)) {
        if (address.isValid() && address.type() == QKnxAddress::Type::Individual)
            payload.appendBytes(address.rawData<std::vector<quint8>>());
    }
    setPayload(payload);
}

QKnxNetIp::DescriptionType QKnxNetIpKnxAddressesDib::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

bool QKnxNetIpKnxAddressesDib::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && (size() % 2 == 0) // must be even sized
        && size() >= 2 // and stores the header and at least one address
        && descriptionType() == QKnxNetIp::DescriptionType::KnxAddresses;
}

QT_END_NAMESPACE
