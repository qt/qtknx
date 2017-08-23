/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXDPT_H
#define QKNXDPT_H

#include <QtKnx/qknxglobal.h>
#include <QtCore/qstring.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxDpt final
{
    enum class Usage : quint8
    {
        // Random numbering
        U = 0x00, // unknown
        G = 0x02,
        FB = 0x02,
        HVAC = 0x03,
        System = 0x04
        //...
    };

    enum class Types : quint16
    {
        Unknown = 0,
        B1 = 1,
        B2 = 2,
        B1U3 = 3,
        A8 = 4, // TODO
        U8 = 5,
        V8 = 6,
        B5N3 = 6,
        U16 = 7,
        V16 = 8,
        F16 = 9,
        N3U5r2U6r2U6 = 10,
        r3U5r4U4r1U7 = 11,
        U32 =12,
        V32 = 13,
        F32 = 14,
        U4U4U4U4U4U4B4N4 = 15,
        r2U6 = 17,
        B1r1U6 = 18,
        r1B1U6 = 26,
        U8r4U4r3U5U3U5r2U6r2U6B16 = 19,
        N8 = 20,
        B8 = 21,
        N2 = 23
    };

    enum class TypesB1 : quint16
    {
        Unknown = 0,
        Switch = 1,
        Bool = 2,
        Enable = 3,
        Ramp = 4,
        Alarm = 5,
        BinaryValue = 6,
        Step = 7,
        UpDown = 8,
        OpenClose = 9,
        Start = 10,
        State = 11,
        Invert = 12,
        DimSendStyle = 13,
        InputSource = 14,
        Reset = 15,
        Ack = 16,
        Trigger = 17,
        Occupancy = 18,
        WindowDoor = 19,
        LogicalFunction = 21,
        SceneAB = 22,
        ShutterBlindsMode = 23
    };

    enum class TypesB2 : quint16
    {
        Unknown = 0,
        Switch = 1,
        Bool = 2,
        Enable = 3,
        Ramp = 4,
        Alarm = 5,
        BinaryValue = 6,
        Step = 7,
        Direction1 = 8,
        Direction2 = 9,
        Start = 10,
        State = 11,
        Invert = 12,
    };

    enum class TypesB1U3 : quint16
    {
        Unknown = 0,
        Dimming = 7,
        Blinds = 8
    };

    enum class TypesU8 : quint16
    {
        Unknown = 0,
        Scaling = 1,
        Angle = 3,
        Percent = 4,
        DecimalFactor = 5,
        Tariff = 6,
        Value = 10
    };

    enum class TypesV8 : quint16
    {
        Unknown = 0,
        Percent = 1,
        Value = 10
    };

    enum class TypesB5N3 : quint16
    {
        Unknown = 0,
        StatusMode3 = 20
    };

    enum class TypesU16 : quint16
    {
        Unknown = 0,
        Value = 1,
        PropertyDataType = 10,
        TimePeriodMilliSec = 2,
        TimePeriod10MilliSec = 3,
        TimePeriod100MilliSec = 4,
        TimePeriodSec = 5,
        TimePeriodMin = 6,
        TimePeriodHrs = 7,
        Length = 11,
        UEICurrent = 12,
        Brightness = 13
    };

    enum class TypesV16 : quint16
    {
        Unknown = 0,
        Value = 1,
        Percent = 10,
        DeltaTimeMilliSec = 2,
        DeltaTime10MilliSec = 3,
        DeltaTime100MilliSec = 4,
        DeltaTimeSec = 5,
        DeltaTimeMin = 6,
        DeltaTimeHrs = 7,
        RotationAngle = 11,
        UEICurrent = 12,
    };

    enum class TypesF16 : quint16
    {
        Unknown = 0,
        TemperatureCelcius = 1,
        TempertureKelvin = 2,
        TemperatureChangeKelvinPerHrs = 3,
        Brightness = 4,
        SpeedMeterPerSec = 5,
        Pression = 6,
        Humidity = 7,
        AirQuality = 8,
        TimeSec = 10,
        TimeMilliSec = 11,
        Voltage = 20,
        Current = 21,
        PowerDensity = 22,
        KelvinPerPercent = 23,
        Power = 24,
        VolumeFlow = 25,
        RainAmount = 26,
        TemperatureFahrenheit = 27,
        SpeedKiloMeterPerHrs = 28
    };

    enum class TypesN3U5r2U6r2U6 : quint16
    {
        Unknown = 0,
        TimeOfDay = 1
    };

    enum class Typesr3U5r4U4r1U7 : quint16
    {
        Unknown = 0,
        Date = 1
    };

    enum class TypesU32 : quint16
    {
        Unknown = 0,
        Value = 1
    };

    enum class TypesV32 : quint16
    {
        Unknown = 0,
        Value = 1,
        FlowRate = 2,
        ActiveEnergyWattHrs =10,
        ApparentEnergyVoltAmpHrs = 11,
        ReactiveEvnergyVARHrs = 12,
        ActiveEnergyKiloWattHrs = 13,
        ApparentEnergyKiloVoltAmpHrs = 14,
        ReactiveEvnergyKiloVARHrs = 15,
        DeltaTimeSec = 100
    };

    enum class TypesF32 : quint16
    {
        Acceleration = 0,
        AccelerationAngular = 1,
        ActivationEnergy = 2,
        Activity = 3,
        AmountMol = 4,
        Amplitude = 5,
        AngleRad = 6,
        AngleDeg = 7,
        AngularMomentum = 8,
        AngularVelocity = 9,
        Area = 10,
        Capacitance = 11,
        ChargeDensitySurface = 12,
        ChargeDensityVolume = 13,
        Compressibility = 14,
        Conductance = 15,
        ElectricalConductivity = 16,
        Density = 17,
        ElectricCharge = 18,
        ElectricCurrent = 19,
        ElectricCurrentDensity = 20,
        ElectricDipoleMoment = 21,
        ElectricDisplacement = 22,
        ElectricFieldStrengh = 23,
        ElectricFlux = 24,
        ElectricFluxDensity = 25,
        ElectricPolarization = 26,
        ElectricPotential = 27,
        ElectricPotentialDifference = 28,
        ElectromagneticMoment = 29,
        ElectromotiveForce = 30,
        Energy = 31,
        Force = 32,
        Frequency = 33,
        AngularFrequency = 34,
        HeatCapacity = 35,
        HeatFlowRate = 36,
        HeatQuantity = 37,
        Impedance = 38,
        Length = 39,
        LightQuantity = 40,
        Luminance = 41,
        LuminousFlux = 42,
        LuminousIntensity = 43,
        MagneticFieldStrength = 44,
        MagneticFlux = 45,
        MagneticFluxDensity = 46,
        MagneticMoment = 47,
        MagneticPolarization = 48,
        Magnetization = 49,
        MagnetomotiveForce = 50,
        Mass = 51,
        MassFlux = 52,
        Momentum = 53,
        PhaseAngleRad = 54,
        PhaseAngleDeg = 55,
        Power = 56,
        PowerFactor = 57,
        Pressure = 58,
        Reactance = 59,
        Resistance = 60,
        Resistivity = 61,
        SelfInductance = 62,
        SolidAngle = 63,
        SoundIntensity = 64,
        Speed = 65,
        Stress = 66,
        SurfaceTension = 67,
        CommonTemperature = 68,
        AbsoluteTemperature = 69,
        TemperatureDifference = 70,
        ThermalCapacity = 71,
        ThermalConductivity = 72,
        ThermoelectricPower = 73,
        Time = 74,
        Torque = 75,
        Volume = 76,
        VolumeFlux = 77,
        Weight = 78,
        Work =79
    };

    enum class TypesU4U4U4U4U4U4B4N4 : quint16
    {
        AccessData = 0
    };

    enum class Typesr2U6 : quint16
    {
        Unknown = 0,
        SceneNumber = 1
    };

    enum class TypesB1r1U6 : quint16
    {
        Unknown = 0,
        SceneControl = 1
    };

    enum class Typesr1B1U6 : quint16
    {
        Unknown = 0,
        SceneInfo = 1
    };

    enum class TypesU8r4U4r3U5U3U5r2U6r2U6B16 : quint16
    {
        Unknown = 0,
        DateTime = 1
    };

    enum class TypesN8 : quint16
    {
        Unknown = 0,
        ScloMode = 1,
        BuildingMode = 2,
        OccMode = 3,
        Priority = 4,
        LightApplicationMode = 5,
        ApplicationArea = 6,
        AlarmClassType = 7,
        PsuMode =8,
        ErrorClassSystem = 11,
        ErrorClassHvac = 12,
        TimeDelay = 13,
        BeaufortWindForceScale = 14,
        SensorSelect = 17,
        ActuatorConnectType = 20
    };

    enum class TypesB8 : quint16
    {
        Unknown = 0,
        GeneralStatus = 1,
        DeviceControl = 2,
    };

    enum class TypesN2 : quint16
    {
        Unknown = 0,
        OnOffAction = 1,
        AlarmReaction = 2,
        UpDownAction = 3
    };

    struct Info
    {
        Types mainType;
        quint16 subType;
        Usage usage;
        QString typeName;
    };

    static Info typeInfo(TypesB1 type)
    {
        Info info;
        info.mainType = Types::B1;
        info.subType = quint16(type);
        switch (type) {
        case TypesB1::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesB1::Switch:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Switch");
            break;
        case TypesB1::Bool:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Bool");
            break;
        case TypesB1::Enable:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Enable");
            break;
        case TypesB1::Ramp:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Ramp");
            break;
        case TypesB1::Alarm:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Alarm");
            break;
        case TypesB1::BinaryValue:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_BinaryValue");
            break;
        case TypesB1::Step:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Step");
            break;
        case TypesB1::UpDown:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_UpDown");
            break;
        case TypesB1::OpenClose:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_OpenClose");
            break;
        case TypesB1::Start:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Start");
            break;
        case TypesB1::State:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_State");
            break;
        case TypesB1::Invert:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Invert");
            break;
        case TypesB1::DimSendStyle:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_DimSendStyle");
            break;
        case TypesB1::InputSource:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_InputSource");
            break;
        case TypesB1::Reset:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Reset");
            break;
        case TypesB1::Ack:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Ack");
            break;
        case TypesB1::Trigger:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Trigger");
            break;
        case TypesB1::Occupancy:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Occupancy");
            break;
        case TypesB1::WindowDoor:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_WindowDoor");
            break;
        case TypesB1::LogicalFunction:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_LogicalFunction");
            break;
        case TypesB1::SceneAB:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Scene_AB");
            break;
        case TypesB1::ShutterBlindsMode:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_ShuttersBlinds_Mode");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesB2 type)
    {
        Info info;
        info.mainType = Types::B2;
        switch (type) {
        case TypesB2::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesB2::Switch:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Switch_Control");
            break;
        case TypesB2::Bool:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Bool_Control");
            break;
        case TypesB2::Enable:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Enable_Control");
            break;
        case TypesB2::Ramp:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Ramp_Control");
            break;
        case TypesB2::Alarm:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Alarm_Control");
            break;
        case TypesB2::BinaryValue:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_BinaryValue_Control");
            break;
        case TypesB2::Step:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Step_Control");
            break;
        case TypesB2::Direction1:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Direction1_Control");
            break;
        case TypesB2::Direction2:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Direction2_Control");
            break;
        case TypesB2::Start:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Start_Control");
            break;
        case TypesB2::State:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_State_Control");
            break;
        case TypesB2::Invert:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Invert_Control");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesB1U3 type)
    {
        Info info;
        info.mainType = Types::B1U3;
        switch (type) {
        case TypesB1U3::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesB1U3::Dimming:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Control_Dimming");
            break;
        case TypesB1U3::Blinds:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Control_Blinds");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesU8 type)
    {
        Info info;
        info.mainType = Types::U8;
        switch (type) {
        case TypesU8::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesU8::Scaling:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Scaling");
            break;
        case TypesU8::Angle:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Angle");
            break;
        case TypesU8::Percent:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Percent_U8");
            break;
        case TypesU8::DecimalFactor:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_DecimalFactor");
            break;
        case TypesU8::Value:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_1_Ucount");
            break;
        case TypesU8::Tariff:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Tariff");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesV8 type)
    {
        Info info;
        info.mainType = Types::V8;
        switch (type) {
        case TypesV8::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesV8::Percent:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Percent_V8");
            break;
        case TypesV8::Value:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_1_Count");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesB5N3 type)
    {
        Info info;
        info.mainType = Types::B5N3;
        switch (type) {
        case TypesB5N3::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesB5N3::StatusMode3:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Status_Mode3");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesU16 type)
    {
        Info info;
        info.mainType = Types::U16;
        switch (type) {
        case TypesU16::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesU16::Value:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_2_Ucount");
            break;
        case TypesU16::PropertyDataType:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_PropDataType");
            break;
        case TypesU16::TimePeriodMilliSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_TimePeriodMsec");
            break;
        case TypesU16::TimePeriod10MilliSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_TimePeriod10Msec");
            break;
        case TypesU16::TimePeriod100MilliSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_TimePeriod100Msec");
            break;
        case TypesU16::TimePeriodSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_TimePeriodSec");
            break;
        case TypesU16::TimePeriodMin:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_TimePeriodMin");
            break;
        case TypesU16::TimePeriodHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_TimePeriodHrs");
            break;
        case TypesU16::Length:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Length_mm");
            break;
        case TypesU16::UEICurrent:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_UEICurrentmA");
            break;
        case TypesU16::Brightness:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Brightness");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesV16 type)
    {
        Info info;
        info.mainType = Types::V16;
        switch (type) {
        case TypesV16::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesV16::Value:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_2_Count");
            break;
        case TypesV16::Percent:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Percent_V16");
            break;
        case TypesV16::DeltaTimeMilliSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_DeltaTimeMsec");
            break;
        case TypesV16::DeltaTime10MilliSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_DeltaTime10Msec");
            break;
        case TypesV16::DeltaTime100MilliSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_DeltaTime100Msec");
            break;
        case TypesV16::DeltaTimeSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_DeltaTimeSec");
            break;
        case TypesV16::DeltaTimeMin:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_DeltaTimeMin");
            break;
        case TypesV16::DeltaTimeHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_TimePeriodHrs");
            break;
        case TypesV16::RotationAngle:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Rotation_Angle");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesF16 type)
    {
        Info info;
        info.mainType = Types::F16;
        switch (type) {
        case TypesF16::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesF16::TemperatureCelcius:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Temp");
            break;
        case TypesF16::TempertureKelvin:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Temppd");
            break;
        case TypesF16::TemperatureChangeKelvinPerHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Tempa");
            break;
        case TypesF16::Brightness:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Lux");
            break;
        case TypesF16::SpeedMeterPerSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Wsp");
            break;
        case TypesF16::Pression:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Pres");
            break;
        case TypesF16::Humidity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Humidity");
            break;
        case TypesF16::AirQuality:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_AirQuality");
            break;
        case TypesF16::TimeSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Time1");
            break;
        case TypesF16::TimeMilliSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Time2");
            break;
        case TypesF16::Voltage:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Volt");
            break;
        case TypesF16::Current:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Curr");
            break;
        case TypesF16::PowerDensity:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_PowerDensity");
            break;
        case TypesF16::KelvinPerPercent:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_KelvinPerPercent");
            break;
        case TypesF16::Power:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Power");
            break;
        case TypesF16::VolumeFlow:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Value_Volume_Flow");
            break;
        case TypesF16::RainAmount:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Rain_Amount");
            break;
        case TypesF16::TemperatureFahrenheit:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Temp_F");
            break;
        case TypesF16::SpeedKiloMeterPerHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Wsp_kmh");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesN3U5r2U6r2U6 type)
    {
        Info info;
        info.mainType = Types::N3U5r2U6r2U6;
        switch (type) {
        case TypesN3U5r2U6r2U6::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesN3U5r2U6r2U6::TimeOfDay:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_TimeOfDay");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(Typesr3U5r4U4r1U7 type)
    {
        Info info;
        info.mainType = Types::r3U5r4U4r1U7;
        switch (type) {
        case Typesr3U5r4U4r1U7::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case Typesr3U5r4U4r1U7::Date:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Date");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesU32 type)
    {
        Info info;
        info.mainType = Types::U32;
        switch (type) {
        case TypesU32::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesU32::Value:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_4_Ucount");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesV32 type)
    {
        Info info;
        info.mainType = Types::V32;
        switch (type) {
        case TypesV32::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        case TypesV32::Value:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_4_Count");
            break;
        case TypesV32::FlowRate:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_FlowRate_m3Perh");
            break;
        case TypesV32::ActiveEnergyWattHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_ActiveEnergy");
            break;
        case TypesV32::ActiveEnergyKiloWattHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_ActiveEnergy_kWh");
            break;
        case TypesV32::ApparentEnergyVoltAmpHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_ApparentEnergy");
            break;
        case TypesV32::ApparentEnergyKiloVoltAmpHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_ApparentEnergy_kVAh");
            break;
        case TypesV32::ReactiveEvnergyVARHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_ReactiveEnergy");
            break;
        case TypesV32::ReactiveEvnergyKiloVARHrs:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_ReactiveEnergy_kVARh");
            break;
        case TypesV32::DeltaTimeSec:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_LongDeltaTimeSec");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesF32 type)
    {
        Info info;
        info.mainType = Types::F32;
        switch (type) {
        case TypesF32::Acceleration:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Acceleration");
            break;
        case TypesF32::AccelerationAngular:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Acceleration_Angular");
            break;
        case TypesF32::ActivationEnergy:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Activation_Energy");
            break;
        case TypesF32::Activity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Activity");
            break;
        case TypesF32::AmountMol:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Mol");
            break;
        case TypesF32::Amplitude:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Amplitude");
            break;
        case TypesF32::AngleRad:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_AngleRad");
            break;
        case TypesF32::AngleDeg:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_AngleDeg");
            break;
        case TypesF32::AngularMomentum:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Angular_Momentum");
            break;
        case TypesF32::AngularVelocity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Angular_Velocity");
            break;
        case TypesF32::Area:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Area");
            break;
        case TypesF32::Capacitance:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Capacitance");
            break;
        case TypesF32::ChargeDensitySurface:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Charge_DensitySurface");
            break;
        case TypesF32::ChargeDensityVolume:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Charge_DensityVolume");
            break;
        case TypesF32::Compressibility:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Compressibility");
            break;
        case TypesF32::Conductance:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Conductance");
            break;
        case TypesF32::ElectricalConductivity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electrical_Conductivity");
            break;
        case TypesF32::Density:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Density");
            break;
        case TypesF32::ElectricCharge:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_Charge");
            break;
        case TypesF32::ElectricCurrent:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_Current");
            break;
        case TypesF32::ElectricCurrentDensity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_CurrentDensity");
            break;
        case TypesF32::ElectricDipoleMoment:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_DipoleMoment");
            break;
        case TypesF32::ElectricDisplacement:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_Displacement");
            break;
        case TypesF32::ElectricFieldStrengh:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_FieldStrength");
            break;
        case TypesF32::ElectricFlux:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_Flux");
            break;
        case TypesF32::ElectricFluxDensity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_FluxDensity");
            break;
        case TypesF32::ElectricPolarization:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_Polarization");
            break;
        case TypesF32::ElectricPotential:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_Potential");
            break;
        case TypesF32::ElectricPotentialDifference:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electric_PotentialDifference");
            break;
        case TypesF32::ElectromagneticMoment:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_ElectromagneticMoment");
            break;
        case TypesF32::ElectromotiveForce:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Electromotive_Force");
            break;
        case TypesF32::Energy:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Energy");
            break;
        case TypesF32::Force:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Force");
            break;
        case TypesF32::Frequency:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Frequency");
            break;
        case TypesF32::AngularFrequency:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Angular_Frequency");
            break;
        case TypesF32::HeatCapacity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Heat_Capacity");
            break;
        case TypesF32::HeatFlowRate:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Heat_FlowRate");
            break;
        case TypesF32::HeatQuantity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Heat_Quantity");
            break;
        case TypesF32::Impedance:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Impedance");
            break;
        case TypesF32::Length:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Length");
            break;
        case TypesF32::LightQuantity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Light_Quantity");
            break;
        case TypesF32::Luminance:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Luminance");
            break;
        case TypesF32::LuminousFlux:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Luminous_Flux");
            break;
        case TypesF32::LuminousIntensity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Luminous_Intensity");
            break;
        case TypesF32::MagneticFieldStrength:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Magnetic_FieldStrength");
            break;
        case TypesF32::MagneticFlux:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Magnetic_Flux");
            break;
        case TypesF32::MagneticFluxDensity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Magnetic_FluxDensity");
            break;
        case TypesF32::MagneticMoment:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Magnetic_Moment");
            break;
        case TypesF32::MagneticPolarization:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Magnetic_Polarization");
            break;
        case TypesF32::Magnetization:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Magnetization");
            break;
        case TypesF32::MagnetomotiveForce:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_MagnetomotiveForce");
            break;
        case TypesF32::Mass:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Mass");
            break;
        case TypesF32::MassFlux:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_MassFlux");
            break;
        case TypesF32::Momentum:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Momentum");
            break;
        case TypesF32::PhaseAngleDeg:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Phase_AngleDeg");
            break;
        case TypesF32::PhaseAngleRad:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Phase_AngleRad");
            break;
        case TypesF32::Power:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Power");
            break;
        case TypesF32::PowerFactor:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Power_Factor");
            break;
        case TypesF32::Pressure:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Pressure");
            break;
        case TypesF32::Reactance:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Reactance");
            break;
        case TypesF32::Resistance:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Resitance");
            break;
        case TypesF32::Resistivity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Resitivity");
            break;
        case TypesF32::SelfInductance:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_SelfInductance");
            break;
        case TypesF32::SolidAngle:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_SolidAngle");
            break;
        case TypesF32::SoundIntensity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_SoundIntensity");
            break;
        case TypesF32::Speed:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Speed");
            break;
        case TypesF32::Stress:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Stress");
            break;
        case TypesF32::SurfaceTension:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Surface_Tension");
            break;
        case TypesF32::CommonTemperature:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Common_Temperature");
            break;
        case TypesF32::AbsoluteTemperature:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Absolute_Temperature");
            break;
        case TypesF32::TemperatureDifference:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_TemperatureDifference");
            break;
        case TypesF32::ThermalCapacity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Thermal_Capacity");
            break;
        case TypesF32::ThermalConductivity:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Thermal_Conductivity");
            break;
        case TypesF32::ThermoelectricPower:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_ThermoelectricPower");
            break;
        case TypesF32::Time:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Time");
            break;
        case TypesF32::Torque:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Thorque");
            break;
        case TypesF32::Volume:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Volume");
            break;
        case TypesF32::VolumeFlux:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Volume_Flux");
            break;
        case TypesF32::Weight:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Weight");
            break;
        case TypesF32::Work:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Value_Work");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesU4U4U4U4U4U4B4N4 type)
    {
        Info info;
        info.mainType = Types::U4U4U4U4U4U4B4N4;
        switch (type) {
        case TypesU4U4U4U4U4U4B4N4::AccessData:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Access_Data");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(Typesr2U6 type)
    {
        Info info;
        info.mainType = Types::r2U6;
        switch (type) {
        case Typesr2U6::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknow");
            break;
        case Typesr2U6::SceneNumber:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_SceneNumber");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(Typesr1B1U6 type)
    {
        Info info;
        info.mainType = Types::r1B1U6;
        switch (type) {
        case Typesr1B1U6::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknow");
            break;
        case Typesr1B1U6::SceneInfo:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_SceneInfo");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesB1r1U6 type)
    {
        Info info;
        info.mainType = Types::B1r1U6;
        switch (type) {
        case TypesB1r1U6::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknow");
            break;
        case TypesB1r1U6::SceneControl:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_SceneControl");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesU8r4U4r3U5U3U5r2U6r2U6B16 type)
    {
        Info info;
        info.mainType = Types::U8r4U4r3U5U3U5r2U6r2U6B16;
        switch (type) {
        case TypesU8r4U4r3U5U3U5r2U6r2U6B16::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknow");
            break;
        case TypesU8r4U4r3U5U3U5r2U6r2U6B16::DateTime:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_DateTime");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesN8 type)
    {
        Info info;
        info.mainType = Types::N8;
        switch (type) {
        case TypesN8::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknow");
            break;
        case TypesN8::ScloMode:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_SCLOMode");
            break;
        case TypesN8::BuildingMode:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_BuildingMode");
            break;
        case TypesN8::OccMode:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_OccMode");
            break;
        case TypesN8::Priority:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Priority");
            break;
        case TypesN8::LightApplicationMode:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_LightApplicationMode");
            break;
        case TypesN8::ApplicationArea:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_ApplicationArea");
            break;
        case TypesN8::AlarmClassType:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_AlarmClassType");
            break;
        case TypesN8::PsuMode:
            info.usage = Usage::System;
            info.typeName = QString::fromLatin1("DPT_PSUMode");
            break;
        case TypesN8::ErrorClassSystem:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_ErrorClass_System");
            break;
        case TypesN8::ErrorClassHvac:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_ErrorClass_HVAC");
            break;
        case TypesN8::TimeDelay:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Time_Delay");
            break;
        case TypesN8::BeaufortWindForceScale:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_Beaufort_Wind_Force_Scale");
            break;
        case TypesN8::SensorSelect:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_SensorSelect");
            break;
        case TypesN8::ActuatorConnectType:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_ActuatorConnectType");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesB8 type)
    {
        Info info;
        info.mainType = Types::B8;
        switch (type) {
        case TypesB8::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknow");
            break;
        case TypesB8::GeneralStatus:
            info.usage = Usage::G;
            info.typeName = QString::fromLatin1("DPT_StatusGen");
            break;
        case TypesB8::DeviceControl:
            info.usage = Usage::System;
            info.typeName = QString::fromLatin1("DPT_Device_Control");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }

    static Info typeInfo(TypesN2 type)
    {
        Info info;
        info.mainType = Types::N2;
        switch (type) {
        case TypesN2::Unknown:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknow");
            break;
        case TypesN2::OnOffAction:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_OnOffAction");
            break;
        case TypesN2::AlarmReaction:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_Alarm_Reaction");
            break;
        case TypesN2::UpDownAction:
            info.usage = Usage::FB;
            info.typeName = QString::fromLatin1("DPT_UpDown_Action");
            break;
        default:
            info.usage = Usage::U;
            info.typeName = QString::fromLatin1("DPT_Unknown");
            break;
        }
        return info;
    }
};

QT_END_NAMESPACE

#endif
