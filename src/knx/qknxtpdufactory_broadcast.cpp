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

#include "qknxtpdufactory.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxTpduFactory::Broadcast

    \inmodule QtKnx
    \brief The QKnxTpduFactory::Broadcast class is used to create a valid
    Transport protocol data unit (\l QKnxTpdu) for application services sent by
    broadcast.
*/

// -- A_NetworkParameter

static QKnxTpdu createNetworkParameterTpdu(QKnxTpdu::ApplicationControlField apci,
                                           QKnxInterfaceObjectType object,
                                           QKnxInterfaceObjectProperty property,
                                           const QVector<quint8> &data, // aka. testInfo
                                           const QVector<quint8> &testResult = {})
{
    if (!QKnxInterfaceObjectType::isMatch(object, property))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast, apci,
        QKnxUtils::QUint16::bytes<QVector<quint8>>(quint16(object)) + QKnxUtils::QUint8::bytes<QVector<quint8>>(quint8(property))
        + data + testResult };
}

QKnxTpdu QKnxTpduFactory::Broadcast::createNetworkParameterReadTpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QVector<quint8> &testInfo)
{
    if (testInfo.size() > 250) // L_Data_Extended -> max 254 bytes payload
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 4 bytes already used for APCI, object type, PID

    return createNetworkParameterTpdu(QKnxTpdu::ApplicationControlField::NetworkParameterRead,
        object, property, testInfo);
}

QKnxTpdu
QKnxTpduFactory::Broadcast::createNetworkParameterResponseTpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QVector<quint8> &testInfo, const QVector<quint8> &testResult)
{
    if ((testInfo.size() + testResult.size()) > 250) // L_Data_Extended -> max 254 bytes payload
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 4 bytes already used for APCI, object type, PID

    return createNetworkParameterTpdu(QKnxTpdu::ApplicationControlField::NetworkParameterResponse,
        object, property, testInfo, testResult);
}

QKnxTpdu QKnxTpduFactory::Broadcast::createNetworkParameterWriteTpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QVector<quint8> &value)
{
    if (value.size() > 250) // L_Data_Extended -> max 254 bytes payload
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 4 bytes already used for APCI, object type, property id

    return createNetworkParameterTpdu(QKnxTpdu::ApplicationControlField::NetworkParameterWrite,
        object, property, value);
}

QKnxTpdu
QKnxTpduFactory::Broadcast::createNetworkParameterInfoReportTpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QVector<quint8> &testInfo, const QVector<quint8> &testResult)
{
    if ((testInfo.size() + testResult.size()) > 250) // L_Data_Extended -> max 254 bytes payload
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 4 bytes already used for APCI, object type, property id

    return createNetworkParameterTpdu(QKnxTpdu::ApplicationControlField::NetworkParameterInfoReport,
        object, property, testInfo, testResult);
}


// -- A_SystemNetworkParameter

static QKnxTpdu createSystemNetworkParameterTpdu(QKnxTpdu::ApplicationControlField apci,
                                                 QKnxInterfaceObjectType object,
                                                 quint16 property,
                                                 const QVector<quint8> &data, // aka. operand, testInfo
                                                 const QVector<quint8> &testResult = {})
{
    if (property > 0x0fff)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    if ((data.size() + testResult.size()) > 249) // L_Data_Extended -> max 254 bytes payload
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid}; // 5 bytes already used for APCI, object type, PID

    return { QKnxTpdu::TransportControlField::DataSystemBroadcast, apci,
        QKnxUtils::QUint16::bytes<QVector<quint8>>(quint16(object)) + QKnxUtils::QUint8::bytes<QVector<quint8>>(quint8(property >> 4))
         + QKnxUtils::QUint8::bytes<QVector<quint8>>(quint8(property << 4)) + data + testResult };
}

QKnxTpdu QKnxTpduFactory::Broadcast
::createSystemNetworkParameterReadTpdu(QKnxInterfaceObjectType object, quint16 property,
    const QVector<quint8> &testInfo)
{
    return createSystemNetworkParameterTpdu(
        QKnxTpdu::ApplicationControlField::SystemNetworkParameterRead, object, property, testInfo);
}

