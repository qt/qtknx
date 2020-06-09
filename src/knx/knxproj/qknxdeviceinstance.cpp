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

#include "qknxdeviceinstance_p.h"

QT_BEGIN_NAMESPACE

static bool fetchBool(const QXmlStreamAttributes &attrs, const QString &name)
{
    auto attr = attrs.value(name);
    if (attr.isNull())
        return false;
    return attrs.value(name).toString() == QStringLiteral("true");
};

// -- QKnxParameterInstanceRef

bool QKnxParameterInstanceRef::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("ParameterInstanceRef")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringView attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("RefId"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("RefId"), attr, &RefId, reader, pedantic))
            return false;

        // optional attributes
        Id = attrs.value(QStringLiteral("Id")).toString(); // TODO: pedantic
        Value = attrs.value(QStringLiteral("Value")).toString();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <ParameterInstanceRef>, got: <%1>.")
            .arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxConnectors

bool QKnxConnectors::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    auto fetchAttributes = [&](const QXmlStreamAttributes &attrs, QKnxConnector *connector) -> bool
    {
        // required attributes
        QStringView attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("GroupAddressRefId"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("GroupAddressRefId"), attr,
            &(connector->GroupAddressRefId), reader, pedantic)) return false;

        // optional attributes
        connector->Acknowledge = fetchBool(attrs, QStringLiteral("Acknowledge"));
        return true;
    };

    // children
    if (reader->name() == QLatin1String("Connectors")) {
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("Send")) {
                    if (pedantic && !Send.GroupAddressRefId.isNull()) {
                        reader->raiseError(tr("Pedantic error: Encountered element <Send> "
                            "more than once."));
                        return false;
                    }
                    if (!fetchAttributes(reader->attributes(), &Send))
                        return false;
                } else if (reader->name() == QStringLiteral("Receive")) {
                    QKnxConnector receive;
                    if (!fetchAttributes(reader->attributes(), &receive))
                        return false;
                    Receive.append(receive);
                }
                reader->skipCurrentElement(); // attribute elements only
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Connectors"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Connectors>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxComObjectInstanceRef

