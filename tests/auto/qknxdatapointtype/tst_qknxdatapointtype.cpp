/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtCore/qvector.h>
#include <QtCore/qmetaobject.h>
#include <QtKnx/qknx1bit.h>
#include <QtKnx/qknx1bitcontrolled.h>
#include <QtKnx/qknx2bytefloat.h>
#include <QtKnx/qknx2bytesignedvalue.h>
#include <QtKnx/qknx2byteunsignedvalue.h>
#include <QtKnx/qknx3bitcontrolled.h>
#include <QtKnx/qknx4bytesignedvalue.h>
#include <QtKnx/qknx4byteunsignedvalue.h>
#include <QtKnx/qknx8bitset.h>
#include <QtKnx/qknx8bitsignedvalue.h>
#include <QtKnx/qknx8bitunsignedvalue.h>
#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxdatapointtypefactory.h>
#include <QtKnx/qknxdatetime.h>
#include <QtKnx/qknxentranceaccess.h>
#include <QtKnx/qknxstatusmode3.h>
#include <QtKnx/qknxtime.h>
#include <QtKnx/qknxutils.h>
#include <QtTest/qtest.h>

class tst_QKnxDatapointType : public QObject
{
    Q_OBJECT

private slots:
    void datapointType();
    void dpt1_1Bit();
    void dpt2_1BitControlled();
    void dpt3_3BitControlled();
    void dpt5_8BitUnsignedValue();
    void dpt6_8BitSignedValue();
    void dpt6_StatusMode3();
    void dpt7_2ByteUnsignedValue();
    void dpt8_2ByteSignedValue();
    void dpt9_2ByteFloat();
    void dpt10_TimeOfDay();
    void dpt11_Date();
    void dpt12_4ByteUnsignedValue();
    void dpt13_4ByteSignedValue();
    void dpt15_EntranceAccess();
    void dpt19_DateTime();
    void dpt21_8BitSet();
};

void tst_QKnxDatapointType::datapointType()
{
    QKnxDatapointType type(QKnxDatapointType::Type::Dpt1_1Bit, 1);
    QCOMPARE(type.mainType(), 1);
    QCOMPARE(type.subType(), 0);
    QCOMPARE(type.type(), QKnxDatapointType::Type::Dpt1_1Bit);

    type = QKnxDatapointType("DPT-2", 1);
    QCOMPARE(type.mainType(), 2);
    QCOMPARE(type.subType(), 0);
    QCOMPARE(type.type(), QKnxDatapointType::Type::Dpt2_1BitControlled);

    type = QKnxDatapointType("DPST-2-1", 1);
    QCOMPARE(type.mainType(), 2);
    QCOMPARE(type.subType(), 1);
    QCOMPARE(type.type(), QKnxDatapointType::Type::DptSwitchControl);

    type = QKnxDatapointType(232, 600, 1);
    QCOMPARE(type.mainType(), 232);
    QCOMPARE(type.subType(), 600);
    QCOMPARE(type.type(), QKnxDatapointType::Type::DptColourRGB);
}

