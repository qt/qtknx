/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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
    void testDataStream();
};

void tst_QKnxNetIpConnectRequest::testDefaultConstructor()
{
    QKnxNetIpConnectRequest connectRequest;
    QCOMPARE(connectRequest.isValid(), false);
    QCOMPARE(connectRequest.controlEndpoint().isValid(), false);
    QCOMPARE(connectRequest.dataEndpoint().isValid(), false);
    QCOMPARE(connectRequest.requestInformation().isValid(), false);
}

void tst_QKnxNetIpConnectRequest::testConstructor()
{
    QKnxNetIpHPAI control(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpHPAI data(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpCRI request;

    QKnxNetIpConnectRequest connectRequest(control, data, request);

    QCOMPARE(connectRequest.isValid(), false);
    QCOMPARE(connectRequest.size(), quint16(22));
    QCOMPARE(connectRequest.bytes<QByteArray>(),
        QByteArray::fromHex("06100205001608017f0000010e5708017f0000010e57"));
    QCOMPARE(connectRequest.payload().size(), quint16(16));
    QCOMPARE(connectRequest.payload().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e5708017f0000010e57"));
    QCOMPARE(connectRequest.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x205 }, Total size { 0x16 }, "
            "Bytes { 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57, 0x08, 0x01, "
            "0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));
    QCOMPARE(connectRequest.controlEndpoint().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.dataEndpoint().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.requestInformation().bytes<QByteArray>(), QByteArray(""));

    request.setConnectionType(QKnxNetIp::ConnectionType::Tunnel);
    request.setTunnelingLayer(QKnxNetIp::TunnelingLayer::Link);
    connectRequest = QKnxNetIpConnectRequest(control, data, request);

    QCOMPARE(connectRequest.isValid(), true);
    QCOMPARE(connectRequest.size(), quint16(26));
    QCOMPARE(connectRequest.bytes<QByteArray>(),
        QByteArray::fromHex("06100205001a08017f0000010e5708017f0000010e5704040200"));
    QCOMPARE(connectRequest.payload().size(), quint16(20));
    QCOMPARE(connectRequest.payload().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e5708017f0000010e5704040200"));
    QCOMPARE(connectRequest.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x205 }, Total size { 0x1a }, "
            "Bytes { 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57, 0x08, 0x01, "
            "0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57, 0x04, 0x04, 0x02, 0x00 }"));

    QCOMPARE(connectRequest.controlEndpoint().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.dataEndpoint().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(connectRequest.requestInformation().bytes<QByteArray>(), QByteArray::fromHex("04040200"));
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

    qDebug() << QKnxNetIpConnectRequest();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpConnectRequest::fromBytes(QByteArray::fromHex("06100205001a0801c0a80001c3"
        "b40801c0a80001c3b404040200"), 0);
    QCOMPARE(s_msg, QString::fromLatin1("0x06100205001a0801c0a80001c3b40801c0a80001c3b404040200"));
}

void tst_QKnxNetIpConnectRequest::testDataStream()
{
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);

        QKnxNetIpHPAI control(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
        QKnxNetIpHPAI data(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
        QKnxNetIpCRI request;
        out << QKnxNetIpConnectRequest(control, data, request);
        QCOMPARE(byteArray, QByteArray(""));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);

        QKnxNetIpHPAI control(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
        QKnxNetIpHPAI data(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
        QKnxNetIpCRI request(QKnxNetIp::TunnelingLayer::Link);
        out << QKnxNetIpConnectRequest(control, data, request);
        QCOMPARE(byteArray, QByteArray::fromHex("06100205001a08017f0000010e5708017f0000010e5704040200"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectRequest)

#include "tst_qknxnetipconnectrequest.moc"
