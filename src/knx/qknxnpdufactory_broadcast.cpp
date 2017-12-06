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

#include "qknxnpdufactory.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNpduFactory::Broadcast

    \inmodule QtKnx
    \brief The QKnxNpduFactory::Broadcast class is used to create a valid
    Network protocol data unit (\l QKnxNpdu) for application services sent by
    broadcast.
*/

// -- A_NetworkParameter

static QKnxNpdu createNetworkParameterNpdu(QKnxNpdu::ApplicationControlField apci,
                                           QKnxInterfaceObjectType object,
                                           QKnxInterfaceObjectProperty property,
                                           const QByteArray &data, // aka. testInfo
                                           const QByteArray &testResult = {})
{
    if (!QKnxInterfaceObjectType::isMatch(object, property))
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast, apci,
        QKnxUtils::QUint16::bytes(quint16(object)) + QKnxUtils::QUint8::bytes(quint8(property))
        + data + testResult };
}

QKnxNpdu QKnxNpduFactory::Broadcast::createNetworkParameterReadNpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QByteArray &testInfo)
{
    if (testInfo.size() > 250) // L_Data_Extended -> max 254 bytes payload
        return {}; // 5 bytes already used for APCI, object, instance

    return createNetworkParameterNpdu(QKnxNpdu::ApplicationControlField::NetworkParameterRead,
        object, property, testInfo);
}

QKnxNpdu
QKnxNpduFactory::Broadcast::createNetworkParameterResponseNpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QByteArray &testInfo, const QByteArray &testResult)
{
    if ((testInfo.size() + testResult.size()) > 11) // L_Data -> max 14 bytes payload
        return {}; // 5 bytes already used for APCI, object, instance

    return createNetworkParameterNpdu(QKnxNpdu::ApplicationControlField::NetworkParameterResponse,
        object, property, testInfo, testResult);
}

QKnxNpdu QKnxNpduFactory::Broadcast::createNetworkParameterWriteNpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QByteArray &value)
{
    if (value.size() > 250) // L_Data_Extended -> max 254 bytes payload
        return {}; // 5 bytes already used for APCI, object, instance

    return createNetworkParameterNpdu(QKnxNpdu::ApplicationControlField::NetworkParameterWrite,
        object, property, value);
}

QKnxNpdu
QKnxNpduFactory::Broadcast::createNetworkParameterInfoReportNpdu(QKnxInterfaceObjectType object,
    QKnxInterfaceObjectProperty property, const QByteArray &testInfo, const QByteArray &testResult)
{
    if ((testInfo.size() + testResult.size()) > 250) // L_Data_Extended -> max 254 bytes payload
        return {}; // 5 bytes already used for APCI, object, instance

    return createNetworkParameterNpdu(QKnxNpdu::ApplicationControlField::NetworkParameterInfoReport,
        object, property, testInfo, testResult);
}


// -- A_SystemNetworkParameter

static QKnxNpdu createSystemNetworkParameterNpdu(QKnxNpdu::ApplicationControlField apci,
                                                 QKnxInterfaceObjectType object,
                                                 quint16 property,
                                                 const QByteArray &data, // aka. operand, testInfo
                                                 const QByteArray &testResult = {})
{
    if (property > 0x0fff)
        return {};

    if ((data.size() + testResult.size()) > 249) // L_Data_Extended -> max 254 bytes payload
        return {}; // 6 bytes already used for APCI, object type, PID

    return { QKnxNpdu::TransportControlField::DataSystemBroadcast, apci,
        QKnxUtils::QUint16::bytes(quint16(object)) + QKnxUtils::QUint8::bytes(quint8(property >> 4))
         + QKnxUtils::QUint8::bytes(quint8(property << 4)) + data + testResult };
}

QKnxNpdu QKnxNpduFactory::Broadcast
::createSystemNetworkParameterReadNpdu(QKnxInterfaceObjectType object, quint16 property,
    const QByteArray &testInfo)
{
    return createSystemNetworkParameterNpdu(
        QKnxNpdu::ApplicationControlField::SystemNetworkParameterRead, object, property, testInfo);
}

QKnxNpdu
QKnxNpduFactory::Broadcast::createSystemNetworkParameterResponseNpdu(QKnxInterfaceObjectType object,
    quint16 property, const QByteArray &testInfo, const QByteArray &testResult)
{
    return createSystemNetworkParameterNpdu(
        QKnxNpdu::ApplicationControlField::SystemNetworkParameterResponse, object, property,
        testInfo, testResult);
}

QKnxNpdu
QKnxNpduFactory::Broadcast::createSystemNetworkParameterWriteNpdu(QKnxInterfaceObjectType object,
    quint16 property, const QByteArray &value)
{
    return createSystemNetworkParameterNpdu(
        QKnxNpdu::ApplicationControlField::SystemNetworkParameterWrite, object, property, value);
}


// -- A_IndividualAddress

/*!
    Returns a NPDU for Individual Address Read Application Service.
*/
QKnxNpdu QKnxNpduFactory::Broadcast::createIndividualAddressReadNpdu()
{
    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::IndividualAddressRead };
}

/*!
    Returns a NPDU for Individual Address Write Application Service with the
    given \l QKnxAddress \a address; or an empty NPDU if the \a address is not
    of type \l QKnxAddress::Individual.
*/
QKnxNpdu QKnxNpduFactory::Broadcast::createIndividualAddressWriteNpdu(const QKnxAddress &address)
{
    if (address.type() != QKnxAddress::Type::Individual)
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::IndividualAddressWrite, address.bytes() };
}

QKnxNpdu
QKnxNpduFactory::Broadcast::createIndividualAddressResponseNpdu()
{
    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::IndividualAddressResponse };
}


