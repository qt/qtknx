/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
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
    void testDataStream();
};

void tst_QKnxNetIpConnectResponse::testDefaultConstructor()
{
    QKnxNetIpConnectResponse connectResponse;
    QCOMPARE(connectResponse.isValid(), false);
    QCOMPARE(connectResponse.channelId(), quint8(0));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::None);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), false);
    QCOMPARE(connectResponse.responseData().isValid(), false);
}

void tst_QKnxNetIpConnectResponse::testConstructorOneArgument()
{
    QKnxNetIpConnectResponse connectResponse(QKnxNetIp::Error::NoMoreConnections);
    QCOMPARE(connectResponse.isValid(), true);
    QCOMPARE(connectResponse.size(), quint16(8));
    QCOMPARE(connectResponse.bytes<QByteArray>(),
        QByteArray::fromHex("0610020600080024"));
    QCOMPARE(connectResponse.payload().size(), quint16(2));
    QCOMPARE(connectResponse.payload().bytes<QByteArray>(),
        QByteArray::fromHex("0024"));
    QCOMPARE(connectResponse.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x206 }, Total size { 0x08 }, "
            "Bytes { 0x00, 0x24 }"));

    QCOMPARE(connectResponse.channelId(), quint8(0));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::NoMoreConnections);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), false);
    QCOMPARE(connectResponse.responseData().isValid(), false);
}

void tst_QKnxNetIpConnectResponse::testConstructorOneArgumentNoError()
{
    QKnxNetIpConnectResponse connectResponse(QKnxNetIp::Error::None);
    QCOMPARE(connectResponse.isValid(), false);
    QCOMPARE(connectResponse.size(), quint16(8));
    QCOMPARE(connectResponse.bytes<QByteArray>(),
        QByteArray::fromHex("0610020600080000"));
    QCOMPARE(connectResponse.payload().size(), quint16(2));
    QCOMPARE(connectResponse.payload().bytes<QByteArray>(),
        QByteArray::fromHex("0000"));
    QCOMPARE(connectResponse.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x206 }, Total size { 0x08 }, "
            "Bytes { 0x00, 0x00 }"));

    QCOMPARE(connectResponse.channelId(), quint8(0));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::None);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), false);
    QCOMPARE(connectResponse.responseData().isValid(), false);
}

void tst_QKnxNetIpConnectResponse::testConstructorFourArguments()
{
    QKnxNetIpHpai dataEnd(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpCrd responseData;
    QKnxNetIpConnectResponse connectResponse(quint8(200), QKnxNetIp::Error::NoMoreConnections,
        dataEnd, responseData);
    QCOMPARE(connectResponse.isValid(), true);
    QCOMPARE(connectResponse.size(), quint16(16));
    QCOMPARE(connectResponse.bytes<QByteArray>(),
        QByteArray::fromHex("061002060010c82408017f0000010e57"));
    QCOMPARE(connectResponse.payload().size(), quint16(10));
    QCOMPARE(connectResponse.payload().bytes<QByteArray>(),
        QByteArray::fromHex("c82408017f0000010e57"));
    QCOMPARE(connectResponse.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x206 }, Total size { 0x10 }, "
            "Bytes { 0xc8, 0x24, 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));

    QCOMPARE(connectResponse.channelId(), quint8(200));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::NoMoreConnections);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), false);
    QCOMPARE(connectResponse.responseData().isValid(), false);
}

void tst_QKnxNetIpConnectResponse::testConstructorFourArgumentsNoError()
{
    QKnxNetIpConnectResponse connectResponse(200u, QKnxNetIp::Error::None,
        QKnxNetIpHpai { QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671 },
        QKnxNetIpCrd { QKnxAddress { QKnxAddress::Type::Individual, QStringLiteral("1.1.10") } });

    QCOMPARE(connectResponse.isValid(), true);
    QCOMPARE(connectResponse.size(), quint16(20));
    QCOMPARE(connectResponse.bytes<QByteArray>(),
        QByteArray::fromHex("061002060014c80008017f0000010e570404110a"));
    QCOMPARE(connectResponse.payload().size(), quint16(14));
    QCOMPARE(connectResponse.payload().bytes<QByteArray>(),
        QByteArray::fromHex("c80008017f0000010e570404110a"));
    QCOMPARE(connectResponse.toString(), QString::fromLatin1("Header size { 0x06 }, "
        "Version { 0x10 }, Code { 0x206 }, Total size { 0x14 }, "
        "Bytes { 0xc8, 0x00, 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57, 0x04, 0x04, 0x11, 0x0a }"));

    QCOMPARE(connectResponse.channelId(), quint8(200));
    QCOMPARE(connectResponse.status(), QKnxNetIp::Error::None);
    QCOMPARE(connectResponse.dataEndpoint().isValid(), true);
    QCOMPARE(connectResponse.dataEndpoint().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectResponse.responseData().isValid(), true);
    QCOMPARE(connectResponse.responseData().individualAddress().toString(), QStringLiteral("1.1.10"));
}

