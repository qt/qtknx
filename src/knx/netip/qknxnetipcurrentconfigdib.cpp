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

#include "qknxnetipcurrentconfigdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCurrentConfigDib::QKnxNetIpCurrentConfigDib(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpCurrentConfigDib::QKnxNetIpCurrentConfigDib(const QNetworkAddressEntry &addressEntry,
        const QHostAddress &gateway, const QHostAddress &dhcp, AssignmentMethod method)
    : QKnxNetIpCurrentConfigDib(addressEntry.ip(), addressEntry.netmask(), gateway, dhcp, method)
{}

QKnxNetIpCurrentConfigDib::QKnxNetIpCurrentConfigDib(const QHostAddress &ip,
    const QHostAddress &subnetMask, const QHostAddress &gateway, const QHostAddress &dhcp,
        AssignmentMethod method)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::CurrentIpConfiguration)
{
    size_t bitcount = std::bitset<8>(quint8(method)).count();
    if (method < AssignmentMethod::Manual || method > AssignmentMethod::AutoIp || bitcount != 1)
        return; // Only one assignment method shall be enabled from within the possible range.

    QKnxByteArray data;
    data.append(QKnxUtils::HostAddress::bytes(ip));
    data.append(QKnxUtils::HostAddress::bytes(subnetMask));
    data.append(QKnxUtils::HostAddress::bytes(gateway));
    data.append(QKnxUtils::HostAddress::bytes(dhcp));
    data.append(quint8(method));
    data.append(quint8(0));
    setData(data);
}

QKnxNetIp::DescriptionType QKnxNetIpCurrentConfigDib::descriptionType() const
{
    return code();
}

QHostAddress QKnxNetIpCurrentConfigDib::ipAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(constData());
}

QHostAddress QKnxNetIpCurrentConfigDib::subnetMask() const
{
    return QKnxUtils::HostAddress::fromBytes(constData(), 4);
}

QHostAddress QKnxNetIpCurrentConfigDib::defaultGateway() const
{
    return QKnxUtils::HostAddress::fromBytes(constData(), 8);
}

QHostAddress QKnxNetIpCurrentConfigDib::dhcpOrBootP() const
{
    return QKnxUtils::HostAddress::fromBytes(constData(), 12);
}

QKnxNetIpCurrentConfigDib::AssignmentMethod QKnxNetIpCurrentConfigDib::assignmentMethod() const
{
    return QKnxNetIpCurrentConfigDib::AssignmentMethod(constData().value(16));
}

bool QKnxNetIpCurrentConfigDib::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && size() == 20
        && descriptionType() == QKnxNetIp::DescriptionType::CurrentIpConfiguration;
}

QT_END_NAMESPACE
