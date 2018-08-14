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

#ifndef QKNX2BITSET_H
#define QKNX2BITSET_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx2BitSet : public QKnxFixedSizeDatapointType
{
    Q_GADGET

public:
    QKnx2BitSet();
    explicit QKnx2BitSet(quint8 value);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x17;

    quint8 value() const;
    bool setValue(quint8 value);

    bool isValid() const override;

protected:
    explicit QKnx2BitSet(int subType, quint8 value);
};

class Q_KNX_EXPORT QKnxOnOffAction : public QKnx2BitSet
{
    Q_GADGET

public:
    enum class Action : quint8
    {
        Off = 0x00,
        On = 0x01,
        OffOn = 0x02,
        OnOff = 0x03
    };
    Q_ENUM(Action)

    QKnxOnOffAction();
    explicit QKnxOnOffAction(Action action);

    static const constexpr int SubType = 0x01;

    Action action() const;
    bool setAction(Action action);
};

class Q_KNX_EXPORT QKnxAlarmReaction : public QKnx2BitSet
{
    Q_GADGET

public:
    enum class Alarm : quint8
    {
        NoAlarm = 0x00,
        AlarmUp = 0x01,
        AlarmDown = 0x02
    };
    Q_ENUM(Alarm)

    QKnxAlarmReaction();
    explicit QKnxAlarmReaction(Alarm alarm);

    static const constexpr int SubType = 0x02;

    Alarm alarm() const;
    bool setAlarm(Alarm alarm);
};

class Q_KNX_EXPORT QKnxUpDownAction : public QKnx2BitSet
{
    Q_GADGET

public:
    enum class Action : quint8
    {
        Up = 0x00,
        Down = 0x01,
        UpDown = 0x02,
        DownUp = 0x03
    };
    Q_ENUM(Action)

    QKnxUpDownAction();
    explicit QKnxUpDownAction(Action action);

    static const constexpr int SubType = 0x03;

    Action action() const;
    bool setAction(Action action);
};

QT_END_NAMESPACE

#endif
