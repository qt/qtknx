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

    This data is used in the data part of the \l QKnxNpdu class. To retrieve
    the data of a data point type, call bytes().

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
    \value DptBool
    \value DptEnable
    \value DptRamp
    \value DptAlarm
    \value DptBinaryValue
    \value DptStep
    \value DptUpDown
    \value DptOpenClose
    \value DptStart
    \value DptState
    \value DptInvert
    \value DptDimSendStyle
    \value DptInputSource
    \value DptReset
    \value DptAck
    \value DptTrigger
    \value DptOccupancy
    \value DptWindowDoor
    \value DptLogicalFunction
    \value DptSceneAB
    \value DptShutterBlindsMode
    \value DptHeatCool
    \value Dpt2_1BitControlled
           A fixed size datapoint type with the length of 2 bits. This is a
           a boolean datapoint type with a control part.
    \value DptSwitchControl
    \value DptBoolControl
    \value DptEnableControl
    \value DptRampControl
    \value DptAlarmControl
    \value DptBinaryValueControl
    \value DptStepControl
    \value DptDirection1Control
    \value DptDirection2Control
    \value DptStartControl
    \value DptStateControl
    \value DptInvertControl
    \value Dpt3_3BitControlled
            A fixed size datapoint type with the length of 3 bits.
    \value DptControlDimming
            Controls light dimming.
    \value DptControlBlinds
            Controls blinds.
    \value Dpt4_Character
            A fixed size datapoint type with the length of 1 byte that encodes
            a character.
    \value DptCharAscii
    \value DptChar88591
    \value Dpt5_8bitUnsigned
    \value DptScaling
    \value DptAngle
    \value DptPercentU8
    \value DptDecimalFactor
    \value DptTariff
    \value DptValue1Ucount
    \value Dpt6_8bitSigned
    \value DptPercentV8
    \value DptValue1Count
    \value DptStatusMode3
    \value Dpt7_2ByteUnsigned
    \value DptValue2UCount
    \value DptTimePeriodMsec
    \value DptTimePeriod10Msec
    \value DptTimePeriod100Msec
    \value DptTimePeriodSec
    \value DptTimePeriodMin
    \value DptTimePeriodHrs
    \value DptPropertyDataType
    \value DptLengthMilliMeter
    \value DptUEICurrentMilliA
    \value DptBrightness
    \value Dpt8_2ByteSigned
    \value DptValue2Count
    \value DptDeltaTimeMsec
    \value DptDeltaTime10Msec
    \value DptDeltaTime100Msec
    \value DptDeltaTimeSec
    \value DptDeltaTimeMin
    \value DptDeltaTimeHrs
    \value DptPercentV16
    \value DptRotationAngle
    \value Dpt9_2ByteFloat
    \value DptTemperatureCelsius
    \value DptTemperatureKelvin
    \value DptTemperatureChange
    \value DptValueLux
    \value DptWindSpeed
    \value DptPressure
    \value DptHumidity
    \value DptAirQuality
    \value DptAirFlow
    \value DptTimeSecond
    \value DptTimeMilliSecond
    \value DptVoltage
    \value DptCurrent
    \value DptPowerDensity
    \value DptKelvinPerPercent
    \value DptPower
    \value DptVolumeFlow
    \value DptAmountRain
    \value DptTemperatureFahrenheit
    \value DptWindSpeedKmPerHour
    \value Dpt10_TimeOfDay
    \value DptTimeOfDay
    \value Dpt11_Date
    \value DptDate
    \value Dpt12_4ByteUnsigned
    \value DptValue4Ucount
    \value Dpt13_4ByteSigned
    \value DptValue4Count
    \value DptFlowRateCubicMeterPerHour
    \value DptApparentEnergy
    \value DptReactiveEnergy
    \value DptActiveEnergykWh
    \value Dpt_ApparentEnergykVAh
    \value DptReactiveEnergykVARh
    \value DptActiveEnergy
    \value DptLongDeltaTimeSec
    \value Dpt14_4ByteFloat
    \value DptValueAccelerationAngular
    \value DptValueActivationEnergy
    \value DptValueActivity
    \value DptValueMol
    \value DptValueAmplitude
    \value DptValueAngleRad
    \value DptValueAngleDeg
    \value DptValueAngularMomentum
    \value DptValueAngularVelocity
    \value DptValueArea
    \value DptValueCapacitance
    \value DptValueChargeDensitySurface
    \value DptValueChargeDensityVolume
    \value DptValueCompressibility
    \value DptValueConductance
    \value DptValueElectricalConductivity
    \value DptValueDensity
    \value DptValueElectricCharge
    \value DptValueElectricCurrent
    \value DptValueElectricCurrentDensity
    \value DptValueElectricDipoleMoment
    \value DptValueElectricDisplacement
    \value DptValueElectricFieldStrength
    \value DptValueElectricFlux
    \value DptValueElectricFluxDensity
    \value DptValueElectricPolarization
    \value DptValueElectricPotential
    \value DptValueElectricPotentialDifference
    \value DptValueElectromagneticMoment
    \value DptValueElectromotiveForce
    \value DptValueEnergy
    \value DptValueForce
    \value DptValueFrequency
    \value DptValueAngularFrequency
    \value DptValueHeatCapacity
    \value DptValueHeatFlowRate
    \value DptValueHeatQuantity
    \value DptValueImpedance
    \value DptValueLength
    \value DptValueLightQuantity
    \value DptValueLuminance
    \value DptValueLuminousFlux
    \value DptValueLuminousIntensity
    \value DptValueMagneticFieldStrength
    \value DptValueMagneticFlux
    \value DptValueMagneticFluxDensity
    \value DptValueMagneticMoment
    \value DptValueMagneticPolarization
    \value DptValueMagnetization
    \value DptValueMagnetomotiveForce
    \value DptValueMass
    \value DptValueMassFlux
    \value DptValueMomentum
    \value DptValuePhaseAngleRad
    \value DptValuePhaseAngleDeg
    \value DptValuePower
    \value DptValuePowerFactor
    \value DptValuePressure
    \value DptValueReactance
    \value DptValueResistance
    \value DptValueResistivity
    \value DptValueSelfInductance
    \value DptValueSolidAngle
    \value DptValueSoundIntensity
    \value DptValueSpeed
    \value DptValueStress
    \value DptValueSurfaceTension
    \value DptValueCommonTemperature
    \value DptValueAbsoluteTemperature
    \value DptValueTemperatureDifference
    \value DptValueThermalCapacity
    \value DptValueThermalConductivity
    \value DptValueThermoelectricPower
    \value DptValueTime
    \value DptValueTorque
    \value DptValueVolume
    \value DptValueVolumeFlux
    \value DptValueWeight
    \value DptValueWork
    \value DptValueAcceleration
    \value Dpt15_EntranceAccess
    \value Dpt16_CharacterString
    \value DptStringASCII
    \value DptString88591
    \value Dpt17_SceneNumber
    \value DptSceneNumber
    \value Dpt18_SceneControl
    \value DptSceneControl
    \value Dpt19_DataTime
    \value DptDateTime
    \value Dpt20_1Byte
    \value DptScloMode
    \value DptBuildingMode
    \value DptOccMode
    \value DptPriority
    \value DptLightApplicationMode
    \value DptApplicationArea
    \value DptAlarmClassType
    \value DptPsuMode
    \value DptErrorClassSystem
    \value DptErrorClassHvac
    \value DptTimeDelay
    \value DptBeaufortWindForceScale
    \value DptSensorSelect
    \value DptActuatorConnectType
    \value DptCommandMode
    \value DptAdditionalInfoTypes
    \value Dpt21_8BitSet
    \value DptGeneralStatus
    \value DptDeviceControl
    \value Dpt22_16BitSet
    \value DptMedia
    \value Dpt23_2BitSet
    \value DptOnOffAction
    \value DptAlarmReaction
    \value DptUpDownAction
    \value Dpt24_VariableString
    \value DptVariableString88591
    \value Dpt25_2NibbleSet
    \value Dpt26_8BitSet
    \value DptSceneInfo
    \value Dpt27_32BitSet
    \value DptCombinedInfoOnOff
    \value Dpt28_StringUtf8
    \value DptUtf8
    \value Dpt29_ElectricalEnergy
    \value DptActiveEnergyV64
    \value DptApparentEnergyV64
    \value DptReactiveEnergyV64
    \value Dpt30_24TimesChannelActivation
    \value DPT217_DatapointTypeVersion
    \value DptVersion
    \value Dpt221
    \value DptSerialNumber
    \value Dpt225_ScalingSpeed
    \value Dpt232_3ByteColourRGB
    \value DptColourRGB
