/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

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
        QCOMPARE(address.isValid(), false);
        QCOMPARE(address.toString(), QStringLiteral(""));
        QCOMPARE(address.bytes(), QKnxByteArray {});
    }

    void testConstructorFromQuint16_data()
    {
        QTEST_COLUMNS
        QTest::addColumn<QKnxByteArray>("bytes");

        //QKnxByteArray raw(2);
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, 2305)")
            << QKnxAddress(QKnxAddress::Type::Group, 2305)
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1/1/1")
            << QKnxByteArray { 0x09, 0x01 };

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, 4353)")
            << QKnxAddress(QKnxAddress::Type::Individual, 4353)
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1.1.1")
            << QKnxByteArray { 0x11, 0x01 };

        QTest::newRow("QKnxAddress(InvalidType, 0)")
            << QKnxAddress(InvalidType, 0)
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
    }

    void testConstructorFromQuint16()
    {
        QFETCH(QKnxAddress, address);
        QTEST(address.type(), "type");
        QTEST(address.isValid(), "isValid");

        auto notation = *static_cast<const QKnxAddress::Notation *>(QTest::qElementData("notation",
            qMetaTypeId<QKnxAddress::Notation>()));

        QTEST(address.toString(notation), "toString");
        QTEST(address.bytes(), "bytes");
    }

    void testConstructorFromQString_data()
    {
        QTEST_COLUMNS
        QTest::addColumn<QKnxByteArray>("bytes");

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
                << QKnxByteArray {};
            QTest::newRow(QByteArray("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QKnxByteArray {};
            QTest::newRow(QByteArray("QKnxAddress(InvalidType, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(InvalidType, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QKnxByteArray {};
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
                << QKnxByteArray {};
            QTest::newRow(QByteArray("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QKnxByteArray {};
            QTest::newRow(QByteArray("QKnxAddress(InvalidType, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(InvalidType, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QKnxByteArray {};
        }
        QTest::newRow("QKnxAddress(InvalidType, QStringLiteral(\"0\"))")
            << QKnxAddress(InvalidType, QStringLiteral("0"))
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
        QTest::newRow("QKnxAddress(InvalidType, QStringLiteral(\"65535\"))")
            << QKnxAddress(InvalidType, QStringLiteral("65535"))
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};

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
                << QKnxByteArray {};
            QTest::newRow(QByteArray("QKnxAddress(InvalidType, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(InvalidType, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QKnxByteArray {};
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
                << QKnxByteArray {};
            QTest::newRow(QByteArray("QKnxAddress(InvalidType, QStringLiteral(\""
                    + QByteArray(data) + "\"))").constData())
                << QKnxAddress(InvalidType, QStringLiteral("%1").arg(data))
                << InvalidType
                << QKnxAddress::Notation::ThreeLevel
                << false
                << QStringLiteral("")
                << QKnxByteArray {};
        }

        // valid numbers

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0/0/0")
            << QKnxByteArray { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"0\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("0"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0.0.0")
            << QKnxByteArray { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"65535\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("65535"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("31/7/255")
            << QKnxByteArray { 0xff, 0xff };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"65535\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("65535"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("15.15.255")
            << QKnxByteArray { 0xff, 0xff };

        // valid 2-level group addresses

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::TwoLevel
            << true
            << QStringLiteral("0/0")
            << QKnxByteArray { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"31/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("31/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::TwoLevel
            << true
            << QStringLiteral("31/0")
            << QKnxByteArray { 0xf8, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/2047\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/2047"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::TwoLevel
            << true
            << QStringLiteral("0/2047")
            << QKnxByteArray { 0x07, 0xff };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"31/2047\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("31/2047"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::TwoLevel
            << true
            << QStringLiteral("31/2047")
            << QKnxByteArray { 0xff, 0xff };

        // valid 3-level group addresses

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/0/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/0/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0/0/0")
            << QKnxByteArray { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"31/0/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("31/0/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("31/0/0")
            << QKnxByteArray { 0xf8, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/7/0\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/7/0"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0/7/0")
            << QKnxByteArray { 0x07, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"0/0/255\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("0/0/255"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0/0/255")
            << QKnxByteArray { 0x00, 0xff };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"31/7/255\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("31/7/255"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("31/7/255")
            << QKnxByteArray { 0xff, 0xff };

        // valid 3-level individual addresses

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"0.0.0\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("0.0.0"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0.0.0")
            << QKnxByteArray { 0x00, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"15.0.0\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("15.0.0"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("15.0.0")
            << QKnxByteArray { 0xf0, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"0.15.0\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("0.15.0"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0.15.0")
            << QKnxByteArray { 0x0f, 0x00 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"0.0.255\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("0.0.255"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("0.0.255")
            << QKnxByteArray { 0x00, 0xff };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"15.15.255\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("15.15.255"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("15.15.255")
            << QKnxByteArray { 0xff, 0xff };
    }

    void testConstructorFromQString()
    {
        QFETCH(QKnxAddress, address);
        QTEST(address.type(), "type");
        QTEST(address.isValid(), "isValid");

        auto notation = *static_cast<const QKnxAddress::Notation *>(QTest::qElementData("notation",
            qMetaTypeId<QKnxAddress::Notation>()));

        QTEST(address.toString(notation), "toString");
        QTEST(address.bytes(), "bytes");
    }

    void testConstructorFromQKnxByteArray_data()
    {
        QTEST_COLUMNS
        QTest::addColumn<QKnxByteArray>("bytes");

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QKnxByteArray {})")
            << QKnxAddress(QKnxAddress::Type::Group, QKnxByteArray {})
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QKnxByteArray { 0x09 })")
            << QKnxAddress(QKnxAddress::Type::Group, QKnxByteArray { 0x09 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QKnxByteArray { 0x09, 0x01 })")
            << QKnxAddress(QKnxAddress::Type::Group, QKnxByteArray { 0x09, 0x01 })
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1/1/1")
            << QKnxByteArray { 0x09, 0x01 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QKnxByteArray { 0x09, 0x01, 0x07 })")
            << QKnxAddress(QKnxAddress::Type::Group, QKnxByteArray { 0x09, 0x01, 0x07 })
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1/1/1")
            << QKnxByteArray { 0x09, 0x01 };

        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QKnxByteArray {})")
            << QKnxAddress(QKnxAddress::Type::Individual, QKnxByteArray {})
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QKnxByteArray { 0x11 })")
            << QKnxAddress(QKnxAddress::Type::Individual, QKnxByteArray { 0x11 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QKnxByteArray { 0x11, 0x01 })")
            << QKnxAddress(QKnxAddress::Type::Individual, QKnxByteArray { 0x11, 0x01 })
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1.1.1")
            << QKnxByteArray { 0x11, 0x01 };
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QKnxByteArray { 0x11, 0x01, 0x03 })")
            << QKnxAddress(QKnxAddress::Type::Individual, QKnxByteArray { 0x11, 0x01, 0x03 })
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << QStringLiteral("1.1.1")
            << QKnxByteArray { 0x11, 0x01 };

        QTest::newRow("QKnxAddress(InvalidType, QKnxByteArray {})")
            << QKnxAddress(InvalidType, QKnxByteArray {})
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
        QTest::newRow("QKnxAddress(InvalidType, QKnxByteArray { 0x11 })")
            << QKnxAddress(InvalidType, QKnxByteArray { 0x11 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
        QTest::newRow("QKnxAddress(InvalidType, QKnxByteArray { 0x11, 0x01 })")
            << QKnxAddress(InvalidType, QKnxByteArray { 0x11, 0x01 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
        QTest::newRow("QKnxAddress(InvalidType, QKnxByteArray { 0x11, 0x01, 0x03 })")
            << QKnxAddress(InvalidType, QKnxByteArray { 0x11, 0x01, 0x03 })
            << InvalidType
            << QKnxAddress::Notation::ThreeLevel
            << false
            << QStringLiteral("")
            << QKnxByteArray {};
    }

    void testConstructorFromQKnxByteArray()
    {
        QFETCH(QKnxAddress, address);
        QTEST(address.type(), "type");
        QTEST(address.isValid(), "isValid");

        auto notation = *static_cast<const QKnxAddress::Notation *>(QTest::qElementData("notation",
            qMetaTypeId<QKnxAddress::Notation>()));

        QTEST(address.toString(notation), "toString");
        QTEST(address.bytes(), "bytes");
    }

    void testCreateGroup()
    {
        // invalid 2-level group address

        QKnxAddress address = QKnxAddress::createGroup(1, 5155);
        QCOMPARE(address.type(), InvalidType);
        QCOMPARE(address.isValid(), false);
        QCOMPARE(address.toString(), QStringLiteral(""));
        QCOMPARE(address.bytes(), QKnxByteArray {});

        // valid 2-level group address

        address = QKnxAddress::createGroup(1, 515);
        QCOMPARE(address.type(), QKnxAddress::Type::Group);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.bytes(), QKnxByteArray ({ 0x0a, 0x03 }));
        QCOMPARE(address.toString(QKnxAddress::Notation::TwoLevel), QStringLiteral("1/515"));

        // valid 3-level group address

        address = QKnxAddress::createGroup(1, 0, 1);
        QCOMPARE(address.type(), QKnxAddress::Type::Group);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.toString(), QStringLiteral("1/0/1"));
        QCOMPARE(address.bytes(), QKnxByteArray ({ 0x08, 0x01 }));
    }

    void testCreateIndividual()
    {
        const QKnxAddress address = QKnxAddress::createIndividual(1, 0, 1);
        QCOMPARE(address.type(), QKnxAddress::Type::Individual);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.toString(), QStringLiteral("1.0.1"));
        QCOMPARE(address.toString(QKnxAddress::Notation::TwoLevel), QString());
        QCOMPARE(address.bytes(), QKnxByteArray ({ 0x10, 0x01 }));
    }

    void testGroupBroadcast()
    {
        QKnxAddress address = QKnxAddress::Group::Broadcast;
        QCOMPARE(address.type(), QKnxAddress::Type::Group);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isBroadcast(), true);
        QCOMPARE(address.toString(), QStringLiteral("0/0/0"));
        QCOMPARE(address.bytes(), QKnxByteArray ({ 0x00, 0x00 }));

        address = { QKnxAddress::Type::Group, 0x0000 };
        QCOMPARE(address.type(), QKnxAddress::Type::Group);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isBroadcast(), true);
        QCOMPARE(address.toString(), QStringLiteral("0/0/0"));
        QCOMPARE(address.bytes(), QKnxByteArray ({ 0x00, 0x00 }));
    }

    void testIndividualUnregistered()
    {
        auto address = QKnxAddress::Individual::Unregistered;
        QCOMPARE(address.type(), QKnxAddress::Type::Individual);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isBroadcast(), false);
        QCOMPARE(address.isUnregistered(), true);
        QCOMPARE(address.toString(), QStringLiteral("15.15.255"));
        QCOMPARE(address.bytes(), QKnxByteArray ({ 0xff, 0xff }));

        address = { QKnxAddress::Type::Individual, 0x0000 };
        QCOMPARE(address.type(), QKnxAddress::Type::Individual);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isBroadcast(), false);
        QCOMPARE(address.isUnregistered(), false);
        QCOMPARE(address.toString(), QStringLiteral("0.0.0"));
        QCOMPARE(address.bytes(), QKnxByteArray ({ 0x00, 0x00 }));

        address = { QKnxAddress::Type::Individual, 0x01ff };
        QCOMPARE(address.type(), QKnxAddress::Type::Individual);
        QCOMPARE(address.isValid(), true);
        QCOMPARE(address.isUnregistered(), true);
        QCOMPARE(address.toString(), QStringLiteral("0.1.255"));
        QCOMPARE(address.bytes(), QKnxByteArray ({ 0x01, 0xff }));
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

    void testComparisonOperators()
    {
         QKnxAddress address = QKnxAddress::Group::Broadcast;
         QCOMPARE(address, QKnxAddress(QKnxAddress::Type::Group, 0x0000));
         QCOMPARE(address == QKnxAddress(QKnxAddress::Type::Individual, 0x0000), false);
         QCOMPARE(address != QKnxAddress(QKnxAddress::Type::Individual, 0x0000), true);
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

    void testAddressComponents_data()
    {
        QTest::addColumn<QKnxAddress>("address");
        QTest::addColumn<QKnxAddress::Type>("type");
        QTest::addColumn<QKnxAddress::Notation>("notation");
        QTest::addColumn<bool>("isValid");
        QTest::addColumn<quint8>("main");
        QTest::addColumn<quint8>("middle");
        QTest::addColumn<quint16>("sub");

        // valid individual
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral(\"1.2.3\"))")
            << QKnxAddress(QKnxAddress::Type::Individual, QStringLiteral("1.2.3"))
            << QKnxAddress::Type::Individual
            << QKnxAddress::Notation::ThreeLevel
            << true
            << (quint8)1
            << (quint8)2
            << (quint16)3;

        // valid 3-level group addresses
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"4/5/6\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("4/5/6"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::ThreeLevel
            << true
            << (quint8)4
            << (quint8)5
            << (quint16)6;

        // valid 2-level group addresses
        QTest::newRow("QKnxAddress(QKnxAddress::Type::Group, QStringLiteral(\"13/2045\"))")
            << QKnxAddress(QKnxAddress::Type::Group, QStringLiteral("13/2045"))
            << QKnxAddress::Type::Group
            << QKnxAddress::Notation::TwoLevel
            << true
            << (quint8)13
            << (quint8)7
            << (quint16)2045;
    }

    void testAddressComponents()
    {
        QFETCH(QKnxAddress, address);

        auto notation = *static_cast<const QKnxAddress::Notation *>(QTest::qElementData("notation",
            qMetaTypeId<QKnxAddress::Notation>()));

        QTEST(address.type(), "type");
        QTEST(address.isValid(), "isValid");
        QTEST(address.mainOrAreaSection(), "main");
        QTEST(address.middleOrLineSection(), "middle");
        QTEST(address.subOrDeviceSection(notation), "sub");
    }
};

QTEST_APPLESS_MAIN(tst_QKnxAddress)

#include "tst_qknxaddress.moc"
