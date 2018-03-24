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

#include "qknx1byte.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx1Byte
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnx1Byte class is a fixed size datapoint type with the length of
    1 byte.

    This datapoint type encodes an absolute value between \c 0 and \c 255.

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
*/
QKnx1Byte::QKnx1Byte(quint8 value)
    : QKnx1Byte(SubType, value)
{}

/*!
    Creates a fixed size datapoint with the subtype \a subType and the value
    \a value.
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
    Sets the byte stored in the datapoint type to \a value.

    Returns \c true if the byte was set; otherwise returns \c false.
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

/*!
    \class QKnxScloMode
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxScloMode class is a datapoint type for storing the system
    clock (SCLO) mode.

    One device in the system may be assigned to provide accurate system time
    and date information and synchronize the local clocks of other devices in
    the system. The system clock information is generated and distributed by
    the system clock that is configured as a \e {master clock}.

    Other clocks in the system are configured as \e {slave clocks}, which are
    also real clocks. They contain an internal clock to keep the time (using an
    oscillator, crystal, mains signal, and so on).

    The clock in the SCLO can also be run autonomously, which means that no
    system clock information is sent and reception of system clock information
    is disabled. In this case, the SCLO is configured as an
    \e {autonomous clock}.

    SCLO mode may be activated automatically or by configuration. Usually, the
    device containing a SCLO with the most accurate clock will be configured as
    master, whereas SCLOs in other devices are configured as slaves or
    autonomous clocks.

    The \l Mode enumeration holds the main functionality of SCLO as an
    autonomous clock, a slave clock, or a master clock. Depending on the mode,
    datapoints in the SCLO become mandatory or optional and alternative
    flowcharts are activated in the device.

    The SCLO mode must be set in a device, but it can be \e read-only. It can be
    set at the factory and is not changeable because a SCLO may have a fixed
    functionality. For example, a DCF77 Radio Clock will always be a master
    clock.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxScloMode::Mode

    This enum holds the main functionality of the SCLO stored in the datapoint
    type.

    \value Autonomous
           The system clock is configured as an autonomous clock.
    \value Slave
           The system clock is configured as a slave clock.
    \value Master
           The system clock is configured as a master clock.
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the SCLO mode set to \c Autonomous.
*/
QKnxScloMode::QKnxScloMode()
    : QKnxScloMode(Mode::Autonomous)
{}

/*!
    Creates a fixed size datapoint type with the SCLO mode set to \a mode.
*/
QKnxScloMode::QKnxScloMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("SCLO Mode"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Autonomous, 0"), tr("Master, 2"));
    setMode(mode);
}

/*!
    Sets the SCLO mode stored in the datapoint type to \a mode.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxScloMode::setMode(Mode mode)
{
    if (mode <= Mode::Master)
        return setByte(0, quint8(mode));
    return false;
}

/*!
    Returns the SCLO mode stored in the datapoint type.
*/
QKnxScloMode::Mode QKnxScloMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxBuildingMode

/*!
    \class QKnxBuildingMode
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxBuildingMode class is a datapoint type for storing the
    building mode.

    This datapoint type stores whether the building is in use and whether it is
    protected.

    The range for the \l Mode value is from \c {Building in use, 0} to
    \c {Building protection, 2}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxBuildingMode::Mode

    This enum holds the building mode stored in the datapoint type.

    \value BuildingInUse
           The building is in use.
    \value BuildingNotUsed
           The building is not in use.
    \value BuildingProtection
           The building is protected.
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the building mode set to
    \c BuildingInUse.
*/
QKnxBuildingMode::QKnxBuildingMode()
    : QKnxBuildingMode(Mode::BuildingInUse)
{}

/*!
    Creates a fixed size datapoint type with the building mode set to
    \a mode.
*/
QKnxBuildingMode::QKnxBuildingMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Building Mode"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Building in use, 0"), tr("Building protection, 2"));
    setMode(mode);
}

/*!
    Sets the building mode stored in the datapoint type to \a mode.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxBuildingMode::setMode(Mode mode)
{
    if (mode <= Mode::BuildingProtection)
        return setByte(0, quint8(mode));
    return false;
}

/*!
    Returns the building mode stored in the datapoint type.
*/
QKnxBuildingMode::Mode QKnxBuildingMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxOccupyMode

