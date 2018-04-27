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
#include <QtKnx/qknx1byte.h>
#include <QtKnx/qknx2bitset.h>
#include <QtKnx/qknx2bytefloat.h>
#include <QtKnx/qknx2bytesignedvalue.h>
#include <QtKnx/qknx2byteunsignedvalue.h>
#include <QtKnx/qknx3bitcontrolled.h>
#include <QtKnx/qknx4bytefloat.h>
#include <QtKnx/qknx4bytesignedvalue.h>
#include <QtKnx/qknx4byteunsignedvalue.h>
#include <QtKnx/qknx8bitset.h>
#include <QtKnx/qknx32bitset.h>
#include <QtKnx/qknx8bitsignedvalue.h>
#include <QtKnx/qknx8bitunsignedvalue.h>
#include <QtKnx/qknxchar.h>
#include <QtKnx/qknxcharstring.h>
#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxdatapointtypefactory.h>
#include <QtKnx/qknxdatetime.h>
#include <QtKnx/qknxelectricalenergy.h>
#include <QtKnx/qknxentranceaccess.h>
#include <QtKnx/qknxscene.h>
#include <QtKnx/qknxstatusmode3.h>
#include <QtKnx/qknxtime.h>
#include <QtKnx/qknxutf8string.h>
#include <QtKnx/qknxvarstring.h>
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
    void dpt4_Character();
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
    void dpt14_4ByteFloat();
    void dpt15_EntranceAccess();
    void dpt17_SceneNumber();
    void dpt16_String();
    void dpt18_SceneControl();
    void dpt19_DateTime();
    void dpt20_1Byte();
    void dpt21_8BitSet();
    void dpt23_2BitSet();
    void dpt24_VarString();
    void dpt26_SceneInfo();
    void dpt27_32BitSet();
    void dpt28_StringUtf8();
    void dpt29_ElectricalEnergy();
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
    QCOMPARE(dpt1Bit.bytes(), QKnxByteArray({ 0x00 }));
    QCOMPARE(dpt1Bit.type(), QKnxDatapointType::Type::Dpt1_1Bit);

    dpt1Bit.setBit(true);
    QCOMPARE(dpt1Bit.bit(), true);
    QCOMPARE(dpt1Bit.isValid(), true);
    QCOMPARE(dpt1Bit.constData()[0], 0x01);
    QCOMPARE(dpt1Bit.bytes(), QKnxByteArray({ 0x01 }));

    dpt1Bit.setBytes(QKnxByteArray { 0x01 }, 0, 1);

    QKnxSwitch dptSwitch;
    QCOMPARE(dptSwitch.size(), 1);
    QCOMPARE(dptSwitch.mainType(), 1);
    QCOMPARE(dptSwitch.subType(), 0x01);
    QCOMPARE(dptSwitch.isValid(), true);
    QCOMPARE(dptSwitch.value(), QKnxSwitch::State::Off);
    QCOMPARE(dptSwitch.bytes(), QKnxByteArray({ 0x00 }));
    QCOMPARE(dptSwitch.type(), QKnxDatapointType::Type::DptSwitch);

    dptSwitch.setValue(QKnxSwitch::State::On);
    QCOMPARE(dptSwitch.isValid(), true);
    QCOMPARE(dptSwitch.value(), QKnxSwitch::State::On);
    QCOMPARE(dptSwitch.bytes(), QKnxByteArray({ 0x01 }));

    dptSwitch.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(dptSwitch.isValid(), false);

    dptSwitch.setBytes(QKnxByteArray { 0x01 }, 0, 1);
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

    QKnxWindowDoor dptWindowDoor;
    QCOMPARE(dptWindowDoor.size(), 1);
    QCOMPARE(dptWindowDoor.mainType(), 1);
    QCOMPARE(dptWindowDoor.subType(), 19);
    QCOMPARE(dptWindowDoor.isValid(), true);
    QCOMPARE(dptWindowDoor.value(), QKnxWindowDoor::State::Closed);
    QCOMPARE(dptWindowDoor.bytes(), QKnxByteArray({ 0x00 }));
    QCOMPARE(dptWindowDoor.type(), QKnxDatapointType::Type::DptWindowDoor);
}

void tst_QKnxDatapointType::dpt2_1BitControlled()
{
    QKnx1BitControlled dpt;
    QCOMPARE(dpt.mainType(), 2);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.valueBit(), false);
    QCOMPARE(dpt.controlBit(), false);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.bytes(), QKnxByteArray({ 0x00 }));
    QCOMPARE(dpt.type(), QKnxDatapointType::Type::Dpt2_1BitControlled);

    QKnxSwitchControl sSwitchControlled(QKnxSwitchControl::State::Off, QKnxSwitchControl::Control::NoControl);
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
    QCOMPARE(dpt.controlBit(), false);
    QCOMPARE(dpt.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::Break);
    dpt.setControlBit(false);
    dpt.setNumberOfIntervals(QKnx3BitControlled::NumberOfIntervals::Sixteen);
    QCOMPARE(dpt.controlBit(), false);
    QCOMPARE(dpt.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::Sixteen);

    QKnxControlDimming dptDim;
    QCOMPARE(dptDim.mainType(), 0x03);
    QCOMPARE(dptDim.subType(), 0x07);
    QCOMPARE(dptDim.isValid(), true);
    QCOMPARE(dptDim.controlBit(), false);
    QCOMPARE(dptDim.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::Break);
    QKnxControlDimming dptDimming(QKnxControlDimming::Decrease,
        QKnxControlDimming::NumberOfIntervals::ThirtyTwo);
    QCOMPARE(dptDimming.mainType(), 0x03);
    QCOMPARE(dptDimming.subType(), 0x07);
    QCOMPARE(dptDimming.isValid(), true);
    QCOMPARE(dptDimming.controlBit(), false);
    QCOMPARE(dptDimming.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::ThirtyTwo);
    dptDimming.setControl(QKnxControlDimming::Increase);
    QCOMPARE(dptDimming.control(), QKnxControlDimming::Increase);
    QCOMPARE(dptDimming.isValid(), true);
    QCOMPARE(dptDimming.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::ThirtyTwo);

    QKnxControlBlinds dptBl;
    QCOMPARE(dptBl.mainType(), 0x03);
    QCOMPARE(dptBl.subType(), 0x08);
    QCOMPARE(dptBl.isValid(), true);
    QCOMPARE(dptBl.controlBit(), false);
    QCOMPARE(dptBl.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::Break);
    QKnxControlBlinds dptBlinds(QKnxControlBlinds::Up,
        QKnxControlBlinds::NumberOfIntervals::ThirtyTwo);
    QCOMPARE(dptBlinds.mainType(), 0x03);
    QCOMPARE(dptBlinds.subType(), 0x08);
    QCOMPARE(dptBlinds.isValid(), true);
    QCOMPARE(dptBlinds.controlBit(), false);
    QCOMPARE(dptBlinds.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::ThirtyTwo);
    dptBlinds.setControl(QKnxControlBlinds::Up);
    QCOMPARE(dptBlinds.isValid(), true);
    QCOMPARE(dptBlinds.control(), QKnxControlBlinds::Up);
    QCOMPARE(dptBlinds.numberOfIntervals(), QKnx3BitControlled::NumberOfIntervals::ThirtyTwo);
}

void tst_QKnxDatapointType::dpt4_Character()
{
    QKnxChar dpt;
    QCOMPARE(dpt.mainType(), 0x04);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.value(), quint8(0));
    QCOMPARE(dpt.setValue(255), true);
    QCOMPARE(dpt.value(), quint8(255));

    QKnxCharASCII asci;
    QCOMPARE(asci.mainType(), 0x04);
    QCOMPARE(asci.subType(), 0x01);
    QCOMPARE(asci.isValid(), true);
    QCOMPARE(asci.value(), quint8(0));
    QCOMPARE(asci.setValue(255), false);
    QCOMPARE(asci.value(), quint8(0));
    QCOMPARE(asci.setCharacter('p'), true);
    QCOMPARE(asci.value(), quint8(112));
    QCOMPARE(asci.setCharacter('P'), true);
    QCOMPARE(asci.value(), quint8(80));
    QCOMPARE(asci.setCharacter('~'), true);
    QCOMPARE(asci.value(), quint8(126));
    QCOMPARE(asci.setCharacter('!'), true);
    QCOMPARE(asci.value(), quint8(33));
    QCOMPARE(asci.setCharacter(char(NULL)), true);
    QCOMPARE(asci.value(), quint8(0));
    QCOMPARE(asci.setCharacter(char('\n')), true); // LF
    QCOMPARE(asci.value(), quint8(10));
    QCOMPARE(asci.setCharacter('p'), true);
    QCOMPARE(asci.character(), 'p');

    QKnxChar88591 char88;
    QCOMPARE(char88.mainType(), 0x04);
    QCOMPARE(char88.subType(), 0x02);
    QCOMPARE(char88.isValid(), true);
    QCOMPARE(char88.value(), quint8(0));
    QCOMPARE(char88.setValue(255), true);
    QCOMPARE(char88.value(), quint8(255));
    QCOMPARE(quint8(char88.character()), quint8(255));
}

void tst_QKnxDatapointType::dpt5_8BitUnsignedValue()
{
    QKnx8BitUnsignedValue dpt;
    QCOMPARE(dpt.mainType(), 0x05);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.setValue(255), true);
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
    QCOMPARE(qRound(10 * scaling.value()) / 10., 0.4);
    QCOMPARE(scaling.setValue(1.2), true);
    QCOMPARE(qRound(10 * scaling.value()) / 10., 1.2);
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
    QCOMPARE(qRound(10 * angle.value()) / 10., double(1.4));

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

    QCOMPARE(dpt.byte(0), quint8(0x0f8));
    QCOMPARE(dpt.mode(), QKnxStatusMode3::Mode::Unknown);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::A), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::B), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::C), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::D), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::E), false);


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

    dpt.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(dpt.mode(), QKnxStatusMode3::Mode::Unknown);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::A), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::B), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::C), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::D), false);
    QCOMPARE(dpt.isSet(QKnxStatusMode3::Status::E), false);

    auto index = dpt.staticMetaObject.indexOfEnumerator("Status");
    QCOMPARE(index >= 0, true);
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
    QCOMPARE(dpt.setValue(65535), true);
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

