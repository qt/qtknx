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

#include <QtKnx/qknxcharstring.h>
#include <QtKnx/qknxtpdufactory.h>
#include <QtKnx/qknxlinklayerframe.h>
#include <QtTest/qtest.h>

class tst_QKnxTpduFactory : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testTpduConstructor();
    void testTpdu();
    void testGroupValueRead();
    void testGroupValueWrite();
    // TODO: GroupValueResponse

    void testGroupPropValueRead();
    void testGroupPropValueWrite();
    // TODO: GroupPropValueResponse
    // TODO: GroupPropValueInfoReport

    void testIndividualAddressRead();
    void testIndividualAddressWrite();
    // TODO: IndividualAddressResponse
    void testIndividualAddressSerialNumberRead();
    void testIndividualAddressSerialNumberWrite();
    // TODO: IndividualAddressSerialNumberResponse
    void testDomainAddressRead();
    void testDomainAddressWrite();
    // TODO: DomainAddressResponse
    // TODO: DomainAddressSelectiveRead
    void testDomainAddressSerialNumberRead();
    void testDomainAddressSerialNumberWrite();
    // TODO: DomainAddressSerialNumberResponse
    // TODO: DeviceDescriptorInfoReport

    void testSystemNetworkParameterRead();
    void testSystemNetworkParameterWrite();
    // TODO: SystemNetworkParameterResponse
    void testNetworkParameterRead();
    void testNetworkParameterWrite();
    void testNetworkParameterResponse();
    void testPropertyValueRead();
    void testPropertyValueWrite();
    // TODO: PropertyValueResponse
    void testPropertyDescriptionRead();
    // TODO: PropertyDescriptionResponse
    void testFunctionPropertyCommand();
    void testFunctionPropertyStateRead();
    // TODO: FunctionPropertyStateResponse
    void testLinkRead();
    void testLinkWrite();
    // TODO: LinkResponse
    void testDeviceDescriptorRead();
    void testDeviceDescriptorResponse();
    // TODO Restart
    void testAdcRead();
    // TODO: AdcResponse
    void testMemoryRead();
    // TODO: MemoryResponse
    void testMemoryWrite();
    // TODO: MemoryBitWrite
    void testUserMemoryRead();
    // TODO: UserMemoryResponse
    void testUserMemoryWrite();
    // TODO: UserMemoryBitWrite
    void testUserManufacturerInfoRead();
    // TODO: UserManufacturerInfoResponse
    // TODO: AuthorizeRequest
    // TODO: AuthorizeResponse
    // TODO: KeyWrite
    // TODO: KeyResponse
};

void tst_QKnxTpduFactory::testTpduConstructor()
{
    QKnxTpdu tpdu1(QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu1.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);

    tpdu1.setApplicationControlField(QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu1.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);

    tpdu1.setData({ 0x01 });
    QCOMPARE(tpdu1.data(), QKnxByteArray(1, 0x01));
    QCOMPARE(tpdu1.bytes(), QKnxByteArray::fromHex("0081"));

    QKnxTpdu tpdu2(QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu2.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu2.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);

    tpdu2.setData({ 0x01 });
    QCOMPARE(tpdu2.data(), QKnxByteArray(1, 0x01));
    QCOMPARE(tpdu2.bytes(), QKnxByteArray::fromHex("0081"));

    QKnxTpdu tpdu3(QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueWrite, { 0x01 });
    QCOMPARE(tpdu3.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu3.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu3.data(), QKnxByteArray(1, 0x01));
    QCOMPARE(tpdu3.bytes(), QKnxByteArray::fromHex("0081"));

    QKnxTpdu tpdu4(QKnxTpdu::TransportControlField::DataGroup, 0);
    QCOMPARE(tpdu4.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);

    tpdu4.setApplicationControlField(QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu4.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);

    tpdu4.setData({ 0x01 });
    QCOMPARE(tpdu4.data(), QKnxByteArray(1, 0x01));
    QCOMPARE(tpdu4.bytes(), QKnxByteArray::fromHex("0081"));

    QKnxTpdu tpdu5(QKnxTpdu::TransportControlField::DataGroup, 0,
        QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu5.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu5.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);

    tpdu5.setData({ 0x01 });
    QCOMPARE(tpdu5.data(), QKnxByteArray(1, 0x01));
    QCOMPARE(tpdu5.bytes(), QKnxByteArray::fromHex("0081"));

    QKnxTpdu tpdu6(QKnxTpdu::TransportControlField::DataGroup, 0,
        QKnxTpdu::ApplicationControlField::GroupValueWrite, { 0x01 });
    QCOMPARE(tpdu6.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu6.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu6.data(), QKnxByteArray(1, 0x01));
    QCOMPARE(tpdu6.bytes(), QKnxByteArray::fromHex("0081"));

    QKnxTpdu tpdu7 = QKnxTpdu::fromBytes(tpdu6.bytes(), 0, 2);
    QCOMPARE(tpdu7.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu7.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu7.data(), QKnxByteArray(1, 0x01));
    QCOMPARE(tpdu7.bytes(), QKnxByteArray::fromHex("0081"));
}

