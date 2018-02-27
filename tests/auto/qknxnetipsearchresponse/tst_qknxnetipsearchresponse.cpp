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
#include <QtTest/qtest.h>
#include <QtKnx/qknxnetipsearchresponse.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpSearchResponse: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpSearchResponse::testDefaultConstructor()
{
    QKnxNetIpSearchResponse search;
    QCOMPARE(search.isValid(), false);
    QCOMPARE(search.controlEndpoint().isValid(), false);
    QCOMPARE(search.deviceHardware().isValid(), false);
    QCOMPARE(search.supportedFamilies().isValid(), false);
}

void tst_QKnxNetIpSearchResponse::testConstructor()
{
    QKnxNetIpHpai endpoint(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpDeviceDib hardware(QKnx::MediumType::NetIP,
                                QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
                                QKnxAddress::Individual::Unregistered,
                                0x1111,
                                QKnxByteArray::fromHex("123456123456"),
                                QHostAddress::AnyIPv4,
                                QKnxByteArray::fromHex("bcaec56690f9"),
                                QKnxByteArray("qt.io KNX device", 16));
    QKnxNetIpServiceFamiliesDib families(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core, 10);

    QKnxNetIpSearchResponse search(endpoint, hardware, families);

    QCOMPARE(search.isValid(), true);
    QCOMPARE(search.size(), quint16(72));
    QCOMPARE(search.bytes(), QKnxByteArray::fromHex("06100202004808017f0000010e57")
        + QKnxByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
        + QKnxByteArray::fromHex("71742e696f204b4e5820646576696365") // qt.io KNX device
        + QKnxByteArray::fromHex("0000000000000000000000000000")
        + QKnxByteArray::fromHex("0402020a"));
    QCOMPARE(search.payload().size(), quint16(66));
    QCOMPARE(search.payload().bytes(), QKnxByteArray::fromHex("08017f0000010e57")
        + QKnxByteArray::fromHex("36012001ffff111112345612345600000000bcaec56690f9")
        + QKnxByteArray::fromHex("71742e696f204b4e5820646576696365") // qt.io KNX device
        + QKnxByteArray::fromHex("0000000000000000000000000000")
        + QKnxByteArray::fromHex("0402020a"));
    QCOMPARE(search.toString(), QString::fromLatin1("Header size { 0x06 }, "
            "Version { 0x10 }, Service type { 0x202 }, Total size { 0x48 }, "
            "Bytes { 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57, 0x36, 0x01, "
            "0x20, 0x01, 0xff, 0xff, 0x11, 0x11, 0x12, 0x34, 0x56, 0x12, 0x34, 0x56, 0x00, "
            "0x00, 0x00, 0x00, 0xbc, 0xae, 0xc5, 0x66, 0x90, 0xf9, 0x71, 0x74, 0x2e, 0x69, 0x6f, "
            "0x20, 0x4b, 0x4e, 0x58, 0x20, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x00, 0x00, 0x00, "
            "0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x02, "
            "0x0a }"));

    QCOMPARE(search.controlEndpoint().isValid(), true);
    QCOMPARE(search.controlEndpoint().bytes(), QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(search.deviceHardware().isValid(), true);
    QCOMPARE(search.deviceHardware().bytes(),
        QKnxByteArray::fromHex("36012001FFFF111112345612345600000000BCAEC56690F9")
        + QKnxByteArray::fromHex("71742e696f204b4e5820646576696365") // qt.io KNX device
        + QKnxByteArray::fromHex("0000000000000000000000000000"));
    QCOMPARE(search.supportedFamilies().isValid(), true);
    QCOMPARE(search.supportedFamilies().bytes(), QKnxByteArray::fromHex("0402020a"));

    QCOMPARE(QKnxNetIpSearchResponse::fromBytes(search.bytes(), 0).isValid(),
        true);
    QCOMPARE(QKnxNetIpSearchResponse::fromBytes(search.bytes(), 56).isValid(),
        false);
    QCOMPARE(QKnxNetIpSearchResponse::fromBytes(search.bytes(), 2).isValid(),
        false);
}

void tst_QKnxNetIpSearchResponse::testDebugStream()
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

    qDebug() << QKnxNetIpSearchResponse();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    QKnxNetIpHpai endpoint(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpDeviceDib hardware(QKnx::MediumType::NetIP,
                                QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
                                QKnxAddress::Individual::Unregistered,
                                0x1111,
                                QKnxByteArray::fromHex("123456123456"),
                                QHostAddress::AnyIPv4,
                                QKnxByteArray::fromHex("bcaec56690f9"),
                                QKnxByteArray("qt.io KNX device", 16));
    QKnxNetIpServiceFamiliesDib families(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core, 10);
    qDebug() << QKnxNetIpSearchResponse(endpoint, hardware, families);
    QCOMPARE(s_msg, QString::fromLatin1("0x06100202004808017f0000010e5736012001ffff1111123456123456"
            "00000000bcaec56690f971742e696f204b4e582064657669636500000000000000000000000000000"
            "402020a"));
}

void tst_QKnxNetIpSearchResponse::testDataStream()
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    QKnxNetIpHpai endpoint(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress::LocalHost, 3671);
    QKnxNetIpDeviceDib hardware(QKnx::MediumType::NetIP,
                                QKnxNetIpDeviceDib::DeviceStatus::ActiveProgrammingMode,
                                QKnxAddress::Individual::Unregistered,
                                0x1111,
                                QKnxByteArray::fromHex("123456123456"),
                                QHostAddress::AnyIPv4,
                                QKnxByteArray::fromHex("bcaec56690f9"),
                                QKnxByteArray("qt.io KNX device", 16));
    QKnxNetIpServiceFamiliesDib families(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core, 10);
    out << QKnxNetIpSearchResponse(endpoint, hardware, families);
    QCOMPARE(byteArray, QByteArray::fromHex("06100202004808017f0000010e5736012001ffff111112345612"
            "345600000000bcaec56690f971742e696f204b4e582064657669636500000000000000000000000000000"
            "402020a"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpSearchResponse)

#include "tst_qknxnetipsearchresponse.moc"
