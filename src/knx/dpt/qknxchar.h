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

#ifndef QKNXCHAR_H
#define QKNXCHAR_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxChar : public QKnxFixedSizeDatapointType
{
public:
    QKnxChar();
    explicit QKnxChar(unsigned char value);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x04;

    unsigned char value() const;
    bool setValue(unsigned char value);

    bool isValid() const override;

protected:
    QKnxChar(int subType, unsigned char value);
};

class Q_KNX_EXPORT QKnxCharASCII : public QKnxChar
{
public:
    QKnxCharASCII();
    explicit QKnxCharASCII(char value);

    static const constexpr int SubType = 0x01;

    char character() const;
    bool setCharacter(char value);
};

class Q_KNX_EXPORT QKnxChar88591 : public QKnxChar
{
public:
    QKnxChar88591();
    explicit QKnxChar88591(unsigned char character);

    static const constexpr int SubType = 0x02;

    unsigned char character() const;
    bool setCharacter(unsigned char value);
};

QT_END_NAMESPACE

#endif
