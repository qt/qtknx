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

#include "qknxbuildings.h"

QT_BEGIN_NAMESPACE

// -- QKnxGroupAddressRef

bool QKnxGroupAddressRef::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("GroupAddressRef")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Id"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("Id"), attr, &Id, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 255, &Name, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("RefId"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("RefId"), attr, &RefId, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Puid"), &attr, reader))
            return false;
        Puid = attr.toUInt();

        // optional attributes
        Role = attrs.value(QStringLiteral("Role")).toString(); // TODO: pedantic, 255 character max.

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <GroupAddressRef>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}

// -- QKnxFunction

bool QKnxFunction::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("Function")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Id"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("Id"), attr, &Id, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 255, &Name, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Puid"), &attr, reader))
            return false;
        Puid = attr.toUInt();

        // optional attributes
        Type = attrs.value(QStringLiteral("Type")).toString(); // TODO: pedantic
        Number = attrs.value(QStringLiteral("Number")).toString(); // TODO: pedantic
        Comment = attrs.value(QStringLiteral("Comment")).toString();
        Description = attrs.value(QStringLiteral("Description")).toString();

        attr = attrs.value(QStringLiteral("CompletionStatus")); // TODO: pedantic
        if (!attr.isNull())
            CompletionStatus = attr.toString();

        // TODO: pedantic
        DefaultGroupRange = attrs.value(QStringLiteral("DefaultGroupRange")).toString();

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("GroupAddressRef")) {
                    QKnxGroupAddressRef ref;
                    if (!ref.parseElement(reader, pedantic))
                        return false;
                    GroupAddressRef.append(ref);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Function"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Function>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxBuildingPart

bool QKnxBuildingPart::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("BuildingPart")) {
       auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Id"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("Id"), attr, &Id, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 255, &Name, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Type"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Type"), attr, {
                QStringLiteral("Building"), QStringLiteral("BuildingPart"),
                QStringLiteral("Floor"), QStringLiteral("Room"),
                QStringLiteral("DistributionBoard"), QStringLiteral("Stairway"),
                QStringLiteral("Corridor")
            }, &Type, reader, pedantic)) return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Puid"), &attr, reader))
            return false;
        Puid = attr.toUInt();

        Type = attrs.value(QStringLiteral("Type")).toString(); // TODO: pedantic
        Number = attrs.value(QStringLiteral("Number")).toString(); // TODO: pedantic
        Comment = attrs.value(QStringLiteral("Comment")).toString();
        Description = attrs.value(QStringLiteral("Description")).toString();

        attr = attrs.value(QStringLiteral("CompletionStatus")); // TODO: pedantic
        if (!attr.isNull())
            CompletionStatus = attr.toString();

        // TODO: pedantic
        DefaultLine = attrs.value(QStringLiteral("DefaultLine")).toString();

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QLatin1String("BuildingPart")) {
                    QKnxBuildingPart part;
                    if (!part.parseElement(reader, pedantic))
                        return false;
                    BuildingPart.append(part);
                } else if (reader->name() == QStringLiteral("DeviceInstanceRef")) {
                    QStringRef attr;
                    if (!QKnxProjectUtils::fetchAttr(reader->attributes(), QLatin1String("RefId"),
                        &attr, reader)) return false;
                    QString refId;
                    if (!QKnxProjectUtils::setNCName(QLatin1String("RefId"), attr, &refId, reader,
                        pedantic)) return false;
                    DeviceInstanceRef.append(refId);
                } else if (reader->name() == QLatin1String("Function")) {
                    QKnxFunction function;
                    if (!function.parseElement(reader, pedantic))
                        return false;
                    Function.append(function);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("BuildingPart"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <BuildingPart>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxBuildings

bool QKnxBuildings::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("Buildings")) {
        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QLatin1String("BuildingPart")) {
                    QKnxBuildingPart part;
                    if (!part.parseElement(reader, pedantic))
                        return false;
                    BuildingPart.append(part);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Buildings"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Buildings>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}

QT_END_NAMESPACE
