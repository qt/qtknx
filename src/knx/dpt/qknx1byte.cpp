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

#include "qknx1byte.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx1Byte
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnx1Byte class is a fixed size datapoint type with the length of
    1 byte.

    \sa QKnxDatapointType
*/
// -- QKnx1Byte

/*!
    Creates a fixed size datapoint type with the value set to \c 0.
*/
QKnx1Byte::QKnx1Byte()
    : QKnx1Byte(0)
{}

/*!
    Creates a fixed size datapoint type with the value \a value.

    The range for the value is from \c 0 to \c 255.
*/
QKnx1Byte::QKnx1Byte(quint8 value)
    : QKnx1Byte(SubType, value)
{}

/*!
    Creates a fixed size datapoint with the subtype \a subType and the value
    \a value.

    The range for the value is from \c 0 to \c 255.
*/
QKnx1Byte::QKnx1Byte(int subType, quint8 value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("1-byte"));
    setRange(QVariant(0x00), QVariant(0xff));
    setRangeText(tr("Value: 0"), tr("Value: 255"));

    setValue(value);
}

/*!
    Returns the value stored in the datapoint type.
*/
quint8 QKnx1Byte::value() const
{
    return byte(0);
}

/*!
    Sets the value of the datapoint type to \a value.
*/
bool QKnx1Byte::setValue(quint8 value)
{
    return setByte(0, value);
}

/*!
    \reimp
*/
bool QKnx1Byte::isValid() const
{
    return QKnxDatapointType::isValid() && byte(0) >= minimum().toUInt()
        && byte(0) <= maximum().toUInt();
}

// -- QKnxScloMode

QKnxScloMode::QKnxScloMode()
    : QKnxScloMode(Mode::Autonomous)
{}

QKnxScloMode::QKnxScloMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("SCLO Mode"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Autonomous, 0"), tr("Master, 2"));
    setMode(mode);
}

bool QKnxScloMode::setMode(Mode mode)
{
    if (mode <= Mode::Master)
        return setByte(0, quint8(mode));
    return false;
}

QKnxScloMode::Mode QKnxScloMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxBuildingMode

QKnxBuildingMode::QKnxBuildingMode()
    : QKnxBuildingMode(Mode::BuildingInUse)
{}

QKnxBuildingMode::QKnxBuildingMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Building Mode"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Building in use, 0"), tr("Building protection, 2"));
    setMode(mode);
}

bool QKnxBuildingMode::setMode(Mode mode)
{
    if (mode <= Mode::BuildingProtection)
        return setByte(0, quint8(mode));
    return false;
}

QKnxBuildingMode::Mode QKnxBuildingMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxOccupyMode

QKnxOccupyMode::QKnxOccupyMode()
    : QKnxOccupyMode(Mode::Occupied)
{}

QKnxOccupyMode::QKnxOccupyMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Occupied"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Occupied, 0"), tr("Not occupied, 2"));
    setMode(mode);
}

bool QKnxOccupyMode::setMode(Mode mode)
{
    if (mode <= Mode::NotOccupied)
        return setByte(0, quint8(mode));
    return false;
}

QKnxOccupyMode::Mode QKnxOccupyMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxPriority

QKnxPriority::QKnxPriority()
    : QKnxPriority(Priority::High)
{}

QKnxPriority::QKnxPriority(Priority priority)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Priority"));
    setRange(QVariant(0x00), QVariant(0x03));
    setRangeText(tr("High, 0"), tr("void, 3"));
    setPriority(priority);
}

bool QKnxPriority::setPriority(Priority priority)
{
    if (priority <= Priority::Void)
        return setByte(0, quint8(priority));
    return false;
}

QKnxPriority::Priority QKnxPriority::priority() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Priority::Invalid : Priority(valueMode));
}


// -- QKnxLightApplicationMode

QKnxLightApplicationMode::QKnxLightApplicationMode()
    : QKnxLightApplicationMode(Mode::Normal)
{}

QKnxLightApplicationMode::QKnxLightApplicationMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Light application mode"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Normal, 0"), tr("Night round, 2"));
    setMode(mode);
}

