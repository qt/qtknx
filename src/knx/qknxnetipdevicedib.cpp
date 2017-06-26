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

#include "qknxnetipdevicedib.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDeviceDIB

    \inmodule QtKnx
    \brief The QKnxNetIpDeviceDIB class represents the structure containing the
    information that is necessary to serve as a device description information
    block.
*/

QKnxNetIpDeviceDIB::QKnxNetIpDeviceDIB(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpDeviceDIB::QKnxNetIpDeviceDIB(Medium mediumCode, DeviceStatus deviceStatus,
        const QKnxAddress &individualAddress, quint16 projectId, const QByteArray &serialNumber,
        const QHostAddress &multicastAddress, const QByteArray &macAddress, const QByteArray deviceName)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::DeviceInfo)
{
    QKnxNetIpPayload payload((quint8) mediumCode);

    if (deviceStatus > DeviceStatus::ActiveProgrammingMode)
        return;
    payload.setByte(1, quint8(deviceStatus));

    payload.appendBytes(individualAddress.rawData<std::vector<quint8>>());
    payload.appendBytes(QKnxUtils::QUint16::bytes(projectId));

    if (serialNumber.size() != 6)
        return;
    payload.appendBytes(serialNumber);

    if (multicastAddress != QHostAddress::AnyIPv4 && !multicastAddress.isMulticast())
        return;
    payload.appendBytes(QKnxUtils::HostAddress::bytes(multicastAddress));

    if (macAddress.size() != 6)
        return;
    payload.appendBytes(macAddress);
    payload.appendBytes(deviceName);
    payload.resize(52); // size enforced by 7.5.4.2 Device information DIB
    setPayload(payload);
}

QKnxNetIp::DescriptionType QKnxNetIpDeviceDIB::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

QKnxNetIpDeviceDIB::Medium QKnxNetIpDeviceDIB::medium() const
{
    return QKnxNetIpDeviceDIB::Medium(payloadRef().byte(0));
}

QKnxNetIpDeviceDIB::DeviceStatus QKnxNetIpDeviceDIB::deviceStatus() const
{
    return QKnxNetIpDeviceDIB::DeviceStatus(payloadRef().byte(1));
}

QKnxAddress QKnxNetIpDeviceDIB::individualAddress() const
{
    return { QKnxAddress::Type::Individual, payloadRef().bytes<QByteArray>(2, 2) };
}

quint16 QKnxNetIpDeviceDIB::projectInstallationIdentfier() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef(), 4);
}

QByteArray QKnxNetIpDeviceDIB::serialNumber() const
{
    return payloadRef().bytes<QByteArray>(6, 6);
}

QHostAddress QKnxNetIpDeviceDIB::multicastAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef(), 12);
}

QByteArray QKnxNetIpDeviceDIB::macAddress() const
{
    return payloadRef().bytes<QByteArray>(16, 6);
}

QByteArray QKnxNetIpDeviceDIB::deviceName() const
{
    return QByteArray(payloadRef().bytes<QByteArray>(22, 30).constData());
}

bool QKnxNetIpDeviceDIB::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && size() == 54
        && descriptionType() == QKnxNetIp::DescriptionType::DeviceInfo;
}

QT_END_NAMESPACE
