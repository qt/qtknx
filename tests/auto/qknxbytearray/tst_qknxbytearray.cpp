/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Copyright (C) 2016 Intel Corporation.
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
****************************************************************************/

#include <QtTest/QtTest>

#include <QtKnx/qknxbytearray.h>

class tst_QKnxByteArray : public QObject
{
    Q_OBJECT

private slots:
    void swap();
    void startsWith_data();
    void startsWith();
    void startsWith_char();
    void endsWith_data();
    void endsWith();
    void endsWith_char();
    void reverseIterators();
    void chop_data();
    void chop();
    void prepend();
    void remove_data();
    void remove();
    void replace_data();
    void replace();
    void replaceWithSpecifiedLength();
    void indexOf_data();
    void indexOf();
    void lastIndexOf_data();
    void lastIndexOf();
    void toFromHex_data();
    void toFromHex();
};

void tst_QKnxByteArray::swap()
{
    QKnxByteArray b1 = { "b1", 2 }, b2 = { "b2", 2 };
    b1.swap(b2);
    QCOMPARE(b1, QKnxByteArray("b2", 2));
    QCOMPARE(b2, QKnxByteArray("b1", 2));
}

void tst_QKnxByteArray::startsWith_data()
{
    QTest::addColumn<QKnxByteArray>("ba");
    QTest::addColumn<QKnxByteArray>("sw");
    QTest::addColumn<bool>("result");

    QTest::newRow("01") << QKnxByteArray() << QKnxByteArray() << true;
    QTest::newRow("02") << QKnxByteArray() << QKnxByteArray("", 0) << true;
    QTest::newRow("03") << QKnxByteArray() << QKnxByteArray("hallo", 5) << false;

    QTest::newRow("04") << QKnxByteArray("", 0) << QKnxByteArray() << true;
    QTest::newRow("05") << QKnxByteArray("", 0) << QKnxByteArray("", 0) << true;
    QTest::newRow("06") << QKnxByteArray("", 0) << QKnxByteArray("h", 1) << false;

    QTest::newRow("07") << QKnxByteArray("hallo", 5) << QKnxByteArray("h", 1) << true;
    QTest::newRow("08") << QKnxByteArray("hallo", 5) << QKnxByteArray("hallo", 5) << true;
    QTest::newRow("09") << QKnxByteArray("hallo", 5) << QKnxByteArray("", 0) << true;
    QTest::newRow("10") << QKnxByteArray("hallo", 5) << QKnxByteArray("hallohallo", 10) << false;
    QTest::newRow("11") << QKnxByteArray("hallo", 5) << QKnxByteArray() << true;
}

void tst_QKnxByteArray::startsWith()
{
    QFETCH(QKnxByteArray, ba);
    QFETCH(QKnxByteArray, sw);
    QFETCH(bool, result);

    QVERIFY(ba.startsWith(sw) == result);

    if (sw.isNull()) {
        QVERIFY(ba.startsWith(0x00u) != result);
    } else {
        QVERIFY(ba.startsWith({ sw.data(), sw.size() }) == result);
    }
}

void tst_QKnxByteArray::startsWith_char()
{
    QVERIFY(QKnxByteArray("hallo", 5).startsWith('h'));
    QVERIFY(!QKnxByteArray("hallo", 5).startsWith('\0'));
    QVERIFY(!QKnxByteArray("hallo", 5).startsWith('o'));
    QVERIFY(QKnxByteArray("h", 1).startsWith('h'));
    QVERIFY(!QKnxByteArray("h", 1).startsWith('\0'));
    QVERIFY(!QKnxByteArray("h", 1).startsWith('o'));
    QVERIFY(!QKnxByteArray("hallo", 5).startsWith('l'));
    QVERIFY(!QKnxByteArray("", 0).startsWith('\0'));
    QVERIFY(!QKnxByteArray("", 0).startsWith('a'));
    QVERIFY(!QKnxByteArray().startsWith('a'));
    QVERIFY(!QKnxByteArray().startsWith('\0'));
}

void tst_QKnxByteArray::endsWith_data()
{
    QTest::addColumn<QKnxByteArray>("ba");
    QTest::addColumn<QKnxByteArray>("sw");
    QTest::addColumn<bool>("result");

    QTest::newRow("01") << QKnxByteArray() << QKnxByteArray() << true;
    QTest::newRow("02") << QKnxByteArray() << QKnxByteArray("", 0) << true;
    QTest::newRow("03") << QKnxByteArray() << QKnxByteArray("hallo", 5) << false;

    QTest::newRow("04") << QKnxByteArray("", 0) << QKnxByteArray() << true;
    QTest::newRow("05") << QKnxByteArray("", 0) << QKnxByteArray("", 0) << true;
    QTest::newRow("06") << QKnxByteArray("", 0) << QKnxByteArray("h", 1) << false;

    QTest::newRow("07") << QKnxByteArray("hallo", 5) << QKnxByteArray("o", 1) << true;
    QTest::newRow("08") << QKnxByteArray("hallo", 5) << QKnxByteArray("hallo", 5) << true;
    QTest::newRow("09") << QKnxByteArray("hallo", 5) << QKnxByteArray("", 0) << true;
    QTest::newRow("10") << QKnxByteArray("hallo", 5) << QKnxByteArray("hallohallo", 10) << false;
    QTest::newRow("11") << QKnxByteArray("hallo", 5) << QKnxByteArray() << true;
}

void tst_QKnxByteArray::endsWith()
{
    QFETCH(QKnxByteArray, ba);
    QFETCH(QKnxByteArray, sw);
    QFETCH(bool, result);

    QVERIFY(ba.endsWith(sw) == result);

    if (sw.isNull()) {
        QVERIFY(ba.endsWith(0x00) != result);
    } else {
        QVERIFY(ba.endsWith({ sw.data(), sw.size() }) == result);
    }
}

