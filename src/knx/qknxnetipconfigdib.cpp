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

#include "qknxnetipconfigdib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

QKnxNetIpConfigDib::QKnxNetIpConfigDib(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpConfigDib::QKnxNetIpConfigDib(const QNetworkAddressEntry &addressEntry,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods)
    : QKnxNetIpConfigDib(addressEntry.ip(), addressEntry.netmask(), gateway, caps, methods)
{}

QKnxNetIpConfigDib::QKnxNetIpConfigDib(const QHostAddress &ip, const QHostAddress &subnetMask,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::IpConfiguration)
{
    // Capabilities are limited and at least one assignment method shall be enabled.
    if (caps > 0x07 || (methods < 0x01 || methods > 0x15))
        return;

    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::HostAddress::bytes(ip));
    payload.appendBytes(QKnxUtils::HostAddress::bytes(subnetMask));
    payload.appendBytes(QKnxUtils::HostAddress::bytes(gateway));
    payload.setByte(12, quint8(caps));
    payload.setByte(13, quint8(methods));
    setPayload(payload);
}

QKnxNetIp::DescriptionType QKnxNetIpConfigDib::descriptionType() const
{
    return code();
}

QHostAddress QKnxNetIpConfigDib::ipAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef());
}

QHostAddress QKnxNetIpConfigDib::subnetMask() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef(), 4);
}

QHostAddress QKnxNetIpConfigDib::defaultGateway() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef(), 8);
}

QKnxNetIpConfigDib::Capabilities QKnxNetIpConfigDib::capabilities() const
{
    return QKnxNetIpConfigDib::Capabilities(payloadRef().byte(12));
}

QKnxNetIpConfigDib::AssignmentMethods QKnxNetIpConfigDib::assignmentMethods() const
{
    return QKnxNetIpConfigDib::AssignmentMethods(payloadRef().byte(13));
}

bool QKnxNetIpConfigDib::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && size() == 16
        && descriptionType() == QKnxNetIp::DescriptionType::IpConfiguration;
}

QT_END_NAMESPACE
