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
    void testConstructor();
    void testDebugStream();
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
    using namespace SrpBuilders;
    auto srpMode = ProgrammingMode()
                   .setMandatory()
                   .create();
    QCOMPARE(srpMode.code(),
             QKnxNetIp::SearchParameterType::SelectByProgrammingMode);
    QVERIFY(srpMode.header().isMandatory());
    QVERIFY(srpMode.header().isValid());
    QVERIFY(srpMode.isValid());

    auto macAddress = QKnxByteArray::fromHex("4CCC6AE40000");
    auto srpMac = MacAddress()
                  .setMandatory()
                  .setMac(macAddress)
                  .create();
    QCOMPARE(srpMac.code(),
             QKnxNetIp::SearchParameterType::SelectByMACAddress);
    QVERIFY(srpMac.header().isMandatory());
    QCOMPARE(srpMac.constData().mid(0,6), macAddress);
    QVERIFY(srpMac.isValid());

    auto serviceFamilyId = QKnxNetIp::ServiceFamily::ObjectServer;
    auto minVersion = 4;
    auto srpSupportedFamily = SupportedFamily()
                              .setMandatory()
                              .setServiceFamilyId(serviceFamilyId)
                              .setMinimumVersion(minVersion)
                              .create();
    QCOMPARE(srpSupportedFamily.code(),
             QKnxNetIp::SearchParameterType::SelectByServiceSRP);
    QVERIFY(srpSupportedFamily.header().isMandatory());
    QCOMPARE(srpSupportedFamily.constData().at(0), quint8(serviceFamilyId));
    QCOMPARE(srpSupportedFamily.constData().at(1), minVersion);
    QVERIFY(srpSupportedFamily.isValid());

    QVector<QKnxServiceInfo> families;
    families.append({ QKnxNetIp::ServiceFamily::Core, 9 });
    families.append({ QKnxNetIp::ServiceFamily::DeviceManagement, 10 });
    families.append({ QKnxNetIp::ServiceFamily::IpTunneling, 11 });
    QKnxNetIpSrp srpDibs = RequestDibs()
                   .setMandatory()
                   .setServiceInfos(families)
                   .create();
    QCOMPARE(srpDibs.code(),
             QKnxNetIp::SearchParameterType::RequestDIBs);
    QVERIFY(srpDibs.header().isMandatory());
    QVERIFY(srpDibs.isValid());

    QKnxNetIpDib dib(QKnxNetIp::DescriptionType::SupportedServiceFamilies,srpDibs.data());
    QKnxNetIpServiceFamiliesDibProxy dibView(dib);
    auto serviceInfos = dibView.serviceInfos();
    for (int i = 0; i < serviceInfos.size(); ++i) {
        QCOMPARE(serviceInfos[i].ServiceFamily, families[i].ServiceFamily);
        QCOMPARE(serviceInfos[i].ServiceFamilyVersion, families[i].ServiceFamilyVersion);
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

QTEST_APPLESS_MAIN(tst_QKnxNetIpSearchRequest)

#include "tst_qknxnetipsearchrequest.moc"
