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

#include "qknx1bit.h"
#include "qknx1bitcontrolled.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE


// -- QKnx1BitControlled

QKnx1BitControlled::QKnx1BitControlled()
    : QKnx1BitControlled(SubType)
{}

QKnx1BitControlled::QKnx1BitControlled(int subType)
    : QKnxDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("1-bit controlled"));

    setRange(QVariant(0x00), QVariant(0x03));
    setRangeText(tr("No control, false"), tr("Controlled, true"));
}

QKnx1BitControlled::QKnx1BitControlled(int subType, const QKnxDatapointType &dpt)
    : QKnx1BitControlled(subType)
{
    setMinimumText(tr("No control, %1").arg(dpt.minimumText()));
    setMaximumText(tr("Controlled, %1").arg(dpt.maximumText()));
}

bool QKnx1BitControlled::valueBit() const
{
    return QKnxDatapointType::testBit(byte(0), 0);
}

void QKnx1BitControlled::setValueBit(bool value)
{
    QKnxDatapointType::setBit(&(operator[](0)), value, 0);
}

bool QKnx1BitControlled::controlBit() const
{
    return QKnxDatapointType::testBit(byte(0), 1);
}

void QKnx1BitControlled::setControlBit(bool control)
{
    QKnxDatapointType::setBit(&(operator[](0)), control, 1);
}

bool QKnx1BitControlled::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}


// QKnxSwitchControlled

QKnxSwitchControl::QKnxSwitchControl()
    : QKnxSwitchControl(Attributes())
{}

QKnxSwitchControl::QKnxSwitchControl(Attributes attributes)
    : QKnx1BitControlled(SubType, QKnxSwitch())
{
    setDescription(tr("Switch Controlled"));
    setValue(attributes);
}

void QKnxSwitchControl::setValue(Attributes attributes)
{
    QKnx1BitControlled::setValueBit(attributes.testFlag(Attribute::On));
    QKnx1BitControlled::setControlBit(attributes.testFlag(Attribute::Controlled));
}

QKnxSwitchControl::Attributes QKnxSwitchControl::value() const
{
    return Attributes().setFlag(Attribute::Controlled, controlBit())
        .setFlag(Attribute::On, valueBit());
}

bool QKnxSwitchControl::isSet(Attribute attribute) const
{
    return value().testFlag(attribute);
}

void QKnxSwitchControl::setAttribute(Attribute attribute)
{
    setValue(value() | attribute);
}

void QKnxSwitchControl::removeAttribute(Attribute attribute)
{
    setValue(value() &~ attribute);
}

#include "moc_qknx1bitcontrolled.cpp"

QT_END_NAMESPACE
