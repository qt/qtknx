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

#include "qknx4bytefloat.h"
#include "qknxdatapointtype_p.h"

#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx4ByteFloat
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnx4ByteFloat class is datapoint type for a 4-byte float value.

    This is fixed size datapoint type with the length of 4 bytes.

    The range for the value of this datapoint type is from \c {-3.40282e+38} to
    \c {3.40282e+38}.

    This class is a base class for the following datapoint types:

    \list
        \li \c QKnxValueAbsoluteTemperature - Absolute temperature in K.
        \li \c QKnxValueAcceleration - Acceleration in m/s2.
        \li \c QKnxValueAccelerationAngular - Angular Acceleration in rad/s2.
        \li \c QKnxValueActivationEnergy - Activation Energy in J/mol.
        \li \c QKnxValueActivity - Radioactive activity in 1/s.
        \li \c QKnxValueAngleDeg - Angle in degrees.
        \li \c QKnxValueAngleRad - Angle in radian.
        \li \c QKnxValueAmplitude - Amplitude.
        \li \c QKnxValueAngularFrequency - Angular frequency in rad/s.
        \li \c QKnxValueAngularMomentum - Angular momentum in Js.
        \li \c QKnxValueAngularVelocity - Angular velocity in rad/s.
        \li \c QKnxValueArea - Area in m2.
        \li \c QKnxValueCapacitance - Capacitance in F.
        \li \c QKnxValueChargeDensitySurface - Flux density in C/m2.
        \li \c QKnxValueChargeDensityVolume - Charge density in C/m3.
        \li \c QKnxValueCompressibility - Compressibility in m2/N.
        \li \c QKnxValueCommonTemperature - Temperature in degrees Celsius.
        \li \c QKnxValueConductance - Conductance in S.
        \li \c QKnxValueDensity - Density in kg/m3.
        \li \c QKnxValueElectricalConductivity - Conductivity in S/m.
        \li \c QKnxValueElectricCharge - Electric charge in C.
        \li \c QKnxValueElectricCurrent - Electric current in A.
        \li \c QKnxValueElectricCurrentDensity - Electric current density in
            A/m2.
        \li \c QKnxValueElectricDipoleMoment - Electric dipole moment in Cm.
        \li \c QKnxValueElectricDisplacement - Electric displacement in C/m2.
        \li \c QKnxValueElectricFieldStrength - Electric field strength in V/m.
        \li \c QKnxValueElectricFlux - Electric flux in C.
        \li \c QKnxValueElectricFluxDensity - Electric flux density in C/m2.
        \li \c QKnxValueElectricPolarization - Electric polarization in C/m2.
        \li \c QKnxValueElectricPotential - Electric potential in V.
        \li \c QKnxValueElectricPotentialDifference - Electric potential
            difference in V.
        \li \c QKnxValueElectromagneticMoment - Electromagnetic moment in Am2.
        \li \c QKnxValueElectromotiveForce - Electromotive force in V.
        \li \c QKnxValueEnergy - Energy in J.
        \li \c QKnxValueForce - Force in N.
        \li \c QKnxValueFrequency - Frequency in Hz.
        \li \c QKnxValueHeatCapacity - Heat capacity in J/K.
        \li \c QKnxValueHeatFlowRate - Heat flow rate in W.
        \li \c QKnxValueHeatQuantity - Heat quantity in J.
        \li \c QKnxValueImpedance - Impedance in Ohm.
        \li \c QKnxValueLength - Length in m.
        \li \c QKnxValueLightQuantity - Light quantity in J.
        \li \c QKnxValueLuminance - Luminance in cd/m2.
        \li \c QKnxValueLuminousFlux - Luminous flux in lm.
        \li \c QKnxValueLuminousIntensity - Luminous intensity in cd.
        \li \c QKnxValueMagneticFieldStrength - Magnetic field strength in A/m.
        \li \c QKnxValueMagneticFlux - Magnetic flux in Wb.
        \li \c QKnxValueMagneticFluxDensity - Magnetic flux density in T.
        \li \c QKnxValueMagneticMoment - Magnetic moment in Am2.
        \li \c QKnxValueMagneticPolarization - Magnetic polarization in T.
        \li \c QKnxValueMagnetization - Magnetization in A/m.
        \li \c QKnxValueMagnetomotiveForce - Magnetomotive force in A.
        \li \c QKnxValueMass - Mass in kg.
        \li \c QKnxValueMassFlux - Mass flux in kg/s.
        \li \c QKnxValueMol - Amount of substance in mol.
        \li \c QKnxValueMomentum - Momentum in N/s.
        \li \c QKnxValuePhaseAngleDeg - Phase angle in degrees.
        \li \c QKnxValuePhaseAngleRad - Phase angle in radian.
        \li \c QKnxValuePower - Power in W.
        \li \c QKnxValuePowerFactor - Power factor in cos Phi.
        \li \c QKnxValuePressure - Pressure in Pa.
        \li \c QKnxValueReactance - Reactance in Ohm.
        \li \c QKnxValueResistance - Resistance in Ohm.
        \li \c QKnxValueResistivity - Resistivity in Ohm*m.
        \li \c QKnxValueSelfInductance - Self inductance in H.
        \li \c QKnxValueSolidAngle - Solid angle in sr.
        \li \c QKnxValueSoundIntensity - Sound intensity in W/m2.
        \li \c QKnxValueSpeed - Speed in m/s.
        \li \c QKnxValueStress - Stress in Pa.
        \li \c QKnxValueSurfaceTension - Surface tension in N/m.
        \li \c QKnxValueTemperatureDifference - Temperature difference in K.
        \li \c QKnxValueThermalCapacity - Thermal capacity in J/K.
        \li \c QKnxValueThermalConductivity - Thermal conductivity in W/mk.
        \li \c QKnxValueThermoelectricPower - Thermoelectric power in V/K.
        \li \c QKnxValueTime - Time in seconds.
        \li \c QKnxValueTorque - Torque in Nm.
        \li \c QKnxValueVolume - Volume in m3.
        \li \c QKnxValueVolumeFlux - Volume flux in m3/s.
        \li \c QKnxValueWeight - Weight in N.
        \li \c QKnxValueWork - Work in J.
    \endlist

    \sa QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

