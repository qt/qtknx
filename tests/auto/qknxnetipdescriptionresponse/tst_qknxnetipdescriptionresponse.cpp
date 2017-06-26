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

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipdescriptionresponse.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDescriptionResponse : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testDefaultConstructor();
    void testConstructorWithArguments();
    void testDeviceHardwareAccessor();
    void testSupportedFamiliesAccessor();
    void testOptionalDibs();
    void testDebugStream();
    void testDataStream();

private:
    QKnxNetIpServiceFamiliesDIB m_sf;
    QKnxNetIpDeviceDIB m_deviceHardware;
};

void tst_QKnxNetIpDescriptionResponse::initTestCase()
{
    m_sf = { QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 0x04 };
    m_deviceHardware = { QKnxNetIpDeviceDIB::Medium::Ip,
                         QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
                         QKnxAddress::Individual::Unregistered,
                         0x1111,
                         QByteArray::fromHex("123456123456"),
                         QHostAddress::AnyIPv4,
                         QByteArray::fromHex("bcaec56690f9"),
                         QByteArray("qt.io KNX device")
    };
}

void tst_QKnxNetIpDescriptionResponse::testDefaultConstructor()
{
    QKnxNetIpDescriptionResponse descriptionResponseDefault;
    QCOMPARE(descriptionResponseDefault.isValid(), false);
    QCOMPARE(descriptionResponseDefault.size(), quint16(0));
    QCOMPARE(descriptionResponseDefault.bytes<QByteArray>(), QByteArray {});
    QCOMPARE(descriptionResponseDefault.payload().size(), quint16(0));
    QCOMPARE(descriptionResponseDefault.payload().bytes<QByteArray>(), QByteArray {});
    QCOMPARE(descriptionResponseDefault.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x00 }, Total size { 0x00 }, Bytes {  }"));
}

void tst_QKnxNetIpDescriptionResponse::testConstructorWithArguments()
{
    QKnxNetIpDescriptionResponse descriptionResponse(m_deviceHardware, m_sf);

    QCOMPARE(descriptionResponse.isValid(), true);
    QCOMPARE(descriptionResponse.size(), quint16(m_deviceHardware.size() + m_sf.size() + 6));
    QCOMPARE(descriptionResponse.payload().size(), quint16(m_deviceHardware.size() + m_sf.size()));
    QCOMPARE(descriptionResponse.bytes<QByteArray>(), QByteArray::fromHex("061002040040")
        + QByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
        + "qt.io KNX device" + QByteArray::fromHex("0000000000000000000000000000")
        + QByteArray::fromHex("04020404"));
    QCOMPARE(descriptionResponse.payload().bytes<QByteArray>(),
        QByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
        + "qt.io KNX device" + QByteArray::fromHex("0000000000000000000000000000")
        + QByteArray::fromHex("04020404"));
    QCOMPARE(descriptionResponse.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x204 }, Total size { 0x40 }, Bytes { 0x36, 0x01, 0x20, "
            "0x01, 0xff, 0xff, 0x11, 0x11, 0x12, 0x34, 0x56, 0x12, 0x34, 0x56, 0x00, 0x00, 0x00, "
            "0x00, 0xbc, 0xae, 0xc5, 0x66, 0x90, 0xf9, 0x71, 0x74, 0x2e, 0x69, 0x6f, 0x20, 0x4b, "
            "0x4e, 0x58, 0x20, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, "
            "0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x04, 0x04 }"));
}

void tst_QKnxNetIpDescriptionResponse::testDeviceHardwareAccessor()
{
    QKnxNetIpDescriptionResponse descriptionResponse(m_deviceHardware, m_sf);

    auto hardware = descriptionResponse.deviceHardware();
    QCOMPARE(hardware.isValid(), m_deviceHardware.isValid());
    QCOMPARE(hardware.medium(), m_deviceHardware.medium());
    QCOMPARE(hardware.descriptionType(), m_deviceHardware.descriptionType());
    QCOMPARE(hardware.deviceStatus(), m_deviceHardware.deviceStatus());
    QCOMPARE(hardware.individualAddress().toString(),
        m_deviceHardware.individualAddress().toString());
    QCOMPARE(hardware.projectInstallationIdentfier(),
        m_deviceHardware.projectInstallationIdentfier());
    QCOMPARE(hardware.serialNumber(), m_deviceHardware.serialNumber());
    QCOMPARE(hardware.multicastAddress(), m_deviceHardware.multicastAddress());
    QCOMPARE(hardware.macAddress(), m_deviceHardware.macAddress());
    QCOMPARE(hardware.deviceName(), m_deviceHardware.deviceName());

    QCOMPARE(hardware.size(), m_deviceHardware.size());
    QCOMPARE(hardware.bytes<QByteArray>(), m_deviceHardware.bytes<QByteArray>());
    QCOMPARE(hardware.payload().size(), m_deviceHardware.payload().size());
    QCOMPARE(hardware.payload().bytes<QByteArray>(),
        m_deviceHardware.payload().bytes<QByteArray>());
    QCOMPARE(hardware.toString(), m_deviceHardware.toString());
}

