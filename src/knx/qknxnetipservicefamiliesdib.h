/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstructure.h>

QT_BEGIN_NAMESPACE

// 7.5.4.3 Supported service families DIB
// The service family IDs shall be the high octet of the Service Type ID

class Q_KNX_EXPORT QKnxNetIpServiceFamiliesDIB final : private QKnxNetIpStructure
{
    enum class ServiceFamilieId : quint8
    {
        Core = 0x02,
        DeviceManagement = 0x03,
        IpTunneling = 0x04,
        IpRouting = 0x05,
        RemoteLogging = 0x06,
        RemoteConfigAndDiagnosis = 0x07,
        ObjectServer = 0x08
    };

public:
    QKnxNetIpServiceFamiliesDIB() = default;

    explicit QKnxNetIpServiceFamiliesDIB(const QByteArray &data)
        : QKnxNetIpStructure(quint8(DescriptionTypeCode::SupportedServiceFamilies), data)
    {
    }
    explicit QKnxNetIpServiceFamiliesDIB(const QVector<quint8> &data)
        : QKnxNetIpStructure(quint8(DescriptionTypeCode::SupportedServiceFamilies), data)
    {
    }
    QKnxNetIpServiceFamiliesDIB(const QByteArray &rawData, qint32 offset)
        : QKnxNetIpStructure(quint8(DescriptionTypeCode::SupportedServiceFamilies), rawData, offset)
    {
    }
    QKnxNetIpServiceFamiliesDIB(const QVector<quint8> &rawData, qint32 offset)
        : QKnxNetIpStructure(quint8(DescriptionTypeCode::SupportedServiceFamilies), rawData, offset)
    {
    }

    QKnxNetIpServiceFamiliesDIB(const QMap<ServiceFamilieId, quint8> &families);
    QKnxNetIpServiceFamiliesDIB(const QVector<ServiceFamilieId> &ids, const QVector<quint8> &versions);

    void add(ServiceFamilieId id, quint8 versions);
    void add(const QMap<ServiceFamilieId, quint8> &families);
    void add(const QVector<ServiceFamilieId> &ids, const QVector<quint8> &versions);

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::descriptionTypeCode;
};

QT_END_NAMESPACE
