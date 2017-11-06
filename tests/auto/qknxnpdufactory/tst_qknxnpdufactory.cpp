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

#include <QtKnx/qknxnpdufactory.h>
#include <QtKnx/qknxtunnelframe.h>
#include <QtTest/qtest.h>

class tst_QKnxNpduFactory : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testNpdu();
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
    // TODO: NetworkParameterResponse
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
    // TODO: DeviceDescriptorResponse
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

void tst_QKnxNpduFactory::testNpdu()
{
    QKnxNpdu npdu1;
    npdu1.setTransportControlField(QKnxNpdu::TransportControlField::DataGroup);
    npdu1.setApplicationControlField(QKnxNpdu::ApplicationControlField::GroupValueWrite);
    npdu1.setData(QByteArray::fromHex("01"));

    QKnxTunnelFrame frame;
    frame.setNpdu(npdu1);
    QCOMPARE(npdu1.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(frame.npdu().data(), QByteArray::fromHex("01"));

    QKnxNpdu npdu;
    QCOMPARE(npdu.isValid(), false);

    npdu.setTransportControlField(QKnxNpdu::TransportControlField::DataGroup);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataGroup);

    npdu.setApplicationControlField(QKnxNpdu::ApplicationControlField::GroupValueRead);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataGroup);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueRead);

    npdu.setTransportControlField(QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueRead);

    npdu.setApplicationControlField(QKnxNpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueWrite);

    QCOMPARE(npdu.isValid(), true);
    npdu.setData(QByteArray::fromHex("3f"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f"));

    npdu.setData(QByteArray::fromHex("43"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("43"));

    npdu = { QKnxNpdu::TransportControlField::DataGroup,
        QKnxNpdu::ApplicationControlField::GroupValueRead };
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataGroup);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueRead);

    npdu = { QKnxNpdu::TransportControlField::DataGroup,
        QKnxNpdu::ApplicationControlField::GroupValueResponse };
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataGroup);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueResponse);

    npdu.setData(QByteArray::fromHex("3f"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("01007f"));

    npdu.setTransportControlField(QKnxNpdu::TransportControlField::DataBroadcast);
    npdu.setApplicationControlField(QKnxNpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("0100bf"));

    npdu.setData(QByteArray::fromHex("43"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("43"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("02008043"));

    npdu = { QKnxNpdu::TransportControlField::DataGroup,
        QKnxNpdu::ApplicationControlField::GroupValueRead };
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataGroup);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueRead);

    npdu.setData(QByteArray::fromHex("3f3f"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f3f"));
    QCOMPARE(npdu.isValid(), false);

    npdu.setData({});
    QCOMPARE(npdu.isValid(), true);
    QCOMPARE(npdu.data(), QByteArray{});

    npdu.setApplicationControlField(QKnxNpdu::ApplicationControlField::GroupValueWrite);
    npdu.setData(QByteArray::fromHex("3f3f"));
    QCOMPARE(npdu.isValid(), true);
    npdu.setTransportControlField(QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueWrite);
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f3f"));

    npdu.setData(QByteArray::fromHex("3f"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("0100bf"));

    npdu.setData(QByteArray::fromHex("00"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("0"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("010080"));

    auto tmpNpdu = QKnxNpdu::fromBytes(npdu.bytes(), 0, quint8(npdu.size()));
    QCOMPARE(tmpNpdu.bytes(), npdu.bytes());
}

void tst_QKnxNpduFactory::testGroupValueRead()
{
    auto npdu = QKnxNpduFactory::Multicast::createGroupValueReadNpdu();

    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataGroup);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueRead);
}

void tst_QKnxNpduFactory::testGroupValueWrite()
{
    auto data = QByteArray::fromHex("01");
    auto npdu = QKnxNpduFactory::Multicast::createGroupValueWriteNpdu(data);

    QCOMPARE(npdu.data(), data);
    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataGroup);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupValueWrite);

    npdu.setData(QByteArray::fromHex("0101"));
    QCOMPARE(npdu.size(), quint16(5));
    QCOMPARE(npdu.data(), QByteArray::fromHex("0101"));

    npdu.setData(QByteArray::fromHex("40"));
    QCOMPARE(npdu.size(), quint16(4));
    QCOMPARE(npdu.data(), QByteArray::fromHex("40"));

    npdu.setData(QByteArray::fromHex("f0"));
    QCOMPARE(npdu.size(), quint16(4));
    QCOMPARE(npdu.data(), QByteArray::fromHex("f0"));

    npdu.setData(QByteArray::fromHex("ff"));
    QCOMPARE(npdu.size(), quint16(4));
    QCOMPARE(npdu.data(), QByteArray::fromHex("ff"));

}

void tst_QKnxNpduFactory::testGroupPropValueRead()
{
    auto npdu = QKnxNpduFactory::Multicast::createGroupPropertyValueReadNpdu(QKnxInterfaceObjectType
        ::System::Device, 5, QKnxInterfaceObjectProperty::General::ManufacturerData);

    QCOMPARE(npdu.size(), quint16(7));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataTagGroup);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupPropValueRead);
}

