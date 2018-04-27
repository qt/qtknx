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
    void testDebugStream();
};

void tst_QKnxNetIpCurrentConfigDib::testDefaultConstructor()
{
    auto currentconfigDib = QKnxNetIpCurrentConfigDibProxy::builder().create();
    QCOMPARE(currentconfigDib.isValid(), true);
    QCOMPARE(currentconfigDib.size(), quint16(2));
    QCOMPARE(currentconfigDib.bytes(), QKnxByteArray({ 0x02, 0x04 }));
    QCOMPARE(currentconfigDib.data().size(), quint16(0));
    QCOMPARE(currentconfigDib.data(), QKnxByteArray {});

    const QKnxNetIpCurrentConfigDibProxy view(currentconfigDib);
    QCOMPARE(view.isValid(), false);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
    QCOMPARE(view.ipAddress(), QHostAddress());
    QCOMPARE(view.subnetMask(), QHostAddress());
    QCOMPARE(view.defaultGateway(), QHostAddress());
    QCOMPARE(view.dhcpOrBootP(), QHostAddress());
    QCOMPARE(view.assignmentMethod(), QKnxNetIp::AssignmentMethod::Unknown);
}

void tst_QKnxNetIpCurrentConfigDib::testConstructorWithFourArguments()
{
    auto currentconfigDib = QKnxNetIpCurrentConfigDibProxy::builder()
        .setIpAddress(QHostAddress("192.168.2.12"))
        .setSubnetMask(QHostAddress("255.255.255.0"))
        .setDefaultGateway(QHostAddress("192.168.2.1"))
        .setDhcpOrBootP(QHostAddress("180.138.3.2"))
        .setAssignmentMethod(QKnxNetIp::AssignmentMethod::Manual)
        .create();

    QCOMPARE(currentconfigDib.isValid(), true);
    QCOMPARE(currentconfigDib.size(), quint16(20));
    QCOMPARE(currentconfigDib.bytes(),
        QKnxByteArray::fromHex("1404C0A8020CFFFFFF00C0A80201B48A03020100"));
    QCOMPARE(currentconfigDib.data().size(), quint16(18));
    QCOMPARE(currentconfigDib.data(),
        QKnxByteArray::fromHex("C0A8020CFFFFFF00C0A80201B48A03020100"));

    const QKnxNetIpCurrentConfigDibProxy view(currentconfigDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::CurrentIpConfiguration);
    QCOMPARE(view.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(view.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(view.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(view.dhcpOrBootP(), QHostAddress("180.138.3.2"));
    QCOMPARE(view.assignmentMethod(), QKnxNetIp::AssignmentMethod::Manual);
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

    qDebug() << QKnxNetIpCurrentConfigDibProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0204"));

    qDebug() << QKnxNetIpCurrentConfigDibProxy::builder()
        .setIpAddress(QHostAddress("192.168.2.12"))
        .setSubnetMask(QHostAddress("255.255.255.0"))
        .setDefaultGateway(QHostAddress("192.168.2.1"))
        .setDhcpOrBootP(QHostAddress("180.138.3.2"))
        .setAssignmentMethod(QKnxNetIp::AssignmentMethod::Manual)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x1404c0a8020cffffff00c0a80201b48a03020100"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCurrentConfigDib)

#include "tst_qknxnetipcurrentconfigdib.moc"
