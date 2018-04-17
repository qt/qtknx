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

#ifndef QKNXNETIPROUTINGBUSY_H
#define QKNXNETIPROUTINGBUSY_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpRoutingBusyProxy final
{
public:
    QKnxNetIpRoutingBusyProxy() = delete;
    ~QKnxNetIpRoutingBusyProxy() = default;

    QKnxNetIpRoutingBusyProxy(const QKnxNetIpFrame &&) = delete;
    explicit QKnxNetIpRoutingBusyProxy(const QKnxNetIpFrame &frame);

    bool isValid() const;

    QKnxNetIp::DeviceState deviceState() const;
    quint16 routingBusyWaitTime() const;
    quint16 routingBusyControl() const;

    class Q_KNX_EXPORT Builder final
    {
    public:
        Builder &setDeviceState(QKnxNetIp::DeviceState state);
        Builder &setRoutingBusyWaitTime(quint16 waitTime);
        Builder &setRoutingBusyControl(quint16 busyControl);

        QKnxNetIpFrame create() const;

    private:
        QKnxNetIp::DeviceState m_state { QKnxNetIp::DeviceState::KnxFault };
        quint16 m_waitTime { quint16(QKnxNetIp::RoutingBusyWaitTime) };
        quint16 m_busyControl { 0x0000 };
    };
    static QKnxNetIpRoutingBusyProxy::Builder builder();

private:
    const QKnxNetIpFrame &m_frame;};

QT_END_NAMESPACE

#endif