bool QKnxLightApplicationMode::setMode(Mode mode)
{
    if (mode <= Mode::NightRound)
        return setByte(0, quint8(mode));
    return false;
}

QKnxLightApplicationMode::Mode QKnxLightApplicationMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxApplicationArea

QKnxApplicationArea::QKnxApplicationArea()
    : QKnxApplicationArea(Area::NoFault)
{}

QKnxApplicationArea::QKnxApplicationArea(Area area)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Application Area"));
    setRange(QVariant(0x00), QVariant(0x32));
    setRangeText(tr("no fault, 0"), tr("Shutters and blinds, 50"));
    setArea(area);
}

bool QKnxApplicationArea::setArea(Area area)
{
    switch (area) {
    case Area::NoFault:
    case Area::SystemFunctionCommonInterest:
    case Area::HvacGeneralFBs:
    case Area::HvacHotWaterHeating:
    case Area::HvacDirectElectricalHeating:
    case Area::HvacTerminalUnits:
    case Area::HvacVac:
    case Area::Lighting:
    case Area::Security:
    case Area::LoadManagement:
    case Area::ShuttersAndBlinds:
        return setByte(0, quint8(area));
    default:
        break;
    }
    return false;
}

QKnxApplicationArea::Area QKnxApplicationArea::area() const
{
    QKnxApplicationArea::Area valueArea = QKnxApplicationArea::Area(value());

    switch (valueArea) {
    case Area::NoFault:
    case Area::SystemFunctionCommonInterest:
    case Area::HvacGeneralFBs:
    case Area::HvacHotWaterHeating:
    case Area::HvacDirectElectricalHeating:
    case Area::HvacTerminalUnits:
    case Area::HvacVac:
    case Area::Lighting:
    case Area::Security:
    case Area::LoadManagement:
    case Area::ShuttersAndBlinds:
        return valueArea;
    default:
        break;
    }
    return Area::Invalid;
}

bool QKnxApplicationArea::isValid() const
{
    return QKnx1Byte::isValid() && area() != Area::Invalid;
}


// -- QKnxAlarmClassType

QKnxAlarmClassType::QKnxAlarmClassType()
    : QKnxAlarmClassType(Type::SimpleAlarm)
{}

QKnxAlarmClassType::QKnxAlarmClassType(Type type)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Alarm"));
    setRange(QVariant(0x01), QVariant(0x03));
    setRangeText(tr("Simple alarm, 1"), tr("Extended alarm, 3"));
    setType(type);
}

bool QKnxAlarmClassType::setType(Type type)
{
    if (type >= Type::SimpleAlarm && type <= Type::ExtendedAlarm)
        return setByte(0, quint8(type));
    return false;
}

QKnxAlarmClassType::Type QKnxAlarmClassType::type() const
{
    auto typeValue = QKnxAlarmClassType::Type(value());
    if (typeValue >= Type::SimpleAlarm && typeValue <= Type::ExtendedAlarm)
        return typeValue;
    return Type::Invalid;
}


// -- QKnxPsuMode

QKnxPsuMode::QKnxPsuMode()
    : QKnxPsuMode(Mode::Disabled)
{}

QKnxPsuMode::QKnxPsuMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("PSU Mode"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Disabled, 0"), tr("Automatic, 2"));
    setMode(mode);
}

bool QKnxPsuMode::setMode(Mode mode)
{
    if (mode <= Mode::Automatic)
        return setByte(0, quint8(mode));
    return false;

}

QKnxPsuMode::Mode QKnxPsuMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxErrorClassSystem

QKnxErrorClassSystem::QKnxErrorClassSystem()
    : QKnxErrorClassSystem(Error::NoFault)
{}

QKnxErrorClassSystem::QKnxErrorClassSystem(Error error)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("System error class"));
    setRange(QVariant(0x00), QVariant(0x12));
    setRangeText(tr("No fault, 0"), tr("Group object type exceeds, 18"));
    setError(error);
}

bool QKnxErrorClassSystem::setError(Error error)
{
    if (error <= Error::GroupObjectTypeExceeds)
        return setByte(0, quint8(error));
    return false;
}

