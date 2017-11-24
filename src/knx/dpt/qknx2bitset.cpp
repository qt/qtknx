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

#include "qknx2bitset.h"
#include "qknxdatapointtype_p.h"
#include "qmath.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx2BitSet
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \brief The QKnx2BitSet class is a fixed size datapoint type with the length
    of 2 bits.

    \sa QKnxDatapointType
*/

// -- QKnx2BitSet

QKnx2BitSet::QKnx2BitSet()
    : QKnx2BitSet(0)
{}

QKnx2BitSet::QKnx2BitSet(quint8 value)
    : QKnx2BitSet(SubType, value)
{}

QKnx2BitSet::QKnx2BitSet(int subType, quint8 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("2-bit set"));
    setRange(QVariant(0x00), QVariant(0x03));
    setValue(value);
}

quint8 QKnx2BitSet::value() const
{
    return byte(0) & 0x03;
}

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

QKnxOnOffAction::QKnxOnOffAction()
    : QKnxOnOffAction(Action::Off)
{}

QKnxOnOffAction::QKnxOnOffAction(Action action)
    : QKnx2BitSet(SubType, quint8(action))
{
    setDescription(tr("On/Off Action"));
    setRangeText(tr("Minimum Off, 0"), tr("Maximum On/Off, 3"));
}

bool QKnxOnOffAction::setAction(Action action)
{
    return setValue(quint8(action));
}

QKnxOnOffAction::Action QKnxOnOffAction::action() const
{
    return Action(value());
}


// -- QKnxAlarmReaction

QKnxAlarmReaction::QKnxAlarmReaction()
    : QKnxAlarmReaction(Alarm::NoAlarm)
{}

QKnxAlarmReaction::QKnxAlarmReaction(Alarm alarm)
    : QKnx2BitSet(SubType, quint8(alarm))
{
    setDescription(tr("Alarm reaction"));
    setRangeText(tr("No alarm is used, 0"), tr("Alarm position is down, 2"));
    setRange(QVariant(0x00), QVariant(0x02));
    setAlarm(alarm);
}

bool QKnxAlarmReaction::setAlarm(Alarm alarm)
{
    return setValue(quint8(alarm));
}

QKnxAlarmReaction::Alarm QKnxAlarmReaction::alarm() const
{
    auto tmp = value();
    return Alarm(QKnxDatapointType::testBit(tmp, 1) ? tmp & 0x02 : tmp);
}


// -- QKnxUpDownAction

QKnxUpDownAction::QKnxUpDownAction()
    : QKnxUpDownAction(Action::Up)
{}

QKnxUpDownAction::QKnxUpDownAction(Action action)
    : QKnx2BitSet(SubType, quint8(action))
{
    setDescription(tr("Up/Down Action"));
    setRangeText(tr("Minimum Up, 0"), tr("Maximum Down/Up, 3"));
}

bool QKnxUpDownAction::setAction(Action action)
{
    return setValue(quint8(action));
}

QKnxUpDownAction::Action QKnxUpDownAction::action() const
{
    return Action(value());
}

#include "moc_qknx2bitset.cpp"

QT_END_NAMESPACE
