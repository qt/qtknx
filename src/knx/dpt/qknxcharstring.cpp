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

#include "qknxcharstring.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE


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

QKnxCharString::QKnxCharString()
    : QKnxCharString(SubType, {})
{}

QKnxCharString::QKnxCharString(QLatin1String string)
    : QKnxCharString(SubType, string)
{}

QKnxCharString::QKnxCharString(int subType, QLatin1String string)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("Character string"));
    setRange(QVariant(0x00), QVariant(0xff));
    setRangeText(tr("Minimum number of characters: 0"), tr("Maximum number of characters: 14"));
    setString(string);
}

QLatin1String QKnxCharString::string() const
{
    return QLatin1String((const char*) constData());
}

bool QKnxCharString::setString(QLatin1String string)
{
    if (string.size() > size() || !isAscii(string.latin1(), string.size(), maximum().toUInt()))
        return false;

    memset(data(), 0, size());
    memcpy(data(), string.latin1(), string.size());
    return true;
}

bool QKnxCharString::isValid() const
{
    return QKnxDatapointType::isValid()
        && isAscii((const char*) constData(), TypeSize, maximum().toUInt());
}


// -- QKnxCharStringASCII

QKnxCharStringASCII::QKnxCharStringASCII()
    : QKnxCharString(SubType, {})
{
    setDescription(tr("Character string (ASCII)"));
    setRange(QVariant(0x00), QVariant(0x7f));
}

QKnxCharStringASCII::QKnxCharStringASCII(QLatin1String string)
    : QKnxCharStringASCII()
{
    setString(string);
}


// -- QKnxCharString88591

QKnxCharString88591::QKnxCharString88591()
    : QKnxCharString(SubType, {})
{
    setDescription(tr("Character string (ISO 8859-1)"));
}

QKnxCharString88591::QKnxCharString88591(QLatin1String string)
    : QKnxCharString88591()
{
    setString(string);
}

QT_END_NAMESPACE
