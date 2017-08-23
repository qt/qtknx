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

#include <QtCore/qdebug.h>
#include <QtKnx/qknxdatapoint.h>
#include <QtKnx/qknxdptfactory.h>
#include <QtTest/qtest.h>
#include <QtTest>

class tst_QKnxDpt : public QObject
{
    Q_OBJECT
private:
    double myRounding(double value);

private slots:
    void test_TwoComplementEncoding();
    void test_TwoComplementEncoding16();
    void test_TwoComplementEncoding8();
    void test_IEEF754Encoding();
    void test_QKnxDptTypeB1();
    void test_QKnxDptControl();
    void test_QKnxDptControlDimming();
    void test_QKnxDptControlBlinds();
    void test_QKnxDptU8();
    void test_QKnxDptV8();
    void test_QKnxDptB5N3();
    void test_QKnxDptU16();
    void test_QKnxDptV16();
    void test_QKnxDptF16();
    void test_QKnxDptTimeOfDay();
    void test_QKnxDptDate();
    void test_QKnxDptUint32();
    void test_QKnxDptInt32();
    void test_QKnxDptF32();
    void test_QKnxDptAccessData();
    void test_QKnxDptScene();
    void test_QKnxDptDateTime();
    void test_QKnxDptN8(); // TODO test all of the TypeN8 classes
    void test_QKnxDptB8();
    void test_QKnxDptN2();
};

void tst_QKnxDpt::test_TwoComplementEncoding()
{
    qint32 value = -658;
    quint32 encodedValue = QKnxDptFactory::Encoding::fromSignedIntToTwoComplementNotation(value);
    qint32 testValue = QKnxDptFactory::Encoding::fromTwoComplementNotationToSignedInt<quint32>(
        encodedValue);
    // encoding and decoding functions are consistent
    QCOMPARE(value, testValue);

    // checking if computer signed int encoding corresponds to TwoComplementNotation implemented in QKnxDptFactory
    quint32* res = reinterpret_cast<quint32*>(&value);
    quint32 *uintfromint = res;
    QCOMPARE(encodedValue, *uintfromint);

    value = INT_MIN;
    encodedValue = QKnxDptFactory::Encoding::fromSignedIntToTwoComplementNotation(value);
    testValue = QKnxDptFactory::Encoding::fromTwoComplementNotationToSignedInt<quint32>(
        encodedValue);
    // encoding and decoding functions are consistent
    QCOMPARE(value, testValue);
    // checking if computer signed int encoding corresponds to TwoComplementNotation implemented in QKnxDptFactory
    res = reinterpret_cast<quint32*>(&value);
    uintfromint = res;
    QCOMPARE(encodedValue, *uintfromint);

    value = INT_MAX;
    encodedValue = QKnxDptFactory::Encoding::fromSignedIntToTwoComplementNotation(value);
    testValue = QKnxDptFactory::Encoding::fromTwoComplementNotationToSignedInt<quint32>(
        encodedValue);
    // encoding and decoding functions are consistent
    QCOMPARE(value, testValue);
    // checking if computer signed int encoding corresponds to TwoComplementNotation implemented in QKnxDptFactory
    res = reinterpret_cast<quint32*>(&value);
    uintfromint = res;
    QCOMPARE(encodedValue, *uintfromint);
}

void tst_QKnxDpt::test_TwoComplementEncoding16()
{
    qint16 value = SHRT_MIN;
    quint16 encodedValue = quint16(QKnxDptFactory::Encoding::fromSignedIntToTwoComplementNotation(value));
    qint16 testValue = qint16(QKnxDptFactory::Encoding::fromTwoComplementNotationToSignedInt<quint16>(
        encodedValue));
    // encoding and decoding functions are consistent
    QCOMPARE(value, testValue);

    // checking if computer signed int encoding corresponds to TwoComplementNotation implemented in QKnxDptFactory
    quint16* res = reinterpret_cast<quint16*>(&value);
    quint16 *uintfromint = res;
    QCOMPARE(encodedValue, *uintfromint);

    value = SHRT_MAX;
    encodedValue = quint16(QKnxDptFactory::Encoding::fromSignedIntToTwoComplementNotation(value));
    testValue = qint16(QKnxDptFactory::Encoding::fromTwoComplementNotationToSignedInt<quint16>(
        encodedValue));
    // encoding and decoding functions are consistent
    QCOMPARE(value, testValue);
    // checking if computer signed int encoding corresponds to TwoComplementNotation implemented in QKnxDptFactory
    res = reinterpret_cast<quint16*>(&value);
    uintfromint = res;
    QCOMPARE(encodedValue, *uintfromint);
}

