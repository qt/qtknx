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

#include <QtCore/qxmlstream.h>
#include <QtKnx/private/qknxprojectroot_p.h>
#include <QtTest/qtest.h>

class tst_QKnxProject : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testProjectInformation()
    {
        QFile file(":/data/projectinformation.xml");
        QCOMPARE(file.open(QIODevice::ReadOnly), true);

        QXmlStreamReader reader(&file);
        QCOMPARE(reader.readNextStartElement(), true);
        QCOMPARE(reader.name(), QLatin1String("KNX"));

        QKnxProjectRoot root;
        QCOMPARE(root.parseElement(&reader, true), true);

        QCOMPARE(root.CreatedBy, QLatin1String("ETS5"));
        QCOMPARE(root.ToolVersion, QLatin1String("5.5.952.23148"));
        QCOMPARE(root.Project.size(), 1);

        auto project = root.Project[0];
        QCOMPARE(project.Id, QLatin1String("ProjectId"));
        QCOMPARE(project.ProjectInformation.size(), 1);

        auto projectInfo = project.ProjectInformation[0];
        QCOMPARE(projectInfo.Name, QLatin1String("Name"));
        QCOMPARE(projectInfo.GroupAddressStyle, QLatin1String("ThreeLevel"));
        QCOMPARE(projectInfo.ProjectNumber, QLatin1String("123456789"));
        QCOMPARE(projectInfo.ContractNumber, QLatin1String("987654321"));
        QCOMPARE(projectInfo.LastModified,
            QDateTime::fromString(QLatin1String("2017-11-27T09:40:34.6799941Z"), Qt::ISODateWithMs));
        QCOMPARE(projectInfo.ProjectStart,
            QDateTime::fromString(QLatin1String("2017-11-01T00:00:00Z"), Qt::ISODateWithMs));
        QCOMPARE(projectInfo.ProjectEnd,
            QDateTime::fromString(QLatin1String("2017-11-30T00:00:00Z"), Qt::ISODateWithMs));
        QCOMPARE(projectInfo.ProjectId, quint16(123));
        QCOMPARE(projectInfo.ProjectPassword, QLatin1String("ProjectPassword"));
        QCOMPARE(projectInfo.Comment, QLatin1String("Comment"));
        QCOMPARE(projectInfo.CompletionStatus, QLatin1String("Editing"));
        QCOMPARE(projectInfo.ProjectTracingLevel, QLatin1String("OperationUsed"));
        QCOMPARE(projectInfo.ProjectTracingPassword, QLatin1String("TracingPassword"));
        QCOMPARE(projectInfo.Hide16BitGroupsFromLegacyPlugins, true);
        QCOMPARE(projectInfo.CodePage, QLatin1String("us-ascii"));
        QCOMPARE(projectInfo.BusAccessLegacyMode, true);
        QCOMPARE(projectInfo.Guid, QLatin1String("eae8eae4-34f3-433b-bb22-5acdba366889"));
        QCOMPARE(projectInfo.LastUsedPuid, 47);

        QCOMPARE(projectInfo.HistoryEntries.size(), 2);
        QCOMPARE(projectInfo.HistoryEntries[0].Date,
            QDateTime::fromString(QLatin1String("2017-11-23T15:02:26.2503409Z"), Qt::ISODateWithMs));
        QCOMPARE(projectInfo.HistoryEntries[0].User, QLatin1String("User"));
        QCOMPARE(projectInfo.HistoryEntries[0].Text, QLatin1String("Text"));
        QCOMPARE(projectInfo.HistoryEntries[0].Detail, QLatin1String("Detail"));
        QCOMPARE(projectInfo.HistoryEntries[1].Date,
            QDateTime::fromString(QLatin1String("2017-11-23T16:13:38.5091389Z"), Qt::ISODateWithMs));
        QCOMPARE(projectInfo.HistoryEntries[1].User, QLatin1String("User2"));
        QCOMPARE(projectInfo.HistoryEntries[1].Text, QLatin1String("Text2"));
        QCOMPARE(projectInfo.HistoryEntries[1].Detail, QLatin1String("Detail2"));

        QCOMPARE(projectInfo.ToDoItems.size(), 2);
        QCOMPARE(projectInfo.ToDoItems[0].Description, QLatin1String("Description"));
        QCOMPARE(projectInfo.ToDoItems[0].ObjectPath, QLatin1String("ObjectPath"));
        QCOMPARE(projectInfo.ToDoItems[0].Status, QLatin1String("Open"));
        QCOMPARE(projectInfo.ToDoItems[1].Description, QLatin1String("Description2"));
        QCOMPARE(projectInfo.ToDoItems[1].ObjectPath, QLatin1String("ObjectPath2"));
        QCOMPARE(projectInfo.ToDoItems[1].Status, QLatin1String("Accomplished"));

        QCOMPARE(projectInfo.ProjectTraces.size(), 2);
        QCOMPARE(projectInfo.ProjectTraces[0].Comment, QLatin1String("Comment"));
        QCOMPARE(projectInfo.ProjectTraces[0].UserName, QLatin1String("UserName"));
        QCOMPARE(projectInfo.ProjectTraces[0].Date,
            QDateTime::fromString(QLatin1String("2017-11-23T16:13:38.5091389Z"), Qt::ISODateWithMs));
        QCOMPARE(projectInfo.ProjectTraces[1].Comment, QLatin1String("Comment2"));
        QCOMPARE(projectInfo.ProjectTraces[1].UserName, QLatin1String("UserName2"));
        QCOMPARE(projectInfo.ProjectTraces[1].Date,
            QDateTime::fromString(QLatin1String("2017-11-23T16:13:38.5091389Z"), Qt::ISODateWithMs));

        QCOMPARE(projectInfo.DeviceCertificates.size(), 2);
        QCOMPARE(projectInfo.DeviceCertificates[0].FDSK, QLatin1String("FDSK"));
        QCOMPARE(projectInfo.DeviceCertificates[0].SerialNumber, QByteArray("U2VyaWFsTnVtYmVy"));
        QCOMPARE(projectInfo.DeviceCertificates[1].FDSK, QLatin1String("FDSK2"));
        QCOMPARE(projectInfo.DeviceCertificates[1].SerialNumber, QByteArray("U2VyaWFsTnVtYmVyMg=="));

        QCOMPARE(projectInfo.UserFiles.size(), 2);
        QCOMPARE(projectInfo.UserFiles[0].Filename, QLatin1String("Filename"));
        QCOMPARE(projectInfo.UserFiles[0].Comment, QLatin1String("Comment"));
        QCOMPARE(projectInfo.UserFiles[1].Filename, QLatin1String("Filename2"));
        QCOMPARE(projectInfo.UserFiles[1].Comment, QLatin1String("Comment2"));

        QCOMPARE(project.Installations.size(), 0);

        QCOMPARE(project.UserFiles.size(), 2);
        QCOMPARE(project.UserFiles[0].Filename, QLatin1String("Filename"));
        QCOMPARE(project.UserFiles[0].Comment, QLatin1String("Comment"));
        QCOMPARE(project.UserFiles[1].Filename, QLatin1String("Filename2"));
        QCOMPARE(project.UserFiles[1].Comment, QLatin1String("Comment2"));

        QCOMPARE(project.AddinData.size(), 2);
        QCOMPARE(project.AddinData[0].AddinId, QLatin1String("AddinId"));
        QCOMPARE(project.AddinData[0].Name, QLatin1String("Name"));
        QCOMPARE(project.AddinData[1].AddinId, QLatin1String("AddinId2"));
        QCOMPARE(project.AddinData[1].Name, QLatin1String("Name2"));
    }

    void testTradesBusAccessSplitInfos()
    {
        QFile file(":/data/trades-busaccess-splitinfos.xml");
        QCOMPARE(file.open(QIODevice::ReadOnly), true);

        QXmlStreamReader reader(&file);
        QCOMPARE(reader.readNextStartElement(), true);
        QCOMPARE(reader.name(), QLatin1String("KNX"));

        QKnxProjectRoot root;
        QCOMPARE(root.parseElement(&reader, true), true);

        QCOMPARE(root.CreatedBy, QLatin1String("ETS5"));
        QCOMPARE(root.ToolVersion, QLatin1String("5.5.952.23148"));
        QCOMPARE(root.Project.size(), 1);

        auto project = root.Project[0];
        QCOMPARE(project.Id, QLatin1String("ProjectId"));

        QCOMPARE(project.Installations.size(), 1);
        auto installation = project.Installations[0];

        QCOMPARE(installation.Name, QLatin1String("Installation"));
        QCOMPARE(installation.BCUKey, 4294967295);
        QCOMPARE(installation.DefaultLine, QLatin1String("P-01EB-0_L-3"));
        QCOMPARE(installation.IPRoutingLatencyTolerance, quint16(2000));
        QCOMPARE(installation.CompletionStatus, QLatin1String("Undefined"));
        QCOMPARE(installation.InstallationId, quint16(1));
        QCOMPARE(installation.IPRoutingBackboneKey, QLatin1String("12345"));
        QCOMPARE(installation.IPRoutingBackboneSecurity, QLatin1String("On"));
        QCOMPARE(installation.IPRoutingMulticastAddress, QLatin1String("192.168.2.1"));
        QCOMPARE(installation.IPSyncLatencyFraction, 2.0f);
        QCOMPARE(installation.MulticastTTL, quint8(25));
        QCOMPARE(installation.SplitType, QLatin1String("Master"));

        QCOMPARE(installation.Topology.size(), 0);
        QCOMPARE(installation.Buildings.size(), 0);
        QCOMPARE(installation.GroupAddresses.size(), 0);

        QCOMPARE(installation.Trades.size(), 2);
        auto trade = installation.Trades[0];

        QCOMPARE(trade.Comment, QLatin1String("Comment"));
        QCOMPARE(trade.CompletionStatus, QLatin1String("FinishedDesign"));
        QCOMPARE(trade.Description, QLatin1String("Description"));
        QCOMPARE(trade.Id, QLatin1String("Id"));
        QCOMPARE(trade.Name, QLatin1String("Name"));
        QCOMPARE(trade.Number, QLatin1String("Number"));
        QCOMPARE(trade.Puid, 1);
        QCOMPARE(trade.Trade.size(), 0);
        QCOMPARE(trade.DeviceInstanceRef.size(), 0);

        trade = installation.Trades[1];

        QCOMPARE(trade.Comment, QLatin1String("Comment2"));
        QCOMPARE(trade.CompletionStatus, QLatin1String("FinishedDesign"));
        QCOMPARE(trade.Description, QLatin1String("Description2"));
        QCOMPARE(trade.Id, QLatin1String("Id2"));
        QCOMPARE(trade.Name, QLatin1String("Name2"));
        QCOMPARE(trade.Number, QLatin1String("Number2"));
        QCOMPARE(trade.Puid, 2);
        QCOMPARE(trade.Trade.size(), 2);
        QCOMPARE(trade.DeviceInstanceRef.size(), 2);

        QCOMPARE(trade.DeviceInstanceRef[0], QLatin1String("RefId"));
        QCOMPARE(trade.DeviceInstanceRef[1], QLatin1String("RefId2"));

        auto subTrade = trade.Trade[0];

        QCOMPARE(subTrade.Comment, QLatin1String("Comment3"));
        QCOMPARE(subTrade.CompletionStatus, QLatin1String("FinishedDesign"));
        QCOMPARE(subTrade.Description, QLatin1String("Description3"));
        QCOMPARE(subTrade.Id, QLatin1String("Id3"));
        QCOMPARE(subTrade.Name, QLatin1String("Name3"));
        QCOMPARE(subTrade.Number, QLatin1String("Number3"));
        QCOMPARE(subTrade.Puid, 3);
        QCOMPARE(subTrade.Trade.size(), 0);
        QCOMPARE(subTrade.DeviceInstanceRef.size(), 0);

        subTrade = trade.Trade[1];

        QCOMPARE(subTrade.Comment, QLatin1String("Comment4"));
        QCOMPARE(subTrade.CompletionStatus, QLatin1String("FinishedDesign"));
        QCOMPARE(subTrade.Description, QLatin1String("Description4"));
        QCOMPARE(subTrade.Id, QLatin1String("Id4"));
        QCOMPARE(subTrade.Name, QLatin1String("Name4"));
        QCOMPARE(subTrade.Number, QLatin1String("Number4"));
        QCOMPARE(subTrade.Puid, 4);
        QCOMPARE(subTrade.Trade.size(), 0);
        QCOMPARE(subTrade.DeviceInstanceRef.size(), 1);

        QCOMPARE(subTrade.DeviceInstanceRef[0], QLatin1String("RefId3"));

        QCOMPARE(installation.BusAccess.size(), 1);
        QCOMPARE(installation.BusAccess[0].Edi, QLatin1String("{B3326C29-C90B-44C1-BA62-4A2A063764E1}"));
        QCOMPARE(installation.BusAccess[0].Name, QLatin1String("Name"));
        QCOMPARE(installation.BusAccess[0].Parameter, QLatin1String("Parameter"));

        QCOMPARE(installation.SplitInfos.size(), 2);
        QCOMPARE(installation.SplitInfos[0].Cookie, QLatin1String("B3326C29-C90B-44C1-BA62-4A2A063764E1"));
        QCOMPARE(installation.SplitInfos[0].ObjectPath, QLatin1String("ObjectPath"));
        QCOMPARE(installation.SplitInfos[1].Cookie, QLatin1String("B3326C29-C90B-44C1-BA62-4A2A063764E2"));
        QCOMPARE(installation.SplitInfos[1].ObjectPath, QLatin1String("ObjectPath2"));
    }

    void testGroupAddresses()
    {
        QFile file(":/data/groupaddresses.xml");
        QCOMPARE(file.open(QIODevice::ReadOnly), true);

        QXmlStreamReader reader(&file);
        QCOMPARE(reader.readNextStartElement(), true);
        QCOMPARE(reader.name(), QLatin1String("KNX"));

        QKnxProjectRoot root;
        QCOMPARE(root.parseElement(&reader, true), true);

        QCOMPARE(root.CreatedBy, QLatin1String("ETS5"));
        QCOMPARE(root.ToolVersion, QLatin1String("5.5.952.23148"));
        QCOMPARE(root.Project.size(), 1);

        auto project = root.Project[0];
        QCOMPARE(project.Id, QLatin1String("ProjectId"));

        QCOMPARE(project.Installations.size(), 1);
        auto installation = project.Installations[0];

        QCOMPARE(installation.Name, QLatin1String("Installation"));
        QCOMPARE(installation.BCUKey, 4294967295);
        QCOMPARE(installation.DefaultLine, QLatin1String("P-01EB-0_L-3"));
        QCOMPARE(installation.IPRoutingLatencyTolerance, quint16(2000));
        QCOMPARE(installation.CompletionStatus, QLatin1String("Undefined"));
        QCOMPARE(installation.InstallationId, quint16(1));
        QCOMPARE(installation.IPRoutingBackboneKey, QLatin1String("12345"));
        QCOMPARE(installation.IPRoutingBackboneSecurity, QLatin1String("On"));
        QCOMPARE(installation.IPRoutingMulticastAddress, QLatin1String("192.168.2.1"));
        QCOMPARE(installation.IPSyncLatencyFraction, 2.0f);
        QCOMPARE(installation.MulticastTTL, quint8(25));
        QCOMPARE(installation.SplitType, QLatin1String("Master"));

        QCOMPARE(installation.Topology.size(), 0);
        QCOMPARE(installation.Buildings.size(), 0);
        QCOMPARE(installation.Trades.size(), 0);
        QCOMPARE(installation.BusAccess.size(), 0);
        QCOMPARE(installation.SplitInfos.size(), 0);

        QCOMPARE(installation.GroupAddresses.size(), 1);
        QCOMPARE(installation.GroupAddresses[0].GroupRanges.size(), 2);
        auto groupRange = installation.GroupAddresses[0].GroupRanges[0];

        QCOMPARE(groupRange.Id, QLatin1String("P-050F-0_GR-1"));
        QCOMPARE(groupRange.RangeStart, quint16(1));
        QCOMPARE(groupRange.RangeEnd, quint16(2047));
        QCOMPARE(groupRange.Name, QLatin1String("GroupRange"));
        QCOMPARE(groupRange.Puid, 58);
        QCOMPARE(groupRange.Unfiltered, false);
        QCOMPARE(groupRange.Description, QLatin1String("Description"));
        QCOMPARE(groupRange.Comment, QLatin1String("Comment"));
        QCOMPARE(groupRange.Security, QLatin1String("Auto"));

        QCOMPARE(groupRange.GroupRange.size(), 1);
        QCOMPARE(groupRange.GroupAddress.size(), 0);

        auto subRange = groupRange.GroupRange[0];

        QCOMPARE(subRange.Id, QLatin1String("P-050F-0_GR-16"));
        QCOMPARE(subRange.RangeStart, quint16(1));
        QCOMPARE(subRange.RangeEnd, quint16(255));
        QCOMPARE(subRange.Name, QLatin1String("GroupRange2"));
        QCOMPARE(subRange.Puid, 308);
        QCOMPARE(subRange.Unfiltered, false); // default
        QCOMPARE(subRange.Description, QString());
        QCOMPARE(subRange.Comment, QString());
        QCOMPARE(subRange.Security, QLatin1String("Auto")); // default

        QCOMPARE(subRange.GroupRange.size(), 0);
        QCOMPARE(subRange.GroupAddress.size(), 2);

        auto address = subRange.GroupAddress[0];

        QCOMPARE(address.Id, QLatin1String("P-050F-0_GA-355"));
        QCOMPARE(address.Address, 1);
        QCOMPARE(address.Name, QLatin1String("Name"));
        QCOMPARE(address.Description, QLatin1String("Description"));
        QCOMPARE(address.DatapointType, QLatin1String("DPST-1-1"));
        QCOMPARE(address.Puid, 436);
        QCOMPARE(address.Unfiltered, false);
        QCOMPARE(address.Central, true);
        QCOMPARE(address.Global, false);
        QCOMPARE(address.Comment, QLatin1String("Comment"));
        QCOMPARE(address.Key, QLatin1String("Key"));
        QCOMPARE(address.Security, QLatin1String("Auto"));

        address = subRange.GroupAddress[1];

        QCOMPARE(address.Id, QLatin1String("P-050F-0_GA-356"));
        QCOMPARE(address.Address, 2);
        QCOMPARE(address.Name, QLatin1String("Name2"));
        QCOMPARE(address.Description, QLatin1String("Description2"));
        QCOMPARE(address.DatapointType, QLatin1String("DPST-20-102"));
        QCOMPARE(address.Puid, 437);
        QCOMPARE(address.Unfiltered, false);
        QCOMPARE(address.Central, true);
        QCOMPARE(address.Global, false);
        QCOMPARE(address.Comment, QLatin1String("Comment"));
        QCOMPARE(address.Key, QLatin1String("Key"));
        QCOMPARE(address.Security, QLatin1String("Auto"));

        groupRange = installation.GroupAddresses[0].GroupRanges[1];

        QCOMPARE(groupRange.Id, QLatin1String("P-050F-0_GR-2"));
        QCOMPARE(groupRange.RangeStart, quint16(2048));
        QCOMPARE(groupRange.RangeEnd, quint16(4095));
        QCOMPARE(groupRange.Name, QLatin1String("GroupRange3"));
        QCOMPARE(groupRange.Puid, 59);
        QCOMPARE(groupRange.Unfiltered, false);
        QCOMPARE(groupRange.Description, QLatin1String("Description"));
        QCOMPARE(groupRange.Comment, QLatin1String("Comment"));
        QCOMPARE(groupRange.Security, QLatin1String("Auto"));

        QCOMPARE(groupRange.GroupRange.size(), 2);
        QCOMPARE(groupRange.GroupAddress.size(), 1);

        address = groupRange.GroupAddress[0];

        QCOMPARE(address.Id, QLatin1String("P-050F-0_GA-261"));
        QCOMPARE(address.Address, 2048);
        QCOMPARE(address.Name, QLatin1String("Name3"));
        QCOMPARE(address.Description, QLatin1String("Description3"));
        QCOMPARE(address.DatapointType, QLatin1String("DPST-1-1"));
        QCOMPARE(address.Puid, 342);
        QCOMPARE(address.Unfiltered, false);
        QCOMPARE(address.Central, true);
        QCOMPARE(address.Global, false);
        QCOMPARE(address.Comment, QLatin1String("Comment3"));
        QCOMPARE(address.Key, QLatin1String("Key3"));
        QCOMPARE(address.Security, QLatin1String("Auto"));

        subRange = groupRange.GroupRange[0];

        QCOMPARE(subRange.Id, QLatin1String("P-050F-0_GR-17"));
        QCOMPARE(subRange.RangeStart, quint16(2048));
        QCOMPARE(subRange.RangeEnd, quint16(2303));
        QCOMPARE(subRange.Name, QLatin1String("GroupRange4"));
        QCOMPARE(subRange.Puid, 309);
        QCOMPARE(subRange.Unfiltered, false);
        QCOMPARE(subRange.Description, QLatin1String("Description"));
        QCOMPARE(subRange.Comment, QLatin1String("Comment"));
        QCOMPARE(subRange.Security, QLatin1String("Auto"));

        QCOMPARE(subRange.GroupRange.size(), 0);
        QCOMPARE(subRange.GroupAddress.size(), 1);

        address = subRange.GroupAddress[0];

        QCOMPARE(address.Id, QLatin1String("P-050F-0_GA-262"));
        QCOMPARE(address.Address, 2049);
        QCOMPARE(address.Name, QLatin1String("Name4"));
        QCOMPARE(address.Description, QLatin1String("Description4"));
        QCOMPARE(address.DatapointType, QLatin1String("DPST-1-1"));
        QCOMPARE(address.Puid, 343);
        QCOMPARE(address.Unfiltered, false);
        QCOMPARE(address.Central, true);
        QCOMPARE(address.Global, false);
        QCOMPARE(address.Comment, QLatin1String("Comment4"));
        QCOMPARE(address.Key, QLatin1String("Key4"));
        QCOMPARE(address.Security, QLatin1String("Auto"));

        subRange = groupRange.GroupRange[1];

        QCOMPARE(subRange.Id, QLatin1String("P-050F-0_GR-18"));
        QCOMPARE(subRange.RangeStart, quint16(2304));
        QCOMPARE(subRange.RangeEnd, quint16(2559));
        QCOMPARE(subRange.Name, QLatin1String("GroupRange5"));
        QCOMPARE(subRange.Puid, 310);
        QCOMPARE(subRange.Unfiltered, false);
        QCOMPARE(subRange.Description, QLatin1String("Description5"));
        QCOMPARE(subRange.Comment, QLatin1String("Comment5"));
        QCOMPARE(subRange.Security, QLatin1String("Auto"));

        QCOMPARE(subRange.GroupRange.size(), 0);
        QCOMPARE(subRange.GroupAddress.size(), 2);

        address = subRange.GroupAddress[0];

        QCOMPARE(address.Id, QLatin1String("P-050F-0_GA-358"));
        QCOMPARE(address.Address, 2304);
        QCOMPARE(address.Name, QLatin1String("Name5"));
        QCOMPARE(address.Description, QLatin1String("Description5"));
        QCOMPARE(address.DatapointType, QLatin1String("DPST-1-1"));
        QCOMPARE(address.Puid, 439);
        QCOMPARE(address.Unfiltered, false);
        QCOMPARE(address.Central, true);
        QCOMPARE(address.Global, false);
        QCOMPARE(address.Comment, QLatin1String("Comment5"));
        QCOMPARE(address.Key, QLatin1String("Key5"));
        QCOMPARE(address.Security, QLatin1String("Auto"));

        address = subRange.GroupAddress[1];

        QCOMPARE(address.Id, QLatin1String("P-050F-0_GA-359"));
        QCOMPARE(address.Address, 2305);
        QCOMPARE(address.Name, QLatin1String("Name6"));
        QCOMPARE(address.Description, QLatin1String("Description6"));
        QCOMPARE(address.DatapointType, QString());
        QCOMPARE(address.Puid, 440);
        QCOMPARE(address.Unfiltered, false);
        QCOMPARE(address.Central, true);
        QCOMPARE(address.Global, false);
        QCOMPARE(address.Comment, QLatin1String("Comment6"));
        QCOMPARE(address.Key, QLatin1String("Key6"));
        QCOMPARE(address.Security, QLatin1String("Auto"));
    }

    void testBuildings()
    {
        QFile file(":/data/buildings.xml");
        QCOMPARE(file.open(QIODevice::ReadOnly), true);

        QXmlStreamReader reader(&file);
        QCOMPARE(reader.readNextStartElement(), true);
        QCOMPARE(reader.name(), QLatin1String("KNX"));

        QKnxProjectRoot root;
        QCOMPARE(root.parseElement(&reader, true), true);

        QCOMPARE(root.CreatedBy, QLatin1String("ETS5"));
        QCOMPARE(root.ToolVersion, QLatin1String("5.5.952.23148"));
        QCOMPARE(root.Project.size(), 1);

        auto project = root.Project[0];
        QCOMPARE(project.Id, QLatin1String("ProjectId"));

        QCOMPARE(project.Installations.size(), 1);
        auto installation = project.Installations[0];

        QCOMPARE(installation.Name, QLatin1String("Installation"));
        QCOMPARE(installation.BCUKey, 4294967295);
        QCOMPARE(installation.DefaultLine, QLatin1String("P-01EB-0_L-3"));
        QCOMPARE(installation.IPRoutingLatencyTolerance, quint16(2000));
        QCOMPARE(installation.CompletionStatus, QLatin1String("Undefined"));
        QCOMPARE(installation.InstallationId, quint16(1));
        QCOMPARE(installation.IPRoutingBackboneKey, QLatin1String("12345"));
        QCOMPARE(installation.IPRoutingBackboneSecurity, QLatin1String("On"));
        QCOMPARE(installation.IPRoutingMulticastAddress, QLatin1String("192.168.2.1"));
        QCOMPARE(installation.IPSyncLatencyFraction, 2.0f);
        QCOMPARE(installation.MulticastTTL, quint8(25));
        QCOMPARE(installation.SplitType, QLatin1String("Master"));

        QCOMPARE(installation.Topology.size(), 0);
        QCOMPARE(installation.GroupAddresses.size(), 0);
        QCOMPARE(installation.Trades.size(), 0);
        QCOMPARE(installation.BusAccess.size(), 0);
        QCOMPARE(installation.SplitInfos.size(), 0);

        QCOMPARE(installation.Buildings.size(), 1);
        auto buildings = installation.Buildings[0];

        QCOMPARE(buildings.BuildingPart.size(), 2);
        auto buildingPart = buildings.BuildingPart[0];

        QCOMPARE(buildingPart.Comment, QLatin1String("Comment"));
        QCOMPARE(buildingPart.CompletionStatus, QLatin1String("Undefined"));
        QCOMPARE(buildingPart.DefaultLine, QLatin1String("1"));
        QCOMPARE(buildingPart.Description, QLatin1String("Description"));
        QCOMPARE(buildingPart.Id, QLatin1String("Id"));
        QCOMPARE(buildingPart.Name, QLatin1String("Name"));
        QCOMPARE(buildingPart.Number, QLatin1String("Number"));
        QCOMPARE(buildingPart.Puid, 1);
        QCOMPARE(buildingPart.Type, QLatin1String("Building"));

        QCOMPARE(buildingPart.BuildingPart.size(), 1);
        auto subBuildingPart = buildingPart.BuildingPart[0];

        QCOMPARE(subBuildingPart.Comment, QLatin1String("Comment2"));
        QCOMPARE(subBuildingPart.CompletionStatus, QLatin1String("Editing"));
        QCOMPARE(subBuildingPart.DefaultLine, QLatin1String("2"));
        QCOMPARE(subBuildingPart.Description, QLatin1String("Description2"));
        QCOMPARE(subBuildingPart.Id, QLatin1String("Id2"));
        QCOMPARE(subBuildingPart.Name, QLatin1String("Name2"));
        QCOMPARE(subBuildingPart.Number, QLatin1String("Number2"));
        QCOMPARE(subBuildingPart.Puid, 2);
        QCOMPARE(subBuildingPart.Type, QLatin1String("BuildingPart"));

        QCOMPARE(subBuildingPart.DeviceInstanceRef.size(), 2);
        QCOMPARE(subBuildingPart.DeviceInstanceRef[0], QLatin1String("RefId"));
        QCOMPARE(subBuildingPart.DeviceInstanceRef[1], QLatin1String("RefId2"));

        QCOMPARE(subBuildingPart.Function.size(), 1);
        auto function = subBuildingPart.Function[0];

        QCOMPARE(function.Comment, QLatin1String("Comment"));
        QCOMPARE(function.CompletionStatus, QLatin1String("Editing"));
        QCOMPARE(function.DefaultGroupRange, QLatin1String("DefaultGroupRange"));
        QCOMPARE(function.Description, QLatin1String("Description"));
        QCOMPARE(function.Id, QLatin1String("Id3"));
        QCOMPARE(function.Name, QLatin1String("Name"));
        QCOMPARE(function.Number, QLatin1String("Number"));
        QCOMPARE(function.Puid, 3);
        QCOMPARE(function.Type, QLatin1String("Type"));

        QCOMPARE(function.GroupAddressRef.size(), 2);
        auto groupAddressRef = function.GroupAddressRef[0];
        QCOMPARE(groupAddressRef.Id, QLatin1String("Id4"));
        QCOMPARE(groupAddressRef.Name, QLatin1String("Name"));
        QCOMPARE(groupAddressRef.Puid, 4);
        QCOMPARE(groupAddressRef.RefId, QLatin1String("RefId3"));
        QCOMPARE(groupAddressRef.Role, QLatin1String("Role"));

        groupAddressRef = function.GroupAddressRef[1];
        QCOMPARE(groupAddressRef.Id, QLatin1String("Id5"));
        QCOMPARE(groupAddressRef.Name, QLatin1String("Name2"));
        QCOMPARE(groupAddressRef.Puid, 5);
        QCOMPARE(groupAddressRef.RefId, QLatin1String("RefId4"));
        QCOMPARE(groupAddressRef.Role, QLatin1String("Role2"));

        QCOMPARE(buildingPart.DeviceInstanceRef.size(), 2);
        QCOMPARE(buildingPart.DeviceInstanceRef[0], QLatin1String("RefId5"));
        QCOMPARE(buildingPart.DeviceInstanceRef[1], QLatin1String("RefId6"));

        QCOMPARE(buildingPart.Function.size(), 2);
        function = buildingPart.Function[0];

        QCOMPARE(function.Comment, QLatin1String("Comment6"));
        QCOMPARE(function.CompletionStatus, QLatin1String("Editing"));
        QCOMPARE(function.DefaultGroupRange, QLatin1String("DefaultGroupRange6"));
        QCOMPARE(function.Description, QLatin1String("Description6"));
        QCOMPARE(function.Id, QLatin1String("Id6"));
        QCOMPARE(function.Name, QLatin1String("Name6"));
        QCOMPARE(function.Number, QLatin1String("Number6"));
        QCOMPARE(function.Puid, 6);
        QCOMPARE(function.Type, QLatin1String("Type6"));

        QCOMPARE(function.GroupAddressRef.size(), 2);
        groupAddressRef = function.GroupAddressRef[0];
        QCOMPARE(groupAddressRef.Id, QLatin1String("Id7"));
        QCOMPARE(groupAddressRef.Name, QLatin1String("Name7"));
        QCOMPARE(groupAddressRef.Puid, 7);
        QCOMPARE(groupAddressRef.RefId, QLatin1String("RefId7"));
        QCOMPARE(groupAddressRef.Role, QLatin1String("Role7"));

        groupAddressRef = function.GroupAddressRef[1];
        QCOMPARE(groupAddressRef.Id, QLatin1String("Id8"));
        QCOMPARE(groupAddressRef.Name, QLatin1String("Name8"));
        QCOMPARE(groupAddressRef.Puid, 8);
        QCOMPARE(groupAddressRef.RefId, QLatin1String("RefId8"));
        QCOMPARE(groupAddressRef.Role, QLatin1String("Role8"));

        function = buildingPart.Function[1];

        QCOMPARE(function.Comment, QLatin1String("Comment9"));
        QCOMPARE(function.CompletionStatus, QLatin1String("Editing"));
        QCOMPARE(function.DefaultGroupRange, QLatin1String("DefaultGroupRange9"));
        QCOMPARE(function.Description, QLatin1String("Description9"));
        QCOMPARE(function.Id, QLatin1String("Id9"));
        QCOMPARE(function.Name, QLatin1String("Name9"));
        QCOMPARE(function.Number, QLatin1String("Number9"));
        QCOMPARE(function.Puid, 9);
        QCOMPARE(function.Type, QLatin1String("Type9"));

        QCOMPARE(function.GroupAddressRef.size(), 2);
        groupAddressRef = function.GroupAddressRef[0];
        QCOMPARE(groupAddressRef.Id, QLatin1String("Id10"));
        QCOMPARE(groupAddressRef.Name, QLatin1String("Name10"));
        QCOMPARE(groupAddressRef.Puid, 10);
        QCOMPARE(groupAddressRef.RefId, QLatin1String("RefId10"));
        QCOMPARE(groupAddressRef.Role, QLatin1String("Role10"));

        groupAddressRef = function.GroupAddressRef[1];
        QCOMPARE(groupAddressRef.Id, QLatin1String("Id11"));
        QCOMPARE(groupAddressRef.Name, QLatin1String("Name11"));
        QCOMPARE(groupAddressRef.Puid, 11);
        QCOMPARE(groupAddressRef.RefId, QLatin1String("RefId11"));
        QCOMPARE(groupAddressRef.Role, QLatin1String("Role11"));

        buildingPart = buildings.BuildingPart[1];
        QCOMPARE(buildingPart.Id, QLatin1String("Id100"));
        QCOMPARE(buildingPart.Name, QLatin1String("Name100"));
        QCOMPARE(buildingPart.Type, QLatin1String("Building"));
        QCOMPARE(buildingPart.Puid, 100);
    }

    void testTopologyArea()
    {
        QFile file(":/data/topology.xml");
        QCOMPARE(file.open(QIODevice::ReadOnly), true);

        QXmlStreamReader reader(&file);
        QCOMPARE(reader.readNextStartElement(), true);
        QCOMPARE(reader.name(), QLatin1String("KNX"));

        QKnxProjectRoot root;
        QCOMPARE(root.parseElement(&reader, true), true);

        QCOMPARE(root.CreatedBy, QLatin1String("ETS5"));
        QCOMPARE(root.ToolVersion, QLatin1String("5.5.952.23148"));
        QCOMPARE(root.Project.size(), 1);

        auto project = root.Project[0];
        QCOMPARE(project.Id, QLatin1String("ProjectId"));

        QCOMPARE(project.Installations.size(), 1);
        auto installation = project.Installations[0];

        QCOMPARE(installation.Name, QLatin1String("Installation"));
        QCOMPARE(installation.BCUKey, 4294967295);
        QCOMPARE(installation.DefaultLine, QLatin1String("P-01EB-0_L-3"));
        QCOMPARE(installation.IPRoutingLatencyTolerance, quint16(2000));
        QCOMPARE(installation.CompletionStatus, QLatin1String("Undefined"));
        QCOMPARE(installation.InstallationId, quint16(1));
        QCOMPARE(installation.IPRoutingBackboneKey, QLatin1String("12345"));
        QCOMPARE(installation.IPRoutingBackboneSecurity, QLatin1String("On"));
        QCOMPARE(installation.IPRoutingMulticastAddress, QLatin1String("192.168.2.1"));
        QCOMPARE(installation.IPSyncLatencyFraction, 2.0f);
        QCOMPARE(installation.MulticastTTL, quint8(25));
        QCOMPARE(installation.SplitType, QLatin1String("Master"));

        QCOMPARE(installation.Buildings.size(), 0);
        QCOMPARE(installation.GroupAddresses.size(), 0);
        QCOMPARE(installation.Trades.size(), 0);
        QCOMPARE(installation.BusAccess.size(), 0);
        QCOMPARE(installation.SplitInfos.size(), 0);

        QCOMPARE(installation.Topology.size(), 1);
        auto topology = installation.Topology[0];

        QCOMPARE(installation.Topology[0].UnassignedDevices.size(), 0);
        QCOMPARE(installation.Topology[0].Area.size(), 1);

        auto area = installation.Topology[0].Area[0];
        QCOMPARE(area.Address, 1);
        QCOMPARE(area.Comment, QLatin1String("Comment1"));
        QCOMPARE(area.CompletionStatus, QLatin1String("Editing"));
        QCOMPARE(area.Description, QLatin1String("Description1"));
        QCOMPARE(area.Id, QLatin1String("Id1"));
        QCOMPARE(area.Name, QLatin1String("Name1"));
        QCOMPARE(area.Puid, 1);

        QCOMPARE(area.Line.size(), 1);
        auto line = area.Line[0];

        QCOMPARE(line.Address, 2);
        QCOMPARE(line.Comment, QLatin1String("Comment2"));
        QCOMPARE(line.CompletionStatus, QLatin1String("FinishedDesign"));
        QCOMPARE(line.Description, QLatin1String("Description2"));
        QCOMPARE(line.DomainAddress, 22);
        QCOMPARE(line.Id, QLatin1String("Id2"));
        QCOMPARE(line.MediumTypeRefId, QLatin1String("MediumTypeRefId2"));
        QCOMPARE(line.Name, QLatin1String("Name2"));
        QCOMPARE(line.Puid, 222);

        QCOMPARE(line.DeviceInstance.size(), 1);
        auto devInstance = line.DeviceInstance[0];

        QCOMPARE(devInstance.Address, 3);
        QCOMPARE(devInstance.ApplicationProgramLoaded, false);
        QCOMPARE(devInstance.Broken, true);
        QCOMPARE(devInstance.CheckSums, QByteArray("Q2hlY2tTdW1zMw=="));
        QCOMPARE(devInstance.Comment, QLatin1String("Comment3"));
        QCOMPARE(devInstance.CommunicationPartLoaded, true);
        QCOMPARE(devInstance.CompletionStatus, QLatin1String("Editing"));
        QCOMPARE(devInstance.Description, QLatin1String("Description3"));
        QCOMPARE(devInstance.Hardware2ProgramRefId, QLatin1String("Hardware2ProgramRefId3"));
        QCOMPARE(devInstance.Id, QLatin1String("Id3"));
        QCOMPARE(devInstance.IndividualAddressLoaded, true);
        QCOMPARE(devInstance.InstallationHints, QLatin1String("InstallationHints3"));
        QCOMPARE(devInstance.IsCommunicationObjectVisibilityCalculated, false);
        QCOMPARE(devInstance.IsRFRetransmitter, false);
        QCOMPARE(devInstance.LastDownload,
            QDateTime::fromString(QLatin1String("2003-11-23T15:02:26.2503409Z"), Qt::ISODateWithMs));
        QCOMPARE(devInstance.LastModified,
            QDateTime::fromString(QLatin1String("2017-03-23T15:02:26.2503409Z"), Qt::ISODateWithMs));
        QCOMPARE(devInstance.LastUsedAPDULength, 3);
        QCOMPARE(devInstance.LoadedImage, QByteArray("TG9hZGVkSW1hZ2Uz"));
        QCOMPARE(devInstance.MediumConfigLoaded, true);
        QCOMPARE(devInstance.Name, QLatin1String("Name3"));
        QCOMPARE(devInstance.ParametersLoaded, true);
        QCOMPARE(devInstance.ProductRefId, QLatin1String("ProductRefId3"));
        QCOMPARE(devInstance.Puid, 33);
        QCOMPARE(devInstance.ReadMaxAPDULength, 333);
        QCOMPARE(devInstance.ReadMaxRoutingAPDULength, quint16(3333));
        QCOMPARE(devInstance.SerialNumber, QByteArray("U2VyaWFsTnVtYmVyMw=="));
        QCOMPARE(devInstance.UniqueId, QLatin1String("{33333333-3333-3333-3333-333333333333}"));

        QCOMPARE(devInstance.AdditionalAddresses.size(), 2);
        auto address = devInstance.AdditionalAddresses[0];
        QCOMPARE(address.Address, 4);
        QCOMPARE(address.Comment, QLatin1String("Comment4"));
        QCOMPARE(address.Description, QLatin1String("Description4"));
        QCOMPARE(address.Name, QLatin1String("Name4"));

        address = devInstance.AdditionalAddresses[1];
        QCOMPARE(address.Address, 5);
        QCOMPARE(address.Comment, QLatin1String("Comment5"));
        QCOMPARE(address.Description, QLatin1String("Description5"));
        QCOMPARE(address.Name, QLatin1String("Name5"));

        QCOMPARE(devInstance.BinaryData.size(), 2);
        auto bd = devInstance.BinaryData[0];

        QCOMPARE(bd.Id, QLatin1String("Id6"));
        QCOMPARE(bd.Name, QLatin1String("Name6"));
        QCOMPARE(bd.RefId, QLatin1String("RefId6"));

        bd = devInstance.BinaryData[1];
        QCOMPARE(bd.Id, QLatin1String("Id7"));
        QCOMPARE(bd.Name, QLatin1String("Name7"));
        QCOMPARE(bd.RefId, QLatin1String("RefId7"));

        QCOMPARE(devInstance.IpConfig.Assign, QLatin1String("Auto"));
        QCOMPARE(devInstance.IpConfig.DefaultGateway, QLatin1String("8.8.8.8"));
        QCOMPARE(devInstance.IpConfig.IPAddress, QLatin1String("88.88.88.88"));
        QCOMPARE(devInstance.IpConfig.MacAddress, QLatin1String("88-88-88-88-88-88"));
        QCOMPARE(devInstance.IpConfig.SubnetMask, QLatin1String("255.255.255.0"));

        QCOMPARE(devInstance.Security.DeviceAuthenticationCode, QLatin1String("AuthenticationCode9"));
        QCOMPARE(devInstance.Security.DeviceManagementPassword, QLatin1String("ManagementPassword9"));
        QCOMPARE(devInstance.Security.LoadedDeviceAuthenticationCode, QLatin1String("LoadedAutCode9"));
        QCOMPARE(devInstance.Security.LoadedDeviceManagementPassword, QLatin1String("LoadedMngmtPassword9"));
        QCOMPARE(devInstance.Security.LoadedIPRoutingBackboneKey, QLatin1String("LoadedIPRoutingBackboneKey9"));
        QCOMPARE(devInstance.Security.LoadedToolKey, QLatin1String("LoadedToolKey9"));
        QCOMPARE(devInstance.Security.SequenceNumber, 9);
        QCOMPARE(devInstance.Security.SequenceNumberTimestamp,
            QDateTime::fromString(QLatin1String("2009-09-09T15:02:26.2503409Z"), Qt::ISODateWithMs));
        QCOMPARE(devInstance.Security.ToolKey, QLatin1String("ToolKey9"));

        QCOMPARE(devInstance.BusInterfaces.size(), 2);
        auto busInterface = devInstance.BusInterfaces[0];

        QCOMPARE(busInterface.Comment, QLatin1String("Comment10"));
        QCOMPARE(busInterface.Description, QLatin1String("Description10"));
        QCOMPARE(busInterface.Name, QLatin1String("Name10"));
        QCOMPARE(busInterface.Password, QLatin1String("Password10"));

        QCOMPARE(busInterface.Connectors.size(), 2);
        QCOMPARE(busInterface.Connectors[0], QLatin1String("GroupAddressRefId11"));
        QCOMPARE(busInterface.Connectors[1], QLatin1String("GroupAddressRefId1111"));

        busInterface = devInstance.BusInterfaces[1];
        QCOMPARE(busInterface.Comment, QLatin1String("Comment12"));
        QCOMPARE(busInterface.Description, QLatin1String("Description12"));
        QCOMPARE(busInterface.Name, QLatin1String("Name12"));
        QCOMPARE(busInterface.Password, QLatin1String("Password12"));

        QCOMPARE(busInterface.Connectors.size(), 2);
        QCOMPARE(busInterface.Connectors[0], QLatin1String("GroupAddressRefId13"));
        QCOMPARE(busInterface.Connectors[1], QLatin1String("GroupAddressRefId1313"));

        QCOMPARE(line.BusAccess.size(), 1);
        QCOMPARE(line.BusAccess[0].Edi, QLatin1String("{14141414-1414-1414-1414-141414141414}"));
        QCOMPARE(line.BusAccess[0].Name, QLatin1String("Name14"));
        QCOMPARE(line.BusAccess[0].Parameter, QLatin1String("Parameter14"));

        QCOMPARE(line.AdditionalGroupAddresses.size(), 2);
        QCOMPARE(line.AdditionalGroupAddresses[0].Address, 15);
        QCOMPARE(line.AdditionalGroupAddresses[1].Address, 16);
    }

    void testUnassignedDevices()
    {
        QFile file(":/data/unassigneddevices.xml");
        QCOMPARE(file.open(QIODevice::ReadOnly), true);

        QXmlStreamReader reader(&file);
        QCOMPARE(reader.readNextStartElement(), true);
        QCOMPARE(reader.name(), QLatin1String("KNX"));

        QKnxProjectRoot root;
        QCOMPARE(root.parseElement(&reader, true), true);

        QCOMPARE(root.CreatedBy, QLatin1String("ETS5"));
        QCOMPARE(root.ToolVersion, QLatin1String("5.5.952.23148"));
        QCOMPARE(root.Project.size(), 1);

        auto project = root.Project[0];
        QCOMPARE(project.Id, QLatin1String("ProjectId"));

        QCOMPARE(project.Installations.size(), 1);
        auto installation = project.Installations[0];

        QCOMPARE(installation.Name, QLatin1String("Installation"));
        QCOMPARE(installation.BCUKey, 4294967295);
        QCOMPARE(installation.DefaultLine, QLatin1String("P-01EB-0_L-3"));
        QCOMPARE(installation.IPRoutingLatencyTolerance, quint16(2000));
        QCOMPARE(installation.CompletionStatus, QLatin1String("Undefined"));
        QCOMPARE(installation.InstallationId, quint16(1));
        QCOMPARE(installation.IPRoutingBackboneKey, QLatin1String("12345"));
        QCOMPARE(installation.IPRoutingBackboneSecurity, QLatin1String("On"));
        QCOMPARE(installation.IPRoutingMulticastAddress, QLatin1String("192.168.2.1"));
        QCOMPARE(installation.IPSyncLatencyFraction, 2.0f);
        QCOMPARE(installation.MulticastTTL, quint8(25));
        QCOMPARE(installation.SplitType, QLatin1String("Master"));

        QCOMPARE(installation.Buildings.size(), 0);
        QCOMPARE(installation.GroupAddresses.size(), 0);
        QCOMPARE(installation.Trades.size(), 0);
        QCOMPARE(installation.BusAccess.size(), 0);
        QCOMPARE(installation.SplitInfos.size(), 0);

        QCOMPARE(installation.Topology.size(), 1);
        auto topology = installation.Topology[0];

        QCOMPARE(installation.Topology[0].Area.size(), 0);
        QCOMPARE(installation.Topology[0].UnassignedDevices.size(), 1);

        auto device = installation.Topology[0].UnassignedDevices[0];

        QCOMPARE(device.Address, 3);
        QCOMPARE(device.ApplicationProgramLoaded, false);
        QCOMPARE(device.Broken, true);
        QCOMPARE(device.CheckSums, QByteArray(""));
        QCOMPARE(device.Comment, QLatin1String("Comment"));
        QCOMPARE(device.CommunicationPartLoaded, true);
        QCOMPARE(device.CompletionStatus, QLatin1String("Editing"));
        QCOMPARE(device.Description, QLatin1String("Description"));
        QCOMPARE(device.Hardware2ProgramRefId, QLatin1String("Hardware2ProgramRefId"));
        QCOMPARE(device.Id, QLatin1String("Id3"));
        QCOMPARE(device.IndividualAddressLoaded, true);
        QCOMPARE(device.InstallationHints, QLatin1String("InstallationHints"));
        QCOMPARE(device.IsCommunicationObjectVisibilityCalculated, false);
        QCOMPARE(device.IsRFRetransmitter, false);
        QCOMPARE(device.LastDownload, QDateTime::fromString(QLatin1String(""), Qt::ISODateWithMs));
        QCOMPARE(device.LastModified, QDateTime::fromString(QLatin1String(""), Qt::ISODateWithMs));
        QCOMPARE(device.LastUsedAPDULength, quint16(40));
        QCOMPARE(device.LoadedImage, QByteArray(""));
        QCOMPARE(device.MediumConfigLoaded, true);
        QCOMPARE(device.Name, QLatin1String("Name"));
        QCOMPARE(device.ParametersLoaded, true);
        QCOMPARE(device.ProductRefId, QLatin1String("ProductRefId"));
        QCOMPARE(device.Puid, quint16(3));
        QCOMPARE(device.ReadMaxAPDULength, quint16(50));
        QCOMPARE(device.ReadMaxRoutingAPDULength, quint16(60));
        QCOMPARE(device.SerialNumber, QByteArray("U2VyaWFsTnVtYmVyMw=="));
        QCOMPARE(device.UniqueId, QLatin1String("{12345678-1234-1234-1234-123456789123}"));

        QCOMPARE(device.ParameterInstanceRefs.size(), 2);
        auto ref = device.ParameterInstanceRefs[0];

        QCOMPARE(ref.Id, QLatin1String("Id15"));
        QCOMPARE(ref.RefId, QLatin1String("Refid15"));
        QCOMPARE(ref.Value, QLatin1String("Value15"));

        ref = device.ParameterInstanceRefs[1];
        QCOMPARE(ref.Id, QLatin1String("Id16"));
        QCOMPARE(ref.RefId, QLatin1String("Refid16"));
        QCOMPARE(ref.Value, QLatin1String("Value16"));

        QCOMPARE(device.ComObjectInstanceRefs.size(), 1);
        auto comRef = device.ComObjectInstanceRefs[0];

        QCOMPARE(comRef.ChannelId, QLatin1String("ChannelId"));
        QCOMPARE(comRef.CommunicationFlag, QLatin1String("Enabled"));
        QCOMPARE(comRef.DatapointType, QVector<QString>({ QStringLiteral("Dpt-1-1") }));
        QCOMPARE(comRef.Description, QLatin1String("Description"));
        QCOMPARE(comRef.FunctionText, QLatin1String("FunctionText"));
        QCOMPARE(comRef.Id, QLatin1String("Id12346"));
        QCOMPARE(comRef.IsActive, false);
        QCOMPARE(comRef.Priority, QLatin1String("High"));
        QCOMPARE(comRef.ReadFlag, QLatin1String("Enabled"));
        QCOMPARE(comRef.ReadOnInitFlag, QLatin1String("Enabled"));
        QCOMPARE(comRef.RefId, QLatin1String("RefId123456"));
        QCOMPARE(comRef.Text, QLatin1String("Text"));
        QCOMPARE(comRef.TransmitFlag, QLatin1String("Disabled"));
        QCOMPARE(comRef.UpdateFlag, QLatin1String("Enabled"));
        QCOMPARE(comRef.WriteFlag, QLatin1String("Disabled"));

        QCOMPARE(comRef.Connectors.size(), 1);
        auto connector = comRef.Connectors[0].Send;

        QCOMPARE(connector.Acknowledge, true);
        QCOMPARE(connector.GroupAddressRefId, QLatin1String("GroupAddressRefId"));

        QCOMPARE(comRef.Connectors[0].Receive.size(), 2);
        connector = comRef.Connectors[0].Receive[0];

        QCOMPARE(connector.Acknowledge, true);
        QCOMPARE(connector.GroupAddressRefId, QLatin1String("GroupAddressRefId2"));

        connector = comRef.Connectors[0].Receive[1];

        QCOMPARE(connector.Acknowledge, false);
        QCOMPARE(connector.GroupAddressRefId, QLatin1String("GroupAddressRefId3"));

        QCOMPARE(device.ChannelInstances.size(), 2);
        auto channel = device.ChannelInstances[0];

        QCOMPARE(channel.Description, QLatin1String("Description"));
        QCOMPARE(channel.Id, QLatin1String("Id1297"));
        QCOMPARE(channel.IsActive, false);
        QCOMPARE(channel.Name, QLatin1String("Name"));
        QCOMPARE(channel.RefId, QLatin1String("RefId099787"));

        channel = device.ChannelInstances[1];
        QCOMPARE(channel.Description, QLatin1String("Description2"));
        QCOMPARE(channel.Id, QLatin1String("Id12972"));
        QCOMPARE(channel.IsActive, true);
        QCOMPARE(channel.Name, QLatin1String("Name2"));
        QCOMPARE(channel.RefId, QLatin1String("RefId0997872"));
    }
};

QTEST_APPLESS_MAIN(tst_QKnxProject)

#include "tst_qknxproject.moc"
