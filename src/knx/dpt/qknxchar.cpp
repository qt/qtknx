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

#include "qknxchar.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxChar
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnxChar class is a datapoint type that encodes a character.

    The encoding of the character is specified in \l QKnxCharASCII and
    \l QKnxChar88591.

    This is a fixed size datapoint type with the length of 1 byte.

    The range for the value is from \c 0 to \c 255.

    \sa QKnxDatapointType
*/

// -- QKnxChar

/*!
    Creates a fixed size datapoint type with the value set to \c 0.
*/
QKnxChar::QKnxChar()
    : QKnxChar(0)
{}

/*!
    Creates a fixed size datapoint type with the value \a value.

*/
QKnxChar::QKnxChar(unsigned char value)
    : QKnxChar(SubType, value)
{}

/*!
    Creates a fixed size datapoint with the subtype \a subType and the value
    \a value.
*/
QKnxChar::QKnxChar(int subType, unsigned char value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("Character"));
    setRange(QVariant(0x00), QVariant(0xff));
    setRangeText(tr("0"), tr("255"));

    setValue(value);
}

/*!
    Returns the value stored in the datapoint type.
*/
unsigned char QKnxChar::value() const
{
    return byte(0);
}

/*!
    Sets the value of the datapoint type to \a value.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxChar::setValue(unsigned char value)
{
    if (value > maximum().toUInt())
        return false;
    return setByte(0, value);
}

/*!
    \reimp
*/
bool QKnxChar::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}

// -- QKnxCharASCII
/*!
    \class QKnxCharASCII
    \inherits QKnxChar
    \inmodule QtKnx

    \brief The QKnxCharASCII class is a datapoint type that encodes an ASCII
    character.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxChar, QKnxDatapointType
*/

/*!
    Creates a fixed size datapoint type with the value set to \c 0.
*/
QKnxCharASCII::QKnxCharASCII()
    : QKnxChar(SubType, 0)
{
    setDescription(tr("Character (ASCII)"));
    setRange(QVariant(0x00), QVariant(0x7f));
}

/*!
    Creates a fixed size datapoint type with the character \a value.

*/
QKnxCharASCII::QKnxCharASCII(char value)
    : QKnxCharASCII()
{
    setCharacter(value);
}

/*!
    Returns the character stored in the datapoint type.
*/
char QKnxCharASCII::character() const
{
    return char(value());
}

/*!
    Sets the character of the datapoint type to \a value.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxCharASCII::setCharacter(char value)
{
    return setValue(value);
}

// -- QKnxChar88591

/*!
    \class QKnxChar88591
    \inherits QKnxChar
    \inmodule QtKnx

    \brief The QKnxChar88591 class is a datapoint type that encodes an
    ISO 8859-1 character.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxChar, QKnxDatapointType
*/

/*!
    Creates a fixed size datapoint type with the value set to \c 0.
*/
QKnxChar88591::QKnxChar88591()
    : QKnxChar(SubType, 0)
{
    setDescription(tr("Character (ISO 8859-1)"));
    setRange(QVariant(0x00), QVariant(0xff));
}

/*!
    Creates a fixed size datapoint type with the character \a character.

*/
QKnxChar88591::QKnxChar88591(unsigned char character)
    : QKnxChar88591()
{
    setCharacter(character);
}

/*!
    Returns the character stored in the datapoint type.
*/
unsigned char QKnxChar88591::character() const
{
    return value();
}

/*!
    Sets the character of the datapoint type to \a value.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxChar88591::setCharacter(unsigned char value)
{
    return setValue(value);
}

QT_END_NAMESPACE