/*!
    \class QKnxOccupyMode
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxOccupyMode class is a datapoint type for storing the occupy
    mode.

    This datapoint type stores information about room occupancy.

    The range for the value is from \c {Occupied, 0} to \c {Not occupied, 2}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxOccupyMode::Mode

    This enum holds the mode stored in the datapoint type.

    \value Occupied
           The room is occupied.
    \value Standby
           Stand-by
    \value NotOccupied
           The room is not occupied.
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the mode set to \c Occupied.
*/
QKnxOccupyMode::QKnxOccupyMode()
    : QKnxOccupyMode(Mode::Occupied)
{}

/*!
    Creates a fixed size datapoint type with the mode set to \a mode.
*/
QKnxOccupyMode::QKnxOccupyMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Occupied"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Occupied, 0"), tr("Not occupied, 2"));
    setMode(mode);
}

/*!
    Sets the mode stored in the datapoint type to \a mode.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxOccupyMode::setMode(Mode mode)
{
    if (mode <= Mode::NotOccupied)
        return setByte(0, quint8(mode));
    return false;
}

/*!
    Returns the mode stored in the datapoint type.
*/
QKnxOccupyMode::Mode QKnxOccupyMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxPriority

/*!
    \class QKnxPriority
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxPriority class is a datapoint type for storing priority.

    This datapoint type is used for parameters, not for runtime interworking.
    For example, it can be used to store the alarm priority of a configurable
    digital alarm input in a device.

    The range for the value is from \c {High, 0} to \c {Void, 3}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxPriority::Priority

    This enum holds the priority stored in the datapoint type.

    \value High
           High
    \value Medium
           Medium
    \value Low
           Low
    \value Void
           Void
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the priority set to \c High.
*/
QKnxPriority::QKnxPriority()
    : QKnxPriority(Priority::High)
{}

/*!
    Creates a fixed size datapoint type with the priority set to \a priority.
*/
QKnxPriority::QKnxPriority(Priority priority)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Priority"));
    setRange(QVariant(0x00), QVariant(0x03));
    setRangeText(tr("High, 0"), tr("void, 3"));
    setPriority(priority);
}

/*!
    Sets the priority stored in the datapoint type to \a priority.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxPriority::setPriority(Priority priority)
{
    if (priority <= Priority::Void)
        return setByte(0, quint8(priority));
    return false;
}

/*!
    Returns the priority stored in the datapoint type.
*/
QKnxPriority::Priority QKnxPriority::priority() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Priority::Invalid : Priority(valueMode));
}


// -- QKnxLightApplicationMode

/*!
    \class QKnxLightApplicationMode
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxLightApplicationMode class is a datapoint type for storing
    the light application mode.

    The range for the \l Mode value is from \c {Normal, 0} to
    \c {Night round, 2}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxLightApplicationMode::Mode

    This enum holds the light application mode stored in the datapoint type.

    \value Normal
           Normal
    \value PresenceSimulation
           Presence simulation
    \value NightRound
           Night round
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the mode set to \c Normal.
*/
QKnxLightApplicationMode::QKnxLightApplicationMode()
    : QKnxLightApplicationMode(Mode::Normal)
{}

/*!
    Creates a fixed size datapoint type with the mode set to \a mode.
*/
QKnxLightApplicationMode::QKnxLightApplicationMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Light application mode"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Normal, 0"), tr("Night round, 2"));
    setMode(mode);
}

/*!
    Sets the mode stored in the datapoint type to \a mode.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxLightApplicationMode::setMode(Mode mode)
{
    if (mode <= Mode::NightRound)
        return setByte(0, quint8(mode));
    return false;
}

/*!
    Returns the mode stored in the datapoint type.
*/
QKnxLightApplicationMode::Mode QKnxLightApplicationMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxApplicationArea

/*!
    \class QKnxApplicationArea
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxApplicationArea class is a datapoint type for storing the
    application area.

    The range for the \l Area value is from \c {No fault, 0} to
    \c {Shutters and blinds, 50}.

    Faults in functions of common interest are mapped to the application area
    \l SystemFunctionCommonInterest. For example, a multiple system clock master
    conflict is a QKnxErrorClassSystem::ConfigurationFault within the
    \c SystemFunctionCommonInterest application area.

    Faults in heating, ventilation, and air conditioning (HVAC) are mapped to
    the application areas \l HvacGeneralFBs, \l HvacHotWaterHeating,
    \l HvacDirectElectricalHeating, \l HvacTerminalUnits, and \l HvacVac.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte, QKnxErrorClassSystem::Error,
        QKnxErrorClassHvac, QKnxScloMode::Mode
*/

