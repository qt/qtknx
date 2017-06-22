/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtTest/qtest.h>
#include <QtKnx/qknxnetipdeviceconfigurationrequest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDeviceConfigurationRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpDeviceConfigurationRequest::testDefaultConstructor()
{
    QKnxNetIpDeviceConfigurationRequest configuration;
    QCOMPARE(configuration.isValid(), false);
}

void tst_QKnxNetIpDeviceConfigurationRequest::testConstructor()
{
    auto ba = QByteArray::fromHex("fc000001531001");
    QKnxNetIpDeviceConfigurationRequest request(1, 1, QKnxCemiFrame::fromBytes(ba, 0, ba.size()));
    QCOMPARE(request.isValid(), true);
    QCOMPARE(request.size(), quint16(17));

    QCOMPARE(request.channelId(), quint8(1));
    QCOMPARE(request.sequenceCounter(), quint8(1));
    QCOMPARE(request.cemi().bytes<QByteArray>(), ba);
}

void tst_QKnxNetIpDeviceConfigurationRequest::testDebugStream()
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

    qDebug() << QKnxNetIpDeviceConfigurationRequest();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    auto ba = QByteArray::fromHex("06100310001104020000fc000001531001");
    qDebug() << QKnxNetIpDeviceConfigurationRequest::fromBytes(ba, 0);
    QCOMPARE(s_msg, QString::fromLatin1("0x06100310001104020000fc000001531001"));
}

void tst_QKnxNetIpDeviceConfigurationRequest::testDataStream()
{
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpDeviceConfigurationRequest();
        QCOMPARE(byteArray, QByteArray::fromHex(""));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);

        auto ba = QByteArray::fromHex("06100310001104020000fc000001531001");
        out << QKnxNetIpDeviceConfigurationRequest::fromBytes(ba, 0);
        QCOMPARE(byteArray, ba);
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpDeviceConfigurationRequest)

#include "tst_qknxnetipdeviceconfigurationrequest.moc"
