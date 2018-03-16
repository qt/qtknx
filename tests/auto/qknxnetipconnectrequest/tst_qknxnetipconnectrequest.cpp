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
};

void tst_QKnxNetIpConnectRequest::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpConnectRequest connectRequest(frame);
    QCOMPARE(connectRequest.isValid(), false);
    QCOMPARE(connectRequest.controlEndpoint().isValid(), false);
    QCOMPARE(connectRequest.dataEndpoint().isValid(), false);
    QCOMPARE(connectRequest.requestInformation().isValid(), false);
}

void tst_QKnxNetIpConnectRequest::testConstructor()
{
    const auto hpai = QKnxNetIpHpaiView::builder()
            .setHostAddress(QHostAddress::LocalHost)
            .setPort(3671).create();

    auto builder = QKnxNetIpConnectRequest::builder();
    auto frame = builder
        .setControlEndpoint(hpai)
        .setDataEndpoint(hpai)
        .create();

    QKnxNetIpConnectRequest connectRequest(frame);

    QCOMPARE(connectRequest.isValid(), false);
    QCOMPARE(frame.size(), quint16(22));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("06100205001608017f0000010e5708017f0000010e57"));
    QCOMPARE(frame.data().size(), quint16(16));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("08017f0000010e5708017f0000010e57"));
    QCOMPARE(connectRequest.controlEndpoint().bytes(), QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.dataEndpoint().bytes(), QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.requestInformation().bytes(), QKnxByteArray {});

    auto cri = QKnxNetIpCriView::builder()
        .setTunnelingLayer(QKnxNetIp::TunnelingLayer::Link)
        .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
        .create();

    frame = builder.setRequestInformation(cri).create();

    QCOMPARE(connectRequest.isValid(), true);
    QCOMPARE(frame.size(), quint16(26));
    QCOMPARE(frame.bytes(),
        QKnxByteArray::fromHex("06100205001a08017f0000010e5708017f0000010e5704040200"));
    QCOMPARE(frame.data().size(), quint16(20));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("08017f0000010e5708017f0000010e5704040200"));

    QCOMPARE(connectRequest.controlEndpoint().bytes(), QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.dataEndpoint().bytes(), QKnxByteArray::fromHex("08017f0000010e57"));
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

    qDebug() << QKnxNetIpConnectRequest::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061002050006"));

    auto frame = QKnxNetIpFrame::fromBytes(QKnxByteArray::fromHex("06100205001a0801c0a80001c3"
        "b40801c0a80001c3b404040200"), 0);

    QKnxNetIpConnectRequest request(frame);
    qDebug() << QKnxNetIpConnectRequest::builder()
        .setControlEndpoint(request.controlEndpoint())
        .setDataEndpoint(request.dataEndpoint())
        .setRequestInformation(request.requestInformation())
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100205001a0801c0a80001c3b40801c0a80001c3b404040200"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectRequest)

#include "tst_qknxnetipconnectrequest.moc"
