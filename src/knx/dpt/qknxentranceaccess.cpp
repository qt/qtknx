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

#include "qknxentranceaccess.h"
#include "qknxdatapointtype_p.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxEntranceAccess::QKnxEntranceAccess()
    : QKnxEntranceAccess(0, Attributes(), 0)
{}

QKnxEntranceAccess::QKnxEntranceAccess(quint32 idCode, Attributes attributes, quint8 index)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Entrance Access"));
    setRangeText(tr("Low Code, 0 0 0 0 0 0"), tr("High Code, 9 9 9 9 9 9"));
    setRange(QVariant::fromValue(0), QVariant::fromValue(2576980479));

    setValue(idCode, attributes, index);
}

qint32 QKnxEntranceAccess::idCode() const
{
    if (!isValid())
        return -1;
    return (quint32(1e6) * digit(6)) + (quint32(1e5) * digit(5)) + (quint32(1e4) * digit(4))
        + (quint32(1e3) * digit(3)) + (quint32(1e2) * digit(2)) + digit(1);
}

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

QKnxEntranceAccess::Attributes QKnxEntranceAccess::attributes() const
{
    return Attributes().setFlag(Attribute::Error, isSet(Attribute::Error))
        .setFlag(Attribute::PermissionAccepted, isSet(Attribute::PermissionAccepted))
        .setFlag(Attribute::ReadRightToLeft, isSet(Attribute::ReadRightToLeft))
        .setFlag(Attribute::Encrypted, isSet(Attribute::Encrypted));
}

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

quint8 QKnxEntranceAccess::index() const
{
    return quint8(byte(3) & 0x0f);
}

bool QKnxEntranceAccess::setIdCode(quint32 idCode)
{
    if (idCode > 999999)
        return false;

    QVector<quint8> digits;
    while (idCode != 0) {
        digits.push_back(idCode % 10);
        idCode = idCode / 10;
    }
    bool success = setByte(0, quint8(digits.value(5) << 4) | digits.value(4));
    success &= setByte(1, quint8(digits.value(3) << 4) | digits.value(2));
    success &= setByte(2, quint8(digits.value(1) << 4) | digits.value(0));
    return success;
}

bool QKnxEntranceAccess::setAttributes(Attributes attributes)
{
    quint8 temp = byte(3);
    setBit(&temp, attributes.testFlag(Attribute::Error), 7);
    setBit(&temp, attributes.testFlag(Attribute::PermissionAccepted), 6);
    setBit(&temp, attributes.testFlag(Attribute::ReadRightToLeft), 5);
    setBit(&temp, attributes.testFlag(Attribute::Encrypted), 4);
    return setByte(3, temp);
}

bool QKnxEntranceAccess::setAttribute(Attribute attribute)
{
    return setAttributes(attributes() | attribute);
}

bool QKnxEntranceAccess::removeAttribute(Attribute attribute)
{
    return setAttributes(attributes() &~ attribute);
}

bool QKnxEntranceAccess::setIndex(quint8 index)
{
    if (index > 15)
        return false;
    quint8 temp = byte(3) & 0xf0;
    return setByte(3, temp | index);
}

bool QKnxEntranceAccess::setValue(quint32 idCode, Attributes attributes, quint8 index)
{
    if (setIdCode(idCode) && setIndex(index))
        return setAttributes(attributes);
    return false;
}

bool QKnxEntranceAccess::isValid() const
{
    return QKnxDatapointType::isValid()
        && QKnxUtils::QUint32::fromBytes(bytes()) <= maximum().toUInt();
}

#include "moc_qknxentranceaccess.cpp"

QT_END_NAMESPACE
