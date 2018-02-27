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
#include <QtKnx/qknxnetipdisconnectresponse.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDisconnectResponse: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testConstructorNoError();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpDisconnectResponse::testDefaultConstructor()
{
    QKnxNetIpDisconnectResponse response;
    QCOMPARE(response.isValid(), false);
    QCOMPARE(response.channelId(), quint8(0));
    QCOMPARE(response.status(), QKnxNetIp::Error::None);
}

void tst_QKnxNetIpDisconnectResponse::testConstructor()
{

    QKnxNetIpDisconnectResponse response(quint8(200), QKnxNetIp::Error::NoMoreConnections);
    QCOMPARE(response.isValid(), true);
    QCOMPARE(response.size(), quint16(8));
    QCOMPARE(response.bytes(), QKnxByteArray::fromHex("0610020a0008c824"));
    QCOMPARE(response.payload().size(), quint16(2));
    QCOMPARE(response.payload().bytes(), QKnxByteArray::fromHex("c824"));
    QCOMPARE(response.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x20a }, Total size { 0x08 }, Bytes { 0xc8, 0x24 }"));

    QCOMPARE(response.channelId(), quint8(200));
    QCOMPARE(response.status(), QKnxNetIp::Error::NoMoreConnections);
}

void tst_QKnxNetIpDisconnectResponse::testConstructorNoError()
{

    QKnxNetIpDisconnectResponse response(quint8(200), QKnxNetIp::Error::None);
    QCOMPARE(response.isValid(), true);
    QCOMPARE(response.size(), quint16(8));
    QCOMPARE(response.bytes(), QKnxByteArray::fromHex("0610020a0008c800"));
    QCOMPARE(response.payload().size(), quint16(2));
    QCOMPARE(response.payload().bytes(), QKnxByteArray::fromHex("c800"));
    QCOMPARE(response.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x20a }, Total size { 0x08 }, Bytes { 0xc8, 0x00 }"));

    QCOMPARE(response.channelId(), quint8(200));
    QCOMPARE(response.status(), QKnxNetIp::Error::None);
}

void tst_QKnxNetIpDisconnectResponse::testDebugStream()
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

    qDebug() << QKnxNetIpDisconnectResponse();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpDisconnectResponse(quint8(200), QKnxNetIp::Error::None);
    QCOMPARE(s_msg, QString::fromLatin1("0x0610020a0008c800"));
}

void tst_QKnxNetIpDisconnectResponse::testDataStream()
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    out << QKnxNetIpDisconnectResponse(quint8(200), QKnxNetIp::Error::None);
    QCOMPARE(byteArray, QByteArray::fromHex("0610020a0008c800"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpDisconnectResponse)

#include "tst_qknxnetipdisconnectresponse.moc"