void tst_QKnxNpduFactory::testGroupPropValueWrite()
{
    auto data = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::Multicast::createGroupPropertyValueWriteNpdu(QKnxInterfaceObjectType
        ::System::Device, 5, QKnxInterfaceObjectProperty::General::ManufacturerData, data);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.data(), QByteArray::fromHex("00001305010203"));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataTagGroup);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::GroupPropValueWrite);
}

void tst_QKnxNpduFactory::testIndividualAddressRead()
{
    auto npdu = QKnxNpduFactory::Broadcast::createIndividualAddressReadNpdu();

    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::IndividualAddressRead);
}

void tst_QKnxNpduFactory::testIndividualAddressWrite()
{
    auto address = QKnxAddress::Group::Broadcast;
    auto npdu = QKnxNpduFactory::Broadcast::createIndividualAddressWriteNpdu(address);
    QCOMPARE(npdu.size(), quint16(0));

    address = QKnxAddress::createIndividual(1, 1, 1);
    npdu = QKnxNpduFactory::Broadcast::createIndividualAddressWriteNpdu(address);

    QCOMPARE(npdu.size(), quint16(5));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::IndividualAddressWrite);
    QCOMPARE(QKnxAddress(QKnxAddress::Type::Individual, npdu.data()), address);
}

void tst_QKnxNpduFactory::testIndividualAddressSerialNumberRead()
{
    auto serialNumber = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::Broadcast::createIndividualAddressSerialNumberReadNpdu(serialNumber);
    QCOMPARE(npdu.size(), quint16(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::Broadcast::createIndividualAddressSerialNumberReadNpdu(serialNumber);

    QCOMPARE(npdu.size(), quint16(9));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::IndividualAddressSerialNumberRead);
}

void tst_QKnxNpduFactory::testIndividualAddressSerialNumberWrite()
{
    auto address = QKnxAddress::Group::Broadcast;
    auto serialNumber = QByteArray::fromHex("010203");

    auto npdu = QKnxNpduFactory::Broadcast::createIndividualAddressSerialNumberWriteNpdu(serialNumber, address);
    QCOMPARE(npdu.size(), quint16(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::Broadcast::createIndividualAddressSerialNumberWriteNpdu(serialNumber, address);
    QCOMPARE(npdu.size(), quint16(0));

    address = QKnxAddress::createIndividual(1, 1, 1);
    npdu = QKnxNpduFactory::Broadcast::createIndividualAddressSerialNumberWriteNpdu(serialNumber, address);

    QCOMPARE(npdu.size(), quint16(15));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::IndividualAddressSerialNumberWrite);
}

void tst_QKnxNpduFactory::testDomainAddressRead()
{
    auto npdu = QKnxNpduFactory::Broadcast::createDomainAddressReadNpdu();

    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::DomainAddressRead);
}

void tst_QKnxNpduFactory::testDomainAddressWrite()
{
    auto address = QKnxAddress::createGroup(1, 1, 1);
    auto npdu = QKnxNpduFactory::Broadcast::createDomainAddressWriteNpdu(address.bytes());

    QCOMPARE(npdu.size(), quint16(5));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::DomainAddressWrite);
}

void tst_QKnxNpduFactory::testDomainAddressSerialNumberRead()
{
    auto serialNumber = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::Broadcast::createDomainAddressSerialNumberReadNpdu(serialNumber);
    QCOMPARE(npdu.size(), quint16(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::Broadcast::createDomainAddressSerialNumberReadNpdu(serialNumber);

    QCOMPARE(npdu.size(), quint16(9));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::DomainAddressSerialNumberRead);
}

void tst_QKnxNpduFactory::testDomainAddressSerialNumberWrite()
{
    auto address = QKnxAddress::createGroup(1, 1, 1).bytes();
    auto serialNumber = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::Broadcast::createDomainAddressSerialNumberWriteNpdu(serialNumber, address);
    QCOMPARE(npdu.size(), quint16(0));

    npdu = QKnxNpduFactory::Broadcast::createDomainAddressSerialNumberWriteNpdu(serialNumber, serialNumber);
    QCOMPARE(npdu.size(), quint16(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::Broadcast::createDomainAddressSerialNumberWriteNpdu(serialNumber, address);

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::DomainAddressSerialNumberWrite);
}

void tst_QKnxNpduFactory::testSystemNetworkParameterRead()
{
    auto npdu = QKnxNpduFactory::Broadcast::createSystemNetworkParameterReadNpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        QByteArray::fromHex("01020304"));

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataSystemBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::SystemNetworkParameterRead);
}

