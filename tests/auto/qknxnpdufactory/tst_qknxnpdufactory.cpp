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

#include <QtKnx/qknxnpdufactory.h>
#include <QtTest/qtest.h>

class tst_QKnxNpduFactory : public QObject
{
    Q_OBJECT

private Q_SLOTS:
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

void tst_QKnxNpduFactory::testGroupValueRead()
{
    // Built it
    QKnxNpdu npdu = QKnxNpduFactory::createGroupValueReadNpdu();
    // print it

    // check the number of bytes
    QCOMPARE(quint8(npdu.size()), quint8(3));
    // check the TPCI
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataGroup);
    // check the APCI
    QCOMPARE(QKnxNpduFactory::applicationLayerControlField(npdu),
        QKnxCemi::ApplicationLayerControlField::GroupValueRead);
}

void tst_QKnxNpduFactory::testGroupValueWrite()
{
    // Built it
    QByteArray data = QByteArray::fromHex("01");
    QKnxNpdu npdu = QKnxNpduFactory::createGroupValueWriteNpdu(data);
    // print it

    // check the number of bytes
    QCOMPARE(quint8(npdu.size()), quint8(3));
    // check the TPCI
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataGroup);
    // check the APCI
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::GroupValueWrite));
}

void tst_QKnxNpduFactory::testGroupPropValueRead()
{
    QKnxNpdu npdu = QKnxNpduFactory::createGroupPropValueReadNpdu(QKnxInterfaceObjectType::System::Device,
        5, QKnxInterfaceObjectProperty::General::ManufacturerData);

    QCOMPARE(quint8(npdu.size()), quint8(7));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataTagGroup);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::GroupPropValueRead));
}

void tst_QKnxNpduFactory::testGroupPropValueWrite()
{
    QByteArray data = QByteArray::fromHex("010203");
    QKnxNpdu npdu = QKnxNpduFactory::createGroupPropValueWriteNpdu(QKnxInterfaceObjectType::System::Device,
        5, QKnxInterfaceObjectProperty::General::ManufacturerData, data);

    QCOMPARE(quint8(npdu.size()), quint8(10));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataTagGroup);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::GroupPropValueWrite));
}

void tst_QKnxNpduFactory::testIndividualAddressRead()
{
    QKnxNpdu npdu = QKnxNpduFactory::createIndividualAddressReadNpdu();

    QCOMPARE(quint8(npdu.size()), quint8(3));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::IndividualAddressRead));
}

void tst_QKnxNpduFactory::testIndividualAddressWrite()
{
    auto address = QKnxAddress::Group::Broadcast;
    auto npdu = QKnxNpduFactory::createIndividualAddressWriteNpdu(address);
    QCOMPARE(quint8(npdu.size()), quint8(0));

    address = QKnxAddress::createIndividual(1, 1, 1);
    npdu = QKnxNpduFactory::createIndividualAddressWriteNpdu(address);

    QCOMPARE(quint8(npdu.size()), quint8(5));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::IndividualAddressWrite));
    QCOMPARE(address.bytes(), QKnxAddress(QKnxAddress::Type::Individual, npdu.bytes(3, 2)).bytes());
}

void tst_QKnxNpduFactory::testIndividualAddressSerialNumberRead()
{
    auto serialNumber = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::createIndividualAddressSerialNumberReadNpdu(serialNumber);
    QCOMPARE(quint8(npdu.size()), quint8(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::createIndividualAddressSerialNumberReadNpdu(serialNumber);

    QCOMPARE(quint8(npdu.size()), quint8(9));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::IndividualAddressSerialNumberRead));
}

void tst_QKnxNpduFactory::testIndividualAddressSerialNumberWrite()
{
    auto address = QKnxAddress::Group::Broadcast;
    auto serialNumber = QByteArray::fromHex("010203");

    auto npdu = QKnxNpduFactory::createIndividualAddressSerialNumberWriteNpdu(serialNumber, address);
    QCOMPARE(quint8(npdu.size()), quint8(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::createIndividualAddressSerialNumberWriteNpdu(serialNumber, address);
    QCOMPARE(quint8(npdu.size()), quint8(0));

    address = QKnxAddress::createIndividual(1, 1, 1);
    npdu = QKnxNpduFactory::createIndividualAddressSerialNumberWriteNpdu(serialNumber, address);

    QCOMPARE(quint8(npdu.size()), quint8(11));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::IndividualAddressSerialNumberWrite));
}

void tst_QKnxNpduFactory::testDomainAddressRead()
{
    auto npdu = QKnxNpduFactory::createDomainAddressReadNpdu();

    QCOMPARE(quint8(npdu.size()), quint8(3));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::DomainAddressRead));
}

void tst_QKnxNpduFactory::testDomainAddressWrite()
{
    auto address = QKnxAddress::createGroup(1, 1, 1);
    auto npdu = QKnxNpduFactory::createDomainAddressWriteNpdu(address);

    QCOMPARE(quint8(npdu.size()), quint8(5));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::DomainAddressWrite));
}