void tst_QKnxDatapointType::dpt1_1Bit()
{
    QKnx1Bit dpt1Bit;
    QCOMPARE(dpt1Bit.size(), 1);
    QCOMPARE(dpt1Bit.bit(), false);
    QCOMPARE(dpt1Bit.mainType(), 1);
    QCOMPARE(dpt1Bit.subType(), 0x00);
    QCOMPARE(dpt1Bit.isValid(), true);
    QCOMPARE(dpt1Bit.bytes(), QByteArray::fromHex("00"));
    QCOMPARE(dpt1Bit.type(), QKnxDatapointType::Type::Dpt1_1Bit);

    dpt1Bit.setBit(true);
    QCOMPARE(dpt1Bit.bit(), true);
    QCOMPARE(dpt1Bit.isValid(), true);
    QCOMPARE(dpt1Bit.bytes(), QByteArray::fromHex("01"));

    dpt1Bit.data()[0] = 0xff;
    QCOMPARE(dpt1Bit.isValid(), false);
    dpt1Bit[0] = 0x00;
    QCOMPARE(dpt1Bit.isValid(), true);
    dpt1Bit.setBytes(QByteArray::fromHex("01"), 0, 1);
    QCOMPARE(dpt1Bit.toString(), QStringLiteral("01"));

    QKnxSwitch dptSwitch;
    QCOMPARE(dptSwitch.size(), 1);
    QCOMPARE(dptSwitch.mainType(), 1);
    QCOMPARE(dptSwitch.subType(), 0x01);
    QCOMPARE(dptSwitch.isValid(), true);
    QCOMPARE(dptSwitch.value(), QKnxSwitch::State::Off);
    QCOMPARE(dptSwitch.bytes(), QByteArray::fromHex("00"));
    QCOMPARE(dptSwitch.type(), QKnxDatapointType::Type::DptSwitch);

    dptSwitch.setValue(QKnxSwitch::State::On);
    QCOMPARE(dptSwitch.isValid(), true);
    QCOMPARE(dptSwitch.value(), QKnxSwitch::State::On);
    QCOMPARE(dptSwitch.bytes(), QByteArray::fromHex("01"));

    dptSwitch.setBytes(QByteArray::fromHex("ff"), 0, 1);
    QCOMPARE(dptSwitch.isValid(), false);

    dptSwitch.setBytes(QByteArray::fromHex("01"), 0, 1);
    QCOMPARE(dptSwitch.isValid(), true);
    QCOMPARE(dptSwitch.value(), QKnxSwitch::State::On);

    auto &factory = QKnxDatapointTypeFactory::instance();
    QCOMPARE(factory.containsMainType(dpt1Bit.mainType()), true);
    QCOMPARE(factory.containsSubType(dpt1Bit.mainType(), dpt1Bit.subType()), true);

    QCOMPARE(factory.containsMainType(dptSwitch.mainType()), true);
    QCOMPARE(factory.containsSubType(dptSwitch.mainType(), dptSwitch.subType()), true);

    QScopedPointer<QKnxDatapointType> dpt(factory.createType(dpt1Bit.mainType(), dptSwitch.subType()));
    QCOMPARE(dpt->type(), QKnxDatapointType::Type::DptSwitch);

    auto knx1bit = dynamic_cast<QKnx1Bit *> (dpt.data());
    QVERIFY2(knx1bit != nullptr, "dynamic_cast to QKnx1Bit was not supposed to fail");

    auto knxSwitch = dynamic_cast<QKnxSwitch *> (dpt.data());
    QVERIFY2(knxSwitch != nullptr, "dynamic_cast to QKnxSwitch was not supposed to fail");

    auto controlled = dynamic_cast<QKnx1BitControlled *> (dpt.data());
    QCOMPARE(controlled, nullptr);

    dpt.reset(factory.createType(QKnxDatapointType::Type::Dpt1_1Bit));
    QCOMPARE(dpt->type(), QKnxDatapointType::Type::Dpt1_1Bit);
}

void tst_QKnxDatapointType::dpt2_1BitControlled()
{
    QKnx1BitControlled dpt;
    QCOMPARE(dpt.mainType(), 2);
    QCOMPARE(dpt.subType(), 0x00);

    QKnxSwitchControl sSwitchControlled;
    QCOMPARE(sSwitchControlled.mainType(), 2);
    QCOMPARE(sSwitchControlled.subType(), 0x01);

    // TODO: Extend.
}

