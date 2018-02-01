/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
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
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxgroupaddressinfos.h>
#include <QtKnx/qknxgroupaddressinfo.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxGroupAddressInfos : public QObject
{
    Q_OBJECT

private slots:
    void test_GroupAddressInfos();
    void test_GroupAddressInfosFromFile();
    void test_DebugStream();
};

void tst_QKnxGroupAddressInfos::test_GroupAddressInfos()
{
    QString id1 = "P-01-000-1";
    QString address1 = "1";
    QString name1 = "address1";
    QString description1 = "Do 1";
    QString id2 = "P-01-000-2";
    QString address2 = "2";
    QString name2 = "address2";
    QString description2 = "Do 2";
    QString dataPointType = "DPT-1-1";
    QString puid = "whatever";
    QKnxGroupAddressInfo info1(id1, address1, name1, description1, dataPointType, puid);
    QKnxGroupAddressInfo info2(id2, address2, name2, description2, dataPointType, puid);
    QKnxGroupAddressInfos groupAddressesInfo;
    QString home = QString::fromLatin1("Home");
    groupAddressesInfo.addEntry(info1, home);
    groupAddressesInfo.addEntry(info2, home);

    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).size(),
        groupAddressesInfo.datapointTypeGroupAddresses(home).size());

    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info1.groupAddress()).groupAddress(), info1.groupAddress());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info1.groupAddress()).id(), info1.id());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info1.groupAddress()).address(), info1.address());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info1.groupAddress()).name(), info1.name());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info1.groupAddress()).description(), info1.description());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info1.groupAddress()).dataPointNumber(), info1.dataPointNumber());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info1.groupAddress()).puid(), info1.puid());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info1.groupAddress()).dataPointType(), info1.dataPointType());

    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info2.groupAddress()).groupAddress(), info2.groupAddress());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info2.groupAddress()).id(), info2.id());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info2.groupAddress()).address(), info2.address());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info2.groupAddress()).name(), info2.name());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info2.groupAddress()).description(), info2.description());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info2.groupAddress()).dataPointNumber(), info2.dataPointNumber());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info2.groupAddress()).puid(), info2.puid());
    QCOMPARE(groupAddressesInfo.groupAddressesFullDescription(home).value(
        info2.groupAddress()).dataPointType(), info2.dataPointType());

    QCOMPARE(groupAddressesInfo.datapointTypeGroupAddresses(home).values(
        info2.dataPointType()).size(), 2);
    QCOMPARE(groupAddressesInfo.datapointTypeGroupAddresses(home).values(info2.dataPointType())[0],
        info2.groupAddress());
    QCOMPARE(groupAddressesInfo.datapointTypeGroupAddresses(home).values(info2.dataPointType())[1],
        info1.groupAddress());
}

