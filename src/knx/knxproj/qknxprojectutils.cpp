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

#include "qknxprojectutils_p.h"
#include "private/qxmlutils_p.h"

QT_BEGIN_NAMESPACE

/*!
    Returns true if \a candidate is an \c NCName. An \c NCName is a string that
    can be used as a name in XML and XQuery, e.g., the prefix or local name in
    an element or attribute, or the name of a variable.
 */
bool QKnxProjectUtils::isNCName(const QString &candidate)
{
    return QXmlUtils::isNCName(candidate);
}

bool QKnxProjectUtils::setNCName(const QString &name, QStringView attr, QString *field,
    QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !field)
        return false;

    auto string = attr.toString();
    if (pedantic && !QKnxProjectUtils::isNCName(string)) {
        reader->raiseError(tr("Pedantic error: %1 is not a valid xs:ID, got '%2'.")
            .arg(name, string));
    } else {
        *field = string;
    }
    return !reader->hasError();
}

bool QKnxProjectUtils::fetchAttr(const QXmlStreamAttributes &attributes, const QString &attrName,
    QStringView *value, QXmlStreamReader *reader)
{
    if (!value || !reader)
        return false;

    *value = attributes.value(attrName);
    if (value->isNull())
        reader->raiseError(tr("Invalid or empty attribute '%1'.").arg(attrName));
    return !reader->hasError();
}

bool QKnxProjectUtils::setString(const QString &name, QStringView attr, int maxSize,
    QString *field, QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !field)
        return false;

    if (pedantic && attr.size() > maxSize) {
        reader->raiseError(tr("Pedantic error: Invalid value for attribute '%1', maximum "
            "length is %2 characters, got: '%3'.").arg(name).arg(maxSize).arg(attr.size()));
    } else {
        *field = attr.toString();
    }
    return !reader->hasError();
}

bool QKnxProjectUtils::setString(const QString &name, QStringView attr,
    const QStringList &list, QString *field, QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !field)
        return false;

    if (pedantic && !list.contains(attr.toString())) {
        reader->raiseError(tr("Pedantic error: Invalid value for attribute '%1', expected "
            "'%2', got: '%3'.").arg(name, list.join(QLatin1String(", "))));
    } else {
        *field = attr.toString();
    }
    return !reader->hasError();
}

QT_END_NAMESPACE
