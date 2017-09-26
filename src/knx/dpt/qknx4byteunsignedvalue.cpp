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

#include "qknx4byteunsignedvalue.h"
#include "qknxdatapointtype_p.h"

#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnx4ByteUnsignedValue::QKnx4ByteUnsignedValue()
    : QKnx4ByteUnsignedValue(0)
{}

QKnx4ByteUnsignedValue::QKnx4ByteUnsignedValue(quint32 value)
    : QKnx4ByteUnsignedValue(SubType, value)
{}

QKnx4ByteUnsignedValue::QKnx4ByteUnsignedValue(int subType, quint32 value)
    : QKnxDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("4-byte unsigned value"));
    setRangeText(tr("Minimum Value, 0"), tr("Maximum Value, 4 294 967 295"));
    setRange(QVariant::fromValue(0), QVariant::fromValue(4294967295));
    setValue(value);
}

QKnxValue4UCount::QKnxValue4UCount()
    : QKnxValue4UCount(0)
{}

QKnxValue4UCount::QKnxValue4UCount(quint32 value)
    : QKnx4ByteUnsignedValue(SubType, value)
{
    setUnit(tr("counter pulses"));
}

quint32 QKnx4ByteUnsignedValue::value() const
{
    return QKnxUtils::QUint32::fromBytes(bytes());
}

bool QKnx4ByteUnsignedValue::setValue(quint32 value)
{
    if (value <= maximum().toUInt() && value >= minimum().toUInt())
        return setBytes(QKnxUtils::QUint32::bytes(value), 0, 4);
    return false;
}

bool QKnx4ByteUnsignedValue::isValid() const
{
    return QKnxDatapointType::isValid()
        && value() >= minimum().toUInt() && value() <= maximum().toUInt();
}

QT_END_NAMESPACE
