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
    return testBit(operator[](0), 0);
}

void QKnx1Bit::setBit(bool value)
{
    operator[](0) = (value ? 0x01 : 0x00);
}

bool QKnx1Bit::isValid() const
{
    return mainType() == MainType && size() == TypeSize && operator[](0) <= maximum().toUInt();
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

void QKnxSwitch::setValue(State state)
{
    QKnx1Bit::setBit(bool(state));
}

#include "moc_qknx1bit.cpp"

QT_END_NAMESPACE

