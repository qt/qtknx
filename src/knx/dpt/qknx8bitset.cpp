/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qknx8bitset.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx8BitSet

    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnx8BitSet class is a datapoint type with binary-coded values in
    all fields.

    This is a fixed size datapoint type with the length of 1 byte and the 8-bit
    set.

    The possible values are: \c{No bits set} (\c 0x00) and \c{All bits set}
    (\c 0xff).

    \sa QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

// -- QKnx8BitSet


/*!
    Creates a fixed size datapoint type with the 8-bit set to \c {0}.
*/
QKnx8BitSet::QKnx8BitSet()
    : QKnx8BitSet(0)
{}

/*!
    Creates a fixed size datapoint type with the value \a value.
*/
QKnx8BitSet::QKnx8BitSet(quint8 value)
    : QKnx8BitSet(SubType, value)
{}

/*!
    Creates a fixed size datapoint with the subtype \a subType and the value
    \a value.
*/
QKnx8BitSet::QKnx8BitSet(int subType, quint8 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("8-bit set"));
    setRange(QVariant(0x00), QVariant(0xff));
    setRangeText(tr("No bits set"), tr("All bits set"));

    setByte(value);
}

/*!
    Returns \c true if the bit at \a index is set, \c false otherwise.
*/
bool QKnx8BitSet::bit(int index) const
{
    return QKnxDatapointType::testBit(byte(), index);
}

/*!
    Sets the bit at \a index to \a value.
*/
void QKnx8BitSet::setBit(bool value, int index)
{
    setByte(QKnxDatapointType::setBit(byte(), value, index));
}

/*!
    Creates a fixed size datapoint type with the value set to \c {0}.
*/
quint8 QKnx8BitSet::byte() const
{
    return QKnxDatapointType::byte(0);
}

/*!
    Sets the value of the datapoint type to \a value.

    Returns \c true if the value was set; otherwise returns \c false.
*/

bool QKnx8BitSet::setByte(quint8 value)
{
    return QKnxDatapointType::setByte(0, value);
}

/*!
    \reimp
*/
bool QKnx8BitSet::isValid() const
{
    return QKnxDatapointType::isValid() && byte() <= maximum().toUInt();
}


// -- QKnxGeneralStatus

/*!
    \class QKnxGeneralStatus
    \inherits QKnx8BitSet
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxGeneralStatus class is a datapoint type for general status.

    This is a fixed size datapoint type with the length of 1 byte.

    The possible values are combinations of the attributes specified by
    QKnxGeneralStatus::Attribute.

    \sa QKnx8BitSet, QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnxGeneralStatus::Attribute

    This enum type holds the status attributes.

    \value OutOfService
    \value Fault
    \value Overridden
    \value InAlarm
    \value AlarmUnacknowledged

*/

/*!
    Creates a fixed size datapoint type with an empty list of  attributes.
*/
QKnxGeneralStatus::QKnxGeneralStatus()
    : QKnxGeneralStatus(Attributes())
{}

/*!
    Creates a fixed size datapoint type with the attributes \a attributes.
*/
QKnxGeneralStatus::QKnxGeneralStatus(Attributes attributes)
    : QKnx8BitSet(SubType, 0)
{
    setDescription(tr("General Status"));
    setRange(QVariant(0x00), QVariant(0x1f));
    setValue(attributes);
}

/*!
    Returns the value stored in the datapoint type as a list of attributes.
*/
QKnxGeneralStatus::Attributes QKnxGeneralStatus::value() const
{
    auto value = byte();
    return Attributes().setFlag(Attribute::OutOfService, QKnxDatapointType::testBit(value, 0))
        .setFlag(Attribute::Fault, QKnxDatapointType::testBit(value, 1))
        .setFlag(Attribute::Overridden, QKnxDatapointType::testBit(value, 2))
        .setFlag(Attribute::InAlarm, QKnxDatapointType::testBit(value, 3))
        .setFlag(Attribute::AlarmUnacknowledged, QKnxDatapointType::testBit(value, 4));
}