void tst_QKnxNpduFactory::testDomainAddressSerialNumberRead()
{
    auto serialNumber = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::createDomainAddressSerialNumberReadNpdu(serialNumber);
    QCOMPARE(quint8(npdu.size()), quint8(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::createDomainAddressSerialNumberReadNpdu(serialNumber);

    QCOMPARE(quint8(npdu.size()), quint8(9));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::DomainAddressSerialNumberRead));
}

void tst_QKnxNpduFactory::testDomainAddressSerialNumberWrite()
{
    auto address = QKnxAddress::createGroup(1, 1, 1);
    auto serialNumber = QByteArray::fromHex("010203");
    auto npdu = QKnxNpduFactory::createDomainAddressSerialNumberWriteNpdu(serialNumber, address);
    QCOMPARE(quint8(npdu.size()), quint8(0));

    serialNumber = QByteArray::fromHex("010203040506");
    npdu = QKnxNpduFactory::createDomainAddressSerialNumberWriteNpdu(serialNumber, address);

    QCOMPARE(quint8(npdu.size()), quint8(11));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::DomainAddressSerialNumberWrite));
}

void tst_QKnxNpduFactory::testSystemNetworkParameterRead()
{
    QByteArray testInfo = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createNetworkParameterReadNpdu(QKnxNpduFactory::Network::System,
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags, testInfo);

    QCOMPARE(quint8(npdu.size()), quint8(11));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::SystemNetworkParameterRead));
}

void tst_QKnxNpduFactory::testSystemNetworkParameterWrite()
{
    QByteArray value = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createNetworkParameterWriteNpdu(QKnxNpduFactory::Network::System,
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags, value);

    QCOMPARE(quint8(npdu.size()), quint8(11));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataBroadcast);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::SystemNetworkParameterWrite));
}

void tst_QKnxNpduFactory::testNetworkParameterRead()
{
    QByteArray testInfo = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createNetworkParameterReadNpdu(QKnxNpduFactory::Network::Local,
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags, testInfo);

    QCOMPARE(quint8(npdu.size()), quint8(10));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::NetworkParameterRead));
}

void tst_QKnxNpduFactory::testNetworkParameterWrite()
{
    QByteArray value = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createNetworkParameterWriteNpdu(QKnxNpduFactory::Network::Local,
        QKnxInterfaceObjectType::System::Device, QKnxInterfaceObjectProperty::Device::ErrorFlags, value);

    QCOMPARE(quint8(npdu.size()), quint8(10));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::NetworkParameterWrite));
}

void tst_QKnxNpduFactory::testPropertyValueRead()
{
    QKnxNpdu npdu = QKnxNpduFactory::createPropertyValueReadNpdu(0, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        1, 1);

    QCOMPARE(quint8(npdu.size()), quint8(7));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::PropertyValueRead));

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = QKnxNpduFactory::createPropertyValueReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(7));
    // check the TPCI
    // add sequence Number to TPCI
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::PropertyValueRead));
}

void tst_QKnxNpduFactory::testPropertyValueWrite()
{
    QByteArray data = QByteArray::fromHex("01020304");
    QKnxNpdu npdu = QKnxNpduFactory::createPropertyValueWriteNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, 1, data);

    QCOMPARE(quint8(npdu.size()), quint8(11));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::PropertyValueWrite));

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = QKnxNpduFactory::createPropertyValueWriteNpdu(0, QKnxInterfaceObjectProperty::Device::ErrorFlags,
        1, 1, data, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(11));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::PropertyValueWrite));
}

void tst_QKnxNpduFactory::testPropertyDescriptionRead()
{
    QKnxNpdu npdu = QKnxNpduFactory::createPropertyDescriptionReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1);

    QCOMPARE(quint8(npdu.size()), quint8(6));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::PropertyDescriptionRead));

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = QKnxNpduFactory::createPropertyDescriptionReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, 1, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(6));
    // check the TPCI
    // add sequence Number to TPCI
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::PropertyDescriptionRead));
}

void tst_QKnxNpduFactory::testFunctionPropertyCommand()
{
    QByteArray data = QByteArray::fromHex("0102030405");
    QKnxNpdu npdu = QKnxNpduFactory::createFunctionPropertyCommandNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data);

    QCOMPARE(quint8(npdu.size()), quint8(10));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::FunctionPropertyCommand));

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createFunctionPropertyCommandNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(10));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::FunctionPropertyCommand));
}

void tst_QKnxNpduFactory::testFunctionPropertyStateRead()
{
    QByteArray data = QByteArray::fromHex("0102030405");
    QKnxNpdu npdu = QKnxNpduFactory::createFunctionPropertyStateReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data);

    QCOMPARE(quint8(npdu.size()), quint8(10));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::FunctionPropertyStateRead));

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createFunctionPropertyStateReadNpdu(0,
        QKnxInterfaceObjectProperty::Device::ErrorFlags, data, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(10));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    // check the APCI
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::FunctionPropertyStateRead));
}

