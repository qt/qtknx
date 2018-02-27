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

#include "qknxcontrolfield.h"

QT_BEGIN_NAMESPACE

const std::bitset<8> gPriorityMask = 0x0c;

/*!
    \class QKnxControlField

    \inmodule QtKnx
    \brief The QKnxControlField class represents a 8 bit KNX control field.

    A KNX frame is made up out of several field, one might be the Control
    field. The Control field shall contain at least the Frame Type, the Repeat
    flag and the frame Priority.
*/

/*!
    Creates a new control field  from a 8 Bit \a data value. Hexadecimal, octal
    and decimal notation are supported.
*/
QKnxControlField::QKnxControlField(quint8 data)
    : m_ctrl1(data)
{}

/*!
    Creates a new control field from the first byte of the \a data byte array.
    Hexadecimal, octal and decimal notation are supported.

    \note The byte array must contain at least one elements.
*/
QKnxControlField::QKnxControlField(const QKnxByteArray &data)
{
    if (data.size() > 0)
        m_ctrl1 = quint8(data[0]);
}

/*!
    Returns the priority that shall be used for transmission or reception of
    the frame.
*/
QKnxControlField::Priority QKnxControlField::priority() const
{
    return static_cast<Priority> ((m_ctrl1 & gPriorityMask).to_ulong() >> 2);
}

/*!
    Sets the \a priority that shall be used for transmission or reception of
    the frame.
*/
void QKnxControlField::setPriority(QKnxControlField::Priority priority)
{
    m_ctrl1 &= ~gPriorityMask; // clear
    m_ctrl1 |= (static_cast<quint8> (priority) << 2); // set
}

/*!
    Returns the control field's \l FrameType, \l Repeat, \l Broadcast,
    \l Priority, \l Acknowledge and \l Confirm fields as string. All values are
    formatted in hexadecimal notation.
*/
QString QKnxControlField::toString() const
{
    return QStringLiteral("Type { 0x%1 }, Repeat { 0x%2 }, Broadcast { 0x%3 }, Priority { 0x%4 }, "
            "Acknowledge { 0x%5 }, Confirm { 0x%6 }")
        .arg(m_ctrl1[7], 2, 16, QLatin1Char('0'))
        .arg(m_ctrl1[6], 2, 16, QLatin1Char('0'))
        .arg(m_ctrl1[5], 2, 16, QLatin1Char('0'))
        .arg(quint8(priority()), 2, 16, QLatin1Char('0'))
        .arg(m_ctrl1[1], 2, 16, QLatin1Char('0'))
        .arg(m_ctrl1[0], 2, 16, QLatin1Char('0'));
}

/*!
    \fn quint8 QKnxControlField::bytes() const

    Returns the control field as range of bytes.
*/

/*!
    \relates QKnxControlField

    Writes the control \a field to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxControlField &field)
{
    QDebugStateSaver _(debug);
    debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar(QLatin1Char('0'))
        << field.bytes();
    return debug;
}

/*!
    \relates QKnxControlField

    Reads the control \a field from the \a stream and returns a reference to
    the \a stream.
*/
QDataStream &operator>>(QDataStream &stream, QKnxControlField &field)
{
    quint8 raw;
    stream >> raw;
    field = QKnxControlField(raw);

    return stream;
}

/*!
    \relates QKnxControlField

    Writes the control \a field to the \a stream and returns a reference to the
    \a stream.
*/
QDataStream &operator<<(QDataStream &stream, const QKnxControlField &field)
{
    return stream << field.bytes();
}

QT_END_NAMESPACE
