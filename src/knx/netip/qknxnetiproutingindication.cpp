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

#include "qknxnetiproutingindication.h"

QT_BEGIN_NAMESPACE

QKnxNetIpRoutingIndicationProxy::QKnxNetIpRoutingIndicationProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

QKnxLinkLayerFrame QKnxNetIpRoutingIndicationProxy::linkLayerFrame() const
{
    return QKnxLinkLayerFrame::fromBytes(m_frame.constData(), 0, QKnx::MediumType::NetIP);
}

bool QKnxNetIpRoutingIndicationProxy::isValid() const
{
    // TODO: fix size check once the minimum CEMI frame size is known
    return m_frame.isValid() && m_frame.size() >= 8
        && m_frame.serviceType() == QKnxNetIp::ServiceType::RoutingIndication;
}

QKnxNetIpRoutingIndicationProxy::Builder QKnxNetIpRoutingIndicationProxy::builder()
{
    return QKnxNetIpRoutingIndicationProxy::Builder();
}


// -- QKnxNetIpRoutingIndicationProxy::Builder

QKnxNetIpRoutingIndicationProxy::Builder &
    QKnxNetIpRoutingIndicationProxy::Builder::setLinkLayerFrame(const QKnxLinkLayerFrame &llf)
{
    m_llf = llf;
    return *this;
}

QKnxNetIpFrame QKnxNetIpRoutingIndicationProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::RoutingIndication, m_llf.bytes() };
}

QT_END_NAMESPACE
