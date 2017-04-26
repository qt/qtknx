/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipcri.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpCRI : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor()
    {
        // TODO: Implement.
    }
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpCRI::testDefaultConstructor()
{
    QKnxNetIpCRI cri;
    QCOMPARE(cri.isValid(), false);
    QCOMPARE(cri.size(), quint16(0));
    QCOMPARE(cri.bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(cri.payload().size(), quint16(0));
    QCOMPARE(cri.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x00 }, Code { 0x00 }, "
        "Bytes {  }"));
    QCOMPARE(quint8(cri.connectionType()), quint8(0));
}

void tst_QKnxNetIpCRI::testDebugStream()
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

    qDebug() << QKnxNetIpCRI();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));
}

void tst_QKnxNetIpCRI::testDataStream()
{
    // To do for constructor that are not default.
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCRI)

#include "tst_qknxnetipcri.moc"
