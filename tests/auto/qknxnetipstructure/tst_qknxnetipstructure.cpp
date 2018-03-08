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
#include <QtKnx/qknxnetipstruct.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class TestStructure : private QKnxNetIpHostProtocolStruct
{
public:
    TestStructure() = default;
    explicit TestStructure(QKnxNetIp::HostProtocol code)
        : QKnxNetIpHostProtocolStruct(code)
    {}
    TestStructure(const QKnxNetIpStructHeader<QKnxNetIp::HostProtocol> &header, const QKnxByteArray &payload)
        : QKnxNetIpHostProtocolStruct(header, payload)
    {}

    using QKnxNetIpHostProtocolStruct::code;
    using QKnxNetIpHostProtocolStruct::setCode;

    using QKnxNetIpHostProtocolStruct::size;
    using QKnxNetIpHostProtocolStruct::header;

    using QKnxNetIpHostProtocolStruct::data;
    using QKnxNetIpHostProtocolStruct::setData;

    using QKnxNetIpHostProtocolStruct::isValid;

    using QKnxNetIpHostProtocolStruct::bytes;

    // TODO: The following function is not used yet...
    template<typename T> static TestStructure fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index, QKnxNetIp::HostProtocol::Unknown);
    }

private:
    TestStructure(const QKnxNetIpHostProtocolStruct &other)
        : QKnxNetIpHostProtocolStruct(other)
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
        QCOMPARE(test.code(), QKnxNetIp::HostProtocol(0));
        QCOMPARE(test.size(), quint16(0));
        QCOMPARE(test.bytes(), QKnxByteArray {});

        auto header = test.header();
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.code(), QKnxNetIp::HostProtocol(0));
        QCOMPARE(header.size(), quint16(0));
        QCOMPARE(header.totalSize(), quint16(0));
        QCOMPARE(header.dataSize(), quint16(0));
        QCOMPARE(header.bytes(), QKnxByteArray {});

        QCOMPARE(test.data().size(), quint16(0));
        QCOMPARE(test.data(), QKnxByteArray {});
    }

    void testConstructor_Code()
    {
        TestStructure test(QKnxNetIp::HostProtocol::Unknown);
        QCOMPARE(test.isValid(), false);
        QCOMPARE(test.code(), QKnxNetIp::HostProtocol(0));
        QCOMPARE(test.size(), quint16(2));
        QCOMPARE(test.bytes(),  QKnxByteArray({ 0x02, 0x00 }));

        auto header = test.header();
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.code(), QKnxNetIp::HostProtocol(0));
        QCOMPARE(header.size(), quint16(2));
        QCOMPARE(header.totalSize(), quint16(2));
        QCOMPARE(header.dataSize(), quint16(0));
        QCOMPARE(header.bytes(), QKnxByteArray({ 0x02, 0x00 }));

        QCOMPARE(test.data().size(), quint16(0));
        QCOMPARE(test.data(), QKnxByteArray {});

        test = TestStructure(QKnxNetIp::HostProtocol::UDP_IPv4);
        QCOMPARE(test.isValid(), true);
        QCOMPARE(test.code(), QKnxNetIp::HostProtocol(0x01));
        QCOMPARE(test.size(), quint16(2));
        QCOMPARE(test.bytes(), QKnxByteArray({ 0x02, 0x01 }));

        header = test.header();
        QCOMPARE(header.isValid(), true);
        QCOMPARE(header.code(), QKnxNetIp::HostProtocol(0x01));
        QCOMPARE(header.size(), quint16(2));
        QCOMPARE(header.totalSize(), quint16(2));
        QCOMPARE(header.dataSize(), quint16(0));
        QCOMPARE(header.bytes(), QKnxByteArray({ 0x02, 0x01 }));

        QCOMPARE(test.data().size(), quint16(0));
        QCOMPARE(test.data(), QKnxByteArray {});
    }

    void testConstructor_HeaderPayload()
    {
        QKnxByteArray payload;

        TestStructure test(QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>(QKnxNetIp::HostProtocol::Unknown), payload);
        QCOMPARE(test.isValid(), false);
        QCOMPARE(test.code(), QKnxNetIp::HostProtocol(0));
        QCOMPARE(test.size(), quint16(2));
        QCOMPARE(test.bytes(), QKnxByteArray({ 0x02, 0x00 }));

        auto header = test.header();
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.code(), QKnxNetIp::HostProtocol(0));
        QCOMPARE(header.size(), quint16(2));
        QCOMPARE(header.totalSize(), quint16(2));
        QCOMPARE(header.dataSize(), quint16(0));
        QCOMPARE(header.bytes(), QKnxByteArray({ 0x02, 0x00 }));

        payload = test.data();
        QCOMPARE(payload.size(), quint16(0));
        QCOMPARE(payload, QKnxByteArray {});

        payload = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
        header = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>(QKnxNetIp::HostProtocol::UDP_IPv4);
        header.setDataSize(payload.size());

        test = TestStructure(header, payload);
        QCOMPARE(test.isValid(), true);
        QCOMPARE(test.code(), QKnxNetIp::HostProtocol(0x01));
        QCOMPARE(test.size(), quint16(0x08));
        QCOMPARE(test.bytes(), QKnxByteArray({ 0x08, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));

        header = test.header();
        QCOMPARE(header.isValid(), true);
        QCOMPARE(header.code(), QKnxNetIp::HostProtocol(0x01));
        QCOMPARE(header.size(), quint16(2));
        QCOMPARE(header.totalSize(), quint16(8));
        QCOMPARE(header.dataSize(), quint16(6));
        QCOMPARE(header.bytes(), QKnxByteArray({ 0x08, 0x01 }));

        payload = test.data();
        QCOMPARE(payload.size(), quint16(0x06));
        QCOMPARE(payload, QKnxByteArray({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }));
    }

    void testHeaderSize()
    {
        QKnxByteArray payload(0xfc, 0x05);

        auto header = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>(QKnxNetIp::HostProtocol::UDP_IPv4);
        header.setDataSize(payload.size());

        TestStructure test(header, payload);
        QCOMPARE(test.size(), quint16(0xfe));
        QCOMPARE(test.bytes(), QKnxByteArray({ 0xfe, 0x01 }) + payload);

        QCOMPARE(test.header().size(), quint16(0x02));
        QCOMPARE(test.header().bytes(), QKnxByteArray({ 0xfe, 0x01 }));

        QCOMPARE(test.data().size(), quint16(0xfc));
        QCOMPARE(test.data(), payload);

        payload += { 0x00, 0x11, 0x22, 0x33, 0x44, 0x66, 0x77, 0x88, 0x99 };
        test.setData(payload);

        QCOMPARE(test.size(), quint16(0x109));
        QCOMPARE(test.bytes(), QKnxByteArray({ 0xff, 0x01, 0x09, 0x01 }) + payload);

        QCOMPARE(test.header().size(), quint16(0x04));
        QCOMPARE(test.header().bytes(), QKnxByteArray({ 0xff, 0x01, 0x09, 0x01 }));

        QCOMPARE(test.data().size(), quint16(0x0105));
        QCOMPARE(test.data(), payload);
    }

    void testDebugStream()
    {
        // TODO: add
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpStruct)

#include "tst_qknxnetipstructure.moc"
