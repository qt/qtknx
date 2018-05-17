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
    \enum QKnx::MediumType

    This enum describes the various medium types supported by KNX bus.

    \value Unknown      The supported medium type is unknown.
    \value TP           The supported medium type is twisted pair 1.
    \value PL           The supported medium type is power-line 110.
    \value RF           The supported medium type is radio frequency.
    \value NetIP        The supported medium type is KNXnet/IP.
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

QT_END_NAMESPACE
