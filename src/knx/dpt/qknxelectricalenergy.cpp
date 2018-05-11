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

#include "qknxelectricalenergy.h"
#include "qknxdatapointtype_p.h"

#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxElectricalEnergy
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxElectricalEnergy class is a datapoint type for electrical
    energy.

    This is a fixed size datapoint type with the length of 8 bytes.

    It is a base class for the following classes:

    \list
        \li \c QKnxActiveEnergyV64 - Active energy in Wh.
        \li \c QKnxApparentEnergyV64 - Active energy in VAh.
        \li \c QKnxReactiveEnergyV64 - Active energy in VARh.
    \endlist

    The range for the value is from \c {-9 223 372 036 854 775 808} to
    \c {9 223 372 036 854 775 807}.

    \sa QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

/*!
    Creates a fixed size datapoint type with the value set to \c 0.
*/
QKnxElectricalEnergy::QKnxElectricalEnergy()
    : QKnxElectricalEnergy(0)
{}

/*!
    Creates a fixed size datapoint type with the value \a value.
*/
QKnxElectricalEnergy::QKnxElectricalEnergy(qint64 value)
    : QKnxElectricalEnergy(SubType, value)
{}

/*!
    Creates a fixed size datapoint with the subtype \a subType and the value
    \a value.

*/
QKnxElectricalEnergy::QKnxElectricalEnergy(int subType, qint64 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("8-byte signed value"));
    setRange(QVariant::fromValue(LONG_MIN), QVariant::fromValue(LONG_MAX));
    setRangeText(tr("Minimum Value, -9 223 372 036 854 775 808"),
        tr("Maximum Value, 9 223 372 036 854 775 807"));

    setValue(value);
}

/*!
    Returns the value stored in the datapoint type.
*/
qint64 QKnxElectricalEnergy::value() const
{
    return qint64(QKnxUtils::QUint64::fromBytes(bytes()));
}

/*!
    Sets the value of the datapoint type to \a value.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxElectricalEnergy::setValue(qint64 value)
{
    if (value <= maximum().toLongLong() && value >= minimum().toLongLong())
        return setBytes(QKnxUtils::QUint64::bytes(quint64(value)), 0, 8);
    return false;
}

/*!
    \reimp
*/
bool QKnxElectricalEnergy::isValid() const
{
    return QKnxDatapointType::isValid()
        && value() >= minimum().toLongLong() && value() <= maximum().toLongLong();
}

#define CREATE_CLASS_BODY(CLASS, DESCRIPTION, UNIT) \
CLASS::CLASS() \
    : QKnxElectricalEnergy(SubType, 0) \
{ \
    setUnit(tr(UNIT)); \
    setDescription(tr(DESCRIPTION)); \
} \
CLASS::CLASS(qint64 value) \
    : CLASS() \
{ \
    setValue(value); \
}

CREATE_CLASS_BODY(QKnxActiveEnergyV64, "Active energy (Wh)", "Wh");
CREATE_CLASS_BODY(QKnxApparentEnergyV64, "Active energy (VAh)", "VAh");
CREATE_CLASS_BODY(QKnxReactiveEnergyV64, "Active energy (VARh)", "VARh");

#undef CREATE_CLASS_BODY

QT_END_NAMESPACE
