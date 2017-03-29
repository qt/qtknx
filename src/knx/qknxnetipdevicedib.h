/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDeviceDIB final : private QKnxNetIpStruct
{
public:
    enum class MediumCode : quint8
    {
        Tp1 = 0x02,
        Pl110 = 0x04,
        Rf = 0x10,
        Ip = 0x20
    };
    QKnxNetIpDeviceDIB::MediumCode mediumCode() const;

    // 4.20 Programming Mode (prog_mode)
    // referred to by 7.5.4.2 Device information DIB device status
    enum class DeviceStatus : quint8
    {
        InactiveProgrammingMode = 0x00,
        ActiveProgrammingMode = 0x01
    };
    QKnxNetIpDeviceDIB::DeviceStatus deviceStatus() const;

    QKnxNetIpDeviceDIB() = default;
    ~QKnxNetIpDeviceDIB() override = default;

    QKnxNetIpDeviceDIB(MediumCode mediumCode,
                       DeviceStatus deviceStatus,
                       const QKnxAddress &address,
                       quint16 projectId,
                       const QByteArray &serialNumber,
                       const QHostAddress &multicastAddress,
                       const QByteArray &macAddress,
                       const QByteArray deviceName);

    template<typename T> static QKnxNetIpDeviceDIB fromBytes(const T &bytes, qint32 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

    QKnxNetIp::DescriptionTypeCode descriptionTypeCode() const;
    QKnxAddress individualAddress() const;
    quint16 projectInstallationIdentfier() const;
    QByteArray serialNumber() const;
    QHostAddress multicastAddress() const;
    QByteArray macAddress() const;
    QByteArray deviceName() const;

    bool isValid() const override;

    using QKnxNetIpStruct::size;
    using QKnxNetIpStruct::bytes;
    using QKnxNetIpStruct::payload;
    using QKnxNetIpStruct::toString;

private:
    QKnxNetIpDeviceDIB(const QKnxNetIpStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpDeviceDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
