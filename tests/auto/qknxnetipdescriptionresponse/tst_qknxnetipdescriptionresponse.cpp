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
#include <QtKnx/qknxnetipconfigdib.h>
#include <QtKnx/qknxnetipcurrentconfigdib.h>
#include <QtKnx/qknxnetipdescriptionresponse.h>
#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetipknxaddressesdib.h>
#include <QtKnx/qknxnetipmanufacturerdib.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtKnx/qknxnetiptunnelinginfodib.h>
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
    void testSupportedFamiliesVersions();
    void testOptionalDibs();
    void testValidFromByteArray();
    void testDebugStream();

private:
    QKnxNetIpDib m_sf;
    QKnxNetIpDib m_deviceHardware;
};

void tst_QKnxNetIpDescriptionResponse::initTestCase()
{
    m_sf = QKnxNetIpServiceFamiliesDibProxy::builder()
        .setServiceInfos({ { QKnxNetIp::ServiceFamily::IpTunneling, 0x04 } })
        .create();

    m_deviceHardware = QKnxNetIpDeviceDibProxy::builder()
        .setMediumType(QKnx::MediumType::NetIP)
        .setDeviceStatus(QKnxNetIp::ProgrammingMode::Active)
        .setIndividualAddress(QKnxAddress::Individual::Unregistered)
        .setProjectInstallationId(0x1111)
        .setSerialNumber(QKnxByteArray::fromHex("123456123456"))
        .setMulticastAddress(QHostAddress::AnyIPv4)
        .setMacAddress(QKnxByteArray::fromHex("bcaec56690f9"))
        .setDeviceName(QByteArray("qt.io KNX device"))
        .create();
}

void tst_QKnxNetIpDescriptionResponse::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpDescriptionResponseProxy descriptionResponseDefault(frame);

    QCOMPARE(descriptionResponseDefault.isValid(), false);
    QCOMPARE(frame.size(), quint16(0));
    QCOMPARE(frame.bytes(),QKnxByteArray {});
    QCOMPARE(frame.data().size(), quint16(0));
    QCOMPARE(frame.data(),QKnxByteArray {});
}

void tst_QKnxNetIpDescriptionResponse::testConstructorWithArguments()
{
    auto frame = QKnxNetIpDescriptionResponseProxy::builder()
        .setDeviceHardware(m_deviceHardware)
        .setSupportedFamilies(m_sf)
        .create();
    QKnxNetIpDescriptionResponseProxy descriptionResponse(frame);

    QCOMPARE(descriptionResponse.isValid(), true);
    QCOMPARE(frame.size(), quint16(m_deviceHardware.size() + m_sf.size() + 6));
    QCOMPARE(frame.data().size(), quint16(m_deviceHardware.size() + m_sf.size()));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("061002040040")
        + QKnxByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
        + QKnxByteArray("qt.io KNX device", 16)
        + QKnxByteArray::fromHex("0000000000000000000000000000")
        + QKnxByteArray::fromHex("04020404"));

    QCOMPARE(frame.data(),
        QKnxByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
        + QKnxByteArray("qt.io KNX device", 16)
        + QKnxByteArray::fromHex("0000000000000000000000000000")
        + QKnxByteArray::fromHex("04020404"));
}

