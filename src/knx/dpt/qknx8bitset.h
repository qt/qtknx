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

#ifndef QKNX8BITSET_H
#define QKNX8BITSET_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx8BitSet : public QKnxDatapointType
{
public:
    QKnx8BitSet();
    explicit QKnx8BitSet(quint8 value);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x15;

    bool bit(int index) const;
    void setBit(bool value, int index);

    quint8 byte() const;
    void setByte(quint8 value);

    bool isValid() const override;

protected:
    explicit QKnx8BitSet(int subType);
};

class Q_KNX_EXPORT QKnxGeneralStatus : public QKnx8BitSet
{
    Q_GADGET

public:
    enum Attribute : quint8
    {
        OutOfService = 0x01,
        Fault = 0x02,
        Overridden = 0x04,
        InAlarm = 0x08,
        AlarmUnacknowledged = 0x10
    };
    Q_ENUM(Attribute)
    Q_DECLARE_FLAGS(Attributes, Attribute)

    QKnxGeneralStatus();
    explicit QKnxGeneralStatus(Attributes attributes);

    static const constexpr int SubType = 0x01;

    Attributes value() const;
    void setValue(Attributes attributes);

    bool isSet(Attribute attribute) const;

    void setAttribute(Attribute attribute);
    void removeAttribute(Attribute attribute);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxGeneralStatus::Attributes)

QT_END_NAMESPACE

#endif
