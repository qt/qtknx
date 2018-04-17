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

#ifndef QKNXPROJECTUTILS_P_H
#define QKNXPROJECTUTILS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QZipReader class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qcoreapplication.h>
#include <QtCore/qxmlstream.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxProjectUtils final
{
    Q_DECLARE_TR_FUNCTIONS(QKnxProjectUtils)

public:
    static bool isNCName(const QString &candidate);

    static bool setNCName(const QString &name, const QStringRef &attr, QString *field,
        QXmlStreamReader *reader, bool pedantic);

    static bool fetchAttr(const QXmlStreamAttributes &attributes, const QString &attrName,
        QStringRef *value, QXmlStreamReader *reader);

    static bool setString(const QString &name, const QStringRef &attr, int maxSize,
        QString *field, QXmlStreamReader *reader, bool pedantic);

    static bool setString(const QString &name, const QStringRef &attr, const QStringList &list,
        QString *field, QXmlStreamReader *reader, bool pedantic);

    template< typename T> static bool setNumber(const QString &name, const T &attr, T maxValue,
        T *field, QXmlStreamReader *reader, bool pedantic)
    {
        if (!reader || !field)
            return false;

        if (pedantic && attr > maxValue) {
            reader->raiseError(tr("Pedantic error: Invalid value for attribute '%1', maximum "
                "value is %2 characters, got: '%3'.").arg(name).arg(maxValue).arg(attr));
        } else {
            *field = attr;
        }
        return !reader->hasError();
    }

    template<typename T>
    static bool parseChildElement(QXmlStreamReader *reader, bool pedantic, QVector<T> *container)
    {
        while (!reader->atEnd() && reader->readNextStartElement()) {
            T element;
            if (!element.parseElement(reader, pedantic))
                break;
            container->append(element);
        }
        return !reader->hasError();
    }
};

QT_END_NAMESPACE

#endif