QKnxErrorClassSystem::Error QKnxErrorClassSystem::error() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Error::Invalid : Error(valueMode));
}


// -- QKnxErrorClassHvac

QKnxErrorClassHvac::QKnxErrorClassHvac()
    : QKnxErrorClassHvac(Error::NoFault)
{}

QKnxErrorClassHvac::QKnxErrorClassHvac(Error error)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("HVAC error class"));
    setRange(QVariant(0x00), QVariant(0x04));
    setRangeText(tr("No fault, 0"), tr("Other fault, 4"));
    setError(error);
}

bool QKnxErrorClassHvac::setError(Error error)
{
    if (error <= Error::OtherFault)
        return setByte(0, quint8(error));
    return false;

}

QKnxErrorClassHvac::Error QKnxErrorClassHvac::error() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Error::Invalid : Error(valueMode));
}


// -- QKnxTimeDelay

QKnxTimeDelay::QKnxTimeDelay()
    : QKnxTimeDelay(Delay::NotActive)
{}

QKnxTimeDelay::QKnxTimeDelay(Delay delay)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Time delay"));
    setRange(QVariant(0x00), QVariant(0x19));
    setRangeText(tr("Not active, 0"), tr("Twenty four hours, 25"));
    setDelay(delay);
}

bool QKnxTimeDelay::setDelay(Delay delay)
{
    if (delay <= Delay::TwentyFourHour)
        return setByte(0, quint8(delay));
    return false;
}

QKnxTimeDelay::Delay QKnxTimeDelay::delay() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Delay::Invalid : Delay(valueMode));
}


// -- QKnxBeaufortWindForceScale

QKnxBeaufortWindForceScale::QKnxBeaufortWindForceScale()
    : QKnxBeaufortWindForceScale(Force::CalmNoWind)
{}

QKnxBeaufortWindForceScale::QKnxBeaufortWindForceScale(Force force)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Wind force scale (0..12)"));
    setRange(QVariant(0x00), QVariant(0x0c));
    setRangeText(tr("Calm (no wind), 0"), tr("Hurricane, 12"));
    setForce(force);
}

bool QKnxBeaufortWindForceScale::setForce(Force force)
{
    if (force <= Force::Hurricane)
        return setByte(0, quint8(force));
    return false;
}

QKnxBeaufortWindForceScale::Force QKnxBeaufortWindForceScale::force() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Force::Invalid : Force(valueMode));
}


// -- QKnxSensorSelect

QKnxSensorSelect::QKnxSensorSelect()
    : QKnxSensorSelect(Mode::Inactive)
{}

QKnxSensorSelect::QKnxSensorSelect(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Sensor mode"));
    setRange(QVariant(0x00), QVariant(0x04));
    setRangeText(tr("Inactive, 0"), tr("Temperature sensor input, 12"));
    setMode(mode);
}

bool QKnxSensorSelect::setMode(Mode mode)
{
    if (mode <= Mode::TemperatureSensorInput)
        return setByte(0, quint8(mode));
    return false;
}

QKnxSensorSelect::Mode QKnxSensorSelect::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxActuatorConnectType

QKnxActuatorConnectType::QKnxActuatorConnectType()
    : QKnxActuatorConnectType(Type::SensorConnection)
{}

QKnxActuatorConnectType::QKnxActuatorConnectType(Type type)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Actuator connect type"));
    setRange(QVariant(0x01), QVariant(0x02));
    setRangeText(tr("Sensor connection, 1"), tr("Controller connection, 2"));
    setType(type);
}

bool QKnxActuatorConnectType::setType(Type type)
{
    if (type == Type::SensorConnection || type == Type::ControllerConnection)
        return setByte(0, quint8(type));
    return false;
}

QKnxActuatorConnectType::Type QKnxActuatorConnectType::type() const
{
    auto typeValue = QKnxActuatorConnectType::Type(value());
    if (typeValue == Type::SensorConnection || typeValue == Type::ControllerConnection)
        return typeValue;
    return Type::Invalid;
}

#include "moc_qknx1byte.cpp"

QT_END_NAMESPACE
