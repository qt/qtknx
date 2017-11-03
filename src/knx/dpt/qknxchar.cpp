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

#include "qknxchar.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE


// -- QKnxChar

QKnxChar::QKnxChar()
    : QKnxChar(0)
{}

QKnxChar::QKnxChar(unsigned char value)
    : QKnxChar(SubType, value)
{}

QKnxChar::QKnxChar(int subType, unsigned char value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("Character"));
    setRange(QVariant(0x00), QVariant(0xff));
    setRangeText(tr("0"), tr("255"));

    setValue(value);
}

unsigned char QKnxChar::value() const
{
    return byte(0);
}

bool QKnxChar::setValue(unsigned char value)
{
    if (value > maximum().toUInt())
        return false;
    return setByte(0, value);
}

bool QKnxChar::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}

// -- QKnxASCII

QKnxASCII::QKnxASCII()
    : QKnxChar(SubType, 0)
{
    setDescription(tr("Character (ASCII)"));
    setRange(QVariant(0x00), QVariant(0x7f));
}

QKnxASCII::QKnxASCII(char value)
    : QKnxASCII()
{
    setCharacter(value);
}

char QKnxASCII::character() const
{
    return char(value());
}

bool QKnxASCII::setCharacter(char value)
{
    return setValue(value);
}

// -- QKnxLatin1

QKnxLatin1::QKnxLatin1()
    : QKnxChar(SubType, 0)
{
    setDescription(tr("Character (ISO 8859-1)"));
    setRange(QVariant(0x00), QVariant(0xff));
}

QKnxLatin1::QKnxLatin1(unsigned char character)
    : QKnxLatin1()
{
    setCharacter(character);
}

unsigned char QKnxLatin1::character() const
{
    return value();
}

bool QKnxLatin1::setCharacter(unsigned char value)
{
    return setValue(value);
}

QT_END_NAMESPACE
