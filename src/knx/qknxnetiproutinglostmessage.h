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

#ifndef QKNXNETIPROUTINGLOSTMESSAGE_H
#define QKNXNETIPROUTINGLOSTMESSAGE_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpRoutingLostMessage final : public QKnxNetIpFrame
{
public:
    QKnxNetIpRoutingLostMessage() = default;
    ~QKnxNetIpRoutingLostMessage() override = default;

    QKnxNetIpRoutingLostMessage(QKnxNetIp::DeviceState state, quint16 lostMessageCount);

    template <typename T>
        static QKnxNetIpRoutingLostMessage fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::RoutingLostMessage);
    }

    QKnxNetIp::DeviceState deviceState() const;
    quint16 lostMessageCount() const;

    bool isValid() const override;

private:
    QKnxNetIpRoutingLostMessage(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
