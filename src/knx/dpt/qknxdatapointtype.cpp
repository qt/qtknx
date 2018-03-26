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

#include "qknxdatapointtype.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxDatapointType

    \inmodule QtKnx
    \brief The QKnxDatapointType class is a base class for datapoint types
    with specific properties.

    A datapoint type represents the data corresponding to a KNX device
    functionality. It describes the data, setting the rules about the format
    and values to be allowed, and it holds the bytes containing the data.

    This data is used in the data part of the \l QKnxTpdu class. To retrieve
    the data of a datapoint type, call bytes().

    There are two types of a QKnxDatapointType: \l QKnxFixedSizeDatapointType
    for fixed length datapoint types and \l QKnxVariableSizeDatapointType for
    variable length datapoint types.

    A datapoint type is identified by its main number and a sub number. Each
    main number corresponds to a datapoint type with a particular structure.
    The sub numbers correspond to different interpretations or naming of this
    structure.

    The Qt KNX module provides a class for each datapoint type with the main
    number less than 30 and sub number less than 100. All datapoint types with
    the same main number inherit from a datapoint type class representing the
    main number datapoint type characteristics.

    \section1 Adding Datapoint Types

    To implement a datapoint type described in the KNX documentation but not
    implemented in the Qt KNX module, one can create a class inheriting from
    \l QKnxFixedSizeDatapointType or \l QKnxVariableSizeDatapointType and
    register this new class using an instance of \l QKnxDatapointTypeFactory:

    \code
        #include mynewdatapointtype.h

        int main()
        {
            int mainType = 65535;
            int subType = 100;
            int typeSize = 1; // size in bytes

            auto &factory = QKnxDatapointTypeFactory::instance() ;
            factory.registerType<MyNewDataPointType>(mainType, subType, typeSize);
        }
    \endcode
*/

