/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipservicefamiliesdib.h"

QT_BEGIN_NAMESPACE

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QMap<ServiceFamilieId, quint8> &families)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::SupportedServiceFamilies), 2 * families.size())
{
    add(families);
}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QVector<ServiceFamilieId> &ids,
        const QVector<quint8> &versions)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::SupportedServiceFamilies), 2 * ids.size())
{
    add(ids, versions);
}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QByteArray &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::SupportedServiceFamilies), data)
{
}

QKnxNetIpServiceFamiliesDIB::QKnxNetIpServiceFamiliesDIB(const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(DescriptionTypeCode::SupportedServiceFamilies), data)
{
}

QKnxNetIpServiceFamiliesDIB QKnxNetIpServiceFamiliesDIB::fromRawData(const QByteArray &rawData,
    qint32 offset)
{
    QKnxNetIpServiceFamiliesDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::SupportedServiceFamilies), rawData, offset);
    return dib;
}

QKnxNetIpServiceFamiliesDIB QKnxNetIpServiceFamiliesDIB::fromRawData(const QVector<quint8> &rawData,
    qint32 offset)
{
    QKnxNetIpServiceFamiliesDIB dib;
    dib.setRawData(quint8(DescriptionTypeCode::SupportedServiceFamilies), rawData, offset);
    return dib;
}

void QKnxNetIpServiceFamiliesDIB::add(ServiceFamilieId id, quint8 versions)
{
    appendData({ quint8(id), versions });
}

void QKnxNetIpServiceFamiliesDIB::add(const QMap<ServiceFamilieId, quint8> &families)
{
    int i = 0;
    QVector<quint8> additionalData(families.size() * 2, 0u);
    for (auto it = std::begin(families); it != std::end(families); ++it, ++i)
        additionalData[i] = quint8(it.key()), additionalData[i + 1] = it.value();
    appendData(additionalData);
}

void QKnxNetIpServiceFamiliesDIB::add(const QVector<ServiceFamilieId> &ids, const QVector<quint8> &versions)
{
    if (ids.size() != versions.size())
        return;

    QVector<quint8> additionalData(ids.size() * 2, 0u);
    for (int i = 0; i < ids.size(); ++i)
        additionalData[i] = quint8(ids[i]), additionalData[i + 1] = versions[i];
    appendData(additionalData);
}

QT_END_NAMESPACE
