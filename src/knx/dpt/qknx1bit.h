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

#ifndef QKNX1BIT_H
#define QKNX1BIT_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx1Bit : public QKnxDatapointType
{
public:
    QKnx1Bit();
    explicit QKnx1Bit(bool bit);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x01;

    bool bit() const;
    bool setBit(bool value);

    bool isValid() const override;

protected:
    QKnx1Bit(int subType, bool bit);
};

class Q_KNX_EXPORT QKnxSwitch : public QKnx1Bit
{
    Q_GADGET

public:
    enum class State : quint8
    {
        Off,
        On
    };
    Q_ENUM(State)

    QKnxSwitch();
    explicit QKnxSwitch(State state);

    static const constexpr int SubType = 0x01;

    State value() const;
    bool setValue(State state);
};

QT_END_NAMESPACE

#endif
