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

struct Q_KNX_EXPORT QKnxNetIpHeader
{
    enum class ServiceType : quint16 {
        // KNXnet/IP Core service type identifiers
        SearchRequest = 0x0201,
        SearchResponse = 0x0202,
        DescriptionRequest = 0x0203,
        DescriptionResponse = 0x0204,
        ConnectRequest = 0x0205,
        ConnectResponse = 0x0206,
        ConnectionStateRequest = 0x0207,
        ConnectionStateResponse = 0x0208,
        DisconnectRequest = 0x0209,
        DisconnectResponse = 0x020a,

        // KNXnet/IP Device Management service type identifiers
        DeviceConfigurationRequest = 0x0310,
        DeviceConfigurationAcknowledge = 0x0311,

        // KNXnet/IP Tunneling service type identifiers
        TunnelingRequest = 0x0420,
        TunnelingAcknowledge = 0x0421,

        // KNXnet/IP Routing service type identifier
        RoutingIndication = 0x0530,
        RoutingLostMessage = 0x0531,
    };
    QKnxNetIpHeader::ServiceType serviceType() const { return m_serviceType; };
    void setServiceType(QKnxNetIpHeader::ServiceType serviceType) { m_serviceType = serviceType; }

    quint8 version() const { return KnxNetIpVersion_10; /* TODO: Adapt on additional versions. */ }
    quint8 headerSize() const { return KnxNetIpHeaderSize_10; /* TODO: Adapt on version change. */ }

private:
    QKnxNetIpHeader::ServiceType m_serviceType;

    static const quint8 KnxNetIpVersion_10 = 0x10;
    static const quint8 KnxNetIpHeaderSize_10 = 0x06;
};

struct Q_KNX_EXPORT QKnxNetIpHostProtocolAddressInformation
{
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

    quint8 size() const { return 8; }
    template <typename T> auto rawData() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        auto address = m_address.toIPv4Address();
        T t(size(), Qt::Uninitialized);
        t[0] = size();
        t[1] = quint8(m_hostProtocolCode);
        t[2] = quint8(address >> 24);
        t[3] = quint8(address >> 16);
        t[4] = quint8(address >> 8);
        t[5] = quint8(address);
        t[6] = quint8(m_port >> 8);
        t[7] = quint8(m_port);
        return t;
    }

private:
    quint16 m_port = 0;
    QHostAddress m_address;
    HostProtocolCode m_hostProtocolCode = HostProtocolCode::IpV4_Udp;
};

class Q_KNX_EXPORT QKnxNetIpFrame
{
public:
    QKnxNetIpHeader header() { return m_header; }
    void setHeader(const QKnxNetIpHeader &header) { m_header = header; }

    quint16 size() const { return m_header.headerSize() + m_frameSize; }

private:
    virtual ~QKnxNetIpFrame() = 0;

private:
    QKnxNetIpHeader m_header;

    quint16 m_frameSize = 0;
};

QT_END_NAMESPACE
