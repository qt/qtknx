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

#ifndef QKNXNETIPSTRUCT_H
#define QKNXNETIPSTRUCT_H

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxnetipstructheader.h>
#include <QtKnx/qknxnetippayload.h>

QT_BEGIN_NAMESPACE

template <typename CodeType> class Q_KNX_EXPORT QKnxNetIpStruct
{
    static_assert(is_type<CodeType, QKnxNetIp::HostProtocol, QKnxNetIp::ConnectionType,
        QKnxNetIp::DescriptionType>::value, "Type not supported.");

public:
    quint16 size() const
    {
        return m_header.totalSize();
    }

    virtual QKnxNetIpStructHeader<CodeType> header() const
    {
        return m_header;
    }

    virtual QKnxNetIpPayload payload() const
    {
        return m_payload;
    }

    QKnxNetIpPayloadRef payloadRef(quint16 index = 0) const
    {
        return m_payload.ref(index);
    }

    virtual bool isValid() const
    {
        return m_header.isValid() && size() == (m_header.size() + m_payload.size());
    }

    virtual QString toString() const
    {
        return QStringLiteral("%1, %2").arg(m_header.toString(), m_payload.toString());
    }

    QKnxByteArray bytes() const
    {
        QKnxByteArray t(m_header.totalSize(), 0);

        auto headr = m_header.bytes();
        std::copy(std::begin(headr), std::end(headr), std::begin(t));

        auto loadRef = m_payload.ref();
        std::copy(std::begin(loadRef), std::end(loadRef), std::next(std::begin(t), m_header.size()));

        return t;
    }

    static QKnxNetIpStruct<CodeType> fromBytes(const QKnxByteArray &bytes, quint16 index,
            CodeType nType)
    {
        auto header = QKnxNetIpStructHeader<CodeType>::fromBytes(bytes, index);
        if (!header.isValid() || header.code() != nType)
            return {};

        return { header, QKnxNetIpPayload::fromBytes(bytes, index + header.size(), header
            .dataSize()) };
    }

    virtual ~QKnxNetIpStruct() = default;

protected:
    QKnxNetIpStruct() = default;

    explicit QKnxNetIpStruct(CodeType code)
        : m_header({ code })
    {}

    QKnxNetIpStruct(const  QKnxNetIpStructHeader<CodeType> &header, const QKnxNetIpPayload &payload)
        : m_header(header)
        , m_payload(payload)
    {}

    CodeType code() const
    {
        return m_header.code();
    }

    void setCode(CodeType code)
    {
        m_header.setCode(code);
    }

    quint16 dataSize() const
    {
        return m_header.dataSize();
    }

    void setDataSize(quint16 dataSize)
    {
        m_header.setDataSize(dataSize);
    }

    void setPayload(const QKnxNetIpPayload &payload)
    {
        m_payload = payload;
        setDataSize(payload.size());
    }

private:
    QKnxNetIpStructHeader<CodeType> m_header;
    QKnxNetIpPayload m_payload;
};

using QKnxNetIpHostProtocolStruct = QKnxNetIpStruct<QKnxNetIp::HostProtocol>;
using QKnxNetIpConnectionTypeStruct = QKnxNetIpStruct<QKnxNetIp::ConnectionType>;
using QKnxNetIpDescriptionTypeStruct = QKnxNetIpStruct<QKnxNetIp::DescriptionType>;

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpHostProtocolStruct &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpHostProtocolStruct &package);

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpConnectionTypeStruct &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpConnectionTypeStruct &package);

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpDescriptionTypeStruct &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpDescriptionTypeStruct &package);

QT_END_NAMESPACE

#endif