void tst_QKnxByteArray::endsWith_char()
{
    QVERIFY(QKnxByteArray("hallo", 5).endsWith('o'));
    QVERIFY(!QKnxByteArray("hallo", 5).endsWith('\0'));
    QVERIFY(!QKnxByteArray("hallo", 5).endsWith('h'));
    QVERIFY(QKnxByteArray("h", 1).endsWith('h'));
    QVERIFY(!QKnxByteArray("h", 1).endsWith('\0'));
    QVERIFY(!QKnxByteArray("h", 1).endsWith('o'));
    QVERIFY(!QKnxByteArray("hallo", 5).endsWith('l'));
    QVERIFY(!QKnxByteArray("", 0).endsWith('\0'));
    QVERIFY(!QKnxByteArray("", 0).endsWith('a'));
    QVERIFY(!QKnxByteArray().endsWith('a'));
    QVERIFY(!QKnxByteArray().endsWith('\0'));
}

void tst_QKnxByteArray::reverseIterators()
{
    QKnxByteArray s = { "1234", 4 };
    QKnxByteArray sr = s;
    std::reverse(sr.begin(), sr.end());
    const QKnxByteArray &csr = sr;
    QVERIFY(std::equal(s.begin(), s.end(), sr.rbegin()));
    QVERIFY(std::equal(s.begin(), s.end(), sr.crbegin()));
    QVERIFY(std::equal(s.begin(), s.end(), csr.rbegin()));
    QVERIFY(std::equal(sr.rbegin(), sr.rend(), s.begin()));
    QVERIFY(std::equal(sr.crbegin(), sr.crend(), s.begin()));
    QVERIFY(std::equal(csr.rbegin(), csr.rend(), s.begin()));
}

void tst_QKnxByteArray::chop_data()
{
    QTest::addColumn<QKnxByteArray>("src");
    QTest::addColumn<int>("choplength");
    QTest::addColumn<QKnxByteArray>("expected");

    QTest::newRow("1") << QKnxByteArray("short1", 6) << 128 << QKnxByteArray();
    QTest::newRow("2") << QKnxByteArray("short2", 6) << int(strlen("short2"))
                    << QKnxByteArray();
    QTest::newRow("3") << QKnxByteArray("abcdef\0foo", 10) << 2
                    << QKnxByteArray("abcdef\0f", 8);
    QTest::newRow("4") << QKnxByteArray("STARTTLS\r\n", 10) << 2
                    << QKnxByteArray("STARTTLS", 8);
    QTest::newRow("5") << QKnxByteArray("", 0) << 1 << QKnxByteArray();
    QTest::newRow("6") << QKnxByteArray("foo", 3) << 0 << QKnxByteArray("foo", 3);
}

void tst_QKnxByteArray::chop()
{
    QFETCH(QKnxByteArray, src);
    QFETCH(int, choplength);
    QFETCH(QKnxByteArray, expected);

    src = src.chop(choplength);
    QCOMPARE(src, expected);
}

void tst_QKnxByteArray::prepend()
{
    QKnxByteArray ba("foo", 3);
    QCOMPARE(ba.prepend(QKnxByteArray()), QKnxByteArray("foo", 3));
    QCOMPARE(ba.prepend(QKnxByteArray("1", 1)), QKnxByteArray("1foo", 4));
    QCOMPARE(ba.prepend('2'), QKnxByteArray("21foo", 5));
    QCOMPARE(ba.prepend(-1, 'x'), QKnxByteArray("21foo", 5));
    QCOMPARE(ba.prepend(3, 'x'), QKnxByteArray("xxx21foo", 8));
}

void tst_QKnxByteArray::remove_data()
{
    QTest::addColumn<QKnxByteArray>("src");
    QTest::addColumn<int>("position");
    QTest::addColumn<int>("length");
    QTest::addColumn<QKnxByteArray>("expected");

    QTest::newRow("1") << QKnxByteArray::fromByteArray("Montreal") << 1 << 4
                    << QKnxByteArray::fromByteArray("Meal");
    QTest::newRow("2") << QKnxByteArray() << 10 << 10 << QKnxByteArray();
    QTest::newRow("3") << QKnxByteArray::fromByteArray("hi") << 0 << 10 << QKnxByteArray();
    QTest::newRow("4") << QKnxByteArray::fromByteArray("Montreal") << 4 << 100
                    << QKnxByteArray::fromByteArray("Mont");

    // index out of range
    QTest::newRow("5") << QKnxByteArray::fromByteArray("Montreal") << 8 << 1
                    << QKnxByteArray::fromByteArray("Montreal");
    QTest::newRow("6") << QKnxByteArray::fromByteArray("Montreal") << 18 << 4
                    << QKnxByteArray::fromByteArray("Montreal");
}

void tst_QKnxByteArray::remove()
{
    QFETCH(QKnxByteArray, src);
    QFETCH(int, position);
    QFETCH(int, length);
    QFETCH(QKnxByteArray, expected);
    QCOMPARE(src.remove(position, length), expected);
}

