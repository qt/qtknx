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

    \brief The QKnx32BitSet class is a datapoint type with the 32-bit set.

    This is a fixed size datapoint type with the length of 4 bytes.

    The acceptable values are \c {No bits set} (\c 0x00) and \c {All bits set}
    (\c 0xffffffff).

    There is only one class inheriting QKnx32BitSet: \l QKnxCombinedInfoOnOff.

    \sa QKnxDatapointType
*/

// -- QKnx32BitSet

/*!
    Creates a fixed size datapoint type with the 32-bit set to \c 0.
*/
QKnx32BitSet::QKnx32BitSet()
    : QKnx32BitSet(0)
{}

/*!
    Creates a fixed size datapoint type with the value \a value.
*/
QKnx32BitSet::QKnx32BitSet(quint32 value)
    : QKnx32BitSet(SubType, value)
{}

/*!
    Creates a fixed size datapoint with the subtype \a subType and the value
    \a value.
*/
QKnx32BitSet::QKnx32BitSet(int subType, quint32 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("32-bit set"));
    setRange(QVariant(0x00), QVariant(0xffffffff));
    setRangeText(tr("No bits set"), tr("All bits set"));
    setValue(value);
}

/*!
    Returns the value stored in the datapoint type.
*/
quint32 QKnx32BitSet::value() const
{
    return QKnxUtils::QUint32::fromBytes(bytes());
}

/*!
    Sets the value of the datapoint type to \a value.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnx32BitSet::setValue(quint32 value)
{
    return setBytes(QKnxUtils::QUint32::bytes(value), 0, 4);
}


// -- QKnxCombinedInfoOnOff
/*!
    \class QKnxCombinedInfoOnOff
    \inherits QKnx32BitSet
    \inmodule QtKnx

    \brief The QKnxCombinedInfoOnOff class is a datapoint type for storing
    combined information about the on and off states of a multiple-channel
    binary actuator.

    This datapoint type may only be used for encoding the combined binary output
    information of a multiple-channel binary actuator. It avoids the bus load
    that is caused by individual single bit state outputs, especially in case of
    simultaneous changes, such as \e {all off}.

    If one or more output bits are not used or the output states are not valid,
    the assigned mask bits of the output shall be set to the value \c 0.

    This is a fixed size datapoint type with the length of 4 bytes.

    \sa QKnx32BitSet, QKnxDatapointType
*/

/*!

    \enum QKnxCombinedInfoOnOff::Output

    This enum type holds the output bit stored in the datapoint type.

    \value First
    \value Second
    \value Third
    \value Fourth
    \value Fifth
    \value Sixth
    \value Seventh
    \value Eighth
    \value Ninth
    \value Tenth
    \value Eleventh
    \value Twelfth
    \value Thirteenth
    \value Fourteenth
    \value Fifteenth
    \value Sixteenth
*/

/*!

    \enum QKnxCombinedInfoOnOff::OutputState

    This enum type holds the state of the output stored in the datapoint type.

    \value Off
           The output state is \c Off.
    \value On
           The output state is \c On.
*/

/*!

    \enum QKnxCombinedInfoOnOff::OutputValidity

    This enum type holds the validity of the output stored in the datapoint
    type.

    \value Invalid
           The output state is invalid.
    \value Valid
           The output state is valid.
*/

/*!
    Creates a fixed size datapoint type with the output bit set to
    QKnxCombinedInfoOnOff::First, output state to QKnxCombinedInfoOnOff::Off,
    and output validity to QKnxCombinedInfoOnOff::Invalid.
*/
QKnxCombinedInfoOnOff::QKnxCombinedInfoOnOff()
    : QKnxCombinedInfoOnOff(Output::First, OutputState::Off, OutputValidity::Invalid)
{}

/*!
    Creates a fixed size datapoint type with the output information \a infos.
*/
QKnxCombinedInfoOnOff::QKnxCombinedInfoOnOff(const QVector<OutputInfo> &infos)
    : QKnx32BitSet(SubType, 0)
{
    setDescription(tr("Bit-combined info On/Off"));

    for (const auto &info : qAsConst(infos))
        setValue(info.Output, info.OutputState, info.OutputValidity);
}

/*!
    Creates a fixed size datapoint type with the output bit \a output, state
    \a state, and validity \a validity.
*/
QKnxCombinedInfoOnOff::QKnxCombinedInfoOnOff(Output output, OutputState state, OutputValidity validity)
    : QKnxCombinedInfoOnOff({ { output, state, validity } })
{}

/*!
    Returns the state of the output bit \a output.
*/
QKnxCombinedInfoOnOff::OutputState QKnxCombinedInfoOnOff::state(Output output) const
{
    return OutputState(QKnxDatapointType::testBit(value(), quint8(output)));
}

/*!
    Sets the output bit stored in the datapoint type to \a output and the
    state to \a state.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxCombinedInfoOnOff::setState(Output output, OutputState state)
{
    if (output > Output::Sixteenth || state > OutputState::On)
        return false;
    return QKnx32BitSet::setValue(QKnxDatapointType::setBit(value(), bool(state), quint8(output)));
}

/*!
    Returns the validity of the output bit \a output.
*/
QKnxCombinedInfoOnOff::OutputValidity QKnxCombinedInfoOnOff::validity(Output output) const
{
    return OutputValidity(QKnxDatapointType::testBit(value(), quint8(output) + 16));
}

/*!
    Sets the output bit stored in the datapoint type to \a output and the
    validity to \a validity.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxCombinedInfoOnOff::setValidity(Output output, OutputValidity validity)
{
    if (output > Output::Sixteenth || validity > OutputValidity::Valid)
        return false;
    return QKnx32BitSet::setValue(QKnxDatapointType::setBit(value(), bool(validity),
        quint8(output) + 16));
}

/*!
    Sets the value stored in the datapoint type to \a infos.

    Returns \c true if the value was set successfully; otherwise returns
    \c false.
*/
bool QKnxCombinedInfoOnOff::setValue(const QVector<OutputInfo>& infos)
{
    bool success = true;
    for (const auto &info : qAsConst(infos))
        success |= setValue(info.Output, info.OutputState, info.OutputValidity);
    return success;
}

/*!
    Sets the output bit stored in the datapoint type to \a output, the output
    state to \a state, and output validity to \a validity.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxCombinedInfoOnOff::setValue(Output output, OutputState state, OutputValidity validity)
{
    return setState(output, state) && setValidity(output, validity);
}

#include "moc_qknx32bitset.cpp"

QT_END_NAMESPACE
