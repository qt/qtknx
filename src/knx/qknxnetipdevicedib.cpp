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

QKnxNetIpDeviceDIB::QKnxNetIpDeviceDIB(MediumCode mediumCode, DeviceStatus deviceStatus,
        const QKnxAddress &individualAddress, quint16 projectId, const QByteArray &serialNumber,
        const QHostAddress &multicastAddress, const QByteArray &macAddress, const QByteArray deviceName)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::DeviceInfo), 52, true)
{
    QByteArray data(0, Qt::Uninitialized);
    data[0] = quint8(mediumCode);
    if (deviceStatus > DeviceStatus::ActiveProgrammingMode)
        return;
    data[1] = quint8(deviceStatus);
    data.insert(2, individualAddress.rawData<QByteArray>());
    data.insert(4, QKnxUtils::Integer::quint16ToArray<QByteArray>(projectId));

    if (serialNumber.size() != 6)
        return;
    data.insert(6, serialNumber);

    if (multicastAddress != QHostAddress::AnyIPv4 && !multicastAddress.isMulticast())
        return;
    data.insert(12, QKnxUtils::HostAddress::toArray<QByteArray>(multicastAddress));

    if (macAddress.size() != 6)
        return;
    data.insert(16, macAddress);
    data.insert(22, deviceName);
    setData(quint8(DescriptionTypeCode::DeviceInfo), data);

    resizeData(52, true); // size enforced by 7.5.4.2 Device information DIB
}

QKnxNetIpDeviceDIB::QKnxNetIpDeviceDIB(const QByteArray &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::DeviceInfo), data)
{
    resizeData(52, true); // size enforced by 7.5.4.2 Device information DIB
}

QKnxNetIpDeviceDIB::QKnxNetIpDeviceDIB(const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::DeviceInfo), data)
{
    resizeData(52, true); // size enforced by 7.5.4.2 Device information DIB
}

QKnxNetIpDeviceDIB QKnxNetIpDeviceDIB::fromRawData(const QByteArray &rawData, qint32 offset)
{
    QKnxNetIpDeviceDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::DeviceInfo), rawData, offset);
    dib.resizeData(52, true); // size enforced by 7.5.4.2 Device information DIB
    return dib;
}
QKnxNetIpDeviceDIB QKnxNetIpDeviceDIB::fromRawData(const QVector<quint8> &rawData, qint32 offset)
{
    QKnxNetIpDeviceDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::DeviceInfo), rawData, offset);
    dib.resizeData(52, true); // size enforced by 7.5.4.2 Device information DIB
    return dib;
}

QKnxNetIpDeviceDIB::MediumCode QKnxNetIpDeviceDIB::mediumCode() const
{
    return QKnxNetIpDeviceDIB::MediumCode(data<QVector<quint8>>(0, 1)[0]);
}

QKnxNetIpDeviceDIB::DeviceStatus QKnxNetIpDeviceDIB::deviceStatus() const
{
    return QKnxNetIpDeviceDIB::DeviceStatus(data<QVector<quint8>>(1, 1)[0]);
}

QKnxAddress QKnxNetIpDeviceDIB::individualAddress() const
{
    return { QKnxAddress::Type::Individual, data<QVector<quint8>>(2, 2) };
}

quint16 QKnxNetIpDeviceDIB::projectInstallationIdentfier() const
{
    return QKnxUtils::Integer::quint16FromArray(data<QVector<quint8>>(4, 2));
}

QByteArray QKnxNetIpDeviceDIB::serialNumber() const
{
    return data<QByteArray>(6, 6);
}

QHostAddress QKnxNetIpDeviceDIB::multicastAddress() const
{
    return QKnxUtils::HostAddress::fromArray(data<QVector<quint8>>(12, 4));
}

QByteArray QKnxNetIpDeviceDIB::macAddress() const
{
    return data<QByteArray>(16, 6);
}

QByteArray QKnxNetIpDeviceDIB::deviceName() const
{
    return QByteArray(data<QByteArray>(22, 30).constData());
}

QT_END_NAMESPACE