bool QKnxComObjectInstanceRef::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("ComObjectInstanceRef")) {
        auto attrs = reader->attributes();

        // optional attributes
        Id = attrs.value(QStringLiteral("Id")).toString(); // TODO: pedantic
        RefId = attrs.value(QStringLiteral("RefId")).toString(); // TODO: pedantic
        Text = attrs.value(QStringLiteral("Text")).toString(); // TODO: pedantic
        FunctionText = attrs.value(QStringLiteral("FunctionText")).toString(); // TODO: pedantic
        Priority = attrs.value(QStringLiteral("Priority")).toString(); // TODO: pedantic
        ReadFlag = attrs.value(QStringLiteral("ReadFlag")).toString(); // TODO: pedantic
        WriteFlag = attrs.value(QStringLiteral("WriteFlag")).toString(); // TODO: pedantic
        CommunicationFlag = attrs.value(QStringLiteral("CommunicationFlag")).toString(); // pedantic
        TransmitFlag = attrs.value(QStringLiteral("TransmitFlag")).toString(); // TODO: pedantic
        UpdateFlag = attrs.value(QStringLiteral("UpdateFlag")).toString(); // TODO: pedantic
        ReadOnInitFlag = attrs.value(QStringLiteral("ReadOnInitFlag")).toString(); // TODO: pedantic

        auto attr = attrs.value(QStringLiteral("DatapointType")).toString();
        DatapointType = attr.split(QLatin1Char(' ')).toVector(); // TODO: pedantic

        Description = attrs.value(QStringLiteral("Description")).toString();
        IsActive = fetchBool(attrs, QStringLiteral("IsActive"));
        ChannelId = attrs.value(QStringLiteral("ChannelId")).toString(); // TODO: pedantic

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("Connectors")) {
                    if (pedantic && Connectors.size() > 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element <Connectors> "
                            "more than once."));
                        return false;
                    }
                    QKnxConnectors connectors;
                    if (!connectors.parseElement(reader, pedantic))
                        return false;
                    Connectors.append(connectors);
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("ComObjectInstanceRef"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <ComObjectInstanceRef>, got: <%1>.")
            .arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxChannelInstance

bool QKnxChannelInstance::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("ChannelInstance")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringView attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Id"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("Id"), attr, &Id, reader, pedantic))
            return false;

        // optional attributes
        RefId = attrs.value(QStringLiteral("RefId")).toString(); // TODO: pedantic
        Name = attrs.value(QStringLiteral("Name")).toString(); // TODO: pedantic
        Description = attrs.value(QStringLiteral("Description")).toString(); // TODO: pedantic
        IsActive = fetchBool(attrs, QStringLiteral("IsActive"));

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <ChannelInstance>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxAdditionalAddress

bool QKnxAdditionalAddress::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("Address")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringView attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Address"), &attr, reader))
            return false;
        auto address = attr.toUInt();
        if (pedantic && (address < 1 || address > 255))
            return false;
        Address = quint8(address);

        // optional attributes
        Name = attrs.value(QStringLiteral("Name")).toString(); // TODO: pedantic
        Description = attrs.value(QStringLiteral("Description")).toString();
        Comment = attrs.value(QStringLiteral("Comment")).toString();

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <Address>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxBinaryData

bool QKnxBinaryData::parseElement(QXmlStreamReader *reader, bool /*pedantic*/)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("BinaryData")) {
        auto attrs = reader->attributes();

        // optional attributes
        Id = attrs.value(QStringLiteral("Id")).toString();
        RefId = attrs.value(QStringLiteral("RefId")).toString(); // TODO: pedantic
        Name = attrs.value(QStringLiteral("Name")).toString(); // TODO: pedantic
        Data = attrs.value(QStringLiteral("Name")).toUtf8(); // TODO: pedantic

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <BinaryData>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxIpConfig

bool QKnxIpConfig::parseElement(QXmlStreamReader *reader, bool /*pedantic*/)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("IPConfig")) {
        auto attrs = reader->attributes();

        // optional attributes
        auto attr = attrs.value(QStringLiteral("Assign"));
        if (!attr.isNull())
            Assign = attr.toString(); // TODO: pedantic
        IPAddress = attrs.value(QStringLiteral("IPAddress")).toString(); // TODO: pedantic
        SubnetMask = attrs.value(QStringLiteral("SubnetMask")).toString(); // TODO: pedantic
        DefaultGateway = attrs.value(QStringLiteral("DefaultGateway")).toString(); // TODO: pedantic
        MacAddress = attrs.value(QStringLiteral("MACAddress")).toString(); // TODO: pedantic

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <IPConfig>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxSecurity

bool QKnxSecurity::parseElement(QXmlStreamReader *reader, bool /*pedantic*/)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("Security")) {
        auto attrs = reader->attributes();

        // optional attributes
        LoadedIPRoutingBackboneKey = attrs.value(QStringLiteral("LoadedIPRoutingBackboneKey"))
            .toString(); // TODO: pedantic
        DeviceAuthenticationCode = attrs.value(QStringLiteral("DeviceAuthenticationCode"))
            .toString(); // TODO: pedantic
        LoadedDeviceAuthenticationCode = attrs
            .value(QStringLiteral("LoadedDeviceAuthenticationCode")).toString(); // TODO: pedantic
        DeviceManagementPassword = attrs.value(QStringLiteral("DeviceManagementPassword"))
            .toString(); // TODO: pedantic
        LoadedDeviceManagementPassword = attrs
            .value(QStringLiteral("LoadedDeviceManagementPassword")).toString(); // TODO: pedantic
        ToolKey = attrs.value(QStringLiteral("ToolKey")).toString(); // TODO: pedantic
        LoadedToolKey = attrs.value(QStringLiteral("LoadedToolKey")).toString(); // TODO: pedantic
        SequenceNumber = attrs.value(QStringLiteral("SequenceNumber")).toUInt();
        SequenceNumberTimestamp = QDateTime::fromString(attrs
            .value(QStringLiteral("SequenceNumberTimestamp")).toString(), Qt::ISODateWithMs);

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <Security>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxBusInterface

bool QKnxBusInterface::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("BusInterface")) {
        auto attrs = reader->attributes();

        // optional attributes
        Name = attrs.value(QStringLiteral("Name")).toString(); // TODO: pedantic
        Description = attrs.value(QStringLiteral("Description")).toString();
        Comment = attrs.value(QStringLiteral("Comment")).toString();
        Password = attrs.value(QStringLiteral("Password")).toString(); // TODO: pedantic

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QLatin1String("Connectors")) {
                    if (pedantic && Connectors.size() > 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element <Connectors> "
                            "more than once."));
                        return false;
                    }

                    while (!reader->atEnd() && !reader->hasError()) {
                        auto tokenType = reader->readNext();
                        if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                            if (reader->name() != QStringLiteral("Connector")) {
                                if (pedantic) {
                                    reader->raiseError(tr("Pedantic error: Expected element "
                                        "<Connector, got: <%1>.").arg(reader->name()));
                                    return false;
                                }
                                reader->skipCurrentElement();
                                continue;
                            }
                            QStringView attr;
                            if (!QKnxProjectUtils::fetchAttr(reader->attributes(),
                                QStringLiteral("GroupAddressRefId"), &attr, reader)) return false;
                            if (pedantic && !QKnxProjectUtils::isNCName(attr.toString()))
                                return false;
                            Connectors.append(attr.toString());
                            reader->skipCurrentElement(); // attributes only element
                        } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                            if (reader->name() == QLatin1String("Connectors"))
                                break;
                        }
                    }
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("BusInterface"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <BusInterface>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxDeviceInstance

bool QKnxDeviceInstance::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("DeviceInstance")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringView attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Id"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("Id"), attr, &Id, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("ProductRefId"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("ProductRefId"), attr, &ProductRefId,
            reader, pedantic)) return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QStringLiteral("Puid"), &attr, reader))
            return false;
        Puid = attr.toUInt();

        // optional attributes
        IsCommunicationObjectVisibilityCalculated = fetchBool(attrs,
            QStringLiteral("IsCommunicationObjectVisibilityCalculated"));
        LastModified = QDateTime::fromString(attrs.value(QStringLiteral("LastModified"))
            .toString(), Qt::ISODateWithMs);
        LastDownload = QDateTime::fromString(attrs.value(QStringLiteral("LastDownload"))
            .toString(), Qt::ISODateWithMs);

        // TODO: add pedantic check for all of the following
        Name = attrs.value(QStringLiteral("Name")).toString();
        Hardware2ProgramRefId = attrs.value(QStringLiteral("Hardware2ProgramRefId")).toString();
        Address = attrs.value(QStringLiteral("Address")).toInt();
        Comment = attrs.value(QStringLiteral("Comment")).toString();
        LastUsedAPDULength = attrs.value(QStringLiteral("LastUsedAPDULength")).toUShort();
        ReadMaxAPDULength = attrs.value(QStringLiteral("ReadMaxAPDULength")).toUShort();
        ReadMaxRoutingAPDULength = attrs.value(QStringLiteral("ReadMaxRoutingAPDULength"))
            .toUShort();
        InstallationHints = attrs.value(QStringLiteral("InstallationHints")).toString();

        attr = attrs.value(QStringLiteral("CompletionStatus")); // TODO: pedantic
        if (!attr.isNull())
            CompletionStatus = attr.toString();

        IndividualAddressLoaded = fetchBool(attrs, QStringLiteral("IndividualAddressLoaded"));
        ApplicationProgramLoaded = fetchBool(attrs, QStringLiteral("ApplicationProgramLoaded"));
        ParametersLoaded = fetchBool(attrs, QStringLiteral("ParametersLoaded"));
        CommunicationPartLoaded = fetchBool(attrs, QStringLiteral("CommunicationPartLoaded"));
        MediumConfigLoaded = fetchBool(attrs, QStringLiteral("MediumConfigLoaded"));
        LoadedImage = attrs.value(QStringLiteral("LoadedImage")).toUtf8();
        Description = attrs.value(QStringLiteral("Description")).toString();
        CheckSums = attrs.value(QStringLiteral("CheckSums")).toUtf8();
        Broken = fetchBool(attrs, QStringLiteral("Broken"));
        SerialNumber = attrs.value(QStringLiteral("SerialNumber")).toUtf8();
        UniqueId = attrs.value(QStringLiteral("UniqueId")).toString();
        IsRFRetransmitter = fetchBool(attrs, QStringLiteral("IsRFRetransmitter"));

        // children
       while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("ParameterInstanceRefs")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &ParameterInstanceRefs))
                        return false;
                } else if (reader->name() == QStringLiteral("ComObjectInstanceRefs")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &ComObjectInstanceRefs))
                        return false;
                } else if (reader->name() == QStringLiteral("ChannelInstances")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &ChannelInstances))
                        return false;
                } else if (reader->name() == QStringLiteral("AdditionalAddresses")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &AdditionalAddresses))
                        return false;
                } else if (reader->name() == QStringLiteral("BinaryData")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &BinaryData))
                        return false;
                } else if (reader->name() == QStringLiteral("IPConfig")) {
                    if (!IpConfig.parseElement(reader, pedantic))
                        return false;
                } else if (reader->name() == QStringLiteral("Security")) {
                    if (!Security.parseElement(reader, pedantic))
                        return false;
                } else if (reader->name() == QStringLiteral("BusInterfaces")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &BusInterfaces))
                        return false;
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("DeviceInstance"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <DeviceInstance>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}

QT_END_NAMESPACE