/*!
    \enum QKnxApplicationArea::Area

    This enum holds the application area stored in the datapoint type.
    This coding corresponds to the numbering of parts in the KNX System
    Specification.

    \value NoFault
           No fault signals were transmitted in any application areas.
    \value SystemFunctionCommonInterest
           The system and functions of common interest
    \value HvacGeneralFBs
           HVAC general functional blocks
    \value HvacHotWaterHeating
           HVAC hot water heating
    \value HvacDirectElectricalHeating
           HVAC direct electrical heating
    \value HvacTerminalUnits
           HVAC terminal units
    \value HvacVac
           HVAC VAC
    \value Lighting
           Lighting
    \value Security
           Security
    \value LoadManagement
           Load management
    \value ShuttersAndBlinds
           Shutters and blinds
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the application area set to
    \c NoFault.
*/
QKnxApplicationArea::QKnxApplicationArea()
    : QKnxApplicationArea(Area::NoFault)
{}

/*!
    Creates a fixed size datapoint type with the the application area set to
    \a area.
*/
QKnxApplicationArea::QKnxApplicationArea(Area area)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Application Area"));
    setRange(QVariant(0x00), QVariant(0x32));
    setRangeText(tr("no fault, 0"), tr("Shutters and blinds, 50"));
    setArea(area);
}

/*!
    Sets the application area stored in the datapoint type to \a area.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
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

/*!
    Returns the application area stored in the datapoint type.
*/
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

/*!
    \reimp
*/
bool QKnxApplicationArea::isValid() const
{
    return QKnx1Byte::isValid() && area() != Area::Invalid;
}


// -- QKnxAlarmClassType

/*!
    \class QKnxAlarmClassType
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxAlarmClassType class is a datapoint type for storing the
    alarm class type.

    The alarm \l Type can be simple, basic, or extended.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxAlarmClassType::Type

    This enum holds the alarm class type stored in the datapoint type.

    \value SimpleAlarm
           Simple alarm
    \value BasicAlarm
           Basic alarm
    \value ExtendedAlarm
           Extended alarm
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the alarm class type set to
    \c SimpleAlarm.
*/
QKnxAlarmClassType::QKnxAlarmClassType()
    : QKnxAlarmClassType(Type::SimpleAlarm)
{}

/*!
    Creates a fixed size datapoint type with the alarm class type set to
    \a type.
*/
QKnxAlarmClassType::QKnxAlarmClassType(Type type)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Alarm"));
    setRange(QVariant(0x01), QVariant(0x03));
    setRangeText(tr("Simple alarm, 1"), tr("Extended alarm, 3"));
    setType(type);
}

/*!
    Sets the alarm class type stored in the datapoint type to \a type.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxAlarmClassType::setType(Type type)
{
    if (type >= Type::SimpleAlarm && type <= Type::ExtendedAlarm)
        return setByte(0, quint8(type));
    return false;
}

/*!
    Returns the alarm class type stored in the datapoint type.
*/
QKnxAlarmClassType::Type QKnxAlarmClassType::type() const
{
    auto typeValue = QKnxAlarmClassType::Type(value());
    if (typeValue >= Type::SimpleAlarm && typeValue <= Type::ExtendedAlarm)
        return typeValue;
    return Type::Invalid;
}


// -- QKnxPsuMode

/*!
    \class QKnxPsuMode
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxPsuMode class is a datapoint type for storing the bus power
    supply unit mode.

    A bus power supply unit is required for bus communications. It can be either
    a decentral (distributed) bus power supply unit, DPSU, or a central bus
    power supply unit, PSU.

    The DPSU or PSU can be activated or deactivated also by mechanical means,
    such as using a jumper or a switch. The supported range is product specific.
    For example, disabled/enabled only or disabled/auto only.

    The range for the \l Mode value is from \c {Disabled, 0} to
    \c {Automatic, 2}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxPsuMode::Mode

    This enum holds the bus power supply unit mode stored in the datapoint type.

    \value Disabled
           The PSU or DPSU in the device is switched off.
    \value Enabled
           The PSU or DPSU in the device is switched on.
    \value Automatic
           The PSU or DPSU in the device was turned on or off automatically.
           For example, by sending a message on the bus.
    \value Invalid
           The mode is invalid.
*/

/*!
    Creates a fixed size datapoint type with the mode set to \c Disabled.
*/
QKnxPsuMode::QKnxPsuMode()
    : QKnxPsuMode(Mode::Disabled)
{}

