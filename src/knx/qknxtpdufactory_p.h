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

#ifndef QKNXTPDUFACTORY_P_H
#define QKNXTPDUFACTORY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt KNX API.  It exists for the convenience
// of the Qt KNX implementation.  This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxinterfaceobjectproperty.h>
#include <QtKnx/qknxinterfaceobjectpropertydatatype.h>
#include <QtKnx/qknxinterfaceobjecttype.h>
#include <QtKnx/qknxtpdu.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxTpduFactory final
{
public:
    QKnxTpduFactory() = delete;

    struct Q_KNX_EXPORT Multicast
    {
        Multicast() = delete;

        static QKnxTpdu createGroupValueReadTpdu();
        static QKnxTpdu createGroupValueResponseTpdu(const QKnxByteArray &data);
        static QKnxTpdu createGroupValueWriteTpdu(const QKnxByteArray &data);

        static QKnxTpdu createGroupPropertyValueReadTpdu(QKnxInterfaceObjectType object,
                                                     quint8 objectInstance,
                                                     QKnxInterfaceObjectProperty property);
        static QKnxTpdu createGroupPropertyValueResponseTpdu(QKnxInterfaceObjectType object,
                                                     quint8 objectInstance,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &data);
        static QKnxTpdu createGroupPropertyValueWriteTpdu(QKnxInterfaceObjectType object,
                                                     quint8 objectInstance,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &data);
        static QKnxTpdu createGroupPropertyValueInfoReportTpdu(QKnxInterfaceObjectType object,
                                                     quint8 objectInstance,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &data);
    };

    struct Q_KNX_EXPORT Broadcast
    {
        Broadcast() = delete;

        static QKnxTpdu createNetworkParameterReadTpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &testInfo);
        static QKnxTpdu createNetworkParameterResponseTpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &testInfo,
                                                     const QKnxByteArray &testResult);
        static QKnxTpdu createNetworkParameterWriteTpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &value);
        static QKnxTpdu createNetworkParameterInfoReportTpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &testInfo,
                                                     const QKnxByteArray &testResult);

        static QKnxTpdu createSystemNetworkParameterReadTpdu(QKnxInterfaceObjectType object,
                                                     quint16 property,
                                                     const QKnxByteArray &testInfo);
        static QKnxTpdu createSystemNetworkParameterResponseTpdu(QKnxInterfaceObjectType object,
                                                     quint16 property,
                                                     const QKnxByteArray &testInfo,
                                                     const QKnxByteArray &testResult);
        static QKnxTpdu createSystemNetworkParameterWriteTpdu(QKnxInterfaceObjectType object,
                                                     quint16 property,
                                                     const QKnxByteArray &value);

        static QKnxTpdu createIndividualAddressReadTpdu();
        static QKnxTpdu createIndividualAddressResponseTpdu();
        static QKnxTpdu createIndividualAddressWriteTpdu(const QKnxAddress &newAddress);

        static QKnxTpdu createIndividualAddressSerialNumberReadTpdu(const QKnxByteArray &sn);
        static QKnxTpdu createIndividualAddressSerialNumberResponseTpdu(const QKnxByteArray &sn,
                                                     const QKnxAddress &domainAddress);
        static QKnxTpdu createIndividualAddressSerialNumberWriteTpdu(const QKnxByteArray &sn,
                                                     const QKnxAddress &newAddress);

        static QKnxTpdu createDomainAddressReadTpdu();
        static QKnxTpdu createDomainAddressResponseTpdu(const QKnxByteArray &domainAddress);
        static QKnxTpdu createDomainAddressWriteTpdu(const QKnxByteArray &domainAddress);

        static QKnxTpdu createDomainAddressSerialNumberReadTpdu(const QKnxByteArray &serialNumber);
        static QKnxTpdu createDomainAddressSerialNumberResponseTpdu(const QKnxByteArray &serialNumber,
                                                     const QKnxByteArray &domainAddress);
        static QKnxTpdu createDomainAddressSerialNumberWriteTpdu(const QKnxByteArray &serialNumber,
                                                     const QKnxByteArray &domainAddress);

        static QKnxTpdu createPll110DomainAddressSelectiveReadTpdu(quint8 domainAddress,
                                                     const QKnxAddress &startAddress,
                                                     quint8 range);
        static QKnxTpdu createRfDomainAddressSelectiveReadTpdu(const QKnxByteArray &startAddress,
                                                     const QKnxByteArray &endAddress);

        static QKnxTpdu createFeDomainAddressSelectiveReadTpdu(quint16 manufacturerId,
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

        static QKnxTpdu createFunctionPropertyCommandTpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &data, quint8 seqNumber = 0);
        static QKnxTpdu createFunctionPropertyStateReadTpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &data, quint8 seqNumber = 0);
        static QKnxTpdu createFunctionPropertyStateResponseTpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     QKnxTpdu::ErrorCode code,
                                                     const QKnxByteArray &data, quint8 seqNumber = 0);

        static QKnxTpdu createDeviceDescriptorReadTpdu(Mode mode, quint8 descriptorType,
                                                     quint8 seqNumber = 0);
        static QKnxTpdu createDeviceDescriptorResponseTpdu(Mode mode, quint8 descriptorType,
                                                     const QKnxByteArray &deviceDescriptor,
                                                     quint8 seqNumber = 0);

        static QKnxTpdu createRestartTpdu(Mode mode, QKnxTpdu::ResetType type,
                                  QKnxTpdu::EraseCode code = QKnxTpdu::EraseCode::Reserved,
                                  quint8 channelNumber = 0, quint8 seqNumber = 0);
        static QKnxTpdu createRestartResponseTpdu(Mode mode, QKnxTpdu::ResetType type,
                                                     QKnxTpdu::ErrorCode code,
                                                     quint16 processTime, quint8 seqNumber = 0);

        static QKnxTpdu createPropertyValueReadTpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 nbElement, quint16 startIndex,
                                                     quint8 seqNumber = 0);
        static QKnxTpdu createPropertyValueResponseTpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 nbElement, quint16 startIndex,
                                                     const QKnxByteArray &data, quint8 seqNumber = 0);
        static QKnxTpdu createPropertyValueWriteTpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 nbElement, quint16 startIndex,
                                                     const QKnxByteArray &data, quint8 seqNumber = 0);

        static QKnxTpdu createPropertyDescriptionReadTpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 propertyIndex, quint8 seqNumber = 0);
        static QKnxTpdu createPropertyDescriptionResponseTpdu(Mode mode, quint8 objectIndex,
                                                     QKnxInterfaceObjectProperty property,
                                                     quint8 propertyIndex, bool writeable,
                                                     QKnxInterfaceObjectPropertyDataType::Id type,
                                                     quint16 maxSize, quint8 readLevel,
                                                     quint8 writeLevel, quint8 seqNumber = 0);

        static QKnxTpdu createLinkReadTpdu(Mode mode, quint8 groupObjectNumber, quint8 startIndex,
                                                     quint8 seqNumber = 0);
        static QKnxTpdu createLinkResponseTpdu(Mode mode, quint8 groupObjectNumber,
                                                     quint8 sendingAddress, quint8 startAddress,
                                                     const QVector<QKnxAddress> &addresses,
                                                     quint8 seqNumber = 0);
        static QKnxTpdu createLinkWriteTpdu(Mode mode, quint8 groupObjectNumber,
                                                     QKnxTpdu::LinkWriteFlags flags,
                                                     const QKnxAddress &groupAddress,
                                                     quint8 seqNumber = 0);

        static QKnxTpdu createFileStreamInfoReportTpdu(Mode mode, quint8 fileHandle,
                                                     quint8 fileBlockSeqNumber,
                                                     const QKnxByteArray &data, quint8 seqNumber = 0);
    };

    struct Q_KNX_EXPORT PointToPointConnectionless
    {
        PointToPointConnectionless() = delete;

        static QKnxTpdu createNetworkParameterResponseTpdu(QKnxInterfaceObjectType object,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &testInfo,
                                                     const QKnxByteArray &testResult);
        static QKnxTpdu createNetworkParameterWriteTpdu(QKnxInterfaceObjectType objectType,
                                                     QKnxInterfaceObjectProperty property,
                                                     const QKnxByteArray &value);
    };

    struct Q_KNX_EXPORT PointToPointConnectionOriented
    {
        PointToPointConnectionOriented() = delete;

        static QKnxTpdu createAdcReadTpdu(quint8 channel, quint8 readCount, quint8 seqNumber);
        static QKnxTpdu createAdcResponseTpdu(quint8 channel, quint8 readCount, quint16 sumOfAdc,
                                                     quint8 seqNumber);

        static QKnxTpdu createMemoryReadTpdu(quint8 number, quint16 address, quint8 seqNumber = 0);
        static QKnxTpdu createMemoryResponseTpdu(quint8 number, quint16 address,
                                                     const QKnxByteArray &data, quint8 seqNumber = 0);
        static QKnxTpdu createMemoryWriteTpdu(quint8 number, quint16 address,
                                                     const QKnxByteArray &data, quint8 seqNumber = 0);

        static QKnxTpdu createUserMemoryReadTpdu(quint8 addressExtention, quint8 number,
                                                     quint16 address, quint8 seqNumber);
        static QKnxTpdu createUserMemoryResponseTpdu(quint8 addressExtention, quint8 number,
                                                     quint16 address, const QKnxByteArray &data,
                                                     quint8 seqNumber);
        static QKnxTpdu createUserMemoryWriteTpdu(quint8 addressExtention, quint8 number,
                                                     quint16 address,
                                                     const QKnxByteArray &data, quint8 seqNumber);

        static QKnxTpdu createUserManufacturerInfoReadTpdu(quint8 seqNumber);
        static QKnxTpdu createUserManufacturerInfoResponseTpdu(quint8 manufacturerId,
                                                     quint16 manufacturerSpecific, quint8 seqNumber);

        static QKnxTpdu createAuthorizeRequestTpdu(quint32 key, quint8 seqNumber);
        static QKnxTpdu createAuthorizeResponseTpdu(quint8 level, quint8 seqNumber);

        static QKnxTpdu createKeyWriteTpdu(quint8 level, quint32 key, quint8 seqNumber);
        static QKnxTpdu createKeyResponseTpdu(quint8 level, quint8 seqNumber);
    };
};

QT_END_NAMESPACE

#endif // QKNXTPDUFACTORY_H
