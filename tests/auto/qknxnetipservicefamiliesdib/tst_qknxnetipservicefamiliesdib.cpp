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
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpServiceFamiliesDib : public QObject
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

void tst_QKnxNetIpServiceFamiliesDib::testDefaultConstructor()
{
    QKnxNetIpServiceFamiliesDib serviceFamiliesDib;
    QCOMPARE(serviceFamiliesDib.isValid(), true);
    QCOMPARE(serviceFamiliesDib.size(), quint16(2));
    QCOMPARE(serviceFamiliesDib.bytes(), QKnxByteArray::fromHex("0202"));
    QCOMPARE(serviceFamiliesDib.data().size(), quint16(0));
    QCOMPARE(serviceFamiliesDib.data(), QKnxByteArray {});
    QCOMPARE(serviceFamiliesDib.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);
}

void tst_QKnxNetIpServiceFamiliesDib::testConstructorWithOneArgument()
{
    QKnxNetIpServiceFamiliesDib::ServiceFamilyIdVersions families;
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core, 9);
    families.insertMulti(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement, 1);
    families.insertMulti(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement, 2);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling, 11);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpRouting, 12);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteLogging, 13);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteConfigAndDiagnosis, 14);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::ObjectServer, 15);
    QKnxNetIpServiceFamiliesDib serviceFamiliesDib(families);

    QCOMPARE(serviceFamiliesDib.isValid(), true);
    QCOMPARE(serviceFamiliesDib.size(), quint16(18));
    QCOMPARE(serviceFamiliesDib.bytes(),
        QKnxByteArray::fromHex("1202020903010302040B050C060D070E080F"));
    QCOMPARE(serviceFamiliesDib.data().size(), quint16(16));
    QCOMPARE(serviceFamiliesDib.data(), QKnxByteArray::fromHex("020903010302040B050C060D070E080F"));
    QCOMPARE(serviceFamiliesDib.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);
}

void tst_QKnxNetIpServiceFamiliesDib::testConstructorWithTwoArguments()
{
    QKnxNetIpServiceFamiliesDib families(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core, 10);

    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(4));
    QCOMPARE(families.bytes(), QKnxByteArray::fromHex("0402020A"));
    QCOMPARE(families.size(), quint16(4));
    QCOMPARE(families.data(), QKnxByteArray::fromHex("020A"));
    QCOMPARE(families.descriptionType(), QKnxNetIp::DescriptionType::SupportedServiceFamilies);
}

void tst_QKnxNetIpServiceFamiliesDib::testAddFunctions()
{
    QKnxNetIpServiceFamiliesDib families(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core, 9);
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(04));
    QCOMPARE(families.bytes(), QKnxByteArray::fromHex("04020209"));
    QCOMPARE(families.data().size(), quint16(2));
    QCOMPARE(families.data(), QKnxByteArray::fromHex("0209"));
    QCOMPARE(families.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);

    families.add(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement, 10);
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(6));
    QCOMPARE(families.bytes(), QKnxByteArray::fromHex("06020209030A"));
    QCOMPARE(families.data().size(), quint16(4));
    QCOMPARE(families.data(), QKnxByteArray::fromHex("0209030A"));
    QCOMPARE(families.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);

    QKnxNetIpServiceFamiliesDib::ServiceFamilyIdVersions familiesMap;
    familiesMap.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling, 11);
    families.add(familiesMap);
    QCOMPARE(families.isValid(), true);
    QCOMPARE(families.size(), quint16(8));
    QCOMPARE(families.bytes(), QKnxByteArray::fromHex("08020209030A040B"));
    QCOMPARE(families.data().size(), quint16(6));
    QCOMPARE(families.data(), QKnxByteArray::fromHex("0209030A040B"));
    QCOMPARE(families.descriptionType(),
        QKnxNetIp::DescriptionType::SupportedServiceFamilies);
}

void tst_QKnxNetIpServiceFamiliesDib::testGetFunction()
{
    QKnxNetIpServiceFamiliesDib::ServiceFamilyIdVersions families;
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::ObjectServer, 15);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core, 9);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement, 10);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling, 11);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpRouting, 12);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteLogging, 13);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteConfigAndDiagnosis, 14);
    QKnxNetIpServiceFamiliesDib serviceFamiliesDib(families);

    auto map = serviceFamiliesDib.serviceFamilyIdVersions();
    auto it = std::begin(map);
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core);
    QCOMPARE(it.value(),quint8(9));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement);
    QCOMPARE(it.value(),quint8(10));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling);
    QCOMPARE(it.value(),quint8(11));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpRouting);
    QCOMPARE(it.value(),quint8(12));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteLogging);
    QCOMPARE(it.value(),quint8(13));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteConfigAndDiagnosis);
    QCOMPARE(it.value(),quint8(14));
    ++it;
    QCOMPARE(it.key(),QKnxNetIpServiceFamiliesDib::ServiceFamilieId::ObjectServer);
    QCOMPARE(it.value(),quint8(15));
}

void tst_QKnxNetIpServiceFamiliesDib::testDebugStream()
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

    qDebug() << QKnxNetIpServiceFamiliesDib();
    QCOMPARE(s_msg, QString::fromLatin1("0x0202"));

    QKnxNetIpServiceFamiliesDib::ServiceFamilyIdVersions families;
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core, 9);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::DeviceManagement, 10);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpTunneling, 11);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::IpRouting, 12);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteLogging, 13);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::RemoteConfigAndDiagnosis, 14);
    families.insert(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::ObjectServer, 15);

    qDebug() << QKnxNetIpServiceFamiliesDib(families);
    QCOMPARE(s_msg, QString::fromLatin1("0x10020209030a040b050c060d070e080f"));

    qDebug() << QKnxNetIpServiceFamiliesDib(QKnxNetIpServiceFamiliesDib::ServiceFamilieId::Core, 10);
    QCOMPARE(s_msg, QString::fromLatin1("0x0402020a"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpServiceFamiliesDib)

#include "tst_qknxnetipservicefamiliesdib.moc"