void tst_QKnxGroupAddressInfos::test_GroupAddressInfosFromFile()
{
    QString fileName = QString::fromLatin1(":/data/0.xml");
    QString projectName = QString::fromLatin1("ProjectId");
    QKnxGroupAddressInfos infos(fileName, projectName);
    QString installation = QString::fromLatin1("Installation");

    QCOMPARE(infos.knxProjectFileName(), fileName);
    QCOMPARE(infos.knxProjectId(), projectName);

    QCOMPARE(infos.readFile(), QKnxGroupAddressInfos::ReadStatus::NoError);
    QCOMPARE(infos.installationsName().size(), 1);
    QCOMPARE(infos.groupAddressesFullDescription(installation).size(), 6);

    QKnxAddress groupAddress1 = { QKnxAddress::Type::Group, quint16(1) };
    QKnxAddress groupAddress2 = { QKnxAddress::Type::Group, quint16(2) };
    QKnxAddress groupAddress2048 = { QKnxAddress::Type::Group, quint16(2048) };
    QKnxAddress groupAddress2049 = { QKnxAddress::Type::Group, quint16(2049) };
    QKnxAddress groupAddress2304 = { QKnxAddress::Type::Group, quint16(2304) };
    QKnxAddress groupAddress2305 = { QKnxAddress::Type::Group, quint16(2305) };

    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress1).groupAddress(),
        groupAddress1);
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress1).id(),
        QLatin1String("P-050F-0_GA-355"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress1).address(),
        groupAddress1.toString());
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress1).name(),
        QLatin1String("Name"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress1).description(),
        QLatin1String("Description"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress1).dataPointNumber(),
        QLatin1String("DPST-1-1"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress1).puid(),
        QLatin1String("436"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress1).dataPointType(),
        QKnxDatapointType::Type::DptSwitch);

    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2).groupAddress(),
        groupAddress2);
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2).id(),
        QLatin1String("P-050F-0_GA-356"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2).address(),
        groupAddress2.toString());
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2).name(),
        QLatin1String("Name2"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2).description(),
        QLatin1String("Description2"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2).dataPointNumber(),
        QLatin1String("DPST-20-102"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2).puid(),
        QLatin1String("437"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2).dataPointType(),
        QKnxDatapointType::Type(2000102));

    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2048).groupAddress(),
        groupAddress2048);
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2048).id(),
        QLatin1String("P-050F-0_GA-261"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2048).address(),
        groupAddress2048.toString());
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2048).name(),
        QLatin1String("Name3"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2048).description(),
        QLatin1String("Description3"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2048).dataPointNumber(),
        QLatin1String("DPST-1-1"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2048).puid(),
        QLatin1String("342"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2048).dataPointType(),
        QKnxDatapointType::Type::DptSwitch);

    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2049).groupAddress(),
        groupAddress2049);
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2049).id(),
        QLatin1String("P-050F-0_GA-262"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2049).address(),
        groupAddress2049.toString());
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2049).name(),
        QLatin1String("Name4"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2049).description(),
        QLatin1String("Description4"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2049).dataPointNumber(),
        QLatin1String("DPST-1-1"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2049).puid(),
        QLatin1String("343"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2049).dataPointType(),
        QKnxDatapointType::Type::DptSwitch);

    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2304).groupAddress(),
        groupAddress2304);
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2304).id(),
        QLatin1String("P-050F-0_GA-358"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2304).address(),
        groupAddress2304.toString());
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2304).name(),
        QLatin1String("Name5"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2304).description(),
        QLatin1String("Description5"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2304).dataPointNumber(),
        QLatin1String("DPST-1-1"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2304).puid(),
        QLatin1String("439"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2304).dataPointType(),
        QKnxDatapointType::Type::DptSwitch);

    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2305).groupAddress(),
        groupAddress2305);
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2305).id(),
        QLatin1String("P-050F-0_GA-359"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2305).address(),
        groupAddress2305.toString());
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2305).name(),
        QLatin1String("Name6"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2305).description(),
        QLatin1String("Description6"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2305).dataPointNumber(),
        QLatin1String(""));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2305).puid(),
        QLatin1String("440"));
    QCOMPARE(infos.groupAddressesFullDescription(installation).value(groupAddress2305).dataPointType(),
        QKnxDatapointType::Type::Unknown);

    QCOMPARE(infos.datapointTypeGroupAddresses(installation).values(
        QKnxDatapointType::Type::DptSwitch).size(), 4);
}

void tst_QKnxGroupAddressInfos::test_DebugStream()
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

    QString id1 = "P-01-000-1";
    QString address1 = "1";
    QString name1 = "address1";
    QString description1 = "Do 1";
    QString dataPointType = "DPT-1-1";
    QString puid = "whatever";

    QKnxGroupAddressInfo info1(id1, address1, name1, description1, dataPointType, puid);

    QString home = QString::fromLatin1("Home");
    QKnxGroupAddressInfos groupAddressesInfo;
    groupAddressesInfo.addEntry(info1, home);

    qDebug() << groupAddressesInfo;
    QCOMPARE(s_msg, QString::fromLatin1("{Installation: Home}, {Group Address: 0x0001}, {Description: Do 1}, "
        "{DPT Type: 100001}"));
}


QTEST_MAIN(tst_QKnxGroupAddressInfos)

#include "tst_qknxgroupaddressinfo.moc"
