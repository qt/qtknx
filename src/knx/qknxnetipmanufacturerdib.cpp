/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipmanufacturerdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(const QKnxNetIpStruct &other)
    : QKnxNetIpStruct(other)
{}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 manufacturerId)
    : QKnxNetIpManufacturerDIB(manufacturerId, QByteArray {})
{
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 manufacturerId, const QByteArray &data)
    : QKnxNetIpStruct(quint8(QKnxNetIp::DescriptionTypeCode::ManufactorData))
{
    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::QUint16::bytes(manufacturerId));
    payload.appendBytes(data);
    setPayload(payload);
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 manufacturerId, const QVector<quint8> &data)
    : QKnxNetIpStruct(quint8(QKnxNetIp::DescriptionTypeCode::ManufactorData))
{
    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::QUint16::bytes(manufacturerId));
    payload.appendBytes(data);
    setPayload(payload);
}

QKnxNetIp::DescriptionTypeCode QKnxNetIpManufacturerDIB::descriptionTypeCode() const
{
    return QKnxNetIp::DescriptionTypeCode(code());
}

quint16 QKnxNetIpManufacturerDIB::manufacturerId() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef());
}

bool QKnxNetIpManufacturerDIB::isValid() const
{
    return QKnxNetIpStruct::isValid() && size() >= 4
        && descriptionTypeCode() == QKnxNetIp::DescriptionTypeCode::ManufactorData;
}

QT_END_NAMESPACE
