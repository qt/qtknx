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

#include "qknx4bytesignedvalue.h"
#include "qknxdatapointtype_p.h"

#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx4ByteSignedValue
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnx4ByteSignedValue class is a datapoint type with a 4-byte
    signed value.

    This is a fixed size datapoint type with the length of 4 bytes.

    Integer values from -2 147 483 648 to -2 147 483 647 can be encoded in this
    datapoint type.

    \sa QKnxDatapointType
*/

QKnx4ByteSignedValue::QKnx4ByteSignedValue()
    : QKnx4ByteSignedValue(0)
{}

QKnx4ByteSignedValue::QKnx4ByteSignedValue(qint32 value)
    : QKnx4ByteSignedValue(SubType, value)
{}

QKnx4ByteSignedValue::QKnx4ByteSignedValue(int subType, qint32 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("4-byte signed value"));
    setRange(QVariant::fromValue(INT_MIN), QVariant::fromValue(INT_MAX));
    setRangeText(tr("Minimum Value, -2 147 483 648"), tr("Maximum Value, 2 147 483 647"));

    setValue(value);
}

qint32 QKnx4ByteSignedValue::value() const
{
    return qint32(QKnxUtils::QUint32::fromBytes(bytes()));
}

bool QKnx4ByteSignedValue::setValue(qint32 value)
{
    if (value <= maximum().toInt() && value >= minimum().toInt())
        return setBytes(QKnxUtils::QUint32::bytes(quint32(value)), 0, 4);
    return false;
}

/*!
    \reimp
*/
bool QKnx4ByteSignedValue::isValid() const
{
    return QKnxDatapointType::isValid()
        && value() >= minimum().toInt() && value() <= maximum().toInt();
}

#define CREATE_CLASS_BODY(CLASS, DESCRIPTION, UNIT) \
CLASS::CLASS() \
    : QKnx4ByteSignedValue(SubType, 0) \
{ \
    setUnit(tr(UNIT)); \
    setDescription(tr(DESCRIPTION)); \
} \
CLASS::CLASS(qint32 value) \
    : CLASS() \
{ \
    setValue(value); \
}

CREATE_CLASS_BODY(QKnxValue4Count, "Counter pulses (signed)", "counter pulses");
CREATE_CLASS_BODY(QKnxFlowRateCubicMeterPerHour, "Flow rate (m3/h)", "m3/h");
CREATE_CLASS_BODY(QKnxActiveEnergy, "Active energy (Wh)", "Wh");
CREATE_CLASS_BODY(QKnxApparentEnergy, "Active energy (VAh)", "VAh");
CREATE_CLASS_BODY(QKnxReactiveEnergy, "Active energy (VARh)", "VARh");
CREATE_CLASS_BODY(QKnxActiveEnergykWh, "Active energy (kWh)", "kWh");
CREATE_CLASS_BODY(QKnxApparentEnergykVAh, "Active energy (kVAh)", "kVAh");
CREATE_CLASS_BODY(QKnxReactiveEnergykVARh, "Active energy (kVARh)", "kVARh");
CREATE_CLASS_BODY(QKnxLongDeltaTimeSec, "Active energy (sec)", "sec");

#undef CREATE_CLASS_BODY

QT_END_NAMESPACE