void tst_QKnxDatapointType::dpt3_3BitControlled()
{
    QKnx3BitControlled dpt;
    QCOMPARE(dpt.mainType(), 0x03);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    dpt.setControlBit(false);
    dpt.setNumberOfIntervals(QKnx3BitControlled::NumberOfIntervals::Sixteen);
    QCOMPARE(dpt.controlBit(), false);
    QCOMPARE(dpt.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::Sixteen);

    QKnxControlDimming dptDimming(QKnxControlDimming::Decrease,
        QKnxControlDimming::NumberOfIntervals::ThirtyTwo);
    QCOMPARE(dptDimming.mainType(), 0x03);
    QCOMPARE(dptDimming.subType(), 0x07);
    QCOMPARE(dptDimming.isValid(), true);
    QCOMPARE(dptDimming.controlBit(), false);
    QCOMPARE(dptDimming.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::ThirtyTwo);
    dptDimming.setControl(QKnxControlDimming::Increase);
    QCOMPARE(dptDimming.control(), QKnxControlDimming::Increase);
    QCOMPARE(dptDimming.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::ThirtyTwo);

    QKnxControlBlinds dptBlinds(QKnxControlBlinds::Up,
        QKnxControlBlinds::NumberOfIntervals::ThirtyTwo);
    QCOMPARE(dptBlinds.mainType(), 0x03);
    QCOMPARE(dptBlinds.subType(), 0x08);
    QCOMPARE(dptBlinds.isValid(), true);
    QCOMPARE(dptBlinds.controlBit(), false);
    QCOMPARE(dptBlinds.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::ThirtyTwo);
    dptBlinds.setControl(QKnxControlBlinds::Down);
    QCOMPARE(dptBlinds.control(), QKnxControlBlinds::Down);
    QCOMPARE(dptBlinds.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::ThirtyTwo);
}

void tst_QKnxDatapointType::dpt5_8BitUnsignedValue()
{
    QKnx8BitUnsignedValue dpt;
    QCOMPARE(dpt.mainType(), 0x05);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.setValue(255),true);
    QCOMPARE(quint8(dpt.value()), quint8(255));

    QKnxScaling scaling;
    QCOMPARE(scaling.mainType(), 0x05);
    QCOMPARE(scaling.subType(), 0x01);
    QCOMPARE(scaling.isValid(), true);

    QCOMPARE(scaling.setValue(255), false);

    QCOMPARE(quint8(scaling.value()), quint8(0));
    QCOMPARE(scaling.setValue(1), true);
    QCOMPARE(qRound(scaling.value()), 1);
    QCOMPARE(scaling.setValue(0.4), true);
    QCOMPARE(qRound(10*scaling.value())/10., 0.4);
    QCOMPARE(scaling.setValue(1.2), true);
    QCOMPARE(qRound(10*scaling.value())/10., 1.2);
    scaling.setByte(0, 255);
    QCOMPARE(scaling.isValid(), true);
    QCOMPARE(quint8(scaling.value()), quint8(100));

    QKnxAngle angle;
    QCOMPARE(angle.mainType(), 0x05);
    QCOMPARE(angle.subType(), 0x03);
    QCOMPARE(angle.isValid(), true);
    QCOMPARE(angle.setValue(360), true);
    QCOMPARE(angle.value(), double(360));
    QCOMPARE(angle.setValue(1.4), true);
    QCOMPARE(qRound(10*angle.value())/10., double(1.4));

    QKnxTariff tariff;
    QCOMPARE(tariff.mainType(), 0x05);
    QCOMPARE(tariff.subType(), 0x06);
    QCOMPARE(tariff.isValid(), true);
    QCOMPARE(tariff.setValue(254), true);
    QCOMPARE(quint8(tariff.value()), quint8(254));
    QCOMPARE(tariff.setValue(255), false);
    QCOMPARE(quint8(tariff.value()), quint8(254));
    tariff.setByte(0, 255);
    QCOMPARE(tariff.isValid(), false);
}