void tst_QKnxNpduFactory::testSystemNetworkParameterWrite()
{
    auto npdu = QKnxNpduFactory::Broadcast::createSystemNetworkParameterWriteNpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        QByteArray::fromHex("01020304"));

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataSystemBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::SystemNetworkParameterWrite);
}

void tst_QKnxNpduFactory::testNetworkParameterRead()
{
    auto npdu = QKnxNpduFactory::Broadcast::createNetworkParameterReadNpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        QByteArray::fromHex("01020304"));

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::NetworkParameterRead);
}

void tst_QKnxNpduFactory::testNetworkParameterWrite()
{
    auto npdu = QKnxNpduFactory::Broadcast::createNetworkParameterWriteNpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        QByteArray::fromHex("01020304"));

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataBroadcast);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::NetworkParameterWrite);

    npdu = QKnxNpduFactory::PointToPointConnectionless::createNetworkParameterWriteNpdu(
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        QByteArray::fromHex("01020304"));

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataIndividual);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::NetworkParameterWrite);
}

void tst_QKnxNpduFactory::testPropertyValueRead()
{
    auto npdu = QKnxNpduFactory::PointToPoint::createPropertyValueReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1);

    QCOMPARE(npdu.size(), quint16(7));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataIndividual);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::PropertyValueRead);

    quint8 sequenceNumber = 2;
    npdu = QKnxNpduFactory::PointToPoint::createPropertyValueReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(7));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::PropertyValueRead);
}

void tst_QKnxNpduFactory::testPropertyValueWrite()
{
    auto data = QByteArray::fromHex("01020304");
    auto npdu = QKnxNpduFactory::PointToPoint::createPropertyValueWriteNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, data);

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataIndividual);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::PropertyValueWrite);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = QKnxNpduFactory::PointToPoint::createPropertyValueWriteNpdu(
        QKnxNpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, data, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::PropertyValueWrite);
}

void tst_QKnxNpduFactory::testPropertyDescriptionRead()
{
    auto npdu = QKnxNpduFactory::PointToPoint::createPropertyDescriptionReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1);

    QCOMPARE(npdu.size(), quint16(6));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataIndividual);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::PropertyDescriptionRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = QKnxNpduFactory::PointToPoint::createPropertyDescriptionReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(6));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::PropertyDescriptionRead);
}

void tst_QKnxNpduFactory::testFunctionPropertyCommand()
{
    auto data = QByteArray::fromHex("0102030405");
    auto npdu = QKnxNpduFactory::PointToPoint::createFunctionPropertyCommandNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataIndividual);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::FunctionPropertyCommand);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::PointToPoint::createFunctionPropertyCommandNpdu(
        QKnxNpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::FunctionPropertyCommand);
}

void tst_QKnxNpduFactory::testFunctionPropertyStateRead()
{
    auto data = QByteArray::fromHex("0102030405");
    auto npdu = QKnxNpduFactory::PointToPoint::createFunctionPropertyStateReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataIndividual);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::FunctionPropertyStateRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::PointToPoint::createFunctionPropertyStateReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::FunctionPropertyStateRead);
}

void tst_QKnxNpduFactory::testLinkRead()
{
    auto data = QByteArray::fromHex("0102030405");
    auto npdu = QKnxNpduFactory::PointToPoint::createLinkReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, 0, 1);

    QCOMPARE(npdu.size(), quint16(5));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataIndividual);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::LinkRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::PointToPoint::createLinkReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::ConnectionOriented, 0, 1, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(5));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::LinkRead);
}

void tst_QKnxNpduFactory::testLinkWrite()
{
    QKnxAddress individualAddress = QKnxAddress::createIndividual(1, 1, 1);
    auto data = QByteArray::fromHex("0102030405");
    auto npdu = QKnxNpduFactory::PointToPoint::createLinkWriteNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxNpdu::LinkWriteFlags::AddGroupAddress, individualAddress);

    QCOMPARE(npdu.size(), quint16(0));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::Invalid);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::Invalid);

    QKnxAddress groupAddress = QKnxAddress::createGroup(1, 1);
    npdu = QKnxNpduFactory::PointToPoint::createLinkWriteNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, 0,
        QKnxNpdu::LinkWriteFlags::AddGroupAddress, groupAddress, 1);

    QCOMPARE(npdu.size(), quint16(7));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataIndividual);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::LinkWrite);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::PointToPoint::createLinkWriteNpdu(
        QKnxNpduFactory::PointToPoint::Mode::ConnectionOriented, 0,
        QKnxNpdu::LinkWriteFlags::AddGroupAddress, groupAddress, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(7));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::LinkWrite);
}

