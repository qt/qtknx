/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipservicefamiliesdib.h"

QT_BEGIN_NAMESPACE

// 7.5.4.3 Supported service families DIB
// The service family IDs shall be the high octet of the Service Type ID

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB()
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(ServiceFamilieId id, quint8 versions)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(id, versions);
}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QMap<ServiceFamilieId, quint8> &families)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(families);
}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QVector<ServiceFamilieId> &ids,
        const QVector<quint8> &versions)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(ids, versions);
}

QKnxNetIp::DescriptionType QKnxNetIpServiceFamiliesDIB::descriptionType() const
{
    return QKnxNetIp::DescriptionType(code());
}

void QKnxNetIpServiceFamiliesDIB::add(ServiceFamilieId id, quint8 versions)
{
    auto load = payload();
    load.appendBytes<std::array<quint8, 2>, 2>(std::array<quint8, 2>{ { quint8(id), versions } });
    setPayload(load);
}

void QKnxNetIpServiceFamiliesDIB::add(const QMap<ServiceFamilieId, quint8> &families)
{
    int i = 0;
    std::vector<quint8> additionalData(families.size() * 2, 0u);
    for (auto it = std::begin(families); it != std::end(families); ++it, ++i)
        additionalData[i] = quint8(it.key()), additionalData[i += 1] = it.value();

    auto load = payload();
    load.appendBytes(additionalData);
    setPayload(load);
}

void QKnxNetIpServiceFamiliesDIB::add(const QVector<ServiceFamilieId> &ids, const QVector<quint8> &versions)
{
    if (ids.size() != versions.size())
        return;

    std::vector<quint8> additionalData(ids.size() * 2, 0u);
    int j = 0;
    for (int i = 0; i < ids.size(); ++i, ++j)
        additionalData[j] = quint8(ids[i]), additionalData[j += 1] = versions[i];

    auto load = payload();
    load.appendBytes(additionalData);
    setPayload(load);
}

QMap<QKnxNetIpServiceFamiliesDIB::ServiceFamilieId, quint8> QKnxNetIpServiceFamiliesDIB::availableServiceFamilieId() const
{
    QMap<QKnxNetIpServiceFamiliesDIB::ServiceFamilieId, quint8> serviceTypesAndVersions;
    for (int i = 0 ; i < payload().size() ; ++i) {
        auto payloadBytes = payload().bytes(i, 2);
        QKnxNetIpServiceFamiliesDIB::ServiceFamilieId service =
            QKnxNetIpServiceFamiliesDIB::ServiceFamilieId(payloadBytes[0]);
        quint8 version = payloadBytes[1];
        serviceTypesAndVersions.insert(service, version);
        ++i;
    }
    return serviceTypesAndVersions;
}


bool QKnxNetIpServiceFamiliesDIB::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && (size() % 2 == 0) // must be even sized
        && descriptionType() == QKnxNetIp::DescriptionType::SupportedServiceFamilies;
}

QT_END_NAMESPACE
