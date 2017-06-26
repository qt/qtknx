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

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 manufacturerId)
    : QKnxNetIpManufacturerDIB(manufacturerId, QByteArray {})
{
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 manufacturerId, const QByteArray &data)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::ManufactorData)
{
    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::QUint16::bytes(manufacturerId));
    payload.appendBytes(data);
    setPayload(payload);
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 manufacturerId, const QVector<quint8> &data)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::ManufactorData)
{
    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::QUint16::bytes(manufacturerId));
    payload.appendBytes(data);
    setPayload(payload);
}

QKnxNetIp::DescriptionType QKnxNetIpManufacturerDIB::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

quint16 QKnxNetIpManufacturerDIB::manufacturerId() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef());
}

bool QKnxNetIpManufacturerDIB::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && size() >= 4
        && descriptionType() == QKnxNetIp::DescriptionType::ManufactorData;
}

QT_END_NAMESPACE
