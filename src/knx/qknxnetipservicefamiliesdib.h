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

#ifndef QKNXNETIPSERVICEFAMILIESDIB_H
#define QKNXNETIPSERVICEFAMILIESDIB_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qmap.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpServiceFamiliesDib final : public QKnxNetIpDescriptionTypeStruct
{
public:
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
    using ServiceFamilyIdVersions = QMultiMap<ServiceFamilieId, quint8>;

    QKnxNetIpServiceFamiliesDib();
    ~QKnxNetIpServiceFamiliesDib() override = default;

    QKnxNetIpServiceFamiliesDib(ServiceFamilieId id, quint8 version);
    QKnxNetIpServiceFamiliesDib(const ServiceFamilyIdVersions &families);

    template <typename T> static QKnxNetIpServiceFamiliesDib fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpStructHelper::fromBytes(bytes, index,
            QKnxNetIp::DescriptionType::SupportedServiceFamilies);
    }
    // TODO: review
    static QKnxNetIp::ConnectionType connectionTypeFromServiceType(ServiceFamilieId serviceType);

    QKnxNetIp::DescriptionType descriptionType() const;
    ServiceFamilyIdVersions serviceFamilyIdVersions() const;

    void add(ServiceFamilieId id, quint8 version);
    void add(const ServiceFamilyIdVersions &families);

    bool isValid() const override;

private:
    QKnxNetIpServiceFamiliesDib(const QKnxNetIpDescriptionTypeStruct &other);
};

Q_DECLARE_TYPEINFO(QKnxNetIpServiceFamiliesDib, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
