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
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxinterfaceobjectproperty.h>
#include <QtKnx/qknxinterfaceobjectpropertydatatype.h>
#include <QtKnx/qknxinterfaceobjecttype.h>
#include <QtKnx/qknxnpdu.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNpduFactory final
{
public:
    QKnxNpduFactory() = delete;

    struct Q_KNX_EXPORT Multicast
    {
        Multicast() = delete;

        static QKnxNpdu createGroupValueReadNpdu();
        static QKnxNpdu createGroupValueResponseNpdu(const QByteArray &data);
        static QKnxNpdu createGroupValueWriteNpdu(const QByteArray &data);

        static QKnxNpdu createGroupPropertyValueReadNpdu(QKnxInterfaceObjectType object,
                                                     quint8 objectInstance,
                                                     QKnxInterfaceObjectProperty property);
        static QKnxNpdu createGroupPropertyValueResponseNpdu(QKnxInterfaceObjectType object,
                                                     quint8 objectInstance,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &data);
        static QKnxNpdu createGroupPropertyValueWriteNpdu(QKnxInterfaceObjectType object,
                                                     quint8 objectInstance,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &data);
        static QKnxNpdu createGroupPropertyValueInfoReportNpdu(QKnxInterfaceObjectType object,
                                                     quint8 objectInstance,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &data);
    };

    struct Q_KNX_EXPORT Broadcast
    {
        Broadcast() = delete;

        static QKnxNpdu createNetworkParameterReadNpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &testInfo);
        static QKnxNpdu createNetworkParameterResponseNpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &testInfo,
                                                     const QByteArray &testResult);
        static QKnxNpdu createNetworkParameterWriteNpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &value);
        static QKnxNpdu createNetworkParameterInfoReportNpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &testInfo,
                                                     const QByteArray &testResult);

        static QKnxNpdu createSystemNetworkParameterReadNpdu(QKnxInterfaceObjectType object,
                                                     quint16 property,
                                                     const QByteArray &testInfo);
        static QKnxNpdu createSystemNetworkParameterResponseNpdu(QKnxInterfaceObjectType object,
                                                     quint16 property,
                                                     const QByteArray &testInfo,
                                                     const QByteArray &testResult);
        static QKnxNpdu createSystemNetworkParameterWriteNpdu(QKnxInterfaceObjectType object,
                                                     quint16 property,
                                                     const QByteArray &value);

        static QKnxNpdu createIndividualAddressReadNpdu();
        static QKnxNpdu createIndividualAddressResponseNpdu();
        static QKnxNpdu createIndividualAddressWriteNpdu(const QKnxAddress &newAddress);

        static QKnxNpdu createIndividualAddressSerialNumberReadNpdu(const QByteArray &sn);
        static QKnxNpdu createIndividualAddressSerialNumberResponseNpdu(const QByteArray &sn,
                                                     const QKnxAddress &domainAddress);
        static QKnxNpdu createIndividualAddressSerialNumberWriteNpdu(const QByteArray &sn,
                                                     const QKnxAddress &newAddress);

        static QKnxNpdu createDomainAddressReadNpdu();
        static QKnxNpdu createDomainAddressResponseNpdu(const QByteArray &domainAddress);
        static QKnxNpdu createDomainAddressWriteNpdu(const QByteArray &domainAddress);

        static QKnxNpdu createDomainAddressSerialNumberReadNpdu(const QByteArray &serialNumber);
        static QKnxNpdu createDomainAddressSerialNumberResponseNpdu(const QByteArray &serialNumber,
                                                     const QByteArray &domainAddress);
        static QKnxNpdu createDomainAddressSerialNumberWriteNpdu(const QByteArray &serialNumber,
                                                     const QByteArray &domainAddress);

        static QKnxNpdu createPll110DomainAddressSelectiveReadNpdu(quint8 domainAddress,
                                                     const QKnxAddress &startAddress,
                                                     quint8 range);
        static QKnxNpdu createRfDomainAddressSelectiveRead(const QByteArray &startAddress,
                                                     const QByteArray &endAddress);

        static QKnxNpdu createFeDomainAddressSelectiveReadNpdu(quint16 manufacturerId,
                                                     QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint16 parameters);
    };

    struct Q_KNX_EXPORT PointToPoint
    {
        PointToPoint() = delete;

        enum Mode
        {
            Connectionless,
            ConnectionOriented
        };

        static QKnxNpdu createMemoryReadNpdu(Mode mode, quint8 number, quint16 address,
                                                     quint8 seqNumber = 0);
        static QKnxNpdu createMemoryResponseNpdu(Mode mode, quint8 number, quint16 address,
                                                     const QByteArray &data, quint8 seqNumber = 0);
        static QKnxNpdu createMemoryWriteNpdu(Mode mode, quint8 number, quint16 address,
                                                     const QByteArray &data, quint8 seqNumber = 0);

        static QKnxNpdu createFunctionPropertyCommandNpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &data, quint8 seqNumber = 0);
        static QKnxNpdu createFunctionPropertyStateReadNpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &data, quint8 seqNumber = 0);
        static QKnxNpdu createFunctionPropertyStateResponseNpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     QKnxNpdu::ErrorCode code,
                                                     const QByteArray &data, quint8 seqNumber = 0);

        static QKnxNpdu createDeviceDescriptorReadNpdu(Mode mode, quint8 descriptorType,
                                                     quint8 seqNumber = 0);
        static QKnxNpdu createDeviceDescriptorResponseNpdu(Mode mode, quint8 descriptorType,
                                                     const QByteArray &deviceDescriptor,
                                                     quint8 seqNumber = 0);

        static QKnxNpdu createRestartNpdu(Mode mode, QKnxNpdu::ResetType type,
                                  QKnxNpdu::EraseCode code = QKnxNpdu::EraseCode::Reserved,
                                  quint8 channelNumber = 0, quint8 seqNumber = 0);
        static QKnxNpdu createRestartResponseNpdu(Mode mode, QKnxNpdu::ResetType type,
                                                     QKnxNpdu::ErrorCode code,
                                                     quint16 processTime, quint8 seqNumber = 0);

        static QKnxNpdu createPropertyValueReadNpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 nbElement, quint16 startIndex,
                                                     quint8 seqNumber = 0);
        static QKnxNpdu createPropertyValueResponseNpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 nbElement, quint16 startIndex,
                                                     const QByteArray &data, quint8 seqNumber = 0);
        static QKnxNpdu createPropertyValueWriteNpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 nbElement, quint16 startIndex,
                                                     const QByteArray &data, quint8 seqNumber = 0);

        static QKnxNpdu createPropertyDescriptionReadNpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 propertyIndex, quint8 seqNumber = 0);
        static QKnxNpdu createPropertyDescriptionResponseNpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 propertyIndex, bool writeable,
                                                     QKnxInterfaceObjectPropertyDataType::Id type,
                                                     quint16 maxSize, quint8 readLevel,
                                                     quint8 writeLevel, quint8 seqNumber = 0);

        static QKnxNpdu createLinkReadNpdu(Mode mode, quint8 groupObjectNumber, quint8 startIndex,
                                                     quint8 seqNumber = 0);
        static QKnxNpdu createLinkResponseNpdu(Mode mode, quint8 groupObjectNumber,
                                                     quint8 sendingAddress, quint8 startAddress,
                                                     const QVector<QKnxAddress> &addresses,
                                                     quint8 seqNumber = 0);
        static QKnxNpdu createLinkWriteNpdu(Mode mode, quint8 groupObjectNumber,
                                                     QKnxNpdu::LinkWriteFlags flags,
                                                     const QKnxAddress &groupAddress,
                                                     quint8 seqNumber = 0);

        static QKnxNpdu createFileStreamInfoReportNpdu(Mode mode, quint8 fileHandle,
                                                     quint8 fileBlockSeqNumber,
                                                     const QByteArray &data, quint8 seqNumber = 0);
    };

    struct Q_KNX_EXPORT PointToPointConnectionless
    {
        PointToPointConnectionless() = delete;

        static QKnxNpdu createNetworkParameterResponseNpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &testInfo,
                                                     const QByteArray &testResult);
        static QKnxNpdu createNetworkParameterWriteNpdu(QKnxInterfaceObjectType objectType,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QByteArray &value);
    };

    struct Q_KNX_EXPORT PointToPointConnectionOriented
    {
        PointToPointConnectionOriented() = delete;

        static QKnxNpdu createAdcReadNpdu(quint8 channel, quint8 readCount, quint8 seqNumber);
        static QKnxNpdu createAdcResponseNpdu(quint8 channel, quint8 readCount, quint16 sumOfAdc,
                                                     quint8 seqNumber);

        static QKnxNpdu createUserMemoryReadNpdu(quint8 addressExtention, quint8 number,
                                                     quint16 address, quint8 seqNumber);
        static QKnxNpdu createUserMemoryResponseNpdu(quint8 addressExtention, quint8 number,
                                                     quint16 address, const QByteArray &data,
                                                     quint8 seqNumber);
        static QKnxNpdu createUserMemoryWriteNpdu(quint8 addressExtention, quint8 number,
                                                     quint16 address,
                                                     const QByteArray &data, quint8 seqNumber);

        static QKnxNpdu createUserManufacturerInfoReadNpdu(quint8 seqNumber);
        static QKnxNpdu createUserManufacturerInfoResponseNpdu(quint8 manufacturerId,
                                                     quint16 manufacturerSpecific, quint8 seqNumber);

        static QKnxNpdu createAuthorizeRequestNpdu(quint32 key, quint8 seqNumber);
        static QKnxNpdu createAuthorizeResponseNpdu(quint8 level, quint8 seqNumber);

        static QKnxNpdu createKeyWriteNpdu(quint8 level, quint32 key, quint8 seqNumber);
        static QKnxNpdu createKeyResponseNpdu(quint8 level, quint8 seqNumber);
    };
};

QT_END_NAMESPACE

#endif // QKNXNPDUFACTORY_H
