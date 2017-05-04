/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPCONNECTIONSTATEREQUEST_H
#define QKNXNETIPCONNECTIONSTATEREQUEST_H

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectionStateRequest final : public QKnxNetIpFrame
{
public:
    QKnxNetIpConnectionStateRequest() = default;
    ~QKnxNetIpConnectionStateRequest() override = default;

    QKnxNetIpConnectionStateRequest(quint8 channelId, const QKnxNetIpHPAI &controlEndpoint);

    template <typename T>
        static QKnxNetIpConnectionStateRequest fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::ConnectionStateRequest);
    }

    quint8 channelId() const;
    QKnxNetIpHPAI controlEndpoint() const;

    bool isValid() const override;

private:
    QKnxNetIpConnectionStateRequest(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
