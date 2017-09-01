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

#include "qknx8bitset.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE


// -- QKnx8BitSet

QKnx8BitSet::QKnx8BitSet()
    : QKnx8BitSet(SubType)
{}

QKnx8BitSet::QKnx8BitSet(quint8 byte)
    : QKnx8BitSet(SubType)
{
    operator[](0) = byte;
}

QKnx8BitSet::QKnx8BitSet(int subType)
    : QKnxDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("8-bit set"));

    setRange(QVariant(0x00), QVariant(0xff));
    setRangeText(tr("No bits set"), tr("All bits set"));
}

bool QKnx8BitSet::bit(int index) const
{
    return testBit(operator[](0), index);
}

void QKnx8BitSet::setBit(bool value, int index)
{
    QKnxDatapointType::setBit(operator[](0), value, index);
}

quint8 QKnx8BitSet::byte() const
{
    return operator[](0);
}

void QKnx8BitSet::setByte(quint8 value)
{
    operator[](0) = value;
}

bool QKnx8BitSet::isValid() const
{
    return mainType() == MainType && size() == TypeSize && operator[](0) <= maximum().toUInt();
}


// -- QKnxGeneralStatus

QKnxGeneralStatus::QKnxGeneralStatus()
    : QKnx8BitSet(SubType)
{
    setDescription(tr("General Status"));
    setRange(QVariant(0x00), QVariant(0x1f));
}

QKnxGeneralStatus::QKnxGeneralStatus(Attributes attributes)
    : QKnx8BitSet(SubType)
{
    setValue(attributes);
}

QKnxGeneralStatus::Attributes QKnxGeneralStatus::value() const
{
    return Attributes().setFlag(Attribute::OutOfService, testBit(operator[](0), 0))
        .setFlag(Attribute::Fault, testBit(operator[](0), 1))
        .setFlag(Attribute::Overridden, testBit(operator[](0), 2))
        .setFlag(Attribute::InAlarm, testBit(operator[](0), 3))
        .setFlag(Attribute::AlarmUnacknowledged, testBit(operator[](0), 4));
}

void QKnxGeneralStatus::setValue(Attributes attributes)
{
    quint8 byte = operator[](0);
    setBit(byte, attributes.testFlag(Attribute::OutOfService), 0);
    setBit(byte, attributes.testFlag(Attribute::Fault), 1);
    setBit(byte, attributes.testFlag(Attribute::Overridden), 2);
    setBit(byte, attributes.testFlag(Attribute::InAlarm), 3);
    setBit(byte, attributes.testFlag(Attribute::AlarmUnacknowledged), 4);
    operator[](0) = byte;
}

bool QKnxGeneralStatus::isSet(Attribute attribute) const
{
    return value().testFlag(attribute);
}

void QKnxGeneralStatus::setAttribute(Attribute attribute)
{
    setValue(value() | attribute);
}

void QKnxGeneralStatus::removeAttribute(Attribute attribute)
{
    setValue(value() &~ attribute);
}

#include "moc_qknx8bitset.cpp"

QT_END_NAMESPACE
