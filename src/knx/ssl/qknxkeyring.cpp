/******************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "qknxcryptographicengine.h"
#include "private/qknxkeyring_p.h"
#include "private/qknxssl_p.h"

#include <QtCore/qcryptographichash.h>
#include <QtCore/qregularexpression.h>
#include <QtCore/qfile.h>
#include <QtCore/qxmlstream.h>

#include <algorithm>

QT_BEGIN_NAMESPACE

/*!
    \internal
    \inmodule QtKnx
    \namespace QKnx::Ets::Keyring
*/

namespace QKnx { namespace Ets { namespace Keyring {

namespace QKnxPrivate
{
    void writeBytes(QByteArray *dest, const QStringRef &source)
    {
        dest->append(quint8(source.size()));
        dest->append(source.toString().toUtf8());
    }

    void writeBytes(QByteArray *dest, const QByteArray &source)
    {
        dest->append(quint8(source.size()));
        dest->append(source);
    }

    static void readElement(QXmlStreamReader *reader, QByteArray *bytes)
    {
        bytes->append(0x01);
        QKnxPrivate::writeBytes(bytes, reader->name());

        auto attributes = reader->attributes();
        std::sort(attributes.begin(), attributes.end(),
            [](const QXmlStreamAttribute &lhs, const QXmlStreamAttribute &rhs) {
                return lhs.name() < rhs.name();
        });

        for (auto attribute : qAsConst(attributes)) {
            QKnxPrivate::writeBytes(bytes, attribute.name());
            QKnxPrivate::writeBytes(bytes, attribute.value());
        }

        while (!reader->atEnd()) {
            auto token = reader->readNext();
            if (token == QXmlStreamReader::StartElement)
                QKnxPrivate::readElement(reader, bytes);
            if (token == QXmlStreamReader::EndElement)
                bytes->append(0x02);
        }
    }

    static bool fetchAttr(const QXmlStreamAttributes &attributes, const QString &attrName,
        QStringRef *value, QXmlStreamReader *reader)
    {
        if (!value || !reader)
            return false;

        *value = attributes.value(attrName);
        if (value->isNull())
            reader->raiseError(QKnxKeyring::tr("Invalid or empty attribute '%1'.").arg(attrName));
        return !reader->hasError();
    }

    static bool setString(const QString &name, const QStringRef &attr, int maxSize,
        QString *field, QXmlStreamReader *reader, bool pedantic)
    {
        if (!reader || !field)
            return false;

        if (pedantic && attr.size() > maxSize) {
            reader->raiseError(QKnxKeyring::tr("Pedantic error: Invalid value for attribute '%1', "
                "maximum length is %2 characters, got: '%3'.").arg(name).arg(maxSize).arg(attr.size()));
        } else {
            *field = attr.toString();
        }
        return !reader->hasError();
    }

    bool setString(const QString &name, const QStringRef &attr, const QStringList &list,
        QString *field, QXmlStreamReader *reader, bool pedantic)
    {
        if (!reader || !field)
            return false;

        if (pedantic && !list.contains(attr.toString())) {
            reader->raiseError(QKnxKeyring::tr("Pedantic error: Invalid value for attribute '%1', "
                "expected '%2', got: '%3'.").arg(name, list.join(QLatin1String(", "))));
        } else {
            *field = attr.toString();
        }
        return !reader->hasError();
    }
}

const char oneBlockBase64[29] = "^[A-Za-z0-9\\+/]{21}[AQgw]==$";
// const char twoBlockBase64[40] = "^[A-Za-z0-9\\+/]{42}[AEIMQUYcgkosw048]=$";

const char multicast[128] = "^2(2[4-9]|3[0-9])\\.((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|"
    "[0-9])\\.){2}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])$";

const char individualAddress[74] = "^((1[0-5]|[0-9])\\.){2}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]"
    "[0-9]|[0-9])$";

bool QKnxBackbone::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("Backbone")) {
        auto attrs = reader->attributes();

        QStringRef attr; // mandatory attributes
        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("MulticastAddress"), &attr, reader))
            return false;
        MulticastAddress = attr.toString();

        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("Latency"), &attr, reader))
            return false;
        Latency = attr.toUShort();

        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("Key"), &attr, reader))
            return false;
        Key = attr.toUtf8();

        if (pedantic) {
            if (!QRegularExpression(QLatin1String(multicast)).match(MulticastAddress).hasMatch()) {
                reader->raiseError(tr("The 'MulticastAddress' attribute is invalid. The Pattern "
                    "constraint failed, got: '%1'.").arg(MulticastAddress));
                return false;
            }

            if (Latency > 8000) {
                reader->raiseError(tr("The 'Latency' attribute is invalid. The MaxInclusive "
                    "constraint failed, got: '%1', MaxInclusive: '8000'.").arg(Latency));
                return false;
            }

            if (!QRegularExpression(QLatin1String(oneBlockBase64)).match(attr).hasMatch()) {
                reader->raiseError(tr("The 'Key' attribute is invalid. The Pattern constraint "
                    "failed, got: '%1'.").arg(QString::fromUtf8(Key)));
                return false;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Backbone>, got: <%1>.").arg(reader->name()
            .toString()));
    }
    return !reader->hasError();
}

