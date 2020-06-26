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
#include <QtKnx/qknxnetipsearchrequest.h>
#include <QtKnx/qknxnetipsrp.h>
#include <QtKnx/QKnxServiceInfo>

char *toString(const QKnxByteArray &ba)
{
    using QTest::toString;
    return toString("QKnxByteArray(" + ba.toByteArray() + ')');
}

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpSearchRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testSearchRequestEndpoint();
    void testConstructor();
    void testDebugStream();
    void testExtendSearchRequest();
    void tst_createSrpFromBytes();
    void tst_srpBuilders();
};

void tst_QKnxNetIpSearchRequest::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpSearchRequestProxy search(frame);
    QCOMPARE(search.isValid(), false);
    QCOMPARE(search.discoveryEndpoint().isValid(), false);
}

void tst_QKnxNetIpSearchRequest::testConstructor()
{
    auto frame = QKnxNetIpSearchRequestProxy::builder()
        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
            .setHostAddress(QHostAddress::LocalHost)
            .setPort(3671).create()
        ).create();

    QKnxNetIpSearchRequestProxy search(frame);
    QCOMPARE(search.isValid(), true);
    QCOMPARE(frame.size(), quint16(14));
    QCOMPARE(frame.bytes(), QKnxByteArray::fromHex("06100201000e08017f0000010e57"));
    QCOMPARE(frame.data().size(), quint16(8));
    QCOMPARE(frame.data(), QKnxByteArray::fromHex("08017f0000010e57"));
    QCOMPARE(search.discoveryEndpoint().isValid(), true);
    QCOMPARE(search.discoveryEndpoint().bytes(), QKnxByteArray::fromHex("08017f0000010e57"));
}

void tst_QKnxNetIpSearchRequest::testSearchRequestEndpoint()
{
    auto frame = QKnxNetIpSearchRequestProxy::builder()
        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
            .setHostProtocol(QKnxNetIp::HostProtocol::TCP_IPv4)
            .setHostAddress(QHostAddress::LocalHost)
            .setPort(3671).create()
        ).create();

    QKnxNetIpSearchRequestProxy search(frame);
    QCOMPARE(search.isValid(), false);
}

void tst_QKnxNetIpSearchRequest::testDebugStream()
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

    qDebug() << QKnxNetIpSearchRequestProxy::builder().create();
    QCOMPARE(s_msg, QStringLiteral("0x061002010006"));

    qDebug() << QKnxNetIpSearchRequestProxy::builder()
        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
            .setHostAddress(QHostAddress::LocalHost)
            .setPort(3671).create()
        ).create();
    QCOMPARE(s_msg, QStringLiteral("0x06100201000e08017f0000010e57"));
}

void tst_QKnxNetIpSearchRequest::tst_srpBuilders()
{
    auto srp = QKnxNetIpSrpProxy::programmingModeBuilder().create();
    QCOMPARE(srp.code(), QKnxNetIp::SearchParameterType::SelectByProgrammingMode);
    QVERIFY(srp.header().isMandatory());
    QVERIFY(srp.header().isValid());
    QVERIFY(srp.isValid());

    {
        QKnxNetIpSrpProxy proxy(srp);
        QVERIFY(proxy.isValid());
        QVERIFY(proxy.isMandatory());
        QVERIFY(proxy.programmingModeOnly());
        QCOMPARE(proxy.searchParameterType(), QKnxNetIp::SearchParameterType::SelectByProgrammingMode);

        QCOMPARE(proxy.macAddress(), QKnxByteArray());
        QCOMPARE(proxy.serviceInfo(), QKnxServiceInfo());
        QCOMPARE(proxy.descriptionTypes(), QList<QKnxNetIp::DescriptionType>());
    }

    auto macAddress = QKnxByteArray::fromHex("4CCC6AE40000");
    srp = QKnxNetIpSrpProxy::macAddressBuilder()
        .setMac(macAddress)
        .create();
    QCOMPARE(srp.code(), QKnxNetIp::SearchParameterType::SelectByMACAddress);
    QVERIFY(srp.header().isMandatory());
    QVERIFY(srp.header().isValid());
    QVERIFY(srp.isValid());

    {
        QKnxNetIpSrpProxy proxy(srp);
        QVERIFY(proxy.isValid());
        QVERIFY(proxy.isMandatory());
        QCOMPARE(proxy.macAddress(), macAddress);
        QCOMPARE(proxy.searchParameterType(), QKnxNetIp::SearchParameterType::SelectByMACAddress);

        QVERIFY(!proxy.programmingModeOnly());
        QCOMPARE(proxy.serviceInfo(), QKnxServiceInfo());
        QCOMPARE(proxy.descriptionTypes(), QList<QKnxNetIp::DescriptionType>());
    }

    QKnxServiceInfo serviceInfo { QKnxNetIp::ServiceFamily::ObjectServer, 0x04 };
    srp = QKnxNetIpSrpProxy::supportedFamilyBuilder()
        .setServiceInfo(serviceInfo)
        .create();
    QCOMPARE(srp.code(), QKnxNetIp::SearchParameterType::SelectByService);
    QVERIFY(srp.header().isMandatory());
    QVERIFY(srp.header().isValid());
    QVERIFY(srp.isValid());

    {
        QKnxNetIpSrpProxy proxy(srp);
        QVERIFY(proxy.isValid());
        QVERIFY(proxy.isMandatory());
        QCOMPARE(proxy.serviceInfo(), serviceInfo);
        QCOMPARE(proxy.searchParameterType(), QKnxNetIp::SearchParameterType::SelectByService);

        QVERIFY(!proxy.programmingModeOnly());
        QCOMPARE(proxy.macAddress(), QKnxByteArray());
        QCOMPARE(proxy.descriptionTypes(), QList<QKnxNetIp::DescriptionType>());
    }

    QList<QKnxNetIp::DescriptionType> types {
        QKnxNetIp::DescriptionType::DeviceInfo,
        QKnxNetIp::DescriptionType::SupportedServiceFamilies,
        QKnxNetIp::DescriptionType::ExtendedDeviceInfo
    };

    srp = QKnxNetIpSrpProxy::requestDibsBuilder()
        .setDescriptionTypes(types)
        .create();
    QCOMPARE(srp.code(), QKnxNetIp::SearchParameterType::RequestDIBs);
    QVERIFY(srp.header().isMandatory());
    QVERIFY(srp.header().isValid());
    QVERIFY(srp.isValid());

    types.append(QKnxNetIp::DescriptionType::Unknown);
    {
        QKnxNetIpSrpProxy proxy(srp);
        QVERIFY(proxy.isValid());
        QVERIFY(proxy.isMandatory());
        QCOMPARE(proxy.descriptionTypes(), types);
        QCOMPARE(proxy.searchParameterType(), QKnxNetIp::SearchParameterType::RequestDIBs);

        QVERIFY(!proxy.programmingModeOnly());
        QCOMPARE(proxy.macAddress(), QKnxByteArray());
        QCOMPARE(proxy.serviceInfo(), QKnxServiceInfo());
    }
}