void tst_QKnxDatapointType::dpt9_2ByteFloat()
{
    QKnx2ByteFloat dpt;
    QCOMPARE(dpt.size(), 2);
    QCOMPARE(dpt.mainType(), 9);
    QCOMPARE(dpt.subType(), 0);
    QCOMPARE(dpt.isValid(), true);

    dpt.setValue(float(-5.2));
    QCOMPARE(dpt.value(), -5.2f);
    dpt.setValue(float(-671088.64));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.value(), -671088.64f);
    dpt.setValue(float(670760.96));
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.value(), 670760.96f);

    QKnxTemperatureCelsius dptTemp;
    QCOMPARE(dptTemp.size(), 2);
    QCOMPARE(dptTemp.mainType(), 9);
    QCOMPARE(dptTemp.subType(), 1);
    QCOMPARE(dptTemp.isValid(), true);
    dptTemp.setValue(float(-5.2));
    QCOMPARE(dptTemp.value(), -5.2f);
    QCOMPARE(dptTemp.setValue(float(-671088.64)), false);
    QCOMPARE(dptTemp.value(), float(-5.2));
    QCOMPARE(dptTemp.setValue(float(670760.96)), false);
    QCOMPARE(dptTemp.value(), float(-5.2));
    dptTemp.setValue(float(-273));
    QCOMPARE(dptTemp.value(), -272.96f);
    dptTemp.setValue(float(670760));
    QCOMPARE(dptTemp.value(), float(670760));

    // TODO: Extend the auto-test.
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
    QCOMPARE(timeOfDay2.isValid(), true);
    QCOMPARE(timeOfDay2.hour(), quint8(0));
    QCOMPARE(timeOfDay2.setHour(23), true);
    QCOMPARE(timeOfDay2.hour(), quint8(23));
    QCOMPARE(timeOfDay2.setHour(24), false);
    QCOMPARE(timeOfDay2.hour(), quint8(23));

    QCOMPARE(timeOfDay2.minute(), quint8(0));
    QCOMPARE(timeOfDay2.setMinute(59), true);
    QCOMPARE(timeOfDay2.minute(), quint8(59));
    QCOMPARE(timeOfDay2.setMinute(60), false);
    QCOMPARE(timeOfDay2.minute(), quint8(59));

    QCOMPARE(timeOfDay2.second(), quint8(0));
    QCOMPARE(timeOfDay2.setSecond(59), true);
    QCOMPARE(timeOfDay2.second(), quint8(59));
    QCOMPARE(timeOfDay2.setSecond(60), false);
    QCOMPARE(timeOfDay2.second(), quint8(59));

    QCOMPARE(timeOfDay2.dayOfDay(), QKnxTime::DayOfWeek::Ignore);
    QCOMPARE(timeOfDay2.setDayOfWeek(QKnxTime::DayOfWeek::Monday), true);
    QCOMPARE(timeOfDay2.dayOfDay(), QKnxTime::DayOfWeek::Monday);
    QCOMPARE(timeOfDay2.setDayOfWeek(QKnxTime::DayOfWeek::Ignore), true);
    QCOMPARE(timeOfDay2.dayOfDay(), QKnxTime::DayOfWeek::Ignore);
    QCOMPARE(timeOfDay2.isValid(), true);
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

    QCOMPARE(date.setValue({ 1990, 01, 01 }), true);
    QCOMPARE(date.value(), date.minimum().toDate());

    QCOMPARE(date.setYear(2089), true);
    QCOMPARE(date.value(), QDate(2089, 01, 01));

    QCOMPARE(date.setMonth(12), true);
    QCOMPARE(date.value(), QDate(2089, 12, 1));
    QCOMPARE(date.setMonth(0), false);
    QCOMPARE(date.value(), QDate(2089, 12, 1));

    QCOMPARE(date.setDay(31), true);
    QCOMPARE(date.value(), QDate(2089, 12, 31));
    QCOMPARE(date.setDay(0), false);
    QCOMPARE(date.value(), QDate(2089, 12, 31));

    QCOMPARE(date.isValid(), true);
    QCOMPARE(date.value(), date.maximum().toDate());

    QCOMPARE(QKnxDatapointTypeFactory::instance().containsMainType(date.mainType()), true);
}

void tst_QKnxDatapointType::dpt12_4ByteUnsignedValue()
{
    QKnx4ByteUnsignedValue dpt;
    QCOMPARE(dpt.mainType(), 0x0c);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.setValue(4294967295), true);
    QCOMPARE(quint32(dpt.value()), quint32(4294967295));

    QKnxValue4UCount dpt2;
    QCOMPARE(dpt2.mainType(), 0x0c);
    QCOMPARE(dpt2.subType(), 0x01);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(dpt2.setValue(4294967295), true);
    QCOMPARE(quint32(dpt2.value()), quint32(4294967295));
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
    QCOMPARE(dpt2.setValue(255), true);
    QCOMPARE(qint32(dpt2.value()), qint32(255));
    QCOMPARE(dpt2.setValue(INT_MIN), true);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(qint32(dpt2.value()), qint32(INT_MIN));
    QCOMPARE(dpt2.setValue(2147483647), true);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(qint32(dpt2.value()), qint32(2147483647));
}

void tst_QKnxDatapointType::dpt14_4ByteFloat()
{
    QKnx4ByteFloat dpt;
    QCOMPARE(dpt.size(), 4);
    QCOMPARE(dpt.mainType(), 14);
    QCOMPARE(dpt.subType(), 0);
    QCOMPARE(dpt.isValid(), true);

    dpt.setValue(float(-5.2));
    QCOMPARE(dpt.value(), -5.2f);
    dpt.setValue(float(-671088.64));
    QCOMPARE(dpt.value(), -671088.64f);
    dpt.setValue(float(670760.96));
    QCOMPARE(dpt.value(), 670760.96f);
    dpt.setValue(float(-3.40282e+38));
    QCOMPARE(dpt.value(), float(-3.40282e+38));
    dpt.setValue(float(3.40282e+38));
    QCOMPARE(dpt.value(), float(3.40282e+38));

    // TODO: Extend the auto-test.
}

void tst_QKnxDatapointType::dpt15_EntranceAccess()
{
    QKnxEntranceAccess dptAccessData;
    QCOMPARE(dptAccessData.size(), 4);
    QCOMPARE(dptAccessData.mainType(), 15);
    QCOMPARE(dptAccessData.subType(), 0x00);
    QCOMPARE(dptAccessData.isValid(), true);
    QCOMPARE(dptAccessData.isSet(QKnxEntranceAccess::Error), false);
    QCOMPARE(dptAccessData.isSet(QKnxEntranceAccess::Encrypted), false);
    QCOMPARE(dptAccessData.isSet(QKnxEntranceAccess::PermissionAccepted), false);
    QCOMPARE(dptAccessData.isSet(QKnxEntranceAccess::ReadRightToLeft), false);
    QCOMPARE(dptAccessData.idCode(), qint32(0));

    dptAccessData.setValue(90, QKnxEntranceAccess::Attributes().setFlag(QKnxEntranceAccess::Error), 0);
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
    QCOMPARE(dptAccessData.setDigit(3, 1), true);
    QCOMPARE(dptAccessData.digit(3), qint8(1));
    QCOMPARE(dptAccessData.setDigit(6, 9), true);
    QCOMPARE(dptAccessData.digit(6), qint8(9));

    QKnxEntranceAccess dpt;
    QCOMPARE(dpt.isValid(), true);
    dpt.setBytes(QKnxByteArray { 0xff, 0x00, 0x00, 0x00 }, 0, 4);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(dpt.digit(1), qint8(-1));
    dpt.setBytes(QKnxByteArray { 0x00, 0x01, 0x00, 0x00 }, 0, 4);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.digit(1), qint8(0));
    QCOMPARE(dpt.digit(3), qint8(1));

    QKnxEntranceAccess dpt2;
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(dpt2.setIdCode(150276), true);
    QCOMPARE(dpt2.idCode(), 150276);
    QCOMPARE(dpt2.setAttributes(QKnxEntranceAccess::PermissionAccepted | QKnxEntranceAccess::Encrypted), true);
    QCOMPARE(dpt2.attributes(), QKnxEntranceAccess::PermissionAccepted | QKnxEntranceAccess::Encrypted);
    QCOMPARE(dpt2.setIndex(3), true);
    QCOMPARE(dpt2.index(), quint8(3));

    QCOMPARE(dpt2.digit(0), -1);
    QCOMPARE(dpt2.digit(7), -1);
    QCOMPARE(dpt2.setDigit(0, 1), false);
    QCOMPARE(dpt2.setDigit(7, 1), false);
    QCOMPARE(dpt2.setDigit(4, 10), false);
}

void tst_QKnxDatapointType::dpt16_String()
{
    QKnxCharString knxString;
    QCOMPARE(knxString.mainType(), 0x10);
    QCOMPARE(knxString.subType(), 0x00);
    QCOMPARE(knxString.size(), 0x0e);
    QCOMPARE(knxString.isValid(), true);
    QCOMPARE(knxString.string(), QLatin1String());

    QCOMPARE(knxString.setString(QLatin1String("KNX is OK")), true);
    QCOMPARE(knxString.string(), QLatin1String("KNX is OK"));
    QCOMPARE(knxString.bytes(), QKnxByteArray({ 0x4b, 0x4e, 0x58, 0x20, 0x69, 0x73, 0x20, 0x4f,
        0x4b, 0x00, 0x00, 0x00, 0x00, 0x00 }));

    unsigned char value[4] = { 0xdf, 0xef, 0xff, 0x00 };
    QCOMPARE(knxString.setString(QLatin1String((const char*) value)), true);
    QCOMPARE(knxString.bytes(), QKnxByteArray({ 0xdf, 0xef, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }));

    QKnxCharStringASCII stringA;
    QCOMPARE(stringA.mainType(), 16);
    QCOMPARE(stringA.subType(), 0);
    QCOMPARE(stringA.size(), 14);
    QCOMPARE(stringA.isValid(), true);

    QCOMPARE(stringA.setString(QLatin1String((const char*) value)), false);
    QCOMPARE(stringA.string(), QLatin1String());
    QCOMPARE(stringA.bytes(), QKnxByteArray(14, 0x00));
    QCOMPARE(stringA.isValid(), true);

    QCOMPARE(stringA.setByte(0, 255), true);
    QCOMPARE(stringA.byte(0), quint8(255));
    QCOMPARE(stringA.isValid(), false);
    QCOMPARE(stringA.setByte(14, 255), false);
    QCOMPARE(stringA.setByte(13, 255), true);
    QCOMPARE(stringA.byte(13), quint8(255));
    QCOMPARE(stringA.isValid(), false);

    QCOMPARE(stringA.setBytes(QKnxByteArray { 0x4b, 0x4e, 0x58, 0x20, 0x69, 0x73, 0x20, 0x4f,
        0x4b, 0x00, 0x00, 0x00, 0x00, 0x00 }, 0, 14), true);
    QCOMPARE(stringA.string(), QLatin1String("KNX is OK"));
    QCOMPARE(stringA.isValid(), true);

    QKnxCharString88591 string8;
    QCOMPARE(string8.mainType(), 16);
    QCOMPARE(string8.subType(), 1);
    QCOMPARE(string8.size(), 14);
    QCOMPARE(string8.isValid(), true);
    QCOMPARE(string8.byte(0), quint8(0));
    QCOMPARE(string8.setString(QLatin1String((const char*) value)), true);
    QCOMPARE(string8.bytes(), QKnxByteArray({ 0xdf, 0xef, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }));
}

