/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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

    quint16 size() const override
    {
        return Package::size() + m_connectionHeader.size();
    }

    QKnxNetIpConnectionHeader connectionHeader() const
    {
        return m_connectionHeader;
    }

    bool isValid() const override
    {
        const auto &headr = header();
        return headr.isValid() && size() == (headr.size() + m_connectionHeader.size()
            + payloadRef().size());
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
        m_connectionHeader = connHeader;
    }

private:
    const QKnxNetIpConnectionHeader *connectionHeader(bool *ok) const override
    {
        if (ok)
            *ok = true;
        return &m_connectionHeader;
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
            QKnxNetIpPayload::fromBytes(bytes, index + connectionHeader.size() + header.size(),
                header.payloadSize() - connectionHeader.size()));
    }
};

QT_END_NAMESPACE

#endif
