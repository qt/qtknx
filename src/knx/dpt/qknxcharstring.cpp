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

#include "qknxcharstring.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxCharString
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnxCharString class is a datapoint type that encodes a string.

    This datapoint type encodes a string of \l QKnxChar. The encoding of the
    string is specified in \l QKnxCharStringASCII and \l QKnxCharString88591.

    This is a fixed size datapoint type with the length of 14 bytes.

    The range for the string size is from \c 0 to \c 14.

    \sa QKnxDatapointType, QKnxVarString, QKnxVarString88591
*/

// -- QKnxCharString

static bool isAscii(const char *bytes, int size, quint8 maxValue)
{
    if (maxValue < 0x80) {
        for (int i = 0; i < size; ++i) {
            if (unsigned (bytes[i]) < 0x80)
                continue;
            return false;
        }
    }
    return true;
}

/*!
    Creates a fixed size datapoint type with the value set to \c 0.
*/
QKnxCharString::QKnxCharString()
    : QKnxCharString(SubType, nullptr, 0)
{}

/*!
    Creates a fixed size datapoint type storing the string \a string.
*/
QKnxCharString::QKnxCharString(QLatin1String string)
    : QKnxCharString(SubType, string.latin1(), string.size())
{}

/*!
    Creates a fixed size datapoint type storing the string \a string with the
    length \a size.
*/
QKnxCharString::QKnxCharString(const char *string, int size)
    : QKnxCharString(SubType, string, size)
{}

/*!
    Creates a fixed size datapoint type with the subtype \a subType, storing the
    string \a string with the length \a size.
*/
QKnxCharString::QKnxCharString(int subType, const char* string, int size)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("Fixed length character string"));
    setRange(QVariant(0x00), QVariant(0xff));
    setRangeText(tr("Minimum number of characters: 0"), tr("Maximum number of characters: 14"));
    setString(string, size);
}

/*!
    Returns the string stored in the datapoint type.
*/
QLatin1String QKnxCharString::string() const
{
    return QLatin1String((const char*) constData());
}

/*!
    Sets the string stored in the datapoint type to \a string.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxCharString::setString(QLatin1String string)
{
    return setString(string.latin1(), string.size());
}

/*!
    Sets the string stored in the datapoint type to \a string with the length
    \a size.

    If \a size is \c -1, the full \a string is used.

    If the value is outside the allowed range, returns \c false and does not set
    the string.
*/
bool QKnxCharString::setString(const char *string, int size)
{
    static auto null = QKnxByteArray(TypeSize, 0);
    if (!string)
        return setBytes(null, 0, TypeSize);

    size = (size < 0 ? int(strlen(string)) : size);
    if (size == 0)
        return setBytes(null, 0, TypeSize);

    if (size > TypeSize || !isAscii(string, size, maximum().toUInt()))
        return false;
    return setBytes(QKnxByteArray(string, size) + null.mid(0, TypeSize - size), 0, TypeSize);
}

/*!
    \reimp
*/
bool QKnxCharString::isValid() const
{
    return QKnxDatapointType::isValid()
        && isAscii((const char*) constData(), TypeSize, maximum().toUInt());
}


// -- QKnxCharStringASCII

/*!
    \class QKnxCharStringASCII
    \inherits QKnxCharString
    \inmodule QtKnx

    \brief The QKnxCharStringASCII class is a datapoint type that encodes a
    string of ASCII characters.

    This datapoint type encodes a string of \l QKnxChar.

    This is a fixed size datapoint type with the length of 14 bytes.

    \sa QKnxChar, QKnxDatapointType
*/

/*!
    Creates a fixed size datapoint type with the value set to \c null.
*/
QKnxCharStringASCII::QKnxCharStringASCII()
    : QKnxCharStringASCII(nullptr)
{}

/*!
    Creates a fixed size datapoint type storing the string \a string.
*/
QKnxCharStringASCII::QKnxCharStringASCII(QLatin1String string)
    : QKnxCharStringASCII(string.latin1(), string.size())
{}

/*!
    Creates a fixed size datapoint type storing the string \a string with the
    length \a size.

    If \a size is \c -1, the full \a string is used.
*/
QKnxCharStringASCII::QKnxCharStringASCII(const char *string, int size)
    : QKnxCharString(SubType, nullptr, 0)
{
    setDescription(tr("Fixed length character string (ASCII)"));
    setRange(QVariant(0x00), QVariant(0x7f));
    setString(string, size);
}


// -- QKnxCharString88591
/*!
    \class QKnxCharString88591
    \inherits QKnxChar
    \inmodule QtKnx

    \brief The QKnxCharString88591 class is a datapoint type that encodes a
    string of ISO 8859-1 characters.

    This datapoint type encodes a string of \l QKnxChar.

    This is a fixed size datapoint type with the length of 14 bytes.

    \sa QKnxChar, QKnxDatapointType
*/

/*!
    Creates a fixed size datapoint type with the value set to \c null.
*/
QKnxCharString88591::QKnxCharString88591()
    : QKnxCharString88591(nullptr)
{}

/*!
    Creates a fixed size datapoint type storing the string \a string.
*/
QKnxCharString88591::QKnxCharString88591(QLatin1String string)
    : QKnxCharString88591(string.latin1(), string.size())
{}

/*!
    Creates a fixed size datapoint type storing the string \a string with the
    length \a size.

    If \a size is \c -1, the full \a string is used.

*/
QKnxCharString88591::QKnxCharString88591(const char *string, int size)
    : QKnxCharString(SubType, string, size)
{
    setDescription(tr("Fixed length character string (ISO 8859-1)"));
}

QT_END_NAMESPACE