/*!
    \enum QKnxDatapointType::Type

    This enum type holds the type of the datapoint type.

    \value Unknown
            An unknown datapoint type.
    \value Dpt1_1Bit
            A fixed size datapoint type with the length of 1 bit. This is a
            boolean datapoint type, such as: switch on or off, move up or down,
            open and close, stop and start.
    \value DptSwitch
           Stores the switch state: \c Off or \c On.
    \value DptBool
           Stores a boolean value: \c False or \c True.
    \value DptEnable
           Stores an action: \c Disable or \c Enable.
    \value DptRamp
           Stores ramp information: \c NoRamp or \c Ramp.
    \value DptAlarm
           Stores alarm information: \c Alarm or \c NoAlarm.
    \value DptBinaryValue
           Stores a binary value: \c Low or \c High.
    \value DptStep
           Stores stepping information: \c Decrease or \c Increase.
    \value DptUpDown
           Stores direction: \c Up or \c Down.
    \value DptOpenClose
           Stores action: \c Open or \c Close.
    \value DptStart
           Stores action: \c Stop or \c Start.
    \value DptState
           Stores state: \c Inactive or \c Active.
    \value DptInvert
           Stores state: \c {Not inverted} or \c Inverted.
    \value DptDimSendStyle
           Stores dimming send style. Possible values are \c Start/Stop or
           \c Cyclically.
    \value DptInputSource
           Stores input source: \c Fixed or \c Calculated.
    \value DptReset
           Stores reset action: \c {No action (dummy)} or
           \c {Reset command (trigger)}.
    \value DptAck
           Stores the acknowledge command (trigger) for alarms, for example.
           Possible values are \c Acknowledge or \c {No action (dummy)}
    \value DptTrigger
           Stores trigger action: \c Trigger or \c {Trigger (also)}.
    \value DptOccupancy
           Stores occupancy state: \c {Not occupied} or \c Occupied.
    \value DptWindowDoor
           Stores window or door state: \c Closed or \c Open.
    \value DptLogicalFunction
           Stores the logical function \c OR or \c AND.
    \value DptSceneAB
           Stores the scene: \c {Scene A} or \c {Scene B}.
    \value DptShutterBlindsMode
           Stores the shutter or blinds mode:
           \c {Only move Up/Down mode (shutter)} or
           \c {Move up down + StepStop mode (blind)}.
    \value DptHeatCool
           Stores the heating mode: \c Cooling or \c Heating.
    \value Dpt2_1BitControlled
           A fixed size datapoint type with the length of 2 bits. This is a
           a boolean datapoint type with a control part.
    \value DptSwitchControl
           Stores a switch with a control part.
    \value DptBoolControl
           Stores a boolean value with a control part.
    \value DptEnableControl
           Stores the enabled state with a control part.
    \value DptRampControl
           Stores the ramp state with a control part.
    \value DptAlarmControl
           Stores alarm information with a control part.
    \value DptBinaryValueControl
           Stores a binary value with a control part.
    \value DptStepControl
           Stores stepping information with a control part.
    \value DptDirection1Control
           Stores \c DptUpDown with a control part.
    \value DptDirection2Control
           Stores \c DptOpenClose with a control part.
    \value DptStartControl
           Stores a start action with a control part.
    \value DptStateControl
           Stores state with a control part.
    \value DptInvertControl
           Stores inverted state with a control part.
    \value Dpt3_3BitControlled
           A fixed size datapoint type with the length of 3 bits.
    \value DptControlDimming
           Increases or decreases the brightness of lights.
    \value DptControlBlinds
           Moves blinds up or down.
    \value Dpt4_Character
           A fixed size datapoint type with the length of 1 byte that encodes
           a character.
    \value DptCharAscii
           Encodes an ASCII character.
    \value DptChar88591
           Encodes an ISO 8859-1 character.
    \value Dpt5_8bitUnsigned
           A fixed size datapoint type with an 8-bit unsigned value.
    \value DptScaling
           Stores percentage from \c 0 to \c 100.
    \value DptAngle
           Stores the angle in degrees from \c 0 to \c 360.
    \value DptPercentU8
           Stores percentage from \c 0 to \c 255.
    \value DptDecimalFactor
           Stores the ratio from \c 0 to \c 255.
    \value DptTariff
           Stores the tariff from \c 0 to \c 254.
    \value DptValue1Ucount
           Stores counter pulses from \c 0 to \c 255.
    \value Dpt6_8bitSigned
           A fixed size datapoint type with an 8-bit signed value.
    \value DptPercentV8
           Stores percentage from \c -128% to \c 127%.
    \value DptValue1Count
           Stores counter pulses from \c -128 to \c 127.
    \value DptStatusMode3
           Stores status mode 3.
    \value Dpt7_2ByteUnsigned
           A fixed size datapoint type with a 2-byte unsigned value.
    \value DptValue2UCount
           Stores pulses.
    \value DptTimePeriodMsec
           Stores the time in milliseconds.
    \value DptTimePeriod10Msec
           Stores the time in multiples of 10 milliseconds.
    \value DptTimePeriod100Msec
           Stores the time in multiples of 100 milliseconds.
    \value DptTimePeriodSec
           Stores the time in seconds.
    \value DptTimePeriodMin
           Stores the time in minutes.
    \value DptTimePeriodHrs
           Stores the time in hours.
    \value DptPropertyDataType
           Stores the Identifier Interface Object Property data type (no unit).
    \value DptLengthMilliMeter
           Stores the length in millimeters.
    \value DptUEICurrentMilliA
           Stores the current in milliampere.
    \value DptBrightness
           Stores the brightness in LUX.
    \value Dpt8_2ByteSigned
           A fixed size datapoint type with a 2-byte signed value.
    \value DptValue2Count
           Stores pulse difference.
    \value DptDeltaTimeMsec
           Stores time lag in milliseconds.
    \value DptDeltaTime10Msec
           Stores time lag in multiples of 10 milliseconds.
    \value DptDeltaTime100Msec
           Stores time lag in multiples of 100 milliseconds.
    \value DptDeltaTimeSec
           Stores time lag in seconds.
    \value DptDeltaTimeMin
           Stores time lag in minutes.
    \value DptDeltaTimeHrs
           Stores time lag in hours.
    \value DptPercentV16
           Stores percentage difference.
    \value DptRotationAngle
           Stores rotation angle in degrees.
    \value Dpt9_2ByteFloat
           A fixed size datapoint type with a 2-byte float value.
    \value DptTemperatureCelsius
           Stores the temperature in degrees Celsius.
    \value DptTemperatureKelvin
           Stores the temperature in degrees Kelvin.
    \value DptTemperatureChange
           Stores the change in temperature in (K) per hour.
    \value DptValueLux
           Stores brightness in LUX.
    \value DptWindSpeed
           Stores the wind speed in meters per second (m/s).
    \value DptPressure
           Stores the pressure in Pascal (Pa).
    \value DptHumidity
           Stores the humidity as a percentage.
    \value DptAirQuality
           Stores the air quality in ppm.
    \value DptAirFlow
           Stores the air flow in m3/h.
    \value DptTimeSecond
           Stores the time in seconds (s).
    \value DptTimeMilliSecond
           Stores the time in milliseconds (ms).
    \value DptVoltage
           Stores the voltage in milli-volts (mV).
    \value DptCurrent
           Stores the current in milliampere (mA).
    \value DptPowerDensity
           Stores the power density in watts per square meter (W/m2).
    \value DptKelvinPerPercent
           Stores the Kelvin per percent (K/Percent).
    \value DptPower
           Stores the power in kilowatts (kW).
    \value DptVolumeFlow
           Stores the volume flow in liters per hour (l/h).
    \value DptAmountRain
           Stores the amount of rain in liters per square meter (l/m2).
    \value DptTemperatureFahrenheit
           Stores the temperature in degrees Fahrenheit.
    \value DptWindSpeedKmPerHour
           Stores the wind speed in kilometers per hour (km/h).
    \value Dpt10_TimeOfDay
           A fixed size datapoint type with the length of 3 bytes.
    \value DptTimeOfDay
           Stores the time of day.
    \value Dpt11_Date
           A fixed size datapoint type with the length of 3 bytes.
    \value DptDate
           Stores a date.
    \value Dpt12_4ByteUnsigned
           A fixed size datapoint type with a 4-byte unsigned value.
    \value DptValue4Ucount
           Stores a a 4-byte unsigned value.
    \value Dpt13_4ByteSigned
           A fixed size datapoint type with a 4-byte signed value.
    \value DptValue4Count
           Stores a a 4-byte signed value.
    \value DptFlowRateCubicMeterPerHour
           Stores the flow rate in cubic meters per hour (m3/h).
    \value DptApparentEnergy
           Stores the active energy in VAh.
    \value DptReactiveEnergy
           Stores the active energy in VARh.
    \value DptActiveEnergykWh
           Stores the active energy in kWh.
    \value Dpt_ApparentEnergykVAh
           Stores the active energy in kVAh.
    \value DptReactiveEnergykVARh
           Stores the active energy in KVARh.
    \value DptActiveEnergy
           Stores the active energy in Wh.
    \value DptLongDeltaTimeSec
           Stores the active energy in seconds.
    \value Dpt14_4ByteFloat
           A fixed size datapoint type with a 4-byte float value.
    \value DptValueAccelerationAngular
           Stores angular acceleration in rad/s2.
    \value DptValueActivationEnergy
           Stores activation energy in J/mol.
    \value DptValueActivity
           Stores radioactive activity in 1/s.
    \value DptValueMol
           Stores the amount of substance in mol.
    \value DptValueAmplitude
           Stores the amplitude.
    \value DptValueAngleRad
           Stores the angle in radian.
    \value DptValueAngleDeg
           Stores the angle in degrees.
    \value DptValueAngularMomentum
           Stores the angular momentum in Js.
    \value DptValueAngularVelocity
           Stores the angular velocity in rad/s.
    \value DptValueArea
           Stores the area in m2.
    \value DptValueCapacitance
           Stores the capacitance in F.
    \value DptValueChargeDensitySurface
           Stores the flux density in C/m2.
    \value DptValueChargeDensityVolume
           Stores the charge density in C/m3.
    \value DptValueCompressibility
           Stores the compressibility in m2/N.
    \value DptValueConductance
           Stores the conductance in S.
    \value DptValueElectricalConductivity
           Stores the conductivity in S/m.
    \value DptValueDensity
           Stores the density in kg/m3.
    \value DptValueElectricCharge
           Stores the electric charge in C.
    \value DptValueElectricCurrent
           Stores the electric charge in A.
    \value DptValueElectricCurrentDensity
           Stores the electric current density in A/m2.
    \value DptValueElectricDipoleMoment
           Stores the electric dipole moment in Cm.
    \value DptValueElectricDisplacement
           Stores the electric displacement in C/m2.
    \value DptValueElectricFieldStrength
           Stores the electric field strength in V/m.
    \value DptValueElectricFlux
           Stores the electric flux in C.
    \value DptValueElectricFluxDensity
           Stores the electric flux density in C/m2.
    \value DptValueElectricPolarization
           Stores the electric polarization in C/m2.
    \value DptValueElectricPotential
           Stores the electric potential in V.
    \value DptValueElectricPotentialDifference
           Stores the electric potential difference in V.
    \value DptValueElectromagneticMoment
           Stores the electromagnetic moment in Am2.
    \value DptValueElectromotiveForce
           Stores the electromotive force in V.
    \value DptValueEnergy
           Stores energy in J.
    \value DptValueForce
           Stores force in N.
    \value DptValueFrequency
           Stores frequency in Hz.
    \value DptValueAngularFrequency
           Stores angular frequency in rad/s.
    \value DptValueHeatCapacity
           Stores heat capacity in J/K.
    \value DptValueHeatFlowRate
           Stores heat flow rate in W.
    \value DptValueHeatQuantity
           Stores heat quantity in J.
    \value DptValueImpedance
           Stores impedance in Ohm.
    \value DptValueLength
           Stores length in m.
    \value DptValueLightQuantity
           Stores light quantity in J.
    \value DptValueLuminance
           Stores luminance in cd/m2.
    \value DptValueLuminousFlux
           Stores luminous flux in lm.
    \value DptValueLuminousIntensity
           Stores luminous intensity in cd.
    \value DptValueMagneticFieldStrength
           Stores magnetic field strength in A/m.
    \value DptValueMagneticFlux
           Stores magnetic flux in Wb.
    \value DptValueMagneticFluxDensity
           Stores magnetic flux density in T.
    \value DptValueMagneticMoment
           Stores magnetic moment in Am2.
    \value DptValueMagneticPolarization
           Stores magnetic polarization in T.
    \value DptValueMagnetization
           Stores magnetization in A/m.
    \value DptValueMagnetomotiveForce
           Stores the magnetomotive force in A.
    \value DptValueMass
           Stores the mass in kg.
    \value DptValueMassFlux
           Stores the mass flux in kg.
    \value DptValueMomentum
           Stores the momentum in N/s.
    \value DptValuePhaseAngleRad
           Stores the phase angle in radian.
    \value DptValuePhaseAngleDeg
           Stores the phase angle in degrees.
    \value DptValuePower
           Stores the power in W.
    \value DptValuePowerFactor
           Stores the power factor in cos Phi.
    \value DptValuePressure
           Stores the pressure in Pa.
    \value DptValueReactance
           Stores the reactance in Ohm.
    \value DptValueResistance
           Stores the resistance in Ohm.
    \value DptValueResistivity
           Stores the resistivity in Ohm*m.
    \value DptValueSelfInductance
           Stores the self inductance in H.
    \value DptValueSolidAngle
           Stores the solid angle in sr.
    \value DptValueSoundIntensity
           Stores the sound intensity in W/m2.
    \value DptValueSpeed
           Stores the speed in m/s.
    \value DptValueStress
           Stores the stress in Pa.
    \value DptValueSurfaceTension
           Stores the surface tension in N/m.
    \value DptValueCommonTemperature
           Stores the temperature in degrees Celsius.
    \value DptValueAbsoluteTemperature
           Stores the absolute temperature in K.
    \value DptValueTemperatureDifference
           Stores the temperature difference in K.
    \value DptValueThermalCapacity
           Stores the thermal capacity in J/K.
    \value DptValueThermalConductivity
           Stores the thermal conductivity in W/mk.
    \value DptValueThermoelectricPower
           Stores the thermoelectric power in V/K.
    \value DptValueTime
           Stores the time in seconds.
    \value DptValueTorque
           Stores the torque in Nm.
    \value DptValueVolume
           Stores the volume in m3.
    \value DptValueVolumeFlux
           Stores the volume flux in m3/s.
    \value DptValueWeight
           Stores the weight in N.
    \value DptValueWork
           Stores the work in J.
    \value DptValueAcceleration
           Stores the acceleration in m/s2.
    \value Dpt15_EntranceAccess
           A fixed size datapoint type for controlling entrance access.
    \value Dpt16_CharacterString
           A fixed size datapoint type that encodes a string.
    \value DptStringASCII
           Encodes a string of ASCII characters.
    \value DptString88591
           Encodes a string of ISO 8859-1 characters.
    \value Dpt17_SceneNumber
           A fixed size datapoint type with the length of 1 byte.
    \value DptSceneNumber
           Stores a scene number.
    \value Dpt18_SceneControl
           A fixed size datapoint type with the length of 1 byte.
    \value DptSceneControl
           Stores a scene control.
    \value Dpt19_DataTime
           A fixed size datapoint type with the length of 8 bytes.
    \value DptDateTime
           Stores the date and time.
    \value Dpt20_1Byte
           A fixed size datapoint type with the length of 1 byte.
    \value DptScloMode
           Stores the main functionality of the system clock (SCLO).
    \value DptBuildingMode
           Stores whether the building is in use and whether it is protected.
    \value DptOccMode
           Stores the occupy mode.
    \value DptPriority
           Stores the priority.
    \value DptLightApplicationMode
           Stores the light application mode.
    \value DptApplicationArea
           Stores the application area.
    \value DptAlarmClassType
           Stores the alarm class type.
    \value DptPsuMode
           Stores the bus power supply unit mode: decentral (distributed) bus
           power supply unit (DPSU) or central bus power supply unit (PSU).
    \value DptErrorClassSystem
           Stores fault signals transmitted in the application area
           \l QKnxApplicationArea::SystemFunctionCommonInterest.
    \value DptErrorClassHvac
           Stores fault signals transmitted in the heating, ventilation, and air
           conditioning (HVAC) application area.
    \value DptTimeDelay
           Stores a time delay.
    \value DptBeaufortWindForceScale
           Stores the wind force using the Beaufort wind force scale.
    \value DptSensorSelect
           Stores the sensor mode.
    \value DptActuatorConnectType
           Stores the actuator connection type. An actuator can be connected to
           a sensor or a controller.
    \value DptCommandMode
    \value DptAdditionalInfoTypes
    \value Dpt21_8BitSet
           A fixed size datapoint type with binary-coded values in all fields.
    \value DptGeneralStatus
           Stores general status: out of service, fault, overridden, or in
           alarm.
    \value DptDeviceControl
           Stores device control state.
    \value Dpt22_16BitSet
           A fixed size datapoint type with the length of 16 bits.
    \value DptMedia
           Stores the supported media: TP1, PL110, RF, or KNX IP.
    \value Dpt23_2BitSet
           A fixed size datapoint type with the length of 2 bits.
    \value DptOnOffAction
           Stores the on/off action.
    \value DptAlarmReaction
           Stores the alarm reaction.
    \value DptUpDownAction
           Stores the up/down action.
    \value Dpt24_VariableString
           A datapoint type that encodes a variable length string.
    \value DptVariableString88591
           Encodes a variable length string of ISO 8859-1 characters.
    \value Dpt25_2NibbleSet
           A fixed size datapoint type for storing the number of \e busy and
           \e inack repetitions.
    \value Dpt26_8BitSet
           A fixed size datapoint type with the length of 1 byte and the 8-bit
           set.
    \value DptSceneInfo
           Stores scene information.
    \value Dpt27_32BitSet
           A fixed size datapoint type with the 32-bit set.
    \value DptCombinedInfoOnOff
           Stores combined information about the on and off states of a
           multiple-channel binary actuator.
    \value Dpt28_StringUtf8
           A variable size datapoint type datapoint type for storing a UTF-8
           string.
    \value DptUtf8
           Stores a UTF-8 (Unicode Transformation Format-8) string.
    \value Dpt29_ElectricalEnergy
           A fixed size datapoint type with the length of 8 bytes for storing
           electrical energy.
    \value DptActiveEnergyV64
           Stores active energy in Wh.
    \value DptApparentEnergyV64
           Stores active energy in VAh.
    \value DptReactiveEnergyV64
           Stores active energy in VARh.
    \value Dpt30_24TimesChannelActivation
           A fixed size datapoint type for storing the activation state of a
           channel.
    \value DPT217_DatapointTypeVersion
           A fixed size datapoint type for storing version information.
    \value DptVersion
           Stores version information, such as software version, hardware
           version, or data-interface version.
    \value Dpt221
           A fixed size datapoint type for storing the serial number of a
           device.
    \value DptSerialNumber
           Stores a manufacturer code and an incremented number. The leading 4
           octets within a manufacturer’s code space must be globally unique.
    \value Dpt225_ScalingSpeed
           A fixed size datapoint type for storing scaling speed. Used in the
           lighting application area, only.
    \value Dpt232_3ByteColourRGB
           A fixed size datapoint type for storing an RGB color value.
    \value DptColourRGB
           Stores an RGB color value. Can be used for simple color control.
           How RGB is interpreted depends on the device, and therefore this
           coding is only suitable for point-to-point communication, where there
           is only a single receiver.
*/

