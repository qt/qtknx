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

#include "qknxprojectinformation.h"

QT_BEGIN_NAMESPACE

// -- QKnxHistoryValue

bool QKnxHistoryEntry::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("HistoryEntry")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Date"), &attr, reader))
            return false;
        Date = QDateTime::fromString(attr.toString(), Qt::ISODateWithMs);

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Text"), &attr, reader))
            return false;
        Text = attr.toString();

        // optional attributes
        attr = attrs.value(QLatin1String("User"));
        if (!attr.isNull() && !QKnxProjectUtils::setString(QLatin1String("User"), attr, 50, &User,
            reader, pedantic)) return false;

        Detail = attrs.value(QStringLiteral("Detail")).toString();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <HistoryEntry>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxToDoItem

bool QKnxToDoItem::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("ToDoItem")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Description"), &attr, reader))
            return false;
        Description = attr.toString();

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Status"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Status"), attr, { QStringLiteral("Open"),
            QStringLiteral("Accomplished") }, &Status, reader, pedantic)) return false;

        // optional attributes
        ObjectPath = attrs.value(QStringLiteral("ObjectPath")).toString();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <ToDoItem>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxProjectTrace

bool QKnxProjectTrace::parseElement(QXmlStreamReader *reader, bool /*pedantic*/)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("ProjectTrace")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Date"), &attr, reader))
            return false;
        Date = QDateTime::fromString(attr.toString(), Qt::ISODateWithMs);

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("UserName"), &attr, reader))
            return false;
        UserName = attr.toString();

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Comment"), &attr, reader))
            return false;
        Comment = attr.toString();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <ProjectTrace>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxDeviceCertificate

bool QKnxDeviceCertificate::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("DeviceCertificate")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("SerialNumber"), &attr, reader))
            return false;
        SerialNumber = attr.toUtf8();
        if (pedantic && SerialNumber.size() % 4 != 0) {
            reader->raiseError(tr("Invalid value for attribute 'SerialNumber', Base64 encoded "
                "string size must be a multiple of 4, got: '%1'.").arg(SerialNumber.size()));
            return false;
        }

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("FDSK"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("FDSK"), attr, 40, &FDSK, reader, pedantic))
            return false;

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <DeviceCertificate>, got: <%1>.")
            .arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxUserFile

bool QKnxUserFile::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("UserFile")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Filename"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Filename"), attr, 255, &Filename,
            reader, pedantic)) return false;

        // optional attributes
        Comment = attrs.value(QStringLiteral("Comment")).toString();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <UserFile>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxProjectInformation

bool QKnxProjectInformation::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("ProjectInformation")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 50, &Name, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("GroupAddressStyle"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("GroupAddressStyle"), attr, {
                QStringLiteral("TwoLevel"), QStringLiteral("ThreeLevel"), QStringLiteral("Free")
            }, &GroupAddressStyle, reader, pedantic)) return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Guid"), &attr, reader))
            return false;
        Guid = attr.toString();

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("LastUsedPuid"), &attr, reader))
            return false;
        LastUsedPuid = attr.toUInt();

        // optional attributes
        ProjectNumber = attrs.value(QStringLiteral("ProjectNumber")).toString(); // TODO: pedantic
        ContractNumber = attrs.value(QStringLiteral("ContractNumber")).toString(); // TODO: pedantic
        LastModified = QDateTime::fromString(attrs.value(QStringLiteral("LastModified"))
            .toString(), Qt::ISODateWithMs);
        ProjectStart = QDateTime::fromString(attrs.value(QStringLiteral("ProjectStart"))
            .toString(), Qt::ISODateWithMs);
        ProjectEnd = QDateTime::fromString(attrs.value(QStringLiteral("ProjectEnd"))
            .toString(), Qt::ISODateWithMs);
        ProjectId = attrs.value(QStringLiteral("ProjectId")).toUShort(); // TODO: pedantic
        ProjectPassword = attrs.value(QStringLiteral("ProjectPassword")).toString(); // TODO: pedantic
        Comment = attrs.value(QStringLiteral("Comment")).toString();

        attr = attrs.value(QStringLiteral("CompletionStatus")); // TODO: pedantic
        if (!attr.isNull())
            CompletionStatus = attr.toString();

        attr = attrs.value(QStringLiteral("ProjectTracingLevel")); // TODO: pedantic
        if (!attr.isNull())
            ProjectTracingLevel = attr.toString();

        ProjectTracingPassword = attrs.value(QStringLiteral("ProjectTracingPassword"))
            .toString(); // TODO: pedantic

        attr = attrs.value(QStringLiteral("Hide16BitGroupsFromLegacyPlugins"));
        if (!attr.isNull())
            Hide16BitGroupsFromLegacyPlugins = attr.toString() == QLatin1String { "true" };

        CodePage = attrs.value(QStringLiteral("CodePage")).toString(); // TODO: pedantic
        BusAccessLegacyMode = attrs.value(QStringLiteral("BusAccessLegacyMode"))
            .toString() == QLatin1String { "true" };

        DeviceCount = attrs.value(QStringLiteral("DeviceCount")).toUInt(); // not declared

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("HistoryEntries")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &HistoryEntries))
                        return false;
                } else if (reader->name() == QStringLiteral("ToDoItems")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &ToDoItems))
                        return false;
                } else if (reader->name() == QStringLiteral("ProjectTraces")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &ProjectTraces))
                        return false;
                } else if (reader->name() == QStringLiteral("DeviceCertificates")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &DeviceCertificates))
                        return false;
                } else if (reader->name() == QStringLiteral("UserFiles")) { // not declared
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &UserFiles))
                        return false;
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("ProjectInformation"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <ProjectInformation>, got: <%1>.")
            .arg(reader->name()));
    }
    return !reader->hasError();
}

QT_END_NAMESPACE

