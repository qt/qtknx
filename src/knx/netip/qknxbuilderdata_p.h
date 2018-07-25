/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#ifndef QKNXBUILDERDATA_P_H
#define QKNXBUILDERDATA_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt KNX API.  It exists for the convenience
// of the Qt KNX implementation.  This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qshareddata.h>
#include <QtKnx/qknxnamespace.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtKnx/qknxnetipsrp.h>

QT_BEGIN_NAMESPACE

class QKnxFeatureBuilderPrivate : public QSharedData
{
public:
    QKnxFeatureBuilderPrivate() = default;
    ~QKnxFeatureBuilderPrivate() = default;

    quint8 m_channelId { 0 };
    quint8 m_sequenceNumber { 0 };
    QKnx::InterfaceFeature m_if { QKnx::InterfaceFeature::Unknown };
    QKnx::ReturnCode m_result { QKnx::ReturnCode::Success };
    QKnxByteArray m_featureValue;
};

class SrpBuilders::ProgrammingMode::ProgrammingModePrivate : public QSharedData
{
public:
    bool m_mandatory { false };
};

class SrpBuilders::MacAddress::MacAddressPrivate : public QSharedData
{
public:
    QKnxByteArray m_macAddress;
    bool m_mandatory { false };
};

class SrpBuilders::SupportedFamily::SupportedFamilyPrivate : public QSharedData
{
public:
    QVector<QKnxServiceInfo> m_infos;
    bool m_mandatory { false };
};

class SrpBuilders::RequestDibs::RequestDibsPrivate : public QSharedData
{
public:
    QVector<QKnxNetIp::DescriptionType> m_types;
    bool m_mandatory { false };
};

class QKnxNetIpSearchRequestExtendedBuilderPrivate : public QSharedData
{
public:
    QKnxNetIpSearchRequestExtendedBuilderPrivate() = default;
    ~QKnxNetIpSearchRequestExtendedBuilderPrivate() = default;

    QKnxNetIpHpai m_hpai = { QKnxNetIp::HostProtocol::Unknown};
    QVector<QKnxNetIpSrp> m_srps;
};

class QKnxNetIpSearchResponseExtendedBuilderPrivate : public QSharedData
{
public:
    QKnxNetIpSearchResponseExtendedBuilderPrivate() = default;
    ~QKnxNetIpSearchResponseExtendedBuilderPrivate() = default;

    QKnxNetIpHpai m_hpai = { QKnxNetIp::HostProtocol::Unknown};
    QSet<QKnxNetIpDib> m_optionalDibs;
    QKnxNetIpDib m_hardware;
    QKnxNetIpDib m_supFamilies;
};

QT_END_NAMESPACE

#endif
