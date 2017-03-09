/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipframe.h"

QT_BEGIN_NAMESPACE

QKnxNetIpFrame::QKnxNetIpFrame(quint16 code)
    : m_header({ code })
{}

QKnxNetIpFrame::QKnxNetIpFrame(const QKnxNetIpFrameHeader &header, const QKnxNetIpPayload &payload)
    : m_header(header)
    , m_payload(payload)
{}

quint8 QKnxNetIpFrame::code() const
{
    return m_header.code();
}

void QKnxNetIpFrame::setCode(quint8 code)
{
    m_header.setCode(code);
}

quint16 QKnxNetIpFrame::size() const
{
    return m_header.totalSize();
}

QKnxNetIpFrameHeader QKnxNetIpFrame::header() const
{
    return m_header;
}

QKnxNetIpPayload QKnxNetIpFrame::payload() const
{
    return m_payload;
}

void QKnxNetIpFrame::setPayload(const QKnxNetIpPayload &payload)
{
    m_payload = payload;
    m_header.setPayloadSize(payload.size());
}

bool QKnxNetIpFrame::isValid() const
{
    return m_header.isValid() && size() == (m_header.size() + m_payload.size());
}

QString QKnxNetIpFrame::toString() const
{
    return QStringLiteral("%1, %2").arg(m_header.toString(), m_payload.toString());
}

QT_END_NAMESPACE
