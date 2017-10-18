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
    // TO FINISH : DomainAddressSelectiveRead
    void testDomainAddressSerialNumberRead();
    void testDomainAddressSerialNumberWrite();
    // TODO: DomainAddressSerialNumberResponse
    // TODO: add DeviceDescriptorInfoReport
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
    QKnxNpdu npdu;
    QCOMPARE(npdu.isValid(), false);

    npdu.setTransportLayerControlField(QKnxNpdu::TransportLayerControlField::DataGroup);
    QCOMPARE(npdu.transportLayerControlField(), QKnxNpdu::TransportLayerControlField::DataGroup);

    npdu.setApplicationLayerControlField(QKnxNpdu::ApplicationLayerControlField::GroupValueRead);
    QCOMPARE(npdu.transportLayerControlField(), QKnxNpdu::TransportLayerControlField::DataGroup);
    QCOMPARE(npdu.applicationLayerControlField(), QKnxNpdu::ApplicationLayerControlField::GroupValueRead);

    npdu.setTransportLayerControlField(QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.transportLayerControlField(), QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(), QKnxNpdu::ApplicationLayerControlField::GroupValueRead);

    npdu.setApplicationLayerControlField(QKnxNpdu::ApplicationLayerControlField::GroupValueWrite);
    QCOMPARE(npdu.transportLayerControlField(), QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(), QKnxNpdu::ApplicationLayerControlField::GroupValueWrite);

    QCOMPARE(npdu.isValid(), true);
    npdu.setData(QByteArray::fromHex("3f"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f"));

    npdu.setData(QByteArray::fromHex("43"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("43"));

    npdu = { QKnxNpdu::TransportLayerControlField::DataGroup,
        QKnxNpdu::ApplicationLayerControlField::GroupValueRead };
    QCOMPARE(npdu.transportLayerControlField(), QKnxNpdu::TransportLayerControlField::DataGroup);
    QCOMPARE(npdu.applicationLayerControlField(), QKnxNpdu::ApplicationLayerControlField::GroupValueRead);

    npdu = { QKnxNpdu::TransportLayerControlField::DataGroup,
        QKnxNpdu::ApplicationLayerControlField::GroupValueResponse };
    QCOMPARE(npdu.transportLayerControlField(), QKnxNpdu::TransportLayerControlField::DataGroup);
    QCOMPARE(npdu.applicationLayerControlField(), QKnxNpdu::ApplicationLayerControlField::GroupValueResponse);

    npdu.setData(QByteArray::fromHex("3f"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("01007f"));

    npdu.setTransportLayerControlField(QKnxNpdu::TransportLayerControlField::DataBroadcast);
    npdu.setApplicationLayerControlField(QKnxNpdu::ApplicationLayerControlField::GroupValueWrite);
    QCOMPARE(npdu.transportLayerControlField(), QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(), QKnxNpdu::ApplicationLayerControlField::GroupValueWrite);
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("0100bf"));

    npdu.setData(QByteArray::fromHex("43"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("43"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("02008043"));

    npdu = { QKnxNpdu::TransportLayerControlField::DataGroup,
        QKnxNpdu::ApplicationLayerControlField::GroupValueRead };
    QCOMPARE(npdu.transportLayerControlField(), QKnxNpdu::TransportLayerControlField::DataGroup);
    QCOMPARE(npdu.applicationLayerControlField(), QKnxNpdu::ApplicationLayerControlField::GroupValueRead);
    QCOMPARE(npdu.setData(QByteArray::fromHex("3f3f")), false);
    QCOMPARE(npdu.data(), QByteArray::fromHex("00"));

    npdu.setApplicationLayerControlField(QKnxNpdu::ApplicationLayerControlField::GroupValueWrite);
    QCOMPARE(npdu.setData(QByteArray::fromHex("3f3f")), true);
    npdu.setTransportLayerControlField(QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.transportLayerControlField(), QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(), QKnxNpdu::ApplicationLayerControlField::GroupValueWrite);
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f3f"));

    npdu.setData(QByteArray::fromHex("3f"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("3f"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("0100bf"));

    npdu.setData(QByteArray::fromHex("00"));
    QCOMPARE(npdu.data(), QByteArray::fromHex("0"));
    QCOMPARE(npdu.bytes(), QByteArray::fromHex("010080"));
}

void tst_QKnxNpduFactory::testGroupValueRead()
{
    // Built it
    QKnxNpdu npdu = QKnxNpduFactory::createGroupValueReadNpdu();
    // print it

    // check the number of bytes
    QCOMPARE(npdu.size(), quint16(3));
    // check the TPCI
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataGroup);
    // check the APCI
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::GroupValueRead);
}

void tst_QKnxNpduFactory::testGroupValueWrite()
{
    // Built it
    QByteArray data = QByteArray::fromHex("01");
    QKnxNpdu npdu = QKnxNpduFactory::createGroupValueWriteNpdu(data);
    // print it

    // check the number of bytes
    QCOMPARE(npdu.size(), quint16(3));
    // check the TPCI
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataGroup);
    // check the APCI
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::GroupValueWrite);
}

void tst_QKnxNpduFactory::testGroupPropValueRead()
{
    QKnxNpdu npdu = QKnxNpduFactory::createGroupPropValueReadNpdu(QKnxInterfaceObjectType::System::Device,
        5, QKnxInterfaceObjectProperty::General::ManufacturerData);

    QCOMPARE(npdu.size(), quint16(7));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataTagGroup);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::GroupPropValueRead);
}