void tst_QKnxDatapointType::dpt17_SceneNumber()
{
    QKnxSceneNumber dpt;
    QCOMPARE(dpt.size(), 1);
    QCOMPARE(dpt.mainType(), 17);
    QCOMPARE(dpt.subType(), 0x01);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));

    QCOMPARE(dpt.setSceneNumber(63), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(63));
    QCOMPARE(dpt.setSceneNumber(0), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));
    QCOMPARE(dpt.setSceneNumber(64), false);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));

    dpt.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(dpt.sceneNumber(), quint8(63));
}

void tst_QKnxDatapointType::dpt18_SceneControl()
{
    QKnxSceneControl dpt;
    QCOMPARE(dpt.size(), 1);
    QCOMPARE(dpt.mainType(), 18);
    QCOMPARE(dpt.subType(), 0x01);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));
    QCOMPARE(dpt.control(), QKnxSceneControl::Control::Activate);

    QCOMPARE(dpt.setSceneNumber(63), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(63));
    QCOMPARE(dpt.setSceneNumber(0), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));
    QCOMPARE(dpt.setSceneNumber(64), false);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));

    QCOMPARE(dpt.setControl(QKnxSceneControl::Activate), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.control(), QKnxSceneControl::Activate);
    QCOMPARE(dpt.setControl(QKnxSceneControl::Learn), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.control(), QKnxSceneControl::Learn);
    QCOMPARE(dpt.setControl(QKnxSceneControl::Control(2)), false);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.control(), QKnxSceneControl::Learn);

    dpt.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(dpt.sceneNumber(), quint8(63));
    QCOMPARE(dpt.control(), QKnxSceneControl::Control::Learn);

    dpt.setBytes(QKnxByteArray { 0x80 }, 0, 1);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));
    QCOMPARE(dpt.control(), QKnxSceneControl::Control::Learn);

    dpt.setBytes(QKnxByteArray { 0x40 }, 0, 1);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(dpt.sceneNumber(), quint8(0));
    QCOMPARE(dpt.control(), QKnxSceneControl::Control::Activate);
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
    QCOMPARE(dt.date(), QDate(2000, 0, 0));
    QCOMPARE(dt.time(), QKnxTime24(0, 0));
    QKnxDateTime::Attributes testAttributes =
        QKnxDateTime::Attributes().setFlag(QKnxDateTime::Attribute::Fault, false)
            .setFlag(QKnxDateTime::Attribute::WorkingDay, false)
            .setFlag(QKnxDateTime::Attribute::NoWorkingDay, false)
            .setFlag(QKnxDateTime::Attribute::NoYear, false)
            .setFlag(QKnxDateTime::Attribute::NoDate, false)
            .setFlag(QKnxDateTime::Attribute::NoDayOfWeek, false)
            .setFlag(QKnxDateTime::Attribute::NoTime, false)
            .setFlag(QKnxDateTime::Attribute::StandardSummerTime, false);
    QCOMPARE(dt.attributes(), testAttributes);
    QCOMPARE(dt.clockQuality(), QKnxDateTime::ClockQuality::WithoutExtendedSyncSignal);

    QCOMPARE(dt.setValue({ 2013, 11, 30 }, { 23, 45, 0 }, QKnxDateTime::Attribute::Fault,
        QKnxDateTime::ClockQuality::WithExtendedSyncSignal), true);
    QCOMPARE(dt.isValid(), true);
    QCOMPARE(QKnxDatapointTypeFactory::instance().containsMainType(dt.mainType()), true);

    QCOMPARE(time.staticMetaObject.enumeratorCount(), 1);

    // TODO: Extend the auto-test.
}

