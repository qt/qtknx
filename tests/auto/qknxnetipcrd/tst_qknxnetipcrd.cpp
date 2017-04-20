/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipcrd.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpCRD : public QObject
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
    QCOMPARE(quint8(crd.connectionTypeCode()), quint8(0));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCRD)

#include "tst_qknxnetipcrd.moc"
