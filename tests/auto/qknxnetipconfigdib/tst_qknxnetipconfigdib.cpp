/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipconfigdib.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpConfigDIB : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithFourArguments();
    void testConstructorWithFiveArguments();
    void testDebugStream()
    {
        // TODO: Implement.
    }
    void testDataStream()
    {
        // TODO: Implement.
    }
};

void tst_QKnxNetIpConfigDIB::testDefaultConstructor()
{
    QKnxNetIpConfigDIB configDIB;
    QCOMPARE(configDIB.isValid(), false);
    QCOMPARE(configDIB.size(), quint16(0));
    QCOMPARE(configDIB.bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(configDIB.payload().size(), quint16(0));
    QCOMPARE(configDIB.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(configDIB.toString(), QString::fromLatin1("Total size { 0x00 }, Code { 0x00 }, "
        "Bytes {  }"));
    QCOMPARE(quint8(configDIB.descriptionTypeCode()), quint8(0));
    QCOMPARE(configDIB.ipAddress(), QHostAddress());
    QCOMPARE(configDIB.subnetMask(), QHostAddress());
    QCOMPARE(configDIB.defaultGateway(), QHostAddress());
    QCOMPARE(configDIB.capabilities(), 0);
    QCOMPARE(configDIB.assignmentMethods(), 0);
}

void tst_QKnxNetIpConfigDIB::testConstructorWithFourArguments()
{
    QNetworkAddressEntry addressEntry;
    addressEntry.setIp(QHostAddress("192.168.2.12"));
    addressEntry.setNetmask(QHostAddress("255.255.255.0"));
    QKnxNetIpConfigDIB configDIB(addressEntry,
                                 QHostAddress("192.168.2.1"),
                                 QKnxNetIpConfigDIB::Capability::AutoIp,
                                 QKnxNetIpConfigDIB::AssignmentMethod::Manual);

    QCOMPARE(configDIB.isValid(), true);
    QCOMPARE(configDIB.size(), quint16(16));
    QCOMPARE(configDIB.bytes<QByteArray>(),
        QByteArray::fromHex("1003C0A8020CFFFFFF00C0A802010201"));
    QCOMPARE(configDIB.payload().size(), quint16(14));
    QCOMPARE(configDIB.payload().bytes<QByteArray>(),
        QByteArray::fromHex("C0A8020CFFFFFF00C0A802010201"));
    QCOMPARE(configDIB.toString(), QString::fromLatin1("Total size { 0x10 }, Code { 0x03 }, "
        "Bytes { 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, 0xc0, 0xa8, 0x02, "
        "0x01, 0x02, 0x01 }"));
    QCOMPARE(configDIB.descriptionTypeCode(), QKnxNetIp::DescriptionTypeCode::IpConfiguration);
    QCOMPARE(configDIB.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(configDIB.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(configDIB.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(configDIB.capabilities(), QKnxNetIpConfigDIB::Capability::AutoIp);
    QCOMPARE(configDIB.assignmentMethods(), QKnxNetIpConfigDIB::AssignmentMethod::Manual);
}

void tst_QKnxNetIpConfigDIB::testConstructorWithFiveArguments()
{
    QKnxNetIpConfigDIB configDIB(QHostAddress("192.168.2.12"),
                                 QHostAddress("255.255.255.0"),
                                 QHostAddress("192.168.2.1"),
                                 QKnxNetIpConfigDIB::Capability::AutoIp,
                                 QKnxNetIpConfigDIB::AssignmentMethod::Manual);

    QCOMPARE(configDIB.isValid(), true);
    QCOMPARE(configDIB.size(), quint16(16));
    QCOMPARE(configDIB.bytes<QByteArray>(),
        QByteArray::fromHex("1003C0A8020CFFFFFF00C0A802010201"));
    QCOMPARE(configDIB.payload().size(), quint16(14));
    QCOMPARE(configDIB.payload().bytes<QByteArray>(),
        QByteArray::fromHex("C0A8020CFFFFFF00C0A802010201"));
    QCOMPARE(configDIB.toString(), QString::fromLatin1("Total size { 0x10 }, Code { 0x03 }, "
        "Bytes { 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, 0xc0, 0xa8, 0x02, "
        "0x01, 0x02, 0x01 }"));
    QCOMPARE(configDIB.descriptionTypeCode(), QKnxNetIp::DescriptionTypeCode::IpConfiguration);
    QCOMPARE(configDIB.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(configDIB.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(configDIB.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(configDIB.capabilities(), QKnxNetIpConfigDIB::Capability::AutoIp);
    QCOMPARE(configDIB.assignmentMethods(), QKnxNetIpConfigDIB::AssignmentMethod::Manual);
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConfigDIB)

#include "tst_qknxnetipconfigdib.moc"
