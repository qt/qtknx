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

#include <QtKnx/qknxlinklayerframebuilder.h>
#include <QtKnx/QKnxLinkLayerFrame>
#include <QtKnx/qknxextendedcontrolfield.h>
#include <QtKnx/private/qknxtpdufactory_p.h>
#include <QtTest/qtest.h>

class tst_QKnxLinkLayerFrameBuilder : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testGroupValueRead();
    void testGroupValueResponse();
    void testGroupValueWrite();
    void testGroupValueReadWrongParameters();
    void testMemoryRead();
    void testMemoryResponse();
    void testMemoryWrite();
    void testDeviceDescriptorRead();
    void testDeviceDescriptorResponse();
private:
    QKnxControlField m_ctrl;
    QKnxExtendedControlField m_extCtrl;
    QKnxTpdu m_tpdu;
};

void tst_QKnxLinkLayerFrameBuilder::initTestCase()
{
    m_ctrl = QKnxControlField::builder()
        .setFrameFormat(QKnxControlField::FrameFormat::Extended)
        .setBroadcast(QKnxControlField::Broadcast::Domain)
        .setPriority(QKnxControlField::Priority::Normal)
        .create();
    QCOMPARE(m_ctrl.bytes(), QKnxByteArray({ 0x34 }));

    m_extCtrl = QKnxExtendedControlField::builder()
        .setDestinationAddressType(QKnxAddress::Type::Group)
        .create();
    QCOMPARE(m_extCtrl.bytes(), QKnxByteArray::fromHex("e0"));

    m_tpdu = QKnxTpduFactory::Multicast::createGroupValueReadTpdu();
    QCOMPARE(m_tpdu.bytes(), QKnxByteArray({ 0, 0 }));
    QVERIFY(m_tpdu.isValid());
}