void tst_QKnxDatapointType::dpt6_8BitSignedValue()
{
    QKnx8BitSignedValue dpt;
    QCOMPARE(dpt.mainType(), 0x06);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.value(), qint8(0));
    dpt.setValue(127);
    QCOMPARE(dpt.value(), qint8(127));
    dpt.setValue(-128);
    QCOMPARE(dpt.value(), qint8(-128));

    QKnxPercentV8 percent;
    QCOMPARE(percent.mainType(), 0x06);
    QCOMPARE(percent.subType(), 0x01);
    QCOMPARE(percent.isValid(), true);
    QCOMPARE(percent.value(), qint8(0));
    percent.setValue(127);
    QCOMPARE(percent.value(), qint8(127));
    percent.setValue(-128);
    QCOMPARE(percent.value(), qint8(-128));

    QKnxValue1Count value;
    QCOMPARE(value.mainType(), 0x06);
    QCOMPARE(value.subType(), 0x0a);
    QCOMPARE(value.isValid(), true);
    QCOMPARE(value.value(), qint8(0));
    value.setValue(127);
    QCOMPARE(value.value(), qint8(127));
    value.setValue(-128);
    QCOMPARE(value.value(), qint8(-128));
}

void tst_QKnxDatapointType::dpt6_StatusMode3()
{
    QKnxStatusMode3 dpt;
    QCOMPARE(dpt.mainType(), 0x06);
    QCOMPARE(dpt.subType(), 0x14);
    QCOMPARE(dpt.isValid(), false);

    QCOMPARE(dpt.setMode(QKnxStatusMode3::Mode::Unknown), false);
    QCOMPARE(dpt.mode(), QKnxStatusMode3::Mode::Unknown);
    QCOMPARE(dpt.setMode(QKnxStatusMode3::Mode::One), true);
    QCOMPARE(dpt.mode(), QKnxStatusMode3::Mode::One);

    dpt.setStatusFlags(QKnxStatusMode3::Status::A);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::A), true);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::B), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::C), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::D), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::E), false);
    dpt.removeStatus(QKnxStatusMode3::Status::A);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::A), false);

    dpt.setStatusFlags(QKnxStatusMode3::StatusFlags());
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::A), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::B), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::C), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::D), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::E), false);

    dpt.setBytes(QByteArray::fromHex("ff"), 0, 1);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(dpt.mode(), QKnxStatusMode3::Mode::Unknown);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::A), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::B), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::C), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::D), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::E), false);

    auto index = dpt.staticMetaObject.indexOfEnumerator("Status");
    QCOMPARE(index>= 0, true);
    auto statusEnum = dpt.staticMetaObject.enumerator(index);
    QCOMPARE(statusEnum.isValid(), true);
    QCOMPARE(statusEnum.keyCount(), 5);
    for (int i = 0; i < statusEnum.keyCount(); ++i) {
        dpt.setStatus(QKnxStatusMode3::Status(statusEnum.value(i)));
        QCOMPARE(dpt.isValid(), false);
        QCOMPARE(dpt.isSet(QKnxStatusMode3::Status(statusEnum.value(i))), true);
    }
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::A), true);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::B), true);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::C), true);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::D), true);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::E), true);
    QCOMPARE(dpt.setMode(QKnxStatusMode3::Mode::Zero), true);
    QCOMPARE(dpt.isValid(), true);
}

void tst_QKnxDatapointType::dpt7_2ByteUnsignedValue()
{
    QKnx2ByteUnsignedValue dpt;
    QCOMPARE(dpt.mainType(), 0x07);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.setValue(65535),true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(qint32(dpt.value()), qint32(65535));

    QKnxValue2Ucount dpt1;
    QCOMPARE(dpt1.mainType(), 0x07);
    QCOMPARE(dpt1.subType(), 0x01);
    QCOMPARE(dpt1.isValid(), true);
    QCOMPARE(dpt1.setValue(65535), true);
    QCOMPARE(quint32(dpt1.value()), quint32(65535));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt1.setValue(655350), false);
    QCOMPARE(quint32(dpt1.value()), quint32(65535));

    QKnxTimePeriod100Msec dpt2;
    QCOMPARE(dpt2.mainType(), 0x07);
    QCOMPARE(dpt2.subType(), 0x04);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(dpt2.setValue(65535), true);
    QCOMPARE(quint32(dpt2.value()), quint32(65500));
    QCOMPARE(dpt2.setValue(65555), true);
    QCOMPARE(quint32(dpt2.value()), quint32(65600));
    QCOMPARE(dpt2.setValue(6553500), true);
    QCOMPARE(quint32(dpt2.value()), quint32(6553500));

    QKnxTimePeriod10Msec dpt3(quint32(6550));
    QCOMPARE(dpt3.mainType(), 0x07);
    QCOMPARE(dpt3.subType(), 0x03);
    QCOMPARE(dpt3.isValid(), true);
    QCOMPARE(quint32(dpt3.value()), quint32(6550));

    // TODO: Extend the auto-test.
}

