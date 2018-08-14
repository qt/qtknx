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

#ifndef QKNXINTERFACEOBJECTTYPE_H
#define QKNXINTERFACEOBJECTTYPE_H

#include <QtCore/qobject.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxinterfaceobjectproperty.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxInterfaceObjectType final
{
    Q_GADGET

public:
    static const constexpr int Invalid = 0x10000;

    enum System : quint16
    {
        Device = 0x0000,
        GroupAddressTable = 0x0001,
        AssociationTable = 0x0002,
        ApplicationProgram = 0x0003,
        InterfaceProgram = 0x0004,
        KnxObjectAssociationTable = 0x0005,
        Router = 0x0006,
        LteAddressRoutingTable = 0x0007,
        CemiServer = 0x0008,
        GroupObjectTable = 0x0009,
        PollingMaster = 0x000a,
        KnxNetIpParameter = 0x000b,
        Reserved = 0x000c,
        FileServer = 0x000d,
        Security = 0x0011,
        RfMedium = 0x0013
    };
    Q_ENUM(System)

    enum Application : quint16
    {
        Hvac = 0x64,
        Lightning = 0x190,
        IndoorBrightnessSensor = 0x199,
        IndoorLuminanceSensor = 0x19a,
        LightSwitchingActuatorBasic = 0x1a1,
        DimmingActuatorBasic = 0x1a2,
        DimmingSensorBasic = 0x1a4,
        SwitchingSensorBasic = 0x1a5,
        SensorsAndActuators = 0x258,
        ShuttersAndBlinds = 0x320,
        FociS = 0x3e8,
        Metering = 0x44d,
        OpenTherm = 0x4b0,
        ApplicationReserved = 0x4e2,
        WhiteGoods = 0xdac,
        ApplicationReserved2 = 0xed8,
        WhiteGoods2 = 0xfa0
    };
    Q_ENUM(Application)

    enum NonStandardized : quint16
    {
        First = 0xc351,
        Last = 0xffff
    };
    Q_ENUM(NonStandardized)

    QKnxInterfaceObjectType(int objectType);
    QKnxInterfaceObjectType &operator=(int objectType);

    operator int() const { return m_objectType; }

    static bool isObjectType(QKnxInterfaceObjectType type);
    static bool isMatch(QKnxInterfaceObjectType type, QKnxInterfaceObjectProperty property);

private:
    int m_objectType = Invalid;
};

Q_DECLARE_TYPEINFO(QKnxInterfaceObjectType::System, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectType::Application, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxInterfaceObjectType::NonStandardized, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
