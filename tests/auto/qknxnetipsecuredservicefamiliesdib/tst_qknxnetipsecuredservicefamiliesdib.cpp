/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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
#include <QtKnx/qknxnetipsecuredservicefamiliesdib.h>
#include <QtTest/qtest.h>

QT_BEGIN_NAMESPACE

char *toString(const QKnxByteArray &ba)
{
    using QTest::toString;
    return toString("QKnxByteArray(" + ba.toByteArray() + ')');
}

QT_END_NAMESPACE

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpSecuredServiceFamiliesDib : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithOneArgument();
    void testConstructorWithTwoArguments();
    void testAddFunctions();
    void testGetFunction();
    void testDebugStream();
};

void tst_QKnxNetIpSecuredServiceFamiliesDib::testDefaultConstructor()
{
    auto serviceFamiliesDib = QKnxNetIpSecuredServiceFamiliesDibProxy::builder().create();
    QCOMPARE(serviceFamiliesDib.isValid(), true);
    QCOMPARE(serviceFamiliesDib.size(), quint16(2));
    QCOMPARE(serviceFamiliesDib.bytes(), QKnxByteArray::fromHex("0206"));
    QCOMPARE(serviceFamiliesDib.data().size(), quint16(0));
    QCOMPARE(serviceFamiliesDib.data(), QKnxByteArray {});

    QKnxNetIpSecuredServiceFamiliesDibProxy view(serviceFamiliesDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::SecuredServices);
}

void tst_QKnxNetIpSecuredServiceFamiliesDib::testConstructorWithOneArgument()
{
    QList<QKnxSecuredServiceInfo> infos;
    infos.append({ QKnxNetIp::ServiceFamily::Core, 9 });
    infos.append({ QKnxNetIp::ServiceFamily::DeviceManagement, 2 });
    infos.append({ QKnxNetIp::ServiceFamily::DeviceManagement, 1 });
    infos.append({ QKnxNetIp::ServiceFamily::IpTunneling, 11 });
    infos.append({ QKnxNetIp::ServiceFamily::IpRouting, 12 });
    infos.append({ QKnxNetIp::ServiceFamily::RemoteLogging, 13 });
    infos.append({ QKnxNetIp::ServiceFamily::RemoteLogging, 13 });
    infos.append({ QKnxNetIp::ServiceFamily::RemoteConfigAndDiagnosis, 14 });
    infos.append({ QKnxNetIp::ServiceFamily::ObjectServer, 15 });
    auto serviceFamiliesDib = QKnxNetIpSecuredServiceFamiliesDibProxy::Builder()
        .setServiceInfos(infos).create();

    QCOMPARE(serviceFamiliesDib.isValid(), true);
    QCOMPARE(serviceFamiliesDib.size(), quint16(18));
    QCOMPARE(serviceFamiliesDib.bytes(),
        QKnxByteArray::fromHex("1206020903010302040B050C060D070E080F"));
    QCOMPARE(serviceFamiliesDib.data().size(), quint16(16));
    QCOMPARE(serviceFamiliesDib.data(), QKnxByteArray::fromHex("020903010302040B050C060D070E080F"));

    QKnxNetIpSecuredServiceFamiliesDibProxy view(serviceFamiliesDib);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::SecuredServices);
}

void tst_QKnxNetIpSecuredServiceFamiliesDib::testConstructorWithTwoArguments()
{
    auto families = QKnxNetIpSecuredServiceFamiliesDibProxy::Builder()
        .setServiceInfos({ { QKnxNetIp::ServiceFamily::Core, 10 } }).create();

    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(4));
    QCOMPARE(families.bytes(), QKnxByteArray::fromHex("0406020A"));
    QCOMPARE(families.size(), quint16(4));
    QCOMPARE(families.data(), QKnxByteArray::fromHex("020A"));

    QKnxNetIpSecuredServiceFamiliesDibProxy view(families);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::SecuredServices);
}

void tst_QKnxNetIpSecuredServiceFamiliesDib::testAddFunctions()
{
    auto builder = QKnxNetIpSecuredServiceFamiliesDibProxy::Builder();
    auto families = QKnxNetIpSecuredServiceFamiliesDibProxy::Builder()
        .setServiceInfos({ { QKnxNetIp::ServiceFamily::Core, 9 } }).create();

    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(04));
    QCOMPARE(families.bytes(), QKnxByteArray::fromHex("04060209"));
    QCOMPARE(families.data().size(), quint16(2));
    QCOMPARE(families.data(), QKnxByteArray::fromHex("0209"));

    QKnxNetIpSecuredServiceFamiliesDibProxy view(families);
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.serviceInfos().count(), 1);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::SecuredServices);

    families = QKnxNetIpSecuredServiceFamiliesDibProxy::Builder()
        .setServiceInfos(view.serviceInfos() += { QKnxNetIp::ServiceFamily::DeviceManagement, 10 })
        .create();
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(6));
    QCOMPARE(families.bytes(), QKnxByteArray::fromHex("06060209030A"));
    QCOMPARE(families.data().size(), quint16(4));
    QCOMPARE(families.data(), QKnxByteArray::fromHex("0209030A"));

    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.serviceInfos().count(), 2);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::SecuredServices);

    families = QKnxNetIpSecuredServiceFamiliesDibProxy::Builder()
        .setServiceInfos(view.serviceInfos() += { QKnxNetIp::ServiceFamily::IpTunneling, 11 })
        .create();
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(8));
    QCOMPARE(families.bytes(), QKnxByteArray::fromHex("08060209030A040B"));
    QCOMPARE(families.data().size(), quint16(6));
    QCOMPARE(families.data(), QKnxByteArray::fromHex("0209030A040B"));

    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.serviceInfos().count(), 3);
    QCOMPARE(view.descriptionType(), QKnxNetIp::DescriptionType::SecuredServices);
}

