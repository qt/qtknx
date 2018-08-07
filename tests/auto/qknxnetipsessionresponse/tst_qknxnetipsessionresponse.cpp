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
#include <QtKnx/qknxnetipsessionresponse.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpSessionResponse: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
};

void tst_QKnxNetIpSessionResponse::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpSessionResponseProxy descriptionResponse(frame);

    QCOMPARE(descriptionResponse.isValid(), false);
    QCOMPARE(descriptionResponse.secureSessionId(), 0x0000);
    QCOMPARE(descriptionResponse.publicKey(), QKnxByteArray());
    QCOMPARE(descriptionResponse.messageAuthenticationCode(), QKnxByteArray());

    frame = QKnxNetIpSessionResponseProxy::builder().create();
    QCOMPARE(descriptionResponse.secureSessionId(), 0x0000);
    QCOMPARE(descriptionResponse.publicKey(), QKnxByteArray());
    QCOMPARE(descriptionResponse.messageAuthenticationCode(), QKnxByteArray());
}

void tst_QKnxNetIpSessionResponse::testConstructor()
{
    auto builder = QKnxNetIpSessionResponseProxy::builder();

    auto frame = builder.create();
    QKnxNetIpSessionResponseProxy descriptionResponse(frame);

    QCOMPARE(descriptionResponse.isValid(), false);
    QCOMPARE(descriptionResponse.secureSessionId(), 0x0000);
    QCOMPARE(descriptionResponse.publicKey(), QKnxByteArray());
    QCOMPARE(descriptionResponse.messageAuthenticationCode(), QKnxByteArray());

    frame = builder.setSecureSessionId(0x0001)
        .create();

    QCOMPARE(descriptionResponse.isValid(), false);
    QCOMPARE(descriptionResponse.secureSessionId(), 0x0000);
    QCOMPARE(descriptionResponse.publicKey(), QKnxByteArray());
    QCOMPARE(descriptionResponse.messageAuthenticationCode(), QKnxByteArray());

    frame = builder.setPublicKey(QKnxByteArray::fromHex("bdf099909923143ef0a5de0b3be3687b"
        "c5bd3cf5f9e6f901699cd870ec1ff824"))
        .create();
    QCOMPARE(descriptionResponse.isValid(), false);
    QCOMPARE(descriptionResponse.secureSessionId(), 0x0000);
    QCOMPARE(descriptionResponse.publicKey(), QKnxByteArray());
    QCOMPARE(descriptionResponse.messageAuthenticationCode(), QKnxByteArray());

    frame = builder.setMessageAuthenticationCode(QKnxByteArray::fromHex("a922505aaa436163"
        "570bd5494c2df2a3"))
        .create();
    QCOMPARE(descriptionResponse.isValid(), true);
    QCOMPARE(descriptionResponse.secureSessionId(), 0x0001);
    QCOMPARE(descriptionResponse.publicKey(),
        QKnxByteArray::fromHex("bdf099909923143ef0a5de0b3be3687bc5bd3cf5f9e6f901699cd870ec1ff824"));
    QCOMPARE(descriptionResponse.messageAuthenticationCode(),
        QKnxByteArray::fromHex("a922505aaa436163570bd5494c2df2a3"));

    QCOMPARE(frame.size(), quint16(56));
    QCOMPARE(frame.serviceType(), QKnxNetIp::ServiceType::SessionResponse);
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("0610095200380001bdf099909923143ef0a5de0b3be3687b"
        "c5bd3cf5f9e6f901699cd870ec1ff824a922505aaa436163570bd5494c2df2a3"));
    QCOMPARE(frame.data().size(), quint16(50));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("0001bdf099909923143ef0a5de0b3be3687b"
        "c5bd3cf5f9e6f901699cd870ec1ff824a922505aaa436163570bd5494c2df2a3"));
}

void tst_QKnxNetIpSessionResponse::testDebugStream()
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

    qDebug() << QKnxNetIpSessionResponseProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061009520006"));

    qDebug() << QKnxNetIpSessionResponseProxy::builder()
        .setSecureSessionId(0x0001)
        .setPublicKey(QKnxByteArray::fromHex("0aa227b4fd7a32319ba9960ac036ce0e"
            "5c4507b5ae55161f1078b1dcfb3cb631"))
        .setMessageAuthenticationCode(QKnxByteArray::fromHex("a922505aaa436163"
        "570bd5494c2df2a3"))
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0610095200380001"
        "0aa227b4fd7a32319ba9960ac036ce0e5c4507b5ae55161f1078b1dcfb3cb631a922505aaa436163"
        "570bd5494c2df2a3"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpSessionResponse)

#include "tst_qknxnetipsessionresponse.moc"
