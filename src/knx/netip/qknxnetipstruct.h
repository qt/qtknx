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

    virtual QKnxByteArray data() const
    {
        return m_data;
    }

    const QKnxByteArray &constData() const
    {
        return m_data;
    }

    virtual bool isValid() const
    {
        return m_header.isValid() && size() == (m_header.size() + m_data.size());
    }

    QKnxByteArray bytes() const
    {
        return m_header.bytes() + m_data;
    }

    static QKnxNetIpStruct<CodeType> fromBytes(const QKnxByteArray &bytes, quint16 index,
        CodeType type)
    {
        auto header = QKnxNetIpStructHeader<CodeType>::fromBytes(bytes, index);
        if (!header.isValid() || header.code() != type)
            return {};
        return { header, bytes.mid(index + header.size(), header.dataSize()) };
    }

    virtual ~QKnxNetIpStruct() = default;

protected:
    QKnxNetIpStruct() = default;

    explicit QKnxNetIpStruct(CodeType code)
        : m_header({ code })
    {}

    QKnxNetIpStruct(const  QKnxNetIpStructHeader<CodeType> &header, const QKnxByteArray &data)
        : m_header(header)
        , m_data(data)
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

    void setData(const QKnxByteArray &data)
    {
        m_data = data;
        setDataSize(data.size());
    }

private:
    QKnxNetIpStructHeader<CodeType> m_header;
    QKnxByteArray m_data;
};

using QKnxNetIpHostProtocolStruct = QKnxNetIpStruct<QKnxNetIp::HostProtocol>;
using QKnxNetIpConnectionTypeStruct = QKnxNetIpStruct<QKnxNetIp::ConnectionType>;
using QKnxNetIpDescriptionTypeStruct = QKnxNetIpStruct<QKnxNetIp::DescriptionType>;

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpHostProtocolStruct &package);
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpConnectionTypeStruct &package);
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpDescriptionTypeStruct &package);

QT_END_NAMESPACE

#endif
