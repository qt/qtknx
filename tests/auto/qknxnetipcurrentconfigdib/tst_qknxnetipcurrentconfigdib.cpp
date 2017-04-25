/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipcurrentconfigdib.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpCurrentConfigDIB : public QObject
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

void tst_QKnxNetIpCurrentConfigDIB::testDefaultConstructor()
{
    QKnxNetIpCurrentConfigDIB currentconfigDIB;
    QCOMPARE(currentconfigDIB.isValid(), false);
    QCOMPARE(currentconfigDIB.size(), quint16(0));
    QCOMPARE(currentconfigDIB.bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(currentconfigDIB.payload().size(), quint16(0));
    QCOMPARE(currentconfigDIB.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(currentconfigDIB.toString(), QString::fromLatin1("Total size { 0x00 }, "
        "Code { 0x00 }, Bytes {  }"));
    QCOMPARE(quint8(currentconfigDIB.descriptionTypeCode()), quint8(0));
    QCOMPARE(currentconfigDIB.ipAddress(), QHostAddress());
    QCOMPARE(currentconfigDIB.subnetMask(), QHostAddress());
    QCOMPARE(currentconfigDIB.defaultGateway(), QHostAddress());
    QCOMPARE(currentconfigDIB.dhcpOrBootP(), QHostAddress());
    QCOMPARE(quint8(currentconfigDIB.assignmentMethod()), quint8(0));
}

void tst_QKnxNetIpCurrentConfigDIB::testConstructorWithFourArguments()
{
    QNetworkAddressEntry addressEntry;
    addressEntry.setIp(QHostAddress("192.168.2.12"));
    addressEntry.setNetmask(QHostAddress("255.255.255.0"));
    QKnxNetIpCurrentConfigDIB currentconfigDIB(addressEntry,
                                               QHostAddress("192.168.2.1"),
                                               QHostAddress("180.138.3.2"),
                                               QKnxNetIpCurrentConfigDIB::AssignmentMethod::Manual);

    QCOMPARE(currentconfigDIB.isValid(), true);
    QCOMPARE(currentconfigDIB.size(), quint16(20));
    QCOMPARE(currentconfigDIB.bytes<QByteArray>(),
        QByteArray::fromHex("1404C0A8020CFFFFFF00C0A80201B48A03020100"));
    QCOMPARE(currentconfigDIB.payload().size(), quint16(18));
    QCOMPARE(currentconfigDIB.payload().bytes<QByteArray>(),
        QByteArray::fromHex("C0A8020CFFFFFF00C0A80201B48A03020100"));
    QCOMPARE(currentconfigDIB.toString(), QString::fromLatin1("Total size { 0x14 }, "
        "Code { 0x04 }, Bytes { 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, "
        "0xc0, 0xa8, 0x02, 0x01, 0xb4, 0x8a, 0x03, 0x02, 0x01, 0x00 }"));
    QCOMPARE(currentconfigDIB.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::CurrentIpConfiguration);
    QCOMPARE(currentconfigDIB.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(currentconfigDIB.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(currentconfigDIB.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(currentconfigDIB.dhcpOrBootP(), QHostAddress("180.138.3.2"));
    QCOMPARE(currentconfigDIB.assignmentMethod(),
        QKnxNetIpCurrentConfigDIB::AssignmentMethod::Manual);
}

void tst_QKnxNetIpCurrentConfigDIB::testConstructorWithFiveArguments()
{
    QKnxNetIpCurrentConfigDIB currentconfigDIB(QHostAddress("192.168.2.12"),
                                               QHostAddress("255.255.255.0"),
                                               QHostAddress("192.168.2.1"),
                                               QHostAddress("180.138.3.2"),
                                               QKnxNetIpCurrentConfigDIB::AssignmentMethod::Manual);

    QCOMPARE(currentconfigDIB.isValid(), true);
    QCOMPARE(currentconfigDIB.size(), quint16(20));
    QCOMPARE(currentconfigDIB.bytes<QByteArray>(),
        QByteArray::fromHex("1404C0A8020CFFFFFF00C0A80201B48A03020100"));
    QCOMPARE(currentconfigDIB.payload().size(), quint16(18));
    QCOMPARE(currentconfigDIB.payload().bytes<QByteArray>(),
        QByteArray::fromHex("C0A8020CFFFFFF00C0A80201B48A03020100"));
    QCOMPARE(currentconfigDIB.toString(), QString::fromLatin1("Total size { 0x14 }, "
        "Code { 0x04 }, Bytes { 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, "
        "0xc0, 0xa8, 0x02, 0x01, 0xb4, 0x8a, 0x03, 0x02, 0x01, 0x00 }"));
    QCOMPARE(currentconfigDIB.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::CurrentIpConfiguration);
    QCOMPARE(currentconfigDIB.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(currentconfigDIB.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(currentconfigDIB.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(currentconfigDIB.dhcpOrBootP(), QHostAddress("180.138.3.2"));
    QCOMPARE(currentconfigDIB.assignmentMethod(),
        QKnxNetIpCurrentConfigDIB::AssignmentMethod::Manual);
}
QTEST_APPLESS_MAIN(tst_QKnxNetIpCurrentConfigDIB)

#include "tst_qknxnetipcurrentconfigdib.moc"