/*!
    Creates a fixed size datapoint type with the value set to \c {0.0}.
*/
QKnx4ByteFloat::QKnx4ByteFloat()
    : QKnx4ByteFloat(0.0)
{}

/*!
    Creates a fixed size datapoint type with the float \a value.
*/
QKnx4ByteFloat::QKnx4ByteFloat(float value)
    : QKnx4ByteFloat(SubType, value)
{}

/*!
    Creates a fixed size datapoint type with the subtype \a subType and float
    \a value.
*/
QKnx4ByteFloat::QKnx4ByteFloat(int subType, float value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("4-byte float value"));
    setRangeText(tr("Minimum Value, -3.40282e+38"), tr("Maximum Value, 3.40282e+38"));
    setRange(QVariant::fromValue(std::numeric_limits<float>::lowest()),
        QVariant::fromValue(std::numeric_limits<float>::max()));

    setValue(value);
}

/*!
    Returns the value stored in the datapoint type.
*/
float QKnx4ByteFloat::value() const
{
    quint32 temp = QKnxUtils::QUint32::fromBytes(bytes());
    float value = 0;
    memcpy(&value, &temp, sizeof(value));
    return value;
}

/*!
    Sets the float of the datapoint type to \a value.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
void QKnx4ByteFloat::setValue(float value)
{
    quint32 tmp;
    memcpy(&tmp, &value, sizeof(value));
    setBytes(QKnxUtils::QUint32::bytes(tmp),0,4);
}

/*!
    \reimp
*/
bool QKnx4ByteFloat::isValid() const
{
    return QKnxDatapointType::isValid()
        && value() >= minimum().toFloat() && value() <= maximum().toFloat();
}

#define CREATE_CLASS_BODY(CLASS, DESCRIPTION, UNIT) \
CLASS::CLASS() \
    : QKnx4ByteFloat(SubType, 0.0) \
{ \
    setUnit(tr(UNIT)); \
    setDescription(tr(DESCRIPTION)); \
} \
CLASS::CLASS(float value) \
    : CLASS() \
{ \
    setValue(value); \
}