/*!
    Sets the value of the datapoint type to the list of attributes specified by
    \a attributes.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxGeneralStatus::setValue(Attributes attributes)
{
    quint8 value = byte();
    value = QKnxDatapointType::setBit(value, attributes.testFlag(Attribute::OutOfService), 0);
    value = QKnxDatapointType::setBit(value, attributes.testFlag(Attribute::Fault), 1);
    value = QKnxDatapointType::setBit(value, attributes.testFlag(Attribute::Overridden), 2);
    value = QKnxDatapointType::setBit(value, attributes.testFlag(Attribute::InAlarm), 3);
    value = QKnxDatapointType::setBit(value, attributes.testFlag(Attribute::AlarmUnacknowledged), 4);
    return setByte(value);
}

/*!
    Returns whether the attribute \a attribute is set in the datapoint type.
*/
bool QKnxGeneralStatus::isSet(Attribute attribute) const
{
    return value().testFlag(attribute);
}

/*!
    Sets the attribute \a attribute within the value of the datapoint type.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxGeneralStatus::setAttribute(Attribute attribute)
{
    return setValue(value() | attribute);
}

/*!
    Removes the attribute \a attribute from the value of the datapoint type.

    Returns \c true if the value was removed; otherwise returns \c false.
*/
bool QKnxGeneralStatus::removeAttribute(Attribute attribute)
{
    return setValue(value() &~ attribute);
}


// -- QKnxDeviceControl

/*!
    \class QKnxDeviceControl
    \inherits QKnx8BitSet
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxDeviceControl class is a datapoint type for device control.

    This is a fixed size datapoint type with the length of 1 byte.

    The possible values are combinations of the attributes specified by
    QKnxGeneralStatus::Attribute.

    \sa QKnx8BitSet, QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnxDeviceControl::Attribute

    This enum type holds the control attributes.

    \value UserStopped
    \value OwnIA
    \value VerifyMode
    \value SafeState
*/

/*!
    Creates a fixed size datapoint type with an empty list of attributes.
*/
QKnxDeviceControl::QKnxDeviceControl()
    : QKnxDeviceControl(Attributes())
{}

/*!
    Creates a fixed size datapoint type with the attributes \a attributes.
*/
QKnxDeviceControl::QKnxDeviceControl(Attributes attributes)
    : QKnx8BitSet(SubType, 0)
{
    setDescription(tr("Device Control"));
    setRange(QVariant(0x00), QVariant(0x15));
    setValue(attributes);
}

/*!
    Returns the value stored in the datapoint type as a list of attributes.
*/
QKnxDeviceControl::Attributes QKnxDeviceControl::value() const
{
    auto value = byte();
    return Attributes().setFlag(Attribute::UserStopped, QKnxDatapointType::testBit(value, 0))
        .setFlag(Attribute::OwnIA, QKnxDatapointType::testBit(value, 1))
        .setFlag(Attribute::VerifyMode, QKnxDatapointType::testBit(value, 2))
        .setFlag(Attribute::SafeState, QKnxDatapointType::testBit(value, 3));
}

/*!
    Sets the value of the datapoint type to the list of attributes specified by
    \a attributes.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxDeviceControl::setValue(Attributes attributes)
{
    quint8 value = byte();
    value = QKnxDatapointType::setBit(value, attributes.testFlag(Attribute::UserStopped), 0);
    value = QKnxDatapointType::setBit(value, attributes.testFlag(Attribute::OwnIA), 1);
    value = QKnxDatapointType::setBit(value, attributes.testFlag(Attribute::VerifyMode), 2);
    value = QKnxDatapointType::setBit(value, attributes.testFlag(Attribute::SafeState), 3);
    return setByte(value);
}

/*!
    Returns whether the attribute \a attribute is set in the datapoint type.
*/
bool QKnxDeviceControl::isSet(Attribute attribute) const
{
    return value().testFlag(attribute);
}

/*!
    Sets the attribute \a attribute within the value of the datapoint type.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxDeviceControl::setAttribute(Attribute attribute)
{
    return setValue(value() | attribute);
}

/*!
    Removes the attribute \a attribute from the value of the datapoint type.

    Returns \c true if the value was removed; otherwise returns \c false.
*/
bool QKnxDeviceControl::removeAttribute(Attribute attribute)
{
    return setValue(value() &~ attribute);
}

#include "moc_qknx8bitset.cpp"

QT_END_NAMESPACE