void tst_QKnxNpduFactory::testGroupPropValueWrite()
{
    QByteArray data = QByteArray::fromHex("010203");
    QKnxNpdu npdu = QKnxNpduFactory::createGroupPropValueWriteNpdu(QKnxInterfaceObjectType::System::Device,
        5, QKnxInterfaceObjectProperty::General::ManufacturerData, data);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataTagGroup);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::GroupPropValueWrite);
}

void tst_QKnxNpduFactory::testIndividualAddressRead()
{
    QKnxNpdu npdu = QKnxNpduFactory::createIndividualAddressReadNpdu();

    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::IndividualAddressRead);
}

void tst_QKnxNpduFactory::testIndividualAddressWrite()
{
    auto address = QKnxAddress::Group::Broadcast;
    auto npdu = QKnxNpduFactory::createIndividualAddressWriteNpdu(address);
    QCOMPARE(npdu.size(), quint16(0));

    address = QKnxAddress::createIndividual(1, 1, 1);
    npdu = QKnxNpduFactory::createIndividualAddressWriteNpdu(address);

    QCOMPARE(npdu.size(), quint16(5));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::IndividualAddressWrite);
    QCOMPARE(address.bytes(), QKnxAddress(QKnxAddress::Type::Individual, npdu.bytes(3, 2)).bytes());
}

void tst_QKnxNpduFactory::testIndividualAddressSerialNumberRead()
{
    auto serialNumber = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::createIndividualAddressSerialNumberReadNpdu(serialNumber);
    QCOMPARE(npdu.size(), quint16(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::createIndividualAddressSerialNumberReadNpdu(serialNumber);

    QCOMPARE(npdu.size(), quint16(9));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::IndividualAddressSerialNumberRead);
}

void tst_QKnxNpduFactory::testIndividualAddressSerialNumberWrite()
{
    auto address = QKnxAddress::Group::Broadcast;
    auto serialNumber = QByteArray::fromHex("010203");

    auto npdu = QKnxNpduFactory::createIndividualAddressSerialNumberWriteNpdu(serialNumber, address);
    QCOMPARE(npdu.size(), quint16(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::createIndividualAddressSerialNumberWriteNpdu(serialNumber, address);
    QCOMPARE(npdu.size(), quint16(0));

    address = QKnxAddress::createIndividual(1, 1, 1);
    npdu = QKnxNpduFactory::createIndividualAddressSerialNumberWriteNpdu(serialNumber, address);

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::IndividualAddressSerialNumberWrite);
}

void tst_QKnxNpduFactory::testDomainAddressRead()
{
    auto npdu = QKnxNpduFactory::createDomainAddressReadNpdu();

    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::DomainAddressRead);
}

void tst_QKnxNpduFactory::testDomainAddressWrite()
{
    auto address = QKnxAddress::createGroup(1, 1, 1);
    auto npdu = QKnxNpduFactory::createDomainAddressWriteNpdu(address);

    QCOMPARE(npdu.size(), quint16(5));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::DomainAddressWrite);
}

