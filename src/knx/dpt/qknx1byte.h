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

#ifndef QKNX1BYTE_H
#define QKNX1BYTE_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx1Byte : public QKnxDatapointType
{

public:
    QKnx1Byte();
    explicit QKnx1Byte(quint8 value);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x14;

    quint8 value() const;
    bool setValue(quint8 value);

    bool isValid() const override;

protected:
    explicit QKnx1Byte(int subType, quint8 value);
};

class Q_KNX_EXPORT QKnxScloMode : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Mode : quint8
    {
        Autonomous = 0x00,
        Slave = 0x01,
        Master = 0x02,
        Invalid = 0xff
    };
    Q_ENUM(Mode)

    QKnxScloMode();
    explicit QKnxScloMode(Mode mode);

    static const constexpr int SubType = 0x01;

    Mode mode() const;
    bool setMode(Mode mode);
};

class Q_KNX_EXPORT QKnxBuildingMode : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Mode : quint8
    {
        BuildingInUse = 0x00,
        BuildingNotUsed = 0x01,
        BuildingProtection = 0x02,
        Invalid = 0xff
    };
    Q_ENUM(Mode)

    QKnxBuildingMode();
    explicit QKnxBuildingMode(Mode mode);

    static const constexpr int SubType = 0x02;

    Mode mode() const;
    bool setMode(Mode mode);
};

class Q_KNX_EXPORT QKnxOccupyMode : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Mode : quint8
    {
        Occupied = 0x00,
        Standby = 0x01,
        NotOccupied = 0x02,
        Invalid = 0xff
    };
    Q_ENUM(Mode)

    QKnxOccupyMode();
    explicit QKnxOccupyMode(Mode mode);

    static const constexpr int SubType = 0x03;

    Mode mode() const;
    bool setMode(Mode mode);
};

class Q_KNX_EXPORT QKnxPriority : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Priority : quint8
    {
        High = 0x00,
        Medium = 0x01,
        Low = 0x02,
        Void = 0x03,
        Invalid = 0xff
    };
    Q_ENUM(Priority)

    QKnxPriority();
    explicit QKnxPriority(Priority priority);

    static const constexpr int SubType = 0x04;

    Priority priority() const;
    bool setPriority(Priority mode);
};

class Q_KNX_EXPORT QKnxLightApplicationMode : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Mode : quint8
    {
        Normal = 0x00,
        PresenceSimulation = 0x01,
        NightRound = 0x02,
        Invalid = 0xff
    };
    Q_ENUM(Mode)

    QKnxLightApplicationMode();
    explicit QKnxLightApplicationMode(Mode mode);

    static const constexpr int SubType = 0x05;

    Mode mode() const;
    bool setMode(Mode mode);
};

class Q_KNX_EXPORT QKnxApplicationArea : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Area : quint8
    {
        NoFault = 0x00,
        SystemFunctionCommonInterest = 0x01,
        HvacGeneralFBs = 0x0a,
        HvacHotWaterHeating = 0x0b,
        HvacDirectElectricalHeating = 0x0c,
        HvacTerminalUnits = 0x0d,
        HvacVac = 0x0e,
        Lighting = 0x14,
        Security = 0x1e,
        LoadManagement = 0x28,
        ShuttersAndBlinds = 0x32,
        Invalid = 0xff
    };
    Q_ENUM(Area)

    QKnxApplicationArea();
    explicit QKnxApplicationArea(Area area);

    static const constexpr int SubType = 0x06;

    Area area() const;
    bool setArea(Area area);

    bool isValid() const override;
};

class Q_KNX_EXPORT QKnxAlarmClassType : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Type : quint8
    {
        SimpleAlarm = 0x01,
        BasicAlarm = 0x02,
        ExtendedAlarm = 0x03,
        Invalid = 0xff
    };
    Q_ENUM(Type)

    QKnxAlarmClassType();
    explicit QKnxAlarmClassType(Type type);

    static const constexpr int SubType = 0x07;

    Type type() const;
    bool setType(Type type);
};

class Q_KNX_EXPORT QKnxPsuMode : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Mode : quint8
    {
        Disabled = 0x00,    // PSU/DSP fixed off
        Enabled = 0x01,     // PSU/DPSU fixed on
        Automatic = 0x02,   // PSU/DPSU automatic on/off
        Invalid = 0xff
    };
    Q_ENUM(Mode)

    QKnxPsuMode();
    explicit QKnxPsuMode(Mode mode);

    static const constexpr int SubType = 0x08;

    Mode mode() const;
    bool setMode(Mode mode);
};

