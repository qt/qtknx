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
#include <QtKnx/qknxnetiphpai.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpHpai : public QObject
{
    Q_OBJECT

private slots:
    void testConstructor();
    void testSetFunctions();
    void testDebugStream();
};

void tst_QKnxNetIpHpai::testConstructor()
{
    auto hpai = QKnxNetIpHpaiProxy::builder()
        .setHostAddress(QHostAddress::LocalHost)
        .setPort(3671).create();
    const QKnxNetIpHpaiProxy view(hpai);

    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::UDP_IPv4);
    QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::LocalHost));
    QCOMPARE(view.port(), quint16(3671));

    QCOMPARE(hpai.data().size(), quint16(6));
    QCOMPARE(hpai.data(), QKnxByteArray::fromHex("7f0000010e57"));

    QCOMPARE(hpai.size(), quint16(8));
    QCOMPARE(hpai.bytes(), QKnxByteArray::fromHex("08017f0000010e57"));
}

void tst_QKnxNetIpHpai::testSetFunctions()
{
    {
        QKnxNetIpHpai hpai;
        QCOMPARE(hpai.isValid(), false);

        hpai = QKnxNetIpHpaiProxy::builder()
            .setHostAddress(QHostAddress::LocalHost)
            .setPort(3671).create();

        QKnxNetIpHpaiProxy view(hpai);
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::UDP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::LocalHost));
        QCOMPARE(view.port(), quint16(3671));

        QCOMPARE(hpai.data().size(), quint16(6));
        QCOMPARE(hpai.data(), QKnxByteArray::fromHex("7f0000010e57"));

        QCOMPARE(hpai.size(), quint16(8));
        QCOMPARE(hpai.bytes(), QKnxByteArray::fromHex("08017f0000010e57"));

        QCOMPARE(hpai.isValid(), true);
    }

    {
        QKnxNetIpHpai hpai;
        QCOMPARE(hpai.isValid(), false);

        hpai = QKnxNetIpHpaiProxy::builder().setHostAddress(QHostAddress::LocalHost)
            .setPort(3333).create();

        QKnxNetIpHpaiProxy view(hpai);
        QCOMPARE(view.isValid(), true);
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::UDP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::LocalHost));
        QCOMPARE(view.port(), quint16(3333));

        hpai = QKnxNetIpHpaiProxy::builder().setHostProtocol(QKnxNetIp::HostProtocol::TCP_IPv4)
            .setHostAddress(QHostAddress::Broadcast).setPort(3671).create();
        QCOMPARE(view.isValid(), true);
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(view.port(), quint16(3671));
    }

    {
        QKnxNetIpHpai hpai;
        QCOMPARE(hpai.isValid(), false);

        QKnxNetIpHpaiProxy view(hpai);
        auto builder = view.builder();

        // Setting address only
        hpai = builder.setHostAddress(QHostAddress::Broadcast).create();

        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::UDP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(view.port(), quint16(0));
        QCOMPARE(view.isValid(), true);

        // Setting code only
        hpai = builder.setHostProtocol(QKnxNetIp::HostProtocol::TCP_IPv4).create();
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(view.port(), quint16(0));
        QCOMPARE(view.isValid(), true);

        // Setting port only
        hpai = builder.setPort(quint16(3671)).create();
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(view.port(), quint16(3671));
        QCOMPARE(view.isValid(), true);
    }

    {
        QKnxNetIpHpai hpai;
        QCOMPARE(hpai.isValid(), false);

        QKnxNetIpHpaiProxy view(hpai);
        auto builder = view.builder();

        // Setting code only
        hpai = builder.setHostProtocol(QKnxNetIp::HostProtocol::TCP_IPv4).create();
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::LocalHost));
        QCOMPARE(view.port(), quint16(0));
        QCOMPARE(view.isValid(), true);

        // Setting address only
        hpai = builder.setHostAddress(QHostAddress::Broadcast).create();
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(view.port(), quint16(0));
        QCOMPARE(view.isValid(), true);

        // Setting port only
        hpai = builder.setPort(quint16(3671)).create();
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(view.port(), quint16(3671));
        QCOMPARE(view.isValid(), true);
    }

    {
        auto builder = QKnxNetIpHpaiProxy::builder();
        auto hpai = builder. setHostProtocol(QKnxNetIp::HostProtocol::UDP_IPv4)
            .setHostAddress(QHostAddress::AnyIPv4).setPort(3333).create();

        QKnxNetIpHpaiProxy view(hpai);

        // Original values
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::UDP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::AnyIPv4));
        QCOMPARE(view.port(), quint16(3333));
        QCOMPARE(view.isValid(), true);

        // Setting code only
        hpai = builder.setHostProtocol(QKnxNetIp::HostProtocol::TCP_IPv4).create();
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::AnyIPv4));
        QCOMPARE(view.port(), quint16(3333));
        QCOMPARE(view.isValid(), true);

        // Setting address only
        hpai = builder.setHostAddress(QHostAddress(QHostAddress::Broadcast)).create();
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(view.port(), quint16(3333));
        QCOMPARE(view.isValid(), true);

        // Setting port only
        hpai = builder.setPort(quint16(3671)).create();
        QCOMPARE(view.hostProtocol(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(view.hostAddress(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(view.port(), quint16(3671));
        QCOMPARE(view.isValid(), true);
    }
}

void tst_QKnxNetIpHpai::testDebugStream()
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

    qDebug() << QKnxNetIpHpaiProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x08017f0000010000"));

    qDebug() << QKnxNetIpHpaiProxy::builder()
        .setHostProtocol(QKnxNetIp::HostProtocol::UDP_IPv4)
        .setHostAddress(QHostAddress::LocalHost)
        .setPort(3671).create();
    QCOMPARE(s_msg, QString::fromLatin1("0x08017f0000010e57"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpHpai)

#include "tst_qknxnetiphpai.moc"
