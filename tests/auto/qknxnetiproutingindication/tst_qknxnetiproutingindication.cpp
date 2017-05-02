/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtTest/qtest.h>
#include <QtKnx/qknxnetiproutingindication.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpRoutingIndication: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor(); // to do
    void testDebugStream(); // to do
    void testDataStream(); // to do
};

void tst_QKnxNetIpRoutingIndication::testDefaultConstructor()
{
    QKnxNetIpRoutingIndication routing;
    QCOMPARE(routing.isValid(), false);
}

void tst_QKnxNetIpRoutingIndication::testConstructor()
{
}

void tst_QKnxNetIpRoutingIndication::testDebugStream()
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

    //qDebug() << DEFAULTCONSTRUCTOR;
    //QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));
}

void tst_QKnxNetIpRoutingIndication::testDataStream()
{
    //QByteArray byteArray;
    //QDataStream out(&byteArray, QIODevice::WriteOnly);
    //out << OTHERCONSTRUCTOR;
    //QCOMPARE(byteArray, QByteArray::fromHex("1404C0A8020CFFFFFF00C0A80201B48A03020100"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpRoutingIndication)

#include "tst_qknxnetiproutingindication.moc"