void tst_QKnxDatapointType::dpt20_1Byte()
{
    QKnx1Byte dpt;
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.size(), 1);
    QCOMPARE(dpt.mainType(), 20);
    QCOMPARE(dpt.subType(), 0);

    dpt.setValue(0);
    QCOMPARE(dpt.value(), quint8(0));

    QKnxScloMode dptScloMode;
    QCOMPARE(dptScloMode.size(), 1);
    QCOMPARE(dptScloMode.mainType(), 20);
    QCOMPARE(dptScloMode.subType(), 1);
    QCOMPARE(dptScloMode.isValid(), true);
    QCOMPARE(dptScloMode.mode(), QKnxScloMode::Mode::Autonomous);
    QCOMPARE(dptScloMode.setMode(QKnxScloMode::Mode::Master), true);
    QCOMPARE(dptScloMode.mode(), QKnxScloMode::Mode::Master);
    QCOMPARE(dptScloMode.value(), quint8(2));
    QCOMPARE(dptScloMode.setMode(QKnxScloMode::Mode(255)), false);
    QCOMPARE(dptScloMode.mode(), QKnxScloMode::Mode::Master);
    QCOMPARE(dptScloMode.value(), quint8(2));
    QCOMPARE(dptScloMode.isValid(), true);
    QCOMPARE(dptScloMode.setValue(255), true);
    QCOMPARE(dptScloMode.value(), quint8(255));
    QCOMPARE(dptScloMode.mode(), QKnxScloMode::Mode::Invalid);
    QCOMPARE(dptScloMode.setMode(QKnxScloMode::Mode::Autonomous), true);
    QCOMPARE(dptScloMode.isValid(), true);
    QCOMPARE(dptScloMode.mode(), QKnxScloMode::Mode::Autonomous);
    QCOMPARE(dptScloMode.value(), quint8(0));
    QCOMPARE(dptScloMode.setMode(QKnxScloMode::Mode::Slave), true);
    QCOMPARE(dptScloMode.isValid(), true);
    QCOMPARE(dptScloMode.mode(), QKnxScloMode::Mode::Slave);
    QCOMPARE(dptScloMode.value(), quint8(1));
    QCOMPARE(dptScloMode.setMode(QKnxScloMode::Mode::Master), true);
    QCOMPARE(dptScloMode.isValid(), true);
    QCOMPARE(dptScloMode.mode(), QKnxScloMode::Mode::Master);
    QCOMPARE(dptScloMode.value(), quint8(2));

    QKnxBuildingMode dptBuildingMode;
    QCOMPARE(dptBuildingMode.size(), 1);
    QCOMPARE(dptBuildingMode.mainType(), 20);
    QCOMPARE(dptBuildingMode.subType(), 2);
    QCOMPARE(dptBuildingMode.isValid(), true);
    QCOMPARE(dptBuildingMode.mode(), QKnxBuildingMode::Mode::BuildingInUse);
    QCOMPARE(dptBuildingMode.setMode(QKnxBuildingMode::Mode::BuildingNotUsed), true);
    QCOMPARE(dptBuildingMode.mode(), QKnxBuildingMode::Mode::BuildingNotUsed);
    QCOMPARE(dptBuildingMode.value(), quint8(1));
    QCOMPARE(dptBuildingMode.isValid(), true);
    QCOMPARE(dptBuildingMode.setMode(QKnxBuildingMode::Mode(255)), false);
    QCOMPARE(dptBuildingMode.mode(), QKnxBuildingMode::Mode::BuildingNotUsed);
    QCOMPARE(dptBuildingMode.value(), quint8(1));
    QCOMPARE(dptBuildingMode.setValue(255), true);
    QCOMPARE(dptBuildingMode.value(), quint8(255));
    QCOMPARE(dptBuildingMode.mode(), QKnxBuildingMode::Mode::Invalid);
    QCOMPARE(dptBuildingMode.setMode(QKnxBuildingMode::Mode::BuildingInUse), true);
    QCOMPARE(dptBuildingMode.isValid(), true);
    QCOMPARE(dptBuildingMode.mode(), QKnxBuildingMode::Mode::BuildingInUse);
    QCOMPARE(dptBuildingMode.value(), quint8(0));
    QCOMPARE(dptBuildingMode.setMode(QKnxBuildingMode::Mode::BuildingNotUsed), true);
    QCOMPARE(dptBuildingMode.isValid(), true);
    QCOMPARE(dptBuildingMode.mode(), QKnxBuildingMode::Mode::BuildingNotUsed);
    QCOMPARE(dptBuildingMode.value(), quint8(1));
    QCOMPARE(dptBuildingMode.setMode(QKnxBuildingMode::Mode::BuildingProtection), true);
    QCOMPARE(dptBuildingMode.isValid(), true);
    QCOMPARE(dptBuildingMode.mode(), QKnxBuildingMode::Mode::BuildingProtection);
    QCOMPARE(dptBuildingMode.value(), quint8(2));

    QKnxBuildingMode dptBuildingModeBis(QKnxBuildingMode::Mode::BuildingNotUsed);
    QCOMPARE(dptBuildingModeBis.isValid(), true);
    QCOMPARE(dptBuildingModeBis.mode(), QKnxBuildingMode::Mode::BuildingNotUsed);
    QCOMPARE(dptBuildingModeBis.value(), quint8(1));

    QKnxOccupyMode dptOccMode;
    QCOMPARE(dptOccMode.size(), 1);
    QCOMPARE(dptOccMode.mainType(), 20);
    QCOMPARE(dptOccMode.subType(), 3);
    QCOMPARE(dptOccMode.isValid(), true);
    QCOMPARE(dptOccMode.mode(), QKnxOccupyMode::Mode::Occupied);
    QCOMPARE(dptOccMode.setMode(QKnxOccupyMode::Mode::NotOccupied), true);
    QCOMPARE(dptOccMode.mode(), QKnxOccupyMode::Mode::NotOccupied);
    QCOMPARE(dptOccMode.value(), quint8(2));
    QCOMPARE(dptOccMode.isValid(), true);
    QCOMPARE(dptOccMode.setMode(QKnxOccupyMode::Mode(255)), false);
    QCOMPARE(dptOccMode.mode(), QKnxOccupyMode::Mode::NotOccupied);
    QCOMPARE(dptOccMode.value(), quint8(2));
    QCOMPARE(dptOccMode.setValue(255), true);
    QCOMPARE(dptOccMode.value(), quint8(255));
    QCOMPARE(dptOccMode.isValid(), false);
    QCOMPARE(dptOccMode.mode(), QKnxOccupyMode::Mode::Invalid);
    QCOMPARE(dptOccMode.setMode(QKnxOccupyMode::Mode::Occupied), true);
    QCOMPARE(dptOccMode.isValid(), true);
    QCOMPARE(dptOccMode.mode(), QKnxOccupyMode::Mode::Occupied);
    QCOMPARE(dptOccMode.value(), quint8(0));
    dptOccMode.setMode(QKnxOccupyMode::Mode::Standby);
    QCOMPARE(dptOccMode.isValid(), true);
    QCOMPARE(dptOccMode.mode(), QKnxOccupyMode::Mode::Standby);
    QCOMPARE(dptOccMode.value(), quint8(1));
    dptOccMode.setMode(QKnxOccupyMode::Mode::NotOccupied);
    QCOMPARE(dptOccMode.isValid(), true);
    QCOMPARE(dptOccMode.mode(), QKnxOccupyMode::Mode::NotOccupied);
    QCOMPARE(dptOccMode.value(), quint8(2));

    QKnxOccupyMode dptOccModeBis(QKnxOccupyMode::Mode::Standby);
    QCOMPARE(dptOccModeBis.isValid(), true);
    QCOMPARE(dptOccModeBis.mode(), QKnxOccupyMode::Mode::Standby);
    QCOMPARE(dptOccModeBis.value(), quint8(1));

    QKnxPriority dptPriority;
    QCOMPARE(dptPriority.size(), 1);
    QCOMPARE(dptPriority.mainType(), 20);
    QCOMPARE(dptPriority.subType(), 4);
    QCOMPARE(dptPriority.isValid(), true);
    QCOMPARE(dptPriority.priority(), QKnxPriority::Priority::High);
    dptPriority.setPriority(QKnxPriority::Priority::Low);
    QCOMPARE(dptPriority.priority(), QKnxPriority::Priority::Low);
    QCOMPARE(dptPriority.value(), quint8(2));
    QCOMPARE(dptPriority.isValid(), true);
    dptPriority.setPriority(QKnxPriority::Priority(255));
    QCOMPARE(dptPriority.priority(), QKnxPriority::Priority::Low);
    QCOMPARE(dptPriority.value(), quint8(2));
    dptPriority.setValue(255);
    QCOMPARE(dptPriority.isValid(), false);
    QCOMPARE(dptPriority.value(), quint8(255));
    QCOMPARE(dptPriority.priority(), QKnxPriority::Priority::Invalid);
    dptPriority.setPriority(QKnxPriority::Priority::High);
    QCOMPARE(dptPriority.isValid(), true);
    QCOMPARE(dptPriority.priority(), QKnxPriority::Priority::High);
    QCOMPARE(dptPriority.value(), quint8(0));
    dptPriority.setPriority(QKnxPriority::Priority::Medium);
    QCOMPARE(dptPriority.isValid(), true);
    QCOMPARE(dptPriority.priority(), QKnxPriority::Priority::Medium);
    QCOMPARE(dptPriority.value(), quint8(1));
    dptPriority.setPriority(QKnxPriority::Priority::Low);
    QCOMPARE(dptPriority.isValid(), true);
    QCOMPARE(dptPriority.priority(), QKnxPriority::Priority::Low);
    QCOMPARE(dptPriority.value(), quint8(2));
    dptPriority.setPriority(QKnxPriority::Priority::Void);
    QCOMPARE(dptPriority.isValid(), true);
    QCOMPARE(dptPriority.priority(), QKnxPriority::Priority::Void);
    QCOMPARE(dptPriority.value(), quint8(3));

    QKnxPriority dptPriorityBis(QKnxPriority::Priority::Medium);
    QCOMPARE(dptPriorityBis.isValid(), true);
    QCOMPARE(dptPriorityBis.priority(), QKnxPriority::Priority::Medium);
    QCOMPARE(dptPriorityBis.value(), quint8(1));

    QKnxLightApplicationMode dptLA;
    QCOMPARE(dptLA.size(), 1);
    QCOMPARE(dptLA.mainType(), 20);
    QCOMPARE(dptLA.subType(), 5);
    QCOMPARE(dptLA.isValid(), true);
    QCOMPARE(dptLA.mode(), QKnxLightApplicationMode::Mode::Normal);
    dptLA.setMode(QKnxLightApplicationMode::Mode::NightRound);
    QCOMPARE(dptLA.mode(), QKnxLightApplicationMode::Mode::NightRound);
    QCOMPARE(dptLA.value(), quint8(2));
    QCOMPARE(dptLA.isValid(), true);
    dptLA.setMode(QKnxLightApplicationMode::Mode(255));
    QCOMPARE(dptLA.mode(), QKnxLightApplicationMode::Mode::NightRound);
    QCOMPARE(dptLA.value(), quint8(2));
    dptLA.setValue(255);
    QCOMPARE(dptLA.value(), quint8(255));
    QCOMPARE(dptLA.mode(), QKnxLightApplicationMode::Mode::Invalid);
    dptLA.setMode(QKnxLightApplicationMode::Mode::Normal);
    QCOMPARE(dptLA.isValid(), true);
    QCOMPARE(dptLA.value(), quint8(0));
    QCOMPARE(dptLA.mode(), QKnxLightApplicationMode::Mode::Normal);
    dptLA.setMode(QKnxLightApplicationMode::Mode::PresenceSimulation);
    QCOMPARE(dptLA.isValid(), true);
    QCOMPARE(dptLA.mode(), QKnxLightApplicationMode::Mode::PresenceSimulation);
    QCOMPARE(dptLA.value(), quint8(1));
    dptLA.setMode(QKnxLightApplicationMode::Mode::NightRound);
    QCOMPARE(dptLA.isValid(), true);
    QCOMPARE(dptLA.mode(), QKnxLightApplicationMode::Mode::NightRound);
    QCOMPARE(dptLA.value(), quint8(2));

    QKnxLightApplicationMode dptLABis(QKnxLightApplicationMode::Mode::PresenceSimulation);
    QCOMPARE(dptLABis.isValid(), true);
    QCOMPARE(dptLABis.mode(), QKnxLightApplicationMode::Mode::PresenceSimulation);
    QCOMPARE(dptLABis.value(), quint8(1));

    QKnxApplicationArea dptArea;
    QCOMPARE(dptArea.size(), 1);
    QCOMPARE(dptArea.mainType(), 20);
    QCOMPARE(dptArea.subType(), 6);
    QCOMPARE(dptArea.isValid(), true);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::NoFault);
    dptArea.setArea(QKnxApplicationArea::Area::SystemFunctionCommonInterest);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::SystemFunctionCommonInterest);
    QCOMPARE(dptArea.value(), quint8(1));
    QCOMPARE(dptArea.isValid(), true);
    dptArea.setArea(QKnxApplicationArea::Area::HvacGeneralFBs);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::HvacGeneralFBs);
    QCOMPARE(dptArea.value(), quint8(10));
    QCOMPARE(dptArea.isValid(), true);
    dptArea.setArea(QKnxApplicationArea::Area::HvacHotWaterHeating);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::HvacHotWaterHeating);
    QCOMPARE(dptArea.value(), quint8(11));
    QCOMPARE(dptArea.isValid(), true);
    dptArea.setArea(QKnxApplicationArea::Area::HvacDirectElectricalHeating);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::HvacDirectElectricalHeating);
    QCOMPARE(dptArea.value(), quint8(12));
    QCOMPARE(dptArea.isValid(), true);
    dptArea.setArea(QKnxApplicationArea::Area::HvacTerminalUnits);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::HvacTerminalUnits);
    QCOMPARE(dptArea.value(), quint8(13));
    QCOMPARE(dptArea.isValid(), true);
    dptArea.setArea(QKnxApplicationArea::Area::HvacVac);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::HvacVac);
    QCOMPARE(dptArea.value(), quint8(14));
    QCOMPARE(dptArea.isValid(), true);
    dptArea.setArea(QKnxApplicationArea::Area::Lighting);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Lighting);
    QCOMPARE(dptArea.value(), quint8(20));
    QCOMPARE(dptArea.isValid(), true);
    dptArea.setArea(QKnxApplicationArea::Area::Security);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Security);
    QCOMPARE(dptArea.value(), quint8(30));
    QCOMPARE(dptArea.isValid(), true);
    dptArea.setArea(QKnxApplicationArea::Area::LoadManagement);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::LoadManagement);
    QCOMPARE(dptArea.value(), quint8(40));
    QCOMPARE(dptArea.isValid(), true);
    dptArea.setArea(QKnxApplicationArea::Area::ShuttersAndBlinds);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::ShuttersAndBlinds);
    QCOMPARE(dptArea.value(), quint8(50));
    dptArea.setValue(2);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(2));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(9);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(9));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(15);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(15));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(19);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(19));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(21);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(21));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(29);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(29));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(31);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(31));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(39);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(39));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(41);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(41));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(49);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(49));
    QCOMPARE(dptArea.isValid(), false);
    dptArea.setValue(255);
    QCOMPARE(dptArea.area(), QKnxApplicationArea::Area::Invalid);
    QCOMPARE(dptArea.value(), quint8(255));
    QCOMPARE(dptArea.isValid(), false);

    QKnxApplicationArea dptAreaBis(QKnxApplicationArea::Area::Lighting);
    QCOMPARE(dptAreaBis.isValid(), true);
    QCOMPARE(dptAreaBis.area(), QKnxApplicationArea::Area::Lighting);
    QCOMPARE(dptAreaBis.value(), quint8(20));

    QKnxAlarmClassType dptAlarm;
    QCOMPARE(dptAlarm.size(), 1);
    QCOMPARE(dptAlarm.mainType(), 20);
    QCOMPARE(dptAlarm.subType(), 7);
    QCOMPARE(dptAlarm.isValid(), true);
    QCOMPARE(dptAlarm.type(), QKnxAlarmClassType::Type::SimpleAlarm);
    QCOMPARE(dptAlarm.value(), quint8(1));
    dptAlarm.setType(QKnxAlarmClassType::Type::BasicAlarm);
    QCOMPARE(dptAlarm.type(), QKnxAlarmClassType::Type::BasicAlarm);
    QCOMPARE(dptAlarm.value(), quint8(2));
    QCOMPARE(dptAlarm.isValid(), true);
    dptAlarm.setType(QKnxAlarmClassType::Type::ExtendedAlarm);
    QCOMPARE(dptAlarm.type(), QKnxAlarmClassType::Type::ExtendedAlarm);
    QCOMPARE(dptAlarm.value(), quint8(3));
    QCOMPARE(dptAlarm.isValid(), true);
    dptAlarm.setType(QKnxAlarmClassType::Type::Invalid);
    QCOMPARE(dptAlarm.type(), QKnxAlarmClassType::Type::ExtendedAlarm);
    QCOMPARE(dptAlarm.value(), quint8(3));
    QCOMPARE(dptAlarm.isValid(), true);
    dptAlarm.setValue(255);
    QCOMPARE(dptAlarm.isValid(), false);
    QCOMPARE(dptAlarm.type(), QKnxAlarmClassType::Type::Invalid);
    QCOMPARE(dptAlarm.value(), quint8(255));
    dptAlarm.setValue(0);
    QCOMPARE(dptAlarm.isValid(), false);
    QCOMPARE(dptAlarm.value(), quint8(0));
    QCOMPARE(dptAlarm.type(), QKnxAlarmClassType::Type::Invalid);
    dptAlarm.setValue(2);
    QCOMPARE(dptAlarm.type(), QKnxAlarmClassType::Type::BasicAlarm);
    QCOMPARE(dptAlarm.value(), quint8(2));
    QCOMPARE(dptAlarm.isValid(), true);

    QKnxAlarmClassType dptAlarmBis(QKnxAlarmClassType::Type::BasicAlarm);
    QCOMPARE(dptAlarmBis.isValid(), true);
    QCOMPARE(dptAlarmBis.type(), QKnxAlarmClassType::Type::BasicAlarm);
    QCOMPARE(dptAlarmBis.value(), quint8(2));

    QKnxPsuMode dptPsuMode;
    QCOMPARE(dptPsuMode.size(), 1);
    QCOMPARE(dptPsuMode.mainType(), 20);
    QCOMPARE(dptPsuMode.subType(), 8);
    QCOMPARE(dptPsuMode.isValid(), true);
    QCOMPARE(dptPsuMode.mode(), QKnxPsuMode::Mode::Disabled);
    QCOMPARE(dptPsuMode.value(), quint8(0));
    dptPsuMode.setMode(QKnxPsuMode::Mode(255));
    QCOMPARE(dptPsuMode.mode(), QKnxPsuMode::Mode::Disabled);
    QCOMPARE(dptPsuMode.value(), quint8(0));
    dptPsuMode.setValue(255);
    QCOMPARE(dptPsuMode.isValid(), false);
    QCOMPARE(dptPsuMode.value(), quint8(255));
    QCOMPARE(dptPsuMode.mode(), QKnxPsuMode::Mode::Invalid);
    dptPsuMode.setMode(QKnxPsuMode::Mode::Disabled);
    QCOMPARE(dptPsuMode.isValid(), true);
    QCOMPARE(dptPsuMode.mode(), QKnxPsuMode::Mode::Disabled);
    QCOMPARE(dptPsuMode.value(), quint8(0));
    dptPsuMode.setMode(QKnxPsuMode::Mode::Enabled);
    QCOMPARE(dptPsuMode.isValid(), true);
    QCOMPARE(dptPsuMode.mode(), QKnxPsuMode::Mode::Enabled);
    QCOMPARE(dptPsuMode.value(), quint8(1));
    dptPsuMode.setMode(QKnxPsuMode::Mode::Automatic);
    QCOMPARE(dptPsuMode.isValid(), true);
    QCOMPARE(dptPsuMode.mode(), QKnxPsuMode::Mode::Automatic);
    QCOMPARE(dptPsuMode.value(), quint8(2));

    QKnxPsuMode dptPsuModeBis(QKnxPsuMode::Mode::Enabled);
    QCOMPARE(dptPsuModeBis.isValid(), true);
    QCOMPARE(dptPsuModeBis.mode(), QKnxPsuMode::Mode::Enabled);
    QCOMPARE(dptPsuModeBis.value(), quint8(1));

    QKnxErrorClassSystem dptError;
    QCOMPARE(dptError.size(), 1);
    QCOMPARE(dptError.mainType(), 20);
    QCOMPARE(dptError.subType(), 11);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::NoFault);
    QCOMPARE(dptError.value(), quint8(0));
    dptError.setError(QKnxErrorClassSystem::Error(255));
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::NoFault);
    QCOMPARE(dptError.value(), quint8(0));
    dptError.setValue(255);
    QCOMPARE(dptError.isValid(), false);
    QCOMPARE(dptError.value(), quint8(255));
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::Invalid);
    dptError.setError(QKnxErrorClassSystem::Error::NoFault);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::NoFault);
    QCOMPARE(dptError.value(), quint8(0));
    dptError.setError(QKnxErrorClassSystem::Error::GeneralDeviceFault);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::GeneralDeviceFault);
    QCOMPARE(dptError.value(), quint8(1));
    dptError.setError(QKnxErrorClassSystem::Error::CommunicationFault);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::CommunicationFault);
    QCOMPARE(dptError.value(), quint8(2));
    dptError.setError(QKnxErrorClassSystem::Error::ConfigurationFault);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::ConfigurationFault);
    QCOMPARE(dptError.value(), quint8(3));
    dptError.setError(QKnxErrorClassSystem::Error::HardwareFault);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::HardwareFault);
    QCOMPARE(dptError.value(), quint8(4));
    dptError.setError(QKnxErrorClassSystem::Error::SoftwareFault);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::SoftwareFault);
    QCOMPARE(dptError.value(), quint8(5));
    dptError.setError(QKnxErrorClassSystem::Error::InsufficientNonVolatileMemory);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::InsufficientNonVolatileMemory);
    QCOMPARE(dptError.value(), quint8(6));
    dptError.setError(QKnxErrorClassSystem::Error::InsufficientVolatileMemory);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::InsufficientVolatileMemory);
    QCOMPARE(dptError.value(), quint8(7));
    dptError.setError(QKnxErrorClassSystem::Error::MemoryAllocationCommandWithSize0Received);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::MemoryAllocationCommandWithSize0Received);
    QCOMPARE(dptError.value(), quint8(8));
    dptError.setError(QKnxErrorClassSystem::Error::CrcError);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::CrcError);
    QCOMPARE(dptError.value(), quint8(9));
    dptError.setError(QKnxErrorClassSystem::Error::WatchdogResetDetected);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::WatchdogResetDetected);
    QCOMPARE(dptError.value(), quint8(10));
    dptError.setError(QKnxErrorClassSystem::Error::InvalidOpCodeDetected);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::InvalidOpCodeDetected);
    QCOMPARE(dptError.value(), quint8(11));
    dptError.setError(QKnxErrorClassSystem::Error::GeneralProtectionFault);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::GeneralProtectionFault);
    QCOMPARE(dptError.value(), quint8(12));
    dptError.setError(QKnxErrorClassSystem::Error::MaximalTableLengthExceeded);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::MaximalTableLengthExceeded);
    QCOMPARE(dptError.value(), quint8(13));
    dptError.setError(QKnxErrorClassSystem::Error::UndefinedLoadCommandReceived);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::UndefinedLoadCommandReceived);
    QCOMPARE(dptError.value(), quint8(14));
    dptError.setError(QKnxErrorClassSystem::Error::GroupAddressTableIsNotSorted);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::GroupAddressTableIsNotSorted);
    QCOMPARE(dptError.value(), quint8(15));
    dptError.setError(QKnxErrorClassSystem::Error::InvalidConnectionNumberTsap);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::InvalidConnectionNumberTsap);
    QCOMPARE(dptError.value(), quint8(16));
    dptError.setError(QKnxErrorClassSystem::Error::InvalidGroupObjectNumberAsap);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::InvalidGroupObjectNumberAsap);
    QCOMPARE(dptError.value(), quint8(17));
    dptError.setError(QKnxErrorClassSystem::Error::GroupObjectTypeExceeds);
    QCOMPARE(dptError.isValid(), true);
    QCOMPARE(dptError.error(), QKnxErrorClassSystem::Error::GroupObjectTypeExceeds);
    QCOMPARE(dptError.value(), quint8(18));

    QKnxErrorClassSystem dptErrorBis(QKnxErrorClassSystem::Error::GeneralDeviceFault);
    QCOMPARE(dptErrorBis.isValid(), true);
    QCOMPARE(dptErrorBis.error(), QKnxErrorClassSystem::Error::GeneralDeviceFault);
    QCOMPARE(dptErrorBis.value(), quint8(1));

    QKnxErrorClassHvac dptErrorHvac;
    QCOMPARE(dptErrorHvac.size(), 1);
    QCOMPARE(dptErrorHvac.mainType(), 20);
    QCOMPARE(dptErrorHvac.subType(), 12);
    QCOMPARE(dptErrorHvac.isValid(), true);
    QCOMPARE(dptErrorHvac.error(), QKnxErrorClassHvac::Error::NoFault);
    QCOMPARE(dptErrorHvac.value(), quint8(0));
    dptErrorHvac.setError(QKnxErrorClassHvac::Error(255));
    QCOMPARE(dptErrorHvac.error(), QKnxErrorClassHvac::Error::NoFault);
    QCOMPARE(dptErrorHvac.value(), quint8(0));
    dptErrorHvac.setValue(255);
    QCOMPARE(dptErrorHvac.isValid(), false);
    QCOMPARE(dptErrorHvac.value(), quint8(255));
    QCOMPARE(dptErrorHvac.error(), QKnxErrorClassHvac::Error::Invalid);
    dptErrorHvac.setError(QKnxErrorClassHvac::Error::NoFault);
    QCOMPARE(dptErrorHvac.isValid(), true);
    QCOMPARE(dptErrorHvac.error(), QKnxErrorClassHvac::Error::NoFault);
    QCOMPARE(dptErrorHvac.value(), quint8(0));
    dptErrorHvac.setError(QKnxErrorClassHvac::Error::SensorFault);
    QCOMPARE(dptErrorHvac.isValid(), true);
    QCOMPARE(dptErrorHvac.error(), QKnxErrorClassHvac::Error::SensorFault);
    QCOMPARE(dptErrorHvac.value(), quint8(1));
    dptErrorHvac.setError(QKnxErrorClassHvac::Error::ProcessControllerFault);
    QCOMPARE(dptErrorHvac.isValid(), true);
    QCOMPARE(dptErrorHvac.error(), QKnxErrorClassHvac::Error::ProcessControllerFault);
    QCOMPARE(dptErrorHvac.value(), quint8(2));
    dptErrorHvac.setError(QKnxErrorClassHvac::Error::ActuatorFault);
    QCOMPARE(dptErrorHvac.isValid(), true);
    QCOMPARE(dptErrorHvac.error(), QKnxErrorClassHvac::Error::ActuatorFault);
    QCOMPARE(dptErrorHvac.value(), quint8(3));
    dptErrorHvac.setError(QKnxErrorClassHvac::Error::OtherFault);
    QCOMPARE(dptErrorHvac.isValid(), true);
    QCOMPARE(dptErrorHvac.error(), QKnxErrorClassHvac::Error::OtherFault);
    QCOMPARE(dptErrorHvac.value(), quint8(4));

    QKnxErrorClassHvac dptErrorHvacBis(QKnxErrorClassHvac::Error::SensorFault);
    QCOMPARE(dptErrorHvacBis.isValid(), true);
    QCOMPARE(dptErrorHvacBis.error(), QKnxErrorClassHvac::Error::SensorFault);
    QCOMPARE(dptErrorHvacBis.value(), quint8(1));

    QKnxTimeDelay dptDelay;
    QCOMPARE(dptDelay.size(), 1);
    QCOMPARE(dptDelay.mainType(), 20);
    QCOMPARE(dptDelay.subType(), 13);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::NotActive);
    QCOMPARE(dptDelay.value(), quint8(0));
    dptDelay.setDelay(QKnxTimeDelay::Delay(255));
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::NotActive);
    QCOMPARE(dptDelay.value(), quint8(0));
    dptDelay.setValue(255);
    QCOMPARE(dptDelay.isValid(), false);
    QCOMPARE(dptDelay.value(), quint8(255));
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::Invalid);
    dptDelay.setDelay(QKnxTimeDelay::Delay::NotActive);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::NotActive);
    QCOMPARE(dptDelay.value(), quint8(0));
    dptDelay.setDelay(QKnxTimeDelay::Delay::OneSecond);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::OneSecond);
    QCOMPARE(dptDelay.value(), quint8(1));
    dptDelay.setDelay(QKnxTimeDelay::Delay::TwoSecond);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::TwoSecond);
    QCOMPARE(dptDelay.value(), quint8(2));
    dptDelay.setDelay(QKnxTimeDelay::Delay::ThreeSecond);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::ThreeSecond);
    QCOMPARE(dptDelay.value(), quint8(3));
    dptDelay.setDelay(QKnxTimeDelay::Delay::FiveSecond);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::FiveSecond);
    QCOMPARE(dptDelay.value(), quint8(4));
    dptDelay.setDelay(QKnxTimeDelay::Delay::TenSecond);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::TenSecond);
    QCOMPARE(dptDelay.value(), quint8(5));
    dptDelay.setDelay(QKnxTimeDelay::Delay::FifteenSecond);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::FifteenSecond);
    QCOMPARE(dptDelay.value(), quint8(6));
    dptDelay.setDelay(QKnxTimeDelay::Delay::TwentySecond);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::TwentySecond);
    QCOMPARE(dptDelay.value(), quint8(7));
    dptDelay.setDelay(QKnxTimeDelay::Delay::ThirtySecond);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::ThirtySecond);
    QCOMPARE(dptDelay.value(), quint8(8));
    dptDelay.setDelay(QKnxTimeDelay::Delay::FortyFiveSecond);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::FortyFiveSecond);
    QCOMPARE(dptDelay.value(), quint8(9));
    dptDelay.setDelay(QKnxTimeDelay::Delay::OneMinute);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::OneMinute);
    QCOMPARE(dptDelay.value(), quint8(10));
    dptDelay.setDelay(QKnxTimeDelay::Delay::OneMinuteOneQuarter);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::OneMinuteOneQuarter);
    QCOMPARE(dptDelay.value(), quint8(11));
    dptDelay.setDelay(QKnxTimeDelay::Delay::OneMinuteAndHaft);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::OneMinuteAndHaft);
    QCOMPARE(dptDelay.value(), quint8(12));
    dptDelay.setDelay(QKnxTimeDelay::Delay::TwoMinute);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::TwoMinute);
    QCOMPARE(dptDelay.value(), quint8(13));
    dptDelay.setDelay(QKnxTimeDelay::Delay::TwoMinuteAndHalf);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::TwoMinuteAndHalf);
    QCOMPARE(dptDelay.value(), quint8(14));
    dptDelay.setDelay(QKnxTimeDelay::Delay::ThreeMinute);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::ThreeMinute);
    QCOMPARE(dptDelay.value(), quint8(15));
    dptDelay.setDelay(QKnxTimeDelay::Delay::FiveMinute);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::FiveMinute);
    QCOMPARE(dptDelay.value(), quint8(16));
    dptDelay.setDelay(QKnxTimeDelay::Delay::FifteenMinute);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::FifteenMinute);
    QCOMPARE(dptDelay.value(), quint8(17));
    dptDelay.setDelay(QKnxTimeDelay::Delay::TwentyMinute);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::TwentyMinute);
    QCOMPARE(dptDelay.value(), quint8(18));
    dptDelay.setDelay(QKnxTimeDelay::Delay::ThirtyMinute);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::ThirtyMinute);
    QCOMPARE(dptDelay.value(), quint8(19));
    dptDelay.setDelay(QKnxTimeDelay::Delay::OneHour);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::OneHour);
    QCOMPARE(dptDelay.value(), quint8(20));
    dptDelay.setDelay(QKnxTimeDelay::Delay::TwoHour);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::TwoHour);
    QCOMPARE(dptDelay.value(), quint8(21));
    dptDelay.setDelay(QKnxTimeDelay::Delay::ThreeHour);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::ThreeHour);
    QCOMPARE(dptDelay.value(), quint8(22));
    dptDelay.setDelay(QKnxTimeDelay::Delay::FiveHour);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::FiveHour);
    QCOMPARE(dptDelay.value(), quint8(23));
    dptDelay.setDelay(QKnxTimeDelay::Delay::TwelveHour);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::TwelveHour);
    QCOMPARE(dptDelay.value(), quint8(24));
    dptDelay.setDelay(QKnxTimeDelay::Delay::TwentyFourHour);
    QCOMPARE(dptDelay.isValid(), true);
    QCOMPARE(dptDelay.delay(), QKnxTimeDelay::Delay::TwentyFourHour);
    QCOMPARE(dptDelay.value(), quint8(25));

    QKnxTimeDelay dptDelayBis(QKnxTimeDelay::Delay::OneSecond);
    QCOMPARE(dptDelayBis.isValid(), true);
    QCOMPARE(dptDelayBis.delay(), QKnxTimeDelay::Delay::OneSecond);
    QCOMPARE(dptDelayBis.value(), quint8(1));

    QKnxBeaufortWindForceScale dptWind;
    QCOMPARE(dptWind.size(), 1);
    QCOMPARE(dptWind.mainType(), 20);
    QCOMPARE(dptWind.subType(), 14);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::CalmNoWind);
    QCOMPARE(dptWind.value(), quint8(0));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force(255));
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::CalmNoWind);
    QCOMPARE(dptWind.value(), quint8(0));
    dptWind.setValue(255);
    QCOMPARE(dptWind.isValid(), false);
    QCOMPARE(dptWind.value(), quint8(255));
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::Invalid);
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::CalmNoWind);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::CalmNoWind);
    QCOMPARE(dptWind.value(), quint8(0));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::LightAir);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::LightAir);
    QCOMPARE(dptWind.value(), quint8(1));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::LightBreeze);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::LightBreeze);
    QCOMPARE(dptWind.value(), quint8(2));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::GentleBreeze);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::GentleBreeze);
    QCOMPARE(dptWind.value(), quint8(3));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::ModerateBreeze);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::ModerateBreeze);
    QCOMPARE(dptWind.value(), quint8(4));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::FreshBreeze);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::FreshBreeze);
    QCOMPARE(dptWind.value(), quint8(5));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::StrongBreeze);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::StrongBreeze);
    QCOMPARE(dptWind.value(), quint8(6));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::NearModerateGale);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::NearModerateGale);
    QCOMPARE(dptWind.value(), quint8(7));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::FreshGale);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::FreshGale);
    QCOMPARE(dptWind.value(), quint8(8));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::StrongGale);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::StrongGale);
    QCOMPARE(dptWind.value(), quint8(9));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::WholeGaleStorm);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::WholeGaleStorm);
    QCOMPARE(dptWind.value(), quint8(10));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::ViolentStorm);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::ViolentStorm);
    QCOMPARE(dptWind.value(), quint8(11));
    dptWind.setForce(QKnxBeaufortWindForceScale::Force::Hurricane);
    QCOMPARE(dptWind.isValid(), true);
    QCOMPARE(dptWind.force(), QKnxBeaufortWindForceScale::Force::Hurricane);
    QCOMPARE(dptWind.value(), quint8(12));

    QKnxBeaufortWindForceScale dptWindBis(QKnxBeaufortWindForceScale::Force::LightAir);
    QCOMPARE(dptWindBis.isValid(), true);
    QCOMPARE(dptWindBis.force(), QKnxBeaufortWindForceScale::Force::LightAir);
    QCOMPARE(dptWindBis.value(), quint8(1));

    QKnxSensorSelect dptSensor;
    QCOMPARE(dptSensor.size(), 1);
    QCOMPARE(dptSensor.mainType(), 20);
    QCOMPARE(dptSensor.subType(), 17);
    QCOMPARE(dptSensor.isValid(), true);
    QCOMPARE(dptSensor.mode(), QKnxSensorSelect::Mode::Inactive);
    QCOMPARE(dptSensor.value(), quint8(0));
    dptSensor.setMode(QKnxSensorSelect::Mode(255));
    QCOMPARE(dptSensor.mode(), QKnxSensorSelect::Mode::Inactive);
    QCOMPARE(dptSensor.value(), quint8(0));
    dptSensor.setValue(255);
    QCOMPARE(dptSensor.isValid(), false);
    QCOMPARE(dptSensor.value(), quint8(255));
    QCOMPARE(dptSensor.mode(), QKnxSensorSelect::Mode::Invalid);
    dptSensor.setMode(QKnxSensorSelect::Mode::Inactive);
    QCOMPARE(dptSensor.isValid(), true);
    QCOMPARE(dptSensor.mode(), QKnxSensorSelect::Mode::Inactive);
    QCOMPARE(dptSensor.value(), quint8(0));
    dptSensor.setMode(QKnxSensorSelect::Mode::DigitalInputNotInverted);
    QCOMPARE(dptSensor.isValid(), true);
    QCOMPARE(dptSensor.mode(), QKnxSensorSelect::Mode::DigitalInputNotInverted);
    QCOMPARE(dptSensor.value(), quint8(1));
    dptSensor.setMode(QKnxSensorSelect::Mode::DigitalInputInverted);
    QCOMPARE(dptSensor.isValid(), true);
    QCOMPARE(dptSensor.mode(), QKnxSensorSelect::Mode::DigitalInputInverted);
    QCOMPARE(dptSensor.value(), quint8(2));
    dptSensor.setMode(QKnxSensorSelect::Mode::AnalogInput);
    QCOMPARE(dptSensor.isValid(), true);
    QCOMPARE(dptSensor.mode(), QKnxSensorSelect::Mode::AnalogInput);
    QCOMPARE(dptSensor.value(), quint8(3));
    dptSensor.setMode(QKnxSensorSelect::Mode::TemperatureSensorInput);
    QCOMPARE(dptSensor.isValid(), true);
    QCOMPARE(dptSensor.mode(), QKnxSensorSelect::Mode::TemperatureSensorInput);
    QCOMPARE(dptSensor.value(), quint8(4));

    QKnxSensorSelect dptSensorBis(QKnxSensorSelect::Mode::DigitalInputNotInverted);
    QCOMPARE(dptSensorBis.isValid(), true);
    QCOMPARE(dptSensorBis.mode(), QKnxSensorSelect::Mode::DigitalInputNotInverted);
    QCOMPARE(dptSensorBis.value(), quint8(1));

    QKnxActuatorConnectType dptConnect;
    QCOMPARE(dptConnect.size(), 1);
    QCOMPARE(dptConnect.mainType(), 20);
    QCOMPARE(dptConnect.subType(), 20);
    QCOMPARE(dptConnect.isValid(), true);
    QCOMPARE(dptConnect.type(), QKnxActuatorConnectType::Type::SensorConnection);
    QCOMPARE(dptConnect.value(), quint8(1));
    dptConnect.setType(QKnxActuatorConnectType::Type::ControllerConnection);
    QCOMPARE(dptConnect.type(), QKnxActuatorConnectType::Type::ControllerConnection);
    QCOMPARE(dptConnect.value(), quint8(2));
    QCOMPARE(dptConnect.isValid(), true);
    dptConnect.setType(QKnxActuatorConnectType::Type::Invalid);
    QCOMPARE(dptConnect.type(), QKnxActuatorConnectType::Type::ControllerConnection);
    QCOMPARE(dptConnect.value(), quint8(2));
    QCOMPARE(dptConnect.isValid(), true);
    dptConnect.setValue(255);
    QCOMPARE(dptConnect.isValid(), false);
    QCOMPARE(dptConnect.value(), quint8(255));
    QCOMPARE(dptConnect.type(), QKnxActuatorConnectType::Type::Invalid);
    dptConnect.setType(QKnxActuatorConnectType::Type::SensorConnection);
    QCOMPARE(dptConnect.type(), QKnxActuatorConnectType::Type::SensorConnection);
    QCOMPARE(dptConnect.value(), quint8(1));
    QCOMPARE(dptConnect.isValid(), true);
    dptConnect.setValue(0);
    QCOMPARE(dptConnect.isValid(), false);
    QCOMPARE(dptConnect.value(), quint8(0));
    QCOMPARE(dptConnect.type(), QKnxActuatorConnectType::Type::Invalid);
    dptConnect.setValue(2);
    QCOMPARE(dptConnect.type(), QKnxActuatorConnectType::Type::ControllerConnection);
    QCOMPARE(dptConnect.value(), quint8(2));
    QCOMPARE(dptConnect.isValid(), true);

    QKnxActuatorConnectType dptConnectBis(QKnxActuatorConnectType::Type::ControllerConnection);
    QCOMPARE(dptConnectBis.isValid(), true);
    QCOMPARE(dptConnectBis.type(), QKnxActuatorConnectType::Type::ControllerConnection);
    QCOMPARE(dptConnectBis.value(), quint8(2));
}

