/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qknxnamespace.h"

QT_BEGIN_NAMESPACE

/*!
    \namespace QKnx

    \inmodule QtKnx
    \brief Contains miscellaneous identifiers used throughout the QtKnx library.
*/

/*!
    \enum QKnx::EmiType

    This enum describes the various external message interface (EMI) types
    supported by KNX bus.

    \value Unknown
            The supported EMI type is unknown.
    \value EMI1
            The supported EMI is type 1 used by bus coupler units (BCU) model
            type 1.
    \value EMI2
            The supported EMI is type 2 used by bus coupler units (BCU) model
            type 2.
    \value cEMI
            The supported EMI type is known as \e {common EMI} and a generic
            structure for medium independent KNX messages.
*/

/*!
    \enum QKnx::MediumType

    This enum describes the various medium types supported by KNX bus.

    \value Unknown      The supported medium type is unknown.
    \value TP           The supported medium type is twisted pair 1.
    \value PL           The supported medium type is power-line 110.
    \value RF           The supported medium type is radio frequency.
    \value NetIP        The supported medium type is KNXnet/IP.
*/

/*!
    \enum QKnx::MediumStatus

    This enum type holds the medium status if the KNXnet/IP router or server
    supports extended device information.

    \value Unknown
            The medium status is unknown. Most likely the discovered KNXnet/IP
            router or server does not support extended device information.
    \value CommunicationPossible
            Communication to a KNX twisted pair 1 (TP1) network via the
            discovered KNXnet/IP router or server is possible.
    \value CommunicationImpossible
            Communication to a KNX TP1 network via the discovered KNXnet/IP
            router or server is not possible.
*/

/*!
    \enum QKnx::InterfaceFeature

    This enumeration contains the values to identify an interface feature
    service (also known as device feature).

    \value Unknown      The used feature service is unknown.
    \value SupportedEmiType
           Getting the supported EMI type(s).
    \value HostDeviceDescriptorType0
           Getting the local device descriptor type 0 for possible local device
           management.
    \value BusConnectionStatus
           Getting and informing on the bus connection status.
    \value KnxManufacturerCode
           Getting the manufacturer code of the bus access server.
    \value ActiveEmiType
           Getting and setting the EMI type to use.
    \value IndividualAddress
           Getting the individual address used by the bus interface.
    \value MaximumApduLength
           Getting the maximal APDU-length that can be transported over a
           KNXnet/IP tunneling connection.
    \value InterfaceFeatureInfoServiceEnable
           Controlling the use of the
           \l{QKnx::NetIp::TunnelingFeatureInfo}{info feature} service by the
           interface.
*/

/*!
    \fn QKnx::isInterfaceFeature(QKnx::InterfaceFeature feature)

    Returns \c true if the specified \a feature is a part of the
    \l InterfaceFeature enumeration; otherwise returns \c false.
*/
bool QKnx::isInterfaceFeature(QKnx::InterfaceFeature feature)
{
    switch (feature) {
    case QKnx::InterfaceFeature::SupportedEmiType:
    case QKnx::InterfaceFeature::HostDeviceDescriptorType0:
    case QKnx::InterfaceFeature::BusConnectionStatus:
    case QKnx::InterfaceFeature::KnxManufacturerCode:
    case QKnx::InterfaceFeature::ActiveEmiType:
    case QKnx::InterfaceFeature::IndividualAddress:
    case QKnx::InterfaceFeature::MaximumApduLength:
    case QKnx::InterfaceFeature::InterfaceFeatureInfoServiceEnable:
        return true;
    case QKnx::InterfaceFeature::Unknown:
        break;
    }
    return false;
}

/*!
    \enum QKnx::ReturnCode

    This enumeration contains the generic return codes used in the KNX
    specification.

    \value Success
            The service, function, or command was executed successfully, without
            additional information.
    \value SuccessWithCrc
            Positive confirmation of a message with CRC16-CCITT (beginning
            after the APCI octet, over received data including address and
            number of data octets).
    \value MemoryError
            Memory cannot be accessed at all or only with faults.
    \value CommandInvalid
            The command is not supported by this server.
    \value CommandImpossible
            The command is supported and well formatted, but it cannot be
            executed, because a dependency is not fulfilled.
    \value LengthExceedsMaxApduLength
            The requested data will not fit into a frame supported by this
            server. This return code indicates device limitations of the maximum
            supported frame length while accessing the resources of the device,
            such as properties, function properties, and memory.
    \value DataOverflow
            An attempt was made to write data beyond what is reserved for
            the addressed resource.
    \value DataMin
            The write value was too low. If the value is lower than the lowest
            supported value, then preferably this value shall be given instead
            of \e {Value not supported}.
    \value DataMax
            The write value was too high. If the value is higher than the highest
            supported value, then preferably this value shall be given instead of
            \e {Value not supported}.
    \value DataVoid
            The service or the function (property) is supported, but the requested
            data is not valid for this receiver. This value shall also be given
            if the requested data contains an enumeration value that is not
            supported, within the supported ranges.
    \value TemporarilyNotAvailable
            The data could be written, but it is not possible at the time,
            because another management client (MaC) is accessing the data or
            the data is currently processed by a management server (MaS)
            (for example, being flashed or being renewed).
    \value AccessWriteOnly
            Read access was attempted to a \e {write only} service or resource.
            This means resources such as properties, function properties, or
            memory that can be written, but that cannot be read.
    \value AccessReadOnly
            Write access was attempted to a \e {read only} service or resource.
            This means resources such as properties, function properties, or
            memory that can be read, but that cannot be written.
    \value AccessDenied
            The access to the data or function was denied because of
            authorization reasons, authorize request, or KNX security.
    \value AddressVoid
            The interface object or the property is not present, or the index
            is out of range.
    \value DataTypeConflict
            Write access with a wrong data type (datapoint length).
    \value Error
            The service, function, or command has failed without additional
            information about the problem.
*/

QT_END_NAMESPACE