void tst_QKnxNetIpSecuredServiceFamiliesDib::testGetFunction()
{
    auto serviceFamiliesDib = QKnxNetIpSecuredServiceFamiliesDibProxy::builder()
        .setServiceInfos({
            { QKnxNetIp::ServiceFamily::ObjectServer, 15 },
            { QKnxNetIp::ServiceFamily::Core, 9 },
            { QKnxNetIp::ServiceFamily::DeviceManagement, 10 },
            { QKnxNetIp::ServiceFamily::IpTunneling, 11 },
            { QKnxNetIp::ServiceFamily::IpRouting, 12 },
            { QKnxNetIp::ServiceFamily::RemoteLogging, 13 },
            { QKnxNetIp::ServiceFamily::RemoteConfigAndDiagnosis, 14 }
        }).create();

    QKnxNetIpSecuredServiceFamiliesDibProxy view(serviceFamiliesDib);
    QCOMPARE(serviceFamiliesDib.isValid(), true);
    QCOMPARE(view.serviceInfos().count(), 7);

    const auto infos = view.serviceInfos();
    QCOMPARE(infos.at(0).ServiceFamily, QKnxNetIp::ServiceFamily::Core);
    QCOMPARE(infos.at(0).RequiredSecurityVersion, quint8(9));

    QCOMPARE(infos.at(1).ServiceFamily, QKnxNetIp::ServiceFamily::DeviceManagement);
    QCOMPARE(infos.at(1).RequiredSecurityVersion, quint8(10));

    QCOMPARE(infos.at(2).ServiceFamily, QKnxNetIp::ServiceFamily::IpTunneling);
    QCOMPARE(infos.at(2).RequiredSecurityVersion, quint8(11));

    QCOMPARE(infos.at(3).ServiceFamily, QKnxNetIp::ServiceFamily::IpRouting);
    QCOMPARE(infos.at(3).RequiredSecurityVersion, quint8(12));

    QCOMPARE(infos.at(4).ServiceFamily, QKnxNetIp::ServiceFamily::RemoteLogging);
    QCOMPARE(infos.at(4).RequiredSecurityVersion, quint8(13));

    QCOMPARE(infos.at(5).ServiceFamily, QKnxNetIp::ServiceFamily::RemoteConfigAndDiagnosis);
    QCOMPARE(infos.at(5).RequiredSecurityVersion, quint8(14));

    QCOMPARE(infos.at(6).ServiceFamily, QKnxNetIp::ServiceFamily::ObjectServer);
    QCOMPARE(infos.at(6).RequiredSecurityVersion, quint8(15));
}

void tst_QKnxNetIpSecuredServiceFamiliesDib::testDebugStream()
{
    struct DebugHandler
    {
        explicit DebugHandler(QtMessageHandler newMessageHandler)
            : oldMessageHandler(qInstallMessageHandler(newMessageHandler))
        {}
        ~DebugHandler()
        {
            qInstallMessageHandler(oldMessageHandler);
        }
        QtMessageHandler oldMessageHandler;
    } _(myMessageHandler);

    qDebug() << QKnxNetIpSecuredServiceFamiliesDibProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0206"));

    QList<QKnxSecuredServiceInfo> families;
    families.append({ QKnxNetIp::ServiceFamily::Core, 9 });
    families.append({ QKnxNetIp::ServiceFamily::DeviceManagement, 10 });
    families.append({ QKnxNetIp::ServiceFamily::IpTunneling, 11 });
    families.append({ QKnxNetIp::ServiceFamily::IpRouting, 12 });
    families.append({ QKnxNetIp::ServiceFamily::RemoteLogging, 13 });
    families.append({ QKnxNetIp::ServiceFamily::RemoteConfigAndDiagnosis, 14 });
    families.append({ QKnxNetIp::ServiceFamily::ObjectServer, 15 });

    qDebug() << QKnxNetIpSecuredServiceFamiliesDibProxy::builder().setServiceInfos(families)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x10060209030a040b050c060d070e080f"));

    qDebug() << QKnxNetIpSecuredServiceFamiliesDibProxy::builder()
        .setServiceInfos({ { QKnxNetIp::ServiceFamily::Core, 10 } }).create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0406020a"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpSecuredServiceFamiliesDib)

#include "tst_qknxnetipsecuredservicefamiliesdib.moc"
