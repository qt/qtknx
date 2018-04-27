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

#include "qknxinterfaceobjecttype.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxInterfaceObjectType

    \inmodule QtKnx
    \brief The QKnxInterfaceObjectType class holds the type of a KNX interface
    object.

    A KNX interface object holds information about device functionality.
    Each interface object type covers particular functionality. This class
    represents the \l System, \l Application, and \l NonStandardized object
    types.

    The type of an interface object is stored as its first property in
    \l QKnxInterfaceObjectProperty::ObjectType.
*/

/*!
    \enum QKnxInterfaceObjectType::System
    This enum describes the types of interface objects dedicated to device
    system functionality.

    \value Device
           Device interface object
    \value GroupAddressTable
           Group address table interface object
    \value AssociationTable
           Association table interface object
    \value ApplicationProgram
           Application program interface object
    \value InterfaceProgram
           Interface program interface object
    \value KnxObjectAssociationTable
           KNX object assiocation table interface object
    \value Router
           Router interface object
    \value LteAddressRoutingTable
           Logical Tag Extended (LTE) address routing table interface object
    \value CemiServer
           Common External Message Interface (cEMI) server interface object
    \value GroupObjectTable
           Group object table interface object
    \value PollingMaster
           Polling master interface object
    \value KnxNetIpParameter
           KNXnet/IP parameter interface object
    \value Reserved
           Reserved
    \value FileServer
           File server interface object
    \value Security
           Security interface object
    \value RfMedium
           Radio frequency (RF) medium interface object
*/

/*!
    \enum QKnxInterfaceObjectType::Application
    This enum describes the type ranges of interface objects dedicated to
    an application area.

    \value Hvac
           Heating, ventilation, and air conditioning (HVAC)
    \value Lightning
           Lightning
    \value IndoorBrightnessSensor
           Indoor brightness sensors
    \value IndoorLuminanceSensor
           Indoor luminance sensors
    \value LightSwitchingActuatorBasic
           Basic light switching actuators
    \value DimmingActuatorBasic
           Basic dimming actuators
    \value DimmingSensorBasic
           Basic dimming sensors
    \value SwitchingSensorBasic
           Basic switching sensors
    \value SensorsAndActuators
           Sensors and actuators
    \value ShuttersAndBlinds
           Shutters and blinds
    \value FociS
           Functions of common interest, such as date and time
    \value Metering
           Metering the consumption of energy, water, gas, and so on
    \value OpenTherm
           The OpenTherm gateway, which connects KNX to the OT Bus used for
           heating systems, such as gas condensing boilers
    \value ApplicationReserved
           Reserved
    \value WhiteGoods
           White goods
    \value ApplicationReserved2
           Reserved
    \value WhiteGoods2
           White goods
*/

/*!
    \enum QKnxInterfaceObjectType::NonStandardized
    This enum describes the type ranges of non-standard interface objects.

    \value First
           First object
    \value Last
           Last object
*/

/*!
    \fn QKnxInterfaceObjectType::QKnxInterfaceObjectType(int objectType)

    Creates a KNX interface object type of the type \a objectType.
*/

/*!
    \fn QKnxInterfaceObjectType::operator int() const

    Returns the value stored in the KNX interface object type as an integer.
*/

QKnxInterfaceObjectType::QKnxInterfaceObjectType(int objectType)
    : m_objectType(objectType)
{}

/*!
    Assigns \a objectType to this KNX interface object type.
*/
QKnxInterfaceObjectType &QKnxInterfaceObjectType::operator=(int objectType)
{
    m_objectType = objectType;
    return *this;
}

/*!
    Returns \c true if \a type is a value of \l System, \l Application, or
    \l NonStandardized; \c false otherwise.

    \note Reserved values are considered invalid object types.
*/
bool QKnxInterfaceObjectType::isObjectType(QKnxInterfaceObjectType type)
{
    if (type == System::Reserved)
        return false;

    if (type >= Application::ApplicationReserved && type < Application::WhiteGoods)
        return false;

    if (type >= Application::ApplicationReserved2 && type < Application::WhiteGoods2)
        return false;

    return type < QKnxInterfaceObjectType::Invalid;
}

/*!
    Returns \c true if the given \a property can be used in conjunction with the
    given \a type; \c false otherwise.

    \note The function performs a check on the object type and returns \c false
    if the object type is invalid.

    \note Only system types are fully matched. For all other valid object
    types the function returns \c true if the property can be interpreted as
    a known or unknown property (depending on the property value).
*/
bool QKnxInterfaceObjectType::isMatch(QKnxInterfaceObjectType type, QKnxInterfaceObjectProperty property)
{
    if (!isObjectType(type))
        return false;

    switch (type) {
    case System::Device:
        return QKnxInterfaceObjectProperty::isDeviceProperty(property);
    case System::GroupAddressTable:
        return QKnxInterfaceObjectProperty::isGroupAddressTableProperty(property);
    case System::AssociationTable:
        return QKnxInterfaceObjectProperty::isAssociationTableProperty(property);
    case System::ApplicationProgram:
        return QKnxInterfaceObjectProperty::isApplicationProgramProperty(property);
    case System::InterfaceProgram:
        return QKnxInterfaceObjectProperty::isInterfaceProgramProperty(property);
    case System::KnxObjectAssociationTable:
        return QKnxInterfaceObjectProperty::isKnxObjectAssociationTableProperty(property);
    case System::Router:
        return QKnxInterfaceObjectProperty::isRouterProperty(property);
    case System::LteAddressRoutingTable:
        return QKnxInterfaceObjectProperty::isLteAddressRoutingTableProperty(property);
    case System::CemiServer:
        return QKnxInterfaceObjectProperty::isCemiServerProperty(property);
    case System::GroupObjectTable:
        return QKnxInterfaceObjectProperty::isGroupObjectTableProperty(property);
    case System::PollingMaster:
        return QKnxInterfaceObjectProperty::isPollingMasterProperty(property);
    case System::KnxNetIpParameter:
        return QKnxInterfaceObjectProperty::isKnxNetIpParameterProperty(property);
    case System::FileServer:
        return QKnxInterfaceObjectProperty::isFileServerProperty(property);
    case System::Security:
        return QKnxInterfaceObjectProperty::isSecurityProperty(property);
    case System::RfMedium:
        return QKnxInterfaceObjectProperty::isRfMediumProperty(property);
    default:
        break;
    }
    return QKnxInterfaceObjectProperty::isProperty(property);
}

QT_END_NAMESPACE
