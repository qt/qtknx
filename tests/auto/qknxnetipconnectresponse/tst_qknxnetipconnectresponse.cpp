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
#include <QtKnx/qknxnetipconnectresponse.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpConnectResponse: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorOneArgument();
    void testConstructorOneArgumentNoError();
    void testConstructorFourArguments();
    void testConstructorFourArgumentsNoError();
    void testFromBytes();
    void testDebugStream();
};

void tst_QKnxNetIpConnectResponse::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpConnectResponse connectResponse(frame);

    QCOMPARE(connectResponse.isValid(), false);
    QCOMPARE(connectResponse.channelId(), quint8(0));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::None);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), false);
    QCOMPARE(connectResponse.responseData().isValid(), false);
}

void tst_QKnxNetIpConnectResponse::testConstructorOneArgument()
{
    auto frame = QKnxNetIpConnectResponse::builder()
        .setStatus(QKnxNetIp::Error::NoMoreConnections)
        .create();

    QKnxNetIpConnectResponse connectResponse(frame);
    QCOMPARE(connectResponse.isValid(), true);
    QCOMPARE(frame.size(), quint16(8));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("0610020600080024"));
    QCOMPARE(frame.data().size(), quint16(2));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("0024"));

    QCOMPARE(connectResponse.channelId(), quint8(0));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::NoMoreConnections);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), false);
    QCOMPARE(connectResponse.responseData().isValid(), false);
}

void tst_QKnxNetIpConnectResponse::testConstructorOneArgumentNoError()
{
    auto frame = QKnxNetIpConnectResponse::builder()
        .setStatus(QKnxNetIp::Error::None)
        .create();

    QKnxNetIpConnectResponse connectResponse(frame);
    QCOMPARE(connectResponse.isValid(), false);
    QCOMPARE(frame.size(), quint16(8));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("0610020600080000"));
    QCOMPARE(frame.data().size(), quint16(2));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("0000"));

    QCOMPARE(connectResponse.channelId(), quint8(0));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::None);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), false);
    QCOMPARE(connectResponse.responseData().isValid(), false);
}

void tst_QKnxNetIpConnectResponse::testConstructorFourArguments()
{
    auto frame = QKnxNetIpConnectResponse::builder()
        .setChannelId(200)
        .setStatus(QKnxNetIp::Error::NoMoreConnections)
        .setDataEndpoint(
            {
                QKnxNetIp::HostProtocol::UDP_IPv4,
                QHostAddress::LocalHost,
                3671
            })
        .create();

    QKnxNetIpConnectResponse connectResponse(frame);
    QCOMPARE(connectResponse.isValid(), true);
    QCOMPARE(frame.size(), quint16(8));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("061002060008c824"));
    QCOMPARE(frame.data().size(), quint16(2));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("c824"));

    QCOMPARE(connectResponse.channelId(), quint8(200));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::NoMoreConnections);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), false);
    QCOMPARE(connectResponse.responseData().isValid(), false);
}

void tst_QKnxNetIpConnectResponse::testConstructorFourArgumentsNoError()
{
    auto frame = QKnxNetIpConnectResponse::builder()
        .setChannelId(200)
        .setStatus(QKnxNetIp::Error::None)
        .setDataEndpoint({ QKnxNetIp::HostProtocol::UDP_IPv4, QHostAddress::LocalHost, 3671 })
        .setResponseData(QKnxNetIpCrd({ QKnxAddress::Type::Individual, QStringLiteral("1.1.10") }))
        .create();

    QKnxNetIpConnectResponse connectResponse(frame);
    QCOMPARE(connectResponse.isValid(), true);
    QCOMPARE(frame.size(), quint16(20));
    QCOMPARE(frame.bytes(),
        QKnxByteArray::fromHex("061002060014c80008017f0000010e570404110a"));
    QCOMPARE(frame.data().size(), quint16(14));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("c80008017f0000010e570404110a"));

    QCOMPARE(connectResponse.channelId(), quint8(200));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::None);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), true);
    QCOMPARE(connectResponse.dataEndpoint().bytes(),
        QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectResponse.responseData().isValid(), true);
    QCOMPARE(connectResponse.responseData().individualAddress().toString(), QStringLiteral("1.1.10"));
}

