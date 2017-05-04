/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPROUTINGBUSY_H
#define QKNXNETIPROUTINGBUSY_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpRoutingBusy final : public QKnxNetIpFrame
{
public:
    QKnxNetIpRoutingBusy() = default;
    ~QKnxNetIpRoutingBusy() override = default;

    explicit QKnxNetIpRoutingBusy(QKnxNetIp::DeviceState state);
    QKnxNetIpRoutingBusy(QKnxNetIp::DeviceState state, quint8 routingBusyWaitTime,
        quint16 routingBusyControl);

    template <typename T> static QKnxNetIpRoutingBusy fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index, QKnxNetIp::ServiceType::RoutingBusy);
    }

    QKnxNetIp::DeviceState deviceState() const;
    quint8 routingBusyWaitTime() const;
    quint16 routingBusyControl() const;

    bool isValid() const override;

private:
    QKnxNetIpRoutingBusy(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
