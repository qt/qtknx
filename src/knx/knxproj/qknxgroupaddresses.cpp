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

#include "qknxgroupaddresses.h"

QT_BEGIN_NAMESPACE

static bool fetchBool(const QXmlStreamAttributes &attrs, const QString &name)
{
    auto attr = attrs.value(name);
    if (attr.isNull())
        return false;
    return attrs.value(name).toString() == QStringLiteral("true");
};

// -- QKnxGroupAddress

bool QKnxGroupAddress::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("GroupAddress")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Id"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("Id"), attr, &Id, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Address"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNumber<quint32>(QLatin1String("Address"), attr.toUInt(), 65535,
            &Address, reader, pedantic)) return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 255, &Name, reader,
            pedantic)) return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Puid"), &attr, reader))
            return false;
        Puid = attr.toUInt();

        // optional attributes
        Unfiltered = fetchBool(attrs, QStringLiteral("Unfiltered"));
        Central = fetchBool(attrs, QStringLiteral("Central"));
        Global = fetchBool(attrs, QStringLiteral("Global"));

        attr = attrs.value(QStringLiteral("DatapointType"));
        if (pedantic && !attr.isNull() && !QKnxProjectUtils::isNCName(attr.toString())) {
            reader->raiseError(tr("Pedantic error: DatapointType is not a valid xs:ID, got '%1'.")
                .arg(attr));
            return false;
        }
        DatapointType = attr.toString();

        Description = attrs.value(QStringLiteral("Description")).toString();
        Comment = attrs.value(QStringLiteral("Comment")).toString();

        attr = attrs.value(QLatin1String("Key"));
        if (!attr.isNull() && !QKnxProjectUtils::setString(QLatin1String("Key"), attr, 40, &Key,
            reader, pedantic)) return false;

        attr = attrs.value(QStringLiteral("Security")); // TODO: pedantic
        if (!attr.isNull())
            Security = attr.toString();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <GroupAddress>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}

// -- QKnxGroupRange

bool QKnxGroupRange::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("GroupRange")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringRef attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Id"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("Id"), attr, &Id, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 255, &Name, reader,
            pedantic)) return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("RangeStart"), &attr, reader))
            return false;
        RangeStart = attr.toUShort();
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("RangeEnd"), &attr, reader))
            return false;
        RangeEnd = attr.toUShort();

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Puid"), &attr, reader))
            return false;
        Puid = attr.toUInt();

        // optional attributes;
        Unfiltered = fetchBool(attrs, QStringLiteral("Unfiltered"));
        Description = attrs.value(QStringLiteral("Description")).toString();
        Comment = attrs.value(QStringLiteral("Comment")).toString();

        attr = attrs.value(QStringLiteral("Security")); // TODO: pedantic
        if (!attr.isNull())
            Security = attr.toString();

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QLatin1String("GroupRange")) {
                    QKnxGroupRange range;
                    if (!range.parseElement(reader, pedantic))
                        return false;
                    GroupRange.append(range);
                } else if (reader->name() == QStringLiteral("GroupAddress")) {
                    QKnxGroupAddress address;
                    if (!address.parseElement(reader, pedantic))
                        return false;
                    GroupAddress.append(address);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("GroupRange"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <GroupRange>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxGroupAddresses

bool QKnxGroupAddresses::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("GroupAddresses")) {
        // child
        if (reader->readNextStartElement() && reader->name() != QLatin1String("GroupRanges")) {
            reader->raiseError(tr("Expected element <GroupRanges>, got: <%1>.")
                .arg(reader->name()));
            return false;
        }

        if (pedantic && GroupRanges.size() > 0) {
            reader->raiseError(tr("Pedantic error: Encountered element <GroupRanges> "
                "more then once."));
            return false;
        }

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("GroupRange")) {
                    if (pedantic && GroupRanges.size() >= 65535) {
                        reader->raiseError(tr("Pedantic error: Encountered element <GroupRange> "
                            "more then 65535 times."));
                        return false;
                    }
                    QKnxGroupRange range;
                    if (!range.parseElement(reader, pedantic))
                        return false;
                    GroupRanges.append(range);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("GroupRanges"))
                    break;
            }
        }

        reader->skipCurrentElement(); // force reading until GroupAddresses end element
    } else {
        reader->raiseError(tr("Expected element <GroupAddresses>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}

QT_END_NAMESPACE