void tst_QKnxDpt::test_TwoComplementEncoding8()
{
    qint8 value = SCHAR_MIN;
    quint8 encodedValue = quint8(QKnxDptFactory::Encoding::fromSignedIntToTwoComplementNotation(value));
    qint8 testValue = qint8(QKnxDptFactory::Encoding::fromTwoComplementNotationToSignedInt<quint8>(
        encodedValue));
    // encoding and decoding functions are consistent
    QCOMPARE(value, testValue);

    // checking if computer signed int encoding corresponds to TwoComplementNotation implemented in QKnxDptFactory
    quint8* res = reinterpret_cast<quint8*>(&value);
    quint8 *uintfromint = res;
    QCOMPARE(encodedValue, *uintfromint);

    value = SCHAR_MAX;
    encodedValue = quint8(QKnxDptFactory::Encoding::fromSignedIntToTwoComplementNotation(value));
    testValue = qint8(QKnxDptFactory::Encoding::fromTwoComplementNotationToSignedInt<quint8>(
        encodedValue));
    // encoding and decoding functions are consistent
    QCOMPARE(value, testValue);
    // checking if computer signed int encoding corresponds to TwoComplementNotation implemented in QKnxDptFactory
    res = reinterpret_cast<quint8*>(&value);
    uintfromint = res;
    QCOMPARE(encodedValue, *uintfromint);
}

void tst_QKnxDpt::test_IEEF754Encoding()
{
    float value = -5.1568262f;
    quint32 encodedValue = quint32(QKnxDptFactory::Encoding::fromFloatToIEEE754(value));
    float testValue = QKnxDptFactory::Encoding::fromIEEE754ToFloat(encodedValue);
    // encoding and decoding function are consistent
    QCOMPARE(testValue, value);

    // checking if computer float encoding corresponds to IEEE754 implemented in QKnxDptFactory
    quint32* res = reinterpret_cast<quint32*>(&value);
    quint32 *uintFromFloat = res;
    QCOMPARE(QKnxDptFactory::Encoding::fromIEEE754ToFloat(encodedValue),
        QKnxDptFactory::Encoding::fromIEEE754ToFloat(*uintFromFloat));
    // are the quint32 exactly the same?
    QCOMPARE(encodedValue, *uintFromFloat);
}

void tst_QKnxDpt::test_QKnxDptTypeB1()
{
    QKnxDataPoint dpt =  QKnxDptFactory::SimpleBinary::setBits(false);
    QCOMPARE(dpt.isValid(),true);
    QCOMPARE(QKnxDptFactory::SimpleBinary::getContent(dpt.bytes()).value, false);

    dpt = QKnxDptFactory::Switch::On;
    QCOMPARE(dpt.isValid(),true);
}

void tst_QKnxDpt::test_QKnxDptControl()
{
    QKnxDataPoint dpt =  QKnxDptFactory::Control::setBits(true, QKnxDptFactory::Control::Status::On);
    QCOMPARE(dpt.isValid(), true); // because we don't know the type
    QCOMPARE(QKnxDptFactory::Control::validate(dpt.bytes()), true);
    QCOMPARE(QKnxDptFactory::Control::getContent(dpt.bytes()).value, true);
    QCOMPARE(QKnxDptFactory::Control::getContent(dpt.bytes()).control, QKnxDptFactory::Control::Status::On);

    dpt = QKnxDptFactory::SwitchOn::ControlOn;
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::SwitchOn::getContent(dpt.bytes()).value, true);
    QCOMPARE(QKnxDptFactory::SwitchOn::getContent(dpt.bytes()).control, QKnxDptFactory::Control::Status::On);
}

