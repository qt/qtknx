/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtCore/qbytearray.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstructure.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDeviceDIB final : public QKnxNetIpStructure
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

    QKnxNetIpDeviceDIB(MediumCode mediumCode,
                       quint8 deviceStatus,
                       const QKnxAddress &address,
                       quint16 projectId,
                       const QByteArray &serialNumber,
                       const QHostAddress &multicastAddress,
                       const QByteArray &macAddress,
                       const QByteArray deviceName);

    explicit QKnxNetIpDeviceDIB(const QByteArray &data);
    explicit QKnxNetIpDeviceDIB(const QVector<quint8> &data);

    QKnxNetIpDeviceDIB fromRawData(const QByteArray &rawData, qint32 offset);
    QKnxNetIpDeviceDIB fromRawData(const QVector<quint8> &rawData, qint32 offset);

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
Q_DECLARE_TYPEINFO(QKnxNetIpDeviceDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
