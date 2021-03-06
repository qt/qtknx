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
#include <QtKnx/qknxnetiphpai.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpStruct : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor()
    {
        QKnxNetIpHpai test;
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
        QKnxNetIpHpai test(QKnxNetIp::HostProtocol::Unknown);
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

        test = QKnxNetIpHpai(QKnxNetIp::HostProtocol::UDP_IPv4);
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

        QKnxNetIpHpai test(QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>(QKnxNetIp::HostProtocol::Unknown), payload);
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

        test = QKnxNetIpHpai(header, payload);
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

        auto header = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>(QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(header.isNull(), false);
        QCOMPARE(header.isValid(), true);
        QCOMPARE(header.size(), 0x02);
        QCOMPARE(header.totalSize(), 0x02);
        QCOMPARE(header.dataSize(), 0x00);
        QCOMPARE(header.code(), QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(header.byte(0), 0x02);
        QCOMPARE(header.bytes(), QKnxByteArray({ 0x02, 0x02 }));

        header.setCode(QKnxNetIp::HostProtocol::UDP_IPv4);
        QCOMPARE(header.code(), QKnxNetIp::HostProtocol::UDP_IPv4);

        header.setDataSize(payload.size());
        QCOMPARE(header.dataSize(), 0xfc);

        auto other = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>(QKnxNetIp::HostProtocol::TCP_IPv4);
        QCOMPARE(header != other, true);
        QCOMPARE(header == other, false);

        QKnxNetIpHpai test(header, payload);
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

    void testSearchParameterTypeHeader()
    {
        auto header = QKnxNetIpStructHeader<QKnxNetIp::SearchParameterType>();
        QCOMPARE(header.isNull(), true);
        QCOMPARE(header.isValid(), false);
        QCOMPARE(header.size(), 0x00);
        QCOMPARE(header.totalSize(), 0x00);
        QCOMPARE(header.dataSize(), 0x00);
        QCOMPARE(header.code(), QKnxNetIp::SearchParameterType::Unknown);
        QCOMPARE(header.isMandatory(), false);
        QCOMPARE(header.bytes(), QKnxByteArray());

        auto other = QKnxNetIpStructHeader<QKnxNetIp::SearchParameterType>
                                                     (QKnxNetIp::SearchParameterType::RequestDIBs);
        QCOMPARE(header != other, true);
        QCOMPARE(header == other, false);

        header.setCode(QKnxNetIp::SearchParameterType::RequestDIBs);
        QCOMPARE(header.isMandatory(), false);
        QCOMPARE(header.code(), QKnxNetIp::SearchParameterType::RequestDIBs);

        header.setMandatory(true);
        QCOMPARE(header.isMandatory(), true);
        QCOMPARE(header.code(), QKnxNetIp::SearchParameterType::RequestDIBs);

        header.setCode(QKnxNetIp::SearchParameterType::SelectByMACAddress);
        QCOMPARE(header.isMandatory(), true);
        QCOMPARE(header.code(), QKnxNetIp::SearchParameterType::SelectByMACAddress);

        header = QKnxNetIpStructHeader<QKnxNetIp::SearchParameterType>();
        QCOMPARE(header.size(), 0x00);
        QCOMPARE(header.totalSize(), 0x00);
        QCOMPARE(header.isMandatory(), false);

        header.setMandatory(true);
        QCOMPARE(header.size(), 0x02);
        QCOMPARE(header.totalSize(), 0x02);
        QCOMPARE(header.isMandatory(), true);

        header.setCode(QKnxNetIp::SearchParameterType::SelectByMACAddress);
        QCOMPARE(header.isMandatory(), true);
        QCOMPARE(header.code(), QKnxNetIp::SearchParameterType::SelectByMACAddress);

        header = { QKnxNetIp::SearchParameterType::RequestDIBs, 2, true};
        QCOMPARE(header.isMandatory(), true);
        QCOMPARE(header.code(), QKnxNetIp::SearchParameterType::RequestDIBs);
    }

    void structHeaderWithMandatoryFlag()
    {
        auto initBytes = QKnxByteArray::fromHex("0882");
        auto srp1 = QKnxNetIpStructHeader<QKnxNetIp::SearchParameterType>::fromBytes(
                        initBytes, 0);
        QVERIFY(srp1.isValid());
        QCOMPARE(srp1.bytes(), initBytes);

        initBytes = QKnxByteArray::fromHex("0802");
        srp1 = QKnxNetIpStructHeader<QKnxNetIp::SearchParameterType>::fromBytes(
                        initBytes, 0);
        QVERIFY(srp1.isValid());
        QCOMPARE(srp1.bytes(), initBytes);
    }

    void testDebugStream()
    {
        struct DebugHandler
        {
            explicit DebugHandler(QtMessageHandler newMessageHandler)
                : oldMessageHandler(qInstallMessageHandler(newMessageHandler))
            {}
            ~DebugHandler()
            { qInstallMessageHandler(oldMessageHandler); }
            QtMessageHandler oldMessageHandler;
        } _(myMessageHandler);

        qDebug() <<  QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>();
        QCOMPARE(s_msg, QStringLiteral("0x"));

        QKnxByteArray payload(0xfc, 0x05);
        auto h = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>(QKnxNetIp::HostProtocol::UDP_IPv4,
            payload.size());

        qDebug() << h;
        QCOMPARE(s_msg, QStringLiteral("0xfe01"));

        h = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>(QKnxNetIp::HostProtocol::UDP_IPv4, 2);
        qDebug() << h;
        QCOMPARE(s_msg, QStringLiteral("0x0401"));

        h = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>(QKnxNetIp::HostProtocol::UDP_IPv4, 0xff);
        qDebug() << h;
        QCOMPARE(s_msg, QStringLiteral("0xff010301"));
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpStruct)

#include "tst_qknxnetipstructure.moc"