void tst_QKnxDpt::test_QKnxDptControlDimming()
{
    QKnxDataPoint dpt =  QKnxDptFactory::TypeB1U3::setBits(true,65);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(QKnxDptFactory::TypeB1U3::validate(dpt.bytes()), false);
    QCOMPARE(QKnxDptFactory::TypeB1U3::getContent(dpt.bytes()).value, false);
    QCOMPARE(QKnxDptFactory::TypeB1U3::getContent(dpt.bytes()).numberOfInterval, quint8(65));

    dpt = QKnxDptFactory::ControlDimming::increaseWithNumberOfInternal(6);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::ControlDimming::getContent(dpt.bytes()).value, true);
    QCOMPARE(QKnxDptFactory::ControlDimming::getContent(dpt.bytes()).numberOfInterval, quint8(32));
}

void tst_QKnxDpt::test_QKnxDptControlBlinds()
{
    QKnxDataPoint dpt = QKnxDptFactory::ControlBlinds::moveUpWithNumberOfInternal(2);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::ControlBlinds::getContent(dpt.bytes()).value, false);
    QCOMPARE(QKnxDptFactory::ControlBlinds::getContent(dpt.bytes()).numberOfInterval, quint8(2));
}

double tst_QKnxDpt::myRounding(double value)
{
    return qRound(value * 10) / 10.;
}

void tst_QKnxDpt::test_QKnxDptU8()
{
    QKnxDataPoint dpt = QKnxDptFactory::Uint8::setValue(255);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::Uint8::getContent(dpt.bytes()).value.toInt(), 255);
    QCOMPARE(QKnxDptFactory::Uint8::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::Tariff::setValue(255);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(QKnxDptFactory::Uint8::getContent(dpt.bytes()).value.toInt(), 0);
    QCOMPARE(QKnxDptFactory::Uint8::getContent(dpt.bytes()).contentIsValid, false);

    dpt = QKnxDptFactory::Scaling::setPercent(0.4);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(myRounding(QKnxDptFactory::Scaling::getContent(dpt.bytes()).value.toDouble()), 0.4);
    dpt = QKnxDptFactory::Scaling::setPercent(50.4);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(myRounding(QKnxDptFactory::Scaling::getContent(dpt.bytes()).value.toDouble()), 50.6);
}

void tst_QKnxDpt::test_QKnxDptV8()
{
    QKnxDataPoint dpt = QKnxDptFactory::Int8::setValue(SCHAR_MIN);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::Int8::getContent(dpt.bytes()).value, qint8(SCHAR_MIN));

    dpt = QKnxDptFactory::PercentSigned::setPercent(-26);
    QCOMPARE(dpt.isValid(),true);
    QCOMPARE(QKnxDptFactory::PercentSigned::getContent(dpt.bytes()).value, qint8(-26));
    QCOMPARE(QKnxDptFactory::PercentSigned::getContent(dpt.bytes()).contentIsValid, true);
}

void tst_QKnxDpt::test_QKnxDptB5N3()
{
    QKnxDptFactory::StatusMode3::Content content;
    content.A = QKnxDptFactory::StatusMode3::Status::Clear;
    content.B = QKnxDptFactory::StatusMode3::Status::Clear;
    content.C = QKnxDptFactory::StatusMode3::Status::Clear;
    content.D = QKnxDptFactory::StatusMode3::Status::Clear;

    QCOMPARE(QKnxDptFactory::StatusMode3::validate(content), false);
    QKnxDataPoint dpt = QKnxDptFactory::StatusMode3::setBits(content);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(QKnxDptFactory::StatusMode3::getContent(dpt.bytes()).D,
        QKnxDptFactory::StatusMode3::Status::Unknown);
    QCOMPARE(QKnxDptFactory::StatusMode3::getContent(dpt.bytes()).D,
        QKnxDptFactory::StatusMode3::Status::Unknown);

    content.E = QKnxDptFactory::StatusMode3::Status::Clear;
    content.mode = QKnxDptFactory::StatusMode3::ActiveMode::M0;
    QCOMPARE(QKnxDptFactory::StatusMode3::validate(content), true);
    dpt = QKnxDptFactory::StatusMode3::setBits(content);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::StatusMode3::getContent(dpt.bytes()).D,
        QKnxDptFactory::StatusMode3::Status::Clear);
    QCOMPARE(QKnxDptFactory::StatusMode3::getContent(dpt.bytes()).mode,
        QKnxDptFactory::StatusMode3::ActiveMode::M0);
}