void tst_QKnxDatapointType::dpt21_8BitSet()
{
    QKnx8BitSet dpt;
    QCOMPARE(dpt.mainType(), 0x15);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);

    QKnxGeneralStatus dptGS;
    QCOMPARE(dptGS.mainType(), 0x15);
    QCOMPARE(dptGS.subType(), 0x01);
    QCOMPARE(dptGS.isValid(), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::Fault), false);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::AlarmUnacknowledged), false);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::InAlarm), false);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::OutOfService), false);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::Overridden), false);
    dptGS.setAttribute(QKnxGeneralStatus::OutOfService);
    QCOMPARE(dptGS.isValid(), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::OutOfService), true);
    dptGS.setValue(QKnxGeneralStatus::Attributes().setFlag(QKnxGeneralStatus::Fault, true)
        .setFlag(QKnxGeneralStatus::OutOfService, false));
    dptGS.setAttribute(QKnxGeneralStatus::Overridden);
    dptGS.removeAttribute(QKnxGeneralStatus::Overridden);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::Fault), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::AlarmUnacknowledged), false);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::InAlarm), false);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::OutOfService), false);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::Overridden), false);
    QCOMPARE(dptGS.isValid(), true);
    dptGS.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(dptGS.isValid(), false);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::Fault), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::AlarmUnacknowledged), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::InAlarm), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::OutOfService), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::Overridden), true);
    dptGS.setBytes(QKnxByteArray { 0x1f }, 0, 1);
    QCOMPARE(dptGS.isValid(), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::Fault), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::AlarmUnacknowledged), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::InAlarm), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::OutOfService), true);
    QCOMPARE(dptGS.isSet(QKnxGeneralStatus::Overridden), true);

    QKnxDeviceControl dptDC;
    QCOMPARE(dptDC.mainType(), 0x15);
    QCOMPARE(dptDC.subType(), 0x02);
    QCOMPARE(dptDC.isValid(), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::UserStopped), false);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::OwnIA), false);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::VerifyMode), false);
    dptDC.setAttribute(QKnxDeviceControl::OwnIA);
    QCOMPARE(dptDC.isValid(), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::OwnIA), true);
    dptDC.removeAttribute(QKnxDeviceControl::OwnIA);
    QCOMPARE(dptDC.isValid(), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::OwnIA), false);
    dptDC.setValue(QKnxDeviceControl::Attributes().setFlag(QKnxDeviceControl::VerifyMode, true));
    QCOMPARE(dptDC.isValid(), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::VerifyMode), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::UserStopped), false);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::OwnIA), false);
    dptDC.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(dptDC.isValid(), false);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::UserStopped), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::OwnIA), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::VerifyMode), true);
    dptDC.setBytes(QKnxByteArray { 0x1f }, 0, 1);
    QCOMPARE(dptDC.isValid(), false);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::UserStopped), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::OwnIA), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::VerifyMode), true);
    dptDC.setBytes(QKnxByteArray { 0x07 }, 0, 1);
    QCOMPARE(dptDC.isValid(), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::UserStopped), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::OwnIA), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::VerifyMode), true);
    dptDC.setBytes(QKnxByteArray { 0x0f }, 0, 1);
    QCOMPARE(dptDC.isValid(), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::UserStopped), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::OwnIA), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::VerifyMode), true);
    QCOMPARE(dptDC.isSet(QKnxDeviceControl::SafeState), true);
}

