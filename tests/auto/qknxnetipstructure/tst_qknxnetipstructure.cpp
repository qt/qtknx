/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipstructure.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class TestStructure : public QKnxNetIpStructure
{
public:
    TestStructure() = default;

    template <typename T> TestStructure(quint8 code, const T &data)
        : QKnxNetIpStructure(code, data)
    {
    }

    template <typename T> static TestStructure fromRawData(quint8 code, const T &data, int offset)
    {
        TestStructure tst;
        tst.setRawData(code, data, offset);
        return tst;
    }

    using QKnxNetIpStructure::toString;
};

class tst_QKnxNetIpStructure : public QObject
{
    Q_OBJECT

private slots:
    void testConstructor()
    {
        TestStructure test;
        QCOMPARE(test.dataSize(), 0);
        QCOMPARE(test.data<QByteArray>(), QByteArray {});
        QCOMPARE(test.data<QVector<quint8>>(), QVector<quint8> {});

        QCOMPARE(test.rawSize(), 2);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("0200"));
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x00 }));

        test = TestStructure(0x01, QByteArray {});
        QCOMPARE(test.dataSize(), 0);
        QCOMPARE(test.data<QByteArray>(), QByteArray {});
        QCOMPARE(test.data<QVector<quint8>>(), QVector<quint8> {});

        QCOMPARE(test.rawSize(), 2);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("0201"));
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x01 }));

        test = TestStructure(0x01, QByteArray::fromHex("0102030405"));
        QCOMPARE(test.dataSize(), 5);
        QCOMPARE(test.data<QByteArray>(), QByteArray::fromHex("0102030405"));
        QCOMPARE(test.data<QVector<quint8>>(), QVector<quint8>({ 0x01, 0x02, 0x03, 0x04, 0x05 }));

        QCOMPARE(test.rawSize(), 7);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("07010102030405"));
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0x07, 0x01, 0x01, 0x02, 0x03, 0x04, 0x05 }));

        auto ba = QByteArray(0xf6, 0x05) + QByteArray::fromHex("001122334466778899");
        auto vec = QVector<quint8>(0xf6, 0x05)
            + QVector<quint8>({ 0x00, 0x11, 0x22, 0x33, 0x44, 0x66, 0x77, 0x88, 0x99 });

        test = TestStructure(0x01, ba);
        QCOMPARE(test.dataSize(), 0xff);
        QCOMPARE(test.data<QByteArray>(), ba);
        QCOMPARE(test.data<QVector<quint8>>(), vec);

        QCOMPARE(test.rawSize(), 0x103);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("ff010301") + ba);
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0xff, 0x01, 0x03, 0x01 }) + vec);

        test = TestStructure::fromRawData(0x01, QByteArray::fromHex("aabbccddeeff010301") + ba
            + QByteArray::fromHex("aabbccddeeff") , 5);
        QCOMPARE(test.dataSize(), 0xff);
        QCOMPARE(test.data<QByteArray>(), ba);
        QCOMPARE(test.data<QVector<quint8>>(), vec);

        QCOMPARE(test.rawSize(), 0x103);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("ff010301") + ba);
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0xff, 0x01, 0x03, 0x01 }) + vec);

        test = TestStructure(0x01, vec);
        QCOMPARE(test.dataSize(), 0xff);
        QCOMPARE(test.data<QByteArray>(), ba);
        QCOMPARE(test.data<QVector<quint8>>(), vec);

        QCOMPARE(test.rawSize(), 0x103);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("ff010301") + ba);
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0xff, 0x01, 0x03, 0x01 }) + vec);

        test = TestStructure::fromRawData(0x01, QByteArray(), 5);
        QCOMPARE(test.dataSize(), 0);
        QCOMPARE(test.data<QByteArray>(), QByteArray {});
        QCOMPARE(test.data<QVector<quint8>>(), QVector<quint8> {});

        QCOMPARE(test.rawSize(), 2);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("0200"));
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x00 }));

        test = TestStructure::fromRawData(0x01, QVector<quint8>(), 5);
        QCOMPARE(test.dataSize(), 0);
        QCOMPARE(test.data<QByteArray>(), QByteArray {});
        QCOMPARE(test.data<QVector<quint8>>(), QVector<quint8> {});
        QCOMPARE(test.data<std::vector<quint8>>(), std::vector<quint8>());

        QCOMPARE(test.rawSize(), 2);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("0200"));
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(test.rawData<std::vector<quint8>>(), std::vector<quint8>({ 0x02, 0x00 }));

        test = TestStructure::fromRawData(0x01, std::vector<quint8>(), 5);
        QCOMPARE(test.dataSize(), 0);
        QCOMPARE(test.data<QByteArray>(), QByteArray {});
        QCOMPARE(test.data<QVector<quint8>>(), QVector<quint8> {});
        QCOMPARE(test.data<std::vector<quint8>>(), std::vector<quint8> {});

        QCOMPARE(test.rawSize(), 2);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("0200"));
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x00 }));
        QCOMPARE(test.rawData<std::vector<quint8>>(), std::vector<quint8>({ 0x02, 0x00 }));

        test = TestStructure(0x01, std::vector<quint8>());
        QCOMPARE(test.dataSize(), 0);
        QCOMPARE(test.data<QByteArray>(), QByteArray {});
        QCOMPARE(test.data<QVector<quint8>>(), QVector<quint8> {});
        QCOMPARE(test.data<std::vector<quint8>>(), std::vector<quint8>());

        QCOMPARE(test.rawSize(), 2);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("0201"));
        QCOMPARE(test.rawData<QVector<quint8>>(), QVector<quint8>({ 0x02, 0x01 }));
        QCOMPARE(test.rawData<std::vector<quint8>>(), std::vector<quint8>({ 0x02, 0x01 }));

        test = TestStructure(0x01, std::deque<quint8> { 0x01, 0x02, 0x03, 0x04, 0x05 });
        QCOMPARE(test.dataSize(), 5);
        QCOMPARE(test.data<QByteArray>(), QByteArray::fromHex("0102030405"));
        QCOMPARE(test.data<std::deque<quint8>>(), std::deque<quint8>({ 0x01, 0x02, 0x03, 0x04, 0x05 }));

        QCOMPARE(test.rawSize(), 7);
        QCOMPARE(test.rawData<QByteArray>(), QByteArray::fromHex("07010102030405"));
        QCOMPARE(test.rawData<std::deque<quint8>>(),
            std::deque<quint8>({ 0x07, 0x01, 0x01, 0x02, 0x03, 0x04, 0x05 }));
        QCOMPARE(test.rawData<std::vector<quint8>>(),
            std::vector<quint8>({ 0x07, 0x01, 0x01, 0x02, 0x03, 0x04, 0x05 }));
    }

    void testData()
    {
        TestStructure test;
        QCOMPARE(test.data<QByteArray>(0, 10), QByteArray {});
        QCOMPARE(test.data<QVector<quint8>>(10, 10), QVector<quint8> {});

        auto ba = QByteArray::fromHex("001122334466778899aabbccddeeff");
        test = { 0x01, ba };
        QCOMPARE(test.data<QByteArray>(0, 10), ba.mid(0, 10));
        QCOMPARE(test.data<QByteArray>(10, 2), ba.mid(10, 2));
        QCOMPARE(test.data<QByteArray>(20, 2), ba.mid(20, 2));
        QCOMPARE(test.data<QByteArray>(10, 10), QByteArray {});
    }

    void testToString()
    {
        QCOMPARE(TestStructure(0x01, QByteArray::fromHex("001122334466778899aabbccddeeff"))
            .toString(), QString::fromLatin1("Raw size { 0x11 }, Code { 0x01 }, Data { 0x00, 0x11,"
                " 0x22, 0x33, 0x44, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff }"));
    }

    void testDebugStream()
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

        qDebug() << TestStructure { 0x01, QByteArray::fromHex("001122334466778899aabbccddeeff") };
        QCOMPARE(s_msg, QString::fromLatin1("0x1101001122334466778899aabbccddeeff"));
    }

    void testDataStream()
    {
        auto data = QByteArray::fromHex("001122334466778899aabbccddeeff");
        TestStructure test = { 0x01, data };

        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out <<test;
        QCOMPARE(byteArray, QByteArray::fromHex("1101001122334466778899aabbccddeeff"));
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpStructure)

#include "tst_qknxnetipstructure.moc"