void tst_QKnxDpt::test_QKnxDptU16()
{
    QKnxDataPoint dpt = QKnxDptFactory::Uint16::setValue(255);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::Uint16::getContent(dpt.bytes()).value.toUInt(), quint32(255));
    QCOMPARE(QKnxDptFactory::Uint16::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::TimePeriod10MilliSec::setValue(655350);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TimePeriod10MilliSec::getContent(dpt.bytes()).value.toUInt(), quint32(655350));
    QCOMPARE(QKnxDptFactory::TimePeriod10MilliSec::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::TimePeriod100MilliSec::setValue(6553500);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TimePeriod100MilliSec::getContent(dpt.bytes()).value.toUInt(), quint32(6553500));
    QCOMPARE(QKnxDptFactory::TimePeriod100MilliSec::getContent(dpt.bytes()).contentIsValid, true);
}

void tst_QKnxDpt::test_QKnxDptV16()
{
    QKnxDataPoint dpt = QKnxDptFactory::Int16::setValue(SHRT_MIN);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::Int16::getContent(dpt.bytes()).value.toInt(), SHRT_MIN);
    QCOMPARE(QKnxDptFactory::Int16::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::DeltaTime10MilliSec::setValue(-255350);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::DeltaTime10MilliSec::getContent(dpt.bytes()).value.toInt(), qint32(-255350));
    QCOMPARE(QKnxDptFactory::DeltaTime10MilliSec::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::DeltaTime100MilliSec::setValue(-2553500);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::DeltaTime100MilliSec::getContent(dpt.bytes()).value.toInt(), qint32(-2553500));
    QCOMPARE(QKnxDptFactory::DeltaTime100MilliSec::getContent(dpt.bytes()).contentIsValid, true);
}

void tst_QKnxDpt::test_QKnxDptF16()
{
    QKnxDataPoint dpt = QKnxDptFactory::TemperatureCelcius::setValue(float(-0.02));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TemperatureCelcius::getContent(dpt.bytes()).value, -0.02f);
    QCOMPARE(QKnxDptFactory::TemperatureCelcius::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::RainAmount::setValue(float(-671088.64));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::RainAmount::getContent(dpt.bytes()).value, -671088.64f);
    QCOMPARE(QKnxDptFactory::RainAmount::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::RainAmount::setValue(float(670760.96));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::RainAmount::getContent(dpt.bytes()).value, 670760.96f);
    QCOMPARE(QKnxDptFactory::RainAmount::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::RainAmount::setValue(float(670760.968));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::RainAmount::getContent(dpt.bytes()).value, 670760.96f);
    QCOMPARE(QKnxDptFactory::RainAmount::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::BrightnessFloat::setValue(-255350);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(QKnxDptFactory::BrightnessFloat::getContent(dpt.bytes()).value, 0.0);
    QCOMPARE(QKnxDptFactory::BrightnessFloat::getContent(dpt.bytes()).contentIsValid, false);

    dpt = QKnxDptFactory::BrightnessFloat::setValue(float(255.357));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::BrightnessFloat::getContent(dpt.bytes()).value, 255.36f);
    QCOMPARE(QKnxDptFactory::BrightnessFloat::getContent(dpt.bytes()).contentIsValid, true);
}

void tst_QKnxDpt::test_QKnxDptTimeOfDay()
{
    QKnxDptFactory::TimeOfDay::Content content;
    content.dayOfTheWeek = 7;
    content.hour = 23;
    content.min = 59;
    content.sec = 59;
    QKnxDataPoint dpt = QKnxDptFactory::TimeOfDay::setTimeOfDay(content);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TimeOfDay::getContent(dpt.bytes()).hour, quint8(23));
    QCOMPARE(QKnxDptFactory::TimeOfDay::getContent(dpt.bytes()).min, quint8(59));
    QCOMPARE(QKnxDptFactory::TimeOfDay::getContent(dpt.bytes()).sec, quint8(59));
    QCOMPARE(QKnxDptFactory::TimeOfDay::getContent(dpt.bytes()).dayOfTheWeek, quint8(7));
}