*/

/*!
    \fn QKnxDatapointType::bytes()

    Returns the content of the bytes stored in the datapoint type.
*/

/*!
    \fn bool QKnxDatapointType::setBytes(const T &bytesToSet, quint16 index, quint16 count)

    Sets the content of the bytes stored in the datapoint type to \a bytesToSet,
    from the position \a index in \a bytesToSet to the position calculated by
    adding \a count to \a index.

    For a fixed size datapoint, \a count must correspond to the size of the
    datapoint type returned by \l size().
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
    \fn QKnxFixedSizeDatapointType::QKnxFixedSizeDatapointType(Type type, int size)

    Creates a fixed size datapoint type with the type \a type and the size
    \a size.
*/

/*!
    \fn QKnxFixedSizeDatapointType::QKnxFixedSizeDatapointType(const QString &dptId, int size)

    Creates a fixed size datapoint type with the identifier \a dptId and the
    size \a size.
*/

/*!
    \fn QKnxFixedSizeDatapointType::QKnxFixedSizeDatapointType(quint16 mainType, quint16 subType, int size)

    Creates a fixed size datapoint type with the main type \a mainType, subtype
    \a subType, and the size \a size.
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
    \fn QKnxVariableSizeDatapointType::QKnxVariableSizeDatapointType(Type type, int size)

    Creates a variable size datapoint type with the type \a type and the size
    \a size.
*/