void tst_QKnxNpduFactory::testDeviceDescriptorRead()
{
    quint8 descriptorType = 64;
    auto npdu = QKnxNpduFactory::PointToPoint::createDeviceDescriptorReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, descriptorType);

    QCOMPARE(npdu.size(), quint16(0));
    QCOMPARE(npdu.transportControlField(),
        QKnxNpdu::TransportControlField::Invalid);
    QCOMPARE(npdu.applicationControlField(),
        QKnxNpdu::ApplicationControlField::Invalid);

    npdu = QKnxNpduFactory::PointToPoint::createDeviceDescriptorReadNpdu(
        QKnxNpduFactory::PointToPoint::Mode::Connectionless, 63);

    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.transportControlField(),
        QKnxNpdu::TransportControlField::DataIndividual);
    QCOMPARE(npdu.applicationControlField(),
        QKnxNpdu::ApplicationControlField::DeviceDescriptorRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::PointToPoint::createDeviceDescriptorReadNpdu(QKnxNpduFactory
        ::PointToPoint::Mode::ConnectionOriented, 0, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::DeviceDescriptorRead);
}

void tst_QKnxNpduFactory::testAdcRead()
{
    quint8 sequenceNumber = 2;
    quint8 channelNumber = 2;
    quint8 readCount = 40;
    auto npdu = QKnxNpduFactory::PointToPointConnectionOriented::createAdcReadNpdu(channelNumber,
        readCount, sequenceNumber);
    QCOMPARE(npdu.size(), quint16(4));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::AdcRead);
}

void tst_QKnxNpduFactory::testMemoryRead()
{
    quint8 sequenceNumber = 2;
    quint8 number = 2;
    quint16 memoryAddress = 0xabcd;
    auto npdu = QKnxNpduFactory::PointToPoint::createMemoryReadNpdu(QKnxNpduFactory::PointToPoint
        ::Mode::ConnectionOriented, number, memoryAddress, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(5));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::MemoryRead);
}

void tst_QKnxNpduFactory::testMemoryWrite()
{
    auto data = QByteArray::fromHex("0102030405");
    quint8 sequenceNumber = 2;
    quint8 number = 2;
    quint16 memoryAddress = 0x1502;
    auto npdu = QKnxNpduFactory::PointToPoint::createMemoryWriteNpdu(QKnxNpduFactory::PointToPoint
        ::Mode::ConnectionOriented, number, memoryAddress, data,
        sequenceNumber);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::MemoryWrite);
}

void tst_QKnxNpduFactory::testUserMemoryRead()
{
    quint8 sequenceNumber = 2;
    quint8 addressExtention = 1;
    quint8 number = 3;
    quint16 memoryAddress = 0xffff;
    auto npdu = QKnxNpduFactory::PointToPointConnectionOriented::createUserMemoryReadNpdu(addressExtention,
        number, memoryAddress, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(6));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.data(), QByteArray::fromHex("13FFFF"));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::UserMemoryRead);
}

void tst_QKnxNpduFactory::testUserMemoryWrite()
{
    auto data = QByteArray::fromHex("0102030405");
    quint8 sequenceNumber = 2;
    quint8 addressExtention = 1;
    quint8 number = 2;
    quint16 memoryAddress = 0x0000;
    auto npdu = QKnxNpduFactory::PointToPointConnectionOriented::createUserMemoryWriteNpdu(addressExtention,
        number, memoryAddress, data, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.data(), QByteArray::fromHex("1200000102030405"));
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::UserMemoryWrite);
}

void tst_QKnxNpduFactory::testUserManufacturerInfoRead()
{
    quint8 sequenceNumber = 2;
    auto npdu = QKnxNpduFactory::PointToPointConnectionOriented::createUserManufacturerInfoReadNpdu(sequenceNumber);

    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.sequenceNumber(), sequenceNumber);
    QCOMPARE(npdu.transportControlField(), QKnxNpdu::TransportControlField::DataConnected);
    QCOMPARE(npdu.applicationControlField(), QKnxNpdu::ApplicationControlField::UserManufacturerInfoRead);
}

QTEST_APPLESS_MAIN(tst_QKnxNpduFactory)

#include "tst_qknxnpdufactory.moc"