/*!
    \fn template <typename T> T QKnxDatapointType::setBit(T byteToSet, bool value, quint8 bit)

    Sets the value of the \a bit in \a byteToSet to \a value.

    Returns \c true if the value was set; otherwise returns \c false.
*/

/*!
    \fn template <typename T> bool QKnxDatapointType::testBit(T byteToTest, quint8 bit)

    Tests the value of the \a bit in \a byteToTest.

    Returns \c true if the value was set; otherwise returns \c false.
*/

/*!
    \class QKnxFixedSizeDatapointType
    \inherits QKnxDatapointType
    \inmodule QtKnx

    \brief The QKnxFixedSizeDatapointType class is a base class for datapoint
    types with a fixed size.

    \sa Type
*/

/*!
    \class QKnxVariableSizeDatapointType
    \inherits QKnxDatapointType
    \inmodule QtKnx

    \brief The QKnxVariableSizeDatapointType is a base class for datapoint
    types with a variable size.

    \sa Type
*/

/*!
    Returns the type of the datapoint type.
*/
QKnxDatapointType::Type QKnxDatapointType::type() const
{
    return static_cast<Type> (d_ptr->m_type);
}

/*!
    Deletes the datapoint type.
*/
QKnxDatapointType::~QKnxDatapointType()
{}

