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
#include <QtNetwork/qnetworkinterface.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpCurrentConfigDIB final : public QKnxNetIpStructure
{
public:
    // 03_08_03 Management v01.06.02 AS, 2.5.5 PID_CURRENT_IP_ASSIGNMENT_METHOD (PID = 54)
    enum class AssignmentMethod : quint8
    {
        Manual = 0x01,
        BootP = 0x02,
        Dhcp = 0x04,
        AutoIp = 0x08
    };

    QKnxNetIpCurrentConfigDIB() = default;

    QKnxNetIpCurrentConfigDIB(const QNetworkAddressEntry &addressAndSubnetMask,
        const QHostAddress &gateway, const QHostAddress &dhcp, AssignmentMethod method);
    QKnxNetIpCurrentConfigDIB(const QHostAddress &ipAddress, const QHostAddress &subnetMask,
        const QHostAddress &gateway, const QHostAddress &dhcp, AssignmentMethod method);


    explicit QKnxNetIpCurrentConfigDIB(const QByteArray &data);
    explicit QKnxNetIpCurrentConfigDIB(const QVector<quint8> &data);

    QKnxNetIpCurrentConfigDIB fromRawData(const QByteArray &rawData, qint32 offset);
    QKnxNetIpCurrentConfigDIB fromRawData(const QVector<quint8> &rawData, qint32 offset);

    QHostAddress ipAddress() const;
    QHostAddress subnetMask() const;
    QHostAddress defaultGateway() const;
    QHostAddress dhcpOrBootP() const;

    AssignmentMethod assignmentMethod() const;

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::descriptionTypeCode;
};

Q_DECLARE_TYPEINFO(QKnxNetIpCurrentConfigDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