void tst_QKnxTpduFactory::testTpdu()
{
    QKnxTpdu tpdu1(QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueWrite);
    tpdu1.setData({ 0x01 });

    QKnxLinkLayerFrame frame;
    frame.setTpdu(tpdu1);
    QCOMPARE(tpdu1.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(frame.tpdu().data(), QKnxByteArray(1, 0x01));

    QKnxTpdu tpdu(QKnxTpdu::TransportControlField::Invalid);
    QCOMPARE(tpdu.isValid(), false);

    tpdu.setTransportControlField(QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);

    tpdu.setApplicationControlField(QKnxTpdu::ApplicationControlField::GroupValueRead);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueRead);

    tpdu.setTransportControlField(QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueRead);

    tpdu.setApplicationControlField(QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);

    QCOMPARE(tpdu.isValid(), true);
    tpdu.setData({ 0x3f });
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0x3f));

    tpdu.setData({ 0x43 });
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0x43));

    tpdu = { QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueRead };
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueRead);

    tpdu = { QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueResponse };
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueResponse);

    tpdu.setData({ 0x3f });
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0x3f));
    QCOMPARE(tpdu.bytes(), QKnxByteArray({ 0x00, 0x7f }));

    tpdu.setTransportControlField(QKnxTpdu::TransportControlField::DataBroadcast);
    tpdu.setApplicationControlField(QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0x3f));
    QCOMPARE(tpdu.bytes(), QKnxByteArray({ 0x00, 0xbf }));

    tpdu.setData({ 0x43 });
    QCOMPARE(tpdu.data(), QKnxByteArray(1, { 0x43 }));
    QCOMPARE(tpdu.bytes(), QKnxByteArray({ 0x00, 0x80, 0x43 }));

    tpdu = { QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueRead };
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueRead);

    tpdu.setData({ 0x3f, 0x3f });
    QCOMPARE(tpdu.data(), QKnxByteArray({ 0x3f, 0x3f }));
    QCOMPARE(tpdu.isValid(), false);

    tpdu.setData({});
    QCOMPARE(tpdu.isValid(), true);
    QCOMPARE(tpdu.data(), QKnxByteArray {});

    tpdu.setApplicationControlField(QKnxTpdu::ApplicationControlField::GroupValueWrite);
    tpdu.setData({ 0x3f, 0x3f });
    QCOMPARE(tpdu.isValid(), true);
    tpdu.setTransportControlField(QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(tpdu.data(), QKnxByteArray({ 0x3f, 0x3f }));

    tpdu.setData({ 0x3f });
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0x3f));
    QCOMPARE(tpdu.bytes(), QKnxByteArray({ 0x00, 0xbf }));

    tpdu.setData({ 0x00 });
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0x00));
    QCOMPARE(tpdu.bytes(), QKnxByteArray({ 0x00, 0x80 }));

    auto tmpTpdu = QKnxTpdu::fromBytes(tpdu.bytes(), 0, quint8(tpdu.size()));
    QCOMPARE(tmpTpdu.bytes(), tpdu.bytes());
}

void tst_QKnxTpduFactory::testGroupValueRead()
{
    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueReadTpdu();

    QCOMPARE(tpdu.size(), quint16(2));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueRead);
}

