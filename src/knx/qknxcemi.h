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

struct Q_KNX_EXPORT QKnxCemi final
{
    struct Server final
    {
        enum class ReturnCode : quint8
        {
            NoError = 0x00,
            Error = 0x01
        };

        enum class Error : quint8
        {
            Unspecified = 0x00,
            OutOfRange = 0x01,
            OutOfMaxRange = 0x02,
            OutOfMinRange = 0x03,
            Memory = 0x04,
            ReadOnly = 0x05,
            IllegalCommand = 0x06,
            NonExistingProperty = 0x07,
            TypeConflict = 0x08,
            PropertyIndexRangeError = 0x09,
            TemporaryNotWritable = 0x0a,
            None = 0xff  // Qt extension
        };
        // See 03_06_03 EMI_IMI v01.03.03 AS.pdf Paragraph 4.1.7.3.7.2 (Table 12)
    };

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
Q_DECLARE_TYPEINFO(QKnxCemi::TransportLayerControlField, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxCemi::ApplicationLayerControlField, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
