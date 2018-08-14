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

#ifndef QKNXINTERFACEOBJECTPROPERTY_H
#define QKNXINTERFACEOBJECTPROPERTY_H

#include <QtCore/qobject.h>
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxInterfaceObjectProperty final
{
    Q_GADGET

public:
    static const constexpr quint16 Invalid = 0x100;
    static bool isProperty(QKnxInterfaceObjectProperty property);

    enum General : quint8
    {
        ObjectType = 0x01,
        ObjectName = 0x02,
        Semaphor = 0x03,
        GroupObjectReference = 0x04,
        LoadStateControl = 0x05,
        RunStateControl = 0x06,
        TableReference = 0x07,
        ServiceControl = 0x08,
        FirmwareRevision = 0x09,
        ServicesSupported = 0x0a,
        SerialNumber = 0x0b,
        ManufacturerId = 0x0c,
        ProgramVersion = 0x0d,
        DeviceControl = 0x0e,
        OrderInfo = 0x0f,
        PeiType = 0x10,
        PortConfiguration = 0x11,
        PollGroupSettings = 0x12,
        ManufacturerData = 0x13,
        Enable = 0x14,
        Description = 0x15,
        File = 0x16,
        Table = 0x17,
        Enrol = 0x18,
        Version = 0x19,
        GroupObjectLink = 0x1a,
        McbTable = 0x1b,
        ErrorCode = 0x1c,
        ObjectIndex = 0x1d,
        DownloadCounter = 0x1e
    };
    Q_ENUM(General)
    static bool isGeneralProperty(QKnxInterfaceObjectProperty property);

    enum Device : quint8
    {
        RoutingCount = 0x33,
        MaxRetryCount = 0x34,
        ErrorFlags = 0x35,
        ProgMode = 0x36,
        ProductId = 0x37,
        MaxApduLengthDevice = 0x38,
        SubnetAddress = 0x39,
        DeviceAddress = 0x3a,
        PbConfig = 0x3b,
        AddressReport = 0x3c,
        AddressCheck = 0x3d,
        ObjectValue = 0x3e,
        ObjectLink = 0x3f,
        Application = 0x40,
        Parameter = 0x41,
        ObjectAddress = 0x42,
        PsuType = 0x43,
        PsuStatus = 0x44,
        PsuEnable = 0x45,
        DomainAddress = 0x46,
        IoList = 0x47,
        MgtDescriptor01 = 0x48,
        PL110Parameter = 0x49,
        RfRepeatCounter = 0x4a,
        ReceiveBlockTable = 0x4b,
        RandomPauseTable = 0x4c,
        ReceiveBlockNumber = 0x4d,
        HardwareType = 0x4e,
        RetransmitterNumber = 0x4f,
        SerialNumberTable = 0x50,
        BibatMasterAddress = 0x51,
        RfDomainAddressDevice = 0x52,
        DeviceDescriptor = 0x53,
        MeteringFilterTable = 0x54,
        GroupTelegramRateLimitTimeBase = 0x55,
        GroupTelegramRateLimitNumberOfTelegrams = 0x56,
        Channel01Parameter = 0x65,
        Channel02Parameter = 0x66,
        Channel03Parameter = 0x67,
        Channel04Parameter = 0x68,
        Channel05Parameter = 0x69,
        Channel06Parameter = 0x6A,
        Channel07Parameter = 0x6B,
        Channel08Parameter = 0x6C,
        Channel09Parameter = 0x6D,
        Channel10Parameter = 0x6E,
        Channel11Parameter = 0x6F,
        Channel12Parameter = 0x70,
        Channel13Parameter = 0x71,
        Channel14Parameter = 0x72,
        Channel15Parameter = 0x73,
        Channel16Parameter = 0x74,
        Channel17Parameter = 0x75,
        Channel18Parameter = 0x76,
        Channel19Parameter = 0x77,
        Channel20Parameter = 0x78,
        Channel21Parameter = 0x79,
        Channel22Parameter = 0x7A,
        Channel23Parameter = 0x7B,
        Channel24Parameter = 0x7C,
        Channel25Parameter = 0x7D,
        Channel26Parameter = 0x7E,
        Channel27Parameter = 0x7F,
        Channel28Parameter = 0x80,
        Channel29Parameter = 0x81,
        Channel30Parameter = 0x82,
        Channel31Parameter = 0x83,
        Channel32Parameter = 0x84
    };
    Q_ENUM(Device)
    static bool isDeviceProperty(QKnxInterfaceObjectProperty property);

    enum GroupAddressTable : quint8
    {
        ExtendedFrameFormat = 0x33,
        AddressTable1 = 0x34,
        GroupResponserTable = 0x35
    };
    Q_ENUM(GroupAddressTable)
    static bool isGroupAddressTableProperty(QKnxInterfaceObjectProperty property);

    using AssociationTable = General;
    //Q_ENUM(AssociationTable)
    static bool isAssociationTableProperty(QKnxInterfaceObjectProperty property);

    enum ApplicationProgram : quint8
    {
        ParameterReference = 0x33
    };
    Q_ENUM(ApplicationProgram)
    static bool isApplicationProgramProperty(QKnxInterfaceObjectProperty property);

    using InterfaceProgram = General;
    //Q_ENUM(InterfaceProgram)
    static bool isInterfaceProgramProperty(QKnxInterfaceObjectProperty property);

    using KnxObjectAssociationTable = General;
    //Q_ENUM(KnxObjectAssociationTable)
    static bool isKnxObjectAssociationTableProperty(QKnxInterfaceObjectProperty property);

    enum Router : quint8
    {
        LineStatus = 0x33, // ### Qt6: remove
        MediumStatus = 0x33,
        MainLcConfig = 0x34,
        SubLcConfig = 0x35,
        MainLcGroupConfig = 0x36,
        SubLcGroupConfig = 0x37,
        RouteTableControl = 0x38,
        CouplerServerControl = 0x39,
        MaxRouterApduLength = 0x3a,
        L2CouplerType = 0x3b,
        Medium = 0x3f,
        FilterTableUse = 0x43,
        RfEnableSbc = 0x70
    };
    Q_ENUM(Router)
    static bool isRouterProperty(QKnxInterfaceObjectProperty property);

    enum LteAddressRoutingTable : quint8
    {
        LteRouteSelect = 0x33,
        LteRouteTable = 0x34
    };
    Q_ENUM(LteAddressRoutingTable)
    static bool isLteAddressRoutingTableProperty(QKnxInterfaceObjectProperty property);

    enum CemiServer : quint8
    {
        MediumType = 0x33,
        CommonMode = 0x34,
        MediumAvailability = 0x35,
        AdditionalInfoTypes = 0x36,
        TimeBase = 0x37,
        TransparencyModeEnabled = 0x38,
        BiBatNextBlock = 0x3b,
        RfModeSelect = 0x3c,
        RfModeSupport = 0x3d,
        RfFilteringModeSelectCemiServer = 0x3e,
        RfFilteringModeSupport = 0x3f,
        MaxInterfaceApduLength = 0x44,
        MaxLocalApduLength = 0x45
    };
    Q_ENUM(CemiServer)
    static bool isCemiServerProperty(QKnxInterfaceObjectProperty property);

    enum GroupObjectTable : quint8
    {
        GroupObjectTableProperty = 0x33,
        ExtendedGroupObjectReference = 0x34
    };
    Q_ENUM(GroupObjectTable)
    static bool isGroupObjectTableProperty(QKnxInterfaceObjectProperty property);

    enum PollingMaster : quint8
    {
        PollingState = 0x33,
        PollingSlaveAddress = 0x34,
        PollingCycle = 0x35
    };
    Q_ENUM(PollingMaster)
    static bool isPollingMasterProperty(QKnxInterfaceObjectProperty property);

    enum KnxNetIpParameter : quint8
    {
        ProjectInstallationId = 0x33,
        KnxIndividualAddress = 0x34,
        AdditionalIndividualAddresses = 0x35,
        CurrentIpAssignmentMethod = 0x36,
        IpAssignmentMethod = 0x37,
        IpCapabilities = 0x38,
        CurrentIpAddress = 0x39,
        CurrentSubnetMask = 0x3a,
        CurrentDefaultGateway = 0x3b,
        IpAddress = 0x3c,
        SubnetMask = 0x3d,
        DefaultGateway = 0x3e,
        DhcpBootpServer = 0x3f,
        MacAddress = 0x40,
        SystemSetupMulticastAddress = 0x41,
        RoutingMulticastAddress = 0x42,
        Ttl = 0x43,
        KnxNetIpDeviceCapabilities = 0x44,
        KnxNetIpDeviceState = 0x45,
        KnxNetIpRoutingCapabilities = 0x46,
        PriorityFifoEnabled = 0x47,
        QueueOverflowToIp = 0x48,
        QueueOverflowToKnx = 0x49,
        MsgTransmitToIp = 0x4a,
        MsgTransmitToKnx = 0x4b,
        FriendlyName = 0x4c,
        RoutingBusyWaitTime = 0x4e,
        TunnelingAddresses =0x4f,
        BackboneKey = 0x5b,
        DeviceAuthenticationCode = 0x5c,
        PasswordHashes = 0x5d,
        SecuredServiceFamilies = 0x5e,
        MulticastLatencyTolerance = 0x5f,
        SyncLatencyFraction = 0x60,
        TunnellingUsers = 0x61
    };
    Q_ENUM(KnxNetIpParameter)
    static bool isKnxNetIpParameterProperty(QKnxInterfaceObjectProperty property);

    using FileServer = General;
    //Q_ENUM(FileServer)
    static bool isFileServerProperty(QKnxInterfaceObjectProperty property);

    enum Security : quint8
    {
        SecurityMode = 0x33,
        P2pKeyTable = 0x34,
        GroupKeyTable = 0x35,
        SecurityIndividualAddressTable = 0x36,
        SecurityFailuresLog = 0x37,
        SkiTool = 0x38,
        SecurityReport = 0x39,
        SecurityReportControl = 0x3a,
        SequenceNumberSending = 0x3b,
        ZoneKeysTable = 0x3c,
        GoSecurityFlags = 0x3d
    };
    Q_ENUM(Security)
    static bool isSecurityProperty(QKnxInterfaceObjectProperty property);

    enum RfMedium : quint8
    {
        RfMultiType = 0x33,
        RfMultiPhysicalFeatures = 0x34,
        RfMultiCallChannel = 0x35,
        RfMultiObjectLink = 0x36,
        RfMultiExtendedGroupAddressRepeated = 0x37,
        RfDomainAddressRfMedium = 0x38,
        RfRetransmitter = 0x39,
        RfSecurityReportControl = 0x3a,
        RfFilteringModeSelectRfMedium = 0x3b,
        RfBidirTimeout = 0x3c,
        RfDiagSaFilterTable = 0x3d,
        RfDiagQualityTable = 0x3e,
        RfDiagProbe = 0x3f,
        TransmissionMode = 0x46,
        ReceptionMode = 0x47,
        TestSignal = 0x48,
        FastAcknowledge = 0x49,
        FastAcknowledgeActivate = 0x4a
    };
    Q_ENUM(RfMedium)
    static bool isRfMediumProperty(QKnxInterfaceObjectProperty property);

    QKnxInterfaceObjectProperty(qint16 property)
        : m_property(property)
    {}
    operator qint16() const { return m_property; }

    private:
        qint16 m_property;
};

Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::General, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::Device, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::GroupAddressTable, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::ApplicationProgram, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::Router, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::LteAddressRoutingTable, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::CemiServer, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::GroupObjectTable, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::PollingMaster, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::KnxNetIpParameter, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectProperty::RfMedium, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
