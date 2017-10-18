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
#include <QtKnx/qknxnetipcurrentconfigdib.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpCurrentConfigDib : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithFourArguments();
    void testConstructorWithFiveArguments();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpCurrentConfigDib::testDefaultConstructor()
{
    QKnxNetIpCurrentConfigDib currentconfigDib;
    QCOMPARE(currentconfigDib.isValid(), false);
    QCOMPARE(currentconfigDib.size(), quint16(0));
    QCOMPARE(currentconfigDib.bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(currentconfigDib.payload().size(), quint16(0));
    QCOMPARE(currentconfigDib.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(currentconfigDib.toString(), QString::fromLatin1("Total size { 0x00 }, "
        "Code { 0x00 }, Bytes {  }"));
    QCOMPARE(quint8(currentconfigDib.descriptionType()), quint8(0));
    QCOMPARE(currentconfigDib.ipAddress(), QHostAddress());
    QCOMPARE(currentconfigDib.subnetMask(), QHostAddress());
    QCOMPARE(currentconfigDib.defaultGateway(), QHostAddress());
    QCOMPARE(currentconfigDib.dhcpOrBootP(), QHostAddress());
    QCOMPARE(quint8(currentconfigDib.assignmentMethod()), quint8(0));
}

void tst_QKnxNetIpCurrentConfigDib::testConstructorWithFourArguments()
{
    QNetworkAddressEntry addressEntry;
    addressEntry.setIp(QHostAddress("192.168.2.12"));
    addressEntry.setNetmask(QHostAddress("255.255.255.0"));
    QKnxNetIpCurrentConfigDib currentconfigDib(addressEntry,
                                               QHostAddress("192.168.2.1"),
                                               QHostAddress("180.138.3.2"),
                                               QKnxNetIpCurrentConfigDib::AssignmentMethod::Manual);

    QCOMPARE(currentconfigDib.isValid(), true);
    QCOMPARE(currentconfigDib.size(), quint16(20));
    QCOMPARE(currentconfigDib.bytes<QByteArray>(),
        QByteArray::fromHex("1404C0A8020CFFFFFF00C0A80201B48A03020100"));
    QCOMPARE(currentconfigDib.payload().size(), quint16(18));
    QCOMPARE(currentconfigDib.payload().bytes<QByteArray>(),
        QByteArray::fromHex("C0A8020CFFFFFF00C0A80201B48A03020100"));
    QCOMPARE(currentconfigDib.toString(), QString::fromLatin1("Total size { 0x14 }, "
        "Code { 0x04 }, Bytes { 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, "
        "0xc0, 0xa8, 0x02, 0x01, 0xb4, 0x8a, 0x03, 0x02, 0x01, 0x00 }"));
    QCOMPARE(currentconfigDib.descriptionType(),
        QKnxNetIp::DescriptionType::CurrentIpConfiguration);
    QCOMPARE(currentconfigDib.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(currentconfigDib.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(currentconfigDib.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(currentconfigDib.dhcpOrBootP(), QHostAddress("180.138.3.2"));
    QCOMPARE(currentconfigDib.assignmentMethod(),
        QKnxNetIpCurrentConfigDib::AssignmentMethod::Manual);
}

void tst_QKnxNetIpCurrentConfigDib::testConstructorWithFiveArguments()
{
    QKnxNetIpCurrentConfigDib currentconfigDib(QHostAddress("192.168.2.12"),
                                               QHostAddress("255.255.255.0"),
                                               QHostAddress("192.168.2.1"),
                                               QHostAddress("180.138.3.2"),
                                               QKnxNetIpCurrentConfigDib::AssignmentMethod::Manual);

    QCOMPARE(currentconfigDib.isValid(), true);
    QCOMPARE(currentconfigDib.size(), quint16(20));
    QCOMPARE(currentconfigDib.bytes<QByteArray>(),
        QByteArray::fromHex("1404C0A8020CFFFFFF00C0A80201B48A03020100"));
    QCOMPARE(currentconfigDib.payload().size(), quint16(18));
    QCOMPARE(currentconfigDib.payload().bytes<QByteArray>(),
        QByteArray::fromHex("C0A8020CFFFFFF00C0A80201B48A03020100"));
    QCOMPARE(currentconfigDib.toString(), QString::fromLatin1("Total size { 0x14 }, "
        "Code { 0x04 }, Bytes { 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, "
        "0xc0, 0xa8, 0x02, 0x01, 0xb4, 0x8a, 0x03, 0x02, 0x01, 0x00 }"));
    QCOMPARE(currentconfigDib.descriptionType(),
        QKnxNetIp::DescriptionType::CurrentIpConfiguration);
    QCOMPARE(currentconfigDib.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(currentconfigDib.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(currentconfigDib.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(currentconfigDib.dhcpOrBootP(), QHostAddress("180.138.3.2"));
    QCOMPARE(currentconfigDib.assignmentMethod(),
        QKnxNetIpCurrentConfigDib::AssignmentMethod::Manual);
}

void tst_QKnxNetIpCurrentConfigDib::testDebugStream()
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

    qDebug() << QKnxNetIpCurrentConfigDib();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    QNetworkAddressEntry addressEntry;
    addressEntry.setIp(QHostAddress("192.168.2.12"));
    addressEntry.setNetmask(QHostAddress("255.255.255.0"));
    qDebug() << QKnxNetIpCurrentConfigDib(addressEntry,
                                          QHostAddress("192.168.2.1"),
                                          QHostAddress("180.138.3.2"),
                                          QKnxNetIpCurrentConfigDib::AssignmentMethod::Manual);
    QCOMPARE(s_msg, QString::fromLatin1("0x1404c0a8020cffffff00c0a80201b48a03020100"));

    qDebug() << QKnxNetIpCurrentConfigDib(QHostAddress("192.168.2.12"),
                                          QHostAddress("255.255.255.0"),
                                          QHostAddress("192.168.2.1"),
                                          QHostAddress("180.138.3.2"),
                                          QKnxNetIpCurrentConfigDib::AssignmentMethod::Manual);
    QCOMPARE(s_msg, QString::fromLatin1("0x1404c0a8020cffffff00c0a80201b48a03020100"));
}

void tst_QKnxNetIpCurrentConfigDib::testDataStream()
{
    {
        QNetworkAddressEntry addressEntry;
        addressEntry.setIp(QHostAddress("192.168.2.12"));
        addressEntry.setNetmask(QHostAddress("255.255.255.0"));

        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpCurrentConfigDib(addressEntry,
                                         QHostAddress("192.168.2.1"),
                                         QHostAddress("180.138.3.2"),
                                         QKnxNetIpCurrentConfigDib::AssignmentMethod::Manual);
        QCOMPARE(byteArray, QByteArray::fromHex("1404C0A8020CFFFFFF00C0A80201B48A03020100"));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpCurrentConfigDib(QHostAddress("192.168.2.12"),
                                         QHostAddress("255.255.255.0"),
                                         QHostAddress("192.168.2.1"),
                                         QHostAddress("180.138.3.2"),
                                         QKnxNetIpCurrentConfigDib::AssignmentMethod::Manual);
        QCOMPARE(byteArray, QByteArray::fromHex("1404C0A8020CFFFFFF00C0A80201B48A03020100"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCurrentConfigDib)

#include "tst_qknxnetipcurrentconfigdib.moc"
