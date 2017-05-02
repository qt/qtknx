/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtTest/qtest.h>
#include <QtKnx/qknxnetipdescriptionrequest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDescriptionRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpDescriptionRequest::testDefaultConstructor()
{
    QKnxNetIpDescriptionRequest descriptionRequest;
    QCOMPARE(descriptionRequest.isValid(), false);
    QCOMPARE(descriptionRequest.controlEndpoint().isValid(), false);
}

void tst_QKnxNetIpDescriptionRequest::testConstructor()
{
    QKnxNetIpHPAI endpoint(QKnxNetIpHPAI::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpDescriptionRequest descriptionRequest(endpoint);
    QCOMPARE(descriptionRequest.isValid(), true);
    QCOMPARE(descriptionRequest.size(), quint16(14));
    QCOMPARE(descriptionRequest.bytes<QByteArray>(),
        QByteArray::fromHex("06100203000e08017f0000010e57"));
    QCOMPARE(descriptionRequest.payload().size(), quint16(8));
    QCOMPARE(descriptionRequest.payload().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(descriptionRequest.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Code { 0x203 }, Total size { 0x0e }, "
            "Bytes { 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));

    QCOMPARE(descriptionRequest.controlEndpoint().isValid(), true);
    QCOMPARE(descriptionRequest.controlEndpoint().bytes<QByteArray>(),
        QByteArray::fromHex("08017f0000010e57"));
}

void tst_QKnxNetIpDescriptionRequest::testDebugStream()
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

    qDebug() << QKnxNetIpDescriptionRequest();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    QKnxNetIpHPAI endpoint(QKnxNetIpHPAI::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    qDebug() << QKnxNetIpDescriptionRequest(endpoint);
    QCOMPARE(s_msg, QString::fromLatin1("0x06100203000e08017f0000010e57"));
}

void tst_QKnxNetIpDescriptionRequest::testDataStream()
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    QKnxNetIpHPAI endpoint(QKnxNetIpHPAI::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    out << QKnxNetIpDescriptionRequest(endpoint);
    QCOMPARE(byteArray, QByteArray::fromHex("06100203000e08017f0000010e57"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpDescriptionRequest)

#include "tst_qknxnetipdescriptionrequest.moc"
