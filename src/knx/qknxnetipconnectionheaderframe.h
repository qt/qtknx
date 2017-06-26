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

#ifndef QKNXNETIPCONNECTIONHEADERFRAME_H
#define QKNXNETIPCONNECTIONHEADERFRAME_H

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipconnectionheader.h>
#include <QtKnx/qknxnetipframe.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectionHeaderFrame
    : public QKnxNetIpPackage<QKnxNetIp::ServiceType, QKnxNetIpFrameHeader>
{
    friend struct QKnxNetIpConnectionHeaderFrameHelper;
    using Package = QKnxNetIpPackage<QKnxNetIp::ServiceType, QKnxNetIpFrameHeader>;

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
    using Package::Package;

    QKnxNetIpConnectionHeaderFrame(const QKnxNetIpFrameHeader &header,
            const QKnxNetIpConnectionHeader &connHeader, const QKnxNetIpPayload &payload)
        : Package(header, payload)
        , m_connectionHeader(connHeader)
    {}

    void setConnectionHeader(const QKnxNetIpConnectionHeader &connHeader)
    {
        QKnxNetIpPayload payload(connHeader.ref().bytes(), connHeader.size());
        payload.appendBytes(payloadRef(m_connectionHeader.size()).bytes<std::vector<quint8>>());
        Package::setPayload(payload);

        m_connectionHeader = connHeader;
    }

    void setPayload(const QKnxNetIpPayload &payload) override
    {
        auto load = payload;
        load.insertBytes(0, m_connectionHeader.bytes());
        Package::setPayload(load);
    }

private:
    QKnxNetIpConnectionHeader m_connectionHeader;
};

struct QKnxNetIpConnectionHeaderFrameHelper
{
    template <typename T, std::size_t S = 0>
        static QKnxNetIpConnectionHeaderFrame fromBytes(const T &bytes, quint16 index,
            QKnxNetIp::ServiceType sType)
    {
        auto header = QKnxNetIpFrameHeader::fromBytes(bytes, index);
        if (!header.isValid() || header.code() != sType)
            return {};

        auto connectionHeader = QKnxNetIpConnectionHeader::fromBytes(bytes, index + header.size());
        if (!connectionHeader.isValid())
            return {};

        return QKnxNetIpConnectionHeaderFrame(header, connectionHeader,
            QKnxNetIpPayload::fromBytes(bytes, index + header.size(), header.payloadSize()));
    }
};

QT_END_NAMESPACE

#endif
