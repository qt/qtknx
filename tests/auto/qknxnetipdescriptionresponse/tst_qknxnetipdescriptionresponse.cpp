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

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnamespace.h>
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
    QKnxNetIpServiceFamiliesDib m_sf;
    QKnxNetIpDeviceDib m_deviceHardware;
};

void tst_QKnxNetIpDescriptionResponse::initTestCase()
{
    m_sf = { QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling, 0x04 };
    m_deviceHardware = { QKnx::MediumType::NetIP,
                         QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
                         QKnxAddress::Individual::Unregistered,
                         0x1111,
                         QKnxByteArray::fromHex("123456123456"),
                         QHostAddress::AnyIPv4,
                         QKnxByteArray::fromHex("bcaec56690f9"),
                         QKnxByteArray("qt.io KNX device", 16)
    };
}

void tst_QKnxNetIpDescriptionResponse::testDefaultConstructor()
{
    QKnxNetIpDescriptionResponse descriptionResponseDefault;
    QCOMPARE(descriptionResponseDefault.isValid(), false);
    QCOMPARE(descriptionResponseDefault.size(), quint16(0));
    QCOMPARE(descriptionResponseDefault.bytes(),QKnxByteArray {});
    QCOMPARE(descriptionResponseDefault.payload().size(), quint16(0));
    QCOMPARE(descriptionResponseDefault.payload().bytes(),QKnxByteArray {});
    QCOMPARE(descriptionResponseDefault.toString(), QString::fromLatin1("Header size { 0x00 }, "
            "Version { 0x00 }, Service type { 0x00 }, Total size { 0x00 }, Bytes {  }"));
}

