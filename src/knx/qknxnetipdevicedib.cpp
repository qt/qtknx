/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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

QKnxNetIpDeviceDIB::QKnxNetIpDeviceDIB(const QKnxNetIpStruct &other)
    : QKnxNetIpStruct(other)
{}

QKnxNetIpDeviceDIB::QKnxNetIpDeviceDIB(MediumCode mediumCode, DeviceStatus deviceStatus,
        const QKnxAddress &individualAddress, quint16 projectId, const QByteArray &serialNumber,
        const QHostAddress &multicastAddress, const QByteArray &macAddress, const QByteArray deviceName)
    : QKnxNetIpStruct(quint8(QKnxNetIp::DescriptionTypeCode::DeviceInfo))
{
    QKnxNetIpPayload payload;
    payload.setByte(0, quint8(mediumCode));

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

QKnxNetIp::DescriptionTypeCode QKnxNetIpDeviceDIB::descriptionTypeCode() const
{
    return QKnxNetIp::DescriptionTypeCode(code());
}

QKnxNetIpDeviceDIB::MediumCode QKnxNetIpDeviceDIB::mediumCode() const
{
    return QKnxNetIpDeviceDIB::MediumCode(payload().bytes(0, 1)[0]);
}

QKnxNetIpDeviceDIB::DeviceStatus QKnxNetIpDeviceDIB::deviceStatus() const
{
    return QKnxNetIpDeviceDIB::DeviceStatus(payload().bytes(1, 1)[0]);
}

QKnxAddress QKnxNetIpDeviceDIB::individualAddress() const
{
    return { QKnxAddress::Type::Individual, payload().bytes<QByteArray>(2, 2) };
}

quint16 QKnxNetIpDeviceDIB::projectInstallationIdentfier() const
{
    return QKnxUtils::QUint16::fromBytes(payload().bytes(4, 2));
}

QByteArray QKnxNetIpDeviceDIB::serialNumber() const
{
    return payload().bytes<QByteArray>(6, 6);
}

QHostAddress QKnxNetIpDeviceDIB::multicastAddress() const
{
    return QKnxUtils::HostAddress::fromBytes(payload().bytes(12, 4));
}

QByteArray QKnxNetIpDeviceDIB::macAddress() const
{
    return payload().bytes<QByteArray>(16, 6);
}

QByteArray QKnxNetIpDeviceDIB::deviceName() const
{
    return QByteArray(payload().bytes<QByteArray>(22, 30).constData());
}

bool QKnxNetIpDeviceDIB::isValid() const
{
    return QKnxNetIpStruct::isValid() && size() == 54
        && descriptionTypeCode() == QKnxNetIp::DescriptionTypeCode::DeviceInfo;
}

QT_END_NAMESPACE
