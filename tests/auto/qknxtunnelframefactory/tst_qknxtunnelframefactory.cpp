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

#include <QtKnx/qknxtunnelframefactory.h>
#include <QtKnx/qknxextendedcontrolfield.h>
#include <QtTest/qtest.h>

class tst_QKnxTunnelFrameFactory : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testGroupValueRead();
    void testGroupValueResponse();
    void testGroupValueWrite();
};

void tst_QKnxTunnelFrameFactory::testGroupValueRead()
{
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Group, QString("0/0/2") };
    auto frame = QKnxTunnelFrameFactory::GroupValue::createReadRequest(source, destination);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("1100b4e000000002010000"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxTunnelFrameFactory::GroupValue::createReadRequest(source, destinationWrong);
    QCOMPARE(frame.bytes(), QKnxTunnelFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxTunnelFrameFactory::GroupValue::createReadConfirmation(source, destination,
        QKnxControlField::Confirm::NoError);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2e00b4e011020002010000"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxTunnelFrameFactory::GroupValue::createReadIndication(source, destination);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2900b4e011020002010000"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);
}

void tst_QKnxTunnelFrameFactory::testGroupValueResponse()
{
    QByteArray data = QByteArray::fromHex("01");
    QKnxAddress src { QKnxAddress::Type::Individual, QString("1.1.1") };
    QKnxAddress destination { QKnxAddress::Type::Group, QString("0/0/2") };
    auto frame = QKnxTunnelFrameFactory::GroupValue::createResponseIndication(src, destination,
        data);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2900b4e011010002010041"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxTunnelFrameFactory::GroupValue::createResponseRequest(src, destination, data);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("1100b4e011010002010041"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxTunnelFrameFactory::GroupValue::createResponseConfirmation(src, destination, data);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2e00b4e011010002010041"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);
}

void tst_QKnxTunnelFrameFactory::testGroupValueWrite()
{
    QByteArray data = QByteArray::fromHex("01");
    QKnxAddress source { QKnxAddress::Type::Individual, QString("1.1.1") };
    QKnxAddress destination { QKnxAddress::Type::Group, QString("0/0/2") };
    auto frame = QKnxTunnelFrameFactory::GroupValue::createWriteIndication(source, destination,
        data);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2900b4e011010002010081"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxTunnelFrameFactory::GroupValue::createWriteRequest(source, destination, data);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("1100b4e011010002010081"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxTunnelFrameFactory::GroupValue::createWriteConfirmation(source, destination, data);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2e00b4e011010002010081"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);
}

QTEST_APPLESS_MAIN(tst_QKnxTunnelFrameFactory)

#include "tst_qknxtunnelframefactory.moc"
