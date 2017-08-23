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

#include "qknxdatapoint.h"
#include "qknxdptfactory.h"

QT_BEGIN_NAMESPACE

bool QKnxDataPoint::isValid() const
{
    switch (m_dptId.mainType) {
    case QKnxDpt::Types::B1:
        return QKnxDptFactory::SimpleBinary::validate(bytes<QByteArray>());
    case QKnxDpt::Types::B2:
        return QKnxDptFactory::Control::validate(bytes<QByteArray>());
    case QKnxDpt::Types::B1U3:
        return QKnxDptFactory::TypeB1U3::validate(bytes<QByteArray>());
    case QKnxDpt::Types::U8:
        return QKnxDptFactory::TypeU8::validate(bytes<QByteArray>());
    case QKnxDpt::Types::V8: // B5N3
        return QKnxDptFactory::TypeV8::validate(bytes<QByteArray>());
    case QKnxDpt::Types::U16:
        return QKnxDptFactory::TypeU16::validate(bytes<QByteArray>());
    case QKnxDpt::Types::V16:
        return QKnxDptFactory::TypeV16::validate(bytes<QByteArray>());
    case QKnxDpt::Types::F16:
        return QKnxDptFactory::TypeF16::validate(bytes<QByteArray>());
    case QKnxDpt::Types::N3U5r2U6r2U6:
        return QKnxDptFactory::TimeOfDay::validate(bytes<QByteArray>());
    case QKnxDpt::Types::r3U5r4U4r1U7:
        return QKnxDptFactory::Date::validate(bytes<QByteArray>());
    case QKnxDpt::Types::U32:
        return QKnxDptFactory::Uint32::validate(bytes<QByteArray>());
    case QKnxDpt::Types::V32:
        return QKnxDptFactory::TypeV32::validate(bytes<QByteArray>());
    case QKnxDpt::Types::F32:
        return QKnxDptFactory::TypeF32::validate(bytes<QByteArray>());
    case QKnxDpt::Types::U4U4U4U4U4U4B4N4:
        return QKnxDptFactory::AccessData::validate(bytes<QByteArray>());
    case QKnxDpt::Types::r2U6:
        return QKnxDptFactory::Scene::validate(bytes<QByteArray>());
    case QKnxDpt::Types::r1B1U6:
        return QKnxDptFactory::Scene::validate(bytes<QByteArray>());
    case QKnxDpt::Types::B1r1U6:
        return QKnxDptFactory::Scene::validate(bytes<QByteArray>());
    case QKnxDpt::Types::U8r4U4r3U5U3U5r2U6r2U6B16:
        return QKnxDptFactory::DateTime::validate(bytes<QByteArray>());
    case QKnxDpt::Types::N8:
        return QKnxDptFactory::TypeN8::validate(bytes<QByteArray>());
    case QKnxDpt::Types::B8:
        return QKnxDptFactory::TypeB8::validate(bytes<QByteArray>());
    case QKnxDpt::Types::N2:
        return QKnxDptFactory::TypeN2::validate(bytes<QByteArray>());
    case QKnxDpt::Types::Unknown:
    default:
        break;
    }
    return false;
}

quint8 QKnxDataPoint::dataPointTypeIdMainNumber() const
{
    return m_dptId.mainNumber;
}
quint8 QKnxDataPoint::dataPointTypeIdSubNumber() const
{
    return m_dptId.subNumber;
}

QString QKnxDataPoint::dataPointTypeName() const
{
    return m_dataPointTypeName;
}

QString QKnxDataPoint::propertyDataTypeName() const
{
    return m_propertyDataTypeName;
}

quint8 QKnxDataPoint::usage() const
{
    return m_usage;
}

void QKnxDataPoint::setMainNumber(quint8 mainNumber)
{
    m_dptId.mainNumber = mainNumber;
}

void QKnxDataPoint::setMainType(QKnxDpt::Types mainType)
{
    m_dptId.mainType = mainType;
}

void QKnxDataPoint::setSubNumber(quint8 subNumber)
{
    m_dptId.subNumber = subNumber;
}

void QKnxDataPoint::setSpecifics(quint8 subNumber, const QString dptName, const QString pdtName,
    QKnxDpt::Usage usage)
{
    setSubNumber(subNumber);
    m_dataPointTypeName = dptName;
    m_propertyDataTypeName = pdtName;
    m_usage = quint8(usage);
}

QT_END_NAMESPACE
