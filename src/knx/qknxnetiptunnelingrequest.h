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

#ifndef QKNXNETIPTUNNELINGREQUEST_H
#define QKNXNETIPTUNNELINGREQUEST_H

#include <QtKnx/qknxcemiframe.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipconnectionheaderframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpTunnelingRequest final : public QKnxNetIpConnectionHeaderFrame
{
public:
    QKnxNetIpTunnelingRequest() = default;
    ~QKnxNetIpTunnelingRequest() override = default;

    QKnxNetIpTunnelingRequest(quint8 channelId, quint8 sequenceCount, const QKnxCemiFrame &cemi);

    template <typename T> static QKnxNetIpTunnelingRequest fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpConnectionHeaderFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::TunnelingRequest);
    }

    quint8 channelId() const;
    quint8 sequenceCount() const;
    QKnxCemiFrame cemi() const;

    bool isValid() const override;

private:
    QKnxNetIpTunnelingRequest(const QKnxNetIpConnectionHeaderFrame &other);
};

QT_END_NAMESPACE

#endif