void tst_QKnxNetIpDescriptionResponse::testDeviceHardwareAccessor()
{
    auto frame = QKnxNetIpDescriptionResponseProxy::builder()
        .setDeviceHardware(m_deviceHardware)
        .setSupportedFamilies(m_sf)
        .create();
    QKnxNetIpDescriptionResponseProxy descriptionResponse(frame);

    auto dib = descriptionResponse.deviceHardware();

    const QKnxNetIpDeviceDibProxy hardware(dib);
    const QKnxNetIpDeviceDibProxy deviceHardware(m_deviceHardware);

    QCOMPARE(hardware.isValid(), deviceHardware.isValid());
    QCOMPARE(hardware.mediumType(), deviceHardware.mediumType());
    QCOMPARE(hardware.descriptionType(), deviceHardware.descriptionType());
    QCOMPARE(hardware.deviceStatus(), deviceHardware.deviceStatus());
    QCOMPARE(hardware.individualAddress(), deviceHardware.individualAddress());
    QCOMPARE(hardware.projectInstallationId(), deviceHardware.projectInstallationId());
    QCOMPARE(hardware.serialNumber(), deviceHardware.serialNumber());
    QCOMPARE(hardware.multicastAddress(), deviceHardware.multicastAddress());
    QCOMPARE(hardware.macAddress(), deviceHardware.macAddress());
    QCOMPARE(hardware.deviceName(), deviceHardware.deviceName());

    QCOMPARE(dib.size(), m_deviceHardware.size());
    QCOMPARE(dib.bytes(), m_deviceHardware.bytes());
    QCOMPARE(dib.data().size(), m_deviceHardware.data().size());
    QCOMPARE(dib.data(), m_deviceHardware.data());
}

void tst_QKnxNetIpDescriptionResponse::testSupportedFamiliesAccessor()
{
    auto frame = QKnxNetIpDescriptionResponseProxy::builder()
        .setDeviceHardware(m_deviceHardware)
        .setSupportedFamilies(m_sf)
        .create();
    QKnxNetIpDescriptionResponseProxy descriptionResponse(frame);

    auto family = descriptionResponse.supportedFamilies();
    const QKnxNetIpServiceFamiliesDibProxy view(family);

    QCOMPARE(view.isValid(), m_sf.isValid());
    QCOMPARE(view.descriptionType(), QKnxNetIpServiceFamiliesDibProxy(m_sf).descriptionType());
    QCOMPARE(family.size(), m_sf.size());
    QCOMPARE(family.bytes(), m_sf.bytes());
    QCOMPARE(family.data().size(), m_sf.data().size());
    QCOMPARE(family.data(), m_sf.data());
}

void tst_QKnxNetIpDescriptionResponse::testSupportedFamiliesVersions()
{
    QList<QKnxServiceInfo> fam = {
        { QKnxNetIp::ServiceFamily::IpTunneling, 0x04 },
        { QKnxNetIp::ServiceFamily::Core, 9 },
        { QKnxNetIp::ServiceFamily::DeviceManagement, 2 },
        { QKnxNetIp::ServiceFamily::DeviceManagement, 1 },
        { QKnxNetIp::ServiceFamily::IpTunneling, 11 },
        { QKnxNetIp::ServiceFamily::IpRouting, 12 },
        { QKnxNetIp::ServiceFamily::RemoteLogging, 13 },
        { QKnxNetIp::ServiceFamily::Security, 1 }
    };
    {   // test that a supported service families dib with security service
        // family is not valid in a description response
        auto securtyFamDib = QKnxNetIpServiceFamiliesDibProxy::builder()
                             .setServiceInfos(fam)
                             .create();
        auto frame = QKnxNetIpDescriptionResponseProxy::builder()
                                          .setDeviceHardware(m_deviceHardware)
                                          .setSupportedFamilies(securtyFamDib)
                                          .create();
        QKnxNetIpDescriptionResponseProxy descriptionResponse(frame);

        auto extractedFamilies = descriptionResponse.supportedFamilies();
        QVERIFY(!extractedFamilies.isValid());
        QCOMPARE(extractedFamilies.size(), 0);
    }
    {   // test that a supported service families dib without service family is
        // valid in a description response
        QCOMPARE(8, fam.size());
        fam.erase(std::remove_if(fam.begin(), fam.end(),
            [](const QKnxServiceInfo &info) {
                return info.ServiceFamily == QKnxNetIp::ServiceFamily::Security;
        }), fam.end());
        auto noSecurityFamDib = QKnxNetIpServiceFamiliesDibProxy::builder()
                                .setServiceInfos(fam)
                                .create();
        auto frame = QKnxNetIpDescriptionResponseProxy::builder()
                                        .setDeviceHardware(m_deviceHardware)
                                        .setSupportedFamilies(noSecurityFamDib)
                                        .create();

        QKnxNetIpDescriptionResponseProxy descriptionResponse(frame);
        auto dibFamilies = descriptionResponse.supportedFamilies();
        QVERIFY(dibFamilies.isValid());

        const QKnxNetIpServiceFamiliesDibProxy view(dibFamilies);
        QVERIFY(view.isValid());

        auto extractedFamilies = view.serviceInfos();
        QCOMPARE(extractedFamilies.size(), fam.size());
    }
}

