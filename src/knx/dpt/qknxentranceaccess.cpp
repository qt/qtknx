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

#include "qknxentranceaccess.h"
#include "qknxdatapointtype_p.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxEntranceAccess
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \brief The QKnxEntranceAccess is a datapoint type for controlling entrance
    access.

    This is a fixed size datapoint type with the length of 4 bytes that stores
    an access identification code, additional \l{Attribute}{attributes}, and an
    index.

    Access identification codes consist of six digits, each between 0 and 9.
    Only a card or key number should be used. A system number, version number,
    country code, and so on, are not necessary. A ciphered access identification
    code should be possible in principle. If 24 bits are not necessary, the most
    significant positions shall be set to zero.

    The range of the value is from \c {Low Code, 0 0 0 0 0 0} to
    \c {High Code, 9 9 9 9 9 9}.

    \sa QKnxDatapointType
*/

/*!
    \enum QKnxEntranceAccess::Attribute

    This enum holds the access identification code attributes.

    \value Error
           Whether the controlling device could successfully read the access
           identification code.
    \value PermissionAccepted
           Whether the controlling device granted the access.
    \value ReadRightToLeft
           Sets the reading direction of a device used for access, such as a
           badge, to be from right to left. Not needed for devices such as
           electronic keys, and can be set to zero.
    \value Encrypted
           Whether the access identification code is encrypted.
*/

/*!
    \fn bool QKnxEntranceAccess::setDigit(quint8 x, quint8 digit)

    Sets the digit \a digit at the position \a x in the access identification
    code.
*/


/*!
    Creates a fixed size datapoint type with the value set to \c 0 and an empty
    list of attributes.
*/
QKnxEntranceAccess::QKnxEntranceAccess()
    : QKnxEntranceAccess(0, Attributes(), 0)
{}

/*!
    Creates a fixed size datapoint type with the access identification code set
    to \a idCode, attributes to \a attributes, and the index to \a index.
*/
QKnxEntranceAccess::QKnxEntranceAccess(quint32 idCode, Attributes attributes, quint8 index)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Entrance Access"));
    setRangeText(tr("Low Code, 0 0 0 0 0 0"), tr("High Code, 9 9 9 9 9 9"));
    setRange(QVariant::fromValue(0), QVariant::fromValue(2576980479));

    setValue(idCode, attributes, index);
}

/*!
    Returns the access identification code stored in the datapoint type or a
    negative value if the datapoint is invalid.
*/
qint32 QKnxEntranceAccess::idCode() const
{
    if (!isValid())
        return -1;
    return (quint32(1e6) * digit(6)) + (quint32(1e5) * digit(5)) + (quint32(1e4) * digit(4))
        + (quint32(1e3) * digit(3)) + (quint32(1e2) * digit(2)) + digit(1);
}

/*!
    Returns the digit \a x of the access identification code or a negative value
    if the datapoint is invalid or \a x is not in the range from \c 1 to \c 6.
*/
qint8 QKnxEntranceAccess::digit(quint8 x) const
{
    qint8 value = -1;
    if (isValid() && x >= 1 && x <= 6) {
        if (x == 5 || x == 6)
            value = qint8(byte(0));
        if (x == 4 || x == 3)
            value = qint8(byte(1));
        if (x == 2 || x == 1)
            value = qint8(byte(2));
        value = (x % 2 ? value & 0x0f : (value & 0xf0) >> 4);
    }
    return value;
}

/*!
    Returns the additional information stored in the datapoint type as a list
    of attributes.

    \sa Attribute
*/
QKnxEntranceAccess::Attributes QKnxEntranceAccess::attributes() const
{
    return Attributes().setFlag(Attribute::Error, isSet(Attribute::Error))
        .setFlag(Attribute::PermissionAccepted, isSet(Attribute::PermissionAccepted))
        .setFlag(Attribute::ReadRightToLeft, isSet(Attribute::ReadRightToLeft))
        .setFlag(Attribute::Encrypted, isSet(Attribute::Encrypted));
}

/*!
    Returns \c true if \a attribute is set; otherwise returns \c false.
*/
bool QKnxEntranceAccess::isSet(Attribute attribute) const
{
    if (attribute == Attribute::Error)
        return bool(quint8((byte(3) & 0x80) >> 7));
    if (attribute == Attribute::PermissionAccepted)
        return bool(quint8((byte(3) & 0x40) >> 6));
    if (attribute == Attribute::ReadRightToLeft)
        return bool(quint8((byte(3) & 0x20) >> 5));
    if (attribute == Attribute::Encrypted)
        return bool(quint8((byte(3) & 0x10) >> 4));
    return false;
}

/*!
    Returns the index stored in the datapoint type.
*/
quint8 QKnxEntranceAccess::index() const
{
    return quint8(byte(3) & 0x0f);
}

/*!
    Sets the access identification code stored in the datapoint type to
    \a idCode.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxEntranceAccess::setIdCode(quint32 idCode)
{
    if (idCode > 999999)
        return false;

    QKnxByteArray digits;
    while (idCode != 0) {
        digits.push_back(idCode % 10);
        idCode = idCode / 10;
    }
    bool success = setByte(0, quint8(digits.value(5) << 4) | digits.value(4));
    success &= setByte(1, quint8(digits.value(3) << 4) | digits.value(2));
    success &= setByte(2, quint8(digits.value(1) << 4) | digits.value(0));
    return success;
}

/*!
    Sets the additional information of the datapoint type to the list of
    attributes specified by \a attributes.

    \sa Attribute
*/
bool QKnxEntranceAccess::setAttributes(Attributes attributes)
{
    quint8 temp = byte(3);
    temp = QKnxDatapointType::setBit(temp, attributes.testFlag(Attribute::Error), 7);
    temp = QKnxDatapointType::setBit(temp, attributes.testFlag(Attribute::PermissionAccepted), 6);
    temp = QKnxDatapointType::setBit(temp, attributes.testFlag(Attribute::ReadRightToLeft), 5);
    temp = QKnxDatapointType::setBit(temp, attributes.testFlag(Attribute::Encrypted), 4);
    return setByte(3, temp);
}

/*!
    Adds \a attribute to the list of attributes.

    \sa Attribute
*/
bool QKnxEntranceAccess::setAttribute(Attribute attribute)
{
    return setAttributes(attributes() | attribute);
}

/*!
    Removes \a attribute from the list of attributes.

    \sa Attribute
*/
bool QKnxEntranceAccess::removeAttribute(Attribute attribute)
{
    return setAttributes(attributes() &~ attribute);
}

/*!
    Sets the index stored in the datapoint type to \a index.

    If the value is outside the allowed range, from \c 0 to \c 15, returns
    \c false and does not set the value.
*/
bool QKnxEntranceAccess::setIndex(quint8 index)
{
    if (index > 15)
        return false;
    quint8 temp = byte(3) & 0xf0;
    return setByte(3, temp | index);
}

/*!
    Sets the access identification code stored in the datapoint type to
    \a idCode, the list of attributes to \a attributes, and the index to
    \a index.
*/
bool QKnxEntranceAccess::setValue(quint32 idCode, Attributes attributes, quint8 index)
{
    if (setIdCode(idCode) && setIndex(index))
        return setAttributes(attributes);
    return false;
}

/*!
    \reimp
*/
bool QKnxEntranceAccess::isValid() const
{
    return QKnxDatapointType::isValid()
        && QKnxUtils::QUint32::fromBytes(bytes()) <= maximum().toUInt();
}

#include "moc_qknxentranceaccess.cpp"

QT_END_NAMESPACE