/*!
    Creates a datapoint type with the type \a type and size \a size.
*/
QKnxDatapointType::QKnxDatapointType(Type type, int size)
    : d_ptr(new QKnxDatapointTypePrivate)
{
    auto number = QString::number(int(type));
    if (number.size() < 6)
        return;

    bool okMain = false, okSub = false;

    // Datapoint Type shall be identified by a 16 bit main number separated
    // by a dot from a 16 bit sub number. The assumption being made is that
    // QKnxDatapointType::Type is encoded in that way while omitting the dot.
    int mainType = number.left(number.size() - 5).toInt(&okMain);
    int subType = number.right(5).toInt(&okSub);

    if (okMain && okSub)
        d_ptr->setup(mainType, subType, quint32(type), size);
}

/*!
    Creates a datapoint type with the identifier \a dptId and size \a size.
*/
QKnxDatapointType::QKnxDatapointType(const QString &dptId, int size)
    : d_ptr(new QKnxDatapointTypePrivate)
{
    auto match = d_ptr->m_dpt.match(dptId);
    if (!match.hasMatch())
        return;

    QString subType = QStringLiteral("0");
    auto mainType = match.captured(QStringLiteral("MainOnly"));
    if (mainType.isEmpty()) {
        subType = match.captured(QStringLiteral("SubType"));
        mainType = match.captured(QStringLiteral("MainType"));
    }

    quint32 tmp;
    if (QKnxDatapointTypePrivate::toType(mainType, subType, &tmp))
        d_ptr->setup(mainType.toUInt(), subType.toUInt(), tmp, size);
}

