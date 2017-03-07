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

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QKnxNetIpStruct &other)
    : QKnxNetIpStruct(other)
{}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(ServiceFamilieId id, quint8 versions)
    : QKnxNetIpStruct(quint8(QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies))
{
    add(id, versions);
}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QMap<ServiceFamilieId, quint8> &families)
    : QKnxNetIpStruct(quint8(QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies))
{
    add(families);
}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QVector<ServiceFamilieId> &ids,
        const QVector<quint8> &versions)
    : QKnxNetIpStruct(quint8(QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies))
{
    add(ids, versions);
}

QKnxNetIp::DescriptionTypeCode QKnxNetIpServiceFamiliesDIB::descriptionTypeCode() const
{
    return QKnxNetIp::DescriptionTypeCode(code());
}

void QKnxNetIpServiceFamiliesDIB::add(ServiceFamilieId id, quint8 versions)
{
    QKnxNetIpPayload payload;
    payload.appendBytes<std::array<quint8, 2>, 2>(std::array<quint8, 2>{ { quint8(id), versions } });
    setPayload(payload);
}

void QKnxNetIpServiceFamiliesDIB::add(const QMap<ServiceFamilieId, quint8> &families)
{
    int i = 0;
    std::vector<quint8> additionalData(families.size() * 2, 0u);
    for (auto it = std::begin(families); it != std::end(families); ++it, ++i)
        additionalData[i] = quint8(it.key()), additionalData[i + 1] = it.value();

    auto load = payload();
    load.appendBytes(additionalData);
    setPayload(load);
}

void QKnxNetIpServiceFamiliesDIB::add(const QVector<ServiceFamilieId> &ids, const QVector<quint8> &versions)
{
    if (ids.size() != versions.size())
        return;

    std::vector<quint8> additionalData(ids.size() * 2, 0u);
    for (int i = 0; i < ids.size(); ++i)
        additionalData[i] = quint8(ids[i]), additionalData[i + 1] = versions[i];

    auto load = payload();
    load.appendBytes(additionalData);
    setPayload(load);
}

bool QKnxNetIpServiceFamiliesDIB::isValid() const
{
    return QKnxNetIpStruct::isValid() && (size() % 2 == 0) // must be even sized
        && descriptionTypeCode() == QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies;
}

QT_END_NAMESPACE
