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

#ifndef QKNXDPTFACTORY_H
#define QKNXDPTFACTORY_H

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxdatapoint.h>
#include <QtKnx/qknxdpt.h>
#include <QtCore/qmath.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxDptFactory final
{
public:
    QKnxDptFactory() = delete;
    ~QKnxDptFactory() = delete;

    class Q_KNX_EXPORT Encoding final
    {
    public:
        static quint32 fromSignedIntToTwoComplementNotation(qint32 integer)
        {
            if (integer < 0)
                return uint(~(-integer) + 1);
            return uint(integer);
        }

        template <typename T> static int fromTwoComplementNotationToSignedInt(T value)
        {
            static_assert(std::is_unsigned<T>::value, "Only unsigned types supported.");

            if (value > (qPow(2, sizeof(value) * 8) / 2 - 1)) {
                std::bitset<sizeof(value) * 8> bits = value;
                bits = bits.to_ulong() - 1;
                return -(int(bits.flip().to_ulong()));
            }
            return int(value);
        }

        static quint32 fromFloatToIEEE754(float value)
        {
            // 32 Bits IEEE754 float
            // |S|E|E|E|E|E|E|E| |E|F|F|F|F|F|F|F| |F|F|F|F|F|F|F|F| |F|F|F|F|F|F|F|F|
            // S = {0,1}
            // E [0, 254]
            // F [0, 8388607]
            // float x
            // x = (-1)^S (1+a)*2^(p)
            // a = [0, 1]      F = 0000...0000 -> a = 0
            //                 F = 1111...1111 -> a = 1
            // p = [-126, 127] E = p + 127

            // ln(|x|)/ln2 = ln(1+a)/ln2 + p
            // 0< ln(1+a)/ln2 <= 1
            // => p is the integer part of ln(|x|)/ln2
            quint32 p = quint32(qFloor(qLn(qFabs(qreal(value))) / qLn(2.)));
            Q_ASSERT_X(p <= 128, "Encoding::fromFloatToIEEE754", "PROBLEM with p");
            if (p > 128)
                return {};

            float a = float(qFabs(qreal(value)) * qPow(2, -qreal(p)) - 1);
            Q_ASSERT_X(a <= 1, "Encoding::fromFloatToIEEE754", "PROBLEM with a");
            if (a > 1)
                return {};

            quint32 F = quint32(qRound(qreal(a * 8388607)));
            quint32 E = p + 127;

            if (value < 0)
                F |= (quint32(1) << 31);
            F |= (E << 23);
            return F;
        }
        // Case: frame reception. Use QUtils::QUint32::fromBytes to extract the quint32
        // Use this function to get the float value from the integer.
        static float fromIEEE754ToFloat(quint32 integer)
        {
            quint32 S = ((integer & 0x80000000) >> 31);
            quint32 E = ((integer & 0x7f800000) >> 23);
            quint32 F = (integer & 0x7fffff);
            float value = float(qPow(-1, S)* (1 + F / 8388607.) * qPow(2, E - 127));
            // other to do this:
            //float* res = reinterpret_cast<float*>(&integer);
            //float *floatFromInt = res;
            //return *floatFromInt
            return value;
        }
    };

    // Just for testing
    static QKnxDataPoint setBits(bool value, QKnxDpt::TypesB1 type = QKnxDpt::TypesB1::Unknown);

    class Q_KNX_EXPORT SimpleBinary
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            bool value = false;
        };

        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setBits(bool value, QKnxDpt::TypesB1 type = QKnxDpt::TypesB1::Unknown);
    };

    struct Q_KNX_EXPORT Switch : public SimpleBinary
    {
        static QKnxDataPoint Off;
        static QKnxDataPoint On;
    };

    struct Q_KNX_EXPORT Bool : public SimpleBinary
    {
        static QKnxDataPoint False;
        static QKnxDataPoint True;
    };

    struct Q_KNX_EXPORT Enable : public SimpleBinary
    {
        static QKnxDataPoint No;
        static QKnxDataPoint Yes;
    };

    struct Q_KNX_EXPORT Ramp : public SimpleBinary
    {
        static QKnxDataPoint No;
        static QKnxDataPoint Yes;
    };

    struct Q_KNX_EXPORT Alarm : public SimpleBinary
    {
        static QKnxDataPoint No;
        static QKnxDataPoint Yes;
    };

    struct Q_KNX_EXPORT BinaryValue : public SimpleBinary
    {
        static QKnxDataPoint Low;
        static QKnxDataPoint High;
    };

    struct Q_KNX_EXPORT Step : public SimpleBinary
    {
        static QKnxDataPoint Decrease;
        static QKnxDataPoint Increase;
    };

    struct Q_KNX_EXPORT Motion : public SimpleBinary
    {
        static QKnxDataPoint Up;
        static QKnxDataPoint Down;
    };

    struct Q_KNX_EXPORT Action : public SimpleBinary
    {
        static QKnxDataPoint Open;
        static QKnxDataPoint Close;
        static QKnxDataPoint Stop;
        static QKnxDataPoint Start;
    };

    struct Q_KNX_EXPORT State : public SimpleBinary
    {
        static QKnxDataPoint Inactive;
        static QKnxDataPoint Active;
    };

    struct Q_KNX_EXPORT Inverted : public SimpleBinary
    {
        static QKnxDataPoint No;
        static QKnxDataPoint Yes;
    };

    struct Q_KNX_EXPORT DimingStyle : public SimpleBinary
    {
        static QKnxDataPoint StartStop;
        static QKnxDataPoint Cyclical;
    };

    struct Q_KNX_EXPORT InputSource : public SimpleBinary
    {
        static QKnxDataPoint Fixed;
        static QKnxDataPoint Calculated;
    };

    struct Q_KNX_EXPORT Reset : public SimpleBinary
    {
        static QKnxDataPoint No;
        static QKnxDataPoint Yes;
    };

    struct Q_KNX_EXPORT Acknowledge : public SimpleBinary
    {
        static QKnxDataPoint No;
        static QKnxDataPoint YesTrigger;
    };

    struct Q_KNX_EXPORT Trigger : public SimpleBinary
    {
        static QKnxDataPoint Firering;
        static QKnxDataPoint FireringAlso;
    };

    struct Q_KNX_EXPORT Occupied : public SimpleBinary
    {
        static QKnxDataPoint No;
        static QKnxDataPoint Yes;
    };

    struct Q_KNX_EXPORT WindowDoor : public SimpleBinary
    {
        static QKnxDataPoint Closed;
        static QKnxDataPoint Open;
    };

    struct Q_KNX_EXPORT LogicalFunction : public SimpleBinary
    {
        static QKnxDataPoint Or;
        static QKnxDataPoint And;
    };

    struct Q_KNX_EXPORT SelectScene : public SimpleBinary
    {
        static QKnxDataPoint A;
        static QKnxDataPoint B;
    };

    struct Q_KNX_EXPORT ShutterBlindsMode : public SimpleBinary
    {
        static QKnxDataPoint Shutter; // only move Up/Down
        static QKnxDataPoint Blind; // Move Up/Down + StepStop mode
    };

    class Q_KNX_EXPORT Control
    {
    public:

        enum Status
        {
            On, Off, Unknown
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Status control = Unknown;
            bool value = false;
        };

        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setBits(bool value, Status control,
            QKnxDpt::TypesB2 type = QKnxDpt::TypesB2::Unknown);
    };

    struct Q_KNX_EXPORT SwitchOn : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT SwitchOff : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT BoolFalse : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT BoolTrue : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT EnableNo : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT EnableYes : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT RampNo : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT RampYes : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT AlarmNo : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT AlarmYes : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT BinaryValueLow : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT BinaryValueHigh : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT StepDecrease : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT StepIncrease : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT MotionUp : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT MotionDown : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT ActionOpen : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT ActionClose : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT ActionStop : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT ActionStart : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT StateInactive : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT StateActive : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT InvertedNo : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    struct Q_KNX_EXPORT InvertedYes : public Control
    {
        static QKnxDataPoint ControlOff;
        static QKnxDataPoint ControlOn;
    };

    class Q_KNX_EXPORT TypeB1U3
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            bool value = false; // Diming false -> decrease / Blinds false -> Up
            quint8 numberOfInterval = 65; // 0 means break
            quint8 stepCode = 8;
        };

        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setBits(bool highBit, quint8 numberInterval,
            QKnxDpt::TypesB1U3 type = QKnxDpt::TypesB1U3::Unknown);
        static QKnxDataPoint setBits(Content content,
            QKnxDpt::TypesB1U3 type = QKnxDpt::TypesB1U3::Unknown);
    };

    class Q_KNX_EXPORT ControlDimming : public TypeB1U3
    {
    public:
        static QKnxDataPoint increaseWithNumberOfInternal(quint8 numberInterval);
        static QKnxDataPoint decreaseWithNumberOfInternal(quint8 numberInterval);
        static QKnxDataPoint BreakIncrease;
        static QKnxDataPoint BreakDecrease;
    };

    //This DPT can be used both for the relative positioning of the vertical blinds positions
    //as well as for the relative positioning of the angle of the slats
    class Q_KNX_EXPORT ControlBlinds : public TypeB1U3
    {
    public:
        static QKnxDataPoint moveUpWithNumberOfInternal(quint8 numberInterval);
        static QKnxDataPoint moveDownWithNumberOfInternal(quint8 numberInterval);
        static QKnxDataPoint BreakMoveUp;
        static QKnxDataPoint BreakMoveDown;
    };

    // TODO: A8

    class Q_KNX_EXPORT TypeU8
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            QVariant value = 0;
        };

        static bool validate(const QByteArray &bytes, QKnxDpt::TypesU8 type = QKnxDpt::TypesU8::Unknown);

    protected:
        static Content getContent(const QByteArray &bytes, QKnxDpt::TypesU8 type);
        static QKnxDataPoint setBits(quint8 value, QKnxDpt::TypesU8 type);
    };

    class Q_KNX_EXPORT Scaling : public TypeU8
    {
    public:
        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setPercent(double degree);
    };

    class Q_KNX_EXPORT Angle : public TypeU8
    {
    public:
        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setDegree(double degree);
    };

    class Q_KNX_EXPORT PercentUnsigned : public TypeU8
    {
    public:
        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setPercent(quint8 percent);
    };

    class Q_KNX_EXPORT DecimalFactor : public TypeU8
    {
    public:
        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setRatio(double ratio);
    };

    class Q_KNX_EXPORT Uint8 : public TypeU8
    {
    public:
        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(quint8 value);
    };

    class Q_KNX_EXPORT Tariff : public TypeU8
    {
    public:
        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(quint8 tariff);
    };

    class Q_KNX_EXPORT TypeV8
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            qint8 value = 0;
        };

        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setBits(qint8 value, QKnxDpt::TypesV8 type);
    };

    class Q_KNX_EXPORT PercentSigned : public TypeV8
    {
    public:
        static QKnxDataPoint setPercent(qint8 percent);
    };

    class Q_KNX_EXPORT Int8 : public TypeV8
    {
    public:
        static QKnxDataPoint setValue(qint8 value);
    };

    class Q_KNX_EXPORT StatusMode3
    {
    public:
        enum class ActiveMode
        {
            M0, M1, M2, Unknown
        };

        enum class Status
        {
            Set, Clear, Unknown
        };

        struct Q_KNX_EXPORT Content
        {
            Status A = Status::Unknown;
            Status B = Status::Unknown;
            Status C = Status::Unknown;
            Status D = Status::Unknown;
            Status E = Status::Unknown;
            ActiveMode mode = ActiveMode::Unknown;
        };

        static bool validate(const QByteArray &bytes);
        static bool validate(Content content);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setBits(Content content);
    };

    class Q_KNX_EXPORT TypeU16
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            QVariant value = 0;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static QKnxDataPoint setBits(quint16 value, QKnxDpt::TypesU16 type);
    };

    class Q_KNX_EXPORT Uint16 : public TypeU16
    {
    public:
        static QKnxDataPoint setValue(quint16 pulse);
    };

    class Q_KNX_EXPORT PropertyDataType : public TypeU16
    {
    public:
        static QKnxDataPoint setValue(quint16 IdInterfaceObjectPropertyDataType);
    };

    class Q_KNX_EXPORT TimePeriodMilliSec : public TypeU16
    {
    public:
        static QKnxDataPoint setValue(quint16 periodMs);
    };

    class Q_KNX_EXPORT TimePeriod10MilliSec : public TypeU16
    {
    public:
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(quint32 period10Ms);
    };

    class Q_KNX_EXPORT TimePeriod100MilliSec : public TypeU16
    {
    public:
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(quint32 periodMs);
    };

    class Q_KNX_EXPORT TimePeriodSec : public TypeU16
    {
    public:
        static QKnxDataPoint setValue(quint16 periodSec);
    };

    class Q_KNX_EXPORT TimePeriodMin : public TypeU16
    {
    public:
        static QKnxDataPoint setValue(quint16 periodMin);
    };

    class Q_KNX_EXPORT TimePeriodHrs : public TypeU16
    {
    public:
        static QKnxDataPoint setValue(quint16 periodHrs);
    };

    class Q_KNX_EXPORT Length : public TypeU16
    {
    public:
        static QKnxDataPoint setValue(quint16 milliMeter);
    };

    class Q_KNX_EXPORT UEICurrent : public TypeU16
    {
    public:
        static QKnxDataPoint setValue(quint16 milliA);
    };

    class Q_KNX_EXPORT Brightness : public TypeU16
    {
    public:
        static QKnxDataPoint setValue(quint16 lux);
    };

    class Q_KNX_EXPORT TypeV16
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            QVariant value = 0;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static QKnxDataPoint setBits(qint16 value, QKnxDpt::TypesV16 type);
    };

    class Q_KNX_EXPORT Int16 : public TypeV16
    {
    public:
        static QKnxDataPoint setValue(qint16 pulse);
    };

    class Q_KNX_EXPORT PercentSigned16 : public TypeV16
    {
    public:
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(double percent);
    };

    class Q_KNX_EXPORT DeltaTimeMilliSec : public TypeV16
    {
    public:
        static QKnxDataPoint setValue(qint16 periodMs);
    };

    class Q_KNX_EXPORT DeltaTime10MilliSec : public TypeV16
    {
    public:
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(qint32 period10Ms);
    };

    class Q_KNX_EXPORT DeltaTime100MilliSec : public TypeV16
    {
    public:
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(qint32 period100Ms);
    };

    class Q_KNX_EXPORT DeltaTimeSec : public TypeV16
    {
    public:
        static QKnxDataPoint setValue(qint16 period);
    };

    class Q_KNX_EXPORT DeltaTimeMin : public TypeV16
    {
    public:
        static QKnxDataPoint setValue(qint16 period);
    };

    class Q_KNX_EXPORT DeltaTimeHrs : public TypeV16
    {
    public:
        static QKnxDataPoint setValue(qint16 period);
    };

    class Q_KNX_EXPORT RotationAngle : public TypeV16
    {
    public:
        static QKnxDataPoint setValue(qint16 period);
    };

    class Q_KNX_EXPORT TypeF16
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            float value = 0;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static bool validate(Content content);
        static QKnxDataPoint setBits(float value, QKnxDpt::TypesF16 type);
    };

    class Q_KNX_EXPORT TemperatureCelcius : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float degree);
    };

    class Q_KNX_EXPORT TemperatureKelvin : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float degree);
    };

    class Q_KNX_EXPORT TemperatureChange : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float kelvinPerHrs);
    };

    class Q_KNX_EXPORT BrightnessFloat : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float lux);
    };

    class Q_KNX_EXPORT SpeedMeterPerSec : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float value);
    };

    class Q_KNX_EXPORT Pression : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float pascal);
    };

    class Q_KNX_EXPORT Humidity : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float percent);
    };

    class Q_KNX_EXPORT AirQuality : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float ppm);
    };

    class Q_KNX_EXPORT TimeSec : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float sec);
    };

    class Q_KNX_EXPORT TimeMilliSec : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float milliSec);
    };

    class Q_KNX_EXPORT Voltage : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float volt);
    };

    class Q_KNX_EXPORT Current : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float milliA);
    };

    class Q_KNX_EXPORT PowerDensity : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float wattPerSquareMeter);
    };

    class Q_KNX_EXPORT KelvinPerPercent : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float value);
    };

    class Q_KNX_EXPORT Power : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float kiloWatt);
    };

    class Q_KNX_EXPORT VolumeFlow : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float literPerHour);
    };

    class Q_KNX_EXPORT RainAmount : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float literPerSquareMeter);
    };

    class Q_KNX_EXPORT TemperatureFahrenheit : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float value);
    };

    class Q_KNX_EXPORT SpeedKiloMeterPerHrs : public TypeF16
    {
    public:
        static QKnxDataPoint setValue(float value);
    };

    class Q_KNX_EXPORT TimeOfDay
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            quint8 dayOfTheWeek = 255;
            quint8 hour = 255;
            quint8 min = 255;
            quint8 sec = 255;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static bool validate(Content content);
        static QKnxDataPoint setTimeOfDay(Content content);
    };

    class Q_KNX_EXPORT Date
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            quint8 day = 255;
            quint8 month = 255;
            quint16 year = 3000;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static bool validate(Content content);
        static QKnxDataPoint setDate(Content content);
    };

    class Q_KNX_EXPORT Uint32
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            quint32 value = 0;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static QKnxDataPoint setValue(quint32 value);
    };

    class Q_KNX_EXPORT TypeV32
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            qint32 value = 0;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static QKnxDataPoint setBits(qint32 value, QKnxDpt::TypesV32 type);
    };

    class Q_KNX_EXPORT Int32 : public TypeV32
    {
    public:
        static QKnxDataPoint setValue(qint32 value);
    };

    class Q_KNX_EXPORT FlowRate : public TypeV32
    {
    public:
        static QKnxDataPoint setValue(qint32 cubicMeterPerHrs);
    };

    class Q_KNX_EXPORT ActiveEnergyWattHrs : public TypeV32
    {
    public:
        static QKnxDataPoint setValue(qint32 value);
    };

    class Q_KNX_EXPORT ApparentEnergyVoltAmpHrs : public TypeV32
    {
    public:
        static QKnxDataPoint setValue(qint32 value);
    };

    class Q_KNX_EXPORT ReactiveEnergyVARHrs : public TypeV32
    {
    public:
        static QKnxDataPoint setValue(qint32 value);
    };

    class Q_KNX_EXPORT ActiveEnergyKiloWattHrs : public TypeV32
    {
    public:
        static QKnxDataPoint setValue(qint32 value);
    };

    class Q_KNX_EXPORT ApparentEnergyKiloVoltAmpHrs : public TypeV32
    {
    public:
        static QKnxDataPoint setValue(qint32 value);
    };

    class Q_KNX_EXPORT ReactiveEnergyKiloVARHrs : public TypeV32
    {
    public:
        static QKnxDataPoint setValue(qint32 value);
    };

    class Q_KNX_EXPORT DeltaTime : public TypeV32
    {
    public:
        static QKnxDataPoint setValue(qint32 sec);
    };

    class Q_KNX_EXPORT TypeF32
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            float value = 0;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static QKnxDataPoint setBits(float value, QKnxDpt::TypesF32 type);
    };

    // TODO : 79 class inheriting from TypeF32
    // to write just to hide the TypeF32 class and set the type automatically...
    /*
    class Q_KNX_EXPORT Acceleration: public TypeF32
    {
    public:
        static QKnxDataPoint setValue(float mPersec2);
    };
    */

    class Q_KNX_EXPORT AccessData
    {
    public:
        enum class ErrorStatus
        {
            NoError, ReadingUnsuccessful, Unknown
        };

        enum class PermissionStatus
        {
            NotAccepted, Accepted, Unknown
        };

        enum class ReadDirection
        {
            LeftToRight, RightToLeft, Unknown
        };

        enum class Encryption
        {
            No, Yes, Unknown
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            ErrorStatus error = ErrorStatus::Unknown;
            PermissionStatus permission = PermissionStatus::Unknown;
            ReadDirection reading = ReadDirection::Unknown;
            Encryption encryption = Encryption::Unknown;
            quint8 d1 = 99;
            quint8 d2 = 99;
            quint8 d3 = 99;
            quint8 d4 = 99;
            quint8 d5 = 99;
            quint8 d6 = 99;
            quint8 index = 99;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static bool validate(Content content);
        static QKnxDataPoint setValue(Content content);
    };

    class Q_KNX_EXPORT Scene
    {
    public:
        enum class Control
        {
            ActivateScene, LearnScene, NotApplicable, Unknown
        };

        enum class Info
        {
            ActiveScene, InactiveScene, NotApplicable, Unknown
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Control control = Control::Unknown;
            Info info = Info::Unknown;
            quint8 sceneNumber = 99;
        };

        static bool validate(const QByteArray &bytes);

    protected:
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setBits(quint8 sceneNumber, Control control, Info info,
            QKnxDpt::Types dptMainType, quint16 dptSubNumber, const QString &dptName,
            QKnxDpt::Usage usage);
    };

    class Q_KNX_EXPORT SceneNumber : public Scene
    {
    public:
        static bool validate(const QByteArray &bytes);
        static bool validate(Content content);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(quint8 sceneNumber);
    };

    class Q_KNX_EXPORT SceneControl : public Scene
    {
    public:
        static bool validate(const QByteArray &bytes);
        static bool validate(Content content);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint activate(quint8 sceneNumber);
        static QKnxDataPoint learn(quint8 sceneNumber);
    };

    class Q_KNX_EXPORT SceneInfo : public Scene
    {
    public:
        static bool validate(const QByteArray &bytes);
        static bool validate(Content content);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint activeScene(quint8 sceneNumber);
        static QKnxDataPoint inactiveScene(quint8 sceneNumber);
    };

    class Q_KNX_EXPORT DateTime
    {
    public:
        enum class Fault
        {
            No, Yes, Unknown
        };

        enum class WorkingDay
        {
            No, Yes, Unknown
        };

        enum class WorkingDayField
        {
            Valid, NotValid, Unknown
        };

        enum class YearField
        {
            Valid, NotValid, Unknown
        };

        enum class DateField
        {
            Valid, NotValid, Unknown
        };

        enum class DayOfWeekField
        {
            Valid, NotValid, Unknown
        };

        enum class TimeField
        {
            Valid, NotValid, Unknown
        };

        enum class SummerTime
        {
            No, Yes, Unknown
        };

        enum class ClockQuality
        {
            WithoutExtendedSyncSignal, WithExtendedSyncSignal, Unknown
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            quint16 year = 3000;
            quint8 month = 255;
            quint8 day = 255;
            quint8 dayOfWeek = 99;
            quint8 hourOfDay = 30;
            quint8 minutes = 99;
            quint8 seconds = 99;
            Fault fault = Fault::Unknown;
            WorkingDay workingDay = WorkingDay::Unknown;
            WorkingDayField workingDayField = WorkingDayField::Unknown;
            YearField yearField = YearField::Unknown;
            DateField dateField = DateField::Unknown;
            DayOfWeekField dayOfWeekField = DayOfWeekField::Unknown;
            TimeField timeField = TimeField::Unknown;
            SummerTime summerTime = SummerTime::Unknown;
            ClockQuality clockQuality = ClockQuality::Unknown;
        };

        static Content getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static bool validate(Content content);
        static QKnxDataPoint setDateTime(Content content);
    };

    class Q_KNX_EXPORT TypeN8
    {
    public:
        struct Q_KNX_EXPORT BasicContent
        {
            bool contentIsValid = false;
            quint8 value = 255;
        };

        static BasicContent getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static QKnxDataPoint setBits(quint8 value, QKnxDpt::TypesN8 type);
    };

    class Q_KNX_EXPORT ScloMode : public TypeN8
    {
    public:
        enum class Value
        {
            Autonomous, Slave, Master
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::Autonomous;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT BuildingMode : private TypeN8
    {
    public:
        enum class Value
        {
            InUse, NotUsed, Protection
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::InUse;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT OccMode : private TypeN8
    {
    public:
        enum class Value
        {
            Occupied, Standby, NotOccupied
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::Occupied;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT Priority : private TypeN8
    {
    public:
        enum class Value
        {
            High, Medium, Low, Void
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::High;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT LightApplicationMode : private TypeN8
    {
    public:
        enum class Value
        {
            Normal, PresenceSimulation, NightRound, ManufacturerValue
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::Normal;
            quint8 manufacturerValue = 0;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
        static QKnxDataPoint setManufacturerValue(quint8 value);
    };

    class Q_KNX_EXPORT ApplicationArea : private TypeN8
    {
    public:
        enum class Value : quint8
        {
            NoFault = 0x00,
            SysAndFuncOfComInterest = 0x01,
            HvacGeneralFB = 0x0a,
            HvacHotWaterHeating = 0x0b,
            HvacDirectElectricalHeating = 0x0c,
            HvacTerminalUnits = 0x0d,
            HvacVac = 0x0e,
            HvacReserved = 0x0f,
            Lighting = 0x14,
            LightingReserved = 0x15,
            Security = 0x1e,
            SecurityReserved = 0x1f,
            LoadManagement = 0x28,
            LoadManagementReserved = 0x29,
            ShuttersAndBlinds = 0x32,
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::NoFault;
            quint8 reservedValue = 0;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
        static QKnxDataPoint setReservedValue(quint8 value, Value valueRange);
    };

    class Q_KNX_EXPORT AlarmClassType : private TypeN8
    {
    public:
        enum class Value : quint8
        {
            Simple = 0x01,
            Basic = 0x02,
            Extended = 0x03
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::Simple;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT PsuMode : private TypeN8
    {
    public:
        enum class Value
        {
            Disabled, Enabled, Auto
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::Disabled;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT ErrorClassSystem : private TypeN8
    {
    public:
        enum class Value : quint8
        {
            NoFault = 0x00,
            GeneralDeviceFault = 0x01,
            CommunicationFault = 0x02,
            ConfigurationFault = 0x03,
            HardwareFault = 0x04,
            SoftwareFault = 0x05,
            InsufficientNonVolMem = 0x06,
            InsufficientVolMem = 0x07,
            MemAllocSizeZeroReceived = 0x08,
            CrcError = 0x09,
            WatchdogResetDetected = 0x0a,
            InvalidOpcodeDetected = 0x0b,
            GeneralProtectionFault = 0x0c,
            MaxTableLengthExceeded = 0x0d,
            UndefinedLoadCommandReceived = 0x0e,
            GroupAddressTableNotSorted = 0x0f,
            InvalidConnectionNumber = 0x10,
            InvalidGroupObjectNumber = 0x11,
            GroupObjectTypeExceeded = 0x12
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::NoFault;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT ErrorClassHvac : private TypeN8
    {
    public:
        enum class Value : quint8
        {
            NoFault = 0x00,
            SensorFault = 0x01,
            ProcessOrControllerFault = 0x02,
            ActuatorFault = 0x03,
            OtherFault = 0x04
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::NoFault;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT TimeDelay : private TypeN8
    {
    public:
        enum class Value : quint8
        {
            NotActive = 0x00,
            OneSec = 0x01,
            TwoSec = 0x02,
            ThreeSec = 0x03,
            FiveSec = 0x04,
            TenSec = 0x05,
            FifteenSec = 0x06,
            TwentySec = 0x07,
            ThirtySec = 0x08,
            FortyFiveSec = 0x09,
            OneMin = 0x0a,
            OneAndQuarterMin = 0x0b,
            OneAndHalfMin = 0x0c,
            TwoMin = 0x0d,
            TwoAndHalfMin = 0x0e,
            ThreeMin = 0x0f,
            FiveMin = 0x10,
            FifteenMin = 0x11,
            TwentyMin = 0x12,
            ThirtyMin = 0x13,
            OneHrs = 0x14,
            TwoHrs = 0x15,
            ThreeHrs = 0x16,
            FiveHrs = 0x17,
            TwelveHrs = 0x18,
            TwentyFourHrs = 0x19
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::NotActive;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT BeaufortWindForceScale : private TypeN8
    {
    public:
        enum class Value : quint8
        {
            Calm = 0x00,
            LightAir = 0x01,
            LightBreeze = 0x02,
            GentleBreeze = 0x03,
            ModerateBreeze = 0x04,
            FreshBreeze = 0x05,
            StrongBreeze = 0x06,
            NearToModerateGale = 0x07,
            FreshGale = 0x08,
            StrongGale = 0x09,
            WholeGaleToStorm = 0x0a,
            ViolentStorm = 0x0b,
            Hurricane = 0x0c
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::Calm;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT SensorSelect : private TypeN8
    {
    public:
        enum class Value : quint8
        {
            Inactive = 0x00,
            DigitalInputNotInverted = 0x01,
            DigitalInputInverted = 0x02,
            AnalogInput = 0x03,
            TemperatureSensorInput = 0x04
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::Inactive;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT ActuatorConnectType : private TypeN8
    {
    public:
        enum class Value : quint8
        {
            SensorConnection = 0x01,
            ControllerConnection = 0x02
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::SensorConnection;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT TypeB8
    {
    public:
        struct Q_KNX_EXPORT BasicContent
        {
            bool contentIsValid = false;
            bool b0 = false;
            bool b1 = false;
            bool b2 = false;
            bool b3 = false;
            bool b4 = false;
            bool b5 = false;
            bool b6 = false;
            bool b7 = false;
        };

        static bool validate(const QByteArray &bytes);
        static BasicContent getContent(const QByteArray &bytes);
        static QKnxDataPoint setBits(bool b0, bool b1, bool b2, bool b3, bool b4,
            QKnxDpt::TypesB8 type);
    };

    class Q_KNX_EXPORT GeneralStatus : private TypeB8
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            bool outOfService = false;
            bool fault = false;
            bool overridden = false;
            bool inAlarm = false;
            bool alarmUnacknowledged = false;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint set(Content content);
    };

    class Q_KNX_EXPORT DeviceControl : private TypeB8
    {
    public:
        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            bool userStopped = false;
            bool ownIndividualAddress = false;
            bool verifyMode = false;
        };

        static bool validate(const QByteArray &bytes);
        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint set(Content content);
    };

    class Q_KNX_EXPORT TypeN2
    {
    public:
        struct Q_KNX_EXPORT BasicContent
        {
            bool contentIsValid = false;
            quint8 value = 255;
        };

        static BasicContent getContent(const QByteArray &bytes);
        static bool validate(const QByteArray &bytes);
        static QKnxDataPoint setBits(quint8 value, QKnxDpt::TypesN2 type);
    };

    class Q_KNX_EXPORT OnOffAction : private TypeN2
    {
    public:
        enum class Value
        {
            Off, On, OffOn, OnOff
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::Off;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT AlarmReaction : private TypeN2
    {
    public:
        enum class Value
        {
            NoAlarmUsed, AlarmUp, AlarmDown
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::NoAlarmUsed;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };

    class Q_KNX_EXPORT UpDownAction : private TypeN2
    {
    public:
        enum class Value
        {
            Up, Down, UpDown, DownUp
        };

        struct Q_KNX_EXPORT Content
        {
            bool contentIsValid = false;
            Value value = Value::Up;
        };

        static Content getContent(const QByteArray &bytes);
        static QKnxDataPoint setValue(Value value);
    };
};

QT_END_NAMESPACE

#endif // QKNXDPTFACTORY_H
