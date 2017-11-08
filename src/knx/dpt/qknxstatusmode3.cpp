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

#include "qknxstatusmode3.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE


// -- QKnxStatusMode3

QKnxStatusMode3::QKnxStatusMode3()
    : QKnxStatusMode3(Mode::Unknown, StatusFlags())
{}

QKnxStatusMode3::QKnxStatusMode3(Mode mode, StatusFlags statusFlags)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Status with Mode"));
    setRange(QVariant(0x01), QVariant(0xfc));
    setRangeText(tr("All set and Mode 0"), tr("All cleared and Mode 2"));

    setMode(mode);
    setStatusFlags(statusFlags);
}

QKnxStatusMode3::Mode QKnxStatusMode3::mode() const
{
    Mode mode = Mode(byte(0) & 0x07);
    if (mode != Mode::Zero && mode != Mode::One && mode != Mode::Two)
        return Mode::Unknown;
    return mode;
}

bool QKnxStatusMode3::setMode(Mode mode)
{
    if (mode != Mode::Zero && mode != Mode::One && mode != Mode::Two)
        return false;
    return setByte(0, (byte(0) & 0xf8) | quint8(mode));
}

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

QKnxStatusMode3::StatusFlags QKnxStatusMode3::statusFlags() const
{
    return StatusFlags().setFlag(Status::A, isSet(Status::A))
        .setFlag(Status::B, isSet(Status::B))
        .setFlag(Status::C, isSet(Status::C))
        .setFlag(Status::D, isSet(Status::D))
        .setFlag(Status::E, isSet(Status::E));
}

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

bool QKnxStatusMode3::setStatus(Status status)
{
    return setStatusFlags(statusFlags() | status);
}

bool QKnxStatusMode3::removeStatus(Status status)
{
    return setStatusFlags(statusFlags() &~ status);
}

bool QKnxStatusMode3::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt()
        && byte(0) >= minimum().toUInt() && mode() != Mode::Unknown;
}

#include "moc_qknxstatusmode3.cpp"

QT_END_NAMESPACE
