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
#include <QtKnx/qknxnetipknxaddressesdib.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpKnxAddressesDib : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithOneArgument();
    void testConstructorWithTwoArguments();
    void testIndividualAddresses();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpKnxAddressesDib::testDefaultConstructor()
{
    QKnxNetIpKnxAddressesDib addresses;
    QCOMPARE(addresses.isValid(), false);
    QCOMPARE(addresses.size(), quint16(0));
    QCOMPARE(addresses.bytes(), QKnxByteArray {});
    QCOMPARE(addresses.payload().size(), quint16(0));
    QCOMPARE(addresses.payload().bytes(), QKnxByteArray {});
    QCOMPARE(addresses.toString(), QString::fromLatin1("Total size { 0x00 }, "
        "Code { 0x00 }, Bytes {  }"));
    QCOMPARE(quint8(addresses.descriptionType()), quint8(0));
}

void tst_QKnxNetIpKnxAddressesDib::testConstructorWithOneArgument()
{
    QKnxNetIpKnxAddressesDib addresses(QKnxAddress::createIndividual(1, 1, 1));
    QCOMPARE(addresses.isValid(), true);
    QCOMPARE(addresses.size(), quint16(4));
    QCOMPARE(addresses.bytes(), QKnxByteArray::fromHex("04051101"));
    QCOMPARE(addresses.payload().size(), quint16(2));
    QCOMPARE(addresses.payload().bytes(), QKnxByteArray::fromHex("1101"));
    QCOMPARE(addresses.toString(), QString::fromLatin1("Total size { 0x04 }, "
        "Code { 0x05 }, Bytes { 0x11, 0x01 }"));
    QCOMPARE(addresses.descriptionType(),
        QKnxNetIp::DescriptionType::KnxAddresses);
}

void tst_QKnxNetIpKnxAddressesDib::testConstructorWithTwoArguments()
{
    QVector<QKnxAddress> qknxAddresses;
    qknxAddresses.append(QKnxAddress::createIndividual(1, 1, 0));
    qknxAddresses.append(QKnxAddress::createIndividual(1, 2, 5));
    qknxAddresses.append(QKnxAddress::createIndividual(2, 3, 8));
    QKnxNetIpKnxAddressesDib addresses(qknxAddresses);
    QCOMPARE(addresses.isValid(), true);
    QCOMPARE(addresses.size(), quint16(8));
    QCOMPARE(addresses.bytes(), QKnxByteArray::fromHex("0805110012052308"));
    QCOMPARE(addresses.payload().size(), quint16(6));
    QCOMPARE(addresses.payload().bytes(), QKnxByteArray::fromHex("110012052308"));
    QCOMPARE(addresses.toString(), QString::fromLatin1("Total size { 0x08 }, "
        "Code { 0x05 }, Bytes { 0x11, 0x00, 0x12, 0x05, 0x23, 0x08 }"));
    QCOMPARE(addresses.descriptionType(),
        QKnxNetIp::DescriptionType::KnxAddresses);
}

void tst_QKnxNetIpKnxAddressesDib::testIndividualAddresses()
{
    QVector<QKnxAddress> qknxAddresses;
    qknxAddresses.append(QKnxAddress::createIndividual(1, 1, 0));
    qknxAddresses.append(QKnxAddress::createIndividual(1, 2, 5));
    qknxAddresses.append(QKnxAddress::createIndividual(2, 3, 8));
    QKnxNetIpKnxAddressesDib addresses(qknxAddresses);

    auto retrievedAddresses = addresses.individualAddresses();
    QCOMPARE(retrievedAddresses.size(), qknxAddresses.size());
    QCOMPARE(retrievedAddresses[0].toString(), qknxAddresses[0].toString());
    QCOMPARE(retrievedAddresses[1].toString(), qknxAddresses[1].toString());
    QCOMPARE(retrievedAddresses[2].toString(), qknxAddresses[2].toString());
}

void tst_QKnxNetIpKnxAddressesDib::testDebugStream()
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

    qDebug() << QKnxNetIpKnxAddressesDib();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpKnxAddressesDib(QKnxAddress::createIndividual(1, 1, 1));
    QCOMPARE(s_msg, QString::fromLatin1("0x04051101"));

    QVector<QKnxAddress> qknxAddresses;
    qknxAddresses.append(QKnxAddress::createIndividual(1, 1, 0));
    qknxAddresses.append(QKnxAddress::createIndividual(1, 2, 5));
    qknxAddresses.append(QKnxAddress::createIndividual(2, 3, 8));
    qDebug() << QKnxNetIpKnxAddressesDib(qknxAddresses);
    QCOMPARE(s_msg, QString::fromLatin1("0x0805110012052308"));
}

void tst_QKnxNetIpKnxAddressesDib::testDataStream()
{
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpKnxAddressesDib(QKnxAddress::createIndividual(1, 1, 1));
        QCOMPARE(byteArray, QByteArray::fromHex("04051101"));
    }

    {
        QVector<QKnxAddress> qknxAddresses;
        qknxAddresses.append(QKnxAddress::createIndividual(1, 1, 0));
        qknxAddresses.append(QKnxAddress::createIndividual(1, 2, 5));
        qknxAddresses.append(QKnxAddress::createIndividual(2, 3, 8));

        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpKnxAddressesDib(qknxAddresses);
        QCOMPARE(byteArray, QByteArray::fromHex("0805110012052308"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpKnxAddressesDib)

#include "tst_qknxnetipknxaddressesdib.moc"