void tst_QKnxDatapointType::dpt23_2BitSet()
{
    QKnx2BitSet dpt;
    QCOMPARE(dpt.mainType(), 23);
    QCOMPARE(dpt.subType(), 0);
    QCOMPARE(dpt.isValid(), true);

    QKnxOnOffAction action;
    QCOMPARE(action.mainType(), 23);
    QCOMPARE(action.subType(), 1);
    QCOMPARE(action.isValid(), true);
    QCOMPARE(action.value(), quint8(0));
    QCOMPARE(action.action(), QKnxOnOffAction::Action::Off);
    QCOMPARE(action.setAction(QKnxOnOffAction::Action::OffOn), true);
    QCOMPARE(action.isValid(), true);
    QCOMPARE(action.action(), QKnxOnOffAction::Action::OffOn);
    QCOMPARE(action.value(), quint8(2));
    action.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(action.isValid(), false);
    QCOMPARE(action.value(), quint8(3));
    QCOMPARE(action.action(), QKnxOnOffAction::Action::OnOff);
    action.setBytes(QKnxByteArray { 0x04 }, 0, 1);
    QCOMPARE(action.isValid(), false);
    QCOMPARE(action.value(), quint8(0));
    QCOMPARE(action.action(), QKnxOnOffAction::Action::Off);
    action.setBytes(QKnxByteArray { 0x03 }, 0, 1);
    QCOMPARE(action.isValid(), true);
    QCOMPARE(action.value(), quint8(3));
    QCOMPARE(action.action(), QKnxOnOffAction::Action::OnOff);

    QKnxAlarmReaction alarm;
    QCOMPARE(alarm.mainType(), 23);
    QCOMPARE(alarm.subType(), 2);
    QCOMPARE(alarm.isValid(), true);
    QCOMPARE(alarm.value(), quint8(0));
    QCOMPARE(alarm.alarm(), QKnxAlarmReaction::Alarm::NoAlarm);
    QCOMPARE(alarm.setAlarm(QKnxAlarmReaction::Alarm::AlarmDown), true);
    QCOMPARE(alarm.isValid(), true);
    QCOMPARE(alarm.alarm(), QKnxAlarmReaction::Alarm::AlarmDown);
    QCOMPARE(alarm.value(), quint8(2));
    alarm.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(alarm.isValid(), false);
    QCOMPARE(alarm.value(), quint8(3));
    QCOMPARE(alarm.alarm(), QKnxAlarmReaction::Alarm::AlarmDown);
    alarm.setBytes(QKnxByteArray { 0x03 }, 0, 1);
    QCOMPARE(alarm.isValid(), false);
    QCOMPARE(alarm.value(), quint8(3));
    QCOMPARE(alarm.alarm(), QKnxAlarmReaction::Alarm::AlarmDown);
    alarm.setBytes(QKnxByteArray { 0x02 }, 0, 1);
    QCOMPARE(alarm.isValid(), true);
    QCOMPARE(alarm.value(), quint8(2));
    QCOMPARE(alarm.alarm(), QKnxAlarmReaction::Alarm::AlarmDown);

    QKnxUpDownAction upAction;
    QCOMPARE(upAction.mainType(), 23);
    QCOMPARE(upAction.subType(), 3);
    QCOMPARE(upAction.isValid(), true);
    QCOMPARE(upAction.value(), quint8(0));
    QCOMPARE(upAction.action(), QKnxUpDownAction::Action::Up);
    QCOMPARE(upAction.setAction(QKnxUpDownAction::Action::UpDown), true);
    QCOMPARE(upAction.isValid(), true);
    QCOMPARE(upAction.action(), QKnxUpDownAction::Action::UpDown);
    QCOMPARE(upAction.value(), quint8(2));
    upAction.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(upAction.isValid(), false);
    QCOMPARE(upAction.value(), quint8(3));
    QCOMPARE(upAction.action(), QKnxUpDownAction::Action::DownUp);
    upAction.setBytes(QKnxByteArray { 0x04 }, 0, 1);
    QCOMPARE(upAction.isValid(), false);
    QCOMPARE(upAction.value(), quint8(0));
    QCOMPARE(upAction.action(), QKnxUpDownAction::Action::Up);
    upAction.setBytes(QKnxByteArray { 0x03 }, 0, 1);
    QCOMPARE(upAction.isValid(), true);
    QCOMPARE(upAction.value(), quint8(3));
    QCOMPARE(upAction.action(), QKnxUpDownAction::Action::DownUp);
}

