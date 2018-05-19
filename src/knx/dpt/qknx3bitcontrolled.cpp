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

#include "qknx3bitcontrolled.h"
#include "qknxdatapointtype_p.h"
#include "qmath.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx3BitControlled
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnx3BitControlled class is a datapoint type with a control part.

    This is a fixed size datapoint type with the length of 1 byte, though only
    4 bits are used by the actual implementation.

    Of the 4 bits, 3 bits are reserved for the step code and 1 bit for the
    control part. The step code allows for the calculation of the number of
    intervals, so that numberOfIntervals() equals \c 2^(step code -1).

    To set this datapoint type, the control and the NumberOfIntervals have to
    be set.

    \sa QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnx3BitControlled::NumberOfIntervals

    This enum type holds the allowed number of intervals for the datapoint type.

    \value Break
    \value One
    \value Two
    \value Four
    \value Eight
    \value Sixteen
    \value ThirtyTwo
*/

// -- QKnx3BitControlled

/*!
    Creates a fixed size datapoint type with the control part set to \c false
    and the number of intervals set to \c Break.
*/
QKnx3BitControlled::QKnx3BitControlled()
    : QKnx3BitControlled(false, NumberOfIntervals::Break)
{}

/*!
    Creates a fixed size datapoint type with the control \a control and the
    number of intervals \a n.
*/
QKnx3BitControlled::QKnx3BitControlled(bool control, NumberOfIntervals n)
    : QKnx3BitControlled(SubType, control, n)
{}

/*!
    Creates a fixed size datapoint type with subtype \a subType, control
    \a control, and number of intervals \a n.
*/
QKnx3BitControlled::QKnx3BitControlled(int subType, bool control, NumberOfIntervals n)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("3-bit controlled"));
    setRange(QVariant(0x00), QVariant(0x0f));
    setRangeText(tr("No control, Break"), tr("Controlled, 32 intervals"));

    setControlBit(control);
    setNumberOfIntervals(n);
}

/*!
    Returns the control stored in the datapoint type.
*/
bool QKnx3BitControlled::controlBit() const
{
    return QKnxDatapointType::testBit(byte(0), 3);
}
/*!
    Sets the control part of the datapoint type to \a value.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
void QKnx3BitControlled::setControlBit(bool value)
{
    setByte(0, QKnxDatapointType::setBit(byte(0), value, 3));
}

/*!
    Sets the number of intervals to \a n.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnx3BitControlled::setNumberOfIntervals(NumberOfIntervals n)
{
    if (n > NumberOfIntervals::ThirtyTwo)
        return false;

    quint8 stepCode = 0;
    if (n != NumberOfIntervals::Break)
        stepCode = quint8(1 + qLn(quint8(n)) / qLn(2.));
    return setByte(0, (byte(0) & 0x08) | stepCode);
}

/*!
    Returns the number of intervals stored in the datapoint type.
*/
QKnx3BitControlled::NumberOfIntervals QKnx3BitControlled::numberOfIntervals() const
{
    quint8 stepCode = quint8(byte(0) & 0x07);
    return NumberOfIntervals(quint8(qPow(2, stepCode - 1)));
}

/*!
    \reimp
*/
bool QKnx3BitControlled::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}


// -- QKnxControlDimming

/*!
    \class QKnxControlDimming
    \inherits QKnx3BitControlled
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxControlDimming class is a datapoint type for controlling
    dimming.

    To set this datapoint type, \l{QKnxControlDimming::}{Control} and
    \l{QKnx3BitControlled::}{NumberOfIntervals} have to be set.

    \sa QKnx3BitControlled, {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnxControlDimming::Control

    This enum type holds whether dimming decreases or increases.

    \value Decrease
    \value Increase
*/

/*!
    Creates a fixed size datapoint type with the control set to \l Decrease and
    the number of intervals set to \l QKnx3BitControlled::Break.
*/
QKnxControlDimming::QKnxControlDimming()
    : QKnxControlDimming(Control::Decrease, NumberOfIntervals::Break)
{}

/*!
    Creates a fixed size datapoint type with the control \a control and the
    number of intervals \a interval.
*/
QKnxControlDimming::QKnxControlDimming(Control control, NumberOfIntervals interval)
    : QKnx3BitControlled(SubType, bool(control), interval)
{
    setDescription(tr("Control Dimming"));
    setRangeText(tr("Decrease, Break"), tr("Increase, 32 intervals"));
}

/*!
    Sets the control part of the datapoint type to \a control.
*/
void QKnxControlDimming::setControl(Control control)
{
    QKnx3BitControlled::setControlBit(bool(control));
}

/*!
    Returns the control stored in the datapoint type.
*/
QKnxControlDimming::Control QKnxControlDimming::control() const
{
    return Control(QKnx3BitControlled::controlBit());
}


// -- QKnxControlBlinds

/*!
    \class QKnxControlBlinds
    \inherits QKnx3BitControlled
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxControlBlinds class is a datapoint type for controlling
    blinds.

    To set this datapoint type, \l{QKnxControlBlinds::}{Control} and
    \l{QKnx3BitControlled::}{NumberOfIntervals} have to be set.

    \sa QKnx3BitControlled, {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnxControlBlinds::Control

    This enum type holds whether the blinds are being raised or lowered.

    \value Up
           Blinds are being raised.
    \value Down
           Blinds are being lowered.
*/

/*!
    Creates a fixed size boolean datapoint type with the control set to \l Up
    and the number of intervals set to \l QKnx3BitControlled::Break.
*/
QKnxControlBlinds::QKnxControlBlinds()
    : QKnxControlBlinds(Control::Up, NumberOfIntervals::Break)
{}

/*!
    Creates a fixed size datapoint type with the control \a control and the
    number of intervals \a interval.
*/
QKnxControlBlinds::QKnxControlBlinds(Control control, NumberOfIntervals interval)
    : QKnx3BitControlled (SubType, bool(control), interval)
{
    setDescription(tr("Control Blinds"));
    setRangeText(tr("Up, Break"), tr("Down, 32 intervals"));
}

/*!
    Sets the control part of the datapoint type to \a control.
*/
void QKnxControlBlinds::setControl(Control control)
{
    QKnx3BitControlled::setControlBit(bool(control));
}

/*!
    Returns the control stored in the datapoint type.
*/
QKnxControlBlinds::Control QKnxControlBlinds::control() const
{
    return Control(QKnx3BitControlled::controlBit());
}

#include "moc_qknx3bitcontrolled.cpp"

QT_END_NAMESPACE