void tst_QKnxNpduFactory::testLinkRead()
{
    QByteArray data = QByteArray::fromHex("0102030405");
    QKnxNpdu npdu = QKnxNpduFactory::createLinkReadNpdu(0, 1);

    QCOMPARE(quint8(npdu.size()), quint8(5));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::LinkRead));

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createLinkReadNpdu(0, 1, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(5));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::LinkRead));
}

void tst_QKnxNpduFactory::testLinkWrite()
{
    QKnxAddress individualAddress = QKnxAddress::createIndividual(1, 1, 1);
    QByteArray data = QByteArray::fromHex("0102030405");
    QKnxNpdu npdu = QKnxNpduFactory::createLinkWriteNpdu(0, 1, individualAddress);

    QCOMPARE(quint8(npdu.size()), quint8(0));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::Error);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::Error));

    QKnxAddress groupAddress = QKnxAddress::createGroup(1, 1);
    npdu = QKnxNpduFactory::createLinkWriteNpdu(0, 1, groupAddress);

    QCOMPARE(quint8(npdu.size()), quint8(7));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::LinkWrite));

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createLinkWriteNpdu(0, 1, groupAddress, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(7));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::LinkWrite));
}

void tst_QKnxNpduFactory::testDeviceDescriptorRead()
{
    quint8 descriptorType = 64;
    QKnxNpdu npdu = QKnxNpduFactory::createDeviceDescriptorReadNpdu(descriptorType);

    QCOMPARE(quint8(npdu.size()), quint8(0));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::Error);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::Error));

    npdu = QKnxNpduFactory::createDeviceDescriptorReadNpdu(63);

    QCOMPARE(quint8(npdu.size()), quint8(3));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu),
        QKnxCemi::TransportLayerControlField::DataIndividual);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::DeviceDescriptorRead));

    //connection oriented
    quint8 sequenceNumber = 2;
    npdu = npdu = QKnxNpduFactory::createDeviceDescriptorReadNpdu(0, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(3));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::DeviceDescriptorRead));
}

void tst_QKnxNpduFactory::testAdcRead()
{
    quint8 sequenceNumber = 2;
    quint8 channelNumber = 2;
    quint8 readCount = 40;
    QKnxNpdu npdu = QKnxNpduFactory::createAdcReadNpdu(sequenceNumber, channelNumber, readCount);

    QCOMPARE(quint8(npdu.size()), quint8(4));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::AdcRead));
}

void tst_QKnxNpduFactory::testMemoryRead()
{
    quint8 sequenceNumber = 2;
    quint8 number = 2;
    QKnxAddress memoryAddress = QKnxAddress::createIndividual(1, 1, 1);
    QKnxNpdu npdu = QKnxNpduFactory::createMemoryReadNpdu(number, memoryAddress, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(5));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    // check the APCI
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::MemoryRead));
}

void tst_QKnxNpduFactory::testMemoryWrite()
{
    QByteArray data = QByteArray::fromHex("0102030405");
    quint8 sequenceNumber = 2;
    quint8 number = 2;
    QKnxAddress memoryAddress = QKnxAddress::createIndividual(1, 1, 1);
    QKnxNpdu npdu = QKnxNpduFactory::createMemoryWriteNpdu(number, memoryAddress, data,
        sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(10));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::MemoryWrite));
}

void tst_QKnxNpduFactory::testUserMemoryRead()
{
    quint8 sequenceNumber = 2;
    quint8 addressExtention = 1;
    quint8 number = 2;
    QKnxAddress memoryAddress = QKnxAddress::createIndividual(1, 1, 1);
    QKnxNpdu npdu = QKnxNpduFactory::createUserMemoryReadNpdu(addressExtention, number,
        memoryAddress, sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(6));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::UserMemoryRead));
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

    QCOMPARE(quint8(npdu.size()), quint8(11));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::UserMemoryWrite));
}

void tst_QKnxNpduFactory::testUserManufacturerInfoRead()
{
    quint8 sequenceNumber = 2;
    QKnxNpdu npdu = QKnxNpduFactory::createUserManufacturerInfoReadNpdu(sequenceNumber);

    QCOMPARE(quint8(npdu.size()), quint8(3));
    std::bitset<8> tpci = quint8(QKnxCemi::TransportLayerControlField::DataConnected);
    std::bitset<8> sequenceNumberBits = quint8(sequenceNumber << 2);
    tpci[2] = sequenceNumberBits[2];
    tpci[3] = sequenceNumberBits[3];
    tpci[4] = sequenceNumberBits[4];
    tpci[5] = sequenceNumberBits[5];
    QKnxCemi::TransportLayerControlField fullTpci =
        QKnxCemi::TransportLayerControlField(quint8(tpci.to_ulong()));
    QCOMPARE(QKnxNpduFactory::transportLayerControlFied(npdu), fullTpci);
    QCOMPARE(quint8(QKnxNpduFactory::applicationLayerControlField(npdu)),
        quint8(QKnxCemi::ApplicationLayerControlField::UserManufacturerInfoRead));
}

QTEST_APPLESS_MAIN(tst_QKnxNpduFactory)

#include "tst_qknxnpdufactory.moc"
