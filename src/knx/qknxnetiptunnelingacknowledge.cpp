/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxnetiptunnelingacknowledge.h"

QT_BEGIN_NAMESPACE

QKnxNetIpTunnelingAcknowledge::QKnxNetIpTunnelingAcknowledge(quint8 id, quint8 sequenceCount,
        QKnxNetIp::Error status)
    : QKnxNetIpConnectionHeaderFrame(QKnxNetIp::ServiceType::TunnelingAcknowledge)
{
    QKnxNetIpConnectionHeader header;
    header.setChannelId(id);
    header.setSequenceCount(sequenceCount);
    header.setServiceTypeSpecificValue(quint8(status));
    setConnectionHeader(header);
}

QKnxNetIpTunnelingAcknowledge::QKnxNetIpTunnelingAcknowledge(
        const QKnxNetIpConnectionHeaderFrame &other)
    : QKnxNetIpConnectionHeaderFrame(other)
{}

quint8 QKnxNetIpTunnelingAcknowledge::channelId() const
{
    return connectionHeader().channelId();
}

quint8 QKnxNetIpTunnelingAcknowledge::sequenceCount() const
{
    return connectionHeader().sequenceCount();
}

QKnxNetIp::Error QKnxNetIpTunnelingAcknowledge::status() const
{
    return QKnxNetIp::Error(connectionHeader().serviceTypeSpecificValue());
}

bool QKnxNetIpTunnelingAcknowledge::isValid() const
{
    return QKnxNetIpConnectionHeaderFrame::isValid() && size() == 10
        && code() == QKnxNetIp::ServiceType::TunnelingAcknowledge;
}

QT_END_NAMESPACE
