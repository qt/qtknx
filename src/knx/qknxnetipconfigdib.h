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

class Q_KNX_EXPORT QKnxNetIpConfigDIB final : public QKnxNetIpStructure
{
public:
    // 03_08_03 Management v01.06.02 AS, 2.5.7 PID_IP_CAPABILITIES (PID = 56)
    enum class Capability : quint8
    {
        BootP = 0x00,
        Dhcp = 0x01,
        AutoIp = 0x02
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)

    // 03_08_03 Management v01.06.02 AS, 2.5.6 PID_IP_ASSIGNMENT_METHOD (PID = 55)
    enum class AssignmentMethod : quint8
    {
        Manual = 0x01,
        BootP = 0x02,
        Dhcp = 0x04,
        AutoIp = 0x08
    };
    Q_DECLARE_FLAGS(AssignmentMethods, AssignmentMethod)

    QKnxNetIpConfigDIB() = default;

    QKnxNetIpConfigDIB(const QNetworkAddressEntry &addressAndSubnetMask,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods);
    QKnxNetIpConfigDIB(const QHostAddress &ipAddress, const QHostAddress &subnetMask,
        const QHostAddress &gateway, Capabilities caps, AssignmentMethods methods);

    explicit QKnxNetIpConfigDIB(const QByteArray &data);
    explicit QKnxNetIpConfigDIB(const QVector<quint8> &data);

    QKnxNetIpConfigDIB fromRawData(const QByteArray &rawData, qint32 offset);
    QKnxNetIpConfigDIB fromRawData(const QVector<quint8> &rawData, qint32 offset);

    QHostAddress ipAddress() const;
    QHostAddress subnetMask() const;
    QHostAddress defaultGateway() const;

    Capabilities capabilities() const;
    AssignmentMethods assignmentMethods() const;

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::descriptionTypeCode;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxNetIpConfigDIB::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxNetIpConfigDIB::AssignmentMethods)

Q_DECLARE_TYPEINFO(QKnxNetIpConfigDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