QKnxTpdu
QKnxTpduFactory::Broadcast::createSystemNetworkParameterResponseTpdu(QKnxInterfaceObjectType object,
    quint16 property, const QVector<quint8> &testInfo, const QVector<quint8> &testResult)
{
    return createSystemNetworkParameterTpdu(
        QKnxTpdu::ApplicationControlField::SystemNetworkParameterResponse, object, property,
        testInfo, testResult);
}

QKnxTpdu
QKnxTpduFactory::Broadcast::createSystemNetworkParameterWriteTpdu(QKnxInterfaceObjectType object,
    quint16 property, const QVector<quint8> &value)
{
    return createSystemNetworkParameterTpdu(
        QKnxTpdu::ApplicationControlField::SystemNetworkParameterWrite, object, property, value);
}


// -- A_IndividualAddress

/*!
    Returns a TPDU for Individual Address Read Application Service.
*/
QKnxTpdu QKnxTpduFactory::Broadcast::createIndividualAddressReadTpdu()
{
    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::IndividualAddressRead };
}

/*!
    Returns a TPDU for Individual Address Write Application Service with the
    given \l QKnxAddress \a address; or an empty TPDU if the \a address is not
    of type \l QKnxAddress::Individual.
*/
QKnxTpdu QKnxTpduFactory::Broadcast::createIndividualAddressWriteTpdu(const QKnxAddress &address)
{
    if (address.type() != QKnxAddress::Type::Individual)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::IndividualAddressWrite, address.bytes<QVector<quint8>>() };
}

QKnxTpdu
QKnxTpduFactory::Broadcast::createIndividualAddressResponseTpdu()
{
    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::IndividualAddressResponse };
}


// -- A_IndividualAddressSerialNumber

/*!
    Returns a TPDU for Individual Address Serial Number Read Application Service
    with the given \a sn set; or an empty TPDU if the \a sn
    has a different size then six octets.
*/
QKnxTpdu
QKnxTpduFactory::Broadcast::createIndividualAddressSerialNumberReadTpdu(const QVector<quint8> &sn)
{
    if (sn.size() != 6)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::IndividualAddressSerialNumberRead, sn };
}

QKnxTpdu
QKnxTpduFactory::Broadcast::createIndividualAddressSerialNumberResponseTpdu(const QVector<quint8> &sn,
    const QKnxAddress &domainAddress)
{
    if ((sn.size() != 6) || (domainAddress.type() != QKnxAddress::Type::Individual))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::IndividualAddressSerialNumberResponse, sn
        + domainAddress.bytes<QVector<quint8>>() + QKnxUtils::QUint16::bytes<QVector<quint8>>(0u) };
}

/*!
    Returns a TPDU for Individual Address Serial Number Write Application Service
    with the given \a sn and \l QKnxAddress \a newAddress set; or otherwise an
    empty TPDU if the \a sn has a different size then six octets or the type of
    \a newAddress is not of type \l QKnxAddress::Individual.
*/
QKnxTpdu
QKnxTpduFactory::Broadcast::createIndividualAddressSerialNumberWriteTpdu(const QVector<quint8> &sn,
    const QKnxAddress &newAddress)
{
    if ((sn.size() != 6) || (newAddress.type() != QKnxAddress::Type::Individual))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::IndividualAddressSerialNumberWrite, sn + newAddress
        .bytes<QVector<quint8>>() + QKnxUtils::QUint32::bytes<QVector<quint8>>(0u) };
}


// -- A_DomainAddress

/*!
    Returns a TPDU for Domain Address Read Application Service.
*/
QKnxTpdu QKnxTpduFactory::Broadcast::createDomainAddressReadTpdu()
{
    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::DomainAddressRead };
}

QKnxTpdu QKnxTpduFactory::Broadcast::createDomainAddressResponseTpdu(const QVector<quint8> &address)
{
    if ((address.size() != 2) && (address.size() != 6))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::DomainAddressResponse, address };
}