void tst_QKnxDatapointType::dpt8_2ByteSignedValue()
{
    QKnx2ByteSignedValue dpt;
    QCOMPARE(dpt.mainType(), 0x08);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.value(), 0.);
    dpt.setValue(-32768);
    QCOMPARE(dpt.value(), double(-32768.));
    dpt.setValue(32767);
    QCOMPARE(dpt.value(), 32767.);

    QKnxPercentV16 percent;
    QCOMPARE(percent.mainType(), 0x08);
    QCOMPARE(percent.subType(), 0x0a);
    QCOMPARE(percent.isValid(), true);
    QCOMPARE(percent.value(), 0.);
    percent.setValue(-32768);
    QCOMPARE(percent.value(), 0.);
    percent.setValue(-327.68);
    QCOMPARE(percent.value(), -327.68);
    percent.setValue(327.67);
    QCOMPARE(percent.value(), 327.67);

    QKnxDeltaTime100Msec delta;
    QCOMPARE(delta.mainType(), 0x08);
    QCOMPARE(delta.subType(), 0x04);
    QCOMPARE(delta.isValid(), true);
    QCOMPARE(delta.value(), 0.);
    delta.setValue(-32768);
    QCOMPARE(delta.value(), double(-32800));
    delta.setValue(-3276800);
    QCOMPARE(delta.value(), double(-3276800));
    delta.setValue(3276700);
    QCOMPARE(delta.value(), double(3276700));

    // TODO: Extend.
}

void tst_QKnxDatapointType::dpt21_8BitSet()
{
    QKnx8BitSet dpt;
    QCOMPARE(dpt.mainType(), 0x15);
    QCOMPARE(dpt.subType(), 0x00);

    QKnxGeneralStatus dptGS;
    QCOMPARE(dptGS.mainType(), 0x15);
    QCOMPARE(dptGS.subType(), 0x01);

    // TODO: Extend.
}

void tst_QKnxDatapointType::dpt10_TimeOfDay()
{
    QKnxTime time;
    QCOMPARE(time, QKnxTime());
    QCOMPARE(time.isNull(), true);
    QCOMPARE(time.isValid(), false);
    QCOMPARE(time.hour(), qint8(-1));
    QCOMPARE(time.minute(), qint8(-1));
    QCOMPARE(time.second(), qint8(-1));
    QCOMPARE(time.dayOfWeek(), QKnxTime::DayOfWeek::Ignore);

    time.setHMS(24, 0, 0);
    QCOMPARE(time.isValid(), false);

    time.setHMS(23, 59, 59);
    QCOMPARE(time.isValid(), true);

    time.setDayOfWeek(QKnxTime::DayOfWeek::Monday);
    QCOMPARE(time.dayOfWeek(), QKnxTime::DayOfWeek::Monday);
    QCOMPARE(time, QKnxTime(23, 59, 59, QKnxTime::DayOfWeek::Monday));

    QKnxTimeOfDay timeOfDay;
    QCOMPARE(timeOfDay.isValid(), true);
    QCOMPARE(timeOfDay.value(), QKnxTime(0, 0, 0, QKnxTime::DayOfWeek::Ignore));

    timeOfDay.setValue(time);
    QCOMPARE(timeOfDay.value(), time);

     QKnxTimeOfDay timeOfDay2;
     QCOMPARE(timeOfDay2.hour(), quint8(0));
     timeOfDay2.setHour(23);

    QCOMPARE(timeOfDay2.minute(), quint8(0));
    timeOfDay2.setMinute(59);

    QCOMPARE(timeOfDay2.second(), quint8(0));
    timeOfDay2.setSecond(59);

    QCOMPARE(timeOfDay2.dayOfDay(), QKnxTime::DayOfWeek::Ignore);
    timeOfDay2.setDayOfWeek(QKnxTime::DayOfWeek::Monday);

    QCOMPARE(timeOfDay.bytes(), timeOfDay2.bytes());
    QCOMPARE(QKnxDatapointTypeFactory::instance().containsMainType(timeOfDay.mainType()), true);

    QCOMPARE(time.staticMetaObject.enumeratorCount(), 1);
}

