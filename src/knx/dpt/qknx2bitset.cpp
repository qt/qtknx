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

#include "qknx2bitset.h"
#include "qknxdatapointtype_p.h"
#include "qmath.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx2BitSet
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnx2BitSet class is a fixed size datapoint type with the length
    of 2 bits.

    The range for the value is from \c 0x00 to \c 0x03.

    \sa QKnxDatapointType, QKnxAlarmReaction, QKnxOnOffAction, QKnxUpDownAction,
        {Qt KNX Datapoint Type Classes}
*/

// -- QKnx2BitSet

/*!
    Creates a fixed size datapoint type with the 2-bit set to \c 0.
*/
QKnx2BitSet::QKnx2BitSet()
    : QKnx2BitSet(0)
{}

/*!
    Creates a fixed size datapoint type with the value \a value.
*/
QKnx2BitSet::QKnx2BitSet(quint8 value)
    : QKnx2BitSet(SubType, value)
{}

/*!
    Creates a fixed size datapoint with the subtype \a subType and the value
    \a value.
*/
QKnx2BitSet::QKnx2BitSet(int subType, quint8 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("2-bit set"));
    setRange(QVariant(0x00), QVariant(0x03));
    setValue(value);
}

/*!
    Returns the value stored in the datapoint type.
*/
quint8 QKnx2BitSet::value() const
{
    return byte(0) & 0x03;
}

/*!
    Sets the value stored in the datapoint type to \a value.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnx2BitSet::setValue(quint8 value)
{
    if (value <= maximum().toUInt())
        return setByte(0, value);
    return false;
}

/*!
    \reimp
*/
bool QKnx2BitSet::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}


// -- QKnxOnOffAction

/*!
    \class QKnxOnOffAction
    \inherits QKnx2BitSet
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxOnOffAction class is a fixed size datapoint type for storing
    the on/off action.

    The range for the \l{Action}{action} value is from \c {Minimum Off, 0} to
    \c {Maximum On/Off, 3}.

    This is a fixed size datapoint type with the length of 2 bits.

    \sa QKnxDatapointType, QKnxAlarmReaction, QKnxUpDownAction,
        {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnxOnOffAction::Action

    This enum holds the action stored in the datapoint type.

    \value Off
    \value On
    \value OffOn
    \value OnOff
*/

/*!
    Creates a fixed size datapoint type with the action set to \c Off.
*/
QKnxOnOffAction::QKnxOnOffAction()
    : QKnxOnOffAction(Action::Off)
{}

/*!
    Creates a fixed size datapoint type with the action set to \a action.
*/
QKnxOnOffAction::QKnxOnOffAction(Action action)
    : QKnx2BitSet(SubType, quint8(action))
{
    setDescription(tr("On/Off Action"));
    setRangeText(tr("Minimum Off, 0"), tr("Maximum On/Off, 3"));
}

/*!
    Sets the action stored in the datapoint type to \a action.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxOnOffAction::setAction(Action action)
{
    return setValue(quint8(action));
}

/*!
    Returns the action stored in the datapoint type.
*/
QKnxOnOffAction::Action QKnxOnOffAction::action() const
{
    return Action(value());
}


// -- QKnxAlarmReaction

/*!
    \class QKnxAlarmReaction
    \inherits QKnx2BitSet
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxAlarmReaction class is a fixed size datapoint type for
    storing the alarm reaction.

    The range for the \l{Alarm}{alarm} value is from \c {No alarm is used, 0} to
    \c {Alarm position is down, 2}.

    This is a fixed size datapoint type with the length of 2 bits.

    \sa QKnxDatapointType, QKnxOnOffAction, QKnxUpDownAction,
        {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnxAlarmReaction::Alarm

    This enum holds the alarm reaction stored in the datapoint type.

    \value NoAlarm
           No alarm is used.
    \value AlarmUp
           Alarm position is up.
    \value AlarmDown
           Alarm position is down.
*/

/*!
    Creates a fixed size datapoint type with the alarm reaction set to
    \c NoAlarm.
*/
QKnxAlarmReaction::QKnxAlarmReaction()
    : QKnxAlarmReaction(Alarm::NoAlarm)
{}

/*!
    Creates a fixed size datapoint type with the alarm reaction set to
    \a alarm.
*/
QKnxAlarmReaction::QKnxAlarmReaction(Alarm alarm)
    : QKnx2BitSet(SubType, quint8(alarm))
{
    setDescription(tr("Alarm reaction"));
    setRangeText(tr("No alarm is used, 0"), tr("Alarm position is down, 2"));
    setRange(QVariant(0x00), QVariant(0x02));
    setAlarm(alarm);
}

/*!
    Sets the alarm reaction stored in the datapoint type to \a alarm.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxAlarmReaction::setAlarm(Alarm alarm)
{
    return setValue(quint8(alarm));
}

/*!
    Returns the alarm reaction stored in the datapoint type.
*/
QKnxAlarmReaction::Alarm QKnxAlarmReaction::alarm() const
{
    auto tmp = value();
    return Alarm(QKnxDatapointType::testBit(tmp, 1) ? tmp & 0x02 : tmp);
}


// -- QKnxUpDownAction

/*!
    \class QKnxUpDownAction
    \inherits QKnx2BitSet
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxOnOffAction class is a fixed size datapoint type for storing
    the up/down action.

    The range for the \l{Action}{action} value is from \c {Minimum Up, 0} to
    \c {Maximum Down/Up, 3}.

    This is a fixed size datapoint type with the length of 2 bits.

    \sa QKnxDatapointType, QKnxAlarmReaction, QKnxOnOffAction,
        {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnxUpDownAction::Action

    This enum holds the action stored in the datapoint type.

    \value Up
    \value Down
    \value UpDown
    \value DownUp
*/

/*!
    Creates a fixed size datapoint type with the action set to \c Up.
*/
QKnxUpDownAction::QKnxUpDownAction()
    : QKnxUpDownAction(Action::Up)
{}

/*!
    Creates a fixed size datapoint type with the action set to \a action.
*/
QKnxUpDownAction::QKnxUpDownAction(Action action)
    : QKnx2BitSet(SubType, quint8(action))
{
    setDescription(tr("Up/Down Action"));
    setRangeText(tr("Minimum Up, 0"), tr("Maximum Down/Up, 3"));
}

/*!
    Sets the action stored in the datapoint type to \a action.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxUpDownAction::setAction(Action action)
{
    return setValue(quint8(action));
}

/*!
    Returns the action stored in the datapoint type.
*/
QKnxUpDownAction::Action QKnxUpDownAction::action() const
{
    return Action(value());
}

#include "moc_qknx2bitset.cpp"

QT_END_NAMESPACE
