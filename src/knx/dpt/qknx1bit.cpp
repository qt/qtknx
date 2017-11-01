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
    setDescription(tr("1-bit"));
    setRangeText(tr("true"), tr("false"));
    setRange(QVariant(0x00), QVariant(0x01));

    setBit(bit);
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

#define CREATE_CLASS_BODY(CLASS, DESCRIPTION, RANGE_TEXT_MINIMUM, RANGE_TEXT_MAXIMUM) \
CLASS::CLASS() \
    : CLASS(State(0)) \
{} \
CLASS::CLASS(State state) \
    : QKnx1Bit(SubType, bool(state)) \
{ \
    setDescription(tr(DESCRIPTION)); \
    setRangeText(tr(RANGE_TEXT_MINIMUM), tr(RANGE_TEXT_MAXIMUM)); \
} \
CLASS::State CLASS::value() const \
{ \
    return CLASS::State(QKnx1Bit::bit()); \
} \
bool CLASS::setValue(State state) \
{ \
    return QKnx1Bit::setBit(bool(state)); \
} \

CREATE_CLASS_BODY(QKnxSwitch, "Switch", "Off", "On")
CREATE_CLASS_BODY(QKnxBool, "Boolean", "False", "True")
CREATE_CLASS_BODY(QKnxEnable, "Enable", "Disable", "Enable")
CREATE_CLASS_BODY(QKnxRamp, "Ramp", "NoRamp", "Ramp")
CREATE_CLASS_BODY(QKnxAlarm, "Alarm", "NoAlarm", "Alarm")
CREATE_CLASS_BODY(QKnxBinaryValue, "Binary value", "Low", "High")
CREATE_CLASS_BODY(QKnxStep, "Step", "Decrease", "Increase")
CREATE_CLASS_BODY(QKnxUpDown, "Up/Down", "Up", "Down")
CREATE_CLASS_BODY(QKnxOpenClose, "Open/Close", "Open", "Close")
CREATE_CLASS_BODY(QKnxStart, "Start/Stop", "Stop", "Start")
CREATE_CLASS_BODY(QKnxState, "State", "Inactive", "Active")
CREATE_CLASS_BODY(QKnxInvert, "Invert", "Not inverted", "Inverted")
CREATE_CLASS_BODY(QKnxDimSendStyle, "Dim send style", "Start/Stop", "Cyclically")
CREATE_CLASS_BODY(QKnxInputSource, "Input source", "Fixed", "Calculated")
CREATE_CLASS_BODY(QKnxReset, "Reset", "No action (dummy)", "Reset command (trigger)")
CREATE_CLASS_BODY(QKnxAck, "Acknowledge", "No action (dummy)",
    "Acknowledge command (trigger) e.g. for alarming")
CREATE_CLASS_BODY(QKnxTrigger, "Trigger", "Trigger", "Trigger (also)")
CREATE_CLASS_BODY(QKnxOccupancy, "Occupancy", "Not occupied", "Occupied")
CREATE_CLASS_BODY(QKnxWindowDoor, "Window/Door", "Closed", "Open")
CREATE_CLASS_BODY(QKnxLogicalFunction, "Logical function", "logical function OR",
    "logical function AND")
CREATE_CLASS_BODY(QKnxSceneAB, "Scene", "Scene A", "Scene B")
CREATE_CLASS_BODY(QKnxShutterBlindsMode, "Shutter/Blinds mode", "Only move Up/Down mode (shutter)",
    "move up down + StepStop mode (blind)")
CREATE_CLASS_BODY(QKnxHeatCool, "Cooling/Heating", "Cooling", "Heating")

#undef CREATE_CLASS_BODY

#include "moc_qknx1bit.cpp"

QT_END_NAMESPACE

