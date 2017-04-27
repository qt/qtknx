/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPCONNECTIONHEADERFRAME_H
#define QKNXNETIPCONNECTIONHEADERFRAME_H

#include <QtKnx/qknxnetipconnectionheader.h>
#include <QtKnx/qknxnetipframe.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectionHeaderFrame : public QKnxNetIpFrame
{
public:
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

    template <typename T, std::size_t S = 0>
        static QKnxNetIpConnectionHeaderFrame fromBytes(const T &bytes, quint16 index)
    {
        auto header = QKnxNetIpFrameHeader::fromBytes(bytes, index);
        if (!header.isValid())
            return {};

        auto connectionHeader = QKnxNetIpConnectionHeader::fromBytes(bytes, index + header.size());
        if (!connectionHeader.isValid())
            return {};

        QKnxNetIpConnectionHeaderFrame frame(header, QKnxNetIpPayload::fromBytes(bytes, index,
            connectionHeader.size() + header.size(), header.payloadSize() - connectionHeader.size()));
        frame.setConnectionHeader(connectionHeader);
        return frame;
    }

protected:
    using QKnxNetIpFrame::QKnxNetIpFrame;

    void setPayload(const QKnxNetIpPayload &payload) override
    {
        QKnxNetIpFrame::setPayload(payload);
        setPayloadSize(m_connectionHeader.size() + payload.size());
    }

    void setConnectionHeader(const QKnxNetIpConnectionHeader &connHeader)
    {
        m_connectionHeader = connHeader;
    }

private:
    QKnxNetIpConnectionHeader m_connectionHeader;
};

QT_END_NAMESPACE

#endif
