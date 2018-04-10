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

#include "qknxstatusmode3.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxStatusMode3
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \brief The QKnxStatusMode3 class is a datapoint type for a status mode 3.

    This datapoint type holds information about which \l{Status}{status} flags
    (from \c A to \c E) are set and which \l{Mode}{mode} (from \c Zero to \c 2)
    is active. Valid values are from \c {All set and Mode 0} to
    \c {All cleared and Mode 2}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType
*/

/*!
    \enum QKnxStatusMode3::Mode

    This enum holds information about which mode is active.

    \value Zero
    Mode 0 is active.
    \value One
    Mode 1 is active.
    \value Two
    Mode 2 is active.
    \value Unknown
    It is not possible to determine which mode is active.
*/

/*!
    \enum QKnxStatusMode3::Status

    This enum indicates whether a status flag is set (\c 0) or cleared (\c 1).

    \value A
    Status value A.
    \value B
    Status value B.
    \value C
    Status value C.
    \value D
    Status value D.
    \value E
    Status value E.
*/

// -- QKnxStatusMode3

/*!
    Creates a fixed size datapoint type with the mode set to \c Unknown and
    the status flags left empty.
*/
QKnxStatusMode3::QKnxStatusMode3()
    : QKnxStatusMode3(Mode::Unknown, StatusFlags())
{}

/*!
    Creates a fixed size datapoint type with the mode set to \a mode and
    the status flags set to \a statusFlags.
*/
QKnxStatusMode3::QKnxStatusMode3(Mode mode, StatusFlags statusFlags)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Status with Mode"));
    setRange(QVariant(0x01), QVariant(0xfc));
    setRangeText(tr("All set and Mode 0"), tr("All cleared and Mode 2"));

    setMode(mode);
    setStatusFlags(statusFlags);
}

/*!
    Returns the mode stored in the datapoint type.
*/
QKnxStatusMode3::Mode QKnxStatusMode3::mode() const
{
    Mode mode = Mode(byte(0) & 0x07);
    if (mode != Mode::Zero && mode != Mode::One && mode != Mode::Two)
        return Mode::Unknown;
    return mode;
}

/*!
    Sets the mode stored in the datapoint type to \a mode.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxStatusMode3::setMode(Mode mode)
{
    if (mode != Mode::Zero && mode != Mode::One && mode != Mode::Two)
        return false;
    return setByte(0, (byte(0) & 0xf8) | quint8(mode));
}

/*!
    Returns \c true if the status flag \a status is set in the status flags.
*/
bool QKnxStatusMode3::isSet(Status status) const
{
    if (status == Status::A)
        return !QKnxDatapointType::testBit(byte(0), 7);
    if (status == Status::B)
        return !QKnxDatapointType::testBit(byte(0), 6);
    if (status == Status::C)
        return !QKnxDatapointType::testBit(byte(0), 5);
    if (status == Status::D)
        return !QKnxDatapointType::testBit(byte(0), 4);
    if (status == Status::E)
        return !QKnxDatapointType::testBit(byte(0), 3);
    return false;
}

/*!
    Returns the status flags stored in the datapoint type.
*/
QKnxStatusMode3::StatusFlags QKnxStatusMode3::statusFlags() const
{
    return StatusFlags().setFlag(Status::A, isSet(Status::A))
        .setFlag(Status::B, isSet(Status::B))
        .setFlag(Status::C, isSet(Status::C))
        .setFlag(Status::D, isSet(Status::D))
        .setFlag(Status::E, isSet(Status::E));
}

/*!
    Sets the statusFlags stored in the datapoint type to \a statusFlags.

    Returns \c true if the value was set; otherwise returns \c false.

*/
bool  QKnxStatusMode3::setStatusFlags(StatusFlags statusFlags)
{
    quint8 value = byte(0);
    value = QKnxDatapointType::setBit(value, !statusFlags.testFlag(Status::A), 7);
    value = QKnxDatapointType::setBit(value, !statusFlags.testFlag(Status::B), 6);
    value = QKnxDatapointType::setBit(value, !statusFlags.testFlag(Status::C), 5);
    value = QKnxDatapointType::setBit(value, !statusFlags.testFlag(Status::D), 4);
    value = QKnxDatapointType::setBit(value, !statusFlags.testFlag(Status::E), 3);
    return setByte(0, value);
}

/*!
    Sets the status \a status in the status flags.

    Returns \c true if the status was set; otherwise returns \c false.
*/
bool QKnxStatusMode3::setStatus(Status status)
{
    return setStatusFlags(statusFlags() | status);
}

/*!
    Removes the status \a status from the status flags.

    Returns \c true if the status was removed; otherwise returns \c false.
*/
bool QKnxStatusMode3::removeStatus(Status status)
{
    return setStatusFlags(statusFlags() &~ status);
}

/*!
    \reimp
*/
bool QKnxStatusMode3::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt()
        && byte(0) >= minimum().toUInt() && mode() != Mode::Unknown;
}

#include "moc_qknxstatusmode3.cpp"

QT_END_NAMESPACE
