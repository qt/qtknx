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

#include "qknxinterfaceobjecttype.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxInterfaceObjectType

    \inmodule QtKnx
    \brief This class holds information about the type of the KNX interface object.

    KNX interface objects hold information about the device functionalities.
    The interface object can be of different types, covering different
    functionalities. The different interface object type are listed here.
    For every interface object, this type is always stored in the
    \l QKnxInterfaceObjectProperty::ObjectType. This is the first property of
    the interface object.
*/

/*!
    \enum QKnxInterfaceObjectType::System
    This enum describes the types of interface object dedicated to system.

    \value Device
    \value GroupAddressTable
    \value AssociationTable
    \value ApplicationProgram
    \value InterfaceProgram
    \value KnxObjectAssociationTable
    \value Router
    \value LteAddressRoutingTable
    \value CemiServer
    \value GroupObjectTable
    \value PollingMaster
    \value KnxNetIpParameter
    \value Reserved
    \value FileServer
    \value Security
    \value RfMedium
*/

/*!
    \enum QKnxInterfaceObjectType::Application
    This enum describes the ranges of types of interface object dedicated to
    application.

    \value Hvac
    \value Lightning
    \value IndoorBrightnessSensor
    \value IndoorLuminanceSensor
    \value LightSwitchingActuatorBasic
    \value DimmingActuatorBasic
    \value DimmingSensorBasic
    \value SwitchingSensorBasic
    \value SensorsAndActuators
    \value ShuttersAndBlinds
    \value FociS
    \value Metering
    \value OpenTherm
    \value ApplicationReserved
    \value WhiteGoods
    \value ApplicationReserved2
    \value WhiteGoods2
*/

/*!
    \enum QKnxInterfaceObjectType::NonStandardized
    This enum describes the types range of non standard interface objects.

    \value First
    \value Last
*/

QKnxInterfaceObjectType::QKnxInterfaceObjectType(int objectType)
    : m_objectType(objectType)
{}

QKnxInterfaceObjectType &QKnxInterfaceObjectType::operator=(int objectType)
{
    m_objectType = objectType;
    return *this;
}

/*!
    Returns true if the \a type is of type \l System, \l Application or
    l\ NonStandardized; false otherwise.

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
    Returns true if the given \a property can be used in conjunction with the
    given \a type; false otherwise.

    \note The function performs a check on the object type first and if it is
    not a valid object type, it will return false.

    \note Only system types are fully matched, for all other valid object
    types the function returns true if the property can be interpreted as
    a known or unknown property (depends on the property value).
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
