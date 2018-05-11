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
#include "qknx1bitcontrolled.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx1BitControlled
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnx1BitControlled class is a boolean datapoint type with a
    control part.

    This is a fixed size datapoint type with the length of 1 byte, though only
    two bits are used by the actual implementation.

    The right side bit corresponds to the \l QKnx1Bit datapoint type value,
    whereas the left side bit represents the control part. For example, switch
    with priority control.

    It is a base class for the following datapoint types:

    \list
        \li \c QKnxAlarmControl
        \li \c QKnxBinaryValueControl
        \li \c QKnxBoolControl
        \li \c QKnxDirection1Control - \c QKnxUpDown with a control part.
        \li \c QKnxDirection2Control - \c QKnxOpenClose with a control part.
        \li \c QKnxEnableControl
        \li \c QKnxInvertControl
        \li \c QKnxRampControl
        \li \c QKnxStartControl
        \li \c QKnxStateControl
        \li \c QKnxStepControl
        \li \c QKnxSwitchControl
    \endlist

    \sa QKnxDatapointType, QKnx1Bit, {Qt KNX Datapoint Type Classes}
*/

// -- QKnx1BitControlled

/*!
    Creates a fixed size datapoint type with the value and control part set to
    \c false.
*/
QKnx1BitControlled::QKnx1BitControlled()
    : QKnx1BitControlled(SubType, false, false)
{}

/*!
    Creates a fixed size datapoint type with the subtype \a subType, value
    \a state, and control \a control.
*/
QKnx1BitControlled::QKnx1BitControlled(int subType, bool state, bool control)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("1-bit controlled"));

    setRange(QVariant(0x00), QVariant(0x03));
    setRangeText(tr("No control, false"), tr("Controlled, true"));
    setValueBit(state);
    setControlBit(control);
}

/*!
    Creates a fixed size datapoint type with the subtype \a subType, datapoint
    type \a dpt, value \a state, and control \a control.
*/

QKnx1BitControlled::QKnx1BitControlled(int subType, const QKnxDatapointType &dpt, bool state,
        bool control)
    : QKnx1BitControlled(subType, state, control)
{
    setMinimumText(tr("No control, %1").arg(dpt.minimumText()));
    setMaximumText(tr("Controlled, %1").arg(dpt.maximumText()));
}

/*!
    Returns the value stored in the datapoint type.
*/
bool QKnx1BitControlled::valueBit() const
{
    return QKnxDatapointType::testBit(byte(0), 0);
}

/*!
    Sets the value of the datapoint type to \a value.
*/
void QKnx1BitControlled::setValueBit(bool value)
{
    setByte(0, QKnxDatapointType::setBit(byte(0), value, 0));
}

/*!
    Returns the control stored in the datapoint type.
*/
bool QKnx1BitControlled::controlBit() const
{
    return QKnxDatapointType::testBit(byte(0), 1);
}

/*!
    Sets the control part of the datapoint type to \a control.
*/
void QKnx1BitControlled::setControlBit(bool control)
{
    setByte(0, QKnxDatapointType::setBit(byte(0), control, 1));
}

/*!
    \reimp
*/
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
