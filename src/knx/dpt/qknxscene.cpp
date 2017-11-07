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

#include "qknxscene.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

// -- QKnxSceneNumber

QKnxSceneNumber::QKnxSceneNumber()
    : QKnxSceneNumber(0)
{}

QKnxSceneNumber::QKnxSceneNumber(quint8 number)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Scene Number"));
    setRange(QVariant(0x00), QVariant(0x3f));
    setRangeText(tr("Minimum, 0"), tr("Maximum, 63"));

    setSceneNumber(number);
}

quint8 QKnxSceneNumber::sceneNumber() const
{
    return byte(0) & 0x3f;
}

bool QKnxSceneNumber::setSceneNumber(quint8 number)
{
    if (number < 64)
        return setByte(0, number);
    return false;
}

bool QKnxSceneNumber::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}


// -- QKnxSceneControl

QKnxSceneControl::QKnxSceneControl()
    : QKnxSceneControl(0, QKnxSceneControl::Control::Activate)
{}

QKnxSceneControl::QKnxSceneControl(quint8 sceneNumber, QKnxSceneControl::Control control)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Scene Control"));
    setRange(QVariant(0x00), QVariant(0xbf));
    setRangeText(tr("Minimum scene Number, 0"), tr("Maximum scene number, 63"));

    setSceneNumber(sceneNumber);
    setControl(control);
}

quint8 QKnxSceneControl::sceneNumber() const
{
    return qint8(byte(0) & 0x3f);
}

bool QKnxSceneControl::setSceneNumber(quint8 number)
{
    if (number < 64)
        return setByte(0, number | quint8(byte(0) & 0xc0));
    return false;
}

QKnxSceneControl::Control QKnxSceneControl::control() const
{
    return Control(QKnxDatapointType::testBit(byte(0), 7));
}

bool QKnxSceneControl::setControl(Control control)
{
    if (control > Control::Learn)
        return false;
    return setByte(0, quint8(quint8(control) << 7) | quint8(byte(0) & 0x3f)) ;
}

bool QKnxSceneControl::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt()
        && !QKnxDatapointType::testBit(byte(0), 6); // bit 7 shall be unused
}

bool QKnxSceneControl::setValue(quint8 sceneNumber, Control control)
{
    return setSceneNumber(sceneNumber) && setControl(control);
}


// -- QKnxSceneInfo

QKnxSceneInfo::QKnxSceneInfo()
    : QKnxSceneInfo(0, QKnxSceneInfo::Info::Active)
{}

QKnxSceneInfo::QKnxSceneInfo(quint8 sceneNumber, QKnxSceneInfo::Info info)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Scene Information"));
    setRange(QVariant(0x00), QVariant(0x7f));
    setRangeText(tr("Minimum scene Number, 0"), tr("Maximum scene number, 63"));

    setSceneNumber(sceneNumber);
    setInfo(info);
}

quint8 QKnxSceneInfo::sceneNumber() const
{
    return qint8(byte(0) & 0x3f);
}

bool QKnxSceneInfo::setSceneNumber(quint8 number)
{
    if (number < 64)
        return setByte(0, number | quint8(byte(0) & 0xc0));
    return false;
}

QKnxSceneInfo::Info QKnxSceneInfo::info() const
{
    return Info(QKnxDatapointType::testBit(byte(0), 6));
}

bool QKnxSceneInfo::setInfo(Info info)
{
    if (info > Info::Inactive)
        return false;
    return setByte(0, quint8(quint8(info) << 6) | quint8(byte(0) & 0x3f)) ;
}

bool QKnxSceneInfo::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}

bool QKnxSceneInfo::setValue(quint8 sceneNumber, Info info)
{
    return setSceneNumber(sceneNumber) && setInfo(info);
}

#include "moc_qknxscene.cpp"

QT_END_NAMESPACE
