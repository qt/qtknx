/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetiptunnelingrequest.h"

QT_BEGIN_NAMESPACE

QKnxNetIpTunnelingRequest::QKnxNetIpTunnelingRequest(quint8 id,
        quint8 sequenceCounter, const QKnxCemi &cemi)
    : QKnxNetIpConnectionHeaderFrame(QKnxNetIp::ServiceType::TunnelingRequest)
{
    setConnectionHeader({ id, sequenceCounter });
    setPayload({ cemi.ref().bytes(), cemi.ref().size() });
}

quint8 QKnxNetIpTunnelingRequest::communicationChannelId() const
{
    return connectionHeader().communicationChannelId();
}

quint8 QKnxNetIpTunnelingRequest::sequenceCounter() const
{
    return connectionHeader().sequenceCounter();
}

QKnxCemi QKnxNetIpTunnelingRequest::cemi() const
{
    return QKnxCemi::fromBytes(payloadRef(), 0, payloadRef().size());
}

QT_END_NAMESPACE
