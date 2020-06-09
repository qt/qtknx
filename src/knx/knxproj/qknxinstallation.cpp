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

#include "qknxinstallation_p.h"
#include "qregularexpression.h"

QT_BEGIN_NAMESPACE

// -- QKnxTrade

bool QKnxTrade::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("Trade")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringView attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 255, &Name, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Puid"), &attr, reader))
            return false;
        Puid = attr.toUInt();

        // optional attributes
        Id = attrs.value(QStringLiteral("Id")).toString(); // TODO: pedantic
        Number = attrs.value(QStringLiteral("Number")).toString(); // TODO: pedantic
        Comment = attrs.value(QStringLiteral("Comment")).toString();

        attr = attrs.value(QStringLiteral("CompletionStatus")); // TODO: pedantic
        if (!attr.isNull())
            CompletionStatus = attr.toString();

        Description = attrs.value(QStringLiteral("Description")).toString();

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QLatin1String("Trade")) {
                    QKnxTrade trade;
                    if (!trade.parseElement(reader, pedantic))
                        return false;
                    Trade.append(trade);
                } else if (reader->name() == QStringLiteral("DeviceInstanceRef")) {
                    QStringView attr;
                    if (!QKnxProjectUtils::fetchAttr(reader->attributes(), QStringLiteral("RefId"),
                        &attr, reader)) return false;
                    QString refId;
                    if (!QKnxProjectUtils::setNCName(QLatin1String("RefId"), attr, &refId, reader,
                        pedantic)) return false;
                    DeviceInstanceRef.append(refId);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Trade"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Trade>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxSplitInfo

const char pattern[65] = "^[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}$";

bool QKnxSplitInfo::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("SplitInfo")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringView attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("ObjectPath"), &attr, reader))
            return false;
        ObjectPath = attr.toString();

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Cookie"), &attr, reader))
            return false;
        if (pedantic && !QRegularExpression(QLatin1String(pattern)).match(attr).hasMatch())
            return false;
        Cookie = attr.toString();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <SplitInfo>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxInstallation

bool QKnxInstallation::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("Installation")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringView attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 50, &Name, reader,
            pedantic)) return false;

        // optional attributes
        InstallationId = attrs.value(QStringLiteral("InstallationId")).toUShort(); // TODO: pedantic

        attr = attrs.value(QStringLiteral("BCUKey")); // TODO: pedantic
        if (!attr.isNull())
            BCUKey = attr.toULong();

        attr = attrs.value(QStringLiteral("IPRoutingMulticastAddress")); // TODO: pedantic
        if (!attr.isNull())
            IPRoutingMulticastAddress = attr.toString();

        attr = attrs.value(QStringLiteral("MulticastTTL")); // TODO: pedantic
        if (!attr.isNull())
            MulticastTTL = quint8(attr.toUShort()); // optional

        attr = attrs.value(QStringLiteral("IPRoutingBackboneKey")); // TODO: pedantic
        if (!attr.isNull())
            IPRoutingBackboneKey = attr.toString();

        IPRoutingLatencyTolerance = attrs.value(QStringLiteral("IPRoutingLatencyTolerance"))
            .toUShort();

        attr = attrs.value(QStringLiteral("IPSyncLatencyFraction")); // TODO: pedantic
        if (!attr.isNull())
            IPSyncLatencyFraction = attr.toFloat();

        DefaultLine = attrs.value(QStringLiteral("DefaultLine")).toString();

        attr = attrs.value(QStringLiteral("CompletionStatus")); // TODO: pedantic
        if (!attr.isNull())
            CompletionStatus = attr.toString();

        attr = attrs.value(QLatin1String("IPRoutingBackboneSecurity"));
        if (!attr.isNull() && !QKnxProjectUtils::setString(QLatin1String("IPRoutingBackboneSecurity"),
            attr, 40, &IPRoutingBackboneSecurity, reader, pedantic)) return false;

        attr = attrs.value(QStringLiteral("IPRoutingBackboneSecurity")); // TODO: pedantic
        if (!attr.isNull())
            IPRoutingBackboneSecurity = attr.toString();

        attr = attrs.value(QStringLiteral("SplitType")); // TODO: pedantic
        if (!attr.isNull())
            SplitType = attr.toString();

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("Topology")) {
                    if (pedantic && Topology.size() >= 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element "
                            "<Topology> more than once."));
                        return false;
                    }
                    QKnxTopology topology;
                    if (!topology.parseElement(reader, pedantic))
                        return false;
                    Topology.append(topology);
                } else if (reader->name() == QStringLiteral("Buildings")
                    || reader->name() == QStringLiteral("Locations")) {
                        if (pedantic && Buildings.size() >= 1) {
                            reader->raiseError(tr("Pedantic error: Encountered element "
                                "<%1> more than once.").arg(reader->name()));
                            return false;
                        }
                        QKnxBuildings buildings;
                        if (!buildings.parseElement(reader, pedantic))
                            return false;
                        Buildings.append(buildings);
                } else if (reader->name() == QStringLiteral("GroupAddresses")) {
                    if (pedantic && GroupAddresses.size() >= 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element "
                            "<GroupAddresses> more than once."));
                        return false;
                    }
                    QKnxGroupAddresses addresses;
                    if (!addresses.parseElement(reader, pedantic))
                        return false;
                    GroupAddresses.append(addresses);
                } else if (reader->name() == QStringLiteral("Trades")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &Trades))
                        return false;
                } else if (reader->name() == QStringLiteral("BusAccess")) {
                    if (pedantic && BusAccess.size() >= 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element <BusAccess> "
                            "more than once."));
                        return false;
                    }
                    QKnxBusAccess busAccess;
                    if (!busAccess.parseElement(reader, pedantic))
                        return false;
                    BusAccess.append(busAccess);
                } else if (reader->name() == QStringLiteral("SplitInfos")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &SplitInfos))
                        return false;
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Installation"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Installation>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}

QT_END_NAMESPACE