void tst_QKnxNetIpDescriptionResponse::testOptionalDibs()
{
    auto builder = QKnxNetIpDescriptionResponseProxy::builder()
        .setDeviceHardware(m_deviceHardware)
        .setSupportedFamilies(m_sf);

    auto configDib = QKnxNetIpConfigDibProxy::builder()
        .setIpAddress(QHostAddress("192.168.2.12"))
        .setSubnetMask(QHostAddress("255.255.255.0"))
        .setDefaultGateway(QHostAddress("192.168.2.1"))
        .setCapabilities(QKnxNetIp::Capability::AutoIp)
        .setAssignmentMethods(QKnxNetIp::AssignmentMethod::Manual)
        .create();

    auto currentConfigDib = QKnxNetIpCurrentConfigDibProxy::builder()
        .setIpAddress(QHostAddress("192.168.2.13"))
        .setSubnetMask(QHostAddress("255.255.255.1"))
        .setDefaultGateway(QHostAddress("192.168.2.1"))
        .setDhcpOrBootP(QHostAddress("192.168.2.2"))
        .setAssignmentMethod(QKnxNetIp::AssignmentMethod::BootP)
        .create();

    auto knxAddressDib = QKnxNetIpKnxAddressesDibProxy::builder()
        .setIndividualAddresses({
            QKnxAddress::createIndividual(1, 1, 0),
            QKnxAddress::createIndividual(1, 2, 5),
            QKnxAddress::createIndividual(2, 3, 8)
        }).create();

    auto manufacturerDib = QKnxNetIpManufacturerDibProxy::builder()
        .setManufacturerId(256)
        .setManufacturerData(QKnxByteArray(5, 8))
        .create();

    auto sfDib =  QKnxNetIpServiceFamiliesDibProxy::builder()
        .setServiceInfos({ { QKnxNetIp::ServiceFamily::Core, 3 } })
        .create();

    builder.setOptionalDibs({ configDib, currentConfigDib, knxAddressDib, manufacturerDib, sfDib });

    auto frame = builder.create();
    QKnxNetIpDescriptionResponseProxy descriptionResponse(frame);

    QCOMPARE(descriptionResponse.isValid(), true);
    auto optionalDibs = descriptionResponse.optionalDibs();

    auto dib = optionalDibs[0]; // Checking optional Dib 1: QKnxNetIpConfigDib
    QCOMPARE(dib.code(), QKnxNetIp::DescriptionType::IpConfiguration);

    const QKnxNetIpConfigDibProxy tmpConfigDib(dib);
    QCOMPARE(tmpConfigDib.isValid(), configDib.isValid());
    QCOMPARE(dib.size(), configDib.size());
    QCOMPARE(dib.bytes(), configDib.bytes());
    QCOMPARE(dib.data().size(), configDib.data().size());
    QCOMPARE(dib.data(), configDib.data());

    QCOMPARE(tmpConfigDib.descriptionType(), QKnxNetIp::DescriptionType::IpConfiguration);
    QCOMPARE(tmpConfigDib.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(tmpConfigDib.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(tmpConfigDib.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(tmpConfigDib.capabilities(), QKnxNetIp::Capability::AutoIp);
    QCOMPARE(tmpConfigDib.assignmentMethods(), QKnxNetIp::AssignmentMethod::Manual);

    dib = optionalDibs[1]; // Checking optional Dib 2: QKnxNetIpCurrentConfigDib
    QCOMPARE(dib.code(), QKnxNetIp::DescriptionType::CurrentIpConfiguration);

    const QKnxNetIpCurrentConfigDibProxy tmpCurrentConfigDib(dib);
    QCOMPARE(tmpCurrentConfigDib.isValid(), currentConfigDib.isValid());
    QCOMPARE(dib.size(), currentConfigDib.size());
    QCOMPARE(dib.bytes(), currentConfigDib.bytes());
    QCOMPARE(dib.data().size(), currentConfigDib.data().size());
    QCOMPARE(dib.data(), currentConfigDib.data());

    QCOMPARE(tmpCurrentConfigDib.descriptionType(), QKnxNetIp::DescriptionType::CurrentIpConfiguration);
    QCOMPARE(tmpCurrentConfigDib.ipAddress(), QHostAddress("192.168.2.13"));
    QCOMPARE(tmpCurrentConfigDib.subnetMask(), QHostAddress("255.255.255.1"));
    QCOMPARE(tmpCurrentConfigDib.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(tmpCurrentConfigDib.dhcpOrBootP(), QHostAddress("192.168.2.2"));
    QCOMPARE(tmpCurrentConfigDib.assignmentMethod(), QKnxNetIp::AssignmentMethod::BootP);

    dib = optionalDibs[2]; // Checking optional Dib 3: QKnxNetIpKnxAddressesDib
    QCOMPARE(dib.code(), QKnxNetIp::DescriptionType::KnxAddresses);

    const QKnxNetIpKnxAddressesDibProxy tmpKnxAddressDib(dib);
    QCOMPARE(tmpKnxAddressDib.isValid(), knxAddressDib.isValid());
    QCOMPARE(dib.size(), knxAddressDib.size());
    QCOMPARE(dib.bytes(), knxAddressDib.bytes());
    QCOMPARE(dib.data().size(), knxAddressDib.data().size());
    QCOMPARE(dib.data(), knxAddressDib.data());

    QCOMPARE(tmpKnxAddressDib.descriptionType(), QKnxNetIp::DescriptionType::KnxAddresses);
    auto addresses = tmpKnxAddressDib.individualAddresses();
    QCOMPARE(addresses.size(), 3);
    QCOMPARE(addresses[0], QKnxAddress::createIndividual(1, 1, 0));
    QCOMPARE(addresses[1], QKnxAddress::createIndividual(1, 2, 5));
    QCOMPARE(addresses[2], QKnxAddress::createIndividual(2, 3, 8));

    dib = optionalDibs[3]; // Checking optional Dib 4: QKnxNetIpManufacturerDib
    QCOMPARE(dib.code(), QKnxNetIp::DescriptionType::ManufacturerData);

    const QKnxNetIpManufacturerDibProxy tmpManufacturerDib(dib);
    QCOMPARE(tmpManufacturerDib.isValid(), manufacturerDib.isValid());
    QCOMPARE(dib.size(), manufacturerDib.size());
    QCOMPARE(dib.bytes(), manufacturerDib.bytes());
    QCOMPARE(dib.data().size(), manufacturerDib.data().size());
    QCOMPARE(dib.data(), manufacturerDib.data());

    QCOMPARE(tmpManufacturerDib.descriptionType(), QKnxNetIp::DescriptionType::ManufacturerData);
    QCOMPARE(tmpManufacturerDib.manufacturerId(), 256);
    // add 0x00 since the specification requires even sized structures
    QCOMPARE(tmpManufacturerDib.manufacturerData(), QKnxByteArray(5, 8).append(0x00));

    dib = optionalDibs[4]; // Checking optional Dib 5: QKnxNetIpServiceFamiliesDib
    QCOMPARE(dib.code(), QKnxNetIp::DescriptionType::SupportedServiceFamilies);

    const QKnxNetIpServiceFamiliesDibProxy tmpSfDib(dib);
    QCOMPARE(tmpSfDib.isValid(), sfDib.isValid());
    QCOMPARE(dib.size(), sfDib.size());
    QCOMPARE(dib.bytes(), sfDib.bytes());
    QCOMPARE(dib.data().size(), sfDib.data().size());
    QCOMPARE(dib.data(), sfDib.data());

    QCOMPARE(tmpSfDib.descriptionType(), QKnxNetIp::DescriptionType::SupportedServiceFamilies);
    QCOMPARE(tmpSfDib.serviceInfos().size(), 1);

    auto tunnelInfoDib = QKnxNetIpTunnelingInfoDibProxy::builder()
        .setMaximumInterfaceApduLength(0x1000)
        .setTunnelingSlotInfo({ { QKnxAddress::Type::Individual, 1976 },
            QKnxNetIpTunnelingSlotInfo::Available })
        .create();

    builder.setOptionalDibs({ configDib, currentConfigDib, knxAddressDib, tunnelInfoDib,
        manufacturerDib, sfDib });
    frame = builder.create();

    optionalDibs = descriptionResponse.optionalDibs();
    QCOMPARE(descriptionResponse.optionalDibs().count(), 5);

    for (const auto &dib : qAsConst(optionalDibs))
        QVERIFY2(dib.code() != QKnxNetIp::DescriptionType::TunnelingInfo, "tunneling info illegal");
}

void tst_QKnxNetIpDescriptionResponse::testValidFromByteArray()
{
    auto frame = QKnxNetIpFrame::fromBytes(QKnxByteArray::fromHex("06100204004036012001ffff111112"
        "345612345600000000bcaec56690f971742e696f204b4e582064657669636500000000000000000000000000"
        "0004020404"));

    QKnxNetIpDescriptionResponseProxy descriptionResponse(frame);
    QCOMPARE(descriptionResponse.isValid(), true);
    QCOMPARE(descriptionResponse.supportedFamilies(), m_sf);
    QCOMPARE(descriptionResponse.deviceHardware(), m_deviceHardware);
    QCOMPARE(descriptionResponse.optionalDibs().count(), 0);

    auto tunnelInfoDib = QKnxNetIpTunnelingInfoDibProxy::builder()
        .setMaximumInterfaceApduLength(0x1000)
        .setTunnelingSlotInfo({ { QKnxAddress::Type::Individual, 1976 },
            QKnxNetIpTunnelingSlotInfo::Available })
        .create();

    // illegally contains a tunneling information DIB
    frame = QKnxNetIpFrame::fromBytes(QKnxByteArray::fromHex("06100204004836012001ffff11111234561"
        "2345600000000bcaec56690f971742e696f204b4e58206465766963650000000000000000000000000000040"
        "20404") + tunnelInfoDib.bytes());
    QCOMPARE(descriptionResponse.isValid(), false);
    QCOMPARE(descriptionResponse.supportedFamilies(), m_sf);
    QCOMPARE(descriptionResponse.deviceHardware(), m_deviceHardware);
    QCOMPARE(descriptionResponse.optionalDibs().count(), 1);
    QCOMPARE(descriptionResponse.optionalDibs().value(0), tunnelInfoDib);
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

    qDebug() << QKnxNetIpDescriptionResponseProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061002040006"));

    qDebug() << QKnxNetIpDescriptionResponseProxy::builder()
        .setSupportedFamilies(m_sf)
        .setDeviceHardware(m_deviceHardware)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100204004036012001ffff111112345612345600000000bc"
            "aec56690f971742e696f204b4e5820646576696365000000000000000000000000000004020404"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpDescriptionResponse)

#include "tst_qknxnetipdescriptionresponse.moc"
