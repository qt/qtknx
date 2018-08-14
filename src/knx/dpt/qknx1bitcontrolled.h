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

#ifndef QKNX1BITCONTROLLED_H
#define QKNX1BITCONTROLLED_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx1BitControlled : public QKnxFixedSizeDatapointType
{
public:
    QKnx1BitControlled();

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x02;

    bool valueBit() const;
    void setValueBit(bool value);

    bool controlBit() const;
    void setControlBit(bool control);

    bool isValid() const override;

protected:
    QKnx1BitControlled(int subType, bool state, bool control);
    QKnx1BitControlled(int subType, const QKnxDatapointType &dpt, bool state, bool control);
};

#define CREATE_CLASS_DECLARATION(CLASS, SUB_TYPE, STATE_1, STATE_2) \
class Q_KNX_EXPORT CLASS : public QKnx1BitControlled \
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
    enum class Control : quint8 \
    { \
        NoControl, \
        Control \
    }; \
    Q_ENUM(Control) \
\
    CLASS(); \
    CLASS(State state, Control control); \
\
    static const constexpr int SubType = SUB_TYPE; \
\
    State state() const; \
    void setState(State state); \
\
    Control control() const; \
    void setControl(Control control); \
\
    void setValue(State state, Control control); \
};

CREATE_CLASS_DECLARATION(QKnxSwitchControl, 0x01, Off, On)
CREATE_CLASS_DECLARATION(QKnxBoolControl, 0x02, False, True)
CREATE_CLASS_DECLARATION(QKnxEnableControl, 0x03, Disable, Enable)
CREATE_CLASS_DECLARATION(QKnxRampControl, 0x04, NoRamp, Ramp)
CREATE_CLASS_DECLARATION(QKnxAlarmControl, 0x05, NoAlarm, Alarm)
CREATE_CLASS_DECLARATION(QKnxBinaryValueControl, 0x06, Low, High)
CREATE_CLASS_DECLARATION(QKnxStepControl, 0x07, Decrease, Increase)
CREATE_CLASS_DECLARATION(QKnxDirection1Control, 0x08, Up, Down)
CREATE_CLASS_DECLARATION(QKnxDirection2Control, 0x09, Open, Close)
CREATE_CLASS_DECLARATION(QKnxStartControl, 0x0a, Stop, Start)
CREATE_CLASS_DECLARATION(QKnxStateControl, 0x0b, Inactive, Active)
CREATE_CLASS_DECLARATION(QKnxInvertControl, 0x0c, NotInverted, Inverted)

#undef CREATE_CLASS_DECLARATION

QT_END_NAMESPACE

#endif
