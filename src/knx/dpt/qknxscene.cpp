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

/*!
    \class QKnxSceneNumber

    \inmodule QtKnx
    \brief The QKnxSceneNumber class is a datapoint type for a scene number.

    This is a fixed size datapoint type with the length of 1 byte.

    The minimum scene number is \c 0 and the maximum scene number is \c 63.

    \sa QKnxDatapointType, QKnxSceneControl, QKnxSceneInfo
*/

/*!
    \class QKnxSceneControl

    \inmodule QtKnx
    \brief The QKnxSceneControl class is a datapoint type for a scene control.

    This is a fixed size datapoint type with the length of 1 byte.

    A scene control can be used to activate or learn the corresponding scene
    number. The range for scene numbers is from \c 0 to \c 63.

    \sa Control, QKnxDatapointType, QKnxSceneNumber, QKnxSceneInfo
*/

/*!
    \enum QKnxSceneControl::Control

    This enum holds the action for the scene control.

    \value Activate
    Activate the scene corresponding to the scene number.
    \value Learn
    Learn the scene corresponding to the scene number.
*/

/*!
    \class QKnxSceneInfo

    \inmodule QtKnx
    \brief The QKnxSceneInfo class is a datapoint type for scene information.

    This is a fixed size datapoint type with the length of 1 byte.

    The scene information holds the activity state of the corresponding scene
    number. The range for scene numbers is from \c 0 to \c 63.

    \sa Info, QKnxDatapointType, QKnxSceneNumber, QKnxSceneControl
*/

/*!
    \enum QKnxSceneInfo::Info

    This enum holds holds whether the scene is active.

    \value Active
    The scene is active.
    \value Inactive
    The scene is inactive.
*/

// -- QKnxSceneNumber

/*!
    Creates a fixed size datapoint type with the scene number set to \c 0.
*/
QKnxSceneNumber::QKnxSceneNumber()
    : QKnxSceneNumber(0)
{}
/*!
    Creates a fixed size datapoint type with the scene number set to \a number.
*/
QKnxSceneNumber::QKnxSceneNumber(quint8 number)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Scene Number"));
    setRange(QVariant(0x00), QVariant(0x3f));
    setRangeText(tr("Minimum, 0"), tr("Maximum, 63"));

    setSceneNumber(number);
}

/*!
    Returns the scene number stored in the datapoint type.
*/
quint8 QKnxSceneNumber::sceneNumber() const
{
    return byte(0) & 0x3f;
}

/*!
    Sets the scene number stored in the datapoint type to \a number.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxSceneNumber::setSceneNumber(quint8 number)
{
    if (number < 64)
        return setByte(0, number);
    return false;
}

/*!
    \reimp
*/
bool QKnxSceneNumber::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}


// -- QKnxSceneControl

/*!
    Creates a fixed size datapoint type with the scene number set to \c 0 and
    the scene control action set to \c Activate.

    \sa Control
*/
QKnxSceneControl::QKnxSceneControl()
    : QKnxSceneControl(0, QKnxSceneControl::Control::Activate)
{}

/*!
    Creates a fixed size datapoint type with the scene number set to
    \a sceneNumber and the scene control action set to \a control.

    \sa Control
*/
QKnxSceneControl::QKnxSceneControl(quint8 sceneNumber, QKnxSceneControl::Control control)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Scene Control"));
    setRange(QVariant(0x00), QVariant(0xbf));
    setRangeText(tr("Minimum scene number, 0"), tr("Maximum scene number, 63"));

    setSceneNumber(sceneNumber);
    setControl(control);
}

/*!
    Returns the scene number stored in the datapoint type.
*/
quint8 QKnxSceneControl::sceneNumber() const
{
    return qint8(byte(0) & 0x3f);
}

/*!
    Sets the scene number stored in the datapoint type to \a number.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxSceneControl::setSceneNumber(quint8 number)
{
    if (number < 64)
        return setByte(0, number | quint8(byte(0) & 0xc0));
    return false;
}

/*!
    Returns the scene control action stored in the datapoint type.
*/
QKnxSceneControl::Control QKnxSceneControl::control() const
{
    return Control(QKnxDatapointType::testBit(byte(0), 7));
}

/*!
    Sets the scene control action stored in the datapoint type to \a control.

    Returns \c true if the value was set successfully.
*/
bool QKnxSceneControl::setControl(Control control)
{
    if (control > Control::Learn)
        return false;
    return setByte(0, quint8(quint8(control) << 7) | quint8(byte(0) & 0x3f)) ;
}

/*!
    \reimp
*/
bool QKnxSceneControl::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt()
        && !QKnxDatapointType::testBit(byte(0), 6); // bit 7 shall be unused
}

/*!
    Sets the scene number stored in the datapoint type to \a number and the
    scene control action to \a control.
*/
bool QKnxSceneControl::setValue(quint8 sceneNumber, Control control)
{
    return setSceneNumber(sceneNumber) && setControl(control);
}


// -- QKnxSceneInfo

/*!
    Creates a fixed size datapoint type with the scene information set to
    \c Active.

    \sa Info
*/
QKnxSceneInfo::QKnxSceneInfo()
    : QKnxSceneInfo(0, QKnxSceneInfo::Info::Active)
{}

/*!
    Creates a fixed size datapoint type with the scene number set to
    \a sceneNumber and the scene information set to \a info.

    \sa Info
*/
QKnxSceneInfo::QKnxSceneInfo(quint8 sceneNumber, QKnxSceneInfo::Info info)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Scene Information"));
    setRange(QVariant(0x00), QVariant(0x7f));
    setRangeText(tr("Minimum scene number, 0"), tr("Maximum scene number, 63"));

    setSceneNumber(sceneNumber);
    setInfo(info);
}

/*!
    Returns the scene number stored in the datapoint type.
*/
quint8 QKnxSceneInfo::sceneNumber() const
{
    return qint8(byte(0) & 0x3f);
}

/*!
    Sets the scene number stored in the datapoint type to \a number.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxSceneInfo::setSceneNumber(quint8 number)
{
    if (number < 64)
        return setByte(0, number | quint8(byte(0) & 0xc0));
    return false;
}

/*!
    Returns the scene information stored in the datapoint type.
*/
QKnxSceneInfo::Info QKnxSceneInfo::info() const
{
    return Info(QKnxDatapointType::testBit(byte(0), 6));
}

/*!
    Sets the scene information stored in the datapoint type to \a info.

    Returns \c true if the value was set successfully.
*/
bool QKnxSceneInfo::setInfo(Info info)
{
    if (info > Info::Inactive)
        return false;
    return setByte(0, quint8(quint8(info) << 6) | quint8(byte(0) & 0x3f)) ;
}

/*!
    \reimp
*/
bool QKnxSceneInfo::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) <= maximum().toUInt();
}

/*!
    Sets the scene number stored in the datapoint type to \a number and the
    scene information to \a info.
*/
bool QKnxSceneInfo::setValue(quint8 sceneNumber, Info info)
{
    return setSceneNumber(sceneNumber) && setInfo(info);
}

#include "moc_qknxscene.cpp"

QT_END_NAMESPACE
