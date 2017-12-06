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

    \brief The QKnx4ByteFloat class is datapoint type for a 4-byte float value.

    This is fixed size datapoint type with the length of 4 bytes.

    \sa QKnxDatapointType
*/

QKnx4ByteFloat::QKnx4ByteFloat()
    : QKnx4ByteFloat(0.0)
{}

QKnx4ByteFloat::QKnx4ByteFloat(float value)
    : QKnx4ByteFloat(SubType, value)
{}

QKnx4ByteFloat::QKnx4ByteFloat(int subType, float value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("4-byte float value"));
    setRangeText(tr("Minimum Value, -3.40282e+38"), tr("Maximum Value, 3.40282e+38"));
    setRange(QVariant::fromValue(std::numeric_limits<float>::lowest()),
        QVariant::fromValue(std::numeric_limits<float>::max()));

    setValue(value);
}

float QKnx4ByteFloat::value() const
{
    quint32 temp = QKnxUtils::QUint32::fromBytes(bytes());
    float value = 0;
    memcpy(&value, &temp, sizeof(value));
    return value;
}

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
CREATE_CLASS_BODY(QKnxValueCommonTemperature, "Temperature (degree Celcius)", "degree Celcius")
CREATE_CLASS_BODY(QKnxValueAbsoluteTemperature, "Absolute temperature (K)", "K")
CREATE_CLASS_BODY(QKnxValueTemperatureDifference, "Temperature difference (K)", "K")
CREATE_CLASS_BODY(QKnxValueThermalCapacity, "Thermal capacity (J/K)", "J/K")
CREATE_CLASS_BODY(QKnxValueThermalConductivity, "Thermal conductivity (W/mk)", "W/mk")
CREATE_CLASS_BODY(QKnxValueThermoelectricPower, "Thermoelectic poser (V/K)", "V/K")
CREATE_CLASS_BODY(QKnxValueTime, "Time (s)", "s")
CREATE_CLASS_BODY(QKnxValueTorque, "Torque (Nm)", "Nm")
CREATE_CLASS_BODY(QKnxValueVolume, "Volume (m3)", "m3")
CREATE_CLASS_BODY(QKnxValueVolumeFlux, "Volume flux (m3/s)", "m3/s")
CREATE_CLASS_BODY(QKnxValueWeight, "Weight (N)", "N")
CREATE_CLASS_BODY(QKnxValueWork, "Work (J)", "J")

#undef CREATE_CLASS_BODY

QT_END_NAMESPACE
