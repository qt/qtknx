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
#include <QtKnx/qknxnetipconnectionstaterequest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpConnectionStateRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpConnectionStateRequest::testDefaultConstructor()
{
    QKnxNetIpConnectionStateRequest connectionStateRequest;
    QCOMPARE(connectionStateRequest.isValid(), false);
    QCOMPARE(connectionStateRequest.channelId(), quint8(0));
    QCOMPARE(connectionStateRequest.controlEndpoint().isValid(), false);
}

void tst_QKnxNetIpConnectionStateRequest::testConstructor()
{
    QKnxNetIpHpai hpai(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost,
            3671);
    quint8 channelID= 255;
    QKnxNetIpConnectionStateRequest connectionStateRequest(channelID, hpai);
    QCOMPARE(connectionStateRequest.isValid(), true);
    QCOMPARE(connectionStateRequest.size(), quint16(16));
    QCOMPARE(connectionStateRequest.bytes(),
        QKnxByteArray({ 0x06, 0x10, 0x02, 0x07, 0x00, 0x10, 0xff, 0x00, 0x08, 0x01, 0x7f, 0x00,
            0x00, 0x01, 0x0e, 0x57 }));
    QCOMPARE(connectionStateRequest.payload().size(), quint16(10));
    QCOMPARE(connectionStateRequest.payload().bytes(),
        QKnxByteArray({ 0xff, 0x00, 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }));
    QCOMPARE(connectionStateRequest.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Service type { 0x207 }, Total size { 0x10 }, "
            "Bytes { 0xff, 0x00, 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));

    QCOMPARE(connectionStateRequest.channelId(), quint8(255));
    QCOMPARE(connectionStateRequest.controlEndpoint().bytes(), hpai.bytes());
}

void tst_QKnxNetIpConnectionStateRequest::testDebugStream()
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

    qDebug() << QKnxNetIpConnectionStateRequest();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    QKnxNetIpHpai hpai(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    quint8 channelID= 255;
    qDebug() << QKnxNetIpConnectionStateRequest(channelID, hpai);
    QCOMPARE(s_msg, QString::fromLatin1("0x061002070010ff0008017f0000010e57"));
}

void tst_QKnxNetIpConnectionStateRequest::testDataStream()
{
    QKnxNetIpHpai hpai(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    quint8 channelID= 255;
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out << QKnxNetIpConnectionStateRequest(channelID, hpai);
    QCOMPARE(byteArray, QByteArray::fromHex("061002070010ff0008017f0000010e57"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectionStateRequest)

#include "tst_qknxnetipconnectionstaterequest.moc"
