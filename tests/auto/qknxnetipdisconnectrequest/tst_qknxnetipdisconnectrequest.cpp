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
#include <QtKnx/qknxnetipdisconnectrequest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDisconnectRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
};

void tst_QKnxNetIpDisconnectRequest::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpDisconnectRequest request(frame);

    QCOMPARE(request.isValid(), false);
    QCOMPARE(request.channelId(), quint8(0));
    QCOMPARE(request.controlEndpoint().isValid(), false);
}

void tst_QKnxNetIpDisconnectRequest::testConstructor()
{
    auto frame = QKnxNetIpDisconnectRequest::builder()
        .setChannelId(200)
        .setControlEndpoint({ QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671 })
        .create();
    QKnxNetIpDisconnectRequest request(frame);

    QCOMPARE(request.isValid(), true);
    QCOMPARE(frame.size(), quint16(16));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("061002090010c80008017f0000010e57"));
    QCOMPARE(frame.data().size(), quint16(10));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("c80008017f0000010e57"));

    QCOMPARE(request.channelId(), quint8(200));
    QCOMPARE(request.controlEndpoint().isValid(), true);
    QCOMPARE(request.controlEndpoint().bytes(), QKnxByteArray::fromHex("08017f0000010e57"));
}

void tst_QKnxNetIpDisconnectRequest::testDebugStream()
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

    qDebug() << QKnxNetIpDisconnectRequest::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0610020900080000"));

    qDebug() << QKnxNetIpDisconnectRequest::builder()
        .setChannelId(200)
        .setControlEndpoint({ QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671 })
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061002090010c80008017f0000010e57"));
}


QTEST_APPLESS_MAIN(tst_QKnxNetIpDisconnectRequest)

#include "tst_qknxnetipdisconnectrequest.moc"
