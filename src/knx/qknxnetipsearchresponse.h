/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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

    QKnxNetIpSearchResponse(const QKnxNetIpHPAI &controlEndpoint,
                            const QKnxNetIpDeviceDIB &deviceHardware,
                            const QKnxNetIpServiceFamiliesDIB &supportedFamilies);

    template <typename T> static QKnxNetIpSearchResponse fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index, QKnxNetIp::ServiceType::SearchResponse);
    }

    QKnxNetIpHPAI controlEndpoint() const;
    QKnxNetIpDeviceDIB deviceHardware() const;
    QKnxNetIpServiceFamiliesDIB supportedFamilies() const;

    bool isValid() const override;

private:
    QKnxNetIpSearchResponse(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
