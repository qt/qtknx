/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDescriptionRequest final : private QKnxNetIpFrame
{
public:
    QKnxNetIpDescriptionRequest() = default;
    ~QKnxNetIpDescriptionRequest() override = default;

    explicit QKnxNetIpDescriptionRequest(const QKnxNetIpHPAI &controlEndpoint);

    bool isValid() const override;
    QKnxNetIpHPAI controlEndpoint() const;

    using QKnxNetIpFrame::size;
    using QKnxNetIpFrame::bytes;
    using QKnxNetIpFrame::payload;
    using QKnxNetIpFrame::toString;
};

QT_END_NAMESPACE
