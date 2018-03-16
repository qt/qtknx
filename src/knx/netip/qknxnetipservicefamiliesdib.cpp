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

#include "qknxnetipservicefamiliesdib.h"

#include <algorithm>

QT_BEGIN_NAMESPACE

// 7.5.4.3 Supported service families DIB
// The service family IDs shall be the high octet of the Service Type ID

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib()
    : QKnxNetIpDib(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{}

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib(const QKnxNetIpDib &other)
    : QKnxNetIpDib(other)
{}

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib(ServiceFamilieId id, quint8 version)
    : QKnxNetIpDib(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(id, version);
}

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib(const ServiceFamilyIdVersions &families)
    : QKnxNetIpDib(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(families);
}

QKnxNetIp::DescriptionType QKnxNetIpServiceFamiliesDib::descriptionType() const
{
    return code();
}

void QKnxNetIpServiceFamiliesDib::add(ServiceFamilieId id, quint8 version)
{
    setData(constData() + QKnxByteArray { quint8(id), version });
}

void QKnxNetIpServiceFamiliesDib::add(const ServiceFamilyIdVersions &families)
{
    QKnxByteArray additionalData;

    int i = 0;
    auto keys = families.uniqueKeys();
    for (auto key : qAsConst(keys)) {
        auto values = families.values(key);
        std::sort(std::begin(values), std::end(values));
        for (auto value : values)
            additionalData[i++] = quint8(key), additionalData[i++] = value;
    }

    setData(constData() + additionalData);
}

QKnxNetIpServiceFamiliesDib::ServiceFamilyIdVersions
    QKnxNetIpServiceFamiliesDib::serviceFamilyIdVersions() const
{
    ServiceFamilyIdVersions serviceTypesAndVersions;

    const auto &data = constData();
    for (int i = 0 ; i < dataSize() ; i += 2)
        serviceTypesAndVersions.insertMulti(ServiceFamilieId(data.value(i)), data.value(i+1));

    return serviceTypesAndVersions;
}

bool QKnxNetIpServiceFamiliesDib::isValid() const
{
    return QKnxNetIpDib::isValid() && (size() % 2 == 0) // must be even sized
        && descriptionType() == QKnxNetIp::DescriptionType::SupportedServiceFamilies;
}

QT_END_NAMESPACE
