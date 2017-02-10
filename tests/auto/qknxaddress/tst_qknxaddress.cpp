/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxaddress.h>
#include <QtTest/qtest.h>

Q_DECLARE_METATYPE(QKnxAddress)
Q_DECLARE_METATYPE(QKnxAddress::Type)
Q_DECLARE_METATYPE(QKnxAddress::Notation)

#define QTEST_COLUMNS \
    QTest::addColumn<QKnxAddress>("address"); \
    QTest::addColumn<QKnxAddress::Type>("type"); \
    QTest::addColumn<QKnxAddress::Notation>("notation"); \
    QTest::addColumn<bool>("isValid"); \
    QTest::addColumn<QString>("toString");

QKnxAddress::Type InvalidType = static_cast<QKnxAddress::Type>(0xff);

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxAddress : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor()
    {
        QKnxAddress address;
        QCOMPARE(address.type(), InvalidType);
        QCOMPARE(address.notation(), QKnxAddress::Notation::ThreeLevel);
        QCOMPARE(address.isValid(), false);
        QCOMPARE(address.toString(), QStringLiteral(""));
        QCOMPARE(address.rawData<QByteArray>(), (QByteArray { }));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { }));
    }

    void testConstructorFromQuint16_data()
    {
        QTEST_COLUMNS
        QTest::addColumn<QVector<quint8>>("rawData");

        QByteArray raw(2, Qt::Uninitialized);
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, 2305)")
            << QKnxAddress(QKnxAddress::Type::Group, 2305)
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1/1/1")
            << QVector<quint8> { 0x09, 0x01 };

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, 4353)")
            << QKnxAddress(QKnxAddress::Type::Individual, 4353)
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1.1.1")
            << QVector<quint8> { 0x11, 0x01 };

        QTest::newRow("QKnxAddress(InvalidType, 0)")
            << QKnxAddress(InvalidType, 0)
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
    }

    void testConstructorFromQuint16()
    {
        QFETCH(QKnxAddress, address);
        QTEST(address.type(), "type");
        QTEST(address.notation(), "notation");
        QTEST(address.isValid(), "isValid");
        QTEST(address.toString(), "toString");
        QTEST(address.rawData<QVector<quint8>>(), "rawData");
    }

    void testConstructorFromQString_data()
    {
        QTEST_COLUMNS
        QTest::addColumn<QVector<quint8>>("rawData");


        // invalid separators

        const char *testInvalidSeparator[] = { "/", "//", "///", "////", ".", "..", "...", "...." };
        for (const char *data : testInvalidSeparator) {
            QTest::newRow(QByteArray("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
            QTest::newRow(QByteArray("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
            QTest::newRow(QByteArray("QKnxAddress(InvalidType, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(InvalidType, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
        }


        // invalid numbers

        const char *testInvalidNumber[] = { "", "-1", "text", "65536" };
        for (const char *data : testInvalidNumber) {
            QTest::newRow(QByteArray("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
            QTest::newRow(QByteArray("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
            QTest::newRow(QByteArray("QKnxAddress(InvalidType, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(InvalidType, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
        }
        QTest::newRow("QKnxAddress(InvalidType, QStringLiteral(\"0\"))")
            << QKnxAddress(InvalidType, QStringLiteral("0"))
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
        QTest::newRow("QKnxAddress(InvalidType, QStringLiteral(\"65535\"))")
            << QKnxAddress(InvalidType, QStringLiteral("65535"))
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };


        // invalid group addresses

        const char *testInvalidGroupAddress[] = { "-1/", "-1/-1", "-1/-1/-1", "-1/-1/-1/",
            "-1/-1/-1/-1", "32/", "32/2048", "32/8/", "32/7/256", "32/8/255/", "32/8/256/256",
            "31/7.255", "1.7/255/", "some/text/255", "1.text/255/" };
        for (const char *data : testInvalidGroupAddress) {
            QTest::newRow(QByteArray("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
            QTest::newRow(QByteArray("QKnxAddress(InvalidType, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(InvalidType, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
        }


        // invalid individual addresses

        const char *testInvalidIndividualAddress[] = { "-1.", "-1.-1", "-1.-1.-1", "-1.-1.-1.",
            "-1.-1.-1.-1", "16.", "16.15", "16.15.", "16.15.256", "15.16.255.", "32.8.256.256",
            "31.7/255", "1.7/255/", "some.text/255", "1.text/255." };
        for (const char *data : testInvalidIndividualAddress) {
            QTest::newRow(QByteArray("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
            QTest::newRow(QByteArray("QKnxAddress(InvalidType, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(InvalidType, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QVector<quint8> { };
        }


        // valid numbers

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0/0/0")
            << QVector<quint8> { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"0\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("0"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0.0.0")
            << QVector<quint8> { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"65535\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("65535"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("31/7/255")
            << QVector<quint8> { 0xff, 0xff };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"65535\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("65535"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("15.15.255")
            << QVector<quint8> { 0xff, 0xff };


        // valid 2-level group addresses

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::TwoLevel
            << true
            << QStringLiteral("0/0")
            << QVector<quint8> { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"31/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("31/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::TwoLevel
            << true
            << QStringLiteral("31/0")
            << QVector<quint8> { 0xf8, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/2047\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/2047"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::TwoLevel
            << true
            << QStringLiteral("0/2047")
            << QVector<quint8> { 0x07, 0xff };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"31/2047\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("31/2047"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::TwoLevel
            << true
            << QStringLiteral("31/2047")
            << QVector<quint8> { 0xff, 0xff };


        // valid 3-level group addresses

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/0/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/0/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0/0/0")
            << QVector<quint8> { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"31/0/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("31/0/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("31/0/0")
            << QVector<quint8> { 0xf8, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/7/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/7/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0/7/0")
            << QVector<quint8> { 0x07, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/0/255\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/0/255"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0/0/255")
            << QVector<quint8> { 0x00, 0xff };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"31/7/255\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("31/7/255"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("31/7/255")
            << QVector<quint8> { 0xff, 0xff };


        // valid 3-level individual addresses

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"0.0.0\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("0.0.0"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0.0.0")
            << QVector<quint8> { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"15.0.0\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("15.0.0"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("15.0.0")
            << QVector<quint8> { 0xf0, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"0.15.0\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("0.15.0"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0.15.0")
            << QVector<quint8> { 0x0f, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"0.0.255\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("0.0.255"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0.0.255")
            << QVector<quint8> { 0x00, 0xff };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"15.15.255\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("15.15.255"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("15.15.255")
            << QVector<quint8> { 0xff, 0xff };
    }

    void testConstructorFromQString()
    {
        QFETCH(QKnxAddress, address);
        QTEST(address.type(), "type");
        QTEST(address.notation(), "notation");
        QTEST(address.isValid(), "isValid");
        QTEST(address.toString(), "toString");
        QTEST(address.rawData<QVector<quint8>>(), "rawData");
    }

    void testConstructorFromQByteArray_data()
    {
        QTEST_COLUMNS
        QTest::addColumn<QByteArray>("rawData");

        QByteArray address(3, Qt::Uninitialized);
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QByteArray { })")
            << QKnxAddress(QKnxAddress::Type::Group, QByteArray { })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QByteArray { };

        address[0] = quint8(0x09);
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QByteArray { 0x09 })")
            << QKnxAddress(QKnxAddress::Type::Group, address.mid(0, 1))
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QByteArray { };
        address[1] = quint8(0x01);
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QByteArray { 0x09, 0x01 })")
            << QKnxAddress(QKnxAddress::Type::Group, address.mid(0, 2))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1/1/1")
            << address.mid(0, 2);
        address[2] = quint8(0x07);
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QByteArray { 0x09, 0x01, 0x07 })")
            << QKnxAddress(QKnxAddress::Type::Group, address)
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1/1/1")
            << address.mid(0, 2);

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QByteArray { })")
            << QKnxAddress(QKnxAddress::Type::Individual, QByteArray { })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QByteArray { };
        address[0] = quint8(0x11);
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QByteArray { 0x11 })")
            << QKnxAddress(QKnxAddress::Type::Individual, address.mid(0, 1))
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QByteArray { };
        address[1] = quint8(0x01);
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QByteArray { 0x11, 0x01 })")
            << QKnxAddress(QKnxAddress::Type::Individual, address.mid(0, 2))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1.1.1")
            << address.mid(0, 2);
        address[2] = quint8(0x03);
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QByteArray { 0x11, 0x01, 0x03 })")
            << QKnxAddress(QKnxAddress::Type::Individual, address)
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1.1.1")
            << address.mid(0, 2);

        QTest::newRow("QKnxAddress(InvalidType, QByteArray { })")
            << QKnxAddress(InvalidType, QByteArray { })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QByteArray { };
        QTest::newRow("QKnxAddress(InvalidType, QByteArray { 0x11 })")
            << QKnxAddress(InvalidType, address.mid(0, 1))
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QByteArray { };
        QTest::newRow("QKnxAddress(InvalidType, QByteArray { 0x11, 0x01 })")
            << QKnxAddress(InvalidType, address.mid(0, 2))
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QByteArray { };
        QTest::newRow("QKnxAddress(InvalidType, QByteArray { 0x11, 0x01, 0x03 })")
            << QKnxAddress(InvalidType, address)
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QByteArray { };
    }

    void testConstructorFromQByteArray()
    {
        QFETCH(QKnxAddress, address);
        QTEST(address.type(), "type");
        QTEST(address.notation(), "notation");
        QTEST(address.isValid(), "isValid");
        QTEST(address.toString(), "toString");
        QTEST(address.rawData<QByteArray>(), "rawData");
    }

    void testConstructorFromQVector_data()
    {
        QTEST_COLUMNS
        QTest::addColumn<QVector<quint8>>("rawData");

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QVector<quint8> { })")
            << QKnxAddress(QKnxAddress::Type::Group, QVector<quint8> { })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QVector<quint8> { 0x09 })")
            << QKnxAddress(QKnxAddress::Type::Group, QVector<quint8> { 0x09 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QVector<quint8> { 0x09, 0x01 })")
            << QKnxAddress(QKnxAddress::Type::Group, QVector<quint8> { 0x09, 0x01 })
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1/1/1")
            << QVector<quint8> { 0x09, 0x01 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QVector<quint8> { 0x09, 0x01, 0x07 })")
            << QKnxAddress(QKnxAddress::Type::Group, QVector<quint8> { 0x09, 0x01, 0x07 })
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1/1/1")
            << QVector<quint8> { 0x09, 0x01 };

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QVector<quint8> { })")
            << QKnxAddress(QKnxAddress::Type::Individual, QVector<quint8> { })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QVector<quint8> { 0x11 })")
            << QKnxAddress(QKnxAddress::Type::Individual, QVector<quint8> { 0x11 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QVector<quint8> { 0x11, 0x01 })")
            << QKnxAddress(QKnxAddress::Type::Individual, QVector<quint8> { 0x11, 0x01 })
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1.1.1")
            << QVector<quint8> { 0x11, 0x01 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QVector<quint8> { 0x11, 0x01, 0x03 })")
            << QKnxAddress(QKnxAddress::Type::Individual, QVector<quint8> { 0x11, 0x01, 0x03 })
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1.1.1")
            << QVector<quint8> { 0x11, 0x01 };

        QTest::newRow("QKnxAddress(InvalidType, QVector<quint8> { })")
            << QKnxAddress(InvalidType, QVector<quint8> { })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
        QTest::newRow("QKnxAddress(InvalidType, QVector<quint8> { 0x11 })")
            << QKnxAddress(InvalidType, QVector<quint8> { 0x11 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
        QTest::newRow("QKnxAddress(InvalidType, QVector<quint8> { 0x11, 0x01 })")
            << QKnxAddress(InvalidType, QVector<quint8> { 0x11, 0x01 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
        QTest::newRow("QKnxAddress(InvalidType, QVector<quint8> { 0x11, 0x01, 0x03 })")
            << QKnxAddress(InvalidType, QVector<quint8> { 0x11, 0x01, 0x03 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QVector<quint8> { };
    }

    void testConstructorFromQVector()
    {
        QFETCH(QKnxAddress, address);
        QTEST(address.type(), "type");
        QTEST(address.notation(), "notation");
        QTEST(address.isValid(), "isValid");
        QTEST(address.toString(), "toString");
        QTEST(address.rawData<QVector<quint8>>(), "rawData");
    }

    void testCreateGroup()
    {
        // invalid 2-level group address

        QKnxAddress address = QKnxAddress::createGroup(1, 5155);
        QCOMPARE(address.type(), InvalidType);
        QCOMPARE(address.notation(), QKnxAddress::Notation::ThreeLevel);
        QCOMPARE(address.isValid(), false);
        QCOMPARE(address.toString(), QStringLiteral(""));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { }));

        // valid 2-level group address

        address = QKnxAddress::createGroup(1, 515);
        QCOMPARE(address.type(), QKnxAddress::Type::Group);
        QCOMPARE(address.notation(), QKnxAddress::Notation::TwoLevel);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.toString(), QStringLiteral("1/515"));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { 0x0a, 0x03 }));

        // valid 3-level group address

        address = QKnxAddress::createGroup(1, 0, 1);
        QCOMPARE(address.type(), QKnxAddress::Type::Group);
        QCOMPARE(address.notation(), QKnxAddress::Notation::ThreeLevel);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.toString(), QStringLiteral("1/0/1"));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { 0x08, 0x01 }));
    }

    void testCreateIndividual()
    {
        const QKnxAddress address = QKnxAddress::createIndividual(1, 0, 1);
        QCOMPARE(address.type(), QKnxAddress::Type::Individual);
        QCOMPARE(address.notation(), QKnxAddress::Notation::ThreeLevel);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.toString(), QStringLiteral("1.0.1"));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { 0x10, 0x01 }));
    }

    void testGroupBroadcast()
    {
        QKnxAddress address = QKnxAddress::Group::Broadcast;
        QCOMPARE(address.type(), QKnxAddress::Type::Group);
        QCOMPARE(address.notation(), QKnxAddress::Notation::ThreeLevel);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isBroadcast(), true);
        QCOMPARE(address.toString(), QStringLiteral("0/0/0"));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { 0x00, 0x00 }));

        address = { QKnxAddress::Type::Group, 0x0000 };
        QCOMPARE(address.type(), QKnxAddress::Type::Group);
        QCOMPARE(address.notation(), QKnxAddress::Notation::ThreeLevel);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isBroadcast(), true);
        QCOMPARE(address.toString(), QStringLiteral("0/0/0"));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { 0x00, 0x00 }));
    }

    void testIndividualUnregistered()
    {
        auto address = QKnxAddress::Individual::Unregistered;
        QCOMPARE(address.type(), QKnxAddress::Type::Individual);
        QCOMPARE(address.notation(), QKnxAddress::Notation::ThreeLevel);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isBroadcast(), false);
        QCOMPARE(address.isUnregistered(), true);
        QCOMPARE(address.toString(), QStringLiteral("15.15.255"));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { 0xff, 0xff }));

        address = { QKnxAddress::Type::Individual, 0x0000 };
        QCOMPARE(address.type(), QKnxAddress::Type::Individual);
        QCOMPARE(address.notation(), QKnxAddress::Notation::ThreeLevel);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isBroadcast(), false);
        QCOMPARE(address.isUnregistered(), false);
        QCOMPARE(address.toString(), QStringLiteral("0.0.0"));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { 0x00, 0x00 }));

        address = { QKnxAddress::Type::Individual, 0x01ff };
        QCOMPARE(address.type(), QKnxAddress::Type::Individual);
        QCOMPARE(address.notation(), QKnxAddress::Notation::ThreeLevel);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isUnregistered(), true);
        QCOMPARE(address.toString(), QStringLiteral("0.1.255"));
        QCOMPARE(address.rawData<QVector<quint8>>(), (QVector<quint8> { 0x01, 0xff }));
    }

    void testIndividualIsCouplerOrRouter()
    {
        QKnxAddress address = QKnxAddress::Group::Broadcast;
        QCOMPARE(address.isCouplerOrRouter(), false);

        address = { QKnxAddress::Type::Individual, 0xffff };
        QCOMPARE(address.isCouplerOrRouter(), false);

        address = { QKnxAddress::Type::Individual, 0x0100 };
        QCOMPARE(address.isCouplerOrRouter(), true);
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

        qDebug() << QKnxAddress();
        QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

        qDebug() << QKnxAddress(QKnxAddress::Type::Group, 2305);
        QCOMPARE(s_msg, QString::fromLatin1("0x0901"));

        qDebug() << QKnxAddress(QKnxAddress::Type::Individual, 4353);
        QCOMPARE(s_msg, QString::fromLatin1("0x1101"));
    }

    void testDataStream()
    {
        {
            QByteArray byteArray;
            QDataStream out(&byteArray, QIODevice::WriteOnly);
            out << QKnxAddress(QKnxAddress::Type::Group, 2305);
            QCOMPARE(byteArray, QByteArray::fromHex("0901"));
        }

        {
            QByteArray byteArray;
            QDataStream out(&byteArray, QIODevice::WriteOnly);
            out << QKnxAddress(QKnxAddress::Type::Individual, 4353);
            QCOMPARE(byteArray, QByteArray::fromHex("1101"));
        }
    }
};

QTEST_APPLESS_MAIN(tst_QKnxAddress)

#include "tst_qknxaddress.moc"
