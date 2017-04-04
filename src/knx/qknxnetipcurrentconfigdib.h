/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPCURRENTCONFIGDIB_H
#define QKNXNETIPCURRENTCONFIGDIB_H

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

class Q_KNX_EXPORT QKnxNetIpCurrentConfigDIB final : private QKnxNetIpStruct
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
    ~QKnxNetIpCurrentConfigDIB() override = default;

    QKnxNetIpCurrentConfigDIB(const QHostAddress &ipAddress,
                              const QHostAddress &subnetMask,
                              const QHostAddress &gateway,
                              const QHostAddress &dhcp,
                              AssignmentMethod method);

    QKnxNetIpCurrentConfigDIB(const QNetworkAddressEntry &addressEntry,
                              const QHostAddress &gateway,
                              const QHostAddress &dhcp,
                              AssignmentMethod method);

    template <typename T> static QKnxNetIpCurrentConfigDIB fromBytes(const T &bytes, qint32 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

    QKnxNetIp::DescriptionTypeCode descriptionTypeCode() const;
    QHostAddress ipAddress() const;
    QHostAddress subnetMask() const;
    QHostAddress defaultGateway() const;
    QHostAddress dhcpOrBootP() const;
    AssignmentMethod assignmentMethod() const;

    bool isValid() const override;

    using QKnxNetIpStruct::size;
    using QKnxNetIpStruct::bytes;
    using QKnxNetIpStruct::payload;
    using QKnxNetIpStruct::toString;

private:
    QKnxNetIpCurrentConfigDIB(const QKnxNetIpStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpCurrentConfigDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