void tst_QKnxDatapointType::dpt11_Date()
{
    QKnxDate date;
    QCOMPARE(date.isValid(), false);
    QCOMPARE(date.value(), QDate(2000, 0, 0));
    QCOMPARE(date.year(), quint16(2000));
    QCOMPARE(date.month(), quint8(0));
    QCOMPARE(date.day(), quint8(0));

    date.setValue({ 1990, 01, 01 });
    QCOMPARE(date.value(), date.minimum().toDate());

    date.setYear(2089);
    QCOMPARE(date.value(), QDate(2089, 01, 01));

    date.setMonth(12);
    QCOMPARE(date.value(), QDate(2089, 12, 1));

    date.setDay(31);
    QCOMPARE(date.value(), QDate(2089, 12, 31));

    QCOMPARE(date.isValid(), true);
    QCOMPARE(date.value(), date.maximum().toDate());

    QCOMPARE(QKnxDatapointTypeFactory::instance().containsMainType(date.mainType()), true);
}

void tst_QKnxDatapointType::dpt13_4ByteSignedValue()
{
    QKnx4ByteSignedValue dpt;
    QCOMPARE(dpt.mainType(), 0x0d);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.setValue(255), true);
    QCOMPARE(qint32(dpt.value()), qint32(255));
    QCOMPARE(dpt.setValue(INT_MIN), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(qint32(dpt.value()), qint32(INT_MIN));
    QCOMPARE(dpt.setValue(2147483647), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(qint32(dpt.value()), qint32(2147483647));

    QKnxLongDeltaTimeSec dpt2;
    QCOMPARE(dpt2.mainType(), 0x0d);
    QCOMPARE(dpt2.subType(), 0x64);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(dpt2.setValue(255),true);
    QCOMPARE(qint32(dpt2.value()), qint32(255));
    QCOMPARE(dpt2.setValue(INT_MIN), true);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(qint32(dpt2.value()), qint32(INT_MIN));
    QCOMPARE(dpt2.setValue(2147483647), true);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(qint32(dpt2.value()), qint32(2147483647));
}

void tst_QKnxDatapointType::dpt9_2ByteFloat()
{
    QKnx2ByteFloat dpt;
    QCOMPARE(dpt.size(), 2);
    QCOMPARE(dpt.mainType(), 9);
    QCOMPARE(dpt.subType(), 0);
    QCOMPARE(dpt.isValid(), true);

    dpt.setValue(float(-5.2));
    QCOMPARE(dpt.value(),-5.2f);
    dpt.setValue(float(-671088.64));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.value(),-671088.64f);
    dpt.setValue(float(670760.96));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.value(),670760.96f);

    QKnxTemperatureCelcius dptTemp;
    QCOMPARE(dptTemp.size(), 2);
    QCOMPARE(dptTemp.mainType(), 9);
    QCOMPARE(dptTemp.subType(), 1);
    QCOMPARE(dptTemp.isValid(), true);
    dptTemp.setValue(float(-5.2));
    QCOMPARE(dptTemp.value(),-5.2f);
    QCOMPARE(dptTemp.setValue(float(-671088.64)), false);
    QCOMPARE(dptTemp.value(),float(-5.2));
    QCOMPARE(dptTemp.setValue(float(670760.96)), false);
    QCOMPARE(dptTemp.value(),float(-5.2));
    dptTemp.setValue(float(-273));
    QCOMPARE(dptTemp.value(),-272.96f);
    dptTemp.setValue(float(670760));
    QCOMPARE(dptTemp.value(),float(670760));

    // TODO: Extend the auto-test.
}