/*!
    \fn QKnxVariableSizeDatapointType::QKnxVariableSizeDatapointType(const QString &dptId, int size)

    Creates a variable size datapoint type with the identifier \a dptId and the
    size \a size.
*/

/*!
    \fn QKnxVariableSizeDatapointType::QKnxVariableSizeDatapointType(quint16 mainType, quint16 subType, int size)

    Creates a variable size datapoint type with the main type \a mainType, subtype
    \a subType, and the size \a size.
*/

//TODO: make sure the code description is correct and enough.

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
    Whether this is a valid datapoint type.
*/
bool QKnxDatapointType::isValid() const
{
    return (d_ptr->m_type != 0) && (d_ptr->m_bytes.size() > 0);
}

/*!
    Returns the contents of the bytes stored in the datapoint type as a string.
*/
QString QKnxDatapointType::toString() const
{
    return QLatin1String(QByteArray((const char*) d_ptr->m_bytes.constData(), d_ptr->m_bytes
        .size()).toHex());
}

/*!
    Returns the content of the byte at index position \a i as a modifiable
    reference.
*/
quint8 &QKnxDatapointType::operator[](int i)
{
    Q_ASSERT_X(i >= 0 && i < d_ptr->m_bytes.size(), "QKnxDatapointType::operator[]",
        "index out of range");
    return d_ptr->m_bytes[i];
}

/*!
    Returns the content of the byte at index position \a i as a modifiable
    reference.
*/
const quint8 &QKnxDatapointType::operator[](int i) const
{
    Q_ASSERT_X(i >= 0 && i < d_ptr->m_bytes.size(), "QKnxDatapointType::operator[]",
        "index out of range");
    return d_ptr->m_bytes[i];
}

/*!
    Returns the data stored in the datapoint type.
*/
quint8 *QKnxDatapointType::data()
{
    return d_ptr->m_bytes.data();
}

/*!
    Returns the data stored in the datapoint type.
*/
const quint8 *QKnxDatapointType::data() const
{
    return d_ptr->m_bytes.data();
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
    return (index < d_ptr->m_bytes.size() ? d_ptr->m_bytes[index] : quint8{});
}

/*!
    Sets the content of the byte at the index \a index to \a bytes.
*/
bool QKnxDatapointType::setByte(quint16 index, quint8 bytes)
{
    if (index < d_ptr->m_bytes.size())
        d_ptr->m_bytes[index] = bytes;
    return (index < d_ptr->m_bytes.size());
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
    Assigns \a other to this datapoint type and returns a reference to this
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
    Swaps the datapoint type \a other with this datapoint type.
*/
void QKnxDatapointType::swap(QKnxDatapointType &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Returns \c true if \a other points to the same item as this datapoint type.
    Otherwise, returns \c false.
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
    Returns \c true if \a other points to a different item than this datapoint
    type. Otherwise, returns \c false.
*/
bool QKnxDatapointType::operator!=(const QKnxDatapointType &other) const
{
    return !operator==(other);
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

#include "moc_qknxdatapointtype.cpp"

QT_END_NAMESPACE
