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

class Q_KNX_EXPORT QKnxNetIpCurrentConfigDib final : public QKnxNetIpDescriptionTypeStruct
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

    QKnxNetIpCurrentConfigDib() = default;
    ~QKnxNetIpCurrentConfigDib() override = default;

    QKnxNetIpCurrentConfigDib(const QHostAddress &ipAddress,
                              const QHostAddress &subnetMask,
                              const QHostAddress &gateway,
                              const QHostAddress &dhcp,
                              AssignmentMethod method);

    QKnxNetIpCurrentConfigDib(const QNetworkAddressEntry &addressEntry,
                              const QHostAddress &gateway,
                              const QHostAddress &dhcp,
                              AssignmentMethod method);

    template <typename T> static QKnxNetIpCurrentConfigDib fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpStructHelper::fromBytes(bytes, index,
            QKnxNetIp::DescriptionType::CurrentIpConfiguration);
    }

    QKnxNetIp::DescriptionType descriptionType() const;
    QHostAddress ipAddress() const;
    QHostAddress subnetMask() const;
    QHostAddress defaultGateway() const;
    QHostAddress dhcpOrBootP() const;
    AssignmentMethod assignmentMethod() const;

    bool isValid() const override;

private:
    QKnxNetIpCurrentConfigDib(const QKnxNetIpDescriptionTypeStruct &other);
};

QT_END_NAMESPACE

#endif
