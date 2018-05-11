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

#include "qknxvarstring.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxVarString
    \inherits QKnxVariableSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types
    \since 5.11

    \brief The QKnxVarString class is a datapoint type that encodes a variable
    length string.

    This datapoint type encodes a string of variable length. The encoding of the
    string is specified in \l QKnxVarString88591.

    This is a variable sized datapoint type.

    \sa QKnxDatapointType, QKnxVarString88591, QKnxCharString,
        QKnxCharStringASCII, QKnxCharString88591,
        {Qt KNX Datapoint Type Classes}
*/

// -- QKnxVarString

/*!
    Creates a variable sized datapoint type.
*/
QKnxVarString::QKnxVarString()
    : QKnxVarString(nullptr)
{}

/*!
    Creates a variable sized datapoint type that stores the string \a string.
*/
QKnxVarString::QKnxVarString(QLatin1String string)
    : QKnxVarString(string.latin1(), string.size())
{}

/*!
    Creates a variable sized datapoint type that stores the string \a string
    with the length \a size.
*/
QKnxVarString::QKnxVarString(const char *string, int size)
    : QKnxVarString(SubType, string, size)
{}

/*!
    Creates a variable sized datapoint type with the sub type \a subType that
    stores the string \a string with the length \a size.
*/
QKnxVarString::QKnxVarString(int subType, const char *string, int size)
    : QKnxVariableSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("Variable length character string (ISO 8859-1)"));
    setRange(QVariant(0x00), QVariant(0xff));
    setString(string, size);
}

/*!
    Returns the string stored in the datapoint type.
*/
QLatin1String QKnxVarString::string() const
{
    return QLatin1String((const char*) constData());
}

/*!
    Sets the string stored in the datapoint type to \a string.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxVarString::setString(QLatin1String string)
{
    return setString(string.latin1(), string.size());
}

/*!
    Sets the string stored in the datapoint type to \a string with the length
    \a size.

    If \a size is \c -1, the full \a string is used.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxVarString::setString(const char *string, int size)
{
    auto null = QKnxByteArray::fromHex("00");
    if (!string)
        return setBytes(null, 0, 1);

    size = (size < 0 ? int(strlen(string)) : size);
    if (size == 0)
        return setBytes(null, 0, 1);

    if (size >= USHRT_MAX)
        return false;
    return setBytes(QKnxByteArray(string, size) + null, 0, size + 1);
}

/*!
    \reimp
*/
bool QKnxVarString::isValid() const
{
    return QKnxDatapointType::isValid() && byte(quint16(size() - 1)) == 0;
}


// -- QKnxVarString88591

/*!
    \class QKnxVarString88591
    \inherits QKnxVarString
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types
    \since 5.11

    \brief The QKnxVarString88591 class is a datapoint type that encodes a
    variable length string of ISO 8859-1 characters.

    This is a variable sized datapoint type.

    \sa QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

/*!
    Creates a variable sized datapoint type.
*/
QKnxVarString88591::QKnxVarString88591()
    : QKnxVarString88591(nullptr)
{}

/*!
    Creates a fixed size datapoint type storing the string \a string.
*/
QKnxVarString88591::QKnxVarString88591(QLatin1String string)
    : QKnxVarString88591(string.latin1(), string.size())
{}

/*!
    Creates a fixed size datapoint type storing the string \a string with the
    length \a size.
*/
QKnxVarString88591::QKnxVarString88591(const char *string, int size)
    : QKnxVarString(SubType, string, size)
{}

QT_END_NAMESPACE
