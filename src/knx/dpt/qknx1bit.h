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
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx1Bit : public QKnxFixedSizeDatapointType
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

#define CREATE_CLASS_DECLARATION(CLASS, SUB_TYPE, STATE_1, STATE_2) \
class Q_KNX_EXPORT CLASS : public QKnx1Bit \
{ \
    Q_GADGET \
\
public: \
    enum class State : quint8 \
    { \
        STATE_1, \
        STATE_2 \
    }; \
    Q_ENUM(State) \
\
    CLASS(); \
    explicit CLASS(State state); \
\
    static const constexpr int SubType = SUB_TYPE; \
\
    State value() const; \
    bool setValue(State state); \
};

CREATE_CLASS_DECLARATION(QKnxSwitch, 0x01, Off, On)
CREATE_CLASS_DECLARATION(QKnxBool, 0x02, False, True)
CREATE_CLASS_DECLARATION(QKnxEnable, 0x03, Disable, Enable)
CREATE_CLASS_DECLARATION(QKnxRamp, 0x04, NoRamp, Ramp)
CREATE_CLASS_DECLARATION(QKnxAlarm, 0x05, NoAlarm, Alarm)
CREATE_CLASS_DECLARATION(QKnxBinaryValue, 0x06, Low, High)
CREATE_CLASS_DECLARATION(QKnxStep, 0x07, Decrease, Increase)
CREATE_CLASS_DECLARATION(QKnxUpDown, 0x08, Up, Down)
CREATE_CLASS_DECLARATION(QKnxOpenClose, 0x09, Open, Close)
CREATE_CLASS_DECLARATION(QKnxStart, 0x0a, Stop, Start)
CREATE_CLASS_DECLARATION(QKnxState, 0x0b, Inactive, Active)
CREATE_CLASS_DECLARATION(QKnxInvert, 0x0c, NotInverted, Inverted)
CREATE_CLASS_DECLARATION(QKnxDimSendStyle, 0x0d, StartStop, Cyclically)
CREATE_CLASS_DECLARATION(QKnxInputSource, 0x0e, Fixed, Calculated)
CREATE_CLASS_DECLARATION(QKnxReset, 0x0f, NoAction, ResetCommand)
CREATE_CLASS_DECLARATION(QKnxAck, 0x10, NoAction, AcknowledgeCommand)
CREATE_CLASS_DECLARATION(QKnxTrigger, 0x11, Trigger, TriggerAlso)
CREATE_CLASS_DECLARATION(QKnxOccupancy, 0x12, NotOccupied, Occupied)
CREATE_CLASS_DECLARATION(QKnxWindowDoor, 0x13, Closed, Open)
CREATE_CLASS_DECLARATION(QKnxLogicalFunction, 0x15, OR, AND)
CREATE_CLASS_DECLARATION(QKnxSceneAB, 0x16, SceneA, SceneB)
CREATE_CLASS_DECLARATION(QKnxShutterBlindsMode, 0x17, OnlyMoveUpDownShutter,
    MoveUpDownAndStepModeBlind)
CREATE_CLASS_DECLARATION(QKnxHeatCool, 0x64, Cooling, Heating)

#undef CREATE_CLASS_DECLARATION

QT_END_NAMESPACE

#endif
