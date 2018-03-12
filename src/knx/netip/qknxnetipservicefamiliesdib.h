/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXNETIPSERVICEFAMILIESDIB_H
#define QKNXNETIPSERVICEFAMILIESDIB_H

#include <QtCore/qmap.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>

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

    static QKnxNetIpServiceFamiliesDib fromBytes(const QKnxByteArray &bytes, quint16 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index,
            QKnxNetIp::DescriptionType::SupportedServiceFamilies);
    }

    QKnxNetIp::DescriptionType descriptionType() const;
    ServiceFamilyIdVersions serviceFamilyIdVersions() const;

    void add(ServiceFamilieId id, quint8 version);
    void add(const ServiceFamilyIdVersions &families);

    bool isValid() const override;

private:
    QKnxNetIpServiceFamiliesDib(const QKnxNetIpDescriptionTypeStruct &other);
};

QT_END_NAMESPACE

#endif
