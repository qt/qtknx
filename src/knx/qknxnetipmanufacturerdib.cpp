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

#include "qknxnetipmanufacturerdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpManufacturerDib::QKnxNetIpManufacturerDib(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpManufacturerDib::QKnxNetIpManufacturerDib(quint16 manufacturerId)
    : QKnxNetIpManufacturerDib(manufacturerId, QByteArray {})
{
}

QKnxNetIpManufacturerDib::QKnxNetIpManufacturerDib(quint16 manufacturerId, const QByteArray &data)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::ManufactorData)
{
    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::QUint16::bytes(manufacturerId));
    payload.appendBytes(data);
    setPayload(payload);
}

QKnxNetIpManufacturerDib::QKnxNetIpManufacturerDib(quint16 manufacturerId, const QVector<quint8> &data)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::ManufactorData)
{
    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::QUint16::bytes(manufacturerId));
    payload.appendBytes(data);
    setPayload(payload);
}

QKnxNetIp::DescriptionType QKnxNetIpManufacturerDib::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

quint16 QKnxNetIpManufacturerDib::manufacturerId() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef());
}

bool QKnxNetIpManufacturerDib::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && size() >= 4
        && descriptionType() == QKnxNetIp::DescriptionType::ManufactorData;
}

QT_END_NAMESPACE
