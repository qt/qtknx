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

#include "qknx2byteunsignedvalue.h"
#include "qknxdatapointtype_p.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx2ByteUnsignedValue
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnx2ByteUnsignedValue class is a datapoint type with a 2-byte
    unsigned value.

    This is a fixed size datapoint type with the length of 2 bytes.

    It is a base class for the following datapoint types:

    \list
        \li \c QKnxBrightness - Brightness in LUX
        \li \c QKnxLengthMilliMeter - Length in millimeters
        \li \c QKnxPropDataType - Identifier Interface Object Property data
            type (no unit)
        \li \c QKnxTimePeriod10Msec - Time in multiples of 10 milliseconds. Not
            allowed for runtime communication. This datapoint type shall only be
            used for parameters and diagnostic data or if specified as such in a
            functional block specification.
        \li \c QKnxTimePeriod100Msec - Time in multiples of 100 milliseconds.
            Not allowed for runtime communication.
        \li \c QKnxTimePeriodHrs - Time in hours
        \li \c QKnxTimePeriodMin - Time in minutes. Not allowed for runtime
            communication.
        \li \c QKnxTimePeriodMsec - Time in milliseconds
        \li \c QKnxTimePeriodSec - Time in seconds
        \li \c QKnxUEICurrentMilliA - Current in milliampere. The minimum value
            is \c 0, which means that no bus poser supply functionality is
            available.
        \li \c QKnxValue2Ucount - Pulses
    \endlist

    Integer values from 0 to 65 535 can be encoded in this datapoint type.

    \sa QKnxDatapointType
*/


// -- QKnx2ByteUnsignedValue

/*!
    Creates a fixed size datapoint type with the value \c 0.
*/
QKnx2ByteUnsignedValue::QKnx2ByteUnsignedValue()
    : QKnx2ByteUnsignedValue(0)
{}

/*!
    Creates a fixed size datapoint type with the value \a value.
*/
QKnx2ByteUnsignedValue::QKnx2ByteUnsignedValue(quint32 value)
    : QKnx2ByteUnsignedValue(SubType, value)
{}

/*!
    Creates a fixed size datapoint type with the subtype \a subType and value
    \a value.
*/
QKnx2ByteUnsignedValue::QKnx2ByteUnsignedValue(int subType, quint32 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("2-byte unsigned value"));
    setRange(QVariant(0x0000), QVariant(0xffff));
    setRangeText(tr("0"), tr("65535"));
    setValue(value);
}

/*!
    Returns the value stored in the datapoint type.
*/
quint32 QKnx2ByteUnsignedValue::value() const
{
    return quint32(QKnxUtils::QUint16::fromBytes(bytes()) * coefficient());
}

/*!
    Sets the value of the datapoint type to \a value.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnx2ByteUnsignedValue::setValue(quint32 value)
{
    if (value <= maximum().toUInt() && value >= minimum().toUInt())
        return setBytes(QKnxUtils::QUint16::bytes(quint16(qRound(value / coefficient()))), 0, 2);
    return false;
}

/*!
    \reimp
*/
bool QKnx2ByteUnsignedValue::isValid() const
{
    return QKnxDatapointType::isValid()
        && value() >= minimum().toUInt() && value() <= maximum().toUInt();
}

#define CREATE_CLASS_BODY(CLASS, DESCRIPTION, RANGE_TEXT_MINIMUM, RANGE_TEXT_MAXIMUM, UNIT, \
    RANGE_VALUE_MINIMUM, RANGE_VALUE_MAXIMUM, COEFFICIENT) \
CLASS::CLASS() \
    : QKnx2ByteUnsignedValue(SubType, 0) \
{ \
    setUnit(tr(UNIT)); \
    setCoefficient(COEFFICIENT); \
    setDescription(tr(DESCRIPTION)); \
    setRangeText(tr(RANGE_TEXT_MINIMUM), tr(RANGE_TEXT_MAXIMUM)); \
    setRange(QVariant::fromValue(RANGE_VALUE_MINIMUM), QVariant::fromValue(RANGE_VALUE_MAXIMUM)); \
} \
CLASS::CLASS(quint32 value) \
    : CLASS() \
{ \
    setValue(value); \
}

CREATE_CLASS_BODY(QKnxValue2Ucount, "Pulses",
    "Minimum Value, 0", "Maximum Value, 65535", "pulse", 0, 65535, 1.)
CREATE_CLASS_BODY(QKnxPropDataType, "Property Data Type",
    "Minimum Value, 0", "Maximum Value, 65535", "", 0, 65535, 1.)
CREATE_CLASS_BODY(QKnxTimePeriodMsec, "Time (ms)",
    "Minimum Value, 0", "Maximum Value, 65535", "ms", 0, 65535, 1.)
CREATE_CLASS_BODY(QKnxTimePeriod10Msec, "Time (multiple of 10ms)",
    "Minimum Value, 0", "Maximum Value, 655350", "ms", 0, 655350, 655350/65535.)
CREATE_CLASS_BODY(QKnxTimePeriod100Msec, "Time (multiple of 100ms)",
    "Minimum Value, 0", "Maximum Value, 6553500", "ms", 0, 6553500, 6553500/65535.)
CREATE_CLASS_BODY(QKnxTimePeriodSec, "Time (s)",
    "Minimum Value, 0", "Maximum Value, 65535", "s", 0, 65535, 1.)
CREATE_CLASS_BODY(QKnxTimePeriodMin, "Time (min)",
    "Minimum Value, 0", "Maximum Value, 65535", "min", 0, 65535, 1.)
CREATE_CLASS_BODY(QKnxTimePeriodHrs, "Time (h)",
    "Minimum Value, 0", "Maximum Value, 65535", "h", 0, 65535, 1.)
CREATE_CLASS_BODY(QKnxLengthMilliMeter, "Length (mm)",
    "Minimum Value, 0", "Maximum Value, 65535", "mm", 0, 65535, 1.)
CREATE_CLASS_BODY(QKnxUEICurrentMilliA, "Current (mA)",
    "Minimum Value, 0 (no bus poser supply functionality available)", "Maximum Value, 65535", "mA",
    0, 65535, 1.)
CREATE_CLASS_BODY(QKnxBrightness, "Brightness (lux)",
    "Minimum Value, 0", "Maximum Value, 65535", "lux", 0, 65535, 1.)

#undef CREATE_CLASS_BODY

QT_END_NAMESPACE