/*!
    Creates a fixed size datapoint type with the mode set to \a mode.
*/
QKnxPsuMode::QKnxPsuMode(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("PSU Mode"));
    setRange(QVariant(0x00), QVariant(0x02));
    setRangeText(tr("Disabled, 0"), tr("Automatic, 2"));
    setMode(mode);
}

/*!
    Sets the mode stored in the datapoint type to \a mode.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxPsuMode::setMode(Mode mode)
{
    if (mode <= Mode::Automatic)
        return setByte(0, quint8(mode));
    return false;

}

/*!
    Returns the mode stored in the datapoint type.
*/
QKnxPsuMode::Mode QKnxPsuMode::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxErrorClassSystem

/*!
    \class QKnxErrorClassSystem
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxErrorClassSystem class is a datapoint type for storing system
    errors.

    This datapoint type stores fault signals transmitted in the application area
    \l QKnxApplicationArea::SystemFunctionCommonInterest.

    The range for the \l Error value is from \c {No fault, 0} to
    \c {Group object type exceeds, 18}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte, QKnxApplicationArea
*/

/*!
    \enum QKnxErrorClassSystem::Error

    This enum holds the system error stored in the datapoint type.

    \value NoFault
           No fault signals were transmitted.
    \value GeneralDeviceFault
           An error occurred in the RAM, EEPROM, UI, watchdog, and so on.
    \value CommunicationFault
           A communication error occurred.
           For example, a failure in a formerly present communication partner
           or a timeout in the system clock signal (heartbeat).
    \value ConfigurationFault
           A configuration error was detected.
           For example, two devices with the same individual address were
           detected or multiple system clocks were set as the master clock.
    \value HardwareFault
           A hardware error occurred.
    \value SoftwareFault
           A software error occurred.
    \value InsufficientNonVolatileMemory
           Not enough non-volatile memory
    \value InsufficientVolatileMemory
           Not enough volatile memory
    \value MemoryAllocationCommandWithSize0Received
           A memory allocation command with the size 0 was received.
    \value CrcError
           A CRC error occurred.
    \value WatchdogResetDetected
           A watchdog reset was detected.
    \value InvalidOpCodeDetected
           An invalid OP code was detected.
    \value GeneralProtectionFault
           A general protection error was detected.
    \value MaximalTableLengthExceeded
           The maximum table length was exceeded.
    \value UndefinedLoadCommandReceived
           An undefined load command was received.
    \value GroupAddressTableIsNotSorted
           The group address table is not sorted.
    \value InvalidConnectionNumberTsap
           The connection number TSAP is invalid.
    \value InvalidGroupObjectNumberAsap
           The connection number ASAP is invalid.
    \value GroupObjectTypeExceeds
           The group object type exceeds \c {PID_MAX_APDU_LENGTH H - 2}.
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the system error set to \c NoFault.
*/
QKnxErrorClassSystem::QKnxErrorClassSystem()
    : QKnxErrorClassSystem(Error::NoFault)
{}

/*!
    Creates a fixed size datapoint type with the system error set to \a error.
*/
QKnxErrorClassSystem::QKnxErrorClassSystem(Error error)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("System error class"));
    setRange(QVariant(0x00), QVariant(0x12));
    setRangeText(tr("No fault, 0"), tr("Group object type exceeds, 18"));
    setError(error);
}

/*!
    Sets the system error stored in the datapoint type to \a error.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxErrorClassSystem::setError(Error error)
{
    if (error <= Error::GroupObjectTypeExceeds)
        return setByte(0, quint8(error));
    return false;
}

/*!
    Returns the system error stored in the datapoint type.
*/
QKnxErrorClassSystem::Error QKnxErrorClassSystem::error() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Error::Invalid : Error(valueMode));
}


// -- QKnxErrorClassHvac

/*!
    \class QKnxErrorClassHvac
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxErrorClassHvac class is a datapoint type for storing errors
    in heating, ventilation, and air conditioning (HVAC).

    The range for the \l Error value is from \c {No fault, 0} to
    \c {Other fault, 4}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte, QKnxApplicationArea
*/

