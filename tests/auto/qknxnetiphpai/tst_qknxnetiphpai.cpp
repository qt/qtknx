/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpHPAI : public QObject
{
    Q_OBJECT

private slots:
    void testConstructor()
    {
        QKnxNetIpHPAI hpai(QKnxNetIp::HostProtocolCode::IpV4_Udp, QHostAddress::LocalHost,
            3671);

        QCOMPARE(hpai.hostProtocolCode(), QKnxNetIp::HostProtocolCode::IpV4_Udp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::LocalHost));
        QCOMPARE(hpai.port(), quint16(3671));

        QCOMPARE(hpai.payload().size(), quint16(6));
        QCOMPARE(hpai.payload().bytes<QByteArray>(), QByteArray::fromHex("7f0000010e57"));

        QCOMPARE(hpai.size(), quint16(8));
        QCOMPARE(hpai.bytes<QByteArray>(), QByteArray::fromHex("08017f0000010e57"));

        QCOMPARE(hpai.toString(), QString::fromLatin1("Total size { 0x08 }, Code { 0x01 }, "
            "Bytes { 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));
    }

    void testDebugStream()
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

        qDebug() << QKnxNetIpHPAI();
        QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

        qDebug() << QKnxNetIpHPAI(QKnxNetIp::HostProtocolCode::IpV4_Udp,
                                  QHostAddress::LocalHost, 3671);
        QCOMPARE(s_msg, QString::fromLatin1("0x08017f0000010e57"));
    }

    void testDataStream()
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpHPAI(QKnxNetIp::HostProtocolCode::IpV4_Udp,
                             QHostAddress::LocalHost, 3671);
        QCOMPARE(byteArray, QByteArray::fromHex("08017f0000010e57"));
    }
};
QTEST_APPLESS_MAIN(tst_QKnxNetIpHPAI)

#include "tst_qknxnetiphpai.moc"
