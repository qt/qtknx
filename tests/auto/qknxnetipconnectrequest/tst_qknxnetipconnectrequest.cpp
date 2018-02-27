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
#include <QtTest/qtest.h>
#include <QtKnx/qknxnetipconnectrequest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpConnectRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpConnectRequest::testDefaultConstructor()
{
    QKnxNetIpConnectRequest connectRequest;
    QCOMPARE(connectRequest.isValid(), false);
    QCOMPARE(connectRequest.controlEndpoint().isValid(), false);
    QCOMPARE(connectRequest.dataEndpoint().isValid(), false);
    QCOMPARE(connectRequest.requestInformation().isValid(), false);
}

void tst_QKnxNetIpConnectRequest::testConstructor()
{
    QKnxNetIpHpai control(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpHpai data(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpCri request;

    QKnxNetIpConnectRequest connectRequest(control, data, request);

    QCOMPARE(connectRequest.isValid(), false);
    QCOMPARE(connectRequest.size(), quint16(22));
    QCOMPARE(connectRequest.bytes(),
        QKnxByteArray::fromHex("06100205001608017f0000010e5708017f0000010e57"));
    QCOMPARE(connectRequest.payload().size(), quint16(16));
    QCOMPARE(connectRequest.payload().bytes(),
        QKnxByteArray::fromHex("08017f0000010e5708017f0000010e57"));
    QCOMPARE(connectRequest.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Service type { 0x205 }, Total size { 0x16 }, "
            "Bytes { 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57, 0x08, 0x01, "
            "0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));
    QCOMPARE(connectRequest.controlEndpoint().bytes(),
        QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.dataEndpoint().bytes(),
        QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.requestInformation().bytes(), QKnxByteArray {});

    request.setConnectionType(QKnxNetIp::ConnectionType::Tunnel);
    request.setTunnelingLayer(QKnxNetIp::TunnelingLayer::Link);
    connectRequest = QKnxNetIpConnectRequest(control, data, request);

    QCOMPARE(connectRequest.isValid(), true);
    QCOMPARE(connectRequest.size(), quint16(26));
    QCOMPARE(connectRequest.bytes(),
        QKnxByteArray::fromHex("06100205001a08017f0000010e5708017f0000010e5704040200"));
    QCOMPARE(connectRequest.payload().size(), quint16(20));
    QCOMPARE(connectRequest.payload().bytes(),
        QKnxByteArray::fromHex("08017f0000010e5708017f0000010e5704040200"));
    QCOMPARE(connectRequest.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Service type { 0x205 }, Total size { 0x1a }, "
            "Bytes { 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57, 0x08, 0x01, "
            "0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57, 0x04, 0x04, 0x02, 0x00 }"));

    QCOMPARE(connectRequest.controlEndpoint().bytes(),
        QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.dataEndpoint().bytes(),
        QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.requestInformation().bytes(), QKnxByteArray::fromHex("04040200"));
}

void tst_QKnxNetIpConnectRequest::testDebugStream()
{
    struct DebugHandler
    {
        explicit DebugHandler(QtMessageHandler newMessageHandler)
            : oldMessageHandler(qInstallMessageHandler(newMessageHandler)) {}
        ~DebugHandler() {
            qInstallMessageHandler(oldMessageHandler);
        }
        QtMessageHandler oldMessageHandler;
    } _(myMessageHandler);

    qDebug() << QKnxNetIpConnectRequest();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpConnectRequest::fromBytes(QKnxByteArray::fromHex("06100205001a0801c0a80001c3"
        "b40801c0a80001c3b404040200"), 0);
    QCOMPARE(s_msg, QString::fromLatin1("0x06100205001a0801c0a80001c3b40801c0a80001c3b404040200"));
}

void tst_QKnxNetIpConnectRequest::testDataStream()
{
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);

        QKnxNetIpHpai control(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
        QKnxNetIpHpai data(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
        QKnxNetIpCri request;
        out << QKnxNetIpConnectRequest(control, data, request);
        QCOMPARE(byteArray, QByteArray(""));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);

        QKnxNetIpHpai control(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
        QKnxNetIpHpai data(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
        QKnxNetIpCri request(QKnxNetIp::TunnelingLayer::Link);
        out << QKnxNetIpConnectRequest(control, data, request);
        QCOMPARE(byteArray, QByteArray::fromHex("06100205001a08017f0000010e5708017f0000010e5704040200"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectRequest)

#include "tst_qknxnetipconnectrequest.moc"
