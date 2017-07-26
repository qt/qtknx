/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXCEMI_H
#define QKNXCEMI_H

#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

struct QKnxInterfaceObject
{
    struct Property final
    {
        enum Independent : quint8
        {
            ObjectType = 0x01,
            ObjectName = 0x02,
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
            Description = 0x15,
            Table = 0x17,
            Version = 0x19,
            McbTable = 0x1b,
            ErrorCode = 0x1c,
            ObjectIndex = 0x1d,
            DownloadCounter = 0x1e
        };
        static bool isIndependentProperty(QKnxInterfaceObject::Property property);

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
            PsuEnables = 0x45,
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
            RfDomainAddress = 0x52,
            DeviceDescriptor = 0x53,
            GroupTelegramRateLimitTimeBase = 0x55,
            GroupTelegramRateLimitNumberOfTelegrams = 0x56,
            Channel01Paramter = 0x65,
            Channel02Paramter = 0x66,
            Channel03Paramter = 0x67,
            Channel04Paramter = 0x68,
            Channel05Paramter = 0x69,
            Channel06Paramter = 0x6A,
            Channel07Paramter = 0x6B,
            Channel08Paramter = 0x6C,
            Channel09Paramter = 0x6D,
            Channel10Paramter = 0x6E,
            Channel11Paramter = 0x6F,
            Channel12Paramter = 0x70,
            Channel13Paramter = 0x71,
            Channel14Paramter = 0x72,
            Channel15Paramter = 0x73,
            Channel16Paramter = 0x74,
            Channel17Paramter = 0x75,
            Channel18Paramter = 0x76,
            Channel19Paramter = 0x77,
            Channel20Paramter = 0x78,
            Channel21Paramter = 0x79,
            Channel22Paramter = 0x7A,
            Channel23Paramter = 0x7B,
            Channel24Paramter = 0x7C,
            Channel25Paramter = 0x7D,
            Channel26Paramter = 0x7E,
            Channel27Paramter = 0x7F,
            Channel28Paramter = 0x80,
            Channel29Paramter = 0x81,
            Channel30Paramter = 0x82,
            Channel31Paramter = 0x83,
            Channel32Paramter = 0x84
        };
        static bool isDeviceProperty(QKnxInterfaceObject::Property property);

        enum GroupAddressTable : quint8
        {
            ExtendedFrameFormat = 0x33,
            AddressTable1 = 0x34,
            GroupResponserTable = 0x35
        };
        static bool isGroupAddressTableProperty(QKnxInterfaceObject::Property property);

        using AssociationTable = Independent;
        static bool isAssociationTableProperty(QKnxInterfaceObject::Property property);

        enum ApplicationProgram : quint8
        {
            ParameterReference = 0x33
        };
        static bool isApplicationProgramProperty(QKnxInterfaceObject::Property property);

        using InterfaceProgram = Independent;
        static bool isInterfaceProgramProperty(QKnxInterfaceObject::Property property);

        using KnxObjectAssociationTable = Independent;
        static bool isKnxObjectAssociationTableProperty(QKnxInterfaceObject::Property property);

        enum Router : quint8
        {
            LineStatus = 0x33,
            MainLcConfig = 0x34,
            SubLcConfig = 0x35,
            MainLcGroupConfig = 0x36,
            SubLcGroupConfig = 0x37,
            RouteTableControl = 0x38,
            CouplerServerControl = 0x39,
            MaxApduLengthRouter = 0x3a,
            L2CouplerType = 0x3b
        };
        static bool isRouterProperty(QKnxInterfaceObject::Property property);