void tst_QKnxNetIpDescriptionResponse::testSupportedFamiliesAccessor()
{
    QKnxNetIpDescriptionResponse descriptionResponse(m_deviceHardware, m_sf);

    auto familie = descriptionResponse.supportedFamilies();
    QCOMPARE(familie.isValid(), m_sf.isValid());
    QCOMPARE(familie.descriptionType(), m_sf.descriptionType());
    QCOMPARE(familie.size(), m_sf.size());
    QCOMPARE(familie.bytes<QByteArray>(), m_sf.bytes<QByteArray>());
    QCOMPARE(familie.payload().size(), m_sf.payload().size());
    QCOMPARE(familie.payload().bytes<QByteArray>(), m_sf.payload().bytes<QByteArray>());
    QCOMPARE(familie.toString(), m_sf.toString());
}

void tst_QKnxNetIpDescriptionResponse::testOptionalDibs()
{
    QKnxNetIpDescriptionResponse descriptionResponse(m_deviceHardware, m_sf);

    QCOMPARE(descriptionResponse.isValid(), true);

    QKnxNetIpConfigDIB configDIB(QHostAddress("192.168.2.12"),
        QHostAddress("255.255.255.0"),
        QHostAddress("192.168.2.1"),
        QKnxNetIpConfigDIB::Capability::AutoIp,
        QKnxNetIpConfigDIB::AssignmentMethod::Manual);
    descriptionResponse.addOptionalDib<QKnxNetIpConfigDIB>(configDIB);

    QKnxNetIpCurrentConfigDIB currentConfigDIB(QHostAddress("192.168.2.13"),
        QHostAddress("255.255.255.1"),
        QHostAddress("192.168.2.1"),
        QHostAddress("192.168.2.2"),
        QKnxNetIpCurrentConfigDIB::AssignmentMethod::BootP);
    descriptionResponse.addOptionalDib<QKnxNetIpCurrentConfigDIB>(currentConfigDIB);

    QVector<QKnxAddress> qknxAdresses;
    qknxAdresses.append(QKnxAddress::createIndividual(1, 1, 0));
    qknxAdresses.append(QKnxAddress::createIndividual(1, 2, 5));
    qknxAdresses.append(QKnxAddress::createIndividual(2, 3, 8));
    QKnxNetIpKnxAddressesDIB knxAddressDIB(qknxAdresses);
    descriptionResponse.addOptionalDib<QKnxNetIpKnxAddressesDIB>(knxAddressDIB);

    QByteArray byteArray(5, 8);
    QKnxNetIpManufacturerDIB manufacturerDIB(256, byteArray);
    descriptionResponse.addOptionalDib<QKnxNetIpManufacturerDIB>(manufacturerDIB);

    QKnxNetIpServiceFamiliesDIB sfDIB(
        QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::RemoteConfigAndDiagnosis, 3);
    descriptionResponse.addOptionalDib<QKnxNetIpServiceFamiliesDIB>(sfDIB);

    auto optionalDib = descriptionResponse.optionalDibs();

    auto ref = optionalDib[0]; // Checking optional DIB 1: QKnxNetIpConfigDIB
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpConfigDIB);

    auto tmpConfigDIB = ref.toType<QKnxNetIpConfigDIB>();
    QCOMPARE(tmpConfigDIB.isValid(), configDIB.isValid());
    QCOMPARE(tmpConfigDIB.size(), configDIB.size());
    QCOMPARE(tmpConfigDIB.bytes<QByteArray>(), configDIB.bytes<QByteArray>());
    QCOMPARE(tmpConfigDIB.payload().size(), configDIB.payload().size());
    QCOMPARE(tmpConfigDIB.payload().bytes<QByteArray>(), configDIB.payload().bytes<QByteArray>());
    QCOMPARE(tmpConfigDIB.toString(), configDIB.toString());

    QCOMPARE(tmpConfigDIB.descriptionType(), configDIB.descriptionType());
    QCOMPARE(tmpConfigDIB.ipAddress().toString(), configDIB.ipAddress().toString());
    QCOMPARE(tmpConfigDIB.subnetMask().toString(), configDIB.subnetMask().toString());
    QCOMPARE(tmpConfigDIB.defaultGateway().toString(), configDIB.defaultGateway().toString());
    QCOMPARE(tmpConfigDIB.capabilities(), configDIB.capabilities());
    QCOMPARE(tmpConfigDIB.assignmentMethods(), configDIB.assignmentMethods());

    ref = optionalDib[1]; // Checking optional DIB 2: QKnxNetIpCurrentConfigDIB
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpCurrentConfigDIB);

    auto tmpCurrentConfigDIB = ref.toType<QKnxNetIpCurrentConfigDIB>();
    QCOMPARE(tmpCurrentConfigDIB.isValid(), currentConfigDIB.isValid());
    QCOMPARE(tmpCurrentConfigDIB.size(), currentConfigDIB.size());
    QCOMPARE(tmpCurrentConfigDIB.bytes<QByteArray>(), currentConfigDIB.bytes<QByteArray>());
    QCOMPARE(tmpCurrentConfigDIB.payload().size(), currentConfigDIB.payload().size());
    QCOMPARE(tmpCurrentConfigDIB.payload().bytes<QByteArray>(),
        currentConfigDIB.payload().bytes<QByteArray>());
    QCOMPARE(tmpCurrentConfigDIB.toString(), currentConfigDIB.toString());

    QCOMPARE(tmpCurrentConfigDIB.descriptionType(), currentConfigDIB.descriptionType());
    QCOMPARE(tmpCurrentConfigDIB.ipAddress().toString(), currentConfigDIB.ipAddress().toString());
    QCOMPARE(tmpCurrentConfigDIB.subnetMask().toString(), currentConfigDIB.subnetMask().toString());
    QCOMPARE(tmpCurrentConfigDIB.defaultGateway().toString(),
        currentConfigDIB.defaultGateway().toString());
    QCOMPARE(tmpCurrentConfigDIB.dhcpOrBootP().toString(),
        currentConfigDIB.dhcpOrBootP().toString());
    QCOMPARE(tmpCurrentConfigDIB.assignmentMethod(), currentConfigDIB.assignmentMethod());

    ref = optionalDib[2]; // Checking optional DIB 3: QKnxNetIpKnxAddressesDIB
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpKnxAddressesDIB);

    auto tmpKnxAddressDIB = ref.toType<QKnxNetIpKnxAddressesDIB>();
    QCOMPARE(tmpKnxAddressDIB.isValid(), knxAddressDIB.isValid());
    QCOMPARE(tmpKnxAddressDIB.size(), knxAddressDIB.size());
    QCOMPARE(tmpKnxAddressDIB.bytes<QByteArray>(), knxAddressDIB.bytes<QByteArray>());
    QCOMPARE(tmpKnxAddressDIB.payload().size(), knxAddressDIB.payload().size());
    QCOMPARE(tmpKnxAddressDIB.payload().bytes<QByteArray>(),
        knxAddressDIB.payload().bytes<QByteArray>());
    QCOMPARE(tmpKnxAddressDIB.toString(), knxAddressDIB.toString());

    QCOMPARE(tmpKnxAddressDIB.descriptionType(), knxAddressDIB.descriptionType());
    QCOMPARE(tmpKnxAddressDIB.individualAddresses<QVector<QKnxAddress>>().size(),
        knxAddressDIB.individualAddresses<QVector<QKnxAddress>>().size());

    ref = optionalDib[3]; // Checking optional DIB 4: QKnxNetIpManufacturerDIB
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpManufacturerDIB);

    auto tmpManufacturerDIB = ref.toType<QKnxNetIpManufacturerDIB>();
    QCOMPARE(tmpManufacturerDIB.isValid(), manufacturerDIB.isValid());
    QCOMPARE(tmpManufacturerDIB.size(), manufacturerDIB.size());
    QCOMPARE(tmpManufacturerDIB.bytes<QByteArray>(), manufacturerDIB.bytes<QByteArray>());
    QCOMPARE(tmpManufacturerDIB.payload().size(), manufacturerDIB.payload().size());
    QCOMPARE(tmpManufacturerDIB.payload().bytes<QByteArray>(),
        manufacturerDIB.payload().bytes<QByteArray>());
    QCOMPARE(tmpManufacturerDIB.toString(), manufacturerDIB.toString());

    QCOMPARE(tmpManufacturerDIB.descriptionType(), manufacturerDIB.descriptionType());
    QCOMPARE(tmpManufacturerDIB.manufacturerId(), manufacturerDIB.manufacturerId());
    QCOMPARE(tmpManufacturerDIB.manufacturerData<QByteArray>(),
        manufacturerDIB.manufacturerData<QByteArray>());

    ref = optionalDib[4]; // Checking optional DIB 5: QKnxNetIpServiceFamiliesDIB
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpServiceFamiliesDIB);

    auto tmpSfDIB = ref.toType<QKnxNetIpServiceFamiliesDIB>();
    QCOMPARE(tmpSfDIB.isValid(), sfDIB.isValid());
    QCOMPARE(tmpSfDIB.size(), sfDIB.size());
    QCOMPARE(tmpSfDIB.bytes<QByteArray>(), sfDIB.bytes<QByteArray>());
    QCOMPARE(tmpSfDIB.payload().size(), sfDIB.payload().size());
    QCOMPARE(tmpSfDIB.payload().bytes<QByteArray>(), sfDIB.payload().bytes<QByteArray>());
    QCOMPARE(tmpSfDIB.toString(), sfDIB.toString());

    QCOMPARE(tmpSfDIB.descriptionType(), sfDIB.descriptionType());
}