void tst_QKnxLinkLayerFrameBuilder::testGroupValueRead()
{
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QCOMPARE(source.bytes(), QKnxByteArray::fromHex("0000"));
    QKnxAddress destination { QKnxAddress::Type::Group, QString("0/0/2") };
    QCOMPARE(destination.bytes(), QKnxByteArray::fromHex("0002"));

    auto frame = QKnxLinkLayerFrameBuilder()
            .setControlField(m_ctrl)
            .setExtendedControlField(m_extCtrl)
            .setTpdu(m_tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("110034e000000002010000"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    auto frameFromBytesString = QKnxLinkLayerFrameBuilder()
            .setMedium(QKnx::MediumType::NetIP)
            .setData(QKnxByteArray::fromHex("110034e000000002010000"))
            .createFrame();
    QCOMPARE(frameFromBytesString.bytes(), QKnxByteArray::fromHex("110034e000000002010000"));
}

void tst_QKnxLinkLayerFrameBuilder::testGroupValueReadWrongParameters() {
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    auto frame = QKnxLinkLayerFrameBuilder()
            .setControlField(QKnxControlField())
            .setExtendedControlField(m_extCtrl)
            .setTpdu(m_tpdu)
            .setDestinationAddress(destinationWrong)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    QKnxAddress destination = { QKnxAddress::Type::Group, QString("0/0/2") };
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(m_ctrl)
            .setExtendedControlField(
                QKnxExtendedControlField::builder()
                .setDestinationAddressType(QKnxAddress::Type::Individual)
                .create())
            .setTpdu(m_tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    // verify wrongly form
    QVERIFY(frame.destinationAddress().type() != frame.extendedControlField().destinationAddressType());
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());
}

void tst_QKnxLinkLayerFrameBuilder::testGroupValueResponse()
{
    QKnxAddress source { QKnxAddress::Type::Individual, QString("1.1.1") };
    QCOMPARE(source.bytes(), QKnxByteArray::fromHex("1101"));

    QKnxAddress destination { QKnxAddress::Type::Group, QString("0/0/2") };
    QCOMPARE(destination.bytes(), QKnxByteArray::fromHex("0002"));

    m_tpdu = QKnxTpduFactory::Multicast::createGroupValueResponseTpdu({ 0x01 });
    QCOMPARE(m_tpdu.bytes(), QKnxByteArray::fromHex("0041"));
    auto frame = QKnxLinkLayerFrameBuilder()
            .setControlField(m_ctrl)
            .setExtendedControlField(m_extCtrl)
            .setTpdu(m_tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("290034e011010002010041"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    auto frameFromBytesString = QKnxLinkLayerFrameBuilder()
            .setMedium(QKnx::MediumType::NetIP)
            .setData(QKnxByteArray::fromHex("290034e011010002010041"))
            .createFrame();
    QCOMPARE(frameFromBytesString.bytes(), QKnxByteArray::fromHex("290034e011010002010041"));

    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(m_ctrl)
            .setExtendedControlField(m_extCtrl)
            .setTpdu(m_tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("110034e011010002010041"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    frameFromBytesString = QKnxLinkLayerFrameBuilder()
                .setMedium(QKnx::MediumType::NetIP)
                .setData(QKnxByteArray::fromHex("110034e011010002010041"))
                .createFrame();
        QCOMPARE(frameFromBytesString.bytes(), QKnxByteArray::fromHex("110034e011010002010041"));

    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(m_ctrl)
            .setExtendedControlField(m_extCtrl)
            .setTpdu(m_tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataConfirmation)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("2e0034e011010002010041"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    frameFromBytesString = QKnxLinkLayerFrameBuilder()
                .setMedium(QKnx::MediumType::NetIP)
                .setData(QKnxByteArray::fromHex("2e0034e011010002010041"))
                .createFrame();
        QCOMPARE(frameFromBytesString.bytes(), QKnxByteArray::fromHex("2e0034e011010002010041"));
}

void tst_QKnxLinkLayerFrameBuilder::testGroupValueWrite()
{
    QKnxAddress source { QKnxAddress::Type::Individual, QString("1.1.1") };
    QCOMPARE(source.bytes(), QKnxByteArray::fromHex("1101"));

    QKnxAddress destination { QKnxAddress::Type::Group, QString("0/0/2") };
    QCOMPARE(destination.bytes(), QKnxByteArray::fromHex("0002"));

    QKnxByteArray data { 0x01};
    m_tpdu = QKnxTpduFactory::Multicast::createGroupValueWriteTpdu(data);
    QCOMPARE(m_tpdu.bytes(), QKnxByteArray::fromHex("0081"));
    auto frame = QKnxLinkLayerFrameBuilder()
            .setControlField(m_ctrl)
            .setExtendedControlField(m_extCtrl)
            .setTpdu(m_tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("290034e011010002010081"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    auto frameFromBytesString = QKnxLinkLayerFrameBuilder()
            .setMedium(QKnx::MediumType::NetIP)
            .setData(QKnxByteArray::fromHex("290034e011010002010081"))
            .createFrame();
    QCOMPARE(frameFromBytesString.bytes(), QKnxByteArray::fromHex("290034e011010002010081"));

    // group value write request
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(m_ctrl)
            .setExtendedControlField(m_extCtrl)
            .setTpdu(m_tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("110034e011010002010081"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    frameFromBytesString = QKnxLinkLayerFrameBuilder()
                .setMedium(QKnx::MediumType::NetIP)
                .setData(QKnxByteArray::fromHex("110034e011010002010081"))
                .createFrame();
    QCOMPARE(frameFromBytesString.bytes(), QKnxByteArray::fromHex("110034e011010002010081"));

    // group value write confirmation
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(m_ctrl)
            .setExtendedControlField(m_extCtrl)
            .setTpdu(m_tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataConfirmation)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("2e0034e011010002010081"));
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Group);
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    frameFromBytesString = QKnxLinkLayerFrameBuilder()
                .setMedium(QKnx::MediumType::NetIP)
                .setData(QKnxByteArray::fromHex("2e0034e011010002010081"))
                .createFrame();
    QCOMPARE(frameFromBytesString.bytes(), QKnxByteArray::fromHex("2e0034e011010002010081"));
}

void tst_QKnxLinkLayerFrameBuilder::testMemoryRead()
{
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Individual, QString("1.2.3") };

    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryReadTpdu(3, 52);
    QCOMPARE(tpdu.bytes(), QKnxByteArray({ 0x42, 0x03, 0x00, 0x34 }));

    auto ctrl = QKnxControlField::builder()
                .setFrameFormat(QKnxControlField::FrameFormat::Extended)
                .setBroadcast(QKnxControlField::Broadcast::Domain)
                .setPriority(QKnxControlField::Priority::Normal)
                .create();
    QCOMPARE(ctrl.frameFormat(), QKnxControlField::FrameFormat::Extended);

    auto extctr = QKnxExtendedControlField::builder()
            .setDestinationAddressType(QKnxAddress::Type::Individual)
            .create();
    QCOMPARE(extctr.destinationAddressType(), QKnxAddress::Type::Individual);
    auto byts = extctr.bytes();
    QCOMPARE(extctr.bytes(), QKnxByteArray{0x60});

    auto frame = QKnxLinkLayerFrameBuilder().setControlField(ctrl).setExtendedControlField(extctr)
            .setTpdu(tpdu).setDestinationAddress(destination).setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("11003460000012030342030034"));

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxLinkLayerFrameBuilder().setControlField(ctrl).setExtendedControlField(extctr)
                .setTpdu(tpdu).setDestinationAddress(destinationWrong).setSourceAddress(source)
                .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
                .setMedium(QKnx::MediumType::NetIP)
                .createFrame();
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxLinkLayerFrameBuilder().setControlField(ctrl).setExtendedControlField(extctr)
                .setTpdu(tpdu).setDestinationAddress(destination).setSourceAddress(source)
                .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataConfirmation)
                .setMedium(QKnx::MediumType::NetIP)
                .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("2e003460110212030342030034"));

    frame = QKnxLinkLayerFrameBuilder().setControlField(ctrl).setExtendedControlField(extctr)
                .setTpdu(tpdu).setDestinationAddress(destination).setSourceAddress(source)
                .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
                .setMedium(QKnx::MediumType::NetIP)
                .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("29003460110212030342030034"));
}

void tst_QKnxLinkLayerFrameBuilder::testMemoryResponse()
{
    QKnxByteArray data { 0x01, 0x02, 0x03 };
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Individual, QString("1.2.3") };
    auto ctrl = QKnxControlField::builder()
                .setFrameFormat(QKnxControlField::FrameFormat::Extended)
                .setBroadcast(QKnxControlField::Broadcast::Domain)
                .setPriority(QKnxControlField::Priority::Normal)
                .create();
    auto extctr = QKnxExtendedControlField::builder()
            .setDestinationAddressType(QKnxAddress::Type::Individual)
            .create();
    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryResponseTpdu(3, 52,
        data);
    auto frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Individual);
    QCOMPARE(frame.extendedControlField().bytes(), QKnxByteArray({0x60}));
    QCOMPARE(frame.tpdu().bytes(), QKnxByteArray({ 0x42, 0x43, 0x00, 0x34, 0x01, 0x02, 0x03 }));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("11003460000012030642430034010203"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destinationWrong)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataConfirmation)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();

    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("2e003460110212030642430034010203"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("29003460110212030642430034010203"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);
}

void tst_QKnxLinkLayerFrameBuilder::testMemoryWrite()
{
    QKnxByteArray data { 0x01, 0x02, 0x03 };
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Individual, QString("1.2.3") };
    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryWriteTpdu(3, 52, data);
    auto ctrl = QKnxControlField::builder()
                .setFrameFormat(QKnxControlField::FrameFormat::Extended)
                .setBroadcast(QKnxControlField::Broadcast::Domain)
                .setPriority(QKnxControlField::Priority::Normal)
                .create();
    auto extctr = QKnxExtendedControlField::builder()
            .setDestinationAddressType(QKnxAddress::Type::Individual)
            .create();
    auto frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Individual);
    QCOMPARE(frame.extendedControlField().bytes(), QKnxByteArray({0x60}));
    QCOMPARE(frame.tpdu().bytes(), QKnxByteArray({ 0x42, 0x83, 0x00, 0x34, 0x01, 0x02, 0x03 }));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("11003460000012030642830034010203"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destinationWrong)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataConfirmation)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("2e003460110212030642830034010203"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("29003460110212030642830034010203"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);
}

