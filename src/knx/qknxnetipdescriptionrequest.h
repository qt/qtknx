/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPDESCRIPTIONREQUEST_H
#define QKNXNETIPDESCRIPTIONREQUEST_H

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDescriptionRequest final : public QKnxNetIpFrame
{
public:
    QKnxNetIpDescriptionRequest() = default;
    ~QKnxNetIpDescriptionRequest() override = default;

    explicit QKnxNetIpDescriptionRequest(const QKnxNetIpHPAI &controlEndpoint);

    template <typename T>
        static QKnxNetIpDescriptionRequest fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::DescriptionRequest);
    }

    bool isValid() const override;
    QKnxNetIpHPAI controlEndpoint() const;

private:
    QKnxNetIpDescriptionRequest(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
