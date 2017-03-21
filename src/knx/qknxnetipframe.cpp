/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipframe.h"

QT_BEGIN_NAMESPACE

/*!
    Creates a new QKnxNetIpStruct with the given \a code.
*/
QKnxNetIpFrame::QKnxNetIpFrame(quint16 code)
    : m_header({ code })
{}

/*!
    Creates a new QKnxNetIpFrame with the given \a header and \a payload.
    The \a header is expected to be fully setup and all values describing the
    payload match the given \a payload.
    \note No adjustments are made to the function arguments.
*/
QKnxNetIpFrame::QKnxNetIpFrame(const QKnxNetIpFrameHeader &header, const QKnxNetIpPayload &payload)
    : m_header(header)
    , m_payload(payload)
{}

/*!
    Returns the generic code stored in the KNXnet/IP frame header.
*/
quint16 QKnxNetIpFrame::code() const
{
    return m_header.code();
}

/*!
    Sets the generic code stored in the KNXnet/IP frame header.
*/
void QKnxNetIpFrame::setCode(quint16 code)
{
    m_header.setCode(code);
}

/*!
    Returns the number of bytes representing the KNXnet/IP frame including
    the header and the payload.
*/
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
