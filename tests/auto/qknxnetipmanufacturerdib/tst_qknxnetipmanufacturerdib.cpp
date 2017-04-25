/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipmanufacturerdib.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpManufacturerDIB : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithOneArguments();
    void testConstructorWithByteArrayDataArguments();
    void testConstructorWithVectorDataArguments();
    void testDebugStream()
    {
        // TODO: Implement.
    }
    void testDataStream()
    {
        // TODO: Implement.
    }
};

void tst_QKnxNetIpManufacturerDIB::testDefaultConstructor()
{
    QKnxNetIpManufacturerDIB manufacturerDIB;
    QCOMPARE(manufacturerDIB.isValid(), false);
    QCOMPARE(manufacturerDIB.size(), quint16(0));
    QCOMPARE(manufacturerDIB.bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(manufacturerDIB.payload().size(), quint16(0));
    QCOMPARE(manufacturerDIB.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(manufacturerDIB.toString(), QString::fromLatin1("Total size { 0x00 }, Code { 0x00 }, "
        "Bytes {  }"));
    QCOMPARE(quint8(manufacturerDIB.descriptionTypeCode()), quint8(0));
    QCOMPARE(manufacturerDIB.manufacturerId(), quint16(0));
}

void tst_QKnxNetIpManufacturerDIB::testConstructorWithOneArguments()
{
    QKnxNetIpManufacturerDIB manufacturerDIB(65535);
    QCOMPARE(manufacturerDIB.isValid(), true);
    QCOMPARE(manufacturerDIB.size(), quint16(4));
    QCOMPARE(manufacturerDIB.bytes<QByteArray>(), QByteArray::fromHex("04FEFFFF"));
    QCOMPARE(manufacturerDIB.payload().size(), quint16(2));
    QCOMPARE(manufacturerDIB.payload().bytes<QByteArray>(), QByteArray::fromHex("FFFF"));
    QCOMPARE(manufacturerDIB.toString(), QString::fromLatin1("Total size { 0x04 }, Code { 0xfe }, "
        "Bytes { 0xff, 0xff }"));
    QCOMPARE(manufacturerDIB.descriptionTypeCode(), QKnxNetIp::DescriptionTypeCode::ManufactorData);
    QCOMPARE(manufacturerDIB.manufacturerId(), quint16(65535));
}

void tst_QKnxNetIpManufacturerDIB::testConstructorWithByteArrayDataArguments()
{
    const QByteArray data = QByteArray::fromHex("0102030405");
    QKnxNetIpManufacturerDIB manufacturerDIB(65535, data);
    QCOMPARE(manufacturerDIB.isValid(), true);
    QCOMPARE(manufacturerDIB.size(), quint16(9));
    QCOMPARE(manufacturerDIB.bytes<QByteArray>(), QByteArray::fromHex("09FEFFFF0102030405"));
    QCOMPARE(manufacturerDIB.payload().size(), quint16(7));
    QCOMPARE(manufacturerDIB.payload().bytes<QByteArray>(), QByteArray::fromHex("FFFF0102030405"));
    QCOMPARE(manufacturerDIB.toString(), QString::fromLatin1("Total size { 0x09 }, Code { 0xfe }, "
        "Bytes { 0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05 }"));
    QCOMPARE(manufacturerDIB.descriptionTypeCode(), QKnxNetIp::DescriptionTypeCode::ManufactorData);
    QCOMPARE(manufacturerDIB.manufacturerId(), quint16(65535));
    QCOMPARE(manufacturerDIB.manufacturerData<QByteArray>().size(), data.size());
    QCOMPARE(manufacturerDIB.manufacturerData<QVector<quint8>>().size(), data.size());
    QCOMPARE(manufacturerDIB.manufacturerData<QByteArray>(), data);
}

void tst_QKnxNetIpManufacturerDIB::testConstructorWithVectorDataArguments()
{
    const QVector<quint8> data = { 1, 2, 3, 4, 5 };
    QKnxNetIpManufacturerDIB manufacturerDIB(65535, data);
    QCOMPARE(manufacturerDIB.isValid(), true);
    QCOMPARE(manufacturerDIB.size(), quint16(9));
    QCOMPARE(manufacturerDIB.bytes<QByteArray>(), QByteArray::fromHex("09FEFFFF0102030405"));
    QCOMPARE(manufacturerDIB.payload().size(), quint16(7));
    QCOMPARE(manufacturerDIB.payload().bytes<QByteArray>(), QByteArray::fromHex("FFFF0102030405"));
    QCOMPARE(manufacturerDIB.toString(), QString::fromLatin1("Total size { 0x09 }, Code { 0xfe }, "
        "Bytes { 0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05 }"));
    QCOMPARE(manufacturerDIB.descriptionTypeCode(), QKnxNetIp::DescriptionTypeCode::ManufactorData);
    QCOMPARE(manufacturerDIB.manufacturerId(), quint16(65535));
    QCOMPARE(manufacturerDIB.manufacturerData<QByteArray>().size(), data.size());
    QCOMPARE(manufacturerDIB.manufacturerData< QVector<quint8> >().size(), data.size());
    QCOMPARE(manufacturerDIB.manufacturerData< QVector<quint8> >(), data);
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpManufacturerDIB)

#include "tst_qknxnetipmanufacturerdib.moc"
