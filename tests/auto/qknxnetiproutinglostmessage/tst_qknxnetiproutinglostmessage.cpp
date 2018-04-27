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
#include <QtKnx/qknxnetiproutinglostmessage.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpRoutingLostMessage: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
};

void tst_QKnxNetIpRoutingLostMessage::testDefaultConstructor()
{
    QKnxNetIpFrame frame;

    QKnxNetIpRoutingLostMessageProxy routing(frame);
    QCOMPARE(routing.isValid(), false);

    frame = QKnxNetIpRoutingLostMessageProxy::builder().create();
    QCOMPARE(routing.isValid(), true);
}

void tst_QKnxNetIpRoutingLostMessage::testConstructor()
{
    auto frame = QKnxNetIpRoutingLostMessageProxy::builder()
        .setDeviceState(QKnxNetIp::DeviceState::IpFault)
        .setLostMessageCount(0xffff)
        .create();
    QKnxNetIpRoutingLostMessageProxy routing(frame);

    QCOMPARE(routing.isValid(), true);
    QCOMPARE(frame.size(), quint16(10));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("06100531000a0401ffff"));
    QCOMPARE(frame.data().size(), quint16(4));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("0401ffff"));

    QCOMPARE(routing.deviceState(), QKnxNetIp::DeviceState::IpFault);
    QCOMPARE(routing.lostMessageCount(), quint16(0xffff));
}

void tst_QKnxNetIpRoutingLostMessage::testDebugStream()
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

    qDebug() << QKnxNetIpRoutingLostMessageProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100531000a04000000"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpRoutingLostMessage)

#include "tst_qknxnetiproutinglostmessage.moc"
