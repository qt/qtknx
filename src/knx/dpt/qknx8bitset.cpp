/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknx8bitset.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE


// -- QKnx8BitSet

QKnx8BitSet::QKnx8BitSet()
    : QKnx8BitSet(0)
{}

QKnx8BitSet::QKnx8BitSet(quint8 value)
    : QKnx8BitSet(SubType, value)
{}

QKnx8BitSet::QKnx8BitSet(int subType, quint8 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("8-bit set"));
    setRange(QVariant(0x00), QVariant(0xff));
    setRangeText(tr("No bits set"), tr("All bits set"));

    setByte(value);
}

bool QKnx8BitSet::bit(int index) const
{
    return QKnxDatapointType::testBit(byte(), index);
}

void QKnx8BitSet::setBit(bool value, int index)
{
    QKnxDatapointType::setBit(data(), value, index);
}

quint8 QKnx8BitSet::byte() const
{
    return QKnxDatapointType::byte(0);
}

bool QKnx8BitSet::setByte(quint8 value)
{
    return QKnxDatapointType::setByte(0, value);
}

bool QKnx8BitSet::isValid() const
{
    return QKnxDatapointType::isValid() && byte() <= maximum().toUInt();
}


// -- QKnxGeneralStatus

QKnxGeneralStatus::QKnxGeneralStatus()
    : QKnxGeneralStatus(Attributes())
{}

QKnxGeneralStatus::QKnxGeneralStatus(Attributes attributes)
    : QKnx8BitSet(SubType, 0)
{
    setDescription(tr("General Status"));
    setRange(QVariant(0x00), QVariant(0x1f));
    setValue(attributes);
}

QKnxGeneralStatus::Attributes QKnxGeneralStatus::value() const
{
    auto value = byte();
    return Attributes().setFlag(Attribute::OutOfService, QKnxDatapointType::testBit(value, 0))
        .setFlag(Attribute::Fault, QKnxDatapointType::testBit(value, 1))
        .setFlag(Attribute::Overridden, QKnxDatapointType::testBit(value, 2))
        .setFlag(Attribute::InAlarm, QKnxDatapointType::testBit(value, 3))
        .setFlag(Attribute::AlarmUnacknowledged, QKnxDatapointType::testBit(value, 4));
}

bool QKnxGeneralStatus::setValue(Attributes attributes)
{
    quint8 value = byte();
    QKnxDatapointType::setBit(&value, attributes.testFlag(Attribute::OutOfService), 0);
    QKnxDatapointType::setBit(&value, attributes.testFlag(Attribute::Fault), 1);
    QKnxDatapointType::setBit(&value, attributes.testFlag(Attribute::Overridden), 2);
    QKnxDatapointType::setBit(&value, attributes.testFlag(Attribute::InAlarm), 3);
    QKnxDatapointType::setBit(&value, attributes.testFlag(Attribute::AlarmUnacknowledged), 4);
    return setByte(value);
}

bool QKnxGeneralStatus::isSet(Attribute attribute) const
{
    return value().testFlag(attribute);
}

bool QKnxGeneralStatus::setAttribute(Attribute attribute)
{
    return setValue(value() | attribute);
}

bool QKnxGeneralStatus::removeAttribute(Attribute attribute)
{
    return setValue(value() &~ attribute);
}


// -- QKnxDeviceControl

QKnxDeviceControl::QKnxDeviceControl()
    : QKnxDeviceControl(Attributes())
{}

QKnxDeviceControl::QKnxDeviceControl(Attributes attributes)
    : QKnx8BitSet(SubType, 0)
{
    setDescription(tr("Device Control"));
    setRange(QVariant(0x00), QVariant(0x15));
    setValue(attributes);
}

QKnxDeviceControl::Attributes QKnxDeviceControl::value() const
{
    auto value = byte();
    return Attributes().setFlag(Attribute::UserStopped, QKnxDatapointType::testBit(value, 0))
        .setFlag(Attribute::OwnIA, QKnxDatapointType::testBit(value, 1))
        .setFlag(Attribute::VerifyMode, QKnxDatapointType::testBit(value, 2))
        .setFlag(Attribute::SafeState, QKnxDatapointType::testBit(value, 3));
}

bool QKnxDeviceControl::setValue(Attributes attributes)
{
    quint8 value = byte();
    QKnxDatapointType::setBit(&value, attributes.testFlag(Attribute::UserStopped), 0);
    QKnxDatapointType::setBit(&value, attributes.testFlag(Attribute::OwnIA), 1);
    QKnxDatapointType::setBit(&value, attributes.testFlag(Attribute::VerifyMode), 2);
    QKnxDatapointType::setBit(&value, attributes.testFlag(Attribute::SafeState), 3);
    return setByte(value);
}

bool QKnxDeviceControl::isSet(Attribute attribute) const
{
    return value().testFlag(attribute);
}

bool QKnxDeviceControl::setAttribute(Attribute attribute)
{
    return setValue(value() | attribute);
}

bool QKnxDeviceControl::removeAttribute(Attribute attribute)
{
    return setValue(value() &~ attribute);
}

#include "moc_qknx8bitset.cpp"

QT_END_NAMESPACE