void tst_QKnxTpduFactory::testGroupValueWrite()
{
    auto tpdu = QKnxTpduFactory::Multicast::createGroupValueWriteTpdu({ 0x01 });
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0x01));
    QCOMPARE(tpdu.size(), quint16(2));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataGroup);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupValueWrite);

    tpdu.setData({ 0x01, 0x01 });
    QCOMPARE(tpdu.size(), quint16(4));
    QCOMPARE(tpdu.data(), QKnxByteArray({ 0x01, 0x01 }));

    tpdu.setData(QKnxByteArray(1, 0x40));
    QCOMPARE(tpdu.size(), quint16(3));
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0x40));

    tpdu.setData({ 0xf0 });
    QCOMPARE(tpdu.size(), quint16(3));
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0xf0));

    tpdu.setData({ 0xff });
    QCOMPARE(tpdu.size(), quint16(3));
    QCOMPARE(tpdu.data(), QKnxByteArray(1, 0xff));

    tpdu.setData(QKnxCharString("The Qt Company").bytes());
    QCOMPARE(tpdu.isValid(), true);
    QCOMPARE(tpdu.size(), quint16(16));
    QCOMPARE(tpdu.dataSize(), quint8(15));

    tpdu = QKnxTpduFactory::Multicast::createGroupValueWriteTpdu(QKnxCharString("The Qt Company")
        .bytes());
    QCOMPARE(tpdu.isValid(), true);
    QCOMPARE(tpdu.size(), quint16(16));
    QCOMPARE(tpdu.dataSize(), quint8(15));
}

void tst_QKnxTpduFactory::testGroupPropValueRead()
{
    auto tpdu = QKnxTpduFactory::Multicast::createGroupPropertyValueReadTpdu(QKnxInterfaceObjectType
        ::System::Device, 5, QKnxInterfaceObjectProperty::General::ManufacturerData);

    QCOMPARE(tpdu.size(), quint16(6));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataTagGroup);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupPropValueRead);
}

void tst_QKnxTpduFactory::testGroupPropValueWrite()
{
    QKnxByteArray data { 0x01, 0x02, 0x03 };
    auto tpdu = QKnxTpduFactory::Multicast::createGroupPropertyValueWriteTpdu(QKnxInterfaceObjectType
        ::System::Device, 5, QKnxInterfaceObjectProperty::General::ManufacturerData, data);

    QCOMPARE(tpdu.size(), quint16(9));
    QCOMPARE(tpdu.data(), QKnxByteArray({ 0x00, 0x00, 0x13, 0x05, 0x01, 0x02, 0x03 }));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataTagGroup);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::GroupPropValueWrite);

    tpdu.setData(QKnxByteArray(253, 1));
    QCOMPARE(tpdu.isValid(), true);
    QCOMPARE(tpdu.size(), quint16(255));
    QCOMPARE(tpdu.dataSize(), quint8(254));

    tpdu = QKnxTpduFactory::Multicast::createGroupPropertyValueWriteTpdu(QKnxInterfaceObjectType
        ::System::Device, 5, QKnxInterfaceObjectProperty::General::ManufacturerData,
        QKnxByteArray(249, 1));
    QCOMPARE(tpdu.isValid(), true);
    QCOMPARE(tpdu.size(), quint16(255));
    QCOMPARE(tpdu.dataSize(), quint8(254));
}

void tst_QKnxTpduFactory::testIndividualAddressRead()
{
    auto tpdu = QKnxTpduFactory::Broadcast::createIndividualAddressReadTpdu();

    QCOMPARE(tpdu.size(), quint16(2));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::IndividualAddressRead);
}

void tst_QKnxTpduFactory::testIndividualAddressWrite()
{
    auto address = QKnxAddress::Group::Broadcast;
    auto tpdu = QKnxTpduFactory::Broadcast::createIndividualAddressWriteTpdu(address);
    QCOMPARE(tpdu.size(), quint16(2));

    address = QKnxAddress::createIndividual(1, 1, 1);
    tpdu = QKnxTpduFactory::Broadcast::createIndividualAddressWriteTpdu(address);

    QCOMPARE(tpdu.size(), quint16(4));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::IndividualAddressWrite);
    QCOMPARE(QKnxAddress(QKnxAddress::Type::Individual, tpdu.data()), address);
}

void tst_QKnxTpduFactory::testIndividualAddressSerialNumberRead()
{
    auto tpdu = QKnxTpduFactory::Broadcast::createIndividualAddressSerialNumberReadTpdu(
        { 0x01, 0x02, 0x03 });
    QCOMPARE(tpdu.size(), quint16(2));

    tpdu = QKnxTpduFactory::Broadcast::createIndividualAddressSerialNumberReadTpdu(
        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 });

    QCOMPARE(tpdu.size(), quint16(8));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::IndividualAddressSerialNumberRead);
}