void tst_QKnxNpduFactory::testDomainAddressSerialNumberRead()
{
    auto serialNumber = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::createDomainAddressSerialNumberReadNpdu(serialNumber);
    QCOMPARE(npdu.size(), quint16(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::createDomainAddressSerialNumberReadNpdu(serialNumber);

    QCOMPARE(npdu.size(), quint16(9));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::DomainAddressSerialNumberRead);
}

void tst_QKnxNpduFactory::testDomainAddressSerialNumberWrite()
{
    auto address = QKnxAddress::createGroup(1, 1, 1);
    auto serialNumber = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::createDomainAddressSerialNumberWriteNpdu(serialNumber, address);
    QCOMPARE(npdu.size(), quint16(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::createDomainAddressSerialNumberWriteNpdu(serialNumber, address);

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::DomainAddressSerialNumberWrite);
}

void tst_QKnxNpduFactory::testSystemNetworkParameterRead()
{
    QByteArray testInfo = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createNetworkParameterReadNpdu(QKnxNpduFactory::Network::System,
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags, testInfo);

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::SystemNetworkParameterRead);
}

void tst_QKnxNpduFactory::testSystemNetworkParameterWrite()
{
    QByteArray value = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createNetworkParameterWriteNpdu(QKnxNpduFactory::Network::System,
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags, value);

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataBroadcast);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::SystemNetworkParameterWrite);
}

void tst_QKnxNpduFactory::testNetworkParameterRead()
{
    QByteArray testInfo = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createNetworkParameterReadNpdu(QKnxNpduFactory::Network::Local,
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags, testInfo);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::NetworkParameterRead);
}

void tst_QKnxNpduFactory::testNetworkParameterWrite()
{
    QByteArray value = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createNetworkParameterWriteNpdu(QKnxNpduFactory::Network::Local,
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags, value);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::NetworkParameterWrite);
}

void tst_QKnxNpduFactory::testPropertyValueRead()
{
    QKnxNpdu npdu = QKnxNpduFactory::createPropertyValueReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1);

    QCOMPARE(npdu.size(), quint16(7));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::PropertyValueRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = QKnxNpduFactory::createPropertyValueReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(7));
    // check the TPCI
    // add sequence Number to TPCI
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::PropertyValueRead);
}

void tst_QKnxNpduFactory::testPropertyValueWrite()
{
    QByteArray data = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createPropertyValueWriteNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, data);

    QCOMPARE(npdu.size(), quint16(11));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::PropertyValueWrite);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = QKnxNpduFactory::createPropertyValueWriteNpdu(0, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        1, 1, data, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(11));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::PropertyValueWrite);
}

void tst_QKnxNpduFactory::testPropertyDescriptionRead()
{
    QKnxNpdu npdu = QKnxNpduFactory::createPropertyDescriptionReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1);

    QCOMPARE(npdu.size(), quint16(6));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::PropertyDescriptionRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = QKnxNpduFactory::createPropertyDescriptionReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(6));
    // check the TPCI
    // add sequence Number to TPCI
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::PropertyDescriptionRead);
}

void tst_QKnxNpduFactory::testFunctionPropertyCommand()
{
    QByteArray data = QByteArray::fromHex("0102030405");
    QKnxNpdu npdu = QKnxNpduFactory::createFunctionPropertyCommandNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::FunctionPropertyCommand);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createFunctionPropertyCommandNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(10));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::FunctionPropertyCommand);
}

void tst_QKnxNpduFactory::testFunctionPropertyStateRead()
{
    QByteArray data = QByteArray::fromHex("0102030405");
    QKnxNpdu npdu = QKnxNpduFactory::createFunctionPropertyStateReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data);

    QCOMPARE(npdu.size(), quint16(10));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::FunctionPropertyStateRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createFunctionPropertyStateReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(10));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    // check the APCI
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::FunctionPropertyStateRead);
}

void tst_QKnxNpduFactory::testLinkRead()
{
    QByteArray data = QByteArray::fromHex("0102030405");
    QKnxNpdu npdu = QKnxNpduFactory::createLinkReadNpdu(0, 1);

    QCOMPARE(npdu.size(), quint16(5));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::LinkRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createLinkReadNpdu(0, 1, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(5));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::LinkRead);
}

