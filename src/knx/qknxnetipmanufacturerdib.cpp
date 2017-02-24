/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipmanufacturerdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 manufacturerId)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::ManufactorData))
{
    setData(QKnxUtils::Integer::quint16ToArray<QByteArray>(manufacturerId));
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 manufacturerId, const QByteArray &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::ManufactorData))
{
    setData(QByteArray(QKnxUtils::Integer::quint16ToArray<QByteArray>(manufacturerId) + data));
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 manufacturerId, const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::ManufactorData))
{
    setData(QVector<quint8>(QKnxUtils::Integer::quint16ToArray<QVector<quint8>>(manufacturerId)
        + data));
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(const QByteArray &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::ManufactorData), data)
{}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::ManufactorData), data)
{}

QKnxNetIpManufacturerDIB QKnxNetIpManufacturerDIB::fromRawData(const QByteArray &rawData, qint32 offset)
{
    return QKnxNetIpStructure::fromRawData(rawData, offset);
}

QKnxNetIpManufacturerDIB QKnxNetIpManufacturerDIB::fromRawData(const QVector<quint8> &rawData, qint32 offset)
{
    return QKnxNetIpStructure::fromRawData(rawData, offset);
}

quint16 QKnxNetIpManufacturerDIB::manufacturerId() const
{
    return QKnxUtils::Integer::quint16FromArray(data<QVector<quint8>>(0, 2));
}

bool QKnxNetIpManufacturerDIB::isValid() const
{
    return QKnxNetIpStructure::isValid() && dataSize() >= 2
        && descriptionTypeCode() == DescriptionTypeCode::ManufactorData;
}

QT_END_NAMESPACE