void tst_QKnxNetIpConnectResponse::testFromBytes()
{
    auto frame = QKnxNetIpFrame::fromBytes(
        QKnxByteArray::fromHex("06100206001415000801c0a8c814c3b404040200"), 0);

    auto header = frame.header();
    QCOMPARE(header.size(), quint16(QKnxNetIpFrameHeader::HeaderSize10));
    QCOMPARE(header.byte(1), quint8(QKnxNetIpFrameHeader::KnxNetIpVersion10));
    QCOMPARE(header.serviceType(), QKnxNetIp::ServiceType::ConnectResponse);
    QCOMPARE(header.totalSize(), quint16(20));

    QKnxNetIpConnectResponse response(frame);
    QCOMPARE(response.channelId(), quint8(21));
    QCOMPARE(response.status(), QKnxNetIp::Error::None);

    auto hpai = response.dataEndpoint();
    QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::UDP_IPv4);
    QCOMPARE(hpai.address(), QHostAddress("192.168.200.20"));
    QCOMPARE(hpai.port(), quint16(50100));

    auto crd = response.responseData().bytes();
    QCOMPARE(quint8(crd[0]), quint8(0x04));
    QCOMPARE(quint8(crd[1]), quint8(0x04));
    QCOMPARE(quint8(crd[2]), quint8(0x02));
    QCOMPARE(quint8(crd[3]), quint8(0x00));

    frame = QKnxNetIpFrame::fromBytes(
        QKnxByteArray::fromHex("061002060014010008010A094E1F0E5704041105"), 0);

    header = frame.header();
    QCOMPARE(header.size(), quint16(QKnxNetIpFrameHeader::HeaderSize10));
    QCOMPARE(header.byte(1), quint8(QKnxNetIpFrameHeader::KnxNetIpVersion10));
    QCOMPARE(header.serviceType(), QKnxNetIp::ServiceType::ConnectResponse);
    QCOMPARE(header.totalSize(), quint16(20));

    QCOMPARE(response.channelId(), quint8(1));
    QCOMPARE(response.status(), QKnxNetIp::Error::None);

    hpai = response.dataEndpoint();
    QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::UDP_IPv4);
    QCOMPARE(hpai.address(), QHostAddress("10.9.78.31"));
    QCOMPARE(hpai.port(), quint16(3671));

    crd = response.responseData().bytes();
    QCOMPARE(quint8(crd[0]), quint8(0x04));
    QCOMPARE(quint8(crd[1]), quint8(0x04));
    QCOMPARE(quint8(crd[2]), quint8(0x11));
    QCOMPARE(quint8(crd[3]), quint8(0x05));
}

void tst_QKnxNetIpConnectResponse::testDebugStream()
{
    struct DebugHandler
    {
        explicit DebugHandler(QtMessageHandler newMessageHandler)
            : oldMessageHandler(qInstallMessageHandler(newMessageHandler))
        {}
        ~DebugHandler()
        {
            qInstallMessageHandler(oldMessageHandler);
        }
        QtMessageHandler oldMessageHandler;
    } _(myMessageHandler);

    qDebug() << QKnxNetIpConnectResponse::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0610020600080000"));

    qDebug() << QKnxNetIpConnectResponse::builder().setStatus(QKnxNetIp::Error::NoMoreConnections)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0610020600080024"));

    QKnxNetIpHpai dataEnd(QKnxNetIp::HostProtocol::UDP_IPv4, QHostAddress::LocalHost, 3671);
    QKnxNetIpCrd responseData;
    qDebug() << QKnxNetIpConnectResponse::builder().setChannelId(200)
        .setStatus(QKnxNetIp::Error::None)
        .setDataEndpoint(dataEnd)
        .setResponseData(responseData)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061002060010c80008017f0000010e57"));

    responseData.setConnectionType(QKnxNetIp::ConnectionType::Tunnel);
    responseData.setIndividualAddress({ QKnxAddress::Type::Individual, QStringLiteral("1.1.10") });

    qDebug() << QKnxNetIpConnectResponse::builder().setChannelId(200)
        .setStatus(QKnxNetIp::Error::None)
        .setDataEndpoint(dataEnd)
        .setResponseData(responseData)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061002060014c80008017f0000010e570404110a"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectResponse)

#include "tst_qknxnetipconnectresponse.moc"
