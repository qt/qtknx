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

#ifndef QKNXNPDUFACTORY_H
#define QKNXNPDUFACTORY_H

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxcemi.h>
#include <QtKnx/qknxinterfaceobjectproperty.h>
#include <QtKnx/qknxinterfaceobjecttype.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnpdu.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNpduFactory final
{
public:
    QKnxNpduFactory() = delete;

    static QKnxNpdu createGroupValueReadNpdu();
    static QKnxNpdu createGroupValueWriteNpdu(const QByteArray &data);

    static QKnxNpdu createGroupPropValueReadNpdu(QKnxInterfaceObjectType objectType,
        quint8 objectInstance, QKnxInterfaceObjectProperty property);
    static QKnxNpdu createGroupPropValueWriteNpdu(QKnxInterfaceObjectType objectType,
        quint8 objectInstance, QKnxInterfaceObjectProperty property, const QByteArray &data);

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
        QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property, const QByteArray &testInfo);

    static QKnxNpdu createNetworkParameterWriteNpdu(Network type,
        QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property, const QByteArray &value);

    static QKnxNpdu createPropertyValueReadNpdu(quint8 objectIndex, QKnxInterfaceObjectProperty property,
        quint8 nbElement, quint16 startIndex, quint8 sequenceNumber = 0);

    static QKnxNpdu createPropertyValueWriteNpdu(quint8 objectIndex, QKnxInterfaceObjectProperty property,
        quint8 nbElement, quint16 startIndex, const QByteArray &data, quint8 sequenceNumber = 0);

    static QKnxNpdu createPropertyDescriptionReadNpdu(quint8 objectIndex, QKnxInterfaceObjectProperty property,
        quint8 propertyIndex, quint8 sequenceNumber = 0);

    static QKnxNpdu createFunctionPropertyStateReadNpdu(quint8 objectIndex, QKnxInterfaceObjectProperty property,
        const QByteArray &data, quint8 sequenceNumber = 0);

    static QKnxNpdu createFunctionPropertyCommandNpdu(quint8 objectIndex, QKnxInterfaceObjectProperty property,
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
    static QByteArray parameterType(QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property);
    static QKnxNpdu::TransportLayerControlField buildTransportControlField(quint8 sequence);

    static QKnxNpdu createGroupPropValueNpdu(QKnxNpdu::ApplicationLayerControlField apci,
        QKnxInterfaceObjectType objectType, quint8 objectInstance, QKnxInterfaceObjectProperty property,
        const QByteArray &data = {});

    static QKnxNpdu createNetworkParameterNpdu(QKnxNpdu::ApplicationLayerControlField apci,
        QKnxInterfaceObjectType objectType, QKnxInterfaceObjectProperty property, const QByteArray &testInfoValue,
        Network type);

    static QKnxNpdu createPropertyValueReadWriteNpdu(quint8 objectIndex, QKnxInterfaceObjectProperty property,
        quint8 nbElement, quint16 startIndex, quint8 sequenceNumber,
        QKnxNpdu::ApplicationLayerControlField apci, const QByteArray &data = {});

    static QKnxNpdu createFunctionPropertyNpdu(QKnxNpdu::ApplicationLayerControlField apci,
        quint8 objectIndex, QKnxInterfaceObjectProperty property, const QByteArray &data, quint8 sequenceNumber);

    static QKnxNpdu setupNpdu(QKnxNpdu::TransportLayerControlField tpci,
        QKnxNpdu::ApplicationLayerControlField apci, const QByteArray &data = {});

    static QKnxNpdu createUserMemoryReadWriteNpdu(quint8 addressExtention, quint8 number,
        const QKnxAddress &address, quint8 sequenceNumber, const QByteArray &data = {});
};

QT_END_NAMESPACE

#endif // QKNXNPDUFACTORY_H
