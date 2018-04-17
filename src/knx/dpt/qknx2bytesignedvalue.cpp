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

#include "qknx2bytesignedvalue.h"
#include "qknxdatapointtype_p.h"

#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx2ByteSignedValue
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnx2ByteSignedValue class is a datapoint type with a 2-byte
    signed value.

    This is a fixed size datapoint type with the length of 2 bytes.

    It is a base class for the following classes:

    \list
        \li \c QKnxDeltaTime10Msec - Time lag in multiples of 10 milliseconds.
            Not allowed for runtime communication. This datapoint type shall
            only be used for parameters and diagnostic data or if specified as
            such in a functional block specification.
        \li \c QKnxDeltaTime100Msec - Time lag in multiples of 100 milliseconds.
            Not allowed for runtime communication.
        \li \c QKnxDeltaTimeHrs - Time lag in hours
        \li \c QKnxDeltaTimeMin - Time lag in minutes. Not allowed for runtime
            communication.
        \li \c QKnxDeltaTimeMsec - Time lag in milliseconds
        \li \c QKnxDeltaTimeSec - Time lag in seconds
        \li \c QKnxPercentV16 - Percentage difference
        \li \c QKnxRotationAngle - Rotation angle in degrees
        \li \c QKnxValue2Count - Pulse difference
    \endlist

    Integer values from -32 768 to 32 767 can be encoded in this datapoint type.

    \sa QKnxDatapointType
*/

/*!
    Creates a fixed size datapoint type with the value \c 0.0.
*/
QKnx2ByteSignedValue::QKnx2ByteSignedValue()
    : QKnx2ByteSignedValue(0.0)
{}

/*!
    Creates a fixed size datapoint type with the value \a value.
*/
QKnx2ByteSignedValue::QKnx2ByteSignedValue(double value)
    : QKnx2ByteSignedValue(SubType, value)
{}

/*!
    Creates a fixed size datapoint type with the subtype \a subType and value
    \a value.
*/
QKnx2ByteSignedValue::QKnx2ByteSignedValue(int subType, double value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("2-byte signed value"));
    setRangeText(tr("Minimum Value, -32 768"), tr("Maximum Value, 32 767"));
    setRange(QVariant::fromValue(-32768), QVariant::fromValue(32767));

    setValue(value);
}

/*!
    Returns the value stored in the datapoint type.
*/
double QKnx2ByteSignedValue::value() const
{
     return qint16(QKnxUtils::QUint16::fromBytes(bytes())) * coefficient();
}

/*!
    Sets the value of the datapoint type to \a value.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnx2ByteSignedValue::setValue(double value)
{
    if (value <= maximum().toDouble() && value >= minimum().toDouble())
        return setBytes(QKnxUtils::QUint16::bytes(quint16(qRound(value / coefficient()))), 0, 2);
    return false;
}

/*!
    \reimp
*/
bool QKnx2ByteSignedValue::isValid() const
{
    return QKnxDatapointType::isValid()
        && value() >= minimum().toDouble() && value() <= maximum().toDouble();
}

#define CREATE_CLASS_BODY(CLASS, DESCRIPTION, RANGE_TEXT_MINIMUM, RANGE_TEXT_MAXIMUM, UNIT, \
    RANGE_VALUE_MINIMUM, RANGE_VALUE_MAXIMUM, COEFFICIENT) \
CLASS::CLASS() \
    : QKnx2ByteSignedValue(SubType, 0.0) \
{ \
    setUnit(tr(UNIT)); \
    setCoefficient(COEFFICIENT); \
    setDescription(tr(DESCRIPTION)); \
    setRangeText(tr(RANGE_TEXT_MINIMUM), tr(RANGE_TEXT_MAXIMUM)); \
    setRange(QVariant::fromValue(RANGE_VALUE_MINIMUM), QVariant::fromValue(RANGE_VALUE_MAXIMUM)); \
} \
CLASS::CLASS(double value) \
    : CLASS() \
{ \
    setValue(value); \
}

CREATE_CLASS_BODY(QKnxValue2Count, "Pulses difference",
    "Minimum Value, -32 768", "Maximum Value, 32 767","pulse", -32768, 32767, 1.)
CREATE_CLASS_BODY(QKnxPercentV16, "Percentage difference",
    "Minimum Value, -327,68", "Maximum Value, 327,67", "percent", -327.68, 327.67,
    327.67/32767)
CREATE_CLASS_BODY(QKnxDeltaTimeMsec, "Time lag (ms)",
    "Minimum Value, -32 768", "Maximum Value, 32 767", "ms", -32768, 32767, 1.)
CREATE_CLASS_BODY(QKnxDeltaTime10Msec, "Time lag (10 ms)",
    "Minimum Value, -32 7680", "Maximum Value, 32 7670", "ms", -327680, 327670, 327670/32767.)
CREATE_CLASS_BODY(QKnxDeltaTime100Msec, "Time lag (100 ms)",
    "Minimum Value, -32 76800", "Maximum Value, 32 76700", "ms", -3276800, 3276700, 3276700/32767.)
CREATE_CLASS_BODY(QKnxDeltaTimeSec, "Time lag (s)",
    "Minimum Value, -32 768", "Maximum Value, 32 767", "s", -32768, 32767, 1.)
CREATE_CLASS_BODY(QKnxDeltaTimeMin, "Time lag (min)",
    "Minimum Value, -32 768", "Maximum Value, 32 767", "min", -32768, 32767, 1.)
CREATE_CLASS_BODY(QKnxDeltaTimeHrs, "Time lag (hrs)",
    "Minimum Value, -32 768", "Maximum Value, 32 767", "hrs", -32768, 32767, 1.)
CREATE_CLASS_BODY(QKnxRotationAngle, "Rotation angle (degree)",
    "Minimum Value, -32 768", "Maximum Value, 32 767", "degree", -32768, 32767, 1.)

#undef CREATE_CLASS_BODY

QT_END_NAMESPACE
