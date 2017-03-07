/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class TestStructure : private QKnxNetIpStruct
{
public:
    TestStructure() = default;
    explicit TestStructure(quint8 code)
        : QKnxNetIpStruct(code)
    {}
    TestStructure(quint8 code, const QKnxNetIpPayload &payload)
        : QKnxNetIpStruct(code, payload)
    {}
    explicit TestStructure(const QKnxNetIpStructHeader &header)
        : QKnxNetIpStruct(header)
    {}
    TestStructure(const QKnxNetIpStructHeader &header, const QKnxNetIpPayload &payload)
        : QKnxNetIpStruct(header, payload)
    {}

    using QKnxNetIpStruct::code;
    using QKnxNetIpStruct::setCode;

    using QKnxNetIpStruct::size;
    using QKnxNetIpStruct::header;

    using QKnxNetIpStruct::payload;
    using QKnxNetIpStruct::setPayload;

    using QKnxNetIpStruct::isValid;
    using QKnxNetIpStruct::toString;

    using QKnxNetIpStruct::bytes;
    template<typename T> static TestStructure fromBytes(const T &bytes, qint32 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

private:
    TestStructure(const QKnxNetIpStruct &other)
        : QKnxNetIpStruct(other)
    {}
};

class tst_QKnxNetIpStruct : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor()
    {
        TestStructure test;
        QCOMPARE(test.isValid(), false);
        QCOMPARE(test.code(), quint8(0));
        QCOMPARE(test.size(), quint16(0));
        QCOMPARE(test.bytes<QByteArray>(), QByteArray {});
        QCOMPARE(test.bytes<QVector<quint8>>(), QVector<quint8> {});
        QCOMPARE(test.bytes<std::deque<quint8>>(), std::deque<quint8> {});
        QCOMPARE(test.bytes<std::vector<quint8>>(), std::vector<quint8> {});
        QCOMPARE(test.toString(), QString("Total size { 0x00 }, Code { 0x00 }, Bytes {  }"));

        auto header = test.header();
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.code(), quint8(0));
        QCOMPARE(header.size(), quint16(0));
        QCOMPARE(header.totalSize(), quint16(0));
        QCOMPARE(header.payloadSize(), quint16(0));
        QCOMPARE(header.bytes<QByteArray>(), QByteArray {});
        QCOMPARE(header.bytes<QVector<quint8>>(), QVector<quint8> {});
        QCOMPARE(header.bytes<std::deque<quint8>>(), std::deque<quint8> {});
        QCOMPARE(header.bytes<std::vector<quint8>>(), std::vector<quint8> {});
        QCOMPARE(header.toString(), QString("Total size { 0x00 }, Code { 0x00 }"));

        auto payload = test.payload();
        QCOMPARE(payload.size(), quint16(0));
        QCOMPARE(payload.bytes<QByteArray>(), QByteArray {});
        QCOMPARE(payload.bytes<QVector<quint8>>(), QVector<quint8> {});
        QCOMPARE(payload.bytes<std::deque<quint8>>(), std::deque<quint8> {});
        QCOMPARE(payload.bytes<std::vector<quint8>>(), std::vector<quint8> {});
        QCOMPARE(payload.toString(), QString("Bytes {  }"));
    }

    void testConstructor_Code()
    {
        TestStructure test(0x00);
        QCOMPARE(test.isValid(), false);
        QCOMPARE(test.code(), quint8(0));
        QCOMPARE(test.size(), quint16(2));
        QCOMPARE(test.bytes<QByteArray>(), QByteArray::fromHex("0200"));
        QCOMPARE(test.bytes<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(test.bytes<std::deque<quint8>>(), std::deque<quint8>({ 0x02, 0x00 }));
        QCOMPARE(test.bytes<std::vector<quint8>>(), std::vector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(test.toString(), QString("Total size { 0x02 }, Code { 0x00 }, Bytes {  }"));

        auto header = test.header();
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.code(), quint8(0));
        QCOMPARE(header.size(), quint16(2));
        QCOMPARE(header.totalSize(), quint16(2));
        QCOMPARE(header.payloadSize(), quint16(0));
        QCOMPARE(header.bytes<QByteArray>(), QByteArray::fromHex("0200"));
        QCOMPARE(header.bytes<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(header.bytes<std::deque<quint8>>(), std::deque<quint8>({ 0x02, 0x00 }));
        QCOMPARE(header.bytes<std::vector<quint8>>(), std::vector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(header.toString(), QString("Total size { 0x02 }, Code { 0x00 }"));

        auto payload = test.payload();
        QCOMPARE(payload.size(), quint16(0));
        QCOMPARE(payload.bytes<QByteArray>(), QByteArray {});
        QCOMPARE(payload.bytes<QVector<quint8>>(), QVector<quint8> {});
        QCOMPARE(payload.bytes<std::deque<quint8>>(), std::deque<quint8> {});
        QCOMPARE(payload.bytes<std::vector<quint8>>(), std::vector<quint8> {});
        QCOMPARE(payload.toString(), QString("Bytes {  }"));

        test = TestStructure(0x01);
        QCOMPARE(test.isValid(), true);
        QCOMPARE(test.code(), quint8(0x01));
        QCOMPARE(test.size(), quint16(2));
        QCOMPARE(test.bytes<QByteArray>(), QByteArray::fromHex("0201"));
        QCOMPARE(test.bytes<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x01 }));
        QCOMPARE(test.bytes<std::deque<quint8>>(), std::deque<quint8>({ 0x02, 0x01 }));
        QCOMPARE(test.bytes<std::vector<quint8>>(), std::vector<quint8>({ 0x02, 0x01 }));
        QCOMPARE(test.toString(), QString("Total size { 0x02 }, Code { 0x01 }, Bytes {  }"));

        header = test.header();
        QCOMPARE(header.isValid(), true);
        QCOMPARE(header.code(), quint8(0x01));
        QCOMPARE(header.size(), quint16(2));
        QCOMPARE(header.totalSize(), quint16(2));
        QCOMPARE(header.payloadSize(), quint16(0));
        QCOMPARE(header.bytes<QByteArray>(), QByteArray::fromHex("0201"));
        QCOMPARE(header.bytes<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x01 }));
        QCOMPARE(header.bytes<std::deque<quint8>>(), std::deque<quint8>({ 0x02, 0x01 }));
        QCOMPARE(header.bytes<std::vector<quint8>>(), std::vector<quint8>({ 0x02, 0x01 }));
        QCOMPARE(header.toString(), QString("Total size { 0x02 }, Code { 0x01 }"));

        payload = test.payload();
        QCOMPARE(payload.size(), quint16(0));
        QCOMPARE(payload.bytes<QByteArray>(), QByteArray {});
        QCOMPARE(payload.bytes<QVector<quint8>>(), QVector<quint8> {});
        QCOMPARE(payload.bytes<std::deque<quint8>>(), std::deque<quint8> {});
        QCOMPARE(payload.bytes<std::vector<quint8>>(), std::vector<quint8> {});
        QCOMPARE(payload.toString(), QString("Bytes {  }"));
    }

    void testConstructor_CodeAndPayload()
    {
        QKnxNetIpPayload payload;

        TestStructure test(0x00, payload);
        QCOMPARE(test.isValid(), false);
        QCOMPARE(test.code(), quint8(0));
        QCOMPARE(test.size(), quint16(2));
        QCOMPARE(test.bytes<QByteArray>(), QByteArray::fromHex("0200"));
        QCOMPARE(test.bytes<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(test.bytes<std::deque<quint8>>(), std::deque<quint8>({ 0x02, 0x00 }));
        QCOMPARE(test.bytes<std::vector<quint8>>(), std::vector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(test.toString(), QString("Total size { 0x02 }, Code { 0x00 }, Bytes {  }"));

        auto header = test.header();
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.code(), quint8(0));
        QCOMPARE(header.size(), quint16(2));
        QCOMPARE(header.totalSize(), quint16(2));
        QCOMPARE(header.payloadSize(), quint16(0));
        QCOMPARE(header.bytes<QByteArray>(), QByteArray::fromHex("0200"));
        QCOMPARE(header.bytes<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(header.bytes<std::deque<quint8>>(), std::deque<quint8>({ 0x02, 0x00 }));
        QCOMPARE(header.bytes<std::vector<quint8>>(), std::vector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(header.toString(), QString("Total size { 0x02 }, Code { 0x00 }"));

        payload = test.payload();
        QCOMPARE(payload.size(), quint16(0));
        QCOMPARE(payload.bytes<QByteArray>(), QByteArray {});
        QCOMPARE(payload.bytes<QVector<quint8>>(), QVector<quint8> {});
        QCOMPARE(payload.bytes<std::deque<quint8>>(), std::deque<quint8> {});
        QCOMPARE(payload.bytes<std::vector<quint8>>(), std::vector<quint8> {});
        QCOMPARE(payload.toString(), QString("Bytes {  }"));

        payload.setBytes(QVector<quint8>({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));
        test = TestStructure(0x01, payload);
        QCOMPARE(test.isValid(), true);
        QCOMPARE(test.code(), quint8(0x01));
        QCOMPARE(test.size(), quint16(0x08));
        QCOMPARE(test.bytes<QByteArray>(), QByteArray::fromHex("0801000102030405"));
        QCOMPARE(test.bytes<QVector<quint8>>(), QVector<quint8>({ 0x08, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));
        QCOMPARE(test.bytes<std::deque<quint8>>(), std::deque<quint8>({ 0x08, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));
        QCOMPARE(test.bytes<std::vector<quint8>>(), std::vector<quint8>({ 0x08, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));
        QCOMPARE(test.toString(), QString("Total size { 0x08 }, Code { 0x01 }, Bytes { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }"));

        header = test.header();
        QCOMPARE(header.isValid(), true);
        QCOMPARE(header.code(), quint8(0x01));
        QCOMPARE(header.size(), quint16(2));
        QCOMPARE(header.totalSize(), quint16(8));
        QCOMPARE(header.payloadSize(), quint16(6));
        QCOMPARE(header.bytes<QByteArray>(), QByteArray::fromHex("0801"));
        QCOMPARE(header.bytes<QVector<quint8>>(), QVector<quint8>({ 0x08, 0x01 }));
        QCOMPARE(header.bytes<std::deque<quint8>>(), std::deque<quint8>({ 0x08, 0x01 }));
        QCOMPARE(header.bytes<std::vector<quint8>>(), std::vector<quint8>({ 0x08, 0x01 }));
        QCOMPARE(header.toString(), QString("Total size { 0x08 }, Code { 0x01 }"));

        payload = test.payload();
        QCOMPARE(payload.size(), quint16(0x06));
        QCOMPARE(payload.bytes<QByteArray>(), QByteArray::fromHex("000102030405"));
        QCOMPARE(payload.bytes<QVector<quint8>>(), QVector<quint8>({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));
        QCOMPARE(payload.bytes<std::deque<quint8>>(), std::deque<quint8>({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));
        QCOMPARE(payload.bytes<std::vector<quint8>>(), std::vector<quint8>({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));
        QCOMPARE(payload.toString(), QString("Bytes { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }"));
    }

    void testConstructor_Header()
    {
        // TODO: add
    }

    void testConstructor_HeaderPayload()
    {
        // TODO: add
    }

    void testHeaderSize()
    {
        QByteArray ba = QByteArray(0xfc, 0x05);

        QKnxNetIpPayload payload;
        payload.setBytes(ba);

        TestStructure test(0x01, payload);
        QCOMPARE(test.size(), quint16(0xfe));
        QCOMPARE(test.bytes<QByteArray>(), QByteArray::fromHex("fe01") + ba);

        QCOMPARE(test.header().size(), quint16(0x02));
        QCOMPARE(test.header().bytes<QByteArray>(), QByteArray::fromHex("fe01"));

        QCOMPARE(test.payload().size(), quint16(0xfc));
        QCOMPARE(test.payload().bytes<QByteArray>(), ba);

        ba += QByteArray::fromHex("001122334466778899");
        payload.setBytes(ba);

        test.setPayload(payload);
        QCOMPARE(test.size(), quint16(0x109));
        QCOMPARE(test.bytes<QByteArray>(), QByteArray::fromHex("ff010901") + ba);

        QCOMPARE(test.header().size(), quint16(0x04));
        QCOMPARE(test.header().bytes<QByteArray>(), QByteArray::fromHex("ff010901"));

        QCOMPARE(test.payload().size(), quint16(0x0105));
        QCOMPARE(test.payload().bytes<QByteArray>(), ba);
    }

    void testToString()
    {
        TestStructure test;
        QCOMPARE(test.toString(), QString("Total size { 0x00 }, Code { 0x00 }, Bytes {  }"));

        test.setCode(0x01);
        QCOMPARE(test.toString(), QString("Total size { 0x02 }, Code { 0x01 }, Bytes {  }"));

        QKnxNetIpPayload payload;
        payload.setBytes(QByteArray::fromHex("001122334466778899aabbccddeeff"));
        test.setPayload(payload);

        QCOMPARE(test.toString(), QString("Total size { 0x11 }, Code { 0x01 }, Bytes { 0x00, "
            "0x11, 0x22, 0x33, 0x44, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff }"));
    }

    void testDebugStream()
    {
        // TODO: add
    }

    void testDataStream()
    {
        // TODO: add
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpStruct)

#include "tst_qknxnetipstructure.moc"
