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

#include "qknxnetiptunnelingrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpTunnelingRequestProxy::QKnxNetIpTunnelingRequestProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

bool QKnxNetIpTunnelingRequestProxy::isValid() const
{
    return m_frame.isValid() && m_frame.size() >= 12 && cemi().isValid()
        && m_frame.serviceType() == QKnxNetIp::ServiceType::TunnelingRequest;
}

quint8 QKnxNetIpTunnelingRequestProxy::channelId() const
{
    return m_frame.channelId();
}

quint8 QKnxNetIpTunnelingRequestProxy::sequenceNumber() const
{
    return m_frame.sequenceNumber();
}

QKnxLinkLayerFrame QKnxNetIpTunnelingRequestProxy::cemi() const
{
    return QKnxLinkLayerFrame::fromBytes(m_frame.data(), 0, QKnx::MediumType::NetIP);
}

QKnxNetIpTunnelingRequestProxy::Builder QKnxNetIpTunnelingRequestProxy::builder()
{
    return QKnxNetIpTunnelingRequestProxy::Builder();
}


// -- QKnxNetIpTunnelingRequestProxy::Builder

QKnxNetIpTunnelingRequestProxy::Builder &
    QKnxNetIpTunnelingRequestProxy::Builder::setChannelId(quint8 channelId)
{
    m_channelId = channelId;
    return *this;
}

QKnxNetIpTunnelingRequestProxy::Builder &
    QKnxNetIpTunnelingRequestProxy::Builder::setSequenceNumber(quint8 sequenceNumber)
{
    m_sequenceNumber = sequenceNumber;
    return *this;
}

QKnxNetIpTunnelingRequestProxy::Builder &
    QKnxNetIpTunnelingRequestProxy::Builder::setCemi(const QKnxLinkLayerFrame &cemi)
{
    m_cemi = cemi;
    return *this;
}

QKnxNetIpFrame QKnxNetIpTunnelingRequestProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::TunnelingRequest, { m_channelId, m_sequenceNumber },
        m_cemi.bytes() };
}

QT_END_NAMESPACE
