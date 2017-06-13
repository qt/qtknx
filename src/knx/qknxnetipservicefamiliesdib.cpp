/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipservicefamiliesdib.h"

#include <algorithm>

QT_BEGIN_NAMESPACE

// 7.5.4.3 Supported service families DIB
// The service family IDs shall be the high octet of the Service Type ID

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB()
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(ServiceFamilieId id, quint8 version)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(id, version);
}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const ServiceFamilyIdVersions &families)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(families);
}

QKnxNetIp::DescriptionType QKnxNetIpServiceFamiliesDIB::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

void QKnxNetIpServiceFamiliesDIB::add(ServiceFamilieId id, quint8 version)
{
    auto load = payload();
    load.appendBytes<std::array<quint8, 2>, 2>(std::array<quint8, 2>{ { quint8(id), version } });
    setPayload(load);
}

void QKnxNetIpServiceFamiliesDIB::add(const ServiceFamilyIdVersions &families)
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

QKnxNetIpServiceFamiliesDIB::ServiceFamilyIdVersions
    QKnxNetIpServiceFamiliesDIB::serviceFamilyIdVersions() const
{
    ServiceFamilyIdVersions serviceTypesAndVersions;

    const auto &ref = payloadRef();
    for (int i = 0 ; i < ref.size() ; i += 2)
        serviceTypesAndVersions.insertMulti(ServiceFamilieId(ref.byte(i)), ref.byte(i+1));

    return serviceTypesAndVersions;
}

QKnxNetIp::ConnectionType
    QKnxNetIpServiceFamiliesDIB::connectionTypeFromServiceType(ServiceFamilieId serviceType)
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

bool QKnxNetIpServiceFamiliesDIB::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && (size() % 2 == 0) // must be even sized
        && descriptionType() == QKnxNetIp::DescriptionType::SupportedServiceFamilies;
}

QT_END_NAMESPACE
