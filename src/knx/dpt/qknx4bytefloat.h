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

#ifndef QKNX4BYTEFLOAT_H
#define QKNX4BYTEFLOAT_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx4ByteFloat : public QKnxFixedSizeDatapointType
{
public:

    QKnx4ByteFloat();
    explicit QKnx4ByteFloat(float value);

    static const constexpr int TypeSize = 0x04;
    static const constexpr int MainType = 0x0e;

    float value() const;
    void setValue(float value);

    bool isValid() const override;

protected:
    QKnx4ByteFloat(int subType, float value);
};

#define CREATE_CLASS_DECLARATION(CLASS, SUB_TYPE) \
class Q_KNX_EXPORT CLASS : public QKnx4ByteFloat \
{ \
public: \
    CLASS(); \
    explicit CLASS(float value); \
    static const constexpr int SubType = SUB_TYPE; \
};

CREATE_CLASS_DECLARATION(QKnxValueAcceleration, 0x00)
CREATE_CLASS_DECLARATION(QKnxValueAccelerationAngular, 0x01)
CREATE_CLASS_DECLARATION(QKnxValueActivationEnergy, 0x02)
CREATE_CLASS_DECLARATION(QKnxValueActivity, 0x03)
CREATE_CLASS_DECLARATION(QKnxValueMol, 0x04)
CREATE_CLASS_DECLARATION(QKnxValueAmplitude, 0x05)
CREATE_CLASS_DECLARATION(QKnxValueAngleRad, 0x06)
CREATE_CLASS_DECLARATION(QKnxValueAngleDeg, 0x07)
CREATE_CLASS_DECLARATION(QKnxValueAngularMomentum, 0x08)
CREATE_CLASS_DECLARATION(QKnxValueAngularVelocity, 0x09)
CREATE_CLASS_DECLARATION(QKnxValueArea, 0x0a)
CREATE_CLASS_DECLARATION(QKnxValueCapacitance, 0x0b)
CREATE_CLASS_DECLARATION(QKnxValueChargeDensitySurface, 0x0c)
CREATE_CLASS_DECLARATION(QKnxValueChargeDensityVolume, 0x0d)
CREATE_CLASS_DECLARATION(QKnxValueCompressibility, 0x0e)
CREATE_CLASS_DECLARATION(QKnxValueConductance, 0x0f)
CREATE_CLASS_DECLARATION(QKnxValueElectricalConductivity, 0x10)
CREATE_CLASS_DECLARATION(QKnxValueDensity, 0x11)
CREATE_CLASS_DECLARATION(QKnxValueElectricCharge, 0x12)
CREATE_CLASS_DECLARATION(QKnxValueElectricCurrent, 0x13)
CREATE_CLASS_DECLARATION(QKnxValueElectricCurrentDensity, 0x14)
CREATE_CLASS_DECLARATION(QKnxValueElectricDipoleMoment, 0x15)
CREATE_CLASS_DECLARATION(QKnxValueElectricDisplacement, 0x16)
CREATE_CLASS_DECLARATION(QKnxValueElectricFieldStrength, 0x17)
CREATE_CLASS_DECLARATION(QKnxValueElectricFlux, 0x18)
CREATE_CLASS_DECLARATION(QKnxValueElectricFluxDensity, 0x19)
CREATE_CLASS_DECLARATION(QKnxValueElectricPolarization, 0x1a)
CREATE_CLASS_DECLARATION(QKnxValueElectricPotential, 0x1b)
CREATE_CLASS_DECLARATION(QKnxValueElectricPotentialDifference, 0x1c)
CREATE_CLASS_DECLARATION(QKnxValueElectromagneticMoment, 0x1d)
CREATE_CLASS_DECLARATION(QKnxValueElectromotiveForce, 0x1e)
CREATE_CLASS_DECLARATION(QKnxValueEnergy, 0x1f)
CREATE_CLASS_DECLARATION(QKnxValueForce, 0x20)
CREATE_CLASS_DECLARATION(QKnxValueFrequency, 0x21)
CREATE_CLASS_DECLARATION(QKnxValueAngularFrequency, 0x22)
CREATE_CLASS_DECLARATION(QKnxValueHeatCapacity, 0x23)
CREATE_CLASS_DECLARATION(QKnxValueHeatFlowRate, 0x24)
CREATE_CLASS_DECLARATION(QKnxValueHeatQuantity, 0x25)
CREATE_CLASS_DECLARATION(QKnxValueImpedance, 0x26)
CREATE_CLASS_DECLARATION(QKnxValueLength, 0x27)
CREATE_CLASS_DECLARATION(QKnxValueLightQuantity, 0x28)
CREATE_CLASS_DECLARATION(QKnxValueLuminance, 0x29)
CREATE_CLASS_DECLARATION(QKnxValueLuminousFlux, 0x2a)
CREATE_CLASS_DECLARATION(QKnxValueLuminousIntensity, 0x2b)
CREATE_CLASS_DECLARATION(QKnxValueMagneticFieldStrength, 0x2c)
CREATE_CLASS_DECLARATION(QKnxValueMagneticFlux, 0x2d)
CREATE_CLASS_DECLARATION(QKnxValueMagneticFluxDensity, 0x2e)
CREATE_CLASS_DECLARATION(QKnxValueMagneticMoment, 0x2f)
CREATE_CLASS_DECLARATION(QKnxValueMagneticPolarization, 0x30)
CREATE_CLASS_DECLARATION(QKnxValueMagnetization, 0x31)
CREATE_CLASS_DECLARATION(QKnxValueMagnetomotiveForce, 0x32)
CREATE_CLASS_DECLARATION(QKnxValueMass, 0x33)
CREATE_CLASS_DECLARATION(QKnxValueMassFlux, 0x34)
CREATE_CLASS_DECLARATION(QKnxValueMomentum, 0x35)
CREATE_CLASS_DECLARATION(QKnxValuePhaseAngleRad, 0x36)
CREATE_CLASS_DECLARATION(QKnxValuePhaseAngleDeg, 0x37)
CREATE_CLASS_DECLARATION(QKnxValuePower, 0x38)
CREATE_CLASS_DECLARATION(QKnxValuePowerFactor, 0x39)
CREATE_CLASS_DECLARATION(QKnxValuePressure, 0x3a)
CREATE_CLASS_DECLARATION(QKnxValueReactance, 0x3b)
CREATE_CLASS_DECLARATION(QKnxValueResistance, 0x3c)
CREATE_CLASS_DECLARATION(QKnxValueResistivity, 0x3d)
CREATE_CLASS_DECLARATION(QKnxValueSelfInductance, 0x3e)
CREATE_CLASS_DECLARATION(QKnxValueSolidAngle, 0x3f)
CREATE_CLASS_DECLARATION(QKnxValueSoundIntensity, 0x40)
CREATE_CLASS_DECLARATION(QKnxValueSpeed, 0x41)
CREATE_CLASS_DECLARATION(QKnxValueStress, 0x42)
CREATE_CLASS_DECLARATION(QKnxValueSurfaceTension, 0x43)
CREATE_CLASS_DECLARATION(QKnxValueCommonTemperature, 0x44)
CREATE_CLASS_DECLARATION(QKnxValueAbsoluteTemperature, 0x45)
CREATE_CLASS_DECLARATION(QKnxValueTemperatureDifference, 0x46)
CREATE_CLASS_DECLARATION(QKnxValueThermalCapacity, 0x47)
CREATE_CLASS_DECLARATION(QKnxValueThermalConductivity, 0x48)
CREATE_CLASS_DECLARATION(QKnxValueThermoelectricPower, 0x49)
CREATE_CLASS_DECLARATION(QKnxValueTime, 0x4a)
CREATE_CLASS_DECLARATION(QKnxValueTorque, 0x4b)
CREATE_CLASS_DECLARATION(QKnxValueVolume, 0x4c)
CREATE_CLASS_DECLARATION(QKnxValueVolumeFlux, 0x4d)
CREATE_CLASS_DECLARATION(QKnxValueWeight, 0x4e)
CREATE_CLASS_DECLARATION(QKnxValueWork, 0x4f)

#undef CREATE_CLASS_DECLARATION

QT_END_NAMESPACE

#endif
