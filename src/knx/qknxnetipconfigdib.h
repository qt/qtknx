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
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qnetworkinterface.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConfigDIB final : private QKnxNetIpStruct
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

    QKnxNetIpConfigDIB(const QHostAddress &ipAddress,
                       const QHostAddress &subnetMask,
                       const QHostAddress &gateway,
                       Capabilities capabilitys,
                       AssignmentMethods methods);

    QKnxNetIpConfigDIB(const QNetworkAddressEntry &addressEntry,
                       const QHostAddress &gateway,
                       Capabilities capabilitys,
                       AssignmentMethods methods);

    template<typename T> static QKnxNetIpConfigDIB fromBytes(const T &bytes, qint32 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

    QKnxNetIp::DescriptionTypeCode descriptionTypeCode() const;
    QHostAddress ipAddress() const;
    QHostAddress subnetMask() const;
    QHostAddress defaultGateway() const;
    Capabilities capabilities() const;
    AssignmentMethods assignmentMethods() const;

    bool isValid() const override;

    using QKnxNetIpStruct::size;
    using QKnxNetIpStruct::bytes;
    using QKnxNetIpStruct::payload;
    using QKnxNetIpStruct::toString;

private:
    QKnxNetIpConfigDIB(const QKnxNetIpStruct &other);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxNetIpConfigDIB::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxNetIpConfigDIB::AssignmentMethods)

Q_DECLARE_TYPEINFO(QKnxNetIpConfigDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