/*!
    Returns a TPDU for Domain Address Write Application Service with the given
    \l QKnxAddress \a address set.
*/
QKnxTpdu QKnxTpduFactory::Broadcast::createDomainAddressWriteTpdu(const QVector<quint8> &address)
{
    if ((address.size() != 2) && (address.size() != 6))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::DomainAddressWrite, address };
}


// -- A_CreateDomainAddressSerialNumber

/*!
    Returns a TPDU for Domain Address Serial Number Read Application Service
    with the given \a sn set; or an empty TPDU if the \a sn has a different size
    then six octets.
*/
QKnxTpdu
QKnxTpduFactory::Broadcast::createDomainAddressSerialNumberReadTpdu(const QVector<quint8> &sn)
{
    if (sn.size() != 6)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::DomainAddressSerialNumberRead, sn };
}

QKnxTpdu
QKnxTpduFactory::Broadcast::createDomainAddressSerialNumberResponseTpdu(const QVector<quint8> &sn,
    const QVector<quint8> &domainAddress)
{
    if ((sn.size() != 6) || ((domainAddress.size() != 2) && (domainAddress.size() != 6)))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::DomainAddressSerialNumberResponse, sn + domainAddress };
}

/*!
    Returns a TPDU for Domain Address Serial Number Write Application Service
    with the given \a sn and \l QKnxAddress \a domainAddress set; or otherwise
    an empty TPDU if the \a sn has a different size then six octets.
*/
QKnxTpdu QKnxTpduFactory::Broadcast::createDomainAddressSerialNumberWriteTpdu(const QVector<quint8> &sn,
    const QVector<quint8> &domainAddress)
{
    if ((sn.size() != 6) || ((domainAddress.size() != 2) && (domainAddress.size() != 6)))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::DomainAddressSerialNumberWrite, sn + domainAddress };
}


// -- A_DomainAddressSelective_Read

QKnxTpdu QKnxTpduFactory::Broadcast::createPll110DomainAddressSelectiveReadTpdu(quint8 domainAddress,
    const QKnxAddress &startAddress, quint8 range)
{
    if (startAddress.type() != QKnxAddress::Type::Individual)
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::DomainAddressSelectiveRead, QKnxUtils::QUint8::bytes<QVector<quint8>>(0u)
        + QKnxUtils::QUint8::bytes<QVector<quint8>>(domainAddress) + startAddress.bytes<QVector<quint8>>()
        + QKnxUtils::QUint8::bytes<QVector<quint8>>(range) };
}

QKnxTpdu
QKnxTpduFactory::Broadcast::createRfDomainAddressSelectiveReadTpdu(const QVector<quint8> &startAddress,
    const QVector<quint8> &endAddress)
{
    if ((startAddress.size() != 6) && (endAddress.size() != 6))
        return {QKnxTpdu::TransportControlField::Invalid,
            QKnxTpdu::ApplicationControlField::Invalid};

    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::DomainAddressSelectiveRead, startAddress + endAddress
        + QKnxUtils::QUint8::bytes<QVector<quint8>>(0u) };
}

QKnxTpdu QKnxTpduFactory::Broadcast::createFeDomainAddressSelectiveReadTpdu(quint16 manufacturerId,
    QKnxInterfaceObjectType obj, QKnxInterfaceObjectProperty property, quint16 parameters)
{
    return { QKnxTpdu::TransportControlField::DataBroadcast,
        QKnxTpdu::ApplicationControlField::DomainAddressSelectiveRead,
        QKnxUtils::QUint16::bytes<QVector<quint8>>(manufacturerId) + QKnxUtils::QUint16::bytes<QVector<quint8>>(quint16(obj))
        + QKnxUtils::QUint8::bytes<QVector<quint8>>(quint8(property)) + QKnxUtils::QUint16::bytes<QVector<quint8>>(parameters) };
}

QT_END_NAMESPACE