        enum LteAddressRoutingTable : quint8
        {
            LteRouteSelect = 0x33,
            LteRouteTable = 0x34
        };
        static bool isLteAddressRoutingTableProperty(QKnxInterfaceObject::Property property);

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
            RfFilteringModeSelect = 0x3e,
            RfFilteringModeSupport = 0x3f
        };
        static bool isCemiServerProperty(QKnxInterfaceObject::Property property);

        enum GroupObjectTable : quint8
        {
            GroupObjectTableProperty = 0x33,
            ExtendedGroupObjectReference = 0x34
        };
        static bool isGroupObjectTableProperty(QKnxInterfaceObject::Property property);

        enum PollingMaster : quint8
        {
            PollingState = 0x33,
            PollingSlaveAddress = 0x34,
            PollingCycle = 0x35
        };
        static bool isPollingMasterProperty(QKnxInterfaceObject::Property property);

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
            RoutingBusyWaitTime = 0x4e
        };
        static bool isKnxNetIpParameterProperty(QKnxInterfaceObject::Property property);

        using FileServer = Independent;
        static bool isFileServerProperty(QKnxInterfaceObject::Property property);

        enum RfMedium : quint8
        {
            RfMultiType = 0x33,
            RfMultiPhysicalFeatures = 0x34,
            RfMultiCallChannel = 0x35,
            RfMultiObjectLink = 0x36,
            RfMultiExtendedGroupAddressRepeated = 0x37,
            TransmissionMode = 0x46,
            ReceptionMode = 0x47,
            TestSignal = 0x48,
            FastAcknowledge = 0x49,
            FastAcknowledgeActivate = 0x4a
        };
        static bool isRfMediumProperty(QKnxInterfaceObject::Property property);

        Property(quint8 property)
            : m_property(property)
        {}
        operator int() const { return m_property; }

        private:
            quint8 m_property;
    };

    struct Type final
    {
        static const constexpr int Invalid = 0x10000;

        enum System : quint16
        {
            Device = 0x0000,
            GroupAddressTableType = 0x0001,
            AssociationTableType = 0x0002,
            ApplicationProgramType = 0x0003,
            InterfaceProgramType = 0x0004,
            KnxObjectAssociationTableType = 0x0005,
            RouterType = 0x0006,
            LteAddressRoutingTableType = 0x0007,
            CemiServerType = 0x0008,
            GroupObjectTableType = 0x0009,
            PollingMasterType = 0x000a,
            KnxNetIpParameterType = 0x000b,
            ReservedType = 0x000c,
            FileServerType = 0x000d,
            RfMediumType = 0x0013
        };

        enum Application : quint16
        {
            Hvac = 0x64,
            Lightning = 0x190,
            SensorsAndActuators = 0x258,
            ShuttersAndBlinds = 0x320,
            FociS = 0x3e8,
            Metering = 0x44d,
            OpenTherm = 0x4b0,
            ApplicationReserved = 0x4e2,
            WhiteGoods = 0xdac,
            ApplicationReserved2 = 0xed8,
            WhiteGoods2 = 0xfa0
        };

        enum NonStandardized : quint16
        {
            First = 0xc351,
            Last = 0xffff
        };

        Type(quint16 objectType)
            : m_objectType(objectType)
        {}
        operator int() const { return m_objectType; }

        static bool isObjectType(QKnxInterfaceObject::Type type);
        static bool isMatch(QKnxInterfaceObject::Type type, QKnxInterfaceObject::Property property);

    private:
        int m_objectType = Invalid;
    };
};

struct Q_KNX_EXPORT QKnxCemi final
{
    // Object Type or Interface Object
    // For each Object type, a certain range of property is accessible

    // Object Type can refer to Interface Object or Group Object
    // they are quint16

    // Properties are linked to the Objects
    // called PID (usually when linked to an Interface Object)
    // called DPT (when representing a DataPoint, accessed directly via a Group Object)
    // some PID can be a DPT (which mean an Interface object can be referenced by a group object)
    // Properties are quint8

    // TPCI in NPDU cf(3/3/4 p6) (only the ones with rooms for APCI in 2 first bits)
    enum class TransportLayerControlField : quint8
    {
        DataGroup = 0x00, // multicast
        DataBroadcast = 0x00, // broadcast
        //? T_DataSystemBroadcast ;
        DataIndividual = 0x00, // point to point connection less
        DataTagGroup = 0x04, // don't know to which communication mode it applies

        // For the connection oriented one (SedNo to be added)
        DataConnected = 0x40,
        Error = 0xff // not from documentation
    };

    // ApplicationLayer(3/3/7) Table 1
    // (Action within cEMI Frame of type L_Data, for example)
    enum class ApplicationLayerControlField : quint16
    {
        Error = 0xffff, // not from documentation

        // Multicast
        GroupValueRead = 0x0000,
        GroupValueResponse = 0x0040,
        GroupValueWrite = 0x0080,

