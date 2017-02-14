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
        QKnxNetIpHPAI hpai(QKnxNetIpStructure::HostProtocolCode::IpV4_Udp, QHostAddress::LocalHost,
            3671);

        QCOMPARE(hpai.hostProtocolCode(), QKnxNetIpStructure::HostProtocolCode::IpV4_Udp);
        QCOMPARE(hpai.address(), QHostAddress(QHostAddress::LocalHost));
        QCOMPARE(hpai.port(), quint16(3671));

        QCOMPARE(hpai.dataSize(), 6);
        QCOMPARE(hpai.data<QByteArray>(), QByteArray::fromHex("7f0000010e57"));

        QCOMPARE(hpai.rawSize(), 8);
        QCOMPARE(hpai.rawData<QByteArray>(), QByteArray::fromHex("08017f0000010e57"));

        QCOMPARE(hpai.toString(), QString::fromLatin1("Raw size { 0x08 }, Code { 0x01 }, "
            "Data { 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }"));
    }

    void testDebugStream()
    {
        // TODO: Implement.
    }

    void testDataStream()
    {
        // TODO: Implement.
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpHPAI)

#include "tst_qknxnetiphpai.moc"
