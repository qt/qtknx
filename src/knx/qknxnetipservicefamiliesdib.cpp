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

#include "qknxnetipservicefamiliesdib.h"

#include <algorithm>

QT_BEGIN_NAMESPACE

// 7.5.4.3 Supported service families DIB
// The service family IDs shall be the high octet of the Service Type ID

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib()
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{}

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib(ServiceFamilieId id, quint8 version)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(id, version);
}

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib(const ServiceFamilyIdVersions &families)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(families);
}

QKnxNetIp::DescriptionType QKnxNetIpServiceFamiliesDib::descriptionType() const
{
    return code();
}

void QKnxNetIpServiceFamiliesDib::add(ServiceFamilieId id, quint8 version)
{
    auto load = payload();
    load.appendBytes<std::array<quint8, 2>, 2>(std::array<quint8, 2>{ { quint8(id), version } });
    setPayload(load);
}

void QKnxNetIpServiceFamiliesDib::add(const ServiceFamilyIdVersions &families)
{
    QByteArray additionalData;

    int i = 0;
    auto keys = families.uniqueKeys();
    for (auto key : qAsConst(keys)) {
        auto values = families.values(key);
        std::sort(std::begin(values), std::end(values));
        for (auto value : values)
            additionalData[i++] = quint8(key), additionalData[i++] = value;
    }

    auto load = payload();
    load.appendBytes(additionalData);
    setPayload(load);
}

QKnxNetIpServiceFamiliesDib::ServiceFamilyIdVersions
    QKnxNetIpServiceFamiliesDib::serviceFamilyIdVersions() const
{
    ServiceFamilyIdVersions serviceTypesAndVersions;

    const auto &ref = payloadRef();
    for (int i = 0 ; i < ref.size() ; i += 2)
        serviceTypesAndVersions.insertMulti(ServiceFamilieId(ref.byte(i)), ref.byte(i+1));

    return serviceTypesAndVersions;
}

QKnxNetIp::ConnectionType
    QKnxNetIpServiceFamiliesDib::connectionTypeFromServiceType(ServiceFamilieId serviceType)
{
    switch (serviceType) {
        case ServiceFamilieId::Core:
            return QKnxNetIp::ConnectionType::Unknown;
        case ServiceFamilieId::DeviceManagement:
            return QKnxNetIp::ConnectionType::DeviceManagement;
        case ServiceFamilieId::IpTunneling:
            return QKnxNetIp::ConnectionType::Tunnel;
        case ServiceFamilieId::IpRouting:
            return QKnxNetIp::ConnectionType::Tunnel;
        case ServiceFamilieId::RemoteLogging:
            return QKnxNetIp::ConnectionType::RemoteLogging;
        case ServiceFamilieId::RemoteConfigAndDiagnosis:
            return QKnxNetIp::ConnectionType::RemoteConfiguration;
        case ServiceFamilieId::ObjectServer:
            return QKnxNetIp::ConnectionType::ObjectServer;
        default:
            break;
    }
    return QKnxNetIp::ConnectionType::Unknown;
}

bool QKnxNetIpServiceFamiliesDib::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && (size() % 2 == 0) // must be even sized
        && descriptionType() == QKnxNetIp::DescriptionType::SupportedServiceFamilies;
}

QT_END_NAMESPACE
