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

#include "qknxnetipdevicedib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDeviceDib

    \inmodule QtKnx
    \brief The QKnxNetIpDeviceDib class represents a device description
    information block.
*/

QKnxNetIpDeviceDib::QKnxNetIpDeviceDib(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpDeviceDib::QKnxNetIpDeviceDib(QKnx::MediumType mediumType, DeviceStatus deviceStatus,
        const QKnxAddress &individualAddress, quint16 projectId, const QKnxByteArray &serialNumber,
        const QHostAddress &multicastAddress, const QKnxByteArray &macAddress, const QKnxByteArray &deviceName)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::DeviceInfo)
{
    QKnxByteArray data { quint8(mediumType) };
    if (deviceStatus > DeviceStatus::ActiveProgrammingMode)
        return;

    data.append(quint8(deviceStatus));
    data.append(individualAddress.bytes());
    data.append(QKnxUtils::QUint16::bytes(projectId));

    if (serialNumber.size() != 6)
        return;
    data.append(serialNumber);

    if (multicastAddress != QHostAddress::AnyIPv4 && !multicastAddress.isMulticast())
        return;
    data.append(QKnxUtils::HostAddress::bytes(multicastAddress));

    if (macAddress.size() != 6)
        return;
    data.append(macAddress);
    data.append(deviceName);

    if (data.size() < 52)
        data.append(QKnxByteArray(52 - data.size(), 0));
    data.resize(52); // size enforced by 7.5.4.2 Device information DIB

    setData(data);
}

QKnxNetIp::DescriptionType QKnxNetIpDeviceDib::descriptionType() const
{
    return code();
}

QKnx::MediumType QKnxNetIpDeviceDib::mediumType() const
{
    return QKnx::MediumType(constData().value(0));
}

QKnxNetIpDeviceDib::DeviceStatus QKnxNetIpDeviceDib::deviceStatus() const
{
    return QKnxNetIpDeviceDib::DeviceStatus(constData().value(1));
}

QKnxAddress QKnxNetIpDeviceDib::individualAddress() const
{
    return { QKnxAddress::Type::Individual, constData().mid(2, 2) };
}

quint16 QKnxNetIpDeviceDib::projectInstallationIdentfier() const
{
    return QKnxUtils::QUint16::fromBytes(constData(), 4);
}

QKnxByteArray QKnxNetIpDeviceDib::serialNumber() const
{
    return constData().mid(6, 6);
}

QHostAddress QKnxNetIpDeviceDib::multicastAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(constData(), 12);
}

QKnxByteArray QKnxNetIpDeviceDib::macAddress() const
{
    return constData().mid(16, 6);
}

QKnxByteArray QKnxNetIpDeviceDib::deviceName() const
{
    auto tmp = constData().mid(22, 30);
    return QKnxByteArray::fromByteArray((const char *) tmp.constData());
}

bool QKnxNetIpDeviceDib::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && size() == 54
        && descriptionType() == QKnxNetIp::DescriptionType::DeviceInfo;
}

QT_END_NAMESPACE
