/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipstruct.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpStruct

    \inmodule QtKnx
    \brief The QKnxNetIpStruct class represents the base for all KNXnet/IP
    structures.
*/

QKnxNetIpStruct::QKnxNetIpStruct(quint8 code)
    : m_header({ code })
{}

QKnxNetIpStruct::QKnxNetIpStruct(const QKnxNetIpStructHeader &h, const QKnxNetIpPayload &pa)
    : m_header(h)
    , m_payload(pa)
{}

/*!
    Returns the generic code stored in the KNXnet/IP structures header.
*/
quint8 QKnxNetIpStruct::code() const
{
    return m_header.code();
}

/*!
    Sets the generic code stored in the KNXnet/IP structures header.
*/
void QKnxNetIpStruct::setCode(quint8 code)
{
    m_header.setCode(code);
}

/*!
    Returns the number of bytes representing the KNXnet/IP structure.
*/
quint16 QKnxNetIpStruct::size() const
{
    return m_header.totalSize();
}

QKnxNetIpStructHeader QKnxNetIpStruct::header() const
{
    return m_header;
}

QKnxNetIpPayload QKnxNetIpStruct::payload() const
{
    return m_payload;
}

void QKnxNetIpStruct::setPayload(const QKnxNetIpPayload &payload)
{
    m_payload = payload;
    m_header.setPayloadSize(payload.size());
}

bool QKnxNetIpStruct::isValid() const
{
    return m_header.isValid() && size() == (m_header.size() + m_payload.size());
}

/*!
    Returns the KNXnet/IP total size, generic code and the bytes as string. Code,
    total size and bytes are formatted in hexadecimal notation.
*/
QString QKnxNetIpStruct::toString() const
{
    return QStringLiteral("%1, %2").arg(m_header.toString(), m_payload.toString());
}

QT_END_NAMESPACE