void tst_QKnxDpt::test_QKnxDptDate()
{
    QKnxDptFactory::Date::Content content;
    content.day = 7;
    content.month = 2;
    content.year = 2059;
    QKnxDataPoint dpt = QKnxDptFactory::Date::setDate(content);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::Date::getContent(dpt.bytes()).day, quint8(7));
    QCOMPARE(QKnxDptFactory::Date::getContent(dpt.bytes()).month, quint8(2));
    QCOMPARE(QKnxDptFactory::Date::getContent(dpt.bytes()).year, quint16(2059));
    QCOMPARE(QKnxDptFactory::Date::getContent(dpt.bytes()).contentIsValid, true);
}

void tst_QKnxDpt::test_QKnxDptUint32()
{
    QKnxDataPoint dpt = QKnxDptFactory::Uint32::setValue(264486);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::Uint32::getContent(dpt.bytes()).value, quint32(264486));
    QCOMPARE(QKnxDptFactory::Uint32::getContent(dpt.bytes()).contentIsValid, true);
}

void tst_QKnxDpt::test_QKnxDptInt32()
{
    QKnxDataPoint dpt = QKnxDptFactory::Int32::setValue(-264486);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::Int32::getContent(dpt.bytes()).value, qint32(-264486));
    QCOMPARE(QKnxDptFactory::Int32::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::FlowRate::setValue(INT_MIN);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::Int32::getContent(dpt.bytes()).value, INT_MIN);
    QCOMPARE(QKnxDptFactory::Int32::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::FlowRate::setValue(2147483647);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::Int32::getContent(dpt.bytes()).value, qint32(2147483647));
    QCOMPARE(QKnxDptFactory::Int32::getContent(dpt.bytes()).contentIsValid, true);
}

void tst_QKnxDpt::test_QKnxDptF32()
{
    QKnxDataPoint dpt = QKnxDptFactory::TypeF32::setBits(float(-2644.86), QKnxDpt::TypesF32::Acceleration);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TypeF32::getContent(dpt.bytes()).value, float(-2644.86));
    QCOMPARE(QKnxDptFactory::TypeF32::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::TypeF32::setBits(float(-2644.86689), QKnxDpt::TypesF32::Acceleration);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TypeF32::getContent(dpt.bytes()).value, float(-2644.86689));
    QCOMPARE(QKnxDptFactory::TypeF32::getContent(dpt.bytes()).contentIsValid, true);
}

void tst_QKnxDpt::test_QKnxDptAccessData()
{
    QKnxDptFactory::AccessData::Content content;
    content.d6 = 6;
    content.d5 = 5;
    content.d4 = 4;
    content.d3 = 3;
    content.d2 = 2;
    content.d1 = 1;
    content.index = 15;
    content.error = QKnxDptFactory::AccessData::ErrorStatus::ReadingUnsuccessful;
    content.permission = QKnxDptFactory::AccessData::PermissionStatus::NotAccepted;
    content.reading = QKnxDptFactory::AccessData::ReadDirection::LeftToRight;
    content.encryption = QKnxDptFactory::AccessData::Encryption::No;
    QKnxDataPoint dpt = QKnxDptFactory::AccessData::setValue(content);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).d6, quint8(6));
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).d5, quint8(5));
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).d4, quint8(4));
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).d3, quint8(3));
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).d2, quint8(2));
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).d1, quint8(1));
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).index, quint8(15));
    QCOMPARE(quint8(QKnxDptFactory::AccessData::getContent(dpt.bytes()).error),
        quint8(QKnxDptFactory::AccessData::ErrorStatus::ReadingUnsuccessful));
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).permission,
        QKnxDptFactory::AccessData::PermissionStatus::NotAccepted);
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).reading,
        QKnxDptFactory::AccessData::ReadDirection::LeftToRight);
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).encryption,
        QKnxDptFactory::AccessData::Encryption::No);
    QCOMPARE(QKnxDptFactory::AccessData::getContent(dpt.bytes()).contentIsValid, true);
}