void tst_QKnxLinkLayerFrameBuilder::testDeviceDescriptorRead()
{
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Individual, QString("1.2.3") };
    QKnxTpduFactory::PointToPoint::Mode mode =
        QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented;
    auto ctrl = QKnxControlField::builder()
                .setFrameFormat(QKnxControlField::FrameFormat::Extended)
                .setBroadcast(QKnxControlField::Broadcast::Domain)
                .setPriority(QKnxControlField::Priority::Normal)
                .create();
    auto extctr = QKnxExtendedControlField::builder()
            .setDestinationAddressType(QKnxAddress::Type::Individual)
            .create();
    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorReadTpdu(mode, 3);
    auto frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Individual);
    QCOMPARE(frame.tpdu().bytes(), QKnxByteArray({ 0x43, 0x03 }));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("1100346000001203014303"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destinationWrong)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("2900346011021203014303"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataConfirmation)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("2e00346011021203014303"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);
}

void tst_QKnxLinkLayerFrameBuilder::testDeviceDescriptorResponse()
{
    QKnxByteArray data { 0x01, 0x02, 0x03 };
    QKnxAddress source { QKnxAddress::Type::Individual, 0 };
    QKnxAddress destination { QKnxAddress::Type::Individual, QString("1.2.3") };
    QKnxTpduFactory::PointToPoint::Mode mode =
        QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented;
    auto ctrl = QKnxControlField::builder()
                .setFrameFormat(QKnxControlField::FrameFormat::Extended)
                .setBroadcast(QKnxControlField::Broadcast::Domain)
                .setPriority(QKnxControlField::Priority::Normal)
                .create();
    auto extctr = QKnxExtendedControlField::builder()
            .setDestinationAddressType(QKnxAddress::Type::Individual)
            .create();
    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorResponseTpdu(mode, 3, data);

    auto frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.extendedControlField().destinationAddressType(), QKnxAddress::Type::Individual);

    QCOMPARE(frame.tpdu().bytes(), QKnxByteArray({ 0x43, 0x43, 0x01, 0x02, 0x03 }));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("1100346000001203044343010203"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    QKnxAddress destinationWrong = { QKnxAddress::Type::Group, QString("0.0.2") };
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destinationWrong)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataRequest)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxLinkLayerFrame().bytes());

    source = { QKnxAddress::Type::Individual, QString("1.1.2") };
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("2900346011021203044343010203"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataConfirmation)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("2e00346011021203044343010203"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    QKnxByteArray dataLong({0x01, 0x02, 0x03, 0x34, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11,
        0x12, 0x13, 0x14, 0x15, 0x16});
    tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorResponseTpdu(mode, 3, dataLong);
    frame = QKnxLinkLayerFrameBuilder()
            .setControlField(ctrl)
            .setExtendedControlField(extctr)
            .setTpdu(tpdu)
            .setDestinationAddress(destination)
            .setSourceAddress(source)
            .setMsgCode(QKnxLinkLayerFrame::MessageCode::DataIndication)
            .setMedium(QKnx::MediumType::NetIP)
            .createFrame();
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("29003c601102120311434301020334050607080910111213141516"));
    QCOMPARE(frame.controlField().frameFormat(), QKnxControlField::FrameFormat::Extended);

    auto arr = QKnxByteArray::fromHex("29003c601102120311434301020334050607080910111213141516");
    auto frameFromBytesString = QKnxLinkLayerFrameBuilder()
            .setMedium(QKnx::MediumType::NetIP)
            .setData(arr)
            .createFrame();
    QCOMPARE(frameFromBytesString.bytes(), QKnxByteArray::fromHex("29003c601102120311434301020334050607080910111213141516"));
}

QTEST_APPLESS_MAIN(tst_QKnxLinkLayerFrameBuilder)

#include "tst_qknxtunnelframefactory.moc"
