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

#include "qknx8bitunsignedvalue.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx8BitUnsignedValue

    \inmodule QtKnx
    \brief The QKnx8BitUnsignedValue class is a datapoint type with an 8-bit
    unsigned value.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType
*/

// -- QKnx8BitUnsignedValue

QKnx8BitUnsignedValue::QKnx8BitUnsignedValue()
    : QKnx8BitUnsignedValue(0.0)
{}

QKnx8BitUnsignedValue::QKnx8BitUnsignedValue(double value)
    : QKnx8BitUnsignedValue(SubType, value)
{}

QKnx8BitUnsignedValue::QKnx8BitUnsignedValue(int subType, double value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("8-bit unsigned value"));
    setRange(QVariant(0x00), QVariant(0xff));
    setRangeText(tr("0"), tr("255"));

    setValue(value);
}

double QKnx8BitUnsignedValue::value() const
{
    if (!isValid())
        return -1;
    return byte(0) * coefficient();
}

bool QKnx8BitUnsignedValue::setValue(double value)
{
    if (value <= maximum().toDouble() && value >= minimum().toDouble())
        return setByte(0, quint8(qRound(value / coefficient())));
    return false;
}

/*!
    \reimp
*/
bool QKnxTariff::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) < 255;
}

#define CREATE_CLASS_BODY(CLASS, DESCRIPTION, RANGE_TEXT_MINIMUM, RANGE_TEXT_MAXIMUM, UNIT, \
    RANGE_VALUE_MINIMUM, RANGE_VALUE_MAXIMUM, COEFFICIENT) \
CLASS::CLASS() \
    : QKnx8BitUnsignedValue(SubType, 0.0) \
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

CREATE_CLASS_BODY(QKnxScaling, "Percentage (0..100%)",
    "Minimum Value, 0", "Maximum Value, 100", "percent", 0, 100, 100 / 255.)
CREATE_CLASS_BODY(QKnxAngle, "Angle (degrees)",
    "Minimum Value, 0", "Maximum Value, 360", "degree", 0, 360, 360 / 255.)
CREATE_CLASS_BODY(QKnxPercentU8, "Percentage (0..255%)",
    "Minimum Value, 0", "Maximum Value, 255", "percent", 0, 255, 1)
CREATE_CLASS_BODY(QKnxDecimalFactor, "Ratio (0...255)",
    "Minimum Value, 0", "Maximum Value, 255", "", 0, 255, 1)
CREATE_CLASS_BODY(QKnxValue1Ucount, "Counter Pulses",
    "Minimum Value, 0", "Maximum Value, 255", "counter pulses", 0, 255, 1)
CREATE_CLASS_BODY(QKnxTariff, "Tarif",
    "Minimum Value, 0", "Maximum Value, 254", "", 0, 254, 1)

#undef CREATE_CLASS_BODY

QT_END_NAMESPACE
