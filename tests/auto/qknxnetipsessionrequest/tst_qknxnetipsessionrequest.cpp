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
#include <QtKnx/qknxnetipsessionrequest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpSessionRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
};

void tst_QKnxNetIpSessionRequest::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpSessionRequestProxy descriptionRequest(frame);

    QCOMPARE(descriptionRequest.isValid(), false);
    QCOMPARE(descriptionRequest.controlEndpoint().isValid(), false);
    QCOMPARE(descriptionRequest.publicKey(), QKnxByteArray());

    frame = QKnxNetIpSessionRequestProxy::builder().create();
    QCOMPARE(descriptionRequest.isValid(), false);
    QCOMPARE(descriptionRequest.controlEndpoint().isValid(), false);
    QCOMPARE(descriptionRequest.publicKey(), QKnxByteArray());
}

void tst_QKnxNetIpSessionRequest::testConstructor()
{
    auto builder = QKnxNetIpSessionRequestProxy::builder();

    auto frame = builder.create();
    QKnxNetIpSessionRequestProxy descriptionRequest(frame);

    QCOMPARE(descriptionRequest.isValid(), false);
    QCOMPARE(descriptionRequest.controlEndpoint().isValid(), false);
    QCOMPARE(descriptionRequest.publicKey(), QKnxByteArray());

    frame = builder.setControlEndpoint(QKnxNetIpHpaiProxy::builder()
            .setHostAddress(QHostAddress::LocalHost)
            .setPort(3671).create())
        .create();

    QCOMPARE(descriptionRequest.isValid(), false);
    QCOMPARE(descriptionRequest.controlEndpoint().isValid(), false);
    QCOMPARE(descriptionRequest.publicKey(), QKnxByteArray());

    frame = builder.setPublicKey(QKnxByteArray::fromHex("0aa227b4fd7a32319ba9960ac036ce0e"
            "5c4507b5ae55161f1078b1dcfb3cb631"))
        .create();

    QCOMPARE(frame.size(), quint16(46));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("06100951002e08017f0000010e57")
        + QKnxByteArray::fromHex("0aa227b4fd7a32319ba9960ac036ce0e5c4507b5ae55161f1078b1dcfb3cb631"));
    QCOMPARE(frame.data().size(), quint16(40));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("08017f0000010e57")
        + QKnxByteArray::fromHex("0aa227b4fd7a32319ba9960ac036ce0e5c4507b5ae55161f1078b1dcfb3cb631"));

    QCOMPARE(descriptionRequest.isValid(), true);
    QCOMPARE(descriptionRequest.controlEndpoint().isValid(), true);
    QCOMPARE(descriptionRequest.controlEndpoint().bytes(),
        QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(descriptionRequest.publicKey(),
        QKnxByteArray::fromHex("0aa227b4fd7a32319ba9960ac036ce0e5c4507b5ae55161f1078b1dcfb3cb631"));
}

void tst_QKnxNetIpSessionRequest::testDebugStream()
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

    qDebug() << QKnxNetIpSessionRequestProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061009510006"));

    qDebug() << QKnxNetIpSessionRequestProxy::builder()
        .setControlEndpoint(QKnxNetIpHpaiProxy::builder()
            .setHostAddress(QHostAddress::LocalHost)
            .setPort(3671).create())
        .setPublicKey(QKnxByteArray::fromHex("0aa227b4fd7a32319ba9960ac036ce0e"
            "5c4507b5ae55161f1078b1dcfb3cb631"))
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100951002e08017f0000010e57"
        "0aa227b4fd7a32319ba9960ac036ce0e5c4507b5ae55161f1078b1dcfb3cb631"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpSessionRequest)

#include "tst_qknxnetipsessionrequest.moc"