void tst_QKnxByteArray::replace_data()
{
    QTest::addColumn<QKnxByteArray>("src");
    QTest::addColumn<int>("pos");
    QTest::addColumn<int>("len");
    QTest::addColumn<QKnxByteArray>("after");
    QTest::addColumn<QKnxByteArray>("expected");

    QTest::newRow("1") << QKnxByteArray::fromByteArray("Say yes!") << 4 << 3
        << QKnxByteArray::fromByteArray("no") << QKnxByteArray::fromByteArray("Say no!");
    QTest::newRow("2") << QKnxByteArray::fromByteArray("rock and roll") << 5 << 3
        << QKnxByteArray::fromByteArray("&") << QKnxByteArray::fromByteArray("rock & roll");
    QTest::newRow("3") << QKnxByteArray::fromByteArray("foo") << 3 << 0 << QKnxByteArray::fromByteArray("bar")
        << QKnxByteArray::fromByteArray("foobar");
    QTest::newRow("4") << QKnxByteArray() << 0 << 0 << QKnxByteArray() << QKnxByteArray();
    // index out of range
    QTest::newRow("5") << QKnxByteArray() << 3 << 0 << QKnxByteArray::fromByteArray("hi")
        << QKnxByteArray::fromByteArray("   hi");
    // Optimized path
    QTest::newRow("6") << QKnxByteArray::fromByteArray("abcdef") << 3 << 12
        << QKnxByteArray::fromByteArray("abcdefghijkl")
        << QKnxByteArray::fromByteArray("abcabcdefghijkl");
    QTest::newRow("7") << QKnxByteArray::fromByteArray("abcdef") << 3 << 4
        << QKnxByteArray::fromByteArray("abcdefghijkl")
        << QKnxByteArray::fromByteArray("abcabcdefghijkl");
    QTest::newRow("8") << QKnxByteArray::fromByteArray("abcdef") << 3 << 3
        << QKnxByteArray::fromByteArray("abcdefghijkl")
        << QKnxByteArray::fromByteArray("abcabcdefghijkl");
    QTest::newRow("9") << QKnxByteArray::fromByteArray("abcdef") << 3 << 2
        << QKnxByteArray::fromByteArray("abcdefghijkl")
        << QKnxByteArray::fromByteArray("abcabcdefghijklf");
    QTest::newRow("10") << QKnxByteArray::fromByteArray("abcdef") << 2 << 2
        << QKnxByteArray::fromByteArray("xx") << QKnxByteArray::fromByteArray("abxxef");
}

void tst_QKnxByteArray::replace()
{
    QFETCH(QKnxByteArray, src);
    QFETCH(int, pos);
    QFETCH(int, len);
    QFETCH(QKnxByteArray, after);
    QFETCH(QKnxByteArray, expected);

    QKnxByteArray str1 = src;
    QKnxByteArray str2 = src;

    QCOMPARE(str1.replace(pos, len, after), expected);
}

void tst_QKnxByteArray::replaceWithSpecifiedLength()
{
    const char after[] = "zxc\0vbnmqwert";
    int lenAfter = 6;
    QKnxByteArray ba("abcdefghjk", 10);
    ba.replace(0, 2, QKnxByteArray(after, lenAfter));

    const char _expected[] = "zxc\0vbcdefghjk";
    QKnxByteArray expected(_expected, sizeof(_expected) - 1);
    QCOMPARE(ba,expected);
}