// -- A_IndividualAddressSerialNumber

/*!
    Returns a NPDU for Individual Address Serial Number Read Application Service
    with the given \a sn set; or an empty NPDU if the \a sn
    has a different size then six octets.
*/
QKnxNpdu
QKnxNpduFactory::Broadcast::createIndividualAddressSerialNumberReadNpdu(const QByteArray &sn)
{
    if (sn.size() != 6)
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::IndividualAddressSerialNumberRead, sn };
}

QKnxNpdu
QKnxNpduFactory::Broadcast::createIndividualAddressSerialNumberResponseNpdu(const QByteArray &sn,
    const QKnxAddress &domainAddress)
{
    if ((sn.size() != 6) || (domainAddress.type() != QKnxAddress::Type::Individual))
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::IndividualAddressSerialNumberResponse, sn
        + domainAddress.bytes() + QKnxUtils::QUint16::bytes(0u) };
}

/*!
    Returns a NPDU for Individual Address Serial Number Write Application Service
    with the given \a sn and \l QKnxAddress \a newAddress set; or otherwise an
    empty NPDU if the \a sn has a different size then six octets or the type of
    \a newAddress is not of type \l QKnxAddress::Individual.
*/
QKnxNpdu
QKnxNpduFactory::Broadcast::createIndividualAddressSerialNumberWriteNpdu(const QByteArray &sn,
    const QKnxAddress &newAddress)
{
    if ((sn.size() != 6) || (newAddress.type() != QKnxAddress::Type::Individual))
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::IndividualAddressSerialNumberWrite, sn + newAddress
        .bytes() + QKnxUtils::QUint32::bytes(0u) };
}


// -- A_DomainAddress

/*!
    Returns a NPDU for Domain Address Read Application Service.
*/
QKnxNpdu QKnxNpduFactory::Broadcast::createDomainAddressReadNpdu()
{
    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::DomainAddressRead };
}

QKnxNpdu QKnxNpduFactory::Broadcast::createDomainAddressResponseNpdu(const QByteArray &address)
{
    if ((address.size() != 2) && (address.size() != 6))
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::DomainAddressResponse, address };
}

/*!
    Returns a NPDU for Domain Address Write Application Service with the given
    \l QKnxAddress \a address set.
*/
QKnxNpdu QKnxNpduFactory::Broadcast::createDomainAddressWriteNpdu(const QByteArray &address)
{
    if ((address.size() != 2) && (address.size() != 6))
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::DomainAddressWrite, address };
}


// -- A_CreateDomainAddressSerialNumber

/*!
    Returns a NPDU for Domain Address Serial Number Read Application Service
    with the given \a sn set; or an empty NPDU if the \a sn has a different size
    then six octets.
*/
QKnxNpdu
QKnxNpduFactory::Broadcast::createDomainAddressSerialNumberReadNpdu(const QByteArray &sn)
{
    if (sn.size() != 6)
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::DomainAddressSerialNumberRead, sn };
}

QKnxNpdu
QKnxNpduFactory::Broadcast::createDomainAddressSerialNumberResponseNpdu(const QByteArray &sn,
    const QByteArray &domainAddress)
{
    if ((sn.size() != 6) || ((domainAddress.size() != 2) && (domainAddress.size() != 6)))
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::DomainAddressSerialNumberResponse, sn + domainAddress };
}

/*!
    Returns a NPDU for Domain Address Serial Number Write Application Service
    with the given \a sn and \l QKnxAddress \a domainAddress set; or otherwise
    an empty NPDU if the \a sn has a different size then six octets.
*/
QKnxNpdu QKnxNpduFactory::Broadcast::createDomainAddressSerialNumberWriteNpdu(const QByteArray &sn,
    const QByteArray &domainAddress)
{
    if ((sn.size() != 6) || ((domainAddress.size() != 2) && (domainAddress.size() != 6)))
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::DomainAddressSerialNumberWrite, sn + domainAddress };
}


// -- A_DomainAddressSelective_Read

QKnxNpdu QKnxNpduFactory::Broadcast::createPll110DomainAddressSelectiveReadNpdu(quint8 domainAddress,
    const QKnxAddress &startAddress, quint8 range)
{
    if (startAddress.type() != QKnxAddress::Type::Individual)
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::DomainAddressSelectiveRead, QKnxUtils::QUint8::bytes(0u)
        + QKnxUtils::QUint8::bytes(domainAddress) + startAddress.bytes()
        + QKnxUtils::QUint8::bytes(range) };
}

QKnxNpdu
QKnxNpduFactory::Broadcast::createRfDomainAddressSelectiveRead(const QByteArray &startAddress,
    const QByteArray &endAddress)
{
    if ((startAddress.size() != 6) && (endAddress.size() != 6))
        return {};

    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::DomainAddressSelectiveRead, startAddress + endAddress
        + QKnxUtils::QUint8::bytes(0u) };
}

QKnxNpdu QKnxNpduFactory::Broadcast::createFeDomainAddressSelectiveReadNpdu(quint16 manufacturerId,
    QKnxInterfaceObjectType obj, QKnxInterfaceObjectProperty property, quint16 parameters)
{
    return { QKnxNpdu::TransportControlField::DataBroadcast,
        QKnxNpdu::ApplicationControlField::DomainAddressSelectiveRead,
        QKnxUtils::QUint16::bytes(manufacturerId) + QKnxUtils::QUint16::bytes(quint16(obj))
        + QKnxUtils::QUint8::bytes(quint8(property)) + QKnxUtils::QUint16::bytes(parameters) };
}

QT_END_NAMESPACE
