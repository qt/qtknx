/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpHeader
{
public:
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

Q_DECLARE_TYPEINFO(QKnxNetIpHeader, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIpHeader::ServiceType, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
