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

#ifndef QKNXNETIPDEVICEDIB_H
#define QKNXNETIPDEVICEDIB_H

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

class Q_KNX_EXPORT QKnxNetIpDeviceDIB final : public QKnxNetIpDescriptionTypeStruct
{
public:
    enum class Medium : quint8
    {
        Tp1 = 0x02,
        Pl110 = 0x04,
        Rf = 0x10,
        Ip = 0x20
    };
    QKnxNetIpDeviceDIB::Medium medium() const;

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

    QKnxNetIpDeviceDIB(Medium mediumCode,
                       DeviceStatus deviceStatus,
                       const QKnxAddress &address,
                       quint16 projectId,
                       const QByteArray &serialNumber,
                       const QHostAddress &multicastAddress,
                       const QByteArray &macAddress,
                       const QByteArray deviceName);

    template <typename T> static QKnxNetIpDeviceDIB fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpStructHelper::fromBytes(bytes, index,
            QKnxNetIp::DescriptionType::DeviceInfo);
    }

    QKnxNetIp::DescriptionType descriptionType() const;
    QKnxAddress individualAddress() const;
    quint16 projectInstallationIdentfier() const;
    QByteArray serialNumber() const;
    QHostAddress multicastAddress() const;
    QByteArray macAddress() const;
    QByteArray deviceName() const;

    bool isValid() const override;

private:
    QKnxNetIpDeviceDIB(const QKnxNetIpDescriptionTypeStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpDeviceDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
