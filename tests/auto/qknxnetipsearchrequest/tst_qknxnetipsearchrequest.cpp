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
#include <QtKnx/qknxnetipsearchrequest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpSearchRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpSearchRequest::testDefaultConstructor()
{
    QKnxNetIpSearchRequest search;
    QCOMPARE(search.isValid(), false);
    QCOMPARE(search.discoveryEndpoint().isValid(), false);
}

void tst_QKnxNetIpSearchRequest::testConstructor()
{
    QKnxNetIpHpai endpoint(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpSearchRequest search(endpoint);
    QCOMPARE(search.isValid(), true);
    QCOMPARE(search.size(), quint16(14));
    QCOMPARE(search.bytes<QByteArray>(),
        QByteArray::fromHex("06100201000e08017f0000010e57"));
    QCOMPARE(search.payload().size(), quint16(8));
    QCOMPARE(search.payload().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(search.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x201 }, Total size { 0x0e }, "
            "Bytes { 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));

    QCOMPARE(search.discoveryEndpoint().isValid(), true);
    QCOMPARE(search.discoveryEndpoint().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
}

void tst_QKnxNetIpSearchRequest::testDebugStream()
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

    qDebug() << QKnxNetIpSearchRequest();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    QKnxNetIpHpai endpoint(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    qDebug() << QKnxNetIpSearchRequest(endpoint);
    QCOMPARE(s_msg, QString::fromLatin1("0x06100201000e08017f0000010e57"));
}

void tst_QKnxNetIpSearchRequest::testDataStream()
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    QKnxNetIpHpai endpoint(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    out << QKnxNetIpSearchRequest(endpoint);
    QCOMPARE(byteArray, QByteArray::fromHex("06100201000e08017f0000010e57"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpSearchRequest)

#include "tst_qknxnetipsearchrequest.moc"
