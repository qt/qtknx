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

#include "qknx1bit.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE


// -- QKnx1Bit

QKnx1Bit::QKnx1Bit()
: QKnx1Bit(false)
{}

QKnx1Bit::QKnx1Bit(bool bit)
    : QKnx1Bit(SubType, bit)
{}

QKnx1Bit::QKnx1Bit(int subType, bool bit)
    : QKnxDatapointType(MainType, subType, TypeSize)
{
    setBit(bit);
    setDescription(tr("1-bit"));

    setRangeText(tr("true"), tr("false"));
    setRange(QVariant(0x00), QVariant(0x01));
}

bool QKnx1Bit::bit() const
{
    return QKnxDatapointType::testBit(byte(0), 0);
}

bool QKnx1Bit::setBit(bool value)
{
    return setByte(0, (value ? 0x01 : 0x00));
}

bool QKnx1Bit::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}


// -- QKnxSwitch

QKnxSwitch::QKnxSwitch()
    : QKnx1Bit(SubType, false)
{
    setDescription(tr("Switch"));
    setRangeText(tr("Off"), tr("On"));
}

QKnxSwitch::QKnxSwitch(State state)
    : QKnx1Bit(SubType, bool(state))
{}

QKnxSwitch::State QKnxSwitch::value() const
{
    return QKnxSwitch::State(QKnx1Bit::bit());
}

bool QKnxSwitch::setValue(State state)
{
    return QKnx1Bit::setBit(bool(state));
}

#include "moc_qknx1bit.cpp"

QT_END_NAMESPACE

