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

#include "qknxinterfaceobjecttype.h"

QT_BEGIN_NAMESPACE

QKnxInterfaceObjectType::QKnxInterfaceObjectType(int objectType)
    : m_objectType(objectType)
{}

QKnxInterfaceObjectType &QKnxInterfaceObjectType::operator=(int objectType)
{
    m_objectType = objectType;
    return *this;
}

/*!
    Returns true if the \a objectType is of type \l System, \l Application or
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
    given \a objectType; false otherwise.

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
