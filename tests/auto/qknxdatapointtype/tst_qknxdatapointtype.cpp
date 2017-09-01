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

#include <QtCore/qvector.h>
#include <QtKnx/qknx1bit.h>
#include <QtKnx/qknx1bitcontrolled.h>
#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxdatapointtypefactory.h>
#include <QtTest/qtest.h>

class tst_QKnxDatapointType : public QObject
{
    Q_OBJECT

private slots:
    void datapointType();
    void dpt1_1Bit();
    void dpt2_1BitControlled();
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

    dptSwitch.setBytes(QVector<quint8>{ 0xff }, 0, 1);
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
    QVERIFY2(knx1bit != nullptr, "dynamic_cast to QKnx1Bit was not suposed to fail");

    auto knxSwitch = dynamic_cast<QKnxSwitch *> (dpt.data());
    QVERIFY2(knxSwitch != nullptr, "dynamic_cast to QKnxSwitch was not suposed to fail");

    auto controlled = dynamic_cast<QKnx1BitControlled *> (dpt.data());
    QCOMPARE(controlled, nullptr);

    dpt.reset(factory.createType(QKnxDatapointType::Type::Dpt1_1Bit));
    QCOMPARE(dpt->type(), QKnxDatapointType::Type::Dpt1_1Bit);
}

void tst_QKnxDatapointType::dpt2_1BitControlled()
{
    // TODO: Implement.
}

void tst_QKnxDatapointType::dpt21_8BitSet()
{
    // TODO: Implement.
}

QTEST_MAIN(tst_QKnxDatapointType)

#include "tst_qknxdatapointtype.moc"