void tst_QKnxDpt::test_QKnxDptScene()
{
    QKnxDataPoint dpt = QKnxDptFactory::SceneNumber::setValue(quint8(56));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::SceneNumber::getContent(dpt.bytes()).sceneNumber, quint8(56));
    QCOMPARE(QKnxDptFactory::SceneNumber::getContent(dpt.bytes()).contentIsValid, true);

    dpt = QKnxDptFactory::SceneInfo::inactiveScene(56);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::SceneInfo::getContent(dpt.bytes()).sceneNumber, quint8(56));
    QCOMPARE(QKnxDptFactory::SceneInfo::getContent(dpt.bytes()).info,
        QKnxDptFactory::Scene::Info::InactiveScene);
    QCOMPARE(QKnxDptFactory::SceneInfo::getContent(dpt.bytes()).control,
        QKnxDptFactory::Scene::Control::NotApplicable);
    QCOMPARE(QKnxDptFactory::SceneInfo::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::SceneNumber::getContent(dpt.bytes()).contentIsValid, false);

    dpt = QKnxDptFactory::SceneControl::learn(56);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::SceneControl::getContent(dpt.bytes()).sceneNumber, quint8(56));
    QCOMPARE(QKnxDptFactory::SceneControl::getContent(dpt.bytes()).info,
        QKnxDptFactory::Scene::Info::NotApplicable);
    QCOMPARE(QKnxDptFactory::SceneControl::getContent(dpt.bytes()).control,
        QKnxDptFactory::Scene::Control::LearnScene);
    QCOMPARE(QKnxDptFactory::SceneControl::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::SceneNumber::getContent(dpt.bytes()).contentIsValid, false);
    QCOMPARE(QKnxDptFactory::SceneInfo::getContent(dpt.bytes()).contentIsValid, false);
}

void tst_QKnxDpt::test_QKnxDptDateTime()
{
    QKnxDptFactory::DateTime::Content content;
    content.year = 2056;
    content.month = 12;
    content.day = 31;
    content.dayOfWeek = 0;
    content.hourOfDay = 15;
    content.minutes = 26;
    content.seconds = 58;
    content.fault = QKnxDptFactory::DateTime::Fault::No;
    content.workingDay = QKnxDptFactory::DateTime::WorkingDay::Yes;
    content.workingDayField = QKnxDptFactory::DateTime::WorkingDayField::Valid;
    content.yearField = QKnxDptFactory::DateTime::YearField::NotValid;
    content.dateField = QKnxDptFactory::DateTime::DateField::Valid;
    content.dayOfWeekField = QKnxDptFactory::DateTime::DayOfWeekField::NotValid;
    content.timeField = QKnxDptFactory::DateTime::TimeField::NotValid;
    content.summerTime = QKnxDptFactory::DateTime::SummerTime::Yes;
    content.clockQuality = QKnxDptFactory::DateTime::ClockQuality::WithoutExtendedSyncSignal;

    QKnxDataPoint dpt = QKnxDptFactory::DateTime::setDateTime(content);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).year, quint16(2056));
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).month, quint8(12));
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).day, quint8(31));
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).dayOfWeek, quint8(0));
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).hourOfDay, quint8(15));
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).minutes, quint8(26));
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).seconds, quint8(58));
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).fault,
        QKnxDptFactory::DateTime::Fault::No);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).workingDay,
        QKnxDptFactory::DateTime::WorkingDay::Yes);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).workingDayField,
        QKnxDptFactory::DateTime::WorkingDayField::Valid);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).yearField,
        QKnxDptFactory::DateTime::YearField::NotValid);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).dateField,
        QKnxDptFactory::DateTime::DateField::Valid);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).dayOfWeekField,
        QKnxDptFactory::DateTime::DayOfWeekField::NotValid);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).timeField,
        QKnxDptFactory::DateTime::TimeField::NotValid);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).summerTime,
        QKnxDptFactory::DateTime::SummerTime::Yes);
    QCOMPARE(QKnxDptFactory::DateTime::getContent(dpt.bytes()).clockQuality,
        QKnxDptFactory::DateTime::ClockQuality::WithoutExtendedSyncSignal);
}

