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
    void testDataStream();
};

void tst_QKnxNetIpHpai::testConstructor()
{
    QKnxNetIpHpai hpai(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);

    QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Udp);
    QCOMPARE(hpai.address(), QHostAddress(QHostAddress::LocalHost));
    QCOMPARE(hpai.port(), quint16(3671));

    QCOMPARE(hpai.payload().size(), quint16(6));
    QCOMPARE(hpai.payload().bytes<QByteArray>(), QByteArray::fromHex("7f0000010e57"));

    QCOMPARE(hpai.size(), quint16(8));
    QCOMPARE(hpai.bytes<QByteArray>(), QByteArray::fromHex("08017f0000010e57"));

    QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x01 }, "
            "Bytes { 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));
}

void tst_QKnxNetIpHpai::testSetFunctions()
{
    // SetAll on an unvalid Hpai
    {
        QKnxNetIpHpai hpai;
        QCOMPARE(hpai.isValid(), false);

        hpai.setHpai(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Udp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::LocalHost));
        QCOMPARE(hpai.port(), quint16(3671));

        QCOMPARE(hpai.payload().size(), quint16(6));
        QCOMPARE(hpai.payload().bytes<QByteArray>(), QByteArray::fromHex("7f0000010e57"));

        QCOMPARE(hpai.size(), quint16(8));
        QCOMPARE(hpai.bytes<QByteArray>(), QByteArray::fromHex("08017f0000010e57"));

        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x01 }, "
                "Bytes { 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));
        QCOMPARE(hpai.isValid(), true);
    }

    // Set All on a valid Hpai
    {
        QKnxNetIpHpai hpai(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3333);
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Udp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::LocalHost));
        QCOMPARE(hpai.port(), quint16(3333));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x01 }, "
                "Bytes { 0x7f, 0x00, 0x00, 0x01, 0x0d, 0x05 }"));
        QCOMPARE(hpai.isValid(), true);

        hpai.setHpai(QKnxNetIp::HostProtocol::IpV4_Tcp, QHostAddress::Broadcast, 3671);
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(hpai.port(), quint16(3671));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x02 }, "
                "Bytes { 0xff, 0xff, 0xff, 0xff, 0x0e, 0x57 }"));
        QCOMPARE(hpai.isValid(), true);
    }

    // Set individual on an unvalid Hpai
    {
        QKnxNetIpHpai hpai;
        // Original values
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x00 }, Code { 0x00 }, "
                "Bytes {  }"));
        QCOMPARE(hpai.isValid(), false);

        // Setting address only
        hpai.setAddress(QHostAddress::Broadcast);
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Udp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(hpai.port(), quint16(0));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x01 }, "
                "Bytes { 0xff, 0xff, 0xff, 0xff, 0x00, 0x00 }"));
        QCOMPARE(hpai.isValid(), true);

        // Setting code only
        hpai.setHostProtocol(QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(hpai.port(), quint16(0));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x02 }, "
                "Bytes { 0xff, 0xff, 0xff, 0xff, 0x00, 0x00 }"));
        QCOMPARE(hpai.isValid(), true);

        // Setting port only
        hpai.setPort(quint16(3671));
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(hpai.port(), quint16(3671));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x02 }, "
                "Bytes { 0xff, 0xff, 0xff, 0xff, 0x0e, 0x57 }"));
        QCOMPARE(hpai.isValid(), true);
    }

    // Set individual on an unvalid Hpai, starting with code
    {
        QKnxNetIpHpai hpai;
        // Original values
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x00 }, Code { 0x00 }, "
                "Bytes {  }"));
        QCOMPARE(hpai.isValid(), false);

        // Setting code only
        hpai.setHostProtocol(QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::LocalHost));
        QCOMPARE(hpai.port(), quint16(0));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x02 }, "
                "Bytes { 0x7f, 0x00, 0x00, 0x01, 0x00, 0x00 }"));
        QCOMPARE(hpai.isValid(), true);

        // Setting address only
        hpai.setAddress(QHostAddress::Broadcast);
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(hpai.port(), quint16(0));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x02 }, "
                "Bytes { 0xff, 0xff, 0xff, 0xff, 0x00, 0x00 }"));
        QCOMPARE(hpai.isValid(), true);

        // Setting port only
        hpai.setPort(quint16(3671));
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(hpai.port(), quint16(3671));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x02 }, "
                "Bytes { 0xff, 0xff, 0xff, 0xff, 0x0e, 0x57 }"));
        QCOMPARE(hpai.isValid(), true);
    }

    // Set individual on a valid Hpai
    {
        QKnxNetIpHpai hpai(QKnxNetIp::HostProtocol::IpV4_Udp,
            QHostAddress(QHostAddress::AnyIPv4), 3333);
        // Original values
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Udp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::AnyIPv4));
        QCOMPARE(hpai.port(), quint16(3333));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x01 }, "
                "Bytes { 0x00, 0x00, 0x00, 0x00, 0x0d, 0x05 }"));
        QCOMPARE(hpai.isValid(), true);

        // Setting code only
        hpai.setHostProtocol(QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::AnyIPv4));
        QCOMPARE(hpai.port(), quint16(3333));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x02 }, "
                "Bytes { 0x00, 0x00, 0x00, 0x00, 0x0d, 0x05 }"));
        QCOMPARE(hpai.isValid(), true);

        // Setting address only
        hpai.setAddress(QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(hpai.port(), quint16(3333));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x02 }, "
                "Bytes { 0xff, 0xff, 0xff, 0xff, 0x0d, 0x05 }"));
        QCOMPARE(hpai.isValid(), true);

        // Setting port only
        hpai.setPort(quint16(3671));
        QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Tcp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::Broadcast));
        QCOMPARE(hpai.port(), quint16(3671));
        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x02 }, "
                "Bytes { 0xff, 0xff, 0xff, 0xff, 0x0e, 0x57 }"));
        QCOMPARE(hpai.isValid(), true);
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

    qDebug() << QKnxNetIpHpai();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpHpai(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QCOMPARE(s_msg, QString::fromLatin1("0x08017f0000010e57"));
}

void tst_QKnxNetIpHpai::testDataStream()
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out << QKnxNetIpHpai(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QCOMPARE(byteArray, QByteArray::fromHex("08017f0000010e57"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpHpai)

#include "tst_qknxnetiphpai.moc"