/*!
    Creates a datapoint type with the main type \a mainType, subtype \a subType,
    and size \a size.
*/
QKnxDatapointType::QKnxDatapointType(quint16 mainType, quint16 subType, int size)
    : d_ptr(new QKnxDatapointTypePrivate)
{
    quint32 tmp;
    if (QKnxDatapointTypePrivate::toType(mainType, subType, &tmp))
        d_ptr->setup(mainType, subType, tmp, size);
}

/*!
    Returns the size of the datapoint type.
*/
int QKnxDatapointType::size() const
{
    return d_ptr->m_bytes.size();
}

/*!
    Returns the subtype of the datapoint type.
*/
int QKnxDatapointType::subType() const
{
    return d_ptr->m_subType;
}

/*!
    Returns the main type of the datapoint type.
*/
int QKnxDatapointType::mainType() const
{
    return d_ptr->m_mainType;
}

/*!
    Returns the minimum value of the datapoint type.
*/
QVariant QKnxDatapointType::minimum() const
{
    return d_ptr->m_minimum;
}

/*!
    Sets the minimum value of the datapoint type to \a minimum.
*/
void QKnxDatapointType::setMinimum(const QVariant &minimum)
{
    d_ptr->m_minimum = minimum;
}

/*!
    Returns the maximum value of the datapoint type.
*/
QVariant QKnxDatapointType::maximum() const
{
    return d_ptr->m_maximum;
}