        // Broadcast
        IndividualAddressWrite = 0x00c0,
        IndividualAddressRead = 0x0100,
        IndividualAddressresponse = 0x0140,

        // System Broadcast
        SystemNetworkParameterRead = 0x01c8,
        SystemNetworkParameterResponse = 0x01c9,
        SystemNetworkParameterWrite = 0x01ca,

        // Point-to-point connection-oriented
        AdcRead = 0x0180,
        AdcResponse = 0x01c0,

        MemoryRead = 0x0200,
        MemoryResponse = 0x0240,
        MemoryWrite = 0x0280,

        UserMemoryRead = 0x02c0,
        UserMemoryResponse = 0x02c1,
        UserMemoryWrite = 0x02c2,

        UserMemoryBitWrite = 0x02c4,

        UserManufacturerInfoRead = 0x02c5,
        UserManufacturerInfoResponse = 0x02c5,

        FunctionPropertyCommand = 0x02c7,       // point-to-point connectionless
        FunctionPropertyStateRead = 0x02c8,     // point-to-point connectionless
        FunctionPropertyStateResponse = 0x02c9, // point-to-point connectionless

        DeviceDescriptorRead = 0x0300,
        DeviceDescriptorResponse = 0x0340,
        Restart = 0x0380,

        // Coupler specific services
        OpenRoutingTableRequest = 0x03c0,
        ReadRoutingTableRequest = 0x03c1,
        ReadRoutingTableResponse = 0x03c2,
        WriteRoutingTableRequest = 0x03c3,
        ReadRouterMemoryRequest = 0x03c8,
        ReadRouterMemoryResponse = 0x03c9,
        WriteRouterMemoryRequest = 0x03ca,
        ReadRouterStatusRequest = 0x03cd,
        ReadRouterStatusResponse = 0x03ce,

        WriteRouterStatusRequest = 0x03cf,

        MemoryBitWrite = 0x03d0,

        AuthorizeRequest = 0x03d1,
        AuthorizeResponse = 0x03d2,
        KeyWrite = 0x03d3,
        KeyResponse = 0x03d4,

        PropertyValueRead = 0x03d5,             // point-to-point connectionless
        PropertyValueResponse = 0x03d6,         // point-to-point connectionless
        PropertyValueWrite = 0x03d7,            // point-to-point connectionless
        PropertyDescriptionRead = 0x03d8,       // point-to-point connectionless
        PropertyDescriptionResponse = 0x03d9,   // point-to-point connectionless

        // Broadcast
        NetworkParameterRead = 0x03da,
        NetworkParameterResponse = 0x03db,      // point-to-point connectionless

        IndividualAddressSerialNumberRead = 0x03dc,
        IndividualAddressSerialNumberResponse = 0x03dd,
        IndividualAddressSerialNumberWrite = 0x03de,

        // Open media specific services
        DomainAddressWrite = 0x03e0,
        DomainAddressRead = 0x03e1,
        DomainAddressResponse = 0x03e2,
        DomainAddressSelectiveRead = 0x03e3,

        NetworkParameterWrite = 0x03e4,
        LinkRead = 0x03e5,
        LinkResponse = 0x03e6,
        LinkWrite = 0x03e7,
        GroupPropValueRead = 0x03e8,
        GroupPropValueResponse = 0x03e9,
        GroupPropValueWrite = 0x03ea,
        GroupPropValueInfoReport = 0x03eb,
        DomainAddressSerialNumberRead = 0x03ec,
        DomainAddressSerialNumberResponse = 0x03ed,
        DomainAddressSerialNumberWrite = 0x03ee,
        FileStreamInfoReport = 0x03ef
    };
};
Q_DECLARE_TYPEINFO(QKnxInterfaceObject, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::Independent, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::Device, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::GroupAddressTable, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::ApplicationProgram, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::Router, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::LteAddressRoutingTable, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::CemiServer, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::GroupObjectTable, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::PollingMaster, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::KnxNetIpParameter, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Property::RfMedium, Q_PRIMITIVE_TYPE);

Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Type::System, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Type::Application, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObject::Type::NonStandardized, Q_PRIMITIVE_TYPE);

Q_DECLARE_TYPEINFO(QKnxCemi::TransportLayerControlField, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxCemi::ApplicationLayerControlField, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
