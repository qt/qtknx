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

#include "qknxtopology.h"
#include "qregularexpression.h"

QT_BEGIN_NAMESPACE

const char pattern[67] = "^\\{[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}\\}$";

// -- QKnxBusAccess

bool QKnxBusAccess::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("BusAccess")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Name"), &attr, reader))
            return false;
        Name = attr.toString();

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Edi"), &attr, reader))
            return false;
        if (pedantic && !QRegularExpression(QLatin1String(pattern)).match(attr).hasMatch())
            return false;
        Edi = attr.toString();

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Parameter"), &attr, reader))
            return false;
        Parameter = attr.toString();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <BusAccess>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxAdditionalGroupAddress

bool QKnxAdditionalGroupAddress::parseElement(QXmlStreamReader *reader, bool /*pedantic*/)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("GroupAddress")) {
        QStringRef attr; // required attribute
        if (!QKnxProjectUtils::fetchAttr(reader->attributes(), QLatin1String("Address"), &attr,
            reader)) return false;
        Address = attr.toUShort();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <GroupAddress>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxLine

bool QKnxLine::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("Line")) {
         auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Id"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("Id"), attr, &Id, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 255, &Name, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("MediumTypeRefId"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("MediumTypeRefId"), attr, &MediumTypeRefId,
            reader, pedantic)) return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Puid"), &attr, reader))
            return false;
        Puid = attr.toUInt();

        // optional attributes
        Address = attrs.value(QStringLiteral("Address")).toInt(); // TODO: pedantic
        Comment = attrs.value(QStringLiteral("Comment")).toString();
        DomainAddress = attrs.value(QStringLiteral("DomainAddress")).toInt();

        attr = attrs.value(QStringLiteral("CompletionStatus")); // TODO: pedantic
        if (!attr.isNull())
            CompletionStatus = attr.toString();

        Description = attrs.value(QStringLiteral("Description")).toString();

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("DeviceInstance")) {
                    QKnxDeviceInstance device;
                    if (!device.parseElement(reader, pedantic))
                        return false;
                    DeviceInstance.append(device);
                } else if (reader->name() == QStringLiteral("BusAccess")) {
                    if (pedantic && BusAccess.size() >= 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element <BusAccess> "
                            "more then once."));
                        return false;
                    }
                    QKnxBusAccess bus;
                    if (!bus.parseElement(reader, pedantic))
                        return false;
                    BusAccess.append(bus);
                } else if (reader->name() == QStringLiteral("AdditionalGroupAddresses")) {
                    if (pedantic && AdditionalGroupAddresses.size() > 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element "
                            "<AdditionalGroupAddresses> more then once."));
                        return false;
                    }
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic,
                        &AdditionalGroupAddresses)) return false;
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Line"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Line>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxArea

bool QKnxArea::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("Area")) {
         auto attrs = reader->attributes();

         // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 255, &Name, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Puid"), &attr, reader))
            return false;
        Puid = attr.toUInt();

        // optional attributes
        Id = attrs.value(QStringLiteral("Id")).toString(); // TODO: pedantic
        Address = attrs.value(QStringLiteral("Address")).toInt(); // TODO: pedantic
        Comment = attrs.value(QStringLiteral("Comment")).toString();

        attr = attrs.value(QStringLiteral("CompletionStatus")); // TODO: pedantic
        if (!attr.isNull())
            CompletionStatus = attr.toString();

        Description = attrs.value(QStringLiteral("Description")).toString();

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("Line")) {
                    if (pedantic && Line.size() >= 16) {
                        reader->raiseError(tr("Pedantic error: Encountered element <Line> more "
                            "then sixteen times."));
                        return false;
                    }
                    QKnxLine line;
                    if (!line.parseElement(reader, pedantic))
                        return false;
                    Line.append(line);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Area"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Area>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxTopology

bool QKnxTopology::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("Topology")) {
        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("Area")) {
                    if (pedantic && Area.size() >= 16) {
                        reader->raiseError(tr("Pedantic error: Encountered element "
                            "<Area> more then sixteen times."));
                        return false;
                    }
                    QKnxArea area;
                    if (!area.parseElement(reader, pedantic))
                        return false;
                    Area.append(area);
                } else if (reader->name() == QStringLiteral("UnassignedDevices")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &UnassignedDevices))
                        return false;
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Topology"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Topology>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}

QT_END_NAMESPACE