void tst_QKnxDatapointType::dpt12_4ByteUnsignedValue()
{
    QKnx4ByteUnsignedValue dpt;
    QCOMPARE(dpt.mainType(), 0x0c);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.setValue(4294967295),true);
    QCOMPARE(quint32(dpt.value()), quint32(4294967295));

    QKnxValue4UCount dpt2;
    QCOMPARE(dpt2.mainType(), 0x0c);
    QCOMPARE(dpt2.subType(), 0x01);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(dpt2.setValue(4294967295),true);
    QCOMPARE(quint32(dpt2.value()), quint32(4294967295));
}

void tst_QKnxDatapointType::dpt15_EntranceAccess()
{
    QKnxEntranceAccess dptAccessData;
    QCOMPARE(dptAccessData.size(), 4);
    QCOMPARE(dptAccessData.mainType(), 15);
    QCOMPARE(dptAccessData.subType(), 0x00);
    QCOMPARE(dptAccessData.isValid(), true);

    dptAccessData.setValue(90,QKnxEntranceAccess::Attributes().setFlag(QKnxEntranceAccess::Error),0);
    QCOMPARE(dptAccessData.isValid(), true);
    QCOMPARE(dptAccessData.isSet(QKnxEntranceAccess::Error), true);
    QCOMPARE(dptAccessData.isSet(QKnxEntranceAccess::Encrypted), false);
    dptAccessData.setAttribute(QKnxEntranceAccess::Encrypted);
    QCOMPARE(dptAccessData.isSet(QKnxEntranceAccess::Error), true);
    QCOMPARE(dptAccessData.isSet(QKnxEntranceAccess::Encrypted), true);
    dptAccessData.removeAttribute(QKnxEntranceAccess::Encrypted);
    QCOMPARE(dptAccessData.isSet(QKnxEntranceAccess::Encrypted), false);
    QCOMPARE(dptAccessData.digit(1), qint8(0));
    QCOMPARE(dptAccessData.digit(2), qint8(9));

    QKnxEntranceAccess dpt;
    QCOMPARE(dpt.isValid(), true);
    dpt.setBytes(QByteArray::fromHex("ff000000"), 0, 4);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(dpt.digit(1), qint8(-1));
    dpt.setBytes(QByteArray::fromHex("00010000"), 0, 4);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.digit(1), qint8(0));
    QCOMPARE(dpt.digit(3), qint8(1));

    // TODO: Extend the auto-test.
}

void tst_QKnxDatapointType::dpt19_DateTime()
{
    QKnxTime24 time;
    QCOMPARE(time, QKnxTime24());
    QCOMPARE(time.isNull(), true);
    QCOMPARE(time.isValid(), false);
    QCOMPARE(time.hour(), qint8(-1));
    QCOMPARE(time.minute(), qint8(-1));
    QCOMPARE(time.second(), qint8(-1));
    QCOMPARE(time.dayOfWeek(), QKnxTime24::DayOfWeek::Ignore);

    time.setHMS(23, 59, 59);
    QCOMPARE(time.isValid(), true);

    time.setHMS(24, 0, 0);
    QCOMPARE(time.isValid(), true);

    time.setDayOfWeek(QKnxTime24::DayOfWeek::Saturday);
    QCOMPARE(time.isValid(), true);

    QKnxDateTime dt;
    QCOMPARE(dt.isValid(), false);

    dt.setValue({ 2013, 11, 30 }, { 23, 45, 0 }, QKnxDateTime::Attribute::Fault,
        QKnxDateTime::ClockQuality::WithExtendedSyncSignal);
    QCOMPARE(dt.isValid(), true);
    QCOMPARE(QKnxDatapointTypeFactory::instance().containsMainType(dt.mainType()), true);

    QCOMPARE(time.staticMetaObject.enumeratorCount(), 1);

    // TODO: Extend the auto-test.
}

QTEST_MAIN(tst_QKnxDatapointType)

#include "tst_qknxdatapointtype.moc"
