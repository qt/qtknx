/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtypecheck.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

// The KNX Standard v2.1:
// 03 Volume 3 System Specifications
// 03_08_02 Core v01.05.01 AS.pdf - Paragraph 8.6.2
class Q_KNX_EXPORT QKnxNetIpHostProtocolAddressInformation
{
public:
    enum class HostProtocolCode : quint8 {
        IpV4_Udp = 0x01,
        IpV4_Tcp = 0x02
    };
    QKnxNetIpHostProtocolAddressInformation::HostProtocolCode hostProtocolCode() const {
        return m_hostProtocolCode;
    };
    void setHostProtocolCode(QKnxNetIpHostProtocolAddressInformation::HostProtocolCode code) {
        m_hostProtocolCode = code;
    }

    quint16 port() { return m_port; }
    QHostAddress address() { return m_address; }

    quint8 dataSize() const { return 6; }
    template <typename T> auto data() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        auto address = m_address.toIPv4Address();
        T t(6, Qt::Uninitialized);
        // TODO: Is the next part correct, regarding endianess?
        t[0] = quint8(address >> 24);
        t[1] = quint8(address >> 16);
        t[2] = quint8(address >> 8);
        t[3] = quint8(address);
        t[4] = quint8(m_port >> 8);
        t[5] = quint8(m_port);
        return t;
    }

    quint8 rawSize() const { return 8; }
    template <typename T> auto rawData() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        auto address = m_address.toIPv4Address();
        T t(2, Qt::Uninitialized);
        t[0] = 8; t[1] = quint8(m_hostProtocolCode);
        return t + data<T>();
    }

private:
    quint16 m_port = 0;
    QHostAddress m_address;
    HostProtocolCode m_hostProtocolCode = HostProtocolCode::IpV4_Udp;
};

Q_DECLARE_TYPEINFO(QKnxNetIpHostProtocolAddressInformation, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIpHostProtocolAddressInformation::HostProtocolCode, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