bool QKnxInterface::QKnxGroup::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("Group")) {
        auto attrs = reader->attributes();

        QStringRef attr; // mandatory attributes
        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("Address"), &attr, reader))
            return false;
        Address = attr.toUShort();

        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("Senders"), &attr, reader))
            return false;
        Senders = attr.toString().split(QLatin1Char(' ')).toVector();
        if (pedantic) {
            QRegularExpression regExp;
            regExp.setPattern(QLatin1String(individualAddress));
            for (auto sender : qAsConst(Senders)) {
                if (!regExp.match(sender).hasMatch()) {
                    reader->raiseError(tr("The 'Senders' attribute is invalid. The Pattern "
                        "constraint failed, got: '%1'.").arg(sender));
                    return false;
                }
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Group>, got: <%1>.").arg(reader->name()
            .toString()));
    }
    return !reader->hasError();
}

bool QKnxInterface::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("Interface")) {
        auto attrs = reader->attributes();

        QStringRef attr; // mandatory attribute
        if (!QKnxPrivate::fetchAttr(attrs, QLatin1String("Type"), &attr, reader))
            return false;
        if (!QKnxPrivate::setString(QLatin1String("Type"), attr, {
                QStringLiteral("Backbone"),
                QStringLiteral("Tunneling"),
                QStringLiteral("USB")
            }, &Type, reader, pedantic)) return false;

        // optional attributes, TODO: pedantic
        Host = attrs.value(QStringLiteral("Host")).toString();
        IndividualAddress = attrs.value(QStringLiteral("IndividualAddress")).toString();
        UserID = attrs.value(QStringLiteral("UserID")).toUShort();
        Password = attrs.value(QStringLiteral("Password")).toUtf8();
        Authentication = attrs.value(QStringLiteral("Authentication")).toUtf8();

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("Group")) {
                    QKnxInterface::QKnxGroup group;
                    if (!group.parseElement(reader, pedantic))
                        return false;
                    Group.append(group);
               }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Interface"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Interface>, got: <%1>.").arg(reader->name()
            .toString()));
    }
    return !reader->hasError();
}

bool QKnxGroupAddresses::QKnxGroup::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("Group")) {
        auto attrs = reader->attributes();

        QStringRef attr; // mandatory attributes
        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("Address"), &attr, reader))
            return false;
        Address = attr.toUShort();

        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("Key"), &attr, reader))
            return false;
        Key = attr.toUtf8();
        if (pedantic && !QRegularExpression(QLatin1String(oneBlockBase64)).match(attr).hasMatch()) {
            reader->raiseError(tr("The 'Key' attribute is invalid. The Pattern "
                "constraint failed, got: '%1'.").arg(QString::fromUtf8(Key)));
            return false;
        }
    } else {
        reader->raiseError(tr("Expected element <Group>, got: <%1>.").arg(reader->name()
            .toString()));
    }
    return !reader->hasError();
}

bool QKnxGroupAddresses::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("GroupAddresses")) {
        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("Group")) {
                    QKnxGroupAddresses::QKnxGroup group;
                    if (!group.parseElement(reader, pedantic))
                        return false;
                    Group.append(group);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("GroupAddresses"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <GroupAddresses>, got: <%1>.").arg(reader->name()
            .toString()));
    }
    return !reader->hasError() && Group.size() >= 1;
}

bool QKnxDevice::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("Device")) {
        auto attrs = reader->attributes();

        QStringRef attr; // mandatory attribute
        if (!QKnxPrivate::fetchAttr(attrs, QLatin1String("IndividualAddress"), &attr, reader))
            return false;
        IndividualAddress = attr.toString();
        if (pedantic && !QRegularExpression(QLatin1String(individualAddress))
            .match(IndividualAddress).hasMatch()) {
                reader->raiseError(tr("The 'IndividualAddress' attribute is invalid. The "
                    "Pattern constraint failed, got: '%1'.").arg(IndividualAddress));
                return false;
        }

        // optional attributes, TODO: pedantic
        ToolKey = attrs.value(QStringLiteral("ToolKey")).toUtf8();
        SequenceNumber = attrs.value(QStringLiteral("SequenceNumber")).toULongLong();
        ManagementPassword = attrs.value(QStringLiteral("ManagementPassword")).toUtf8();
        Authentication = attrs.value(QStringLiteral("Authentication")).toUtf8();
    } else {
        reader->raiseError(tr("Expected element <Device>, got: <%1>.").arg(reader->name()
            .toString()));
    }
    return !reader->hasError();
}