void tst_QKnxNetIpDescriptionResponse::testConstructorWithArguments()
{
    QKnxNetIpDescriptionResponse descriptionResponse(m_deviceHardware, m_sf);

    QCOMPARE(descriptionResponse.isValid(), true);
    QCOMPARE(descriptionResponse.size(), quint16(m_deviceHardware.size() + m_sf.size() + 6));
    QCOMPARE(descriptionResponse.payload().size(), quint16(m_deviceHardware.size() + m_sf.size()));
    QCOMPARE(descriptionResponse.bytes(), QKnxByteArray::fromHex("061002040040")
        + QKnxByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
        + QKnxByteArray("qt.io KNX device", 16)
        + QKnxByteArray::fromHex("0000000000000000000000000000")
        + QKnxByteArray::fromHex("04020404"));

    QCOMPARE(descriptionResponse.payload().bytes(),
        QKnxByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
        + QKnxByteArray("qt.io KNX device", 16)
        + QKnxByteArray::fromHex("0000000000000000000000000000")
        + QKnxByteArray::fromHex("04020404"));

    QCOMPARE(descriptionResponse.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Service type { 0x204 }, Total size { 0x40 }, Bytes { 0x36, 0x01, 0x20, "
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
    QCOMPARE(hardware.mediumType(), m_deviceHardware.mediumType());
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
    QCOMPARE(hardware.bytes(), m_deviceHardware.bytes());
    QCOMPARE(hardware.payload().size(), m_deviceHardware.payload().size());
    QCOMPARE(hardware.payload().bytes(),
        m_deviceHardware.payload().bytes());
    QCOMPARE(hardware.toString(), m_deviceHardware.toString());
}

void tst_QKnxNetIpDescriptionResponse::testSupportedFamiliesAccessor()
{
    QKnxNetIpDescriptionResponse descriptionResponse(m_deviceHardware, m_sf);

    auto familie = descriptionResponse.supportedFamilies();
    QCOMPARE(familie.isValid(), m_sf.isValid());
    QCOMPARE(familie.descriptionType(), m_sf.descriptionType());
    QCOMPARE(familie.size(), m_sf.size());
    QCOMPARE(familie.bytes(), m_sf.bytes());
    QCOMPARE(familie.payload().size(), m_sf.payload().size());
    QCOMPARE(familie.payload().bytes(), m_sf.payload().bytes());
    QCOMPARE(familie.toString(), m_sf.toString());
}

void tst_QKnxNetIpDescriptionResponse::testOptionalDibs()
{
    QKnxNetIpDescriptionResponse descriptionResponse(m_deviceHardware, m_sf);

    QCOMPARE(descriptionResponse.isValid(), true);

    QKnxNetIpConfigDib configDib(QHostAddress("192.168.2.12"),
        QHostAddress("255.255.255.0"),
        QHostAddress("192.168.2.1"),
        QKnxNetIpConfigDib::Capability::AutoIp,
        QKnxNetIpConfigDib::AssignmentMethod::Manual);
    descriptionResponse.addOptionalDib<QKnxNetIpConfigDib>(configDib);

    QKnxNetIpCurrentConfigDib currentConfigDib(QHostAddress("192.168.2.13"),
        QHostAddress("255.255.255.1"),
        QHostAddress("192.168.2.1"),
        QHostAddress("192.168.2.2"),
        QKnxNetIpCurrentConfigDib::AssignmentMethod::BootP);
    descriptionResponse.addOptionalDib<QKnxNetIpCurrentConfigDib>(currentConfigDib);

    QVector<QKnxAddress> qknxAdresses;
    qknxAdresses.append(QKnxAddress::createIndividual(1, 1, 0));
    qknxAdresses.append(QKnxAddress::createIndividual(1, 2, 5));
    qknxAdresses.append(QKnxAddress::createIndividual(2, 3, 8));
    QKnxNetIpKnxAddressesDib knxAddressDib(qknxAdresses);
    descriptionResponse.addOptionalDib<QKnxNetIpKnxAddressesDib>(knxAddressDib);

    QKnxByteArray byteArray(5, 8);
    QKnxNetIpManufacturerDib manufacturerDib(256, byteArray);
    descriptionResponse.addOptionalDib<QKnxNetIpManufacturerDib>(manufacturerDib);

    QKnxNetIpServiceFamiliesDib sfDib(
        QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteConfigAndDiagnosis, 3);
    descriptionResponse.addOptionalDib<QKnxNetIpServiceFamiliesDib>(sfDib);

    auto optionalDib = descriptionResponse.optionalDibs();

    auto ref = optionalDib[0]; // Checking optional Dib 1: QKnxNetIpConfigDib
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpConfigDib);

    auto tmpConfigDib = ref.toType<QKnxNetIpConfigDib>();
    QCOMPARE(tmpConfigDib.isValid(), configDib.isValid());
    QCOMPARE(tmpConfigDib.size(), configDib.size());
    QCOMPARE(tmpConfigDib.bytes(), configDib.bytes());
    QCOMPARE(tmpConfigDib.payload().size(), configDib.payload().size());
    QCOMPARE(tmpConfigDib.payload().bytes(), configDib.payload().bytes());
    QCOMPARE(tmpConfigDib.toString(), configDib.toString());

    QCOMPARE(tmpConfigDib.descriptionType(), configDib.descriptionType());
    QCOMPARE(tmpConfigDib.ipAddress().toString(), configDib.ipAddress().toString());
    QCOMPARE(tmpConfigDib.subnetMask().toString(), configDib.subnetMask().toString());
    QCOMPARE(tmpConfigDib.defaultGateway().toString(), configDib.defaultGateway().toString());
    QCOMPARE(tmpConfigDib.capabilities(), configDib.capabilities());
    QCOMPARE(tmpConfigDib.assignmentMethods(), configDib.assignmentMethods());

    ref = optionalDib[1]; // Checking optional Dib 2: QKnxNetIpCurrentConfigDib
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpCurrentConfigDib);

    auto tmpCurrentConfigDib = ref.toType<QKnxNetIpCurrentConfigDib>();
    QCOMPARE(tmpCurrentConfigDib.isValid(), currentConfigDib.isValid());
    QCOMPARE(tmpCurrentConfigDib.size(), currentConfigDib.size());
    QCOMPARE(tmpCurrentConfigDib.bytes(), currentConfigDib.bytes());
    QCOMPARE(tmpCurrentConfigDib.payload().size(), currentConfigDib.payload().size());
    QCOMPARE(tmpCurrentConfigDib.payload().bytes(),
        currentConfigDib.payload().bytes());
    QCOMPARE(tmpCurrentConfigDib.toString(), currentConfigDib.toString());

    QCOMPARE(tmpCurrentConfigDib.descriptionType(), currentConfigDib.descriptionType());
    QCOMPARE(tmpCurrentConfigDib.ipAddress().toString(), currentConfigDib.ipAddress().toString());
    QCOMPARE(tmpCurrentConfigDib.subnetMask().toString(), currentConfigDib.subnetMask().toString());
    QCOMPARE(tmpCurrentConfigDib.defaultGateway().toString(),
        currentConfigDib.defaultGateway().toString());
    QCOMPARE(tmpCurrentConfigDib.dhcpOrBootP().toString(),
        currentConfigDib.dhcpOrBootP().toString());
    QCOMPARE(tmpCurrentConfigDib.assignmentMethod(), currentConfigDib.assignmentMethod());

    ref = optionalDib[2]; // Checking optional Dib 3: QKnxNetIpKnxAddressesDib
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpKnxAddressesDib);

    auto tmpKnxAddressDib = ref.toType<QKnxNetIpKnxAddressesDib>();
    QCOMPARE(tmpKnxAddressDib.isValid(), knxAddressDib.isValid());
    QCOMPARE(tmpKnxAddressDib.size(), knxAddressDib.size());
    QCOMPARE(tmpKnxAddressDib.bytes(), knxAddressDib.bytes());
    QCOMPARE(tmpKnxAddressDib.payload().size(), knxAddressDib.payload().size());
    QCOMPARE(tmpKnxAddressDib.payload().bytes(),
        knxAddressDib.payload().bytes());
    QCOMPARE(tmpKnxAddressDib.toString(), knxAddressDib.toString());

    QCOMPARE(tmpKnxAddressDib.descriptionType(), knxAddressDib.descriptionType());
    QCOMPARE(tmpKnxAddressDib.individualAddresses().size(),
        knxAddressDib.individualAddresses().size());

    ref = optionalDib[3]; // Checking optional Dib 4: QKnxNetIpManufacturerDib
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpManufacturerDib);

    auto tmpManufacturerDib = ref.toType<QKnxNetIpManufacturerDib>();
    QCOMPARE(tmpManufacturerDib.isValid(), manufacturerDib.isValid());
    QCOMPARE(tmpManufacturerDib.size(), manufacturerDib.size());
    QCOMPARE(tmpManufacturerDib.bytes(), manufacturerDib.bytes());
    QCOMPARE(tmpManufacturerDib.payload().size(), manufacturerDib.payload().size());
    QCOMPARE(tmpManufacturerDib.payload().bytes(),
        manufacturerDib.payload().bytes());
    QCOMPARE(tmpManufacturerDib.toString(), manufacturerDib.toString());

    QCOMPARE(tmpManufacturerDib.descriptionType(), manufacturerDib.descriptionType());
    QCOMPARE(tmpManufacturerDib.manufacturerId(), manufacturerDib.manufacturerId());
    QCOMPARE(tmpManufacturerDib.manufacturerData(),
        manufacturerDib.manufacturerData());

    ref = optionalDib[4]; // Checking optional Dib 5: QKnxNetIpServiceFamiliesDib
    QCOMPARE(ref.type(), QKnxNetIpStructRef::Type::QKnxNetIpServiceFamiliesDib);

    auto tmpSfDib = ref.toType<QKnxNetIpServiceFamiliesDib>();
    QCOMPARE(tmpSfDib.isValid(), sfDib.isValid());
    QCOMPARE(tmpSfDib.size(), sfDib.size());
    QCOMPARE(tmpSfDib.bytes(), sfDib.bytes());
    QCOMPARE(tmpSfDib.payload().size(), sfDib.payload().size());
    QCOMPARE(tmpSfDib.payload().bytes(), sfDib.payload().bytes());
    QCOMPARE(tmpSfDib.toString(), sfDib.toString());

    QCOMPARE(tmpSfDib.descriptionType(), sfDib.descriptionType());
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

    QKnxNetIpDeviceDib hardware(QKnx::MediumType::NetIP,
        QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
        QKnxAddress::Individual::Unregistered,
        0x1111,
        QKnxByteArray::fromHex("123456123456"),
        QHostAddress::AnyIPv4,
        QKnxByteArray::fromHex("bcaec56690f9"),
        QKnxByteArray("qt.io KNX device", 16));
    const QKnxNetIpServiceFamiliesDib supportedFamilies(
        QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling, 0x04);
    qDebug() << QKnxNetIpDescriptionResponse(hardware, supportedFamilies);
    QCOMPARE(s_msg, QString::fromLatin1("0x06100204004036012001ffff111112345612345600000000bc"
            "aec56690f971742e696f204b4e5820646576696365000000000000000000000000000004020404"));
}

void tst_QKnxNetIpDescriptionResponse::testDataStream()
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    QKnxNetIpDeviceDib hardware(QKnx::MediumType::NetIP,
        QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
        QKnxAddress::Individual::Unregistered,
        0x1111,
        QKnxByteArray::fromHex("123456123456"),
        QHostAddress::AnyIPv4,
        QKnxByteArray::fromHex("bcaec56690f9"),
        QKnxByteArray("qt.io KNX device", 16));
    const QKnxNetIpServiceFamiliesDib supportedFamilies(
        QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling, 0x04);
    out << QKnxNetIpDescriptionResponse(hardware, supportedFamilies);
    QCOMPARE(byteArray, QByteArray::fromHex("06100204004036012001ffff111112345612345600000000bc"
            "aec56690f971742e696f204b4e5820646576696365000000000000000000000000000004020404"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpDescriptionResponse)

#include "tst_qknxnetipdescriptionresponse.moc"