void tst_QKnxNetIpDescriptionResponse::testDebugStream()
{
    struct DebugHandler
    {
        explicit DebugHandler(QtMessageHandler newMessageHandler)
            : oldMessageHandler(qInstallMessageHandler(newMessageHandler)) {}
        ~DebugHandler() {
            qInstallMessageHandler(oldMessageHandler);
        }
        QtMessageHandler oldMessageHandler;
    } _(myMessageHandler);

    qDebug() << QKnxNetIpDescriptionResponse();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    QKnxNetIpDeviceDIB hardware(QKnxNetIpDeviceDIB::Medium::Ip,
        QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
        QKnxAddress::Individual::Unregistered,
        0x1111,
        QByteArray::fromHex("123456123456"),
        QHostAddress::AnyIPv4,
        QByteArray::fromHex("bcaec56690f9"),
        QByteArray("qt.io KNX device"));
    const QKnxNetIpServiceFamiliesDIB supportedFamilies(
        QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 0x04);
    qDebug() << QKnxNetIpDescriptionResponse(hardware, supportedFamilies);
    QCOMPARE(s_msg, QString::fromLatin1("0x06100204004036012001ffff111112345612345600000000bc"
            "aec56690f971742e696f204b4e5820646576696365000000000000000000000000000004020404"));
}

void tst_QKnxNetIpDescriptionResponse::testDataStream()
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    QKnxNetIpDeviceDIB hardware(QKnxNetIpDeviceDIB::Medium::Ip,
        QKnxNetIpDeviceDIB::DeviceStatus::ActiveProgrammingMode,
        QKnxAddress::Individual::Unregistered,
        0x1111,
        QByteArray::fromHex("123456123456"),
        QHostAddress::AnyIPv4,
        QByteArray::fromHex("bcaec56690f9"),
        QByteArray("qt.io KNX device"));
    const QKnxNetIpServiceFamiliesDIB supportedFamilies(
        QKnxNetIpServiceFamiliesDIB::ServiceFamilieId::IpTunneling, 0x04);
    out << QKnxNetIpDescriptionResponse(hardware, supportedFamilies);
    QCOMPARE(byteArray, QByteArray::fromHex("06100204004036012001ffff111112345612345600000000bc"
            "aec56690f971742e696f204b4e5820646576696365000000000000000000000000000004020404"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpDescriptionResponse)

#include "tst_qknxnetipdescriptionresponse.moc"