void tst_QKnxDpt::test_QKnxDptN8()
{
    QKnxDataPoint dpt = QKnxDptFactory::TypeN8::setBits(1, QKnxDpt::TypesN8::Unknown);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TypeN8::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::TypeN8::getContent(dpt.bytes()).value, quint8(1));

    dpt = QKnxDptFactory::ApplicationArea::setValue(QKnxDptFactory::ApplicationArea::Value::Lighting);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TypeN8::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::TypeN8::getContent(dpt.bytes()).value, quint8(20));
    QCOMPARE(QKnxDptFactory::ApplicationArea::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::ApplicationArea::getContent(dpt.bytes()).reservedValue, quint8(0));
    QCOMPARE(QKnxDptFactory::ApplicationArea::getContent(dpt.bytes()).value,
        QKnxDptFactory::ApplicationArea::Value::Lighting);

    dpt = QKnxDptFactory::ApplicationArea::setReservedValue(21,
        QKnxDptFactory::ApplicationArea::Value::LightingReserved);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TypeN8::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::TypeN8::getContent(dpt.bytes()).value, quint8(21));
    QCOMPARE(QKnxDptFactory::ApplicationArea::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::ApplicationArea::getContent(dpt.bytes()).reservedValue, quint8(21));

    // The following two will assert in debug mode, do not test here
    // dpt = QKnxDptFactory::ApplicationArea::setValue(QKnxDptFactory::ApplicationArea::Value::LightingReserved);
    // dpt = QKnxDptFactory::LightApplicationMode::setValue(QKnxDptFactory::LightApplicationMode::Value::ManufacturerValue);
}

void tst_QKnxDpt::test_QKnxDptB8()
{
    QKnxDataPoint dpt = QKnxDptFactory::TypeB8::setBits(false, false, true, true, true,
        QKnxDpt::TypesB8::Unknown);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b0, false);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b1, false);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b2, true);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b3, true);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b4, true);

    QKnxDptFactory::GeneralStatus::Content content;
    content.outOfService = false;
    content.fault = false;
    content.overridden = true;
    content.inAlarm = true;
    content.alarmUnacknowledged = true;
    dpt = QKnxDptFactory::GeneralStatus::set(content);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b0, false);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b1, false);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b2, true);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b3, true);
    QCOMPARE(QKnxDptFactory::TypeB8::getContent(dpt.bytes()).b4, true);
    QCOMPARE(QKnxDptFactory::GeneralStatus::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::GeneralStatus::getContent(dpt.bytes()).outOfService, false);
    QCOMPARE(QKnxDptFactory::GeneralStatus::getContent(dpt.bytes()).fault, false);
    QCOMPARE(QKnxDptFactory::GeneralStatus::getContent(dpt.bytes()).overridden, true);
    QCOMPARE(QKnxDptFactory::GeneralStatus::getContent(dpt.bytes()).inAlarm, true);
    QCOMPARE(QKnxDptFactory::GeneralStatus::getContent(dpt.bytes()).alarmUnacknowledged, true);
}

void tst_QKnxDpt::test_QKnxDptN2()
{
    QKnxDataPoint dpt = QKnxDptFactory::TypeN2::setBits(254, QKnxDpt::TypesN2::Unknown);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(QKnxDptFactory::TypeN2::getContent(dpt.bytes()).contentIsValid, false);
    QCOMPARE(QKnxDptFactory::TypeN2::getContent(dpt.bytes()).value, quint8(255));

    dpt = QKnxDptFactory::TypeN2::setBits(254, QKnxDpt::TypesN2::OnOffAction);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(QKnxDptFactory::TypeN2::getContent(dpt.bytes()).contentIsValid, false);
    QCOMPARE(QKnxDptFactory::TypeN2::getContent(dpt.bytes()).value, quint8(255));

    dpt = QKnxDptFactory::OnOffAction::setValue(QKnxDptFactory::OnOffAction::Value::On);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(QKnxDptFactory::TypeN2::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::TypeN2::getContent(dpt.bytes()).value, quint8(1));
    QCOMPARE(QKnxDptFactory::OnOffAction::getContent(dpt.bytes()).contentIsValid, true);
    QCOMPARE(QKnxDptFactory::OnOffAction::getContent(dpt.bytes()).value,
        QKnxDptFactory::OnOffAction::Value::On);
}

QTEST_MAIN(tst_QKnxDpt)

#include "tst_qknxdpt.moc"
