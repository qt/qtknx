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
#include <QtKnx/qknxnetipconnectionstateresponse.h>
#include <QtKnx/QKnxNetIpHpaiProxy>
#include <QtKnx/qknxnetip.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpConnectionStateResponse: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testProxyMethods();
    void testDebugStream();
};

void tst_QKnxNetIpConnectionStateResponse::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpConnectionStateResponseProxy connectionStateResponse(frame);

    QCOMPARE(connectionStateResponse.isValid(), false);
    QCOMPARE(connectionStateResponse.channelId(), quint8(0));
    QCOMPARE(connectionStateResponse.status(), QKnxNetIp::Error::None);

    frame = QKnxNetIpConnectionStateResponseProxy::builder().create();
    const QKnxNetIpConnectionStateResponseProxy response(frame);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QEXPECT_FAIL("", "TODO: Fix this for the Qt6 release!", Continue);
    QCOMPARE(response.isValid(), false);
#else
    // TODO: isValid is not consistent with connection state request and
    // connection state response when creating a frame from an empty builder...
    QCOMPARE(response.isValid(), true);
#endif
}

void tst_QKnxNetIpConnectionStateResponse::testConstructor()
{
    quint8 channelId = 200;
    auto frame = QKnxNetIpConnectionStateResponseProxy::builder()
        .setChannelId(channelId)
        .setStatus(QKnxNetIp::Error::ConnectionId)
        .create();
    QKnxNetIpConnectionStateResponseProxy connectionStateResponse(frame);

    QCOMPARE(connectionStateResponse.isValid(), true);
    QCOMPARE(frame.size(), quint16(8));
    QCOMPARE(frame.bytes(), QKnxByteArray({ 0x06, 0x10, 0x02, 0x08, 0x00, 0x08, 0xc8, 0x21 }));
    QCOMPARE(frame.data().size(), quint16(2));
    QCOMPARE(frame.data(), QKnxByteArray({ 0xc8, 0x21 }));
    QCOMPARE(connectionStateResponse.channelId(), channelId);
    QCOMPARE(connectionStateResponse.status(), QKnxNetIp::Error::ConnectionId);
}

void tst_QKnxNetIpConnectionStateResponse::testProxyMethods()
{
    quint8 channelId = 200;
    auto status = QKnxNetIp::Error::ConnectionId;
    {
        // Check channelId(), status() and isValid() with Wrong service type
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::ConnectionStateRequest,
                                 QKnxByteArray { channelId, quint8(status) } };
        const QKnxNetIpConnectionStateResponseProxy view(frame);
        QCOMPARE(view.channelId(), channelId);
        QCOMPARE(view.status(), status);
        QCOMPARE(view.isValid(), false);
    }
    {
        // Check channelId(), status() and isValid() on a valid connection state response
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::ConnectionStateResponse,
                                 QKnxByteArray { channelId, quint8(status) } };
        const QKnxNetIpConnectionStateResponseProxy view(frame);
        QCOMPARE(view.channelId(), channelId);
        QCOMPARE(view.status(), status);
        QCOMPARE(view.isValid(), true);
    }
    {
        // Check channelId(), status() and isValid() on a ivalid connection state response
        // with no channelId and status.
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::ConnectionStateResponse };
        const QKnxNetIpConnectionStateResponseProxy view(frame);
        QCOMPARE(frame.header().size(), 6);
        QCOMPARE(frame.dataSize(), 0);
        QCOMPARE(view.isValid(), false);
    }
}

void tst_QKnxNetIpConnectionStateResponse::testDebugStream()
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

    qDebug() << QKnxNetIpConnectionStateResponseProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0610020800080000"));

    qDebug() << QKnxNetIpConnectionStateResponseProxy::builder()
        .setChannelId(200)
        .setStatus(QKnxNetIp::Error::ConnectionId)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061002080008c821"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectionStateResponse)

#include "tst_qknxnetipconnectionstateresponse.moc"