void tst_QKnxDatapointType::dpt24_VarString()
{
    QKnxVarString string;
    QCOMPARE(string.mainType(), 24);
    QCOMPARE(string.subType(), 0);
    QCOMPARE(string.size(), 1);
    QCOMPARE(string.isValid(), true);
    QCOMPARE(string.byte(0), quint8(0));

    QCOMPARE(string.setString(QLatin1String("Test")), true);
    QCOMPARE(string.size(), 5);
    QCOMPARE(string.isValid(), true);
    QCOMPARE(string.byte(0), quint8(0x54));
    QCOMPARE(string.string(), QLatin1String("Test"));
    QCOMPARE(string.bytes(), QKnxByteArray({ 0x54, 0x65, 0x73, 0x74, 0x00 }));

    QCOMPARE(string.setString(QLatin1String("This format allows transmission of long strings!")),
        true);
    QCOMPARE(string.size(), 49);
    QCOMPARE(string.isValid(), true);
    QCOMPARE(string.string(), QLatin1String("This format allows transmission of long strings!"));
    QCOMPARE(string.bytes(), QKnxByteArray({ 0x54, 0x68, 0x69, 0x73, 0x20, 0x66, 0x6f, 0x72,
        0x6d, 0x61, 0x74, 0x20, 0x61, 0x6c, 0x6c, 0x6f, 0x77, 0x73, 0x20, 0x74, 0x72, 0x61,
        0x6e, 0x73, 0x6d, 0x69, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x6f, 0x66, 0x20, 0x6c,
        0x6f, 0x6e, 0x67, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x73, 0x21, 0x00 }));

    QCOMPARE(string.setString(QLatin1String("KNX is OK")), true);
    QCOMPARE(string.size(), 10);
    QCOMPARE(string.isValid(), true);
    QCOMPARE(string.string(), QLatin1String("KNX is OK"));
    QCOMPARE(string.bytes(), QKnxByteArray({ 0x4b, 0x4e, 0x58, 0x20, 0x69, 0x73, 0x20, 0x4f,
        0x4b, 0x00 }));

    QCOMPARE(string.setBytes(QKnxByteArray { 0x01 }, 0, 1), true);
    QCOMPARE(string.isValid(), false);
    QCOMPARE(string.setBytes(QKnxByteArray { 0x01, 0x00 }, 0, 2), true);
    QCOMPARE(string.isValid(), true);
}