/*!
    \enum QKnxErrorClassHvac::Error

    This enum holds the HVAC fault signals stored in the datapoint type.

    \value NoFault
           No fault signals were transmitted.
    \value SensorFault
           An error occurred in the RAM, EEPROM, UI, watchdog, and so on.
    \value ProcessControllerFault
           An error occurred in the process or controller.
    \value ActuatorFault
           An error occurred in the actuator.
    \value OtherFault
           An undefined error occurred.
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the error set to \c NoFault.
*/
QKnxErrorClassHvac::QKnxErrorClassHvac()
    : QKnxErrorClassHvac(Error::NoFault)
{}

/*!
    Creates a fixed size datapoint type with the error set to \a error.
*/
QKnxErrorClassHvac::QKnxErrorClassHvac(Error error)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("HVAC error class"));
    setRange(QVariant(0x00), QVariant(0x04));
    setRangeText(tr("No fault, 0"), tr("Other fault, 4"));
    setError(error);
}

/*!
    Sets the error stored in the datapoint type to \a error.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxErrorClassHvac::setError(Error error)
{
    if (error <= Error::OtherFault)
        return setByte(0, quint8(error));
    return false;

}

/*!
    Returns the error stored in the datapoint type.
*/
QKnxErrorClassHvac::Error QKnxErrorClassHvac::error() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Error::Invalid : Error(valueMode));
}


// -- QKnxTimeDelay

/*!
    \class QKnxTimeDelay
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxTimeDelay class is a datapoint type for storing a time delay.

    The range for the \l Delay value is from \c {Not active, 0} to
    \c {Twenty four hours, 2}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxTimeDelay::Delay

    This enum holds the time delay stored in the datapoint type.

    \value NotActive
           No time delay was set.
    \value OneSecond
    \value TwoSecond
    \value ThreeSecond
    \value FiveSecond
    \value TenSecond
    \value FifteenSecond
    \value TwentySecond
    \value ThirtySecond
    \value FortyFiveSecond
    \value OneMinute
    \value OneMinuteOneQuarter
    \value OneMinuteAndHaft
    \value TwoMinute
    \value TwoMinuteAndHalf
    \value ThreeMinute
    \value FiveMinute
    \value FifteenMinute
    \value TwentyMinute
    \value ThirtyMinute
    \value OneHour
    \value TwoHour
    \value ThreeHour
    \value FiveHour
    \value TwelveHour
    \value TwentyFourHour
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the time delay set to \c NotActive.
*/
QKnxTimeDelay::QKnxTimeDelay()
    : QKnxTimeDelay(Delay::NotActive)
{}

/*!
    Creates a fixed size datapoint type with the time delay set to \a delay.
*/
QKnxTimeDelay::QKnxTimeDelay(Delay delay)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Time delay"));
    setRange(QVariant(0x00), QVariant(0x19));
    setRangeText(tr("Not active, 0"), tr("Twenty four hours, 25"));
    setDelay(delay);
}

/*!
    Sets the time delay stored in the datapoint type to \a delay.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxTimeDelay::setDelay(Delay delay)
{
    if (delay <= Delay::TwentyFourHour)
        return setByte(0, quint8(delay));
    return false;
}

/*!
    Returns the time delay stored in the datapoint type.
*/
QKnxTimeDelay::Delay QKnxTimeDelay::delay() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Delay::Invalid : Delay(valueMode));
}


// -- QKnxBeaufortWindForceScale

/*!
    \class QKnxBeaufortWindForceScale
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxBeaufortWindForceScale class is a datapoint type for storing
    the Beaufort wind force scale.

    The range for the \l Force value is from \c {Calm (no wind), 0} to
    \c {Hurricane, 12}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxBeaufortWindForceScale::Force

    This enum holds the Beaufort wind force scale stored in the datapoint type.

    \value CalmNoWind
    \value LightAir
    \value LightBreeze
    \value GentleBreeze
    \value ModerateBreeze
    \value FreshBreeze
    \value StrongBreeze
    \value NearModerateGale
    \value FreshGale
    \value StrongGale
    \value WholeGaleStorm
    \value ViolentStorm
    \value Hurricane
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the Beaufort wind force scale set
    to \c CalmNoWind.
*/
QKnxBeaufortWindForceScale::QKnxBeaufortWindForceScale()
    : QKnxBeaufortWindForceScale(Force::CalmNoWind)
{}

/*!
    Creates a fixed size datapoint type with the Beaufort wind force scale set
    to \a force.
*/
QKnxBeaufortWindForceScale::QKnxBeaufortWindForceScale(Force force)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Wind force scale (0..12)"));
    setRange(QVariant(0x00), QVariant(0x0c));
    setRangeText(tr("Calm (no wind), 0"), tr("Hurricane, 12"));
    setForce(force);
}

