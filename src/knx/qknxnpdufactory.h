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

#ifndef QKNXNPDUFACTORY_H
#define QKNXNPDUFACTORY_H

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxcemi.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnpdu.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNpduFactory final
{
public:
    QKnxNpduFactory() = delete;

    static QKnxCemi::TransportControlField transportControlFied(const QKnxNpdu &npdu);
    static QKnxCemi::ApplicationLayerControlField applicationLayerControlField(const QKnxNpdu &npdu);

    static QKnxNpdu createNpduGroupValueRead();
    static QKnxNpdu createNpduGroupValueWrite(const QByteArray &data);

    template <typename T = QKnxCemi::DevicePID>
    static QKnxNpdu createNpduGroupPropValueRead(QKnxCemi::ObjectType objectType,
        quint8 objectInstance, T propertyId)
    {
        return createNpduGroupPropValue(QKnxCemi::ApplicationLayerControlField::GroupPropValueRead,
            objectType, objectInstance, propertyId);
    }

    template <typename T = QKnxCemi::DevicePID>
    static QKnxNpdu createNpduGroupPropValueWrite(QKnxCemi::ObjectType objectType,
        quint8 objectInstance, T propertyId, QByteArray data)
    {
        return createNpduGroupPropValue(QKnxCemi::ApplicationLayerControlField::GroupPropValueWrite,
            objectType, objectInstance, propertyId, data);
    }

    static QKnxNpdu createNpduIndividualAddressRead();
    static QKnxNpdu createNpduIndividualAddressWrite(const QKnxAddress &newAddress);
    static QKnxNpdu createNpduIndividualAddressSerialNumberRead(const QByteArray &serialNumber);
    static QKnxNpdu createNpduIndividualAddressSerialNumberWrite(const QByteArray &serialNumber,
        const QKnxAddress &newAddress);
    static QKnxNpdu createNpduDomainAddressRead();
    static QKnxNpdu createNpduDomainAddressWrite(const QKnxAddress &domainAddress);
    // Only available for the 2 octet Domain Address (3/3/7 p35)
    //static QKnxNpdu createNpduDomainAddressSelectiveRead();
    static QKnxNpdu createNpduDomainAddressSerialNumberRead(const QByteArray &serialNumber);
    static QKnxNpdu createNpduDomainAddressSerialNumberWrite(const QByteArray &serialNumber,
        const QKnxAddress &domainAddress);

    template <typename T = QKnxCemi::DevicePID>
    static QKnxNpdu createNpduSystemNetworkParameterRead(QKnxCemi::ObjectType objectType,
        T propertyId, const QByteArray &testInfo)
    {
        return createNpduNetworkParameter(
            QKnxCemi::ApplicationLayerControlField::SystemNetworkParameterRead, objectType,
            propertyId, testInfo, true);
    }

    template <typename T = QKnxCemi::DevicePID>
    static QKnxNpdu createNpduSystemNetworkParameterWrite(QKnxCemi::ObjectType objectType,
        T propertyId, const QByteArray &value)
    {
        return createNpduNetworkParameter(
            QKnxCemi::ApplicationLayerControlField::SystemNetworkParameterWrite, objectType,
            propertyId, value, true);
    }

    template <typename T = QKnxCemi::DevicePID> static QKnxNpdu createNpduNetworkParameterRead(
        QKnxCemi::ObjectType objectType, T propertyId, const QByteArray &testInfo)
    {
        return createNpduNetworkParameter(
            QKnxCemi::ApplicationLayerControlField::NetworkParameterRead, objectType, propertyId,
            testInfo, false);
    }

    template <typename T = QKnxCemi::DevicePID> static QKnxNpdu createNpduNetworkParameterWrite(
        QKnxCemi::ObjectType objectType, T propertyId, const QByteArray &value)
    {
        return createNpduNetworkParameter(
            QKnxCemi::ApplicationLayerControlField::NetworkParameterWrite, objectType, propertyId,
            value, false);
    }

    template <typename T = QKnxCemi::DevicePID> static QKnxNpdu createNpduPropertyValueRead(
        quint8 objectIndex, T propertyId, quint8 nbElement, quint16 startIndex,
        quint8 sequenceNumber = 0)
    {
        return createNpduPropertyValueReadWrite(objectIndex, propertyId, nbElement, startIndex,
            sequenceNumber, QKnxCemi::ApplicationLayerControlField::PropertyValueRead);
    }

    template <typename T = QKnxCemi::DevicePID> static QKnxNpdu createNpduPropertyValueWrite(
        quint8 objectIndex, T propertyId, quint8 nbElement, quint16 startIndex,
        const QByteArray &data, quint8 sequenceNumber = 0)
    {
        return createNpduPropertyValueReadWrite(objectIndex, propertyId, nbElement, startIndex,
            sequenceNumber, QKnxCemi::ApplicationLayerControlField::PropertyValueWrite, data);
    }

    template <typename T = QKnxCemi::DevicePID> static QKnxNpdu createNpduPropertyDescriptionRead(
        quint8 objectIndex, T propertyId, quint8 propertyIndex, quint8 sequenceNumber = 0)
    {
        // Check if the propertyId belongs to an InterfaceOject
        // TODO: Not sure the Application Interface Object (as opposed to system Interface Objects)
        // have fixed names.
        static_assert(is_type<T, QKnxCemi::DevicePID, QKnxCemi::ApplicationProgramPID,
            QKnxCemi::ApplicationLayerControlField, QKnxCemi::GroupAddressTablePID,
            QKnxCemi::GroupObjectAssociationTablePID, QKnxCemi::GroupObjectTablePID,
            QKnxCemi::InterfaceProgramPID, QKnxCemi::KnxNetIpParameterPID>::value,
            "Type not supported.");

        auto tpci = buildTransportControlField(sequenceNumber);
        if (tpci == QKnxCemi::TransportControlField::Error)
            return {};

        auto npdu = createNpduOctets6And7(tpci,
            QKnxCemi::ApplicationLayerControlField::PropertyDescriptionRead);
        npdu.setByte(npdu.size(), objectIndex);
        npdu.setByte(npdu.size(), quint8(propertyId));
        npdu.setByte(npdu.size(), propertyIndex);
        npdu.setByte(0, quint8(npdu.size() - 2));
        return npdu;
    }

    template <typename T = QKnxCemi::DevicePID> static QKnxNpdu createNpduFunctionPropertyStateRead(
        quint8 objectIndex, T propertyId, const QByteArray &data, quint8 sequenceNumber = 0)
    {
        return createNpduFunctionProperty(
            QKnxCemi::ApplicationLayerControlField::FunctionPropertyStateRead,
            objectIndex, propertyId, data, sequenceNumber);
    }

    template <typename T = QKnxCemi::DevicePID> static QKnxNpdu createNpduFunctionPropertyCommand(
        quint8 objectIndex, T propertyId, const QByteArray &data, quint8 sequenceNumber = 0)
    {
        return createNpduFunctionProperty(
            QKnxCemi::ApplicationLayerControlField::FunctionPropertyCommand, objectIndex,
            propertyId, data, sequenceNumber);
    }

    static QKnxNpdu createNpduLinkRead(quint8 groupObjectNumber, quint8 startIndex,
        quint8 sequenceNumber = 0)
    {
        auto tpci = buildTransportControlField(sequenceNumber);
        if (tpci == QKnxCemi::TransportControlField::Error)
            return {};

        auto npdu = createNpduOctets6And7(tpci, QKnxCemi::ApplicationLayerControlField::LinkRead);
        npdu.setByte(npdu.size(), groupObjectNumber);
        npdu.setByte(npdu.size(), startIndex);
        npdu.setByte(0, quint8(npdu.size() - 2));
        return npdu;
    }

    static QKnxNpdu createNpduLinkWrite(quint8 groupObjectNumber, quint8 flags,
        const QKnxAddress &groupAddress, quint8 sequenceNumber = 0)
    {
        if (groupAddress.type() != QKnxAddress::Type::Group)
            return {};

        auto tpci = buildTransportControlField(sequenceNumber);
        if (tpci == QKnxCemi::TransportControlField::Error)
            return {};

        auto npdu = createNpduOctets6And7(tpci, QKnxCemi::ApplicationLayerControlField::LinkWrite);
        npdu.setByte(npdu.size(), groupObjectNumber);
        npdu.setByte(npdu.size(), flags);
        npdu.appendAndUpdate(groupAddress.bytes());
        return npdu;
    }

    static QKnxNpdu createNpduDeviceDescriptorRead(quint8 descriptorType, quint8 sequenceNumber = 0)
    {
        if (descriptorType >= 64)
            return {};

        auto tpci = buildTransportControlField(sequenceNumber);
        if (tpci == QKnxCemi::TransportControlField::Error)
            return {};

        // Device Descriptor can be of type 0 or 2 (cf Glossary)
        // but if not supported just receive an answer with error message
        return createNpduOctets6And7(tpci,
            QKnxCemi::ApplicationLayerControlField::DeviceDescriptorRead, descriptorType);
    }

    static QKnxNpdu createNpduUserManufacturerInfoRead(quint8 sequenceNumber);
    static QKnxNpdu createNpduMemoryRead(quint8 number, const QKnxAddress &address,
        quint8 sequenceNumber);
    static QKnxNpdu createNpduMemoryWrite(quint8 number, const QKnxAddress &address,
        const QByteArray &data, quint8 sequenceNumber);
    static QKnxNpdu createNpduUserMemoryRead(quint8 addressExtention, quint8 number,
        const QKnxAddress &address, quint8 sequenceNumber);
    static QKnxNpdu createNpduUserMemoryWrite(quint8 addressExtention, quint8 number,
        const QKnxAddress &address, const QByteArray &data, quint8 sequenceNumber);
    static QKnxNpdu createNpduAdcRead(quint8 sequenceNumber, quint8 channelNumber,
        quint8 readCount);

private:
    static QKnxCemi::TransportControlField buildTransportControlField(quint8 sequenceNumber);

    template <typename T> static QByteArray parameterType(QKnxCemi::ObjectType objectType, T property)
    {
        switch (objectType) {
        case QKnxCemi::ObjectType::Device:
            if (!std::is_same<T, QKnxCemi::DevicePID>::value)
                return {};
            break;
        case QKnxCemi::ObjectType::GroupAddressTable:
            if (!std::is_same<T, QKnxCemi::GroupAddressTablePID>::value)
                return {};
            break;
        case QKnxCemi::ObjectType::GroupAssociationTable:
            if (!std::is_same<T, QKnxCemi::GroupObjectAssociationTablePID>::value)
                return {};
            break;
        case QKnxCemi::ObjectType::ApplicationProgram:
            if (!std::is_same<T, QKnxCemi::ApplicationProgramPID>::value)
                return {};
            break;
        case QKnxCemi::ObjectType::InterfaceProgram:
            if (!std::is_same<T, QKnxCemi::InterfaceProgramPID>::value)
                return {};
            break;
        case QKnxCemi::ObjectType::GroupObjectTable:
            if (!std::is_same<T, QKnxCemi::GroupObjectTablePID>::value)
                return {};
            break;
        case QKnxCemi::ObjectType::KnxNetIpParameter:
            if (!std::is_same<T, QKnxCemi::KnxNetIpParameterPID>::value)
                return {};
            break;
        default:
            return {};
        }
        return QKnxUtils::QUint16::bytes<QByteArray>(quint16(objectType)).append(quint8(property));
    }

    template <typename T = QKnxCemi::DevicePID> static QKnxNpdu createNpduGroupPropValue(
        QKnxCemi::ApplicationLayerControlField apci, QKnxCemi::ObjectType objectType,
        quint8 objectInstance, T propertyId, const QByteArray &data = {})
    {
        QByteArray bytes = parameterType(objectType, propertyId);
        if (bytes.size() == 0)
            return {};

        // parameter type = Object Type + PID
        // For GroupPropValueRead we need Object Type + Object Instance + PID
        // parameter type is build (and checked first) then the Object Instance is inserted
        bytes.insert(2, objectInstance);
        // if objectInstance is 0, it means all instance (10/1 Logical Tag Extended p81)
        auto npdu = createNpduOctets6And7(QKnxCemi::TransportControlField::DataTagGroup, apci);
        npdu.appendAndUpdate(bytes);
        if (data.size() > 0)
            npdu.appendAndUpdate(data);
        return npdu;
    }

    template <typename T = QKnxCemi::DevicePID>
    static QKnxNpdu createNpduNetworkParameter(QKnxCemi::ApplicationLayerControlField apci,
        QKnxCemi::ObjectType objectType, T propertyId, const QByteArray &testInfoValue, bool system)
    {
        // testInfo: Value against which the resource indicated by parameterType is tested
        QByteArray bytes = parameterType(objectType, propertyId);
        if (bytes.size() == 0)
            return {};

        auto npdu = createNpduOctets6And7(system ? QKnxCemi::TransportControlField::DataBroadcast
            : QKnxCemi::TransportControlField::DataIndividual, apci);

        //--- part PID part test_info reserved: Setting the whole octet 11 to zero---
        if (system)
            bytes.append(QByteArray::fromHex("00"));
        bytes.append(testInfoValue);
        npdu.appendAndUpdate(bytes);
        return npdu;
    }

    // This is for Interface Object (reduced one: 3/4/1 p16)
    template <typename T = QKnxCemi::DevicePID> static QKnxNpdu createNpduPropertyValueReadWrite(
        quint8 objectIndex, T propertyId, quint8 nbElement, quint16 startIndex, quint8 sequenceNumber,
        QKnxCemi::ApplicationLayerControlField apci, const QByteArray &data = {})
    {
        if (apci == QKnxCemi::ApplicationLayerControlField::PropertyValueRead && data.size() != 0)
            return {};
        if (apci == QKnxCemi::ApplicationLayerControlField::PropertyValueWrite && data.size() == 0)
            return {};

        // ojectIndex is relative to the device. Different from the ObjectType!!!!
        // Before arriving here the propertyId should probably be checked against the objectType of
        // the object with index objectIndex.
        // Does not need to be checked at this level:
        // ApplicationLayer p52. if it does not fit the response is empty

        // Check if the propertyId belongs to an InterfaceOject
        // TODO: not sure the Application Interface Object (as opposed to system Interface Objects)
        // have fixed names.
        static_assert(is_type<T, QKnxCemi::DevicePID, QKnxCemi::ApplicationProgramPID,
            QKnxCemi::ApplicationLayerControlField, QKnxCemi::GroupAddressTablePID,
            QKnxCemi::GroupObjectAssociationTablePID, QKnxCemi::GroupObjectTablePID,
            QKnxCemi::InterfaceProgramPID, QKnxCemi::KnxNetIpParameterPID>::value,
            "Type not supported.");

        auto tpci = buildTransportControlField(sequenceNumber);
        if (tpci == QKnxCemi::TransportControlField::Error)
            return {};

        auto npdu = createNpduOctets6And7(tpci, apci);
        npdu.setByte(npdu.size(), objectIndex);
        npdu.setByte(npdu.size(), quint8(propertyId));

        auto bytes = QKnxUtils::QUint16::bytes(quint16(startIndex));
        bytes[0] = quint8(quint8(bytes[0]) | (nbElement << 4));

        npdu.appendAndUpdate(bytes);
        if (apci == QKnxCemi::ApplicationLayerControlField::PropertyValueWrite)
            npdu.appendAndUpdate(data);
        return npdu;
    }

    template <typename T = QKnxCemi::DevicePID>
    static QKnxNpdu createNpduFunctionProperty(QKnxCemi::ApplicationLayerControlField apci,
        quint8 objectIndex, T propertyId, const QByteArray &data, quint8 sequenceNumber)
    {
        // TODO: Figure out what is data
        auto tpci = buildTransportControlField(sequenceNumber);
        if (tpci == QKnxCemi::TransportControlField::Error)
            return {};

        auto npdu = createNpduOctets6And7(tpci, apci);
        npdu.setByte(npdu.size(), objectIndex);
        npdu.setByte(npdu.size(), quint8(propertyId));
        npdu.appendAndUpdate(data);
        return npdu;
    }

    static QKnxNpdu createNpduUserMemoryReadWrite(quint8 addressExtention, quint8 number,
        const QKnxAddress &address, quint8 sequenceNumber, const QByteArray &data = {});
    static QKnxNpdu createNpduOctets6And7(QKnxCemi::TransportControlField tpci,
        QKnxCemi::ApplicationLayerControlField apci, quint8 dataBits = 0);
};

QT_END_NAMESPACE

#endif // QKNXNPDUFACTORY_H
