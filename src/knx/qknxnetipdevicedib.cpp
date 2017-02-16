/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdevicedib.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpDeviceDIB

    \inmodule QtKnx
    \brief The QKnxNetIpDeviceDIB class represents the structure containing the
    information that is necessary to serve as a device description information
    block.
*/

QKnxNetIpDeviceDIB::QKnxNetIpDeviceDIB(MediumCode mediumCode, quint8 deviceStatus,
        const QKnxAddress &individualAddress, quint16 projectId, const QByteArray &serialNumber,
        const QHostAddress &multicastAddress, const QByteArray &macAddress, const QByteArray deviceName)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::DeviceInfo), 52, true)
{
    QByteArray data;
    data[0] = quint8(mediumCode);
    if (deviceStatus > 0x01)
        return;
    data[1] = deviceStatus;
    data += individualAddress.rawData<QByteArray>();
    data[4] = quint8(projectId >> 8);
    data[5] = quint8(projectId);

    if (serialNumber.size() != 6)
        return;
    data += serialNumber;

    if (multicastAddress != QHostAddress::AnyIPv4 && !multicastAddress.isMulticast())
        return;

    auto address = multicastAddress.toIPv4Address();
    data[12] = quint8(address >> 24);
    data[13] = quint8(address >> 16);
    data[14] = quint8(address >> 8);
    data[15] = quint8(address);

    if (macAddress.size() != 6)
        return;
    setData(quint8(DescriptionTypeCode::DeviceInfo), QByteArray(data + macAddress + deviceName));

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

quint8 QKnxNetIpDeviceDIB::deviceStatus() const
{
    return data<QVector<quint8>>(1, 1)[0];
}

QKnxAddress QKnxNetIpDeviceDIB::individualAddress() const
{
    return { QKnxAddress::Type::Individual, data<QVector<quint8>>(2, 2) };
}

quint16 QKnxNetIpDeviceDIB::projectInstallationIdentfier() const
{
    auto tmp = data<QVector<quint8>>(4, 2);
    return quint16(tmp[0]) << 8 | tmp[1];
}

QByteArray QKnxNetIpDeviceDIB::serialNumber() const
{
    return data<QByteArray>(6, 6);
}

QHostAddress QKnxNetIpDeviceDIB::multicastAddress() const
{
    auto tmp = data<QVector<quint8>>(12, 4);
    return QHostAddress(quint32(tmp[0] << 24 | tmp[1] << 16 | tmp[2] << 8 | tmp[3]));
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
