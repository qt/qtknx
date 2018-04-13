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

#include "qknxextendedcontrolfield.h"

QT_BEGIN_NAMESPACE

const std::bitset<8> gHopCountMask = 0x70;
const std::bitset<8> gExtFormatMask = 0x0f;

/*!
    \class QKnxExtendedControlField

    \inmodule QtKnx
    \brief The QKnxExtendedControlField class represents an 8-bit extended KNX
    control field.

    A KNX frame contains several fields, one of which might be the extended
    control field. The extended control field must specify
    \l destinationAddressType(), \l hopCount(), and \l ExtendedFrameFormat.
*/

/*!
    \enum QKnxExtendedControlField::ExtendedFrameFormat

    This enum type holds the extended frame format.

    \value Standard
           The frame has standard format.
    \value Lte
           The frame has Logical Tag Extended (LTE) format.
*/

/*!
    \fn QKnxExtendedControlField::byte() const

    Returns the extended control field as a byte.
*/

/*!
    \fn QKnxExtendedControlField::size() const

    Returns the number of bytes in the control field.
*/

/*!
    \fn QKnxExtendedControlField::QKnxExtendedControlField()

    Creates an extended control field.
*/

/*!
    Creates a new extended control field from an 8-bit \a data value.
*/
QKnxExtendedControlField::QKnxExtendedControlField(quint8 data)
    : m_ctrl2(data)
{}

/*!
    Creates a new extended control field from the first byte of the \a data
    byte array.

    \note The byte array must contain at least one element.
*/
QKnxExtendedControlField::QKnxExtendedControlField(const QKnxByteArray &data)
{
    if (data.size() > 0)
        m_ctrl2 = quint8(data.at(0));
}

/*!
    Returns the destination address type.
*/
QKnxAddress::Type QKnxExtendedControlField::destinationAddressType() const
{
    return static_cast<QKnxAddress::Type> (quint8(m_ctrl2[7]));
}

/*!
    Sets the destination address type to \a address.
*/
void QKnxExtendedControlField::setDestinationAddressType(QKnxAddress::Type address)
{
    m_ctrl2[7] = static_cast<int> (address);
}

/*!
    Returns the hop count.
*/
quint8 QKnxExtendedControlField::hopCount() const
{
    return static_cast<quint8> ((m_ctrl2 & gHopCountMask).to_ulong() >> 4);
}

/*!
    Sets the hop count to \a hopCount.
*/
void QKnxExtendedControlField::setHopCount(quint8 hopCount)
{
    if (hopCount > 7)
        return;

    m_ctrl2 &= ~gHopCountMask; // clear
    m_ctrl2 |= (hopCount << 4); // set
}

/*!
    Returns the format of the extended control field.
*/
QKnxExtendedControlField::ExtendedFrameFormat QKnxExtendedControlField::format() const
{
    return static_cast<ExtendedFrameFormat> ((m_ctrl2 & gExtFormatMask).to_ulong());
}

/*!
    Sets the format of the extended control field to \a format.
*/
void QKnxExtendedControlField::setFormat(QKnxExtendedControlField::ExtendedFrameFormat format)
{
    m_ctrl2 &= ~gExtFormatMask; // clear
    if (format == QKnxExtendedControlField::ExtendedFrameFormat::Lte)
        m_ctrl2 |= gExtFormatMask; // set
}

/*!
    \fn quint8 QKnxExtendedControlField::bytes() const

    Returns the extended control field as a range of bytes.
*/

/*!
    \relates QKnxExtendedControlField

    Writes the extended control field \a field to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxExtendedControlField &field)
{
    QDebugStateSaver _(debug);
    debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar(QLatin1Char('0'))
        << field.bytes();
    return debug;
}

QT_END_NAMESPACE