/*!
    Sets the maximum value of the datapoint type to \a maximum.
*/
void QKnxDatapointType::setMaximum(const QVariant &maximum)
{
    d_ptr->m_maximum = maximum;
}

/*!
    Returns the coefficient of the datapoint type.
*/
double QKnxDatapointType::coefficient() const
{
    return d_ptr->m_coefficient;
}

/*!
    Sets the coefficient of the datapoint type to \a coef.
*/
void QKnxDatapointType::setCoefficient(double coef)
{
    d_ptr->m_coefficient = coef;
}

/*!
    Returns a description of the minimum value of the datapoint type.
*/
QString QKnxDatapointType::minimumText() const
{
    return d_ptr->m_minimumText;
}

/*!
    Sets the description of the minimum value of the datapoint type to
    \a minimumText.
*/
void QKnxDatapointType::setMinimumText(const QString &minimumText)
{
    d_ptr->m_minimumText = minimumText;
}

/*!
    Returns a description of the maximum value of the datapoint type.
*/
QString QKnxDatapointType::maximumText() const
{
    return d_ptr->m_maximumText;
}

/*!
    Sets the description of the maximum value of the datapoint type to
    \a maximumText.
*/
void QKnxDatapointType::setMaximumText(const QString &maximumText)
{
    d_ptr->m_maximumText = maximumText;
}