void tst_QKnxNetIpSearchRequest::tst_createSrpFromBytes()
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

void tst_QKnxNetIpSearchRequest::testExtendSearchRequest()
{
    auto macAddress1 = QKnxByteArray::fromHex("4CCC6AE40001");
    auto macAddress2 = QKnxByteArray::fromHex("4CCC6AE40002");
    QKnxNetIpSrp macSrp1 = QKnxNetIpSrpProxy::macAddressBuilder()
                                    .setMac(macAddress1)
                                    .create();
    QKnxNetIpSrp macSrp2 = QKnxNetIpSrpProxy::macAddressBuilder()
                                    .setMac(macAddress2)
                                    .create();
    QList<QKnxNetIpSrp> srps = { macSrp1, macSrp2 };
    QCOMPARE(srps.constFirst().isValid(), true);
    QVERIFY(srps.constFirst().header().isMandatory());
    QCOMPARE(srps.constFirst().bytes(),
             QKnxByteArray::fromHex("0882") + macAddress1);
    QCOMPARE(srps.constLast().isValid(), true);
    QVERIFY(srps.constLast().header().isMandatory());
    QCOMPARE(srps.constLast().bytes(),
             QKnxByteArray::fromHex("0882") + macAddress2);

    auto frame = QKnxNetIpSearchRequestProxy::extendedBuilder()
        .setDiscoveryEndpoint(QKnxNetIpHpaiProxy::builder()
            .setHostAddress(QHostAddress::LocalHost)
            .setPort(3671).create())
        .setExtendedParameters(srps)
        .create();

    // header checks
    QVERIFY(frame.header().HeaderSize10 == frame.header().byte(0));
    QCOMPARE(frame.header().serviceType(),
             QKnxNetIp::ServiceType::ExtendedSearchRequest);
    const quint16 srpSize = 8*2;
    const quint16 knxNetIPheaderSize = 14;
    QVERIFY(frame.header().totalSize() == (knxNetIPheaderSize + srpSize));

    // frame checks
    QCOMPARE(frame.size(), quint16(knxNetIPheaderSize + srpSize));

    QCOMPARE(frame.bytes(),
        QKnxByteArray::fromHex("0610020b001e08017f0000010e5708824ccc6ae4000108824ccc6ae40002"));
    const quint16 hpaiSize = 8;
    QCOMPARE(frame.data().size(), hpaiSize + srpSize);
    QCOMPARE(frame.data(),
             QKnxByteArray::fromHex("08017f0000010e5708824ccc6ae4000108824ccc6ae40002"));

    // extended search proxy checks
    QKnxNetIpSearchRequestProxy extendedSearch(frame);
    QVERIFY(extendedSearch.isExtended());
    QCOMPARE(extendedSearch.isValid(), true);
    QCOMPARE(extendedSearch.discoveryEndpoint().isValid(), true);
    QCOMPARE(extendedSearch.discoveryEndpoint().bytes(),
             QKnxByteArray::fromHex("08017f0000010e57"));

    QKnxByteArray bytes;
    for (const auto &srp: extendedSearch.extendedSearchParameters())
        bytes += srp.bytes();
    QCOMPARE(bytes, QKnxByteArray::fromHex("0882") + macAddress1
             + QKnxByteArray::fromHex("0882") + macAddress2);
}


QTEST_APPLESS_MAIN(tst_QKnxNetIpSearchRequest)

#include "tst_qknxnetipsearchrequest.moc"