bool QKnxDevices::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("Devices")) {
        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("Device")) {
                    QKnxDevice device;
                    if (!device.parseElement(reader, pedantic))
                        return false;
                    Device.append(device);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Devices"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Devices>, got: <%1>.").arg(reader->name()
            .toString()));
    }
    return !reader->hasError() && Device.size() >= 1;
}

bool QKnxKeyring::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->readNextStartElement())
        return false;

    if (reader->name() == QStringLiteral("Keyring")) {
        auto attrs = reader->attributes();

        QStringRef attr; // mandatory attributes
        if (!QKnxPrivate::fetchAttr(attrs, QLatin1String("Project"), &attr, reader))
            return false;
        if (!QKnxPrivate::setString(QLatin1String("Project"), attr, 255, &Project, reader, pedantic))
            return false;

        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("CreatedBy"), &attr, reader))
            return false;
        CreatedBy = attr.toString();

        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("Created"), &attr, reader))
            return false;
        Created = attr.toString();

        if (!QKnxPrivate::fetchAttr(attrs, QStringLiteral("Signature"), &attr, reader))
            return false;
        Signature = attr.toUtf8();
        if (pedantic) {
            if (!QRegularExpression(QLatin1String(oneBlockBase64)).match(attr).hasMatch()) {
                reader->raiseError(tr("The 'Signature' attribute is invalid. The Pattern "
                    "constraint failed, got: '%1'.").arg(attr));
                return false;
            }
        }

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("Backbone")) {
                    if (pedantic && Backbone.size() >= 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element <Backbone> "
                            "more than once."));
                        return false;
                    }
                    QKnxBackbone backbone;
                    if (!backbone.parseElement(reader, pedantic))
                        return false;
                    Backbone.append(backbone);
                } else if (reader->name() == QStringLiteral("Interface")) {
                    QKnxInterface interface;
                    if (!interface.parseElement(reader, pedantic))
                        return false;
                    Interface.append(interface);
                } else if (reader->name() == QStringLiteral("GroupAddresses")) {
                    if (pedantic && GroupAddresses.size() >= 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element "
                            "<GroupAddresses> more than once."));
                        return false;
                    }
                    QKnxGroupAddresses groupAddresses;
                    if (!groupAddresses.parseElement(reader, pedantic))
                        return false;
                    GroupAddresses.append(groupAddresses);
                } else if (reader->name() == QStringLiteral("Devices")) {
                    if (pedantic && Devices.size() >= 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element "
                            "<Devices> more than once."));
                        return false;
                    }
                    QKnxDevices devices;
                    if (!devices.parseElement(reader, pedantic))
                        return false;
                    Devices.append(devices);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Keyring"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Keyring>, got: <%1>.").arg(reader->name()
            .toString()));
    }
    return !reader->hasError();
}

bool QKnxKeyring::validate(QXmlStreamReader *reader, const QKnxByteArray &pwHash) const
{
    if (!reader || !reader->readNextStartElement())
        return false;

    if (reader->name() != QStringLiteral("Keyring"))
        return false;

    QByteArray bytes;
    bytes.append(0x01);

    QKnxPrivate::writeBytes(&bytes, reader->name());

    auto attributes = reader->attributes();
    if (attributes.isEmpty())
        return false;

    auto signature = attributes.value({}, QStringLiteral("Signature")).toUtf8();
    if (signature.isEmpty())
        return false;

    std::sort(attributes.begin(), attributes.end(),
        [](const QXmlStreamAttribute &lhs, const QXmlStreamAttribute &rhs) {
            return lhs.name() < rhs.name();
    });

    for (auto attribute : attributes) {
        if (attribute.name() == QStringLiteral("xmlns"))
            continue;
        if (attribute.name() == QStringLiteral("Signature"))
            continue;

        QKnxPrivate::writeBytes(&bytes, attribute.name());
        QKnxPrivate::writeBytes(&bytes, attribute.value());
    }

    if (reader->readNextStartElement())
        QKnxPrivate::readElement(reader, &bytes);
    QKnxPrivate::writeBytes(&bytes, pwHash.toByteArray().toBase64());

    return QKnxCryptographicEngine::hashSha256(bytes).left(16)
        == QKnxByteArray::fromByteArray(QByteArray::fromBase64(signature));
}

}}} // QKnx::Ets::Keyring

QT_END_NAMESPACE
