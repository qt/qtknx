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

#include "qknx32bitset.h"
#include "qknxdatapointtype_p.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx32BitSet
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnx32BitSet class is a datapoint type with binary-coded values
    in all fields.

    This is a fixed size datapoint type with the length of 4 bytes.

    There is only one class inheriting QKnx32BitSet: \l QKnxCombinedInfoOnOff.

    \sa QKnxDatapointType
*/

// -- QKnx32BitSet

QKnx32BitSet::QKnx32BitSet()
    : QKnx32BitSet(0)
{}

QKnx32BitSet::QKnx32BitSet(quint32 value)
    : QKnx32BitSet(SubType, value)
{}

QKnx32BitSet::QKnx32BitSet(int subType, quint32 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("32-bit set"));
    setRange(QVariant(0x00), QVariant(0xffffffff));
    setRangeText(tr("No bits set"), tr("All bits set"));
    setValue(value);
}

quint32 QKnx32BitSet::value() const
{
    return QKnxUtils::QUint32::fromBytes(bytes());
}

bool QKnx32BitSet::setValue(quint32 value)
{
    return setBytes(QKnxUtils::QUint32::bytes(value), 0, 4);
}


// -- QKnxCombinedInfoOnOff

QKnxCombinedInfoOnOff::QKnxCombinedInfoOnOff()
    : QKnxCombinedInfoOnOff(Output::First, OutputState::Off, OutputValidity::Invalid)
{}

QKnxCombinedInfoOnOff::QKnxCombinedInfoOnOff(const QVector<OutputInfo> &infos)
    : QKnx32BitSet(SubType, 0)
{
    setDescription(tr("Bit-combined info On/Off"));

    for (const auto &info : qAsConst(infos))
        setValue(info.Output, info.OutputState, info.OutputValidity);
}

QKnxCombinedInfoOnOff::QKnxCombinedInfoOnOff(Output output, OutputState state, OutputValidity validity)
    : QKnxCombinedInfoOnOff({ { output, state, validity } })
{}

QKnxCombinedInfoOnOff::OutputState QKnxCombinedInfoOnOff::state(Output output) const
{
    return OutputState(QKnxDatapointType::testBit(value(), quint8(output)));
}

bool QKnxCombinedInfoOnOff::setState(Output output, OutputState state)
{
    if (output > Output::Sixteenth || state > OutputState::On)
        return false;
    return QKnx32BitSet::setValue(QKnxDatapointType::setBit(value(), bool(state), quint8(output)));
}

QKnxCombinedInfoOnOff::OutputValidity QKnxCombinedInfoOnOff::validity(Output output) const
{
    return OutputValidity(QKnxDatapointType::testBit(value(), quint8(output) + 16));
}

bool QKnxCombinedInfoOnOff::setValidity(Output output, OutputValidity validity)
{
    if (output > Output::Sixteenth || validity > OutputValidity::Valid)
        return false;
    return QKnx32BitSet::setValue(QKnxDatapointType::setBit(value(), bool(validity),
        quint8(output) + 16));
}

bool QKnxCombinedInfoOnOff::setValue(const QVector<OutputInfo>& infos)
{
    bool success = true;
    for (const auto &info : qAsConst(infos))
        success |= setValue(info.Output, info.OutputState, info.OutputValidity);
    return success;
}

bool QKnxCombinedInfoOnOff::setValue(Output output, OutputState state, OutputValidity validity)
{
    return setState(output, state) && setValidity(output, validity);
}

#include "moc_qknx32bitset.cpp"

QT_END_NAMESPACE
