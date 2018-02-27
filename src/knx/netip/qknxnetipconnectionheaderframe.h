/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXNETIPCONNECTIONHEADERFRAME_H
#define QKNXNETIPCONNECTIONHEADERFRAME_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipconnectionheader.h>
#include <QtKnx/qknxnetipframe.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectionHeaderFrame : public QKnxNetIpFrame
{
    friend struct QKnxNetIpConnectionHeaderFrameHelper;

public:
    QKnxNetIpConnectionHeaderFrame() = default;
    ~QKnxNetIpConnectionHeaderFrame() override = default;

    QKnxNetIpConnectionHeaderFrame(const QKnxNetIpConnectionHeaderFrame &) = default;
    QKnxNetIpConnectionHeaderFrame &operator=(const QKnxNetIpConnectionHeaderFrame &) = default;

    quint8 connectionHeaderSize() const
    {
        return m_connectionHeader.size();
    }

    QKnxNetIpConnectionHeader connectionHeader() const
    {
        return m_connectionHeader;
    }

    virtual QString toString() const override
    {
        return QStringLiteral("Header { %1 }, Connection header { %2 }, Payload { %3 } ")
            .arg(header().toString(), m_connectionHeader.toString(), payload().toString());
    }

protected:
    using QKnxNetIpFrame::QKnxNetIpFrame;

    QKnxNetIpConnectionHeaderFrame(const QKnxNetIpFrameHeader &header,
            const QKnxNetIpConnectionHeader &connHeader, const QKnxNetIpPayload &payload)
        : QKnxNetIpFrame(header, payload)
        , m_connectionHeader(connHeader)
    {}

    void setConnectionHeader(const QKnxNetIpConnectionHeader &connHeader)
    {
        QKnxNetIpPayload payload(connHeader.bytes());
        payload.appendBytes(payloadRef(m_connectionHeader.size()).bytes(0));
        QKnxNetIpFrame::setPayload(payload);

        m_connectionHeader = connHeader;
    }

    void setPayload(const QKnxNetIpPayload &payload) override
    {
        auto load = payload;
        load.insertBytes(0, m_connectionHeader.bytes());
        QKnxNetIpFrame::setPayload(load);
    }

private:
    QKnxNetIpConnectionHeader m_connectionHeader;
};

struct QKnxNetIpConnectionHeaderFrameHelper
{
    static QKnxNetIpConnectionHeaderFrame fromBytes(const QKnxByteArray &bytes, quint16 index,
            QKnxNetIp::ServiceType sType)
    {
        auto header = QKnxNetIpFrameHeader::fromBytes(bytes, index);
        if (!header.isValid() || header.serviceType() != sType)
            return {};

        auto connectionHeader = QKnxNetIpConnectionHeader::fromBytes(bytes, index + header.size());
        if (!connectionHeader.isValid())
            return {};

        return QKnxNetIpConnectionHeaderFrame(header, connectionHeader,
            QKnxNetIpPayload::fromBytes(bytes, index + header.size(), header.dataSize()));
    }
};

QT_END_NAMESPACE

#endif
