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
#include <QtKnx/qknxinterfaceobject.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnpdu.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNpduFactory final
{
public:
    QKnxNpduFactory() = delete;

    static QKnxCemi::TransportLayerControlField transportLayerControlFied(const QKnxNpdu &npdu);
    static QKnxCemi::ApplicationLayerControlField applicationLayerControlField(const QKnxNpdu &npdu);

    static QKnxNpdu createGroupValueReadNpdu();
    static QKnxNpdu createGroupValueWriteNpdu(const QByteArray &data);

    static QKnxNpdu createGroupPropValueReadNpdu(QKnxInterfaceObject::Type objectType,
        quint8 objectInstance, QKnxInterfaceObject::Property property);
    static QKnxNpdu createGroupPropValueWriteNpdu(QKnxInterfaceObject::Type objectType,
        quint8 objectInstance, QKnxInterfaceObject::Property property, const QByteArray &data);

    static QKnxNpdu createIndividualAddressReadNpdu();
    static QKnxNpdu createIndividualAddressWriteNpdu(const QKnxAddress &newAddress);
    static QKnxNpdu createIndividualAddressSerialNumberReadNpdu(const QByteArray &serialNumber);
    static QKnxNpdu createIndividualAddressSerialNumberWriteNpdu(const QByteArray &serialNumber,
        const QKnxAddress &newAddress);

    static QKnxNpdu createDomainAddressReadNpdu();
    static QKnxNpdu createDomainAddressWriteNpdu(const QKnxAddress &domainAddress);
    // Only available for the 2 octet Domain Address (3/3/7 p35)
    //static QKnxNpdu createDomainAddressSelectiveReadNpdu();
    static QKnxNpdu createDomainAddressSerialNumberReadNpdu(const QByteArray &serialNumber);
    static QKnxNpdu createDomainAddressSerialNumberWriteNpdu(const QByteArray &serialNumber,
        const QKnxAddress &domainAddress);

    enum class Network : quint8
    {
        Local,
        System
    };
    static QKnxNpdu createNetworkParameterReadNpdu(Network type,
        QKnxInterfaceObject::Type objectType, QKnxInterfaceObject::Property property, const QByteArray &testInfo);

    static QKnxNpdu createNetworkParameterWriteNpdu(Network type,
        QKnxInterfaceObject::Type objectType, QKnxInterfaceObject::Property property, const QByteArray &value);

    static QKnxNpdu createPropertyValueReadNpdu(quint8 objectIndex, QKnxInterfaceObject::Property property,
        quint8 nbElement, quint16 startIndex, quint8 sequenceNumber = 0);

    static QKnxNpdu createPropertyValueWriteNpdu(quint8 objectIndex, QKnxInterfaceObject::Property property,
        quint8 nbElement, quint16 startIndex, const QByteArray &data, quint8 sequenceNumber = 0);

    static QKnxNpdu createPropertyDescriptionReadNpdu(quint8 objectIndex, QKnxInterfaceObject::Property property,
        quint8 propertyIndex, quint8 sequenceNumber = 0);

    static QKnxNpdu createFunctionPropertyStateReadNpdu(quint8 objectIndex, QKnxInterfaceObject::Property property,
        const QByteArray &data, quint8 sequenceNumber = 0);

    static QKnxNpdu createFunctionPropertyCommandNpdu(quint8 objectIndex, QKnxInterfaceObject::Property property,
        const QByteArray &data, quint8 sequenceNumber = 0);

    static QKnxNpdu createLinkReadNpdu(quint8 groupObjectNumber, quint8 startIndex,
        quint8 sequenceNumber = 0);

    static QKnxNpdu createLinkWriteNpdu(quint8 groupObjectNumber, quint8 flags,
        const QKnxAddress &groupAddress, quint8 sequenceNumber = 0);

    static QKnxNpdu createDeviceDescriptorReadNpdu(quint8 descriptorType, quint8 sequenceNumber = 0);

    static QKnxNpdu createUserManufacturerInfoReadNpdu(quint8 sequenceNumber);
    static QKnxNpdu createMemoryReadNpdu(quint8 number, const QKnxAddress &address,
        quint8 sequenceNumber);
    static QKnxNpdu createMemoryWriteNpdu(quint8 number, const QKnxAddress &address,
        const QByteArray &data, quint8 sequenceNumber);
    static QKnxNpdu createUserMemoryReadNpdu(quint8 addressExtention, quint8 number,
        const QKnxAddress &address, quint8 sequenceNumber);
    static QKnxNpdu createUserMemoryWriteNpdu(quint8 addressExtention, quint8 number,
        const QKnxAddress &address, const QByteArray &data, quint8 sequenceNumber);
    static QKnxNpdu createAdcReadNpdu(quint8 sequenceNumber, quint8 channelNumber,
        quint8 readCount);

private:
    static QByteArray parameterType(QKnxInterfaceObject::Type objectType, QKnxInterfaceObject::Property property);
    static QKnxCemi::TransportLayerControlField buildTransportControlField(quint8 sequence);

    static QKnxNpdu createGroupPropValueNpdu(QKnxCemi::ApplicationLayerControlField apci,
        QKnxInterfaceObject::Type objectType, quint8 objectInstance, QKnxInterfaceObject::Property property,
        const QByteArray &data = {});

    static QKnxNpdu createNetworkParameterNpdu(QKnxCemi::ApplicationLayerControlField apci,
        QKnxInterfaceObject::Type objectType, QKnxInterfaceObject::Property property, const QByteArray &testInfoValue,
        Network type);

    static QKnxNpdu createPropertyValueReadWriteNpdu(quint8 objectIndex, QKnxInterfaceObject::Property property,
        quint8 nbElement, quint16 startIndex, quint8 sequenceNumber,
        QKnxCemi::ApplicationLayerControlField apci, const QByteArray &data = {});

    static QKnxNpdu createFunctionPropertyNpdu(QKnxCemi::ApplicationLayerControlField apci,
        quint8 objectIndex, QKnxInterfaceObject::Property property, const QByteArray &data, quint8 sequenceNumber);

    static QKnxNpdu createNpduOctets6And7(QKnxCemi::TransportLayerControlField tpci,
        QKnxCemi::ApplicationLayerControlField apci, quint8 data = 0);

    static QKnxNpdu createUserMemoryReadWriteNpdu(quint8 addressExtention, quint8 number,
        const QKnxAddress &address, quint8 sequenceNumber, const QByteArray &data = {});
};

QT_END_NAMESPACE

#endif // QKNXNPDUFACTORY_H
