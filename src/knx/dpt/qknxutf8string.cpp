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

#include "qknxutf8string.h"
#include "qknxdatapointtype_p.h"
#include "qtextcodec.h"

QT_BEGIN_NAMESPACE

// -- QKnxUtf8String

QKnxUtf8String::QKnxUtf8String()
    : QKnxUtf8String(nullptr)
{}

QKnxUtf8String::QKnxUtf8String(const QString &string)
    : QKnxUtf8String(string.toUtf8(), string.size())
{}

QKnxUtf8String::QKnxUtf8String(const char *string, int size)
    : QKnxUtf8String(SubType, string, size)
{}

QKnxUtf8String::QKnxUtf8String(int subType, const char *string, int size)
    : QKnxVariableSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("Variable length character string (UTF-8)"));
    setRange(QVariant(0x00), QVariant(0xff));
    setString(string, size);
}

QString QKnxUtf8String::string() const
{
    return QString::fromUtf8(bytes());
}

bool QKnxUtf8String::setString(const QString &string)
{
    if (string.size() >= USHRT_MAX)
        return false;

    auto utf8 = string.toUtf8();
    return setBytes(QByteArray(utf8 + QByteArray::fromHex("00")), 0, utf8.size() + 1);
}

bool QKnxUtf8String::setString(const char *string, int size)
{
    auto null = QByteArray::fromHex("00");
    if (!string)
        return setBytes(null, 0, 1);

    size = (size < 0 ? int(strlen(string)) : size);
    if (size == 0)
        return setBytes(null, 0, 1);

    if (size >= USHRT_MAX)
        return false;

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    if (!codec)
        return false;

    QTextCodec::ConverterState state;
    auto text = codec->toUnicode(string, size, &state);
    if (state.invalidChars > 0)
        return false;
    return setString(text);
}

bool QKnxUtf8String::isValid() const
{
    return QKnxDatapointType::isValid() && byte(quint8(size()-1)) == 0;
}


// -- QKnxUtf8

QKnxUtf8::QKnxUtf8()
    : QKnxUtf8(nullptr)
{}

QKnxUtf8::QKnxUtf8(const QString &string)
    : QKnxUtf8(string.toUtf8(), string.size())
{}

QKnxUtf8::QKnxUtf8(const char *string, int size)
    : QKnxUtf8String(SubType, string, size)
{}

QT_END_NAMESPACE
