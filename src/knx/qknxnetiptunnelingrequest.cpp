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

QKnxNetIpTunnelingRequest::QKnxNetIpTunnelingRequest(quint8 id, quint8 sequenceCount,
        const QKnxCemiFrame &cemi)
    : QKnxNetIpConnectionHeaderFrame(QKnxNetIp::ServiceType::TunnelingRequest)
{
    setConnectionHeader({ id, sequenceCount });
    setPayload(QKnxNetIpPayload::fromBytes(cemi.bytes(), 0, cemi.size()));
}

QKnxNetIpTunnelingRequest::QKnxNetIpTunnelingRequest(const QKnxNetIpConnectionHeaderFrame &other)
    : QKnxNetIpConnectionHeaderFrame(other)
{}

quint8 QKnxNetIpTunnelingRequest::channelId() const
{
    return connectionHeader().channelId();
}

quint8 QKnxNetIpTunnelingRequest::sequenceCount() const
{
    return connectionHeader().sequenceCount();
}

QKnxCemiFrame QKnxNetIpTunnelingRequest::cemi() const
{
    auto ref = payloadRef(connectionHeaderSize());
    return QKnxCemiFrame::fromBytes(ref.bytes<QByteArray>(), 0, ref.size());
}

bool QKnxNetIpTunnelingRequest::isValid() const
{
    // TODO: fix size check once the minimum CEMI frame size is known
    return QKnxNetIpConnectionHeaderFrame::isValid() && size() >= 12
        && code() == QKnxNetIp::ServiceType::TunnelingRequest;
}

QT_END_NAMESPACE
