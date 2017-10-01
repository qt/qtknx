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

#include "qknx3bitcontrolled.h"
#include "qknxdatapointtype_p.h"
#include "qmath.h"

QT_BEGIN_NAMESPACE

// -- QKnx3BitControlled

QKnx3BitControlled::QKnx3BitControlled()
    : QKnx3BitControlled(SubType)
{}

QKnx3BitControlled::QKnx3BitControlled(int subType)
    : QKnxDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("3-bit controlled"));
    setRange(QVariant(0x00), QVariant(0x07));
    setRangeText(tr("No control, Break"), tr("Controlled, 32 intervals"));
}

bool QKnx3BitControlled::controlBit() const
{
    return QKnxDatapointType::testBit(byte(0), 3);
}

void QKnx3BitControlled::setControlBit(bool value)
{
    QKnxDatapointType::setBit(&(operator[](0)), value, 3);
}

bool QKnx3BitControlled::setNumberOfIntervals(NumberOfIntervals n)
{
    if (n > NumberOfIntervals::ThirtyTwo)
        return false;

    quint8 stepCode = 0;
    if (n != NumberOfIntervals::Break)
        stepCode = quint8(1 + qLn(quint8(n)) / qLn(2.));
    setByte(0, (byte(0) & 0x08) | stepCode);
    return true;
}

QKnx3BitControlled::NumberOfIntervals QKnx3BitControlled::numberOfIntervals() const
{
    quint8 stepCode = quint8(byte(0) & 0x07);
    return NumberOfIntervals(quint8(qPow(2, stepCode - 1)));
}

bool QKnx3BitControlled::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}


// -- QKnxControlDimming

QKnxControlDimming::QKnxControlDimming()
    : QKnx3BitControlled(SubType)
{
    setDescription(tr("Control Dimming"));
    setRange(QVariant(0x00), QVariant(0x07));
    setRangeText(tr("Decrease, Break"), tr("Increase, 32 intervals"));
}

QKnxControlDimming::QKnxControlDimming(Control control, NumberOfIntervals interval)
    : QKnxControlDimming()
{
    setControl(control);
    setNumberOfIntervals(interval);
}

void QKnxControlDimming::setControl(Control control)
{
    QKnx3BitControlled::setControlBit(bool(control));
}

QKnxControlDimming::Control QKnxControlDimming::control() const
{
    return Control(QKnx3BitControlled::controlBit());
}


// -- QKnxControlBlinds

QKnxControlBlinds::QKnxControlBlinds()
    : QKnx3BitControlled(SubType)
{
    setDescription(tr("Control Blinds"));
    setRange(QVariant(0x00), QVariant(0x07));
    setRangeText(tr("Up, Break"), tr("Down, 32 intervals"));
}

QKnxControlBlinds::QKnxControlBlinds(Control control, NumberOfIntervals interval)
    : QKnxControlBlinds()
{
    setControl(control);
    setNumberOfIntervals(interval);
}

void QKnxControlBlinds::setControl(Control control)
{
    QKnx3BitControlled::setControlBit(bool(control));
}

QKnxControlBlinds::Control QKnxControlBlinds::control() const
{
    return Control(QKnx3BitControlled::controlBit());
}

#include "moc_qknx3bitcontrolled.cpp"

QT_END_NAMESPACE