void tst_QKnxNpduFactory::testLinkWrite()
{
    QKnxAddress individualAddress = QKnxAddress::createIndividual(1, 1, 1);
    QByteArray data = QByteArray::fromHex("0102030405");
    QKnxNpdu npdu = QKnxNpduFactory::createLinkWriteNpdu(0, 1, individualAddress);

    QCOMPARE(npdu.size(), quint16(0));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::Invalid);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::Invalid);

    QKnxAddress groupAddress = QKnxAddress::createGroup(1, 1);
    npdu = QKnxNpduFactory::createLinkWriteNpdu(0, 1, groupAddress);

    QCOMPARE(npdu.size(), quint16(7));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::LinkWrite);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createLinkWriteNpdu(0, 1, groupAddress, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(7));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::LinkWrite);
}

void tst_QKnxNpduFactory::testDeviceDescriptorRead()
{
    quint8 descriptorType = 64;
    QKnxNpdu npdu = QKnxNpduFactory::createDeviceDescriptorReadNpdu(descriptorType);

    QCOMPARE(npdu.size(), quint16(0));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::Invalid);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::Invalid);

    npdu = QKnxNpduFactory::createDeviceDescriptorReadNpdu(63);

    QCOMPARE(npdu.size(), quint16(3));
    QCOMPARE(npdu.transportLayerControlField(),
        QKnxNpdu::TransportLayerControlField::DataIndividual);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::DeviceDescriptorRead);

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createDeviceDescriptorReadNpdu(0, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(3));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::DeviceDescriptorRead);
}

void tst_QKnxNpduFactory::testAdcRead()
{
    quint8 sequenceNumber = 2;
    quint8 channelNumber = 2;
    quint8 readCount = 40;
    QKnxNpdu npdu = QKnxNpduFactory::createAdcReadNpdu(sequenceNumber, channelNumber, readCount);
    QCOMPARE(npdu.size(), quint16(4));

    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::AdcRead);
}

void tst_QKnxNpduFactory::testMemoryRead()
{
    quint8 sequenceNumber = 2;
    quint8 number = 2;
    QKnxAddress memoryAddress = QKnxAddress::createIndividual(1, 1, 1);
    QKnxNpdu npdu = QKnxNpduFactory::createMemoryReadNpdu(number, memoryAddress, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(5));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    // check the APCI
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::MemoryRead);
}

void tst_QKnxNpduFactory::testMemoryWrite()
{
    QByteArray data = QByteArray::fromHex("0102030405");
    quint8 sequenceNumber = 2;
    quint8 number = 2;
    QKnxAddress memoryAddress = QKnxAddress::createIndividual(1, 1, 1);
    QKnxNpdu npdu = QKnxNpduFactory::createMemoryWriteNpdu(number, memoryAddress, data,
        sequenceNumber);

    QCOMPARE(npdu.size(), quint16(10));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::MemoryWrite);
}

void tst_QKnxNpduFactory::testUserMemoryRead()
{
    quint8 sequenceNumber = 2;
    quint8 addressExtention = 1;
    quint8 number = 2;
    QKnxAddress memoryAddress = QKnxAddress::createIndividual(1, 1, 1);
    QKnxNpdu npdu = QKnxNpduFactory::createUserMemoryReadNpdu(addressExtention, number,
        memoryAddress, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(6));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::UserMemoryRead);
}

void tst_QKnxNpduFactory::testUserMemoryWrite()
{
    QByteArray data = QByteArray::fromHex("0102030405");
    quint8 sequenceNumber = 2;
    quint8 addressExtention = 1;
    quint8 number = 2;
    QKnxAddress memoryAddress = QKnxAddress::createIndividual(1, 1, 1);
    QKnxNpdu npdu = QKnxNpduFactory::createUserMemoryWriteNpdu(addressExtention, number,
        memoryAddress, data, sequenceNumber);

    QCOMPARE(npdu.size(), quint16(11));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::UserMemoryWrite);
}

void tst_QKnxNpduFactory::testUserManufacturerInfoRead()
{
    quint8 sequenceNumber = 2;
    QKnxNpdu npdu = QKnxNpduFactory::createUserManufacturerInfoReadNpdu(sequenceNumber);

    QCOMPARE(npdu.size(), quint16(3));
    std::bitset<8> tpci = quint8(QKnxNpdu::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxNpdu::TransportLayerControlField fullTpci =
        QKnxNpdu::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(npdu.transportLayerControlField(), fullTpci);
    QCOMPARE(npdu.applicationLayerControlField(),
        QKnxNpdu::ApplicationLayerControlField::UserManufacturerInfoRead);
}

QTEST_APPLESS_MAIN(tst_QKnxNpduFactory)

#include "tst_qknxnpdufactory.moc"
