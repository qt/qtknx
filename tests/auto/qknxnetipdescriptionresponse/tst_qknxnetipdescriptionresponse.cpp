/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipdescriptionresponse.h>
#include <QtKnx/qknxnetipdevicedib.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpDescriptionResponse : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor()
    {
        QKnxNetIpDescriptionResponse descriptionResponseDefault;
        QCOMPARE(descriptionResponseDefault.isValid(),false);
        QCOMPARE(descriptionResponseDefault.size(), quint16(0));
        QCOMPARE(descriptionResponseDefault.bytes<QByteArray>(),QByteArray(""));
        QCOMPARE(descriptionResponseDefault.payload().size(), quint16(0));
        QCOMPARE(descriptionResponseDefault.payload().bytes<QByteArray>(), QByteArray(""));
        QCOMPARE(descriptionResponseDefault.toString(),QString::fromLatin1("Header size { 0x06 }, Version { 0x10 }, Code { 0x00 }, Total size { 0x00 }, Bytes {  }"));
    }

    void testConstructorWithArguments()
    {
        QKnxNetIpDeviceDIB deviceHardware(QKnxNetIpDeviceDIB::MediumCode::Ip,
                                     QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
                                     QKnxAddress::Individual::Unregistered,
                                     0x1111,
                                     QByteArray::fromHex("123456123456"),
                                     QHostAddress::AnyIPv4,
                                     QByteArray::fromHex("bcaec56690f9"),
                                     QByteArray("qt.io KNX device"));
        const QKnxNetIpServiceFamiliesDIB supportedFamilies(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling,
                                    0x04);
        QKnxNetIpDescriptionResponse descriptionResponse(deviceHardware, supportedFamilies);

        QCOMPARE(descriptionResponse.isValid(),true);
        QCOMPARE(descriptionResponse.size(), quint16(deviceHardware.size() + supportedFamilies.size()+ 6));
        QCOMPARE(descriptionResponse.payload().size(), quint16(deviceHardware.size() + supportedFamilies.size()));
        QCOMPARE(descriptionResponse.bytes<QByteArray>(),QByteArray::fromHex("061002040040") + QByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
                 + "qt.io KNX device" + QByteArray::fromHex("0000000000000000000000000000") + QByteArray::fromHex("04020404"));
        QCOMPARE(descriptionResponse.payload().bytes<QByteArray>(),QByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
                 + "qt.io KNX device" + QByteArray::fromHex("0000000000000000000000000000") + QByteArray::fromHex("04020404"));
        QCOMPARE(descriptionResponse.toString(),QString::fromLatin1("Header size { 0x06 }, Version { 0x10 }, Code { 0x204 }, Total size { 0x40 }, Bytes { 0x36, 0x01, 0x20, 0x01, 0xff, 0xff, 0x11, 0x11, 0x12, 0x34, 0x56, 0x12, 0x34, 0x56, 0x00, 0x00, 0x00, 0x00, 0xbc, 0xae, 0xc5, 0x66, 0x90, 0xf9, 0x71, 0x74, 0x2e, 0x69, 0x6f, 0x20, 0x4b, 0x4e, 0x58, 0x20, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x04, 0x04 }"));
    }

    void testDeviceHardwareAccessor()
    {
        QKnxNetIpDeviceDIB deviceHardware(QKnxNetIpDeviceDIB::MediumCode::Ip,
                                     QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
                                     QKnxAddress::Individual::Unregistered,
                                     0x1111,
                                     QByteArray::fromHex("123456123456"),
                                     QHostAddress::AnyIPv4,
                                     QByteArray::fromHex("bcaec56690f9"),
                                     QByteArray("qt.io KNX device"));
        const QKnxNetIpServiceFamiliesDIB supportedFamilies(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling,
                                    0x04);
        QKnxNetIpDescriptionResponse descriptionResponse(deviceHardware, supportedFamilies);

        QCOMPARE(descriptionResponse.deviceHardware().isValid(),deviceHardware.isValid());
        QCOMPARE(descriptionResponse.deviceHardware().mediumCode(), deviceHardware.mediumCode());
        QCOMPARE(descriptionResponse.deviceHardware().descriptionTypeCode(), deviceHardware.descriptionTypeCode());
        QCOMPARE(descriptionResponse.deviceHardware().deviceStatus(), deviceHardware.deviceStatus());
        QCOMPARE(descriptionResponse.deviceHardware().individualAddress().toString(), deviceHardware.individualAddress().toString());
        QCOMPARE(descriptionResponse.deviceHardware().projectInstallationIdentfier(), deviceHardware.projectInstallationIdentfier());
        QCOMPARE(descriptionResponse.deviceHardware().serialNumber(), deviceHardware.serialNumber());
        QCOMPARE(descriptionResponse.deviceHardware().multicastAddress(), deviceHardware.multicastAddress());
        QCOMPARE(descriptionResponse.deviceHardware().macAddress(), deviceHardware.macAddress());
        QCOMPARE(descriptionResponse.deviceHardware().deviceName(), deviceHardware.deviceName());

        QCOMPARE(descriptionResponse.deviceHardware().size(), deviceHardware.size());
        QCOMPARE(descriptionResponse.deviceHardware().bytes<QByteArray>(), deviceHardware.bytes<QByteArray>());
        QCOMPARE(descriptionResponse.deviceHardware().payload().size(), deviceHardware.payload().size());
        QCOMPARE(descriptionResponse.deviceHardware().payload().bytes<QByteArray>(), deviceHardware.payload().bytes<QByteArray>());
        QCOMPARE(descriptionResponse.deviceHardware().toString(), deviceHardware.toString());
    }

    void testSupportedFamiliesAccessor()
    {
        QKnxNetIpDeviceDIB deviceHardware(QKnxNetIpDeviceDIB::MediumCode::Ip,
                                          QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
                                          QKnxAddress::Individual::Unregistered,
                                          0x1111,
                                          QByteArray::fromHex("123456123456"),
                                          QHostAddress::AnyIPv4,
                                          QByteArray::fromHex("bcaec56690f9"),
                                          QByteArray("qt.io KNX device"));
        const QKnxNetIpServiceFamiliesDIB supportedFamilies(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling,
                                                            0x04);
        QKnxNetIpDescriptionResponse descriptionResponse(deviceHardware, supportedFamilies);

        QCOMPARE(descriptionResponse.supportedFamilies().isValid(),supportedFamilies.isValid());
        QCOMPARE(descriptionResponse.supportedFamilies().descriptionTypeCode(),supportedFamilies.descriptionTypeCode());

        QCOMPARE(descriptionResponse.supportedFamilies().size(), supportedFamilies.size());
        QCOMPARE(descriptionResponse.supportedFamilies().bytes<QByteArray>(), supportedFamilies.bytes<QByteArray>());
        QCOMPARE(descriptionResponse.supportedFamilies().payload().size(), supportedFamilies.payload().size());
        QCOMPARE(descriptionResponse.supportedFamilies().payload().bytes<QByteArray>(), supportedFamilies.payload().bytes<QByteArray>());
        QCOMPARE(descriptionResponse.supportedFamilies().toString(), supportedFamilies.toString());
    }

    void testOptionalDibs()
    {
        QKnxNetIpDeviceDIB deviceHardware(QKnxNetIpDeviceDIB::MediumCode::Ip,
                                     QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
                                     QKnxAddress::Individual::Unregistered,
                                     0x1111,
                                     QByteArray::fromHex("123456123456"),
                                     QHostAddress::AnyIPv4,
                                     QByteArray::fromHex("bcaec56690f9"),
                                     QByteArray("qt.io KNX device"));
        const QKnxNetIpServiceFamiliesDIB supportedFamilies(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling,
                                    0x04);
        QKnxNetIpDescriptionResponse descriptionResponse(deviceHardware, supportedFamilies);

        QCOMPARE(descriptionResponse.isValid(),true);

        QKnxNetIpConfigDIB configDIB(QHostAddress("192.168.2.12"),
                                     QHostAddress("255.255.255.0"),
                                     QHostAddress("192.168.2.1"),
                                     QKnxNetIpConfigDIB::Capability::AutoIp ,
                                     QKnxNetIpConfigDIB::AssignmentMethod::Manual);

        QKnxNetIpCurrentConfigDIB currentConfigDIB(QHostAddress("192.168.2.13"),
                                                   QHostAddress("255.255.255.1"),
                                                   QHostAddress("192.168.2.1"),
                                                   QHostAddress("192.168.2.2"),
                                                   QKnxNetIpCurrentConfigDIB::AssignmentMethod::BootP);
        QVector<QKnxAddress> qknxAdresses;
        qknxAdresses.append(QKnxAddress::createIndividual(1, 1, 0));
        qknxAdresses.append(QKnxAddress::createIndividual(1, 2, 5));
        qknxAdresses.append(QKnxAddress::createIndividual(2, 3, 8));
        QKnxNetIpKnxAddressesDIB knxAddressDIB(qknxAdresses);
        qDebug()<<"knxAddressDIB"<<knxAddressDIB.bytes();

        QByteArray byteArray(5,8);
        QKnxNetIpManufacturerDIB manufacturerDIB(256,byteArray);
        qDebug()<<"manufacturerDIB"<<manufacturerDIB.bytes();

        QKnxNetIpServiceFamiliesDIB serviceFamiliesDIB(QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteConfigAndDiagnosis, 3);
        qDebug()<<"serviceFamiliesDIB"<<serviceFamiliesDIB.bytes();

        descriptionResponse.addOptionalDib<QKnxNetIpConfigDIB>(configDIB);
        descriptionResponse.addOptionalDib<QKnxNetIpCurrentConfigDIB>(currentConfigDIB);
        descriptionResponse.addOptionalDib<QKnxNetIpKnxAddressesDIB>(knxAddressDIB);
        descriptionResponse.addOptionalDib<QKnxNetIpManufacturerDIB>(manufacturerDIB);
        descriptionResponse.addOptionalDib<QKnxNetIpServiceFamiliesDIB>(serviceFamiliesDIB);

        auto vectorOptionalDib = descriptionResponse.optionalDibs();

        // Checking optional DIB 1: QKnxNetIpConfigDIB
        QCOMPARE(configDIB.isValid(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().isValid());
        QCOMPARE(configDIB.size(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().size());
        QCOMPARE(configDIB.bytes<QByteArray>(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().bytes<QByteArray>());
        QCOMPARE(configDIB.payload().size(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().payload().size());
        QCOMPARE(configDIB.payload().bytes<QByteArray>(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().payload().bytes<QByteArray>());
        QCOMPARE(configDIB.toString(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().toString());

        QCOMPARE(configDIB.descriptionTypeCode(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().descriptionTypeCode());
        QCOMPARE(configDIB.ipAddress().toString(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().ipAddress().toString());
        QCOMPARE(configDIB.subnetMask().toString(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().subnetMask().toString());
        QCOMPARE(configDIB.defaultGateway().toString(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().defaultGateway().toString());
        QCOMPARE(configDIB.capabilities(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().capabilities());
        QCOMPARE(configDIB.assignmentMethods(),vectorOptionalDib[0].toType<QKnxNetIpConfigDIB>().assignmentMethods());

        // Checking optional DIB 2: QKnxNetIpCurrentConfigDIB
        QCOMPARE(currentConfigDIB.isValid(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().isValid());
        QCOMPARE(currentConfigDIB.size(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().size());
        QCOMPARE(currentConfigDIB.bytes<QByteArray>(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().bytes<QByteArray>());
        QCOMPARE(currentConfigDIB.payload().size(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().payload().size());
        QCOMPARE(currentConfigDIB.payload().bytes<QByteArray>(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().payload().bytes<QByteArray>());
        QCOMPARE(currentConfigDIB.toString(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().toString());

        QCOMPARE(currentConfigDIB.descriptionTypeCode(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().descriptionTypeCode());
        QCOMPARE(currentConfigDIB.ipAddress().toString(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().ipAddress().toString());
        QCOMPARE(currentConfigDIB.subnetMask().toString(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().subnetMask().toString());
        QCOMPARE(currentConfigDIB.defaultGateway().toString(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().defaultGateway().toString());
        QCOMPARE(currentConfigDIB.dhcpOrBootP().toString(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().dhcpOrBootP().toString());
        QCOMPARE(currentConfigDIB.assignmentMethod(),vectorOptionalDib[1].toType<QKnxNetIpCurrentConfigDIB>().assignmentMethod());

        // Checking optional DIB 3: QKnxNetIpKnxAddressesDIB
        QCOMPARE(knxAddressDIB.isValid(),vectorOptionalDib[2].toType<QKnxNetIpKnxAddressesDIB>().isValid());
        QCOMPARE(knxAddressDIB.size(),vectorOptionalDib[2].toType<QKnxNetIpKnxAddressesDIB>().size());
        QCOMPARE(knxAddressDIB.bytes<QByteArray>(),vectorOptionalDib[2].toType<QKnxNetIpKnxAddressesDIB>().bytes<QByteArray>());
        QCOMPARE(knxAddressDIB.payload().size(),vectorOptionalDib[2].toType<QKnxNetIpKnxAddressesDIB>().payload().size());
        QCOMPARE(knxAddressDIB.payload().bytes<QByteArray>(),vectorOptionalDib[2].toType<QKnxNetIpKnxAddressesDIB>().payload().bytes<QByteArray>());
        QCOMPARE(knxAddressDIB.toString(),vectorOptionalDib[2].toType<QKnxNetIpKnxAddressesDIB>().toString());

        QCOMPARE(knxAddressDIB.descriptionTypeCode(),vectorOptionalDib[2].toType<QKnxNetIpKnxAddressesDIB>().descriptionTypeCode());
        QCOMPARE(knxAddressDIB.individualAddresses< QVector<QKnxAddress> >().size(),
                 vectorOptionalDib[2].toType<QKnxNetIpKnxAddressesDIB>().individualAddresses< QVector<QKnxAddress> >().size());

        // Checking optional DIB 4: QKnxNetIpManufacturerDIB
        QCOMPARE(manufacturerDIB.isValid(),vectorOptionalDib[3].toType<QKnxNetIpManufacturerDIB>().isValid());
        QCOMPARE(manufacturerDIB.size(),vectorOptionalDib[3].toType<QKnxNetIpManufacturerDIB>().size());
        QCOMPARE(manufacturerDIB.bytes<QByteArray>(),vectorOptionalDib[3].toType<QKnxNetIpManufacturerDIB>().bytes<QByteArray>());
        QCOMPARE(manufacturerDIB.payload().size(),vectorOptionalDib[3].toType<QKnxNetIpManufacturerDIB>().payload().size());
        QCOMPARE(manufacturerDIB.payload().bytes<QByteArray>(),vectorOptionalDib[3].toType<QKnxNetIpManufacturerDIB>().payload().bytes<QByteArray>());
        QCOMPARE(manufacturerDIB.toString(),vectorOptionalDib[3].toType<QKnxNetIpManufacturerDIB>().toString());

        QCOMPARE(manufacturerDIB.descriptionTypeCode(),vectorOptionalDib[3].toType<QKnxNetIpManufacturerDIB>().descriptionTypeCode());
        QCOMPARE(manufacturerDIB.manufacturerId(),vectorOptionalDib[3].toType<QKnxNetIpManufacturerDIB>().manufacturerId());
        QCOMPARE(manufacturerDIB.manufacturerData<QByteArray>(),vectorOptionalDib[3].toType<QKnxNetIpManufacturerDIB>().manufacturerData<QByteArray>());

        // Checking optional DIB 5: QKnxNetIpServiceFamiliesDIB
        QCOMPARE(serviceFamiliesDIB.isValid(),vectorOptionalDib[4].toType<QKnxNetIpServiceFamiliesDIB>().isValid());
        QCOMPARE(serviceFamiliesDIB.size(),vectorOptionalDib[4].toType<QKnxNetIpServiceFamiliesDIB>().size());
        QCOMPARE(serviceFamiliesDIB.bytes<QByteArray>(),vectorOptionalDib[4].toType<QKnxNetIpServiceFamiliesDIB>().bytes<QByteArray>());
        QCOMPARE(serviceFamiliesDIB.payload().size(),vectorOptionalDib[4].toType<QKnxNetIpServiceFamiliesDIB>().payload().size());
        QCOMPARE(serviceFamiliesDIB.payload().bytes<QByteArray>(),vectorOptionalDib[4].toType<QKnxNetIpServiceFamiliesDIB>().payload().bytes<QByteArray>());
        QCOMPARE(serviceFamiliesDIB.toString(),vectorOptionalDib[4].toType<QKnxNetIpServiceFamiliesDIB>().toString());

        QCOMPARE(serviceFamiliesDIB.descriptionTypeCode(),vectorOptionalDib[4].toType<QKnxNetIpServiceFamiliesDIB>().descriptionTypeCode());
    }

    void testDebugStream()
    {
        // TODO: Implement.
    }

    void testDataStream()
    {
        // TODO: Implement.
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpDescriptionResponse)

#include "tst_qknxnetipdescriptionresponse.moc"
