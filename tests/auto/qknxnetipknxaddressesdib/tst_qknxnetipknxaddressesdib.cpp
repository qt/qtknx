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
};

void tst_QKnxNetIpKnxAddressesDib::testDefaultConstructor()
{
    auto addrDib = QKnxNetIpKnxAddressesDibProxy::builder().create();
    QCOMPARE(addrDib.isValid(), true);
    QCOMPARE(addrDib.size(), quint16(2));
    QCOMPARE(addrDib.bytes(), QKnxByteArray::fromHex("0205"));
    QCOMPARE(addrDib.data().size(), quint16(0));
    QCOMPARE(addrDib.data(), QKnxByteArray {});

    QKnxNetIpKnxAddressesDibProxy view(addrDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::KnxAddresses);
    QCOMPARE(view.individualAddresses(), QVector<QKnxAddress>{});
}

void tst_QKnxNetIpKnxAddressesDib::testConstructorWithOneArgument()
{
    auto addresses = QKnxNetIpKnxAddressesDibProxy::builder()
        .setIndividualAddresses({ QKnxAddress::createIndividual(1, 1, 1) })
        .create();
    QCOMPARE(addresses.isValid(), true);
    QCOMPARE(addresses.size(), quint16(4));
    QCOMPARE(addresses.bytes(), QKnxByteArray::fromHex("04051101"));
    QCOMPARE(addresses.data().size(), quint16(2));
    QCOMPARE(addresses.data(), QKnxByteArray::fromHex("1101"));

    QKnxNetIpKnxAddressesDibProxy view(addresses);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::KnxAddresses);
    QCOMPARE(view.individualAddresses(), QVector<QKnxAddress> { QKnxAddress::createIndividual(1, 1, 1) });
}

void tst_QKnxNetIpKnxAddressesDib::testConstructorWithTwoArguments()
{
    QVector<QKnxAddress> knxAddresses;
    knxAddresses.append(QKnxAddress::createIndividual(1, 1, 0));
    knxAddresses.append(QKnxAddress::createIndividual(1, 2, 5));
    knxAddresses.append(QKnxAddress::createIndividual(2, 3, 8));

    auto addresses = QKnxNetIpKnxAddressesDibProxy::builder()
        .setIndividualAddresses(knxAddresses)
        .create();

    QCOMPARE(addresses.isValid(), true);
    QCOMPARE(addresses.size(), quint16(8));
    QCOMPARE(addresses.bytes(), QKnxByteArray::fromHex("0805110012052308"));
    QCOMPARE(addresses.data().size(), quint16(6));
    QCOMPARE(addresses.data(), QKnxByteArray::fromHex("110012052308"));

    QKnxNetIpKnxAddressesDibProxy view(addresses);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::KnxAddresses);
    QCOMPARE(view.individualAddresses(), knxAddresses);
}

void tst_QKnxNetIpKnxAddressesDib::testIndividualAddresses()
{
    QVector<QKnxAddress> knxAddresses;
    knxAddresses.append(QKnxAddress::createIndividual(1, 1, 0));
    knxAddresses.append(QKnxAddress::createGroup(1, 2, 5));
    knxAddresses.append(QKnxAddress::createIndividual(2, 3, 8));

    auto addresses = QKnxNetIpKnxAddressesDibProxy::builder()
        .setIndividualAddresses(knxAddresses)
        .create();

    QCOMPARE(addresses.isValid(), true);
    QCOMPARE(addresses.size(), quint16(6));
    QCOMPARE(addresses.bytes(), QKnxByteArray::fromHex("060511002308"));
    QCOMPARE(addresses.data().size(), quint16(4));
    QCOMPARE(addresses.data(), QKnxByteArray::fromHex("11002308"));

    QKnxNetIpKnxAddressesDibProxy view(addresses);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::KnxAddresses);

    auto retrievedAddresses = view.individualAddresses();
    QCOMPARE(retrievedAddresses.size(), knxAddresses.size() - 1);

    QCOMPARE(retrievedAddresses[0], knxAddresses[0]);
    QCOMPARE(retrievedAddresses[1], knxAddresses[2]);
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

    qDebug() << QKnxNetIpKnxAddressesDibProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0205"));

    qDebug() << QKnxNetIpKnxAddressesDibProxy::builder()
        .setIndividualAddresses({ QKnxAddress::createIndividual(1, 1, 1) })
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x04051101"));

    qDebug() << QKnxNetIpKnxAddressesDibProxy::builder()
        .setIndividualAddresses({
            QKnxAddress::createIndividual(1, 1, 0),
            QKnxAddress::createIndividual(1, 2, 5),
            QKnxAddress::createIndividual(2, 3, 8)
        }).create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0805110012052308"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpKnxAddressesDib)

#include "tst_qknxnetipknxaddressesdib.moc"
