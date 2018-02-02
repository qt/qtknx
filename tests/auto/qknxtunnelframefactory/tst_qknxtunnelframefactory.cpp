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

#include <QtKnx/qknxlinklayerframefactory.h>
#include <QtKnx/qknxextendedcontrolfield.h>
#include <QtTest/qtest.h>

class tst_QKnxLinkLayerFrameFactory : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testGroupValueRead();
    void testGroupValueResponse();
    void testGroupValueWrite();
    void testMemoryRead();
    void testMemoryResponse();
    void testMemoryWrite();
};

void tst_QKnxLinkLayerFrameFactory::testGroupValueRead()
{
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Group, QString("0/0/2") };
    auto frame = QKnxLinkLayerFrameFactory::GroupValue::createReadRequest(source, destination);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("1100b4e000000002010000"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxLinkLayerFrameFactory::GroupValue::createReadRequest(source, destinationWrong);
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxLinkLayerFrameFactory::GroupValue::createReadConfirmation(source, destination,
        QKnxControlField::Confirm::NoError);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2e00b4e011020002010000"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxLinkLayerFrameFactory::GroupValue::createReadIndication(source, destination);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2900b4e011020002010000"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);
}

void tst_QKnxLinkLayerFrameFactory::testGroupValueResponse()
{
    QKnxAddress src { QKnxAddress::Type::Individual, QString("1.1.1") };
    QKnxAddress destination { QKnxAddress::Type::Group, QString("0/0/2") };
    auto frame = QKnxLinkLayerFrameFactory::GroupValue::createResponseIndication(src, destination,
        QVector<quint8>(1, 0x01));
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2900b4e011010002010041"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxLinkLayerFrameFactory::GroupValue::createResponseRequest(src, destination,
        QVector<quint8>(1, 0x01));
    QCOMPARE(frame.bytes(), QByteArray::fromHex("1100b4e011010002010041"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxLinkLayerFrameFactory::GroupValue::createResponseConfirmation(src, destination,
        QVector<quint8>(1, 0x01));
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2e00b4e011010002010041"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);
}

void tst_QKnxLinkLayerFrameFactory::testGroupValueWrite()
{
    QKnxAddress source { QKnxAddress::Type::Individual, QString("1.1.1") };
    QKnxAddress destination { QKnxAddress::Type::Group, QString("0/0/2") };
    auto frame = QKnxLinkLayerFrameFactory::GroupValue::createWriteIndication(source, destination,
        QVector<quint8>(1, 0x01));
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2900b4e011010002010081"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxLinkLayerFrameFactory::GroupValue::createWriteRequest(source, destination,
        QVector<quint8>(1, 0x01));
    QCOMPARE(frame.bytes(), QByteArray::fromHex("1100b4e011010002010081"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxLinkLayerFrameFactory::GroupValue::createWriteConfirmation(source, destination,
        QVector<quint8>(1, 0x01));
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2e00b4e011010002010081"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);
}

void tst_QKnxLinkLayerFrameFactory::testMemoryRead()
{
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Individual, QString("1.2.3") };
    auto frame = QKnxLinkLayerFrameFactory::Memory::createReadRequest(source, destination, 52, 3);
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Individual);
    QCOMPARE(frame.extendedControlField().bytes(), quint8(96));
    QCOMPARE(frame.tpdu().bytes(), QVector<quint8>({ 0x42, 0x03, 0x00, 0x34 }));
    QCOMPARE(frame.bytes(), QByteArray::fromHex("1100b460000012030342030034"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxLinkLayerFrameFactory::Memory::createReadRequest(source, destinationWrong, 52, 3);
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxLinkLayerFrameFactory::Memory::createReadConfirmation(source, destination, 52, 3, 0,
        QKnxControlField::Confirm::NoError);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2e00b460110212030342030034"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxLinkLayerFrameFactory::Memory::createReadIndication(source, destination, 52, 3);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2900b460110212030342030034"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);
}

void tst_QKnxLinkLayerFrameFactory::testMemoryResponse()
{
    QVector<quint8> data({ 0x01, 0x02, 0x03 });
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Individual, QString("1.2.3") };
    auto frame = QKnxLinkLayerFrameFactory::Memory::createResponseRequest(source, destination, 52, 3,
        data);
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Individual);
    QCOMPARE(frame.extendedControlField().bytes(), quint8(96));
    QCOMPARE(frame.tpdu().bytes(), QVector<quint8>({ 0x42, 0x43, 0x00, 0x34, 0x01, 0x02, 0x03 }));
    QCOMPARE(frame.bytes(), QByteArray::fromHex("1100b460000012030642430034010203"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxLinkLayerFrameFactory::Memory::createResponseRequest(source, destinationWrong, 52, 3,
        data);
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxLinkLayerFrameFactory::Memory::createResponseConfirmation(source, destination, 52, 3,
        data, 0, QKnxControlField::Confirm::NoError);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2e00b460110212030642430034010203"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxLinkLayerFrameFactory::Memory::createResponseIndication(source, destination, 52, 3,
        data);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2900b460110212030642430034010203"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);
}

void tst_QKnxLinkLayerFrameFactory::testMemoryWrite()
{
    QVector<quint8> data({ 0x01, 0x02, 0x03 });
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Individual, QString("1.2.3") };
    auto frame = QKnxLinkLayerFrameFactory::Memory::createWriteRequest(source, destination, 52, 3,
        data);
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Individual);
    QCOMPARE(frame.extendedControlField().bytes(), quint8(96));
    QCOMPARE(frame.tpdu().bytes(), QVector<quint8>({ 0x42, 0x83, 0x00, 0x34, 0x01, 0x02, 0x03 }));
    QCOMPARE(frame.bytes(), QByteArray::fromHex("1100b460000012030642830034010203"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxLinkLayerFrameFactory::Memory::createWriteRequest(source, destinationWrong, 52, 3,
        data);
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxLinkLayerFrameFactory::Memory::createWriteConfirmation(source, destination, 52, 3,
        data, 0, QKnxControlField::Confirm::NoError);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2e00b460110212030642830034010203"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);

    frame = QKnxLinkLayerFrameFactory::Memory::createWriteIndication(source, destination, 52, 3,
        data);
    QCOMPARE(frame.bytes(), QByteArray::fromHex("2900b460110212030642830034010203"));
    QCOMPARE(frame.controlField().frameType(), QKnxControlField::FrameType::Standard);
}

QTEST_APPLESS_MAIN(tst_QKnxLinkLayerFrameFactory)

#include "tst_qknxtunnelframefactory.moc"