void tst_QKnxTpduFactory::testIndividualAddressSerialNumberWrite()
{
    auto address = QKnxAddress::Group::Broadcast;

    auto tpdu = QKnxTpduFactory::Broadcast::createIndividualAddressSerialNumberWriteTpdu(
        { 0x01, 0x02, 0x03 }, address);
    QCOMPARE(tpdu.size(), quint16(2));

    tpdu = QKnxTpduFactory::Broadcast::createIndividualAddressSerialNumberWriteTpdu(
        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 }, address);
    QCOMPARE(tpdu.size(), quint16(2));

    address = QKnxAddress::createIndividual(1, 1, 1);
    tpdu = QKnxTpduFactory::Broadcast::createIndividualAddressSerialNumberWriteTpdu(
        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 }, address);

    QCOMPARE(tpdu.size(), quint16(14));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::IndividualAddressSerialNumberWrite);
}

void tst_QKnxTpduFactory::testDomainAddressRead()
{
    auto tpdu = QKnxTpduFactory::Broadcast::createDomainAddressReadTpdu();

    QCOMPARE(tpdu.size(), quint16(2));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::DomainAddressRead);
}

void tst_QKnxTpduFactory::testDomainAddressWrite()
{
    auto address = QKnxAddress::createGroup(1, 1, 1);
    auto tpdu = QKnxTpduFactory::Broadcast::createDomainAddressWriteTpdu(address.bytes());

    QCOMPARE(tpdu.size(), quint16(4));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::DomainAddressWrite);
}

void tst_QKnxTpduFactory::testDomainAddressSerialNumberRead()
{
    auto tpdu = QKnxTpduFactory::Broadcast::createDomainAddressSerialNumberReadTpdu(
        { 0x01, 0x02, 0x03 });
    QCOMPARE(tpdu.size(), quint16(2));

    tpdu = QKnxTpduFactory::Broadcast::createDomainAddressSerialNumberReadTpdu(
        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 });

    QCOMPARE(tpdu.size(), quint16(8));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::DomainAddressSerialNumberRead);
}

void tst_QKnxTpduFactory::testDomainAddressSerialNumberWrite()
{
    auto address = QKnxAddress::createGroup(1, 1, 1).bytes();
    auto tpdu = QKnxTpduFactory::Broadcast::createDomainAddressSerialNumberWriteTpdu(
        { 0x01, 0x02, 0x03 }, address);
    QCOMPARE(tpdu.size(), quint16(2));

    tpdu = QKnxTpduFactory::Broadcast::createDomainAddressSerialNumberWriteTpdu(
        { 0x01, 0x02, 0x03 }, { 0x01, 0x02, 0x03 });
    QCOMPARE(tpdu.size(), quint16(2));

    tpdu = QKnxTpduFactory::Broadcast::createDomainAddressSerialNumberWriteTpdu(
        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 }, address);

    QCOMPARE(tpdu.size(), quint16(10));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::DomainAddressSerialNumberWrite);
}

void tst_QKnxTpduFactory::testSystemNetworkParameterRead()
{
    auto tpdu = QKnxTpduFactory::Broadcast::createSystemNetworkParameterReadTpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        { 0x01, 0x02, 0x03, 0x04 });

    QCOMPARE(tpdu.size(), quint16(10));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataSystemBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::SystemNetworkParameterRead);
}

void tst_QKnxTpduFactory::testSystemNetworkParameterWrite()
{
    auto tpdu = QKnxTpduFactory::Broadcast::createSystemNetworkParameterWriteTpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        { 0x01, 0x02, 0x03, 0x04 });

    QCOMPARE(tpdu.size(), quint16(10));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataSystemBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::SystemNetworkParameterWrite);
}

void tst_QKnxTpduFactory::testNetworkParameterRead()
{
    auto tpdu = QKnxTpduFactory::Broadcast::createNetworkParameterReadTpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        { 0x01, 0x02, 0x03, 0x04 });

    QCOMPARE(tpdu.size(), quint16(9));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::NetworkParameterRead);

    tpdu = QKnxTpduFactory::Broadcast::createNetworkParameterReadTpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        QKnxByteArray(250, 1));
    QCOMPARE(tpdu.size(), quint16(255));
    QCOMPARE(tpdu.dataSize(), quint8(254));
}

void tst_QKnxTpduFactory::testNetworkParameterWrite()
{
    auto tpdu = QKnxTpduFactory::Broadcast::createNetworkParameterWriteTpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        { 0x01, 0x02, 0x03, 0x04 });

    QCOMPARE(tpdu.size(), quint16(9));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataBroadcast);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::NetworkParameterWrite);

    tpdu = QKnxTpduFactory::PointToPointConnectionless::createNetworkParameterWriteTpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        { 0x01, 0x02, 0x03, 0x04 });

    QCOMPARE(tpdu.size(), quint16(9));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataIndividual);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::NetworkParameterWrite);
}