class Q_KNX_EXPORT QKnxErrorClassSystem : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Error : quint8
    {
        NoFault = 0x00,
        GeneralDeviceFault = 0x01, // RAM, EEPROM, UI, watchdog...
        CommunicationFault = 0x02,
        ConfigurationFault = 0x03,
        HardwareFault = 0x04,
        SoftwareFault = 0x05,
        InsufficientNonVolatileMemory = 0x06,
        InsufficientVolatileMemory = 0x07,
        MemoryAllocationCommandWithSize0Received = 0x08,
        CrcError = 0x09,
        WatchdogResetDetected = 0x0a,
        InvalidOpCodeDetected = 0x0b,
        GeneralProtectionFault = 0x0c,
        MaximalTableLengthExceeded = 0x0d,
        UndefinedLoadCommandReceived = 0x0e,
        GroupAddressTableIsNotSorted = 0x0f,
        InvalidConnectionNumberTsap = 0x10,
        InvalidGroupObjectNumberAsap = 0x11,
        GroupObjectTypeExceeds = 0x12, // PID_MAX_APDU_LENGTH H - 2
        Invalid = 0xff
    };
    Q_ENUM(Error)

    QKnxErrorClassSystem();
    explicit QKnxErrorClassSystem(Error error);

    static const constexpr int SubType = 0x0b;

    Error error() const;
    bool setError(Error error);
};

class Q_KNX_EXPORT QKnxErrorClassHvac : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Error : quint8
    {
        NoFault = 0x00,
        SensorFault = 0x01, // RAM, EEPROM, UI, watchdog...
        ProcessControllerFault = 0x02,
        ActuatorFault = 0x03,
        OtherFault = 0x04,
        Invalid = 0xff
    };
    Q_ENUM(Error)

    QKnxErrorClassHvac();
    explicit QKnxErrorClassHvac(Error error);

    static const constexpr int SubType = 0x0c;

    Error error() const;
    bool setError(Error error);
};

class Q_KNX_EXPORT QKnxTimeDelay : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Delay : quint8
    {
        NotActive = 0x00,
        OneSecond = 0x01,
        TwoSecond = 0x02,
        ThreeSecond = 0x03,
        FiveSecond = 0x04,
        TenSecond = 0x05,
        FifteenSecond = 0x06,
        TwentySecond = 0x07,
        ThirtySecond = 0x08,
        FortyFiveSecond = 0x09,
        OneMinute = 0x0a,
        OneMinuteOneQuarter = 0x0b,
        OneMinuteAndHaft = 0x0c,
        TwoMinute = 0x0d,
        TwoMinuteAndHalf = 0x0e,
        ThreeMinute = 0x0f,
        FiveMinute = 0x10,
        FifteenMinute = 0x11,
        TwentyMinute = 0x12,
        ThirtyMinute = 0x13,
        OneHour = 0x14,
        TwoHour = 0x015,
        ThreeHour = 0x16,
        FiveHour = 0x17,
        TwelveHour = 0x18,
        TwentyFourHour = 0x19,
        Invalid = 0xff
    };
    Q_ENUM(Delay)

    QKnxTimeDelay();
    explicit QKnxTimeDelay(Delay delay);

    static const constexpr int SubType = 0x0d;

    Delay delay() const;
    bool setDelay(Delay delay);
};

class Q_KNX_EXPORT QKnxBeaufortWindForceScale : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Force : quint8
    {
        CalmNoWind = 0x00,
        LightAir = 0x01,
        LightBreeze = 0x02,
        GentleBreeze = 0x03,
        ModerateBreeze = 0x04,
        FreshBreeze = 0x05,
        StrongBreeze = 0x06,
        NearModerateGale = 0x07,
        FreshGale = 0x08,
        StrongGale = 0x09,
        WholeGaleStorm = 0x0a,
        ViolentStorm = 0x0b,
        Hurricane = 0x0c,
        Invalid = 0xff
    };
    Q_ENUM(Force)

    QKnxBeaufortWindForceScale();
    explicit QKnxBeaufortWindForceScale(Force force);

    static const constexpr int SubType = 0x0e;

    Force force() const;
    bool setForce(Force force);
};

class Q_KNX_EXPORT QKnxSensorSelect : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Mode : quint8
    {
        Inactive = 0x00,
        DigitalInputNotInverted = 0x01,
        DigitalInputInverted = 0x02,
        AnalogInput = 0x03,
        TemperatureSensorInput = 0x04,
        Invalid = 0xff
    };
    Q_ENUM(Mode)

    QKnxSensorSelect();
    explicit QKnxSensorSelect(Mode mode);

    static const constexpr int SubType = 0x11;

    Mode mode() const;
    bool setMode(Mode mode);
};

class Q_KNX_EXPORT QKnxActuatorConnectType : public QKnx1Byte
{
    Q_GADGET

public:
    enum class Type : quint8
    {
        SensorConnection = 0x01,
        ControllerConnection = 0x02,
        Invalid = 0xff
    };
    Q_ENUM(Type)

    QKnxActuatorConnectType();
    explicit QKnxActuatorConnectType(Type type);

    static const constexpr int SubType = 0x14;

    Type type() const;
    bool setType(Type type);
};

QT_END_NAMESPACE

#endif
