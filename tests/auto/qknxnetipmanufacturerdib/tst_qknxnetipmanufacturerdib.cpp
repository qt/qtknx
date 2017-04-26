/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipmanufacturerdib.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpManufacturerDIB : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithOneArguments();
    void testConstructorWithByteArrayDataArguments();
    void testConstructorWithVectorDataArguments();
    void testDebugStream();
    void testDataStream();
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
    QCOMPARE(quint8(manufacturerDIB.descriptionType()), quint8(0));
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
    QCOMPARE(manufacturerDIB.descriptionType(), QKnxNetIp::DescriptionType::ManufactorData);
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
    QCOMPARE(manufacturerDIB.descriptionType(), QKnxNetIp::DescriptionType::ManufactorData);
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
    QCOMPARE(manufacturerDIB.descriptionType(), QKnxNetIp::DescriptionType::ManufactorData);
    QCOMPARE(manufacturerDIB.manufacturerId(), quint16(65535));
    QCOMPARE(manufacturerDIB.manufacturerData<QByteArray>().size(), data.size());
    QCOMPARE(manufacturerDIB.manufacturerData< QVector<quint8> >().size(), data.size());
    QCOMPARE(manufacturerDIB.manufacturerData< QVector<quint8> >(), data);
}

void tst_QKnxNetIpManufacturerDIB::testDebugStream()
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

    qDebug() << QKnxNetIpManufacturerDIB();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpManufacturerDIB(65535);
    QCOMPARE(s_msg, QString::fromLatin1("0x04feffff"));

    qDebug() << QKnxNetIpManufacturerDIB(65535, QByteArray::fromHex("0102030405"));
    QCOMPARE(s_msg, QString::fromLatin1("0x09feffff0102030405"));

    qDebug() << QKnxNetIpManufacturerDIB(65535, { { 1, 2, 3, 4, 5 } });
    QCOMPARE(s_msg, QString::fromLatin1("0x09feffff0102030405"));
}

void tst_QKnxNetIpManufacturerDIB::testDataStream()
{
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpManufacturerDIB(65535);
        QCOMPARE(byteArray, QByteArray::fromHex("04FEFFFF"));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpManufacturerDIB(65535, QByteArray::fromHex("0102030405"));
        QCOMPARE(byteArray, QByteArray::fromHex("09FEFFFF0102030405"));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpManufacturerDIB(65535, { { 1, 2, 3, 4, 5 } });
        QCOMPARE(byteArray, QByteArray::fromHex("09FEFFFF0102030405"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpManufacturerDIB)

#include "tst_qknxnetipmanufacturerdib.moc"
