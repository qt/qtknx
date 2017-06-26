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
    \class QKnxNetIpDeviceDib

    \inmodule QtKnx
    \brief The QKnxNetIpDeviceDib class represents the structure containing the
    information that is necessary to serve as a device description information
    block.
*/

QKnxNetIpDeviceDib::QKnxNetIpDeviceDib(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpDeviceDib::QKnxNetIpDeviceDib(Medium mediumCode, DeviceStatus deviceStatus,
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

QKnxNetIp::DescriptionType QKnxNetIpDeviceDib::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

QKnxNetIpDeviceDib::Medium QKnxNetIpDeviceDib::medium() const
{
    return QKnxNetIpDeviceDib::Medium(payloadRef().byte(0));
}

QKnxNetIpDeviceDib::DeviceStatus QKnxNetIpDeviceDib::deviceStatus() const
{
    return QKnxNetIpDeviceDib::DeviceStatus(payloadRef().byte(1));
}

QKnxAddress QKnxNetIpDeviceDib::individualAddress() const
{
    return { QKnxAddress::Type::Individual, payloadRef().bytes<QByteArray>(2, 2) };
}

quint16 QKnxNetIpDeviceDib::projectInstallationIdentfier() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef(), 4);
}

QByteArray QKnxNetIpDeviceDib::serialNumber() const
{
    return payloadRef().bytes<QByteArray>(6, 6);
}

QHostAddress QKnxNetIpDeviceDib::multicastAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef(), 12);
}

QByteArray QKnxNetIpDeviceDib::macAddress() const
{
    return payloadRef().bytes<QByteArray>(16, 6);
}

QByteArray QKnxNetIpDeviceDib::deviceName() const
{
    return QByteArray(payloadRef().bytes<QByteArray>(22, 30).constData());
}

bool QKnxNetIpDeviceDib::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && size() == 54
        && descriptionType() == QKnxNetIp::DescriptionType::DeviceInfo;
}

QT_END_NAMESPACE
