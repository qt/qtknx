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
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::ManufactorData), 2)
{
    setData(quint8(DescriptionTypeCode::ManufactorData),
        QKnxUtils::Integer::quint16ToArray<QByteArray>(manufacturerId));
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 id, const QByteArray &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::ManufactorData), 2)
{
    setData(quint8(DescriptionTypeCode::ManufactorData),
        QByteArray(QKnxUtils::Integer::quint16ToArray<QByteArray>(id) + data));
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(quint16 id, const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::ManufactorData), 2)
{
    setData(quint8(DescriptionTypeCode::ManufactorData),
        QVector<quint8>(QKnxUtils::Integer::quint16ToArray<QVector<quint8>>(id) + data));
}

QKnxNetIpManufacturerDIB::QKnxNetIpManufacturerDIB(const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::ManufactorData), data)
{}

QKnxNetIpManufacturerDIB QKnxNetIpManufacturerDIB::fromRawData(const QByteArray &rawData, qint32 offset)
{
    QKnxNetIpManufacturerDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::ManufactorData), rawData, offset);
    return dib;
}

QKnxNetIpManufacturerDIB QKnxNetIpManufacturerDIB::fromRawData(const QVector<quint8> &rawData, qint32 offset)
{
    QKnxNetIpManufacturerDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::ManufactorData), rawData, offset);
    return dib;
}

quint16 QKnxNetIpManufacturerDIB::manufacturerId() const
{
    return QKnxUtils::Integer::quint16FromArray(data<QVector<quint8>>(0, 2));
}

QT_END_NAMESPACE
