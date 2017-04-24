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

    bool isValid() const override;
    QKnxNetIpHPAI controlEndpoint() const;
};

QT_END_NAMESPACE

#endif