/*!
    Sets the minimum value of the datapoint type to \a minimum and the maximum
    value to \a maximum.
*/
void QKnxDatapointType::setRange(const QVariant &minimum, const QVariant &maximum)
{
    d_ptr->m_minimum = minimum;
    d_ptr->m_maximum = maximum;
}

/*!
    Sets the description of the minimum value of the datapoint type to
    \a minimumText and the description of the maximum value to \a maximumText.
*/
void QKnxDatapointType::setRangeText(const QString &minimumText, const QString &maximumText)
{
    d_ptr->m_minimumText = minimumText;
    d_ptr->m_maximumText = maximumText;
}

/*!
    Returns the unit of the datapoint type.
*/
QString QKnxDatapointType::unit() const
{
    return d_ptr->m_unit;
}

/*!
    Sets the unit of the datapoint type to \a unit.
*/
void QKnxDatapointType::setUnit(const QString &unit)
{
    d_ptr->m_unit = unit;
}

/*!
    Returns a description of the datapoint type.
*/
QString QKnxDatapointType::description() const
{
    return d_ptr->m_descrition;
}

/*!
    Sets the description of the datapoint type to \a description.
*/
void QKnxDatapointType::setDescription(const QString &description)
{
    d_ptr->m_descrition = description;
}

/*!
    Returns \c true if this is a valid datapoint type; otherwise returns
    \c false.
*/
bool QKnxDatapointType::isValid() const
{
    return (d_ptr->m_type != 0) && (d_ptr->m_bytes.size() > 0);
}

/*!
    Returns the data stored in the datapoint type.
*/
const quint8 *QKnxDatapointType::constData() const
{
    return d_ptr->m_bytes.constData();
}

/*!
    Returns the content of the byte at the index \a index.
*/
quint8 QKnxDatapointType::byte(quint16 index) const
{
    return d_ptr->m_bytes.value(index);
}

/*!
    Sets the content of the byte at the index \a index to \a bytes.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxDatapointType::setByte(quint16 index, quint8 bytes)
{
    if (index < d_ptr->m_bytes.size())
        d_ptr->m_bytes[index] = bytes;
    return (index < d_ptr->m_bytes.size());
}

/*!
    Returns the content of the bytes stored in the datapoint type.
*/
QKnxByteArray QKnxDatapointType::bytes() const
{
    return d_ptr->m_bytes;
}

/*!
    Sets the content of the bytes stored in the datapoint type to \a bytesToSet,
    from the position \a index in \a bytesToSet to the position calculated by
    adding \a count to \a index.

    For a fixed size datapoint, returns \c false if \a count does not correspond
    to the size of the datapoint type returned by \l size().
*/
bool QKnxDatapointType::setBytes(const QKnxByteArray &bytesToSet, quint16 index, quint16 count)
{
    if (((bytesToSet.size() - index) < count) || (size() != count))
        return false;
    d_ptr->m_bytes = bytesToSet;
    return true;
}

