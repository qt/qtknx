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

#ifndef QKNX32BITSET_H
#define QKNX32BITSET_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx32BitSet : public QKnxFixedSizeDatapointType
{
public:
    QKnx32BitSet();
    explicit QKnx32BitSet(quint32 value);

    static const constexpr int TypeSize = 0x04;
    static const constexpr int MainType = 0x1b;

    quint32 value() const;
    bool setValue(quint32 value);

protected:
    QKnx32BitSet(int subType, quint32 value);
};

class Q_KNX_EXPORT QKnxCombinedInfoOnOff : public QKnx32BitSet
{
    Q_GADGET

public:
    enum class Output : quint8
    {
        First,
        Second,
        Third,
        Fourth,
        Fifth,
        Sixth,
        Seventh,
        Eighth,
        Ninth,
        Tenth,
        Eleventh,
        Twelfth,
        Thirteenth,
        Fourteenth,
        Fifteenth,
        Sixteenth
    };
    Q_ENUM(Output)

    enum class OutputState : quint8
    {
        Off = 0x00,
        On = 0x01
    };
    Q_ENUM(OutputState)

    enum class OutputValidity : quint8
    {
        Invalid = 0x00,
        Valid = 0x01
    };
    Q_ENUM(OutputValidity)

    struct Q_KNX_EXPORT OutputInfo
    {
        OutputInfo(QKnxCombinedInfoOnOff::Output o, QKnxCombinedInfoOnOff::OutputState os,
            QKnxCombinedInfoOnOff::OutputValidity osv)
            : Output(o), OutputState(os), OutputValidity(osv)
        {}

        QKnxCombinedInfoOnOff::Output Output;
        QKnxCombinedInfoOnOff::OutputState OutputState;
        QKnxCombinedInfoOnOff::OutputValidity OutputValidity;
    };

    QKnxCombinedInfoOnOff();
    explicit QKnxCombinedInfoOnOff(const QList<OutputInfo> &infos);
    QKnxCombinedInfoOnOff(Output output, OutputState state, OutputValidity validity);

    static const constexpr int SubType = 0x01;

    OutputState state(Output output) const;
    bool setState(Output output, OutputState state);

    OutputValidity validity(Output output) const;
    bool setValidity(Output output, OutputValidity validity);

    bool setValue(const QList<OutputInfo> &infos);
    bool setValue(Output output, OutputState state, OutputValidity validity);
};

QT_END_NAMESPACE

#endif
