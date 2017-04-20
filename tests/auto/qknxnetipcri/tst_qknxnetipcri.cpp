/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipcri.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpCRI: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor()
    {
        // TODO: Implement.
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
    QCOMPARE(quint8(cri.connectionTypeCode()), quint8(0));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCRI)

#include "tst_qknxnetipcri.moc"