void tst_QKnxTpduFactory::testNetworkParameterResponse()
{
   auto tpdu = QKnxTpduFactory::Broadcast::createNetworkParameterResponseTpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        {}, QKnxByteArray(11, 1));

    QCOMPARE(tpdu.size(), quint16(16));
    QCOMPARE(tpdu.dataSize(), quint8(15));

    // TODO: extend test
}

void tst_QKnxTpduFactory::testPropertyValueRead()
{
    auto tpdu = QKnxTpduFactory::PointToPoint::createPropertyValueReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1);

    QCOMPARE(tpdu.size(), quint16(6));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataIndividual);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::PropertyValueRead);

    quint8 sequenceNumber = 2;
    tpdu = QKnxTpduFactory::PointToPoint::createPropertyValueReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(6));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::PropertyValueRead);
}

void tst_QKnxTpduFactory::testPropertyValueWrite()
{
    QKnxByteArray data { 0x01, 0x02, 0x03, 0x04 };
    auto tpdu = QKnxTpduFactory::PointToPoint::createPropertyValueWriteTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, data);

    QCOMPARE(tpdu.size(), quint16(10));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataIndividual);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::PropertyValueWrite);

    //connection oriented
    quint8 sequenceNumber = 2;
    tpdu = QKnxTpduFactory::PointToPoint::createPropertyValueWriteTpdu(
        QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, data, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(10));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::PropertyValueWrite);
}

void tst_QKnxTpduFactory::testPropertyDescriptionRead()
{
    auto tpdu = QKnxTpduFactory::PointToPoint::createPropertyDescriptionReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1);

    QCOMPARE(tpdu.size(), quint16(5));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataIndividual);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::PropertyDescriptionRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    tpdu = QKnxTpduFactory::PointToPoint::createPropertyDescriptionReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(5));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::PropertyDescriptionRead);
}

void tst_QKnxTpduFactory::testFunctionPropertyCommand()
{
    QKnxByteArray data { 0x01, 0x02, 0x03, 0x04, 0x05 };
    auto tpdu = QKnxTpduFactory::PointToPoint::createFunctionPropertyCommandTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data);

    QCOMPARE(tpdu.size(), quint16(9));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataIndividual);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::FunctionPropertyCommand);

    //connection oriented
    quint8 sequenceNumber = 2;
    tpdu = tpdu = QKnxTpduFactory::PointToPoint::createFunctionPropertyCommandTpdu(
        QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(9));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::FunctionPropertyCommand);
}

void tst_QKnxTpduFactory::testFunctionPropertyStateRead()
{
    QKnxByteArray data { 0x01, 0x02, 0x03, 0x04, 0x05 };

    auto tpdu = QKnxTpduFactory::PointToPoint::createFunctionPropertyStateReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data);

    QCOMPARE(tpdu.size(), quint16(9));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataIndividual);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::FunctionPropertyStateRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    tpdu = tpdu = QKnxTpduFactory::PointToPoint::createFunctionPropertyStateReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(9));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::FunctionPropertyStateRead);
}

void tst_QKnxTpduFactory::testLinkRead()
{
    auto tpdu = QKnxTpduFactory::PointToPoint::createLinkReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, 0, 1);

    QCOMPARE(tpdu.size(), quint16(4));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataIndividual);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::LinkRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    tpdu = tpdu = QKnxTpduFactory::PointToPoint::createLinkReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented, 0, 1, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(4));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::LinkRead);
}

void tst_QKnxTpduFactory::testLinkWrite()
{
    QKnxAddress individualAddress = QKnxAddress::createIndividual(1, 1, 1);

    auto tpdu = QKnxTpduFactory::PointToPoint::createLinkWriteTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxTpdu::LinkWriteFlags::AddGroupAddress, individualAddress);

    QCOMPARE(tpdu.size(), quint16(2));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::Invalid);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::Invalid);

    QKnxAddress groupAddress = QKnxAddress::createGroup(1, 1);
    tpdu = QKnxTpduFactory::PointToPoint::createLinkWriteTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxTpdu::LinkWriteFlags::AddGroupAddress, groupAddress, 1);

    QCOMPARE(tpdu.size(), quint16(6));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataIndividual);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::LinkWrite);

    //connection oriented
    quint8 sequenceNumber = 2;
    tpdu = tpdu = QKnxTpduFactory::PointToPoint::createLinkWriteTpdu(
        QKnxTpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxTpdu::LinkWriteFlags::AddGroupAddress, groupAddress, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(6));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::LinkWrite);
}

