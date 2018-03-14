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

#ifndef QKNXUTF8STRING_H
#define QKNXUTF8STRING_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxUtf8String : public QKnxVariableSizeDatapointType
{
public:
    QKnxUtf8String();
    explicit QKnxUtf8String(const QString &string);
    explicit QKnxUtf8String(const char *string, int size = -1);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x1c;

    QString string() const;
    bool setString(const QString &string);
    bool setString(const char *string, int size = -1);

    bool isValid() const override;

protected:
    QKnxUtf8String(int subType, const char *string, int size);
};

class Q_KNX_EXPORT QKnxUtf8 : public QKnxUtf8String
{
public:
    QKnxUtf8();
    explicit QKnxUtf8(const QString &string);
    explicit QKnxUtf8(const char *string, int size = -1);

    static const constexpr int SubType = 0x01;
};

QT_END_NAMESPACE

#endif