void tst_QKnxNetIpConnectResponse::testFromBytes()
{
    auto response = QKnxNetIpConnectResponse::fromBytes(
        QByteArray::fromHex("06100206001415000801c0a8c814c3b404040200"), 0);

    auto header = response.header();
    QCOMPARE(header.size(), quint16(QKnxNetIpFrameHeader::HeaderSize10));
    QCOMPARE(header.byte(1), QKnxNetIpFrameHeader::KnxNetIpVersion10);
    QCOMPARE(header.code(), QKnxNetIp::ServiceType::ConnectResponse);
    QCOMPARE(header.totalSize(), quint16(20));

    QCOMPARE(response.channelId(), quint8(21));
    QCOMPARE(response.status(), QKnxNetIp::Error::None);

    auto hpai = response.dataEndpoint();
    QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Udp);
    QCOMPARE(hpai.address(), QHostAddress("192.168.200.20"));
    QCOMPARE(hpai.port(), quint16(50100));

    auto crd = response.responseData().bytes();
    QCOMPARE(crd[0], quint8(0x04));
    QCOMPARE(crd[1], quint8(0x04));
    QCOMPARE(crd[2], quint8(0x02));
    QCOMPARE(crd[3], quint8(0x00));

    response = QKnxNetIpConnectResponse::fromBytes(
        QByteArray::fromHex("061002060014010008010A094E1F0E5704041105"), 0);

    header = response.header();
    QCOMPARE(header.size(), quint16(QKnxNetIpFrameHeader::HeaderSize10));
    QCOMPARE(header.byte(1), QKnxNetIpFrameHeader::KnxNetIpVersion10);
    QCOMPARE(header.code(), QKnxNetIp::ServiceType::ConnectResponse);
    QCOMPARE(header.totalSize(), quint16(20));

    QCOMPARE(response.channelId(), quint8(1));
    QCOMPARE(response.status(), QKnxNetIp::Error::None);

    hpai = response.dataEndpoint();
    QCOMPARE(hpai.hostProtocol(), QKnxNetIp::HostProtocol::IpV4_Udp);
    QCOMPARE(hpai.address(), QHostAddress("10.9.78.31"));
    QCOMPARE(hpai.port(), quint16(3671));

    crd = response.responseData().bytes();
    QCOMPARE(crd[0], quint8(0x04));
    QCOMPARE(crd[1], quint8(0x04));
    QCOMPARE(crd[2], quint8(0x11));
    QCOMPARE(crd[3], quint8(0x05));
}

void tst_QKnxNetIpConnectResponse::testDebugStream()
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

    qDebug() << QKnxNetIpConnectResponse();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpConnectResponse(QKnxNetIp::Error::NoMoreConnections);
    QCOMPARE(s_msg, QString::fromLatin1("0x0610020600080024"));

    QKnxNetIpHpai dataEnd(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpCrd responseData;
    qDebug() <<QKnxNetIpConnectResponse(quint8(200), QKnxNetIp::Error::None, dataEnd, responseData);
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    responseData.setConnectionType(QKnxNetIp::ConnectionType::Tunnel);
    responseData.setIndividualAddress({ QKnxAddress::Type::Individual, QStringLiteral("1.1.10") });

    qDebug() <<QKnxNetIpConnectResponse(quint8(200), QKnxNetIp::Error::None, dataEnd, responseData);
    QCOMPARE(s_msg, QString::fromLatin1("0x061002060014c80008017f0000010e570404110a"));
}

void tst_QKnxNetIpConnectResponse::testDataStream()
{
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);

        out << QKnxNetIpConnectResponse(QKnxNetIp::Error::NoMoreConnections);
        QCOMPARE(byteArray, QByteArray::fromHex("0610020600080024"));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);

        out << QKnxNetIpConnectResponse(21u, QKnxNetIp::Error::None,
            QKnxNetIpHpai { QHostAddress("192.168.200.20"), 50100 },
            QKnxNetIpCrd { { QKnxAddress::Type::Individual, QStringLiteral("1.1.10") } });
        QCOMPARE(byteArray, QByteArray::fromHex("06100206001415000801c0a8c814c3b40404110a"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectResponse)

#include "tst_qknxnetipconnectresponse.moc"
