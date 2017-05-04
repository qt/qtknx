/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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
