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

#include "qknxutf8string.h"
#include "qknxdatapointtype_p.h"

#include <QtCore/qstringconverter.h>

QT_BEGIN_NAMESPACE

/*!
    \class QKnxUtf8String
    \inherits QKnxVariableSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types
    \since 5.11

    \brief The QKnxUtf8String class is a datapoint type for a UTF-8 string.

    This datapoint type stores a \l {UTF-8} (Unicode Transformation Format-8)
    string. The data length for one character can vary from 1 to 4
    octets. Each character is encoded according to UTF-8.

    This is a variable sized datapoint type.

    \sa QKnxDatapointType, QKnxUtf8, {Qt KNX Datapoint Type Classes}
*/

// -- QKnxUtf8String

/*!
    Creates a variable sized datapoint type.
*/
QKnxUtf8String::QKnxUtf8String()
    : QKnxUtf8String(nullptr)
{}

/*!
    Creates a variable sized datapoint type that stores the string \a string.
*/
QKnxUtf8String::QKnxUtf8String(const QString &string)
    : QKnxUtf8String(string.toUtf8(), string.size())
{}

/*!
    Creates a variable sized datapoint type that stores the string \a string
    with the length \a size.
*/
QKnxUtf8String::QKnxUtf8String(const char *string, int size)
    : QKnxUtf8String(SubType, string, size)
{}

/*!
    Creates a variable sized datapoint type with the sub type \a subType that
    stores the string \a string with the length \a size.
*/
QKnxUtf8String::QKnxUtf8String(int subType, const char *string, int size)
    : QKnxVariableSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("Variable length character string (UTF-8)"));
    setRange(QVariant(0x00), QVariant(0xff));
    setString(string, size);
}

/*!
    Returns the string stored in the datapoint type.
*/
QString QKnxUtf8String::string() const
{
    auto data = bytes(); // TODO: fix this
    return QString::fromUtf8((const char*) data.constData(), data.size() - 1);
}

/*!
    Sets the string stored in the datapoint type to \a string.

    If the value is outside the allowed range, returns \c false and does not set
    the string.
*/
bool QKnxUtf8String::setString(const QString &string)
{
    if (string.size() >= USHRT_MAX)
        return false;

    auto utf8 = string.toUtf8();
    return setBytes(QKnxByteArray(utf8.constData(), utf8.size()) + QKnxByteArray::fromHex("00"), 0,
        utf8.size() + 1);
}

/*!
    Sets the string stored in the datapoint type to \a string with the length
    \a size.

    If \a size is \c -1, the full \a string is used.

    If the string contains invalid UTF-8 sequences, returns \c false and does
    not set the string.
*/
bool QKnxUtf8String::setString(const char *string, int size)
{
    auto null = QKnxByteArray::fromHex("00");
    if (!string)
        return setBytes(null, 0, 1);

    size = (size < 0 ? int(strlen(string)) : size);
    if (size == 0)
        return setBytes(null, 0, 1);

    if (size >= USHRT_MAX)
        return false;

    auto toUtf16 = QStringDecoder(QStringDecoder::Utf8, QStringDecoder::Flag::Stateless);
    if (!toUtf16.isValid())
        return false;

    auto text = toUtf16(string, size);
    if (toUtf16.hasError())
        return false;
    return setString(text);
}

/*!
    \reimp
*/
bool QKnxUtf8String::isValid() const
{
    return QKnxDatapointType::isValid() && byte(quint8(size()-1)) == 0;
}


// -- QKnxUtf8

/*!
    \class QKnxUtf8
    \inherits QKnxUtf8String
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types
    \since 5.11

    \brief The QKnxUtf8 class is a datapoint type for a UTF-8 string.

    This datapoint type stores a \l {UTF-8} (Unicode Transformation Format-8)
    string. The data length for one character can vary from 1 to 4
    octets. Each character is encoded according to UTF-8.

    This is a variable sized datapoint type.

    \sa QKnxDatapointType, QKnxUtf8String, {Qt KNX Datapoint Type Classes}
*/

/*!
    Creates a variable sized datapoint type.
*/
QKnxUtf8::QKnxUtf8()
    : QKnxUtf8(nullptr)
{}

/*!
    Creates a variable sized datapoint type that stores the string \a string.
*/
QKnxUtf8::QKnxUtf8(const QString &string)
    : QKnxUtf8(string.toUtf8(), string.size())
{}

/*!
    Creates a variable sized datapoint type that stores the string \a string
    with the length \a size.
*/
QKnxUtf8::QKnxUtf8(const char *string, int size)
    : QKnxUtf8String(SubType, string, size)
{}

QT_END_NAMESPACE
