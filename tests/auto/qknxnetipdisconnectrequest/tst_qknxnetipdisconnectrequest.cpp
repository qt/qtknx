/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtTest/qtest.h>
#include <QtKnx/qknxnetipdisconnectrequest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDisconnectRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpDisconnectRequest::testDefaultConstructor()
{
    QKnxNetIpDisconnectRequest request;
    QCOMPARE(request.isValid(), false);
    QCOMPARE(request.channelId(), quint8(0));
    QCOMPARE(request.controlEndpoint().isValid(), false);
}

void tst_QKnxNetIpDisconnectRequest::testConstructor()
{
    QKnxNetIpHPAI endpoint(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpDisconnectRequest request(quint8(200), endpoint);

    QCOMPARE(request.isValid(), true);
    QCOMPARE(request.size(), quint16(16));
    QCOMPARE(request.bytes<QByteArray>(),
        QByteArray::fromHex("061002090010c80008017f0000010e57"));
    QCOMPARE(request.payload().size(), quint16(10));
    QCOMPARE(request.payload().bytes<QByteArray>(),
        QByteArray::fromHex("c80008017f0000010e57"));
    QCOMPARE(request.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x209 }, Total size { 0x10 }, "
            "Bytes { 0xc8, 0x00, 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));

    QCOMPARE(request.channelId(), quint8(200));
    QCOMPARE(request.controlEndpoint().isValid(), true);
    QCOMPARE(request.controlEndpoint().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
}

void tst_QKnxNetIpDisconnectRequest::testDebugStream()
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

    qDebug() << QKnxNetIpDisconnectRequest();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    QKnxNetIpHPAI endpoint(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    qDebug() << QKnxNetIpDisconnectRequest(quint8(200), endpoint);
    QCOMPARE(s_msg, QString::fromLatin1("0x061002090010c80008017f0000010e57"));
}

void tst_QKnxNetIpDisconnectRequest::testDataStream()
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    QKnxNetIpHPAI endpoint(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    out << QKnxNetIpDisconnectRequest(quint8(200), endpoint);
    QCOMPARE(byteArray, QByteArray::fromHex("061002090010c80008017f0000010e57"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpDisconnectRequest)

#include "tst_qknxnetipdisconnectrequest.moc"
