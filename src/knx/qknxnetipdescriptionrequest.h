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

    explicit QKnxNetIpDescriptionRequest(const QKnxNetIpHpai &controlEndpoint);

    template <typename T>
        static QKnxNetIpDescriptionRequest fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index,
            QKnxNetIp::ServiceType::DescriptionRequest);
    }

    bool isValid() const override;
    QKnxNetIpHpai controlEndpoint() const;

private:
    QKnxNetIpDescriptionRequest(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