/*!
    Constructs a copy of \a other.
*/
QKnxDatapointType::QKnxDatapointType(const QKnxDatapointType &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns \a other to this datapoint type and returns a reference to this
    datapoint type.
*/
QKnxDatapointType &QKnxDatapointType::operator=(const QKnxDatapointType &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxDatapointType::QKnxDatapointType(QKnxDatapointType &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = Q_NULLPTR;
}

/*!
    Move assigns \a other to this datapoint type and returns a reference to this
    datapoint type.
*/
#ifdef Q_COMPILER_RVALUE_REFS
QKnxDatapointType &QKnxDatapointType::operator=(QKnxDatapointType &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}
#endif

/*!
    Swaps \a other with this datapoint. This operation never fails.
*/
void QKnxDatapointType::swap(QKnxDatapointType &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Returns \c true if this datapoint and the given \a other are equal;
    otherwise returns \c false.
*/
bool QKnxDatapointType::operator==(const QKnxDatapointType &other) const
{
    return d_ptr == other.d_ptr
        || (d_ptr->m_subType == other.d_ptr->m_subType
            && d_ptr->m_mainType == other.d_ptr->m_mainType
            && d_ptr->m_bytes == other.d_ptr->m_bytes
            && d_ptr->m_unit == other.d_ptr->m_unit
            && d_ptr->m_descrition == other.d_ptr->m_descrition
            && d_ptr->m_minimum == other.d_ptr->m_minimum
            && d_ptr->m_maximum == other.d_ptr->m_maximum
            && d_ptr->m_coefficient == other.d_ptr->m_coefficient
            && d_ptr->m_minimumText == other.d_ptr->m_minimumText
            && d_ptr->m_maximumText == other.d_ptr->m_maximumText);
}

/*!
    Returns \c true if this datapoint and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxDatapointType::operator!=(const QKnxDatapointType &other) const
{
    return !operator==(other);
}

/*!
    Converts a KNX datapoint type \a dpt of the format \c DPT-* or \c DPST-*-*
    into a \l {QKnxDatapointType::Type} enumeration used throughout the QtKnx
    API.
*/
QKnxDatapointType::Type QKnxDatapointType::toType(const QString &dpt)
{
    QKnxDatapointTypePrivate dtp;
    auto match = dtp.m_dpt.match(dpt);
    if (!match.hasMatch())
        return QKnxDatapointType::Type::Unknown;

    QString subType = QStringLiteral("0");
    auto mainType = match.captured(QStringLiteral("MainOnly"));
    if (mainType.isEmpty()) {
        subType = match.captured(QStringLiteral("SubType"));
        mainType = match.captured(QStringLiteral("MainType"));
    }

    quint32 type;
    if (dtp.toType(mainType, subType, &type))
        return static_cast<Type> (type);
    return QKnxDatapointType::Type::Unknown;
}


// -- private

/*!
    \internal
*/
void QKnxDatapointType::resize(int newSize)
{
    d_ptr->m_bytes.resize(newSize);
}

/*!
    \internal
*/
QKnxDatapointType::QKnxDatapointType(QKnxDatapointTypePrivate &dd)
    : d_ptr(new QKnxDatapointTypePrivate(dd))
{}


// -- QKnxVariableSizeDatapointType

/*!
    Creates a variable size datapoint type with the type \a type and the size
    \a size.
*/
QKnxVariableSizeDatapointType::QKnxVariableSizeDatapointType(Type type, int size)
    : QKnxDatapointType(type, size)
{}

/*!
   Creates a variable size datapoint type with the identifier \a dptId and the
    size \a size.
*/
QKnxVariableSizeDatapointType::QKnxVariableSizeDatapointType(const QString &dptId, int size)
    : QKnxDatapointType(dptId, size)
{}

/*!
    Creates a variable size datapoint type with the main type \a mainType, subtype
    \a subType, and the size \a size.
*/

QKnxVariableSizeDatapointType::QKnxVariableSizeDatapointType(quint16 mainType, quint16 subType, int size)
    : QKnxDatapointType(mainType, subType, size)
{}

/*!
    \reimp
*/
bool QKnxVariableSizeDatapointType::setBytes(const QKnxByteArray &bytesToSet, quint16 index, quint16 count)
{
    if ((bytesToSet.size() - index) < count)
        return false;

    resize(count);
    return QKnxDatapointType::setBytes(bytesToSet, index, count);
}

#include "moc_qknxdatapointtype.cpp"

QT_END_NAMESPACE