/*!
    Sets the Beaufort wind force scale stored in the datapoint type to \a force.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxBeaufortWindForceScale::setForce(Force force)
{
    if (force <= Force::Hurricane)
        return setByte(0, quint8(force));
    return false;
}

/*!
    Returns the Beaufort wind force scale stored in the datapoint type.
*/
QKnxBeaufortWindForceScale::Force QKnxBeaufortWindForceScale::force() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Force::Invalid : Force(valueMode));
}


// -- QKnxSensorSelect

/*!
    \class QKnxSensorSelect
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxSensorSelect class is a datapoint type for storing the sensor
    mode.

    The range for the \l Mode value is from \c {Inactive, 0} to
    \c {Temperature sensor input, 12}.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxSensorSelect::Mode

    This enum holds the sensor mode stored in the datapoint type.

    \value Inactive
           The sensor is inactive.
    \value DigitalInputNotInverted
           The sensor provides digital input in not inverted form.
    \value DigitalInputInverted
           The sensor provides inverted digital input.
    \value AnalogInput
           The sensor provides analog input.
    \value TemperatureSensorInput
           The sensor provides temperature input.
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the mode set to \c Inactive.
*/
QKnxSensorSelect::QKnxSensorSelect()
    : QKnxSensorSelect(Mode::Inactive)
{}

/*!
    Creates a fixed size datapoint type with the mode set to \a mode.
*/
QKnxSensorSelect::QKnxSensorSelect(Mode mode)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Sensor mode"));
    setRange(QVariant(0x00), QVariant(0x04));
    setRangeText(tr("Inactive, 0"), tr("Temperature sensor input, 12"));
    setMode(mode);
}

/*!
    Sets the mode stored in the datapoint type to \a mode.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxSensorSelect::setMode(Mode mode)
{
    if (mode <= Mode::TemperatureSensorInput)
        return setByte(0, quint8(mode));
    return false;
}

/*!
    Returns the mode stored in the datapoint type.
*/
QKnxSensorSelect::Mode QKnxSensorSelect::mode() const
{
    quint8 valueMode = value();
    return (valueMode > maximum().toUInt() ? Mode::Invalid : Mode(valueMode));
}


// -- QKnxActuatorConnectType

/*!
    \class QKnxActuatorConnectType
    \inherits QKnx1Byte
    \inmodule QtKnx

    \brief The QKnxActuatorConnectType class is a datapoint type for storing the
    actuator connection type.

    An actuator can be connected to a sensor or a controller.

    This is a fixed size datapoint type with the length of 1 byte.

    \sa QKnxDatapointType, QKnx1Byte
*/

/*!
    \enum QKnxActuatorConnectType::Type

    This enum holds the actuator connection type stored in the datapoint type.

    \value SensorConnection
           The actuator is connected to a sensor.
    \value ControllerConnection
           The actuator is connected to a controller.
    \value Invalid
           The value is invalid.
*/

/*!
    Creates a fixed size datapoint type with the actuator connection type set to
    \c SensorConnection.
*/
QKnxActuatorConnectType::QKnxActuatorConnectType()
    : QKnxActuatorConnectType(Type::SensorConnection)
{}

/*!
    Creates a fixed size datapoint type with the actuator connection type set to
    \a type.
*/
QKnxActuatorConnectType::QKnxActuatorConnectType(Type type)
    : QKnx1Byte(SubType, 0)
{
    setDescription(tr("Actuator connect type"));
    setRange(QVariant(0x01), QVariant(0x02));
    setRangeText(tr("Sensor connection, 1"), tr("Controller connection, 2"));
    setType(type);
}

/*!
    Sets the actuator connection type stored in the datapoint type to \a type.

    Returns \c true if the byte was set; otherwise returns \c false.
*/
bool QKnxActuatorConnectType::setType(Type type)
{
    if (type == Type::SensorConnection || type == Type::ControllerConnection)
        return setByte(0, quint8(type));
    return false;
}

/*!
    Returns the actuator connection type stored in the datapoint type.
*/
QKnxActuatorConnectType::Type QKnxActuatorConnectType::type() const
{
    auto typeValue = QKnxActuatorConnectType::Type(value());
    if (typeValue == Type::SensorConnection || typeValue == Type::ControllerConnection)
        return typeValue;
    return Type::Invalid;
}

#include "moc_qknx1byte.cpp"

QT_END_NAMESPACE