void tst_QKnxTpduFactory::testDeviceDescriptorRead()
{
    quint8 descriptorType = 64;
    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, descriptorType);

    QCOMPARE(tpdu.size(), quint16(2));
    QCOMPARE(tpdu.transportControlField(),
        QKnxTpdu::TransportControlField::Invalid);
    QCOMPARE(tpdu.applicationControlField(),
        QKnxTpdu::ApplicationControlField::Invalid);

    tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorReadTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, 63);

    QCOMPARE(tpdu.size(), quint16(2));
    QCOMPARE(tpdu.transportControlField(),
        QKnxTpdu::TransportControlField::DataIndividual);
    QCOMPARE(tpdu.applicationControlField(),
        QKnxTpdu::ApplicationControlField::DeviceDescriptorRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    tpdu = tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorReadTpdu(QKnxTpduFactory
        ::PointToPoint::Mode::ConnectionOriented, 0, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(2));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::DeviceDescriptorRead);
}

void tst_QKnxTpduFactory::testDeviceDescriptorResponse()
{
    quint8 descriptorType = 63;
    auto tpdu = QKnxTpduFactory::PointToPoint::createDeviceDescriptorResponseTpdu(
        QKnxTpduFactory::PointToPoint::Mode::Connectionless, descriptorType, QKnxByteArray(253, 0));

    QCOMPARE(tpdu.size(), quint16(255));
    QCOMPARE(tpdu.dataSize(), quint8(254));

    // TODO: extend test
}

void tst_QKnxTpduFactory::testAdcRead()
{
    quint8 sequenceNumber = 2;
    quint8 channelNumber = 2;
    quint8 readCount = 40;
    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createAdcReadTpdu(channelNumber,
        readCount, sequenceNumber);
    QCOMPARE(tpdu.size(), quint16(3));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::AdcRead);
}

void tst_QKnxTpduFactory::testMemoryRead()
{
    quint8 sequenceNumber = 2;
    quint8 number = 2;
    quint16 memoryAddress = 0xabcd;
    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryReadTpdu(number,
        memoryAddress, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(4));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::MemoryRead);
}

void tst_QKnxTpduFactory::testMemoryWrite()
{
    quint8 sequenceNumber = 2;
    quint8 number = 2;
    quint16 memoryAddress = 0x1502;
    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createMemoryWriteTpdu(number,
        memoryAddress, { 0x01, 0x02, 0x03, 0x04, 0x05 }, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(9));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::MemoryWrite);
}

void tst_QKnxTpduFactory::testUserMemoryRead()
{
    quint8 sequenceNumber = 2;
    quint8 addressExtention = 1;
    quint8 number = 3;
    quint16 memoryAddress = 0xffff;
    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createUserMemoryReadTpdu(addressExtention,
        number, memoryAddress, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(5));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.data(), QKnxByteArray({ 0x13, 0xff, 0xff }));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::UserMemoryRead);
}

void tst_QKnxTpduFactory::testUserMemoryWrite()
{
    quint8 sequenceNumber = 2;
    quint8 addressExtention = 1;
    quint8 number = 2;
    quint16 memoryAddress = 0x0000;
    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createUserMemoryWriteTpdu(addressExtention,
        number, memoryAddress, { 0x01, 0x02, 0x03, 0x04, 0x05 }, sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(10));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.data(),QKnxByteArray({ 0x12, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::UserMemoryWrite);
}

void tst_QKnxTpduFactory::testUserManufacturerInfoRead()
{
    quint8 sequenceNumber = 2;
    auto tpdu = QKnxTpduFactory::PointToPointConnectionOriented::createUserManufacturerInfoReadTpdu(sequenceNumber);

    QCOMPARE(tpdu.size(), quint16(2));
    QCOMPARE(tpdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(tpdu.transportControlField(), QKnxTpdu::TransportControlField::DataConnected);
    QCOMPARE(tpdu.applicationControlField(), QKnxTpdu::ApplicationControlField::UserManufacturerInfoRead);
}

QTEST_APPLESS_MAIN(tst_QKnxTpduFactory)

#include "tst_qknxtpdufactory.moc"
