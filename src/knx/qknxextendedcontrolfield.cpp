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

/*!
    \class QKnxExtendedControlField

    \inmodule QtKnx
    \brief The QKnxExtendedControlField class represents an 8-bit extended KNX
    control field.

    A KNX frame contains several fields, one of which might be the extended
    control field. The extended control field must specify
    \l destinationAddressType(), \l hopCount(), and \l ExtendedFrameFormat.

    The address type determines whether group addresses or individual addresses
    are used to establish a communication channel between devices.

    The hop count specifies the number of subnetworks that the frame is allowed
    to pass. It prevents the endless circulation of messages in incorrectly
    configured installations.

    The value of the \l ExtendedFrameFormat flag is mapped to the value of
    \l QKnxControlField::FrameFormat. If it is set to \c Extended, the
    extended frame format is used and the frame type is selected by the frame
    type parameter bit.

    The following is an example of how to create a control field from a
    KNX byte array:

    \code
        auto data = QKnxByteArray::fromHex("e0");
        const QKnxExtendedControlField extCtrlField(data);
    \endcode

    In addition to the default constructors a builder can be used:

    \code
        auto extCtrl = QKnxExtendedControlField::builder()
            .setDestinationAddressType(QKnxAddress::Type::Group)
            .setHopCount(6)
            .setFormat(QKnxExtendedControlField::ExtendedFrameFormat::Standard)
            .create();
    \endcode

    \sa builder(), QKnxControlField
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
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxExtendedControlField::operator==(const QKnxExtendedControlField &other) const
{
    return m_ctrl2 == other.m_ctrl2;
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxExtendedControlField::operator!=(const QKnxExtendedControlField &other) const
{
    return !operator==(other);
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
    return static_cast<ExtendedFrameFormat> (m_ctrl2.test(3));
}

/*!
    Sets the format of the extended control field to \a format.
*/
void QKnxExtendedControlField::setFormat(QKnxExtendedControlField::ExtendedFrameFormat format)
{
    m_ctrl2.set(3, format == QKnxExtendedControlField::ExtendedFrameFormat::Lte);
}

/*!
    Returns a builder to create a KNX extended control field object.
*/
QKnxExtendedControlField::Builder QKnxExtendedControlField::builder()
{
    return Builder();
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


/*!
    \class QKnxExtendedControlField::Builder

    \inmodule QtKnx
    \inheaderfile QKnxExtendedControlField

    \brief The QKnxExtendedControlField::Builder class creates a KNX extended
    control field with some default values set.

    The default values produce extended control fields that are suitable for
    multicast frames that read or write group values.

    By default, the address type is set to \l{QKnxAddress::Type}{Group} to
    specify that the frame is sent to a group address. The hop count is set
    to 6 to limit the number of subnetworks the frame is allowed to pass to
    six. This prevents the endless circulation of messages in incorrectly
    configured installations. The frame format is mapped to the value of
    \l QKnxControlField::FrameFormat, which is set to \c Standard by default.
    That is the preferred format for short frames.

    The following sample code creates an extended control field using the
    default values:

    Example:
    \code
        auto ctrl = QKnxExtendedControlField::builder.create();
    \endcode

    Some flags can be modified for more advanced use, such as setting up an
    extended control field used for transmitting a \c {L_Data_Extended} frame
    to the individual address of a device:

    \code
        auto ctrl = QKnxControlField::builder
            .setDestinationAddressType(QKnxAddress::Type::Individual)
            .create();
    \endcode
*/

/*!
    Sets the destination address type to \a address and returns a reference to
    the builder.
*/
QKnxExtendedControlField::Builder &
    QKnxExtendedControlField::Builder::setDestinationAddressType(QKnxAddress::Type address)
{
    m_address = address;
    return *this;
}

/*!
    Sets the hop count to \a hopCount and returns a reference to the builder.
*/
QKnxExtendedControlField::Builder &QKnxExtendedControlField::Builder::setHopCount(quint8 hopCount)
{
    m_hopCount = hopCount;
    return *this;
}

/*!
    Sets the extended frame format to \a format and returns a reference to the
    builder.
*/
QKnxExtendedControlField::Builder &
    QKnxExtendedControlField::Builder::setFormat(QKnxExtendedControlField::ExtendedFrameFormat format)
{
    m_format = format;
    return *this;
}

/*!
    Creates and returns a QKnxExtendedControlField.
*/
QKnxExtendedControlField QKnxExtendedControlField::Builder::create() const
{
    QKnxExtendedControlField extCtrl;
    extCtrl.setDestinationAddressType(m_address);
    extCtrl.setHopCount(m_hopCount);
    extCtrl.setFormat(m_format);
    return extCtrl;
}

QT_END_NAMESPACE
