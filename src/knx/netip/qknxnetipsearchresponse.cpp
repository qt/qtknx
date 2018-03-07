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

#include "qknxnetipsearchresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpSearchResponse::QKnxNetIpSearchResponse(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

QKnxNetIpHpai QKnxNetIpSearchResponse::controlEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 0);
}

QKnxNetIpDeviceDib QKnxNetIpSearchResponse::deviceHardware() const
{
    return QKnxNetIpDeviceDib::fromBytes(m_frame.constData(), 8);
}

QKnxNetIpServiceFamiliesDib QKnxNetIpSearchResponse::supportedFamilies() const
{
    return QKnxNetIpServiceFamiliesDib::fromBytes(m_frame.constData(), 62);
}

bool QKnxNetIpSearchResponse::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::SearchResponse
        && m_frame.size() >= 70;
}

QKnxNetIpSearchResponse::Builder QKnxNetIpSearchResponse::builder()
{
    return QKnxNetIpSearchResponse::Builder();
}


// -- QKnxNetIpSearchResponse::Builder

QKnxNetIpSearchResponse::Builder &
    QKnxNetIpSearchResponse::Builder::setControlEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

QKnxNetIpSearchResponse::Builder &
QKnxNetIpSearchResponse::Builder::setDeviceHardware(const QKnxNetIpDeviceDib &ddib)
{
    m_ddib = ddib;
    return *this;
}

QKnxNetIpSearchResponse::Builder &
    QKnxNetIpSearchResponse::Builder::setSupportedFamilies(const QKnxNetIpServiceFamiliesDib &sdib)
{
    m_sdib = sdib;
    return *this;
}

QKnxNetIpFrame QKnxNetIpSearchResponse::Builder::create() const
{
    return { QKnxNetIp::ServiceType::SearchResponse, m_hpai.bytes() + m_ddib.bytes() + m_sdib
        .bytes() };
}

QT_END_NAMESPACE
