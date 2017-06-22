/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetiptunnelingrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpTunnelingRequest::QKnxNetIpTunnelingRequest(quint8 id, quint8 sequenceCount,
        const QKnxCemiFrame &cemi)
    : QKnxNetIpConnectionHeaderFrame(QKnxNetIp::ServiceType::TunnelingRequest)
{
    setConnectionHeader({ id, sequenceCount });
    setPayload({ cemi.ref().bytes(), cemi.ref().size() });
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
    return QKnxCemiFrame::fromBytes(payloadRef(), 0, payloadRef().size());
}

bool QKnxNetIpTunnelingRequest::isValid() const
{
    // TODO: fix size check once the minimum CEMI frame size is known
    return QKnxNetIpConnectionHeaderFrame::isValid() && size() >= 12
        && code() == QKnxNetIp::ServiceType::TunnelingRequest;
}

QT_END_NAMESPACE
