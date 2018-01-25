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

#ifndef QKNXDEVICEINSTANCE_H
#define QKNXDEVICEINSTANCE_H

#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxprojectutils.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxParameterInstanceRef
{
    Q_DECLARE_TR_FUNCTIONS(QKnxParameterInstanceRef)

public:
    QString Id; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString RefId; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Value; // optional

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxConnectors
{
    Q_DECLARE_TR_FUNCTIONS(QKnxConnectors)

public:
    struct Q_KNX_EXPORT QKnxConnector
    {
        QString GroupAddressRefId; // non-colonized name, pattern [\i-[:]][\c-[:]]*
        bool Acknowledge { false }; // optional
    }
    Send;
    QVector<QKnxConnector> Receive; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxComObjectInstanceRef
{
    Q_DECLARE_TR_FUNCTIONS(QKnxComObjectInstanceRef)

public:
    QString Id; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString RefId; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Text; // optional, 255 character max.
    QString FunctionText; // optional, 255 character max.
    QString Priority; // optional, Low, High, Alert
    QString ReadFlag; // optional, Enabled, Disabled
    QString WriteFlag; // optional, Enabled, Disabled
    QString CommunicationFlag; // optional, Enabled, Disabled
    QString TransmitFlag; // optional, Enabled, Disabled
    QString UpdateFlag; // optional, Enabled, Disabled
    QString ReadOnInitFlag; // optional, Enabled, Disabled
    QVector<QString> DatapointType; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Description; // optional
    bool IsActive; // optional
    QString ChannelId; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*

    QVector<QKnxConnectors> Connectors; // 0..1

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxChannelInstance
{
    Q_DECLARE_TR_FUNCTIONS(QKnxChannelInstance)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString RefId; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // optional, 255 character max.
    QString Description; // optional, 255 character max.
    bool IsActive; // optional

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxAdditionalAddress
{
    Q_DECLARE_TR_FUNCTIONS(QKnxAdditionalAddress)

public:
    quint8 Address; // min. value 1, 255 value max.
    QString Name; // optional, 255 character max.
    QString Description; // optional
    QString Comment; // optional

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxBinaryData
{
    Q_DECLARE_TR_FUNCTIONS(QKnxBinaryData)

public:
    QString Id; // optional
    QString RefId; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // optional, 50 character max.
    QByteArray Data; // 0..1, Base64 encoded

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxIpConfig
{
    Q_DECLARE_TR_FUNCTIONS(QKnxIpConfig)

public:
    QString Assign { QLatin1String("Auto") }; // optional, Fixed, Auto
    QString IPAddress; // optional, pattern ((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])
    QString SubnetMask; // optional, pattern ((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])
    QString DefaultGateway; // optional, pattern ((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])
    QString MacAddress; // optional, 50 character max.

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxSecurity
{
    Q_DECLARE_TR_FUNCTIONS(QKnxSecurity)

public:
    QString LoadedIPRoutingBackboneKey; // optional, 40 character max.
    QString DeviceAuthenticationCode; // optional, 20 character max.
    QString LoadedDeviceAuthenticationCode; // optional, 20 character max.
    QString DeviceManagementPassword; // optional, 20 character max.
    QString LoadedDeviceManagementPassword; // optional, 20 character max.
    QString ToolKey; // optional, 40 character max.
    QString LoadedToolKey; // optional, 40 character max.
    quint32 SequenceNumber; // optional
    QDateTime SequenceNumberTimestamp; // optional

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxBusInterface
{
    Q_DECLARE_TR_FUNCTIONS(QKnxBusInterface)

public:
    QString Name; // optional, 255 character max.
    QString Description; // optional
    QString Comment; // optional
    QString Password; // optional, 20 character max.

    QVector<QString> Connectors; // 0..n, non-colonized name, pattern [\i-[:]][\c-[:]]*

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxDeviceInstance
{
    Q_DECLARE_TR_FUNCTIONS(QKnxDeviceInstance)

public:
    QString Name; // optional, 255 character max.
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString ProductRefId; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Hardware2ProgramRefId; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    qint32 Address; // optional, min. value 0, 255 value max.
    QString Comment; // optional
    QDateTime LastModified; // optional
    QDateTime LastDownload; // optional
    quint16 LastUsedAPDULength; // optional
    quint16 ReadMaxAPDULength; // optional
    quint16 ReadMaxRoutingAPDULength; // optional
    QString InstallationHints; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    bool IndividualAddressLoaded { false }; // optional
    bool ApplicationProgramLoaded { false }; // optional
    bool ParametersLoaded { false }; // optional
    bool CommunicationPartLoaded { false }; // optional
    bool MediumConfigLoaded { false }; // optional
    QByteArray LoadedImage; // optional, Base64 encoded
    QString Description; // optional
    QByteArray CheckSums; // optional, Base64 encoded
    bool IsCommunicationObjectVisibilityCalculated; // optional
    bool Broken { false }; // optional
    QByteArray SerialNumber; // optional, Base64 encoded
    QString UniqueId; // optional, pattern \{[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}\}
    bool IsRFRetransmitter { false }; // optional
    qint32 Puid;

    QVector<QKnxParameterInstanceRef> ParameterInstanceRefs; // 0..n
    QVector<QKnxComObjectInstanceRef> ComObjectInstanceRefs; // 0..n
    QVector<QKnxChannelInstance> ChannelInstances; // 0..n
    QVector<QKnxAdditionalAddress> AdditionalAddresses; // 1..254
    QVector<QKnxBinaryData> BinaryData; // 1..n
    QKnxIpConfig IpConfig;
    QKnxSecurity Security;
    QVector<QKnxBusInterface> BusInterfaces; // 1..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

QT_END_NAMESPACE

#endif
