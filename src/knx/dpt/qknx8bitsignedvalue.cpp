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

#include "qknx8bitsignedvalue.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

// -- QKnx8BitSignedValue

QKnx8BitSignedValue::QKnx8BitSignedValue()
    : QKnx8BitSignedValue(0)
{}

QKnx8BitSignedValue::QKnx8BitSignedValue(qint8 value)
    : QKnx8BitSignedValue(SubType, value)
{}

QKnx8BitSignedValue::QKnx8BitSignedValue(int subType, qint8 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setRangeText(tr("-128"), tr("127"));
    setRange(QVariant(0x00), QVariant(0xff));
    setDescription(tr("8-bit signed value"));

    setValue(value);
}

qint8 QKnx8BitSignedValue::value() const
{
    return qint8(QKnxDatapointType::byte(0));
}

void QKnx8BitSignedValue::setValue(qint8 value)
{
    QKnxDatapointType::setByte(0, quint8(value));
}

#define CREATE_CLASS_BODY(CLASS, DESCRIPTION, RANGE_TEXT_MINIMUM, RANGE_TEXT_MAXIMUM, UNIT, \
    RANGE_VALUE_MINIMUM, RANGE_VALUE_MAXIMUM) \
CLASS::CLASS() \
    : QKnx8BitSignedValue(SubType, 0) \
{ \
    setUnit(tr(UNIT)); \
    setDescription(tr(DESCRIPTION)); \
    setRangeText(tr(RANGE_TEXT_MINIMUM), tr(RANGE_TEXT_MAXIMUM)); \
    setRange(QVariant::fromValue(RANGE_VALUE_MINIMUM), QVariant::fromValue(RANGE_VALUE_MAXIMUM)); \
} \
CLASS::CLASS(qint8 value) \
    : CLASS() \
{ \
    setValue(value); \
}

CREATE_CLASS_BODY(QKnxPercentV8, "Percentage (-128% ... 127%)",
    "Minimum Value, -128", "Maximum Value, 127", "%", 0x00, 0xff)
CREATE_CLASS_BODY(QKnxValue1Count, "Counter pulses (-128 ... 127)",
    "Minimum Value, -128", "Maximum Value, 127", "counter pulses", 0x00, 0xff)

#undef CREATE_CLASS_BODY

QT_END_NAMESPACE
