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

#include "qknxvarstring.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

// -- QKnxVarString

QKnxVarString::QKnxVarString()
    : QKnxVarString(SubType, {})
{}

QKnxVarString::QKnxVarString(QLatin1String string)
    : QKnxVarString(SubType, string)
{}

QKnxVarString::QKnxVarString(int subType, QLatin1String string)
    : QKnxVariableSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("Variable length character string (ISO 8859-1)"));
    setRange(QVariant(0x00), QVariant(0xff));
    setString(string);
}

QLatin1String QKnxVarString::string() const
{
    return QLatin1String((const char*) constData());
}

bool QKnxVarString::setString(QLatin1String string)
{
    if (string.size() >= INT_MAX)
        return false;

    resize(string.size() + 1);
    memcpy(data(), string.latin1(), string.size());
    return setByte(string.size(), 0);
}

bool QKnxVarString::isValid() const
{
    return QKnxDatapointType::isValid() && byte(quint16(size() - 1)) == 0;
}

QT_END_NAMESPACE
