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
#include <QtKnx/qknxnetipconfigdib.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpConfigDib : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithFourArguments();
    void testConstructorWithFiveArguments();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpConfigDib::testDefaultConstructor()
{
    QKnxNetIpConfigDib configDib;
    QCOMPARE(configDib.isValid(), false);
    QCOMPARE(configDib.size(), quint16(0));
    QCOMPARE(configDib.bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(configDib.payload().size(), quint16(0));
    QCOMPARE(configDib.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(configDib.toString(), QString::fromLatin1("Total size { 0x00 }, Code { 0x00 }, "
        "Bytes {  }"));
    QCOMPARE(quint8(configDib.descriptionType()), quint8(0));
    QCOMPARE(configDib.ipAddress(), QHostAddress());
    QCOMPARE(configDib.subnetMask(), QHostAddress());
    QCOMPARE(configDib.defaultGateway(), QHostAddress());
    QCOMPARE(configDib.capabilities(), 0);
    QCOMPARE(configDib.assignmentMethods(), 0);
}

void tst_QKnxNetIpConfigDib::testConstructorWithFourArguments()
{
    QNetworkAddressEntry addressEntry;
    addressEntry.setIp(QHostAddress("192.168.2.12"));
    addressEntry.setNetmask(QHostAddress("255.255.255.0"));
    QKnxNetIpConfigDib configDib(addressEntry,
                                 QHostAddress("192.168.2.1"),
                                 QKnxNetIpConfigDib::Capability::AutoIp,
                                 QKnxNetIpConfigDib::AssignmentMethod::Manual);

    QCOMPARE(configDib.isValid(), true);
    QCOMPARE(configDib.size(), quint16(16));
    QCOMPARE(configDib.bytes<QByteArray>(),
        QByteArray::fromHex("1003C0A8020CFFFFFF00C0A802010201"));
    QCOMPARE(configDib.payload().size(), quint16(14));
    QCOMPARE(configDib.payload().bytes<QByteArray>(),
        QByteArray::fromHex("C0A8020CFFFFFF00C0A802010201"));
    QCOMPARE(configDib.toString(), QString::fromLatin1("Total size { 0x10 }, Code { 0x03 }, "
        "Bytes { 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, 0xc0, 0xa8, 0x02, "
        "0x01, 0x02, 0x01 }"));
    QCOMPARE(configDib.descriptionType(), QKnxNetIp::DescriptionType::IpConfiguration);
    QCOMPARE(configDib.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(configDib.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(configDib.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(configDib.capabilities(), QKnxNetIpConfigDib::Capability::AutoIp);
    QCOMPARE(configDib.assignmentMethods(), QKnxNetIpConfigDib::AssignmentMethod::Manual);
}

void tst_QKnxNetIpConfigDib::testConstructorWithFiveArguments()
{
    QKnxNetIpConfigDib configDib(QHostAddress("192.168.2.12"),
                                 QHostAddress("255.255.255.0"),
                                 QHostAddress("192.168.2.1"),
                                 QKnxNetIpConfigDib::Capability::AutoIp,
                                 QKnxNetIpConfigDib::AssignmentMethod::Manual);

    QCOMPARE(configDib.isValid(), true);
    QCOMPARE(configDib.size(), quint16(16));
    QCOMPARE(configDib.bytes<QByteArray>(),
        QByteArray::fromHex("1003C0A8020CFFFFFF00C0A802010201"));
    QCOMPARE(configDib.payload().size(), quint16(14));
    QCOMPARE(configDib.payload().bytes<QByteArray>(),
        QByteArray::fromHex("C0A8020CFFFFFF00C0A802010201"));
    QCOMPARE(configDib.toString(), QString::fromLatin1("Total size { 0x10 }, Code { 0x03 }, "
        "Bytes { 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, 0xc0, 0xa8, 0x02, "
        "0x01, 0x02, 0x01 }"));
    QCOMPARE(configDib.descriptionType(), QKnxNetIp::DescriptionType::IpConfiguration);
    QCOMPARE(configDib.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(configDib.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(configDib.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(configDib.capabilities(), QKnxNetIpConfigDib::Capability::AutoIp);
    QCOMPARE(configDib.assignmentMethods(), QKnxNetIpConfigDib::AssignmentMethod::Manual);
}

void tst_QKnxNetIpConfigDib::testDebugStream()
{
    struct DebugHandler
    {
        explicit DebugHandler(QtMessageHandler newMessageHandler)
            : oldMessageHandler(qInstallMessageHandler(newMessageHandler))
        {}
        ~DebugHandler()
        {
            qInstallMessageHandler(oldMessageHandler);
        }
        QtMessageHandler oldMessageHandler;
    } _(myMessageHandler);

    qDebug() << QKnxNetIpConfigDib();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    QNetworkAddressEntry addressEntry;
    addressEntry.setIp(QHostAddress("192.168.2.12"));
    addressEntry.setNetmask(QHostAddress("255.255.255.0"));
    qDebug() << QKnxNetIpConfigDib(addressEntry,
                                   QHostAddress("192.168.2.1"),
                                   QKnxNetIpConfigDib::Capability::AutoIp,
                                   QKnxNetIpConfigDib::AssignmentMethod::Manual);
    QCOMPARE(s_msg, QString::fromLatin1("0x1003c0a8020cffffff00c0a802010201"));

    qDebug() << QKnxNetIpConfigDib(QHostAddress("192.168.2.12"),
                                   QHostAddress("255.255.255.0"),
                                   QHostAddress("192.168.2.1"),
                                   QKnxNetIpConfigDib::Capability::AutoIp,
                                   QKnxNetIpConfigDib::AssignmentMethod::Manual);
    QCOMPARE(s_msg, QString::fromLatin1("0x1003c0a8020cffffff00c0a802010201"));
}

void tst_QKnxNetIpConfigDib::testDataStream()
{
    {
        QNetworkAddressEntry addressEntry;
        addressEntry.setIp(QHostAddress("192.168.2.12"));
        addressEntry.setNetmask(QHostAddress("255.255.255.0"));

        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpConfigDib(addressEntry,
                                  QHostAddress("192.168.2.1"),
                                  QKnxNetIpConfigDib::Capability::AutoIp,
                                  QKnxNetIpConfigDib::AssignmentMethod::Manual);
        QCOMPARE(byteArray, QByteArray::fromHex("1003C0A8020CFFFFFF00C0A802010201"));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpConfigDib(QHostAddress("192.168.2.12"),
                                  QHostAddress("255.255.255.0"),
                                  QHostAddress("192.168.2.1"),
                                  QKnxNetIpConfigDib::Capability::AutoIp,
                                  QKnxNetIpConfigDib::AssignmentMethod::Manual);
        QCOMPARE(byteArray, QByteArray::fromHex("1003C0A8020CFFFFFF00C0A802010201"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConfigDib)

#include "tst_qknxnetipconfigdib.moc"
