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

QKnxNetIpSearchResponseProxy::QKnxNetIpSearchResponseProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

QKnxNetIpHpai QKnxNetIpSearchResponseProxy::controlEndpoint() const
{
    return QKnxNetIpHpai::fromBytes(m_frame.constData(), 0);
}

QKnxNetIpDib QKnxNetIpSearchResponseProxy::deviceHardware() const
{
    return QKnxNetIpDib::fromBytes(m_frame.constData(), 8);
}

QKnxNetIpDib QKnxNetIpSearchResponseProxy::supportedFamilies() const
{
    return QKnxNetIpDib::fromBytes(m_frame.constData(), 62);
}

bool QKnxNetIpSearchResponseProxy::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::SearchResponse
        && m_frame.size() >= 70;
}

QKnxNetIpSearchResponseProxy::Builder QKnxNetIpSearchResponseProxy::builder()
{
    return QKnxNetIpSearchResponseProxy::Builder();
}


// -- QKnxNetIpSearchResponseProxy::Builder

QKnxNetIpSearchResponseProxy::Builder &
    QKnxNetIpSearchResponseProxy::Builder::setControlEndpoint(const QKnxNetIpHpai &hpai)
{
    m_hpai = hpai;
    return *this;
}

QKnxNetIpSearchResponseProxy::Builder &
    QKnxNetIpSearchResponseProxy::Builder::setDeviceHardware(const QKnxNetIpDib &ddib)
{
    if (QKnxNetIpDeviceDibProxy(ddib).isValid())
        m_ddib = ddib;
    return *this;
}

QKnxNetIpSearchResponseProxy::Builder &
    QKnxNetIpSearchResponseProxy::Builder::setSupportedFamilies(const QKnxNetIpDib &sdib)
{
    if (QKnxNetIpServiceFamiliesDibProxy(sdib).isValid())
        m_sdib = sdib;
    return *this;
}

QKnxNetIpFrame QKnxNetIpSearchResponseProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::SearchResponse, m_hpai.bytes() + m_ddib.bytes() + m_sdib
        .bytes() };
}

QT_END_NAMESPACE
