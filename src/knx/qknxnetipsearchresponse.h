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

#ifndef QKNXNETIPSEARCHRESPONSE_H
#define QKNXNETIPSEARCHRESPONSE_H

#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpSearchResponse final : public QKnxNetIpFrame
{
public:
    QKnxNetIpSearchResponse() = default;
    ~QKnxNetIpSearchResponse() override = default;

    QKnxNetIpSearchResponse(const QKnxNetIpHpai &controlEndpoint,
                            const QKnxNetIpDeviceDib &deviceHardware,
                            const QKnxNetIpServiceFamiliesDib &supportedFamilies);

    template <typename T> static QKnxNetIpSearchResponse fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index, QKnxNetIp::ServiceType::SearchResponse);
    }

    QKnxNetIpHpai controlEndpoint() const;
    QKnxNetIpDeviceDib deviceHardware() const;
    QKnxNetIpServiceFamiliesDib supportedFamilies() const;

    bool isValid() const override;

private:
    QKnxNetIpSearchResponse(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