void tst_QKnxDatapointType::dpt26_SceneInfo()
{
    QKnxSceneInfo dpt;
    QCOMPARE(dpt.size(), 1);
    QCOMPARE(dpt.mainType(), 26);
    QCOMPARE(dpt.subType(), 0x01);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));
    QCOMPARE(dpt.info(), QKnxSceneInfo::Info::Active);

    QCOMPARE(dpt.setSceneNumber(63), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(63));
    QCOMPARE(dpt.setSceneNumber(0), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));
    QCOMPARE(dpt.setSceneNumber(64), false);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(0));

    QCOMPARE(dpt.setInfo(QKnxSceneInfo::Active), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.info(), QKnxSceneInfo::Active);
    QCOMPARE(dpt.setInfo(QKnxSceneInfo::Inactive), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.info(), QKnxSceneInfo::Inactive);
    QCOMPARE(dpt.setInfo(QKnxSceneInfo::Info(2)), false);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.info(), QKnxSceneInfo::Inactive);

    dpt.setBytes(QKnxByteArray { 0xff }, 0, 1);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(dpt.sceneNumber(), quint8(63));
    QCOMPARE(dpt.info(), QKnxSceneInfo::Info::Inactive);

    dpt.setBytes(QKnxByteArray { 0x80 }, 0, 1);
    QCOMPARE(dpt.isValid(), false);
    QCOMPARE(dpt.sceneNumber(), quint8(0));
    QCOMPARE(dpt.info(), QKnxSceneInfo::Info::Active);

    dpt.setBytes(QKnxByteArray { 0x42 }, 0, 1);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.sceneNumber(), quint8(2));
    QCOMPARE(dpt.info(), QKnxSceneInfo::Info::Inactive);
}

void tst_QKnxDatapointType::dpt27_32BitSet()
{
    QKnx32BitSet dpt32;
    QCOMPARE(dpt32.mainType(), 27);
    QCOMPARE(dpt32.subType(), 0);
    QCOMPARE(dpt32.size(), 4);

    QKnxCombinedInfoOnOff dpt;
    QCOMPARE(dpt.mainType(), 27);
    QCOMPARE(dpt.subType(), 1);
    QCOMPARE(dpt.size(), 4);
    QCOMPARE(dpt.isValid(), true);

    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::First), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Second), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Third), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Fourth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Fifth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Sixth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Seventh), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Eighth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Ninth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Tenth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Eleventh), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Twelfth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Thirteenth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Fourteenth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Fifteenth), QKnxCombinedInfoOnOff::OutputState::Off);
    QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output::Sixteenth), QKnxCombinedInfoOnOff::OutputState::Off);

    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::First), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Second), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Third), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Fourth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Fifth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Sixth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Seventh), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Eighth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Ninth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Tenth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Eleventh), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Twelfth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Thirteenth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Fourteenth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Fifteenth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output::Sixteenth), QKnxCombinedInfoOnOff::OutputValidity::Invalid);

    for (int i = 0; i < 16; ++i) {
        dpt.setState(QKnxCombinedInfoOnOff::Output(i), QKnxCombinedInfoOnOff::OutputState::On);
        dpt.setValidity(QKnxCombinedInfoOnOff::Output(i), QKnxCombinedInfoOnOff::OutputValidity::Valid);

        QCOMPARE(QKnxDatapointType::testBit(dpt.value(), i), true);
        QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output(i)), QKnxCombinedInfoOnOff::OutputState::On);

        QCOMPARE(QKnxDatapointType::testBit(dpt.value(), i + 16), true);
        QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output(i)), QKnxCombinedInfoOnOff::OutputValidity::Valid);

        dpt.setState(QKnxCombinedInfoOnOff::Output(i), QKnxCombinedInfoOnOff::OutputState::Off);
        dpt.setValidity(QKnxCombinedInfoOnOff::Output(i), QKnxCombinedInfoOnOff::OutputValidity::Invalid);

        QCOMPARE(QKnxDatapointType::testBit(dpt.value(), i), false);
        QCOMPARE(dpt.state(QKnxCombinedInfoOnOff::Output(i)), QKnxCombinedInfoOnOff::OutputState::Off);

        QCOMPARE(QKnxDatapointType::testBit(dpt.value(), i + 16), false);
        QCOMPARE(dpt.validity(QKnxCombinedInfoOnOff::Output(i)), QKnxCombinedInfoOnOff::OutputValidity::Invalid);
    }

    dpt = QKnxCombinedInfoOnOff(
        {
            { QKnxCombinedInfoOnOff::Output::First, QKnxCombinedInfoOnOff::OutputState::On,
                QKnxCombinedInfoOnOff::OutputValidity::Valid },
            { QKnxCombinedInfoOnOff::Output::Third, QKnxCombinedInfoOnOff::OutputState::Off,
                QKnxCombinedInfoOnOff::OutputValidity::Valid },
            { QKnxCombinedInfoOnOff::Output::Fifth, QKnxCombinedInfoOnOff::OutputState::Off,
                QKnxCombinedInfoOnOff::OutputValidity::Invalid },
            { QKnxCombinedInfoOnOff::Output::Sixteenth, QKnxCombinedInfoOnOff::OutputState::On,
                QKnxCombinedInfoOnOff::OutputValidity::Valid }
        }
    );
    QCOMPARE(QKnxDatapointType::testBit(dpt.value(), 0), true); // Output::First -> State on
    QCOMPARE(QKnxDatapointType::testBit(dpt.value(), 16), true); // Output::First -> State valid

    QCOMPARE(QKnxDatapointType::testBit(dpt.value(), 2), false); // Output::Third -> State off
    QCOMPARE(QKnxDatapointType::testBit(dpt.value(), 18), true); // Output::Third -> State valid

    QCOMPARE(QKnxDatapointType::testBit(dpt.value(), 4), false); // Output::Fifth -> State off
    QCOMPARE(QKnxDatapointType::testBit(dpt.value(), 20), false); // Output::Fifth -> State invalid

    QCOMPARE(QKnxDatapointType::testBit(dpt.value(), 15), true); // Output::Sixteenth -> State on
    QCOMPARE(QKnxDatapointType::testBit(dpt.value(), 31), true); // Output::Sixteenth -> State valid
}

void tst_QKnxDatapointType::dpt28_StringUtf8()
{
    QKnxUtf8String string;
    QCOMPARE(string.mainType(), 28);
    QCOMPARE(string.subType(), 0);
    QCOMPARE(string.size(), 1);
    QCOMPARE(string.isValid(), true);
    QCOMPARE(string.setString(QString(QByteArray(1, char(85)))), true);
    QCOMPARE(string.byte(0), quint8(85));

    QKnxByteArray data { 0xe2, 0x88, 0x83, 0x79, 0x20, 0xe2, 0x88, 0x80, 0x78, 0x20, 0xc2,
        0xac, 0x28, 0x78, 0x20, 0xe2, 0x89, 0xba, 0x20, 0x79, 0x29, 0x00};

    QCOMPARE(string.setString(QString::fromUtf8({ (const char*) data.constData(), data.size() - 1 })), true);
    QCOMPARE(string.size(), 22);
    QCOMPARE(string.isValid(), true);
    QCOMPARE(string.byte(0), data.at(0));
    QCOMPARE(string.byte(1), data.at(1));
    QCOMPARE(string.byte(2), data.at(2));
    QCOMPARE(string.bytes(), data);

    QCOMPARE(string.setString(QString("KNX is OK")), true);
    QCOMPARE(string.byte(0), quint8(75));
    QCOMPARE(string.string(), QString("KNX is OK"));
    QCOMPARE(string.bytes(), QKnxByteArray({ 0x4b, 0x4e, 0x58, 0x20, 0x69, 0x73, 0x20, 0x4f,
        0x4b, 0x00 }));

    QKnxUtf8 utf8;
    QCOMPARE(utf8.setString((const char*) data.constData(), data.size() - 1), true);
    QCOMPARE(utf8.size(), 22);
    QCOMPARE(utf8.isValid(), true);
    QCOMPARE(utf8.byte(0), data.at(0));
    QCOMPARE(utf8.byte(1), data.at(1));
    QCOMPARE(utf8.byte(2), data.at(2));
    QCOMPARE(utf8.bytes(), data);

    QCOMPARE(utf8.setString(QString("KNX is OK")), true);
    QCOMPARE(utf8.byte(0), quint8(75));
    QCOMPARE(utf8.string(), QString("KNX is OK"));
    QCOMPARE(string.bytes(), QKnxByteArray({ 0x4b, 0x4e, 0x58, 0x20, 0x69, 0x73, 0x20, 0x4f,
        0x4b, 0x00 }));
}

void tst_QKnxDatapointType::dpt29_ElectricalEnergy()
{
    QKnxElectricalEnergy dpt;
    QCOMPARE(dpt.mainType(), 0x1d);
    QCOMPARE(dpt.subType(), 0x00);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.setValue(255), true);
    QCOMPARE(dpt.value(), qint64(255));
    QCOMPARE(dpt.setValue(LONG_MIN), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(dpt.value(), qint64(LONG_MIN));
    QCOMPARE(dpt.setValue(2147483647), true);
    QCOMPARE(dpt.isValid(), true);
    QCOMPARE(qint64(dpt.value()), qint64(2147483647));

    QKnxActiveEnergyV64 dpt2;
    QCOMPARE(dpt2.mainType(), 0x1d);
    QCOMPARE(dpt2.subType(), 0x0a);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(dpt2.setValue(255), true);
    QCOMPARE(dpt2.value(), qint64(255));
    QCOMPARE(dpt2.setValue(LONG_MIN), true);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(dpt2.value(), qint64(LONG_MIN));
    QCOMPARE(dpt2.setValue(2147483647), true);
    QCOMPARE(dpt2.isValid(), true);
    QCOMPARE(dpt2.value(), qint64(2147483647));
}

QTEST_MAIN(tst_QKnxDatapointType)

#include "tst_qknxdatapointtype.moc"
