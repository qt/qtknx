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
#include "qknx1bitcontrolled.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

// -- QKnx1BitControlled

QKnx1BitControlled::QKnx1BitControlled()
    : QKnx1BitControlled(SubType, false, false)
{}

QKnx1BitControlled::QKnx1BitControlled(int subType, bool state, bool control)
    : QKnxDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("1-bit controlled"));

    setRange(QVariant(0x00), QVariant(0x03));
    setRangeText(tr("No control, false"), tr("Controlled, true"));
    setValueBit(state);
    setControlBit(control);
}

QKnx1BitControlled::QKnx1BitControlled(int subType, const QKnxDatapointType &dpt, bool state,
        bool control)
    : QKnx1BitControlled(subType, state, control)
{
    setMinimumText(tr("No control, %1").arg(dpt.minimumText()));
    setMaximumText(tr("Controlled, %1").arg(dpt.maximumText()));
}

bool QKnx1BitControlled::valueBit() const
{
    return QKnxDatapointType::testBit(byte(0), 0);
}

void QKnx1BitControlled::setValueBit(bool value)
{
    QKnxDatapointType::setBit(data(), value, 0);
}

bool QKnx1BitControlled::controlBit() const
{
    return QKnxDatapointType::testBit(byte(0), 1);
}

void QKnx1BitControlled::setControlBit(bool control)
{
    QKnxDatapointType::setBit(data(), control, 1);
}

bool QKnx1BitControlled::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}

#define CREATE_CLASS_BODY(CLASS, CLASS1, DESCRIPTION) \
CLASS::CLASS() \
    : CLASS(State(0), Control::NoControl) \
{} \
CLASS::CLASS(State state, Control control) \
    : QKnx1BitControlled(SubType, CLASS1(), bool(state), bool(control)) \
{ \
    setDescription(tr(DESCRIPTION)); \
} \
CLASS::State CLASS::state() const \
{ \
    return CLASS::State(valueBit()); \
} \
void CLASS::setState(State state) \
{ \
    setValueBit(bool(state)); \
} \
CLASS::Control CLASS::control() const \
{ \
    return CLASS::Control(controlBit()); \
} \
void CLASS::setControl(Control control) \
{ \
    setControlBit(bool(control)); \
} \
void CLASS::setValue(State state, Control control) \
{ \
    setValueBit(bool(state)); \
    setControlBit(bool(control)); \
}

CREATE_CLASS_BODY(QKnxSwitchControl, QKnxSwitch, "Switch control")
CREATE_CLASS_BODY(QKnxBoolControl, QKnxBool, "Boolean control")
CREATE_CLASS_BODY(QKnxEnableControl, QKnxEnable, "Enable control")
CREATE_CLASS_BODY(QKnxRampControl, QKnxRamp, "Ramp control")
CREATE_CLASS_BODY(QKnxAlarmControl, QKnxAlarm, "Alarm control")
CREATE_CLASS_BODY(QKnxBinaryValueControl, QKnxBinaryValue, "Binary value control")
CREATE_CLASS_BODY(QKnxStepControl, QKnxStep, "Step control")
CREATE_CLASS_BODY(QKnxDirection1Control, QKnxUpDown, "Direction control 1")
CREATE_CLASS_BODY(QKnxDirection2Control, QKnxOpenClose, "Direction control 2")
CREATE_CLASS_BODY(QKnxStartControl, QKnxStart, "Start control")
CREATE_CLASS_BODY(QKnxStateControl, QKnxState, "State control")
CREATE_CLASS_BODY(QKnxInvertControl, QKnxInvert, "Invert control")

#undef CREATE_CLASS_BODY

#include "moc_qknx1bitcontrolled.cpp"

QT_END_NAMESPACE
