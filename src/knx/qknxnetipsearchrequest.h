/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPSEARCHREQUEST_H
#define QKNXNETIPSEARCHREQUEST_H

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpSearchRequest final : public QKnxNetIpFrame
{
public:
    QKnxNetIpSearchRequest() = default;
    ~QKnxNetIpSearchRequest() override = default;

    explicit QKnxNetIpSearchRequest(const QKnxNetIpHPAI &discoveryEndpoint);

    template <typename T> static QKnxNetIpSearchRequest fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index, QKnxNetIp::ServiceType::SearchRequest);
    }

    bool isValid() const override;
    QKnxNetIpHPAI discoveryEndpoint() const;

private:
    QKnxNetIpSearchRequest(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
