/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

/*!
    \class QKnx1Bit
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnx1Bit class is a boolean datapoint type, such as switch on or
    off.

    This is a fixed size datapoint type with the length of 1 byte, though only
    one bit is used by the actual implementation.

    It is a base class for the following datapoint types:

    \list
        \li \c QKnxAck - Acknowledge command (trigger) for alarms, for example.
            Possible values are \c Acknowledge and \c {No action (dummy)}.
        \li \c QKnxAlarm - Possible values are \c Alarm and \c NoAlarm.
        \li \c QKnxBinaryValue - Possible values are \c Low and \c High.
        \li \c QKnxBool - Possible values are \c False and \c True.
        \li \c QKnxDimSendStyle - Dim send style. Possible values are
            \c Start/Stop or \c Cyclically.
        \li \c QKnxEnable - Possible values are  \c Disable and \c Enable.
        \li \c QKnxHeatCool - Possible values are \c Cooling and \c Heating.
        \li \c QKnxInputSource - Possible values are \c Fixed and \c Calculated.
        \li \c QKnxInvert - Possible values are \c {Not inverted} and
            \c Inverted.
        \li \c QKnxLogicalFunction - Possible values are
            \c {logical function OR} and \c {logical function AND}.
        \li \c QKnxOccupancy - Possible values are \c {Not occupied} and
            \c Occupied.
        \li \c QKnxOpenClose - Possible values are \c Open and \c Close.
        \li \c QKnxRamp - Possible values are \c NoRamp and \c Ramp.
        \li \c QKnxReset - Possible values are \c {No action (dummy)} and
            \c {Reset command (trigger)}.
        \li \c QKnxSceneAB - Possible values are \c {Scene A} and \c {Scene B}.
        \li \c QKnxShutterBlindsMode - Shutter or blinds mode. Possible values
            are \c {Only move Up/Down mode (shutter)} and
            \c {Move up down + StepStop mode (blind)}.
        \li \c QKnxStart - Possible values are \c Stop and \c Start.
        \li \c QKnxState - Possible values are \c Inactive and \c Active.
        \li \c QKnxStep - Possible values are \c Decrease and \c Increase.
        \li \c QKnxSwitch - Possible values are \c Off and \c On.
        \li \c QKnxTrigger - Possible values are \c Trigger and
            \c {Trigger (also)}.
        \li \c QKnxUpDown - Possible values are \c Up and \c Down.
        \li \c QKnxWindowDoor - Window or door. Possible values are \c Closed
            and \c Open.
    \endlist

    The possible values for this datapoint type are \c true and \c false.

    \sa QKnxDatapointType
*/

// -- QKnx1Bit

/*!
    Creates a fixed size datapoint type with the value set to \c false.
*/
QKnx1Bit::QKnx1Bit()
    : QKnx1Bit(false)
{}

/*!
    Creates a fixed size datapoint type with the value \a bit.
*/
QKnx1Bit::QKnx1Bit(bool bit)
    : QKnx1Bit(SubType, bit)
{}

/*!
    Creates a fixed size datapoint with the subtype \a subType and the value
    \a bit.
*/
QKnx1Bit::QKnx1Bit(int subType, bool bit)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("1-bit"));
    setRangeText(tr("true"), tr("false"));
    setRange(QVariant(0x00), QVariant(0x01));

    setBit(bit);
}

/*!
    Returns the value stored in the datapoint type.
*/
bool QKnx1Bit::bit() const
{
    return QKnxDatapointType::testBit(byte(0), 0);
}

/*!
    Sets the bit stored in the datapoint type to \a value.

    Returns \c true if the bit was set; otherwise returns \c false.
*/
bool QKnx1Bit::setBit(bool value)
{
    return setByte(0, (value ? 0x01 : 0x00));
}

/*!
    \reimp
*/
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