void tst_QKnxByteArray::indexOf_data()
{
    QTest::addColumn<QKnxByteArray>("haystack");
    QTest::addColumn<QKnxByteArray>("needle");
    QTest::addColumn<int>("startpos");
    QTest::addColumn<int>("expected");

    QTest::newRow( "1" ) << QKnxByteArray("abc", 3) << QKnxByteArray("a", 1) << 0 << 0;
    QTest::newRow( "2" ) << QKnxByteArray("abc", 3) << QKnxByteArray("A", 1) << 0 << -1;
    QTest::newRow( "3" ) << QKnxByteArray("abc", 3) << QKnxByteArray("a", 1) << 1 << -1;
    QTest::newRow( "4" ) << QKnxByteArray("abc", 3) << QKnxByteArray("A", 1) << 1 << -1;
    QTest::newRow( "5" ) << QKnxByteArray("abc", 3) << QKnxByteArray("b", 1) << 0 << 1;
    QTest::newRow( "6" ) << QKnxByteArray("abc", 3) << QKnxByteArray("B", 1) << 0 << -1;
    QTest::newRow( "7" ) << QKnxByteArray("abc", 3) << QKnxByteArray("b", 1) << 1 << 1;
    QTest::newRow( "8" ) << QKnxByteArray("abc", 3) << QKnxByteArray("B", 1) << 1 << -1;
    QTest::newRow( "9" ) << QKnxByteArray("abc", 3) << QKnxByteArray("b", 1) << 2 << -1;
    QTest::newRow( "10" ) << QKnxByteArray("abc", 3) << QKnxByteArray("c", 1) << 0 << 2;
    QTest::newRow( "11" ) << QKnxByteArray("abc", 3) << QKnxByteArray("C", 1) << 0 << -1;
    QTest::newRow( "12" ) << QKnxByteArray("abc", 3) << QKnxByteArray("c", 1) << 1 << 2;
    QTest::newRow( "13" ) << QKnxByteArray("abc", 3) << QKnxByteArray("C", 1) << 1 << -1;
    QTest::newRow( "14" ) << QKnxByteArray("abc", 3) << QKnxByteArray("c", 1) << 2 << 2;
    QTest::newRow( "15" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("bc", 2) << 0 << -1;
    QTest::newRow( "16" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("Bc", 2) << 0 << 1;
    QTest::newRow( "17" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("bC", 2) << 0 << -1;
    QTest::newRow( "18" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("BC", 2) << 0 << -1;

    static const char h19[] = {'x', 0x00, (char)0xe7, 0x25, 0x1c, 0x0a};
    static const char n19[] = {0x00, 0x00, 0x01, 0x00};
    QTest::newRow( "19" ) << QKnxByteArray(h19, sizeof(h19))
                          << QKnxByteArray(n19, sizeof(n19)) << 0 << -1;

    QTest::newRow( "empty" ) << QKnxByteArray("", 0) << QKnxByteArray("x", 1) << 0 << -1;
    QTest::newRow( "null" ) << QKnxByteArray() << QKnxByteArray("x", 1) << 0 << -1;
    QTest::newRow( "null-in-null") << QKnxByteArray() << QKnxByteArray() << 0 << 0;
    QTest::newRow( "empty-in-null") << QKnxByteArray() << QKnxByteArray("", 0) << 0 << 0;
    QTest::newRow( "null-in-empty") << QKnxByteArray("", 0) << QKnxByteArray() << 0 << 0;
    QTest::newRow( "empty-in-empty") << QKnxByteArray("", 0) << QKnxByteArray("", 0) << 0 << 0;

    QKnxByteArray veryBigHaystack(500, 'a');
    veryBigHaystack += 'B';
    QTest::newRow("BoyerMooreStressTest") << veryBigHaystack << veryBigHaystack << 0 << 0;
    QTest::newRow("BoyerMooreStressTest2") << QKnxByteArray(veryBigHaystack + 'c') <<  QKnxByteArray(veryBigHaystack) << 0 << 0;
    QTest::newRow("BoyerMooreStressTest3") << QKnxByteArray('c' + veryBigHaystack) <<  QKnxByteArray(veryBigHaystack) << 0 << 1;
    QTest::newRow("BoyerMooreStressTest4") << QKnxByteArray(veryBigHaystack) <<  QKnxByteArray(veryBigHaystack + 'c') << 0 << -1;
    QTest::newRow("BoyerMooreStressTest5") << QKnxByteArray(veryBigHaystack) <<  QKnxByteArray('c' + veryBigHaystack) << 0 << -1;
    QTest::newRow("BoyerMooreStressTest6") << QKnxByteArray('d' + veryBigHaystack) <<  QKnxByteArray('c' + veryBigHaystack) << 0 << -1;
    QTest::newRow("BoyerMooreStressTest7") << QKnxByteArray(veryBigHaystack + 'c') <<  QKnxByteArray('c' + veryBigHaystack) << 0 << -1;
}

void tst_QKnxByteArray::indexOf()
{
    QFETCH( QKnxByteArray, haystack );
    QFETCH( QKnxByteArray, needle );
    QFETCH( int, startpos );
    QFETCH( int, expected );

    bool hasNull = needle.contains('\0');

    QCOMPARE( haystack.indexOf(needle, startpos), expected );
    if (!hasNull)
        QCOMPARE( haystack.indexOf(needle, startpos), expected );
    if (needle.size() == 1)
        QCOMPARE( haystack.indexOf(needle.at(0), startpos), expected );

    if (startpos == 0) {
        QCOMPARE( haystack.indexOf(needle), expected );
        if (!hasNull)
            QCOMPARE( haystack.indexOf(needle), expected );
        if (needle.size() == 1)
            QCOMPARE( haystack.indexOf(needle.at(0)), expected );
    }
}

void tst_QKnxByteArray::lastIndexOf_data()
{
    QTest::addColumn<QKnxByteArray>("haystack");
    QTest::addColumn<QKnxByteArray>("needle");
    QTest::addColumn<int>("startpos");
    QTest::addColumn<int>("expected");

    QTest::newRow( "1" ) << QKnxByteArray("abc", 3) << QKnxByteArray("a", 1) << 0 << 0;
    QTest::newRow( "2" ) << QKnxByteArray("abc", 3) << QKnxByteArray("A", 1) << 0 << -1;
    QTest::newRow( "3" ) << QKnxByteArray("abc", 3) << QKnxByteArray("a", 1) << 1 << 0;
    QTest::newRow( "4" ) << QKnxByteArray("abc", 3) << QKnxByteArray("A", 1) << 1 << -1;
    QTest::newRow( "5" ) << QKnxByteArray("abc", 3) << QKnxByteArray("a", 1) << -1 << 0;
    QTest::newRow( "6" ) << QKnxByteArray("abc", 3) << QKnxByteArray("b", 1) << 0 << -1;
    QTest::newRow( "7" ) << QKnxByteArray("abc", 3) << QKnxByteArray("B", 1) << 0 << -1;
    QTest::newRow( "8" ) << QKnxByteArray("abc", 3) << QKnxByteArray("b", 1) << 1 << 1;
    QTest::newRow( "9" ) << QKnxByteArray("abc", 3) << QKnxByteArray("B", 1) << 1 << -1;
    QTest::newRow( "10" ) << QKnxByteArray("abc", 3) << QKnxByteArray("b", 1) << 2 << 1;
    QTest::newRow( "11" ) << QKnxByteArray("abc", 3) << QKnxByteArray("b", 1) << -1 << 1;
    QTest::newRow( "12" ) << QKnxByteArray("abc", 3) << QKnxByteArray("c", 1) << 0 << -1;
    QTest::newRow( "13" ) << QKnxByteArray("abc", 3) << QKnxByteArray("C", 1) << 0 << -1;
    QTest::newRow( "14" ) << QKnxByteArray("abc", 3) << QKnxByteArray("c", 1) << 1 << -1;
    QTest::newRow( "15" ) << QKnxByteArray("abc", 3) << QKnxByteArray("C", 1) << 1 << -1;
    QTest::newRow( "16" ) << QKnxByteArray("abc", 3) << QKnxByteArray("c", 1) << 2 << 2;
    QTest::newRow( "17" ) << QKnxByteArray("abc", 3) << QKnxByteArray("c", 1) << -1 << 2;
    QTest::newRow( "18" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("bc", 2) << 0 << -1;
    QTest::newRow( "19" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("Bc", 2) << 0 << -1;
    QTest::newRow( "20" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("Bc", 2) << 2 << 1;
    QTest::newRow( "21" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("Bc", 2) << 1 << 1;
    QTest::newRow( "22" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("Bc", 2) << -1 << 1;
    QTest::newRow( "23" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("bC", 2) << 0 << -1;
    QTest::newRow( "24" ) << QKnxByteArray("aBc", 3) << QKnxByteArray("BC", 2) << 0 << -1;

    static const char h25[] = {0x00, (char)0xbc, 0x03, 0x10, 0x0a };
    static const char n25[] = {0x00, 0x00, 0x01, 0x00};
    QTest::newRow( "25" ) << QKnxByteArray(h25, sizeof(h25))
                          << QKnxByteArray(n25, sizeof(n25)) << 0 << -1;

    QTest::newRow( "empty" ) << QKnxByteArray("", 0) << QKnxByteArray("x", 1) << -1 << -1;
    QTest::newRow( "null" ) << QKnxByteArray() << QKnxByteArray("x", 1) << -1 << -1;
    QTest::newRow( "null-in-null") << QKnxByteArray() << QKnxByteArray() << -1 << 0;
    QTest::newRow( "empty-in-null") << QKnxByteArray() << QKnxByteArray("", 0) << -1 << 0;
    QTest::newRow( "null-in-empty") << QKnxByteArray("", 0) << QKnxByteArray() << -1 << 0;
    QTest::newRow( "empty-in-empty") << QKnxByteArray("", 0) << QKnxByteArray("", 0) << -1 << 0;
}

void tst_QKnxByteArray::lastIndexOf()
{
    QFETCH( QKnxByteArray, haystack );
    QFETCH( QKnxByteArray, needle );
    QFETCH( int, startpos );
    QFETCH( int, expected );

    bool hasNull = needle.contains('\0');

    QCOMPARE( haystack.lastIndexOf(needle, startpos), expected );
    if (!hasNull)
        QCOMPARE( haystack.lastIndexOf(needle, startpos), expected );
    if (needle.size() == 1)
        QCOMPARE( haystack.lastIndexOf(needle.at(0), startpos), expected );

    if (startpos == -1) {
        QCOMPARE( haystack.lastIndexOf(needle), expected );
        if (!hasNull)
            QCOMPARE( haystack.lastIndexOf(needle), expected );
        if (needle.size() == 1)
            QCOMPARE( haystack.lastIndexOf(needle.at(0)), expected );
    }
}

void tst_QKnxByteArray::toFromHex_data()
{
    QTest::addColumn<QKnxByteArray>("str");
    QTest::addColumn<char>("sep");
    QTest::addColumn<QKnxByteArray>("hex");
    QTest::addColumn<QKnxByteArray>("hex_alt1");

    QTest::newRow("Qt is great! (default)")
        << QKnxByteArray::fromByteArray("Qt is great!")
        << '\0'
        << QKnxByteArray::fromByteArray("517420697320677265617421")
        << QKnxByteArray::fromByteArray("51 74 20 69 73 20 67 72 65 61 74 21");

    QTest::newRow("Qt is great! (with space)")
        << QKnxByteArray::fromByteArray("Qt is great!")
        << ' '
        << QKnxByteArray::fromByteArray("51 74 20 69 73 20 67 72 65 61 74 21")
        << QKnxByteArray::fromByteArray("51 74 20 69 73 20 67 72 65 61 74 21");

    QTest::newRow("Qt is great! (with minus)")
        << QKnxByteArray::fromByteArray("Qt is great!")
        << '-'
        << QKnxByteArray::fromByteArray("51-74-20-69-73-20-67-72-65-61-74-21")
        << QKnxByteArray::fromByteArray("51-74-20-69-73-20-67-72-65-61-74-21");

    QTest::newRow("Qt is so great!")
        << QKnxByteArray::fromByteArray("Qt is so great!")
        << '\0'
        << QKnxByteArray::fromByteArray("517420697320736f20677265617421")
        << QKnxByteArray::fromByteArray("51 74 20 69 73 20 73 6f 20 67 72 65 61 74 21");

    QTest::newRow("default-constructed")
        << QKnxByteArray()
        << '\0'
        << QKnxByteArray()
        << QKnxByteArray();

    QTest::newRow("default-constructed (with space)")
        << QKnxByteArray()
        << ' '
        << QKnxByteArray()
        << QKnxByteArray();

    QTest::newRow("empty")
        << QKnxByteArray::fromByteArray("")
        << '\0'
        << QKnxByteArray::fromByteArray("")
        << QKnxByteArray::fromByteArray("");

    QTest::newRow("empty (with space)")
        << QKnxByteArray::fromByteArray("")
        << ' '
        << QKnxByteArray::fromByteArray("")
        << QKnxByteArray::fromByteArray("");

    QTest::newRow("array-of-null")
        << QKnxByteArray("\0", 1)
        << '\0'
        << QKnxByteArray::fromByteArray("00")
        << QKnxByteArray::fromByteArray("0");

    QTest::newRow("no-leading-zero")
        << QKnxByteArray::fromByteArray("\xf")
        << '\0'
        << QKnxByteArray::fromByteArray("0f")
        << QKnxByteArray::fromByteArray("f");

    QTest::newRow("single-byte")
        << QKnxByteArray::fromByteArray("\xaf")
        << '\0'
        << QKnxByteArray::fromByteArray("af")
        << QKnxByteArray::fromByteArray("xaf");

    QTest::newRow("no-leading-zero")
        << QKnxByteArray::fromByteArray("\xd\xde\xad\xc0\xde")
        << '\0'
        << QKnxByteArray::fromByteArray("0ddeadc0de")
        << QKnxByteArray::fromByteArray("ddeadc0de");

    QTest::newRow("garbage")
        << QKnxByteArray::fromByteArray("\xC\xde\xeC\xea\xee\xDe\xee\xee")
        << '\0'
        << QKnxByteArray::fromByteArray("0cdeeceaeedeeeee")
        << QKnxByteArray::fromByteArray("Code less. Create more. Deploy everywhere.");

    QTest::newRow("under-defined-1")
        << QKnxByteArray::fromByteArray("\x1\x23")
        << '\0'
        << QKnxByteArray::fromByteArray("0123")
        << QKnxByteArray::fromByteArray("x123");

    QTest::newRow("under-defined-2")
        << QKnxByteArray::fromByteArray("\x12\x34")
        << '\0'
        << QKnxByteArray::fromByteArray("1234")
        << QKnxByteArray::fromByteArray("x1234");
}

void tst_QKnxByteArray::toFromHex()
{
    QFETCH(QKnxByteArray, str);
    QFETCH(char,       sep);
    QFETCH(QKnxByteArray, hex);
    QFETCH(QKnxByteArray, hex_alt1);

    if (sep == 0) {
        const QKnxByteArray th = str.toHex();
        QCOMPARE(th.size(), hex.size());
        QCOMPARE(th, hex);
    }

    {
        const QKnxByteArray th = str.toHex(sep);
        QCOMPARE(th.size(), hex.size());
        QCOMPARE(th, hex);
    }

    {
        const QKnxByteArray fh = QKnxByteArray::fromHex(hex);
        QCOMPARE(fh.size(), str.size());
        QCOMPARE(fh, str);
    }

    QCOMPARE(QKnxByteArray::fromHex(hex_alt1), str);
}

//void tst_QKnxByteArray::compare_data()
//{
//    QTest::addColumn<QKnxByteArray>("str1");
//    QTest::addColumn<QKnxByteArray>("str2");
//    QTest::addColumn<int>("result");
//
//    QTest::newRow("null")      << QKnxByteArray() << QKnxByteArray() << 0;
//    QTest::newRow("null-empty")<< QKnxByteArray() << QKnxByteArray("") << 0;
//    QTest::newRow("empty-null")<< QKnxByteArray("") << QKnxByteArray() << 0;
//    QTest::newRow("null-full") << QKnxByteArray() << QKnxByteArray("abc") << -1;
//    QTest::newRow("full-null") << QKnxByteArray("abc") << QKnxByteArray() << +1;
//    QTest::newRow("empty-full")<< QKnxByteArray("") << QKnxByteArray("abc") << -1;
//    QTest::newRow("full-empty")<< QKnxByteArray("abc") << QKnxByteArray("") << +1;
//    QTest::newRow("rawempty-full") << QKnxByteArray::fromRawData("abc", 0) << QKnxByteArray("abc") << -1;
//    QTest::newRow("full-rawempty") << QKnxByteArray("abc") << QKnxByteArray::fromRawData("abc", 0) << +1;
//
//    QTest::newRow("equal   1") << QKnxByteArray("abc") << QKnxByteArray("abc") << 0;
//    QTest::newRow("equal   2") << QKnxByteArray::fromRawData("abc", 3) << QKnxByteArray("abc") << 0;
//    QTest::newRow("equal   3") << QKnxByteArray::fromRawData("abcdef", 3) << QKnxByteArray("abc") << 0;
//    QTest::newRow("equal   4") << QKnxByteArray("abc") << QKnxByteArray::fromRawData("abc", 3) << 0;
//    QTest::newRow("equal   5") << QKnxByteArray("abc") << QKnxByteArray::fromRawData("abcdef", 3) << 0;
//    QTest::newRow("equal   6") << QKnxByteArray("a\0bc", 4) << QKnxByteArray("a\0bc", 4) << 0;
//    QTest::newRow("equal   7") << QKnxByteArray::fromRawData("a\0bcdef", 4) << QKnxByteArray("a\0bc", 4) << 0;
//    QTest::newRow("equal   8") << QKnxByteArray("a\0bc", 4) << QKnxByteArray::fromRawData("a\0bcdef", 4) << 0;
//
//    QTest::newRow("less    1") << QKnxByteArray("000") << QKnxByteArray("abc") << -1;
//    QTest::newRow("less    2") << QKnxByteArray::fromRawData("00", 3) << QKnxByteArray("abc") << -1;
//    QTest::newRow("less    3") << QKnxByteArray("000") << QKnxByteArray::fromRawData("abc", 3) << -1;
//    QTest::newRow("less    4") << QKnxByteArray("abc", 3) << QKnxByteArray("abc", 4) << -1;
//    QTest::newRow("less    5") << QKnxByteArray::fromRawData("abc\0", 3) << QKnxByteArray("abc\0", 4) << -1;
//    QTest::newRow("less    6") << QKnxByteArray("a\0bc", 4) << QKnxByteArray("a\0bd", 4) << -1;
//
//    QTest::newRow("greater 1") << QKnxByteArray("abc") << QKnxByteArray("000") << +1;
//    QTest::newRow("greater 2") << QKnxByteArray("abc") << QKnxByteArray::fromRawData("00", 3) << +1;
//    QTest::newRow("greater 3") << QKnxByteArray("abcd") << QKnxByteArray::fromRawData("abcd", 3) << +1;
//    QTest::newRow("greater 4") << QKnxByteArray("a\0bc", 4) << QKnxByteArray("a\0bb", 4) << +1;
//}
//
//void tst_QKnxByteArray::compare()
//{
//    QFETCH(QKnxByteArray, str1);
//    QFETCH(QKnxByteArray, str2);
//    QFETCH(int, result);
//
//    const bool isEqual   = result == 0;
//    const bool isLess    = result < 0;
//    const bool isGreater = result > 0;
//
//    // basic tests:
//    QCOMPARE(str1 == str2, isEqual);
//    QCOMPARE(str1 < str2, isLess);
//    QCOMPARE(str1 > str2, isGreater);
//
//    // composed tests:
//    QCOMPARE(str1 <= str2, isLess || isEqual);
//    QCOMPARE(str1 >= str2, isGreater || isEqual);
//    QCOMPARE(str1 != str2, !isEqual);
//
//    // inverted tests:
//    QCOMPARE(str2 == str1, isEqual);
//    QCOMPARE(str2 < str1, isGreater);
//    QCOMPARE(str2 > str1, isLess);
//
//    // composed, inverted tests:
//    QCOMPARE(str2 <= str1, isGreater || isEqual);
//    QCOMPARE(str2 >= str1, isLess || isEqual);
//    QCOMPARE(str2 != str1, !isEqual);
//
//    if (isEqual)
//        QVERIFY(qHash(str1) == qHash(str2));
//}
//
//void tst_QKnxByteArray::compareCharStar_data()
//{
//    QTest::addColumn<QKnxByteArray>("str1");
//    QTest::addColumn<QString>("string2");
//    QTest::addColumn<int>("result");
//
//    QTest::newRow("null-null") << QKnxByteArray() << QString() << 0;
//    QTest::newRow("null-empty") << QKnxByteArray() << "" << 0;
//    QTest::newRow("null-full") << QKnxByteArray() << "abc" << -1;
//    QTest::newRow("empty-null") << QKnxByteArray("") << QString() << 0;
//    QTest::newRow("empty-empty") << QKnxByteArray("") << "" << 0;
//    QTest::newRow("empty-full") << QKnxByteArray("") << "abc" << -1;
//    QTest::newRow("raw-null") << QKnxByteArray::fromRawData("abc", 0) << QString() << 0;
//    QTest::newRow("raw-empty") << QKnxByteArray::fromRawData("abc", 0) << QString("") << 0;
//    QTest::newRow("raw-full") << QKnxByteArray::fromRawData("abc", 0) << "abc" << -1;
//
//    QTest::newRow("full-null") << QKnxByteArray("abc") << QString() << +1;
//    QTest::newRow("full-empty") << QKnxByteArray("abc") << "" << +1;
//
//    QTest::newRow("equal1") << QKnxByteArray("abc") << "abc" << 0;
//    QTest::newRow("equal2") << QKnxByteArray("abcd", 3) << "abc" << 0;
//    QTest::newRow("equal3") << QKnxByteArray::fromRawData("abcd", 3) << "abc" << 0;
//
//    QTest::newRow("less1") << QKnxByteArray("ab") << "abc" << -1;
//    QTest::newRow("less2") << QKnxByteArray("abb") << "abc" << -1;
//    QTest::newRow("less3") << QKnxByteArray::fromRawData("abc", 2) << "abc" << -1;
//    QTest::newRow("less4") << QKnxByteArray("", 1) << "abc" << -1;
//    QTest::newRow("less5") << QKnxByteArray::fromRawData("", 1) << "abc" << -1;
//    QTest::newRow("less6") << QKnxByteArray("a\0bc", 4) << "a.bc" << -1;
//
//    QTest::newRow("greater1") << QKnxByteArray("ac") << "abc" << +1;
//    QTest::newRow("greater2") << QKnxByteArray("abd") << "abc" << +1;
//    QTest::newRow("greater3") << QKnxByteArray("abcd") << "abc" << +1;
//    QTest::newRow("greater4") << QKnxByteArray::fromRawData("abcd", 4) << "abc" << +1;
//}
//
//void tst_QKnxByteArray::compareCharStar()
//{
//    QFETCH(QKnxByteArray, str1);
//    QFETCH(QString, string2);
//    QFETCH(int, result);
//
//    const bool isEqual   = result == 0;
//    const bool isLess    = result < 0;
//    const bool isGreater = result > 0;
//    QKnxByteArray qba = string2.toLatin1();
//    const char *str2 = qba.constData();
//    if (string2.isNull())
//        str2 = 0;
//
//    // basic tests:
//    QCOMPARE(str1 == str2, isEqual);
//    QCOMPARE(str1 < str2, isLess);
//    QCOMPARE(str1 > str2, isGreater);
//
//    // composed tests:
//    QCOMPARE(str1 <= str2, isLess || isEqual);
//    QCOMPARE(str1 >= str2, isGreater || isEqual);
//    QCOMPARE(str1 != str2, !isEqual);
//
//    // inverted tests:
//    QCOMPARE(str2 == str1, isEqual);
//    QCOMPARE(str2 < str1, isGreater);
//    QCOMPARE(str2 > str1, isLess);
//
//    // composed, inverted tests:
//    QCOMPARE(str2 <= str1, isGreater || isEqual);
//    QCOMPARE(str2 >= str1, isLess || isEqual);
//    QCOMPARE(str2 != str1, !isEqual);
//}
//
//void tst_QKnxByteArray::repeatedSignature() const
//{
//    /* repated() should be a const member. */
//    const QKnxByteArray string;
//    string.repeated(3);
//}
//
//void tst_QKnxByteArray::repeated() const
//{
//    QFETCH(QKnxByteArray, string);
//    QFETCH(QKnxByteArray, expected);
//    QFETCH(int, count);
//
//    QCOMPARE(string.repeated(count), expected);
//}
//
//void tst_QKnxByteArray::repeated_data() const
//{
//    QTest::addColumn<QKnxByteArray>("string" );
//    QTest::addColumn<QKnxByteArray>("expected" );
//    QTest::addColumn<int>("count" );
//
//    /* Empty strings. */
//    QTest::newRow("data1")
//        << QKnxByteArray()
//        << QKnxByteArray()
//        << 0;
//
//    QTest::newRow("data2")
//        << QKnxByteArray()
//        << QKnxByteArray()
//        << -1004;
//
//    QTest::newRow("data3")
//        << QKnxByteArray()
//        << QKnxByteArray()
//        << 1;
//
//    QTest::newRow("data4")
//        << QKnxByteArray()
//        << QKnxByteArray()
//        << 5;
//
//    /* On simple string. */
//    QTest::newRow("data5")
//        << QKnxByteArray("abc")
//        << QKnxByteArray()
//        << -1004;
//
//    QTest::newRow("data6")
//        << QKnxByteArray("abc")
//        << QKnxByteArray()
//        << -1;
//
//    QTest::newRow("data7")
//        << QKnxByteArray("abc")
//        << QKnxByteArray()
//        << 0;
//
//    QTest::newRow("data8")
//        << QKnxByteArray("abc")
//        << QKnxByteArray("abc")
//        << 1;
//
//    QTest::newRow("data9")
//        << QKnxByteArray(("abc"))
//        << QKnxByteArray(("abcabc"))
//        << 2;
//
//    QTest::newRow("data10")
//        << QKnxByteArray(("abc"))
//        << QKnxByteArray(("abcabcabc"))
//        << 3;
//
//    QTest::newRow("data11")
//        << QKnxByteArray(("abc"))
//        << QKnxByteArray(("abcabcabcabc"))
//        << 4;
//
//    QTest::newRow("static not null terminated")
//        << QKnxByteArray(staticNotNullTerminated)
//        << QKnxByteArray("datadatadatadata")
//        << 4;
//    QTest::newRow("static standard")
//        << QKnxByteArray(staticStandard)
//        << QKnxByteArray("datadatadatadata")
//        << 4;
//    QTest::newRow("static shifted not null terminated")
//        << QKnxByteArray(staticShiftedNotNullTerminated)
//        << QKnxByteArray("datadatadatadata")
//        << 4;
//    QTest::newRow("static shifted")
//        << QKnxByteArray(staticShifted)
//        << QKnxByteArray("datadatadatadata")
//        << 4;
//}
//
//void tst_QKnxByteArray::byteRefDetaching() const
//{
//    {
//        QKnxByteArray str = "str";
//        QKnxByteArray copy;
//        copy[0] = 'S';
//
//        QCOMPARE(str, QKnxByteArray("str"));
//    }
//
//    {
//        char buf[] = { 's', 't', 'r' };
//        QKnxByteArray str = QKnxByteArray::fromRawData(buf, 3);
//        str[0] = 'S';
//
//        QCOMPARE(buf[0], char('s'));
//    }
//
//    {
//        static const char buf[] = { 's', 't', 'r' };
//        QKnxByteArray str = QKnxByteArray::fromRawData(buf, 3);
//
//        // this causes a crash in most systems if the detaching doesn't work
//        str[0] = 'S';
//
//        QCOMPARE(buf[0], char('s'));
//    }
//}
//
//void tst_QKnxByteArray::reserve()
//{
//    int capacity = 100;
//    QKnxByteArray qba;
//    qba.reserve(capacity);
//    QVERIFY(qba.capacity() == capacity);
//    char *data = qba.data();
//
//    for (int i = 0; i < capacity; i++) {
//        qba.resize(i);
//        QVERIFY(capacity == qba.capacity());
//        QVERIFY(data == qba.data());
//    }
//
//    qba.resize(capacity);
//
//    QKnxByteArray copy = qba;
//    qba.reserve(capacity / 2);
//    QCOMPARE(qba.size(), capacity); // we didn't shrink the size!
//    QCOMPARE(qba.capacity(), capacity);
//    QCOMPARE(copy.capacity(), capacity);
//
//    qba = copy;
//    qba.reserve(capacity * 2);
//    QCOMPARE(qba.size(), capacity);
//    QCOMPARE(qba.capacity(), capacity * 2);
//    QCOMPARE(copy.capacity(), capacity);
//    QVERIFY(qba.constData() != data);
//
//    QKnxByteArray nil1, nil2;
//    nil1.reserve(0);
//    nil2.squeeze();
//    nil1.squeeze();
//    nil2.reserve(0);
//}
//
//void tst_QKnxByteArray::reserveExtended_data()
//{
//    prependExtended_data();
//}
//
//void tst_QKnxByteArray::reserveExtended()
//{
//    QFETCH(QKnxByteArray, array);
//    array.reserve(1024);
//    QVERIFY(array.capacity() == 1024);
//    QCOMPARE(array, QKnxByteArray("data"));
//    array.squeeze();
//    QCOMPARE(array, QKnxByteArray("data"));
//    QCOMPARE(array.capacity(), array.size());
//}
//
//void tst_QKnxByteArray::movablity_data()
//{
//    QTest::addColumn<QKnxByteArray>("array");
//
//    QTest::newRow("0x00000000") << QKnxByteArray("\x00\x00\x00\x00", 4);
//    QTest::newRow("0x000000ff") << QKnxByteArray("\x00\x00\x00\xff", 4);
//    QTest::newRow("0xffffffff") << QKnxByteArray("\xff\xff\xff\xff", 4);
//    QTest::newRow("empty") << QKnxByteArray("");
//    QTest::newRow("null") << QKnxByteArray();
//    QTest::newRow("sss") << QKnxByteArray(3, 's');
//
//    prependExtended_data();
//}
//
//void tst_QKnxByteArray::movablity()
//{
//    QFETCH(QKnxByteArray, array);
//
//    QVERIFY(!QTypeInfo<QKnxByteArray>::isStatic);
//
//    const int size = array.size();
//    const bool isEmpty = array.isEmpty();
//    const bool isNull = array.isNull();
//
//    QKnxByteArray memSpace;
//
//    // we need only memory space not the instance
//    memSpace.~QKnxByteArray();
//    // move array -> memSpace
//    memcpy(&memSpace, &array, sizeof(QKnxByteArray));
//    // reconstruct empty QKnxByteArray
//    new (&array) QKnxByteArray;
//
//    QCOMPARE(memSpace.size(), size);
//    QCOMPARE(memSpace.isEmpty(), isEmpty);
//    QCOMPARE(memSpace.isNull(), isNull);
//    QCOMPARE(memSpace.capacity(), capacity);
//
//    // try to not crash
//    memSpace.toLower();
//    memSpace.toUpper();
//    memSpace.prepend('a');
//    memSpace.append("b", 1);
//    memSpace.squeeze();
//    memSpace.reserve(array.size() + 16);
//
//    QKnxByteArray copy(memSpace);
//
//    // reinitialize base values
//    const int newSize = size + 2;
//    const bool newIsEmpty = false;
//    const bool newIsNull = false;
//    const int newCapacity = 16;
//
//    // move back memSpace -> array
//    array.~QKnxByteArray();
//    memcpy(&array, &memSpace, sizeof(QKnxByteArray));
//    // reconstruct empty QKnxByteArray
//    new (&memSpace) QKnxByteArray;
//
//    QCOMPARE(array.size(), newSize);
//    QCOMPARE(array.isEmpty(), newIsEmpty);
//    QCOMPARE(array.isNull(), newIsNull);
//    QCOMPARE(array.capacity(), newCapacity);
//    QVERIFY(array.startsWith('a'));
//    QVERIFY(array.endsWith('b'));
//
//    QCOMPARE(copy.size(), newSize);
//    QCOMPARE(copy.isEmpty(), newIsEmpty);
//    QCOMPARE(copy.isNull(), newIsNull);
//    QCOMPARE(copy.capacity(), newCapacity);
//    QVERIFY(copy.startsWith('a'));
//    QVERIFY(copy.endsWith('b'));
//
//    // try to not crash
//    array.squeeze();
//    array.reserve(array.size() + 3);
//    QVERIFY(true);
//}

QTEST_MAIN(tst_QKnxByteArray)
#include "tst_qknxbytearray.moc"
