/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipcrd.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpCRD : public QObject
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

void tst_QKnxNetIpCRD::testDefaultConstructor()
{
    QKnxNetIpCRD crd;
    QCOMPARE(crd.isValid(), false);
    QCOMPARE(crd.size(), quint16(0));
    QCOMPARE(crd.bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(crd.payload().size(), quint16(0));
    QCOMPARE(crd.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x00 }, Code { 0x00 }, "
        "Bytes {  }"));
    QCOMPARE(quint8(crd.connectionType()), quint8(0));
}

void tst_QKnxNetIpCRD::testDebugStream()
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

    qDebug() << QKnxNetIpCRD();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));
}

void tst_QKnxNetIpCRD::testDataStream()
{
    // To do for constructor that are not default.
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCRD)

#include "tst_qknxnetipcrd.moc"