CREATE_CLASS_BODY(QKnxValueAcceleration, "Acceleration (m/s2)", "m/s2")
CREATE_CLASS_BODY(QKnxValueAccelerationAngular, "Angular Acceleration (rad/s2)", "rad/s2")
CREATE_CLASS_BODY(QKnxValueActivationEnergy, "Activation Energy (J/mol)", "J/mol")
CREATE_CLASS_BODY(QKnxValueActivity, "Radioactive activity (1/s)", "1/s")
CREATE_CLASS_BODY(QKnxValueMol, "Amount of substance (mol)", "mol")
CREATE_CLASS_BODY(QKnxValueAmplitude, "Amplitude", "")
CREATE_CLASS_BODY(QKnxValueAngleRad, "Angle (radian)", "rad")
CREATE_CLASS_BODY(QKnxValueAngleDeg, "Angle (degree)", "degree")
CREATE_CLASS_BODY(QKnxValueAngularMomentum, "Angular momentum (Js)", "Js")
CREATE_CLASS_BODY(QKnxValueAngularVelocity, "Angular velocity (rad/s)", "rad/s")
CREATE_CLASS_BODY(QKnxValueArea, "Area (m2)", "m2")
CREATE_CLASS_BODY(QKnxValueCapacitance, "Capacitance (F)", "F")
CREATE_CLASS_BODY(QKnxValueChargeDensitySurface, "Flux density (C/m2)", "C/m2")
CREATE_CLASS_BODY(QKnxValueChargeDensityVolume, "Charge density (C/m3)", "C/m3")
CREATE_CLASS_BODY(QKnxValueCompressibility, "Compressibility (m2/N)", "m2/N")
CREATE_CLASS_BODY(QKnxValueConductance, "Conductance (S)", "S")
CREATE_CLASS_BODY(QKnxValueElectricalConductivity, "Conductivity (S/m)", "S/m")
CREATE_CLASS_BODY(QKnxValueDensity, "Density (kg/m3)", "kg/m3")
CREATE_CLASS_BODY(QKnxValueElectricCharge, "Electric charge (C)", "C")
CREATE_CLASS_BODY(QKnxValueElectricCurrent, "Electric current (A)", "A")
CREATE_CLASS_BODY(QKnxValueElectricCurrentDensity, "Electric current density (A/m2)", "A/m2")
CREATE_CLASS_BODY(QKnxValueElectricDipoleMoment, "Electric dipole moment (Cm)", "Cm")
CREATE_CLASS_BODY(QKnxValueElectricDisplacement, "Electric displacement (C/m2)", "C/m2")
CREATE_CLASS_BODY(QKnxValueElectricFieldStrength, "Electric field strength (V/m)", "V/m")
CREATE_CLASS_BODY(QKnxValueElectricFlux, "Electric flux (C)", "C")
CREATE_CLASS_BODY(QKnxValueElectricFluxDensity, "Electric flux density (C/m2)", "C/m2")
CREATE_CLASS_BODY(QKnxValueElectricPolarization, "Electric polarization (C/m2)", "C/m2")
CREATE_CLASS_BODY(QKnxValueElectricPotential, "Electric potential(V)", "V")
CREATE_CLASS_BODY(QKnxValueElectricPotentialDifference, "Electric potential difference (V)", "V")
CREATE_CLASS_BODY(QKnxValueElectromagneticMoment, "Electromagnetic moment (Am2)", "Am2")
CREATE_CLASS_BODY(QKnxValueElectromotiveForce, "Electromotive force (V)", "V")
CREATE_CLASS_BODY(QKnxValueEnergy, "Energy (J)", "J")
CREATE_CLASS_BODY(QKnxValueForce, "Force (N)", "N")
CREATE_CLASS_BODY(QKnxValueFrequency, "Frequency (Hz)", "Hz")
CREATE_CLASS_BODY(QKnxValueAngularFrequency, "Angular frequency (rad/s)", "rad/s")
CREATE_CLASS_BODY(QKnxValueHeatCapacity, "Heat capacity (J/K)", "J/K")
CREATE_CLASS_BODY(QKnxValueHeatFlowRate, "Heat flow rate (W)", "W")
CREATE_CLASS_BODY(QKnxValueHeatQuantity, "Heat quantity", "J")
CREATE_CLASS_BODY(QKnxValueImpedance, "Impedance (Ohm)", "Ohm")
CREATE_CLASS_BODY(QKnxValueLength, "Length (m)", "m")
CREATE_CLASS_BODY(QKnxValueLightQuantity, "Light quantity (J)", "J")
CREATE_CLASS_BODY(QKnxValueLuminance, "Luminance (cd/m2)", "cd/m2")
CREATE_CLASS_BODY(QKnxValueLuminousFlux, "Luminous flux (lm)", "lm")
CREATE_CLASS_BODY(QKnxValueLuminousIntensity, "Luminous intensity (cd)", "cd")
CREATE_CLASS_BODY(QKnxValueMagneticFieldStrength, "Magnetic field strength (A/m)", "A/m")
CREATE_CLASS_BODY(QKnxValueMagneticFlux, "Magnetic flux (Wb)", "Wb")
CREATE_CLASS_BODY(QKnxValueMagneticFluxDensity, "Magnetic flux density (T)", "T")
CREATE_CLASS_BODY(QKnxValueMagneticMoment, "Magnetic moment (Am2)", "Am2")
CREATE_CLASS_BODY(QKnxValueMagneticPolarization, "Magnetic polarization (T)", "T")
CREATE_CLASS_BODY(QKnxValueMagnetization, "Magnetization (A/m)", "A/m")
CREATE_CLASS_BODY(QKnxValueMagnetomotiveForce, "Magnetomotive force (A)", "A")
CREATE_CLASS_BODY(QKnxValueMass, "Mass (kg)", "kg")
CREATE_CLASS_BODY(QKnxValueMassFlux, "Mass flux (kg/s)", "kg/s")
CREATE_CLASS_BODY(QKnxValueMomentum, "Momentum (N/s)", "N/s")
CREATE_CLASS_BODY(QKnxValuePhaseAngleRad, "Phase angle (rad)", "rad")
CREATE_CLASS_BODY(QKnxValuePhaseAngleDeg, "Phase Angle (degree)", "degree")
CREATE_CLASS_BODY(QKnxValuePower, "Power (W)", "W")
CREATE_CLASS_BODY(QKnxValuePowerFactor, "Power factor (cos Phi)", "cos Phi")
CREATE_CLASS_BODY(QKnxValuePressure, "Pressure (Pa)", "Pa")
CREATE_CLASS_BODY(QKnxValueReactance, "Reactance (Ohm)", "Ohm")
CREATE_CLASS_BODY(QKnxValueResistance, "Resistance (Ohm)", "Ohm")
CREATE_CLASS_BODY(QKnxValueResistivity, "Resistivity (Ohm*m)", "Ohm*m")
CREATE_CLASS_BODY(QKnxValueSelfInductance, "Self inductance (H)", "H")
CREATE_CLASS_BODY(QKnxValueSolidAngle, "Solid angle (sr)", "sr")
CREATE_CLASS_BODY(QKnxValueSoundIntensity, "Sound intensity (W/m2)", "W/m2")
CREATE_CLASS_BODY(QKnxValueSpeed, "Speed (m/s)", "m/s")
CREATE_CLASS_BODY(QKnxValueStress, "Stress (Pa)", "Pa")
CREATE_CLASS_BODY(QKnxValueSurfaceTension, "Surface tension (N/m)", "N/m")
CREATE_CLASS_BODY(QKnxValueCommonTemperature, "Temperature (degree Celsius)", "degree Celsius")
CREATE_CLASS_BODY(QKnxValueAbsoluteTemperature, "Absolute temperature (K)", "K")
CREATE_CLASS_BODY(QKnxValueTemperatureDifference, "Temperature difference (K)", "K")
CREATE_CLASS_BODY(QKnxValueThermalCapacity, "Thermal capacity (J/K)", "J/K")
CREATE_CLASS_BODY(QKnxValueThermalConductivity, "Thermal conductivity (W/mk)", "W/mk")
CREATE_CLASS_BODY(QKnxValueThermoelectricPower, "Thermoelectric power (V/K)", "V/K")
CREATE_CLASS_BODY(QKnxValueTime, "Time (s)", "s")
CREATE_CLASS_BODY(QKnxValueTorque, "Torque (Nm)", "Nm")
CREATE_CLASS_BODY(QKnxValueVolume, "Volume (m3)", "m3")
CREATE_CLASS_BODY(QKnxValueVolumeFlux, "Volume flux (m3/s)", "m3/s")
CREATE_CLASS_BODY(QKnxValueWeight, "Weight (N)", "N")
CREATE_CLASS_BODY(QKnxValueWork, "Work (J)", "J")

#undef CREATE_CLASS_BODY

QT_END_NAMESPACE
