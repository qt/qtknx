/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstructure.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDeviceDIB : public QKnxNetIpStructure
{
public:
    enum struct MediumCode : quint8
    {
        Tp1 = 0x02,
        Pl110 = 0x04,
        Rf = 0x10,
        Ip = 0x20
    };
    QKnxNetIpDeviceDIB::MediumCode mediumCode() const;

    QKnxNetIpDeviceDIB() = default;

    explicit QKnxNetIpDeviceDIB(const QByteArray &data)
        : QKnxNetIpStructure(quint8(DescriptionTypeCode::DeviceInfo), data)
    {
        resize(52, true); // size enforced by 7.5.4.2 Device information DIB
    }
    explicit QKnxNetIpDeviceDIB(const QVector<quint8> &data)
        : QKnxNetIpStructure(quint8(DescriptionTypeCode::DeviceInfo), data)
    {
        resize(52, true); // size enforced by 7.5.4.2 Device information DIB
    }
    QKnxNetIpDeviceDIB(const QByteArray &rawData, qint32 offset)
        : QKnxNetIpStructure(quint8(DescriptionTypeCode::DeviceInfo), rawData, offset)
    {
        resize(52, true); // size enforced by 7.5.4.2 Device information DIB
    }
    QKnxNetIpDeviceDIB(const QVector<quint8> &rawData, qint32 offset)
        : QKnxNetIpStructure(quint8(DescriptionTypeCode::DeviceInfo), rawData, offset)
    {
        resize(52, true); // size enforced by 7.5.4.2 Device information DIB
    }

    QKnxNetIpDeviceDIB(MediumCode mediumCode, quint8 deviceStatus, const QKnxAddress &address,
        quint16 projectId, const QByteArray &serialNumber, const QHostAddress &multicastAddress,
        const QByteArray &macAddress, const QByteArray deviceName);

    quint8 deviceStatus() const;
    QKnxAddress individualAddress() const;
    quint16 projectInstallationIdentfier() const;
    QByteArray serialNumber() const;
    QHostAddress multicastAddress() const;
    QByteArray macAddress() const;
    QByteArray deviceName() const;

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::descriptionTypeCode;
};

QT_END_NAMESPACE
