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
};

void tst_QKnxNetIpConfigDib::testDefaultConstructor()
{
    auto configDib = QKnxNetIpConfigDibProxy::builder().create();
    QCOMPARE(configDib.isValid(), true);
    QCOMPARE(configDib.size(), quint16(2));
    QCOMPARE(configDib.bytes(), QKnxByteArray({ 0x02, 0x03 }));
    QCOMPARE(configDib.data().size(), quint16(0));
    QCOMPARE(configDib.data(), QKnxByteArray {});

    const QKnxNetIpConfigDibProxy view(configDib);
    QCOMPARE(view.isValid(), false);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
    QCOMPARE(view.ipAddress(), QHostAddress());
    QCOMPARE(view.subnetMask(), QHostAddress());
    QCOMPARE(view.defaultGateway(), QHostAddress());
    QCOMPARE(view.capabilities(), QKnxNetIp::Capability::Unknown);
    QCOMPARE(view.assignmentMethods(), QKnxNetIp::AssignmentMethod::Unknown);
}

void tst_QKnxNetIpConfigDib::testConstructorWithFourArguments()
{
    auto configDib = QKnxNetIpConfigDibProxy::builder()
        .setIpAddress(QHostAddress("192.168.2.12"))
        .setSubnetMask(QHostAddress("255.255.255.0"))
        .setDefaultGateway(QHostAddress("192.168.2.1"))
        .setCapabilities(QKnxNetIp::Capability::AutoIp | QKnxNetIp::Capability::Dhcp)
        .setAssignmentMethods(QKnxNetIp::AssignmentMethod::Manual
            | QKnxNetIp::AssignmentMethod::AutoIp | QKnxNetIp::AssignmentMethod::Dhcp)
        .create();

    QCOMPARE(configDib.isValid(), true);
    QCOMPARE(configDib.size(), quint16(16));
    QCOMPARE(configDib.bytes(),
        QKnxByteArray({ 0x10, 0x03, 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, 0xc0, 0xa8,
            0x02, 0x01, 0x06, 0x0d }));
    QCOMPARE(configDib.data().size(), quint16(14));
    QCOMPARE(configDib.data(),
        QKnxByteArray({ 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, 0xc0, 0xa8, 0x02,
            0x01, 0x06, 0x0d }));

    const QKnxNetIpConfigDibProxy view(configDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::IpConfiguration);
    QCOMPARE(view.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(view.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(view.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(view.capabilities(), QKnxNetIp::Capability::AutoIp | QKnxNetIp::Capability::Dhcp);
    QCOMPARE(view.assignmentMethods(), QKnxNetIp::AssignmentMethod::Manual
        | QKnxNetIp::AssignmentMethod::AutoIp | QKnxNetIp::AssignmentMethod::Dhcp);
}

void tst_QKnxNetIpConfigDib::testConstructorWithFiveArguments()
{
    auto configDib = QKnxNetIpConfigDibProxy::builder()
        .setIpAddress(QHostAddress("192.168.2.12"))
        .setSubnetMask(QHostAddress("255.255.255.0"))
        .setDefaultGateway(QHostAddress("192.168.2.1"))
        .setCapabilities(QKnxNetIp::Capability::BootP)
        .setAssignmentMethods(QKnxNetIp::AssignmentMethod::Manual
            | QKnxNetIp::AssignmentMethod::BootP)
        .create();

    QCOMPARE(configDib.isValid(), true);
    QCOMPARE(configDib.size(), quint16(16));
    QCOMPARE(configDib.bytes(),
        QKnxByteArray({ 0x10, 0x03, 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, 0xc0, 0xa8,
            0x02, 0x01, 0x01, 0x03 }));
    QCOMPARE(configDib.data().size(), quint16(14));
    QCOMPARE(configDib.data(),
        QKnxByteArray({ 0xc0, 0xa8, 0x02, 0x0c, 0xff, 0xff, 0xff, 0x00, 0xc0, 0xa8, 0x02,
            0x01, 0x01, 0x03 }));

    const QKnxNetIpConfigDibProxy view(configDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::IpConfiguration);
    QCOMPARE(view.ipAddress(), QHostAddress("192.168.2.12"));
    QCOMPARE(view.subnetMask(), QHostAddress("255.255.255.0"));
    QCOMPARE(view.defaultGateway(), QHostAddress("192.168.2.1"));
    QCOMPARE(view.capabilities(), QKnxNetIp::Capability::BootP);
    QCOMPARE(view.assignmentMethods(), QKnxNetIp::AssignmentMethod::Manual
        | QKnxNetIp::AssignmentMethod::BootP);
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

    qDebug() << QKnxNetIpConfigDibProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0203"));

    qDebug() << QKnxNetIpConfigDibProxy::builder()
        .setIpAddress(QHostAddress("192.168.2.12"))
        .setSubnetMask(QHostAddress("255.255.255.0"))
        .setDefaultGateway(QHostAddress("192.168.2.1"))
        .setCapabilities(QKnxNetIp::Capability::AutoIp)
        .setAssignmentMethods(QKnxNetIp::AssignmentMethod::Manual)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x1003c0a8020cffffff00c0a802010401"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConfigDib)

#include "tst_qknxnetipconfigdib.moc"
