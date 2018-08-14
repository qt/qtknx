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

#ifndef QKNX3BITCONTROLLED_H
#define QKNX3BITCONTROLLED_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx3BitControlled : public QKnxFixedSizeDatapointType
{
    Q_GADGET

public:
    enum NumberOfIntervals : quint8
    {
        Break = 0x00,
        One = 0x01,
        Two = 0x02,
        Four = 0x04,
        Eight = 0x08,
        Sixteen = 0x10,
        ThirtyTwo = 0x20
    };
    Q_ENUM(NumberOfIntervals)

    QKnx3BitControlled();
    QKnx3BitControlled(bool control, NumberOfIntervals n);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x03;

    bool controlBit() const;
    void setControlBit(bool control);

    NumberOfIntervals numberOfIntervals() const;
    bool setNumberOfIntervals(NumberOfIntervals n);

    bool isValid() const override;

protected:
    explicit QKnx3BitControlled(int subType, bool control, NumberOfIntervals n);
};

class Q_KNX_EXPORT QKnxControlDimming : public QKnx3BitControlled
{
    Q_GADGET

public:
    enum Control : quint8
    {
        Decrease = 0x00,
        Increase = 0x01
    };
    Q_ENUM(Control)

    QKnxControlDimming();
    QKnxControlDimming(Control control, NumberOfIntervals interval);

    static const constexpr int SubType = 0x07;

    Control control() const;
    void setControl(Control control);
};

class Q_KNX_EXPORT QKnxControlBlinds : public QKnx3BitControlled
{
    Q_GADGET

public:
    enum Control : quint8
    {
        Up = 0x00,
        Down = 0x01
    };
    Q_ENUM(Control)

    QKnxControlBlinds();
    QKnxControlBlinds(Control control, NumberOfIntervals interval);

    static const constexpr int SubType = 0x08;

    Control control() const;
    void setControl(Control control);
};

QT_END_NAMESPACE

#endif
