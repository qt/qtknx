/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPROUTINGINDICATION_H
#define QKNXNETIPROUTINGINDICATION_H

#include <QtKnx/qknxcemi.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpRoutingIndication final : public QKnxNetIpFrame
{
public:
    QKnxNetIpRoutingIndication() = default;
    ~QKnxNetIpRoutingIndication() override = default;

    explicit QKnxNetIpRoutingIndication(const QKnxCemi &cemi);

    QKnxCemi cemi() const;
    void setCemi(const QKnxCemi &cemi);
};

QT_END_NAMESPACE

#endif
