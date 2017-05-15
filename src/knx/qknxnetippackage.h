/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPPACKAGE_H
#define QKNXNETIPPACKAGE_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipconnectionheader.h>
#include <QtKnx/qknxnetipframeheader.h>
#include <QtKnx/qknxnetippayload.h>
#include <QtKnx/qknxnetipstructheader.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

template <typename CodeType, typename QKnxNetIpHeader> class Q_KNX_EXPORT QKnxNetIpPackage
{
    friend struct QKnxNetIpFrameHelper;
    friend struct QKnxNetIpStructHelper;
    friend struct QKnxNetIpConnectionHeaderFrameHelper;

public:
    virtual quint16 size() const
    {
        return m_header.totalSize();
    }

    virtual QKnxNetIpHeader header() const
    {
        return m_header;
    }

    virtual QKnxNetIpPayload payload() const
    {
        return m_payload;
    }

    virtual QKnxNetIpPayloadRef payloadRef(quint16 index = 0) const
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

    template <typename T = std::vector<quint8>> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(m_header.totalSize(), 0);

        auto bytesGet = m_header.bytes();
        std::copy(std::begin(bytesGet), std::end(bytesGet), std::begin(t));

        quint16 s = m_header.size();
        if (auto ch = connectionHeader()) {
            bytesGet = ch->bytes();
            std::copy(std::begin(bytesGet), std::end(bytesGet), std::next(std::begin(t), s));
            s += ch->size();
        }

        auto loadRef = m_payload.ref();
        std::copy(std::begin(loadRef), std::end(loadRef), std::next(std::begin(t), s));

        return t;
    }

    virtual ~QKnxNetIpPackage() = default;

protected:
    QKnxNetIpPackage() = default;

    explicit QKnxNetIpPackage(CodeType code)
        : m_header({ code })
    {}

    QKnxNetIpPackage(const QKnxNetIpHeader &header, const QKnxNetIpPayload &payload)
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

    quint16 payloadSize() const
    {
        return m_header.payloadSize();
    }

    void setPayloadSize(quint16 payloadSize)
    {
        m_header.setPayloadSize(payloadSize);
    }

    void setPayload(const QKnxNetIpPayload &payload)
    {
        m_payload = payload;
        setPayloadSize(payload.size());
    }

    virtual const QKnxNetIpConnectionHeader *connectionHeader(bool *ok = nullptr) const
    {
        Q_UNUSED(ok)
        return nullptr;
    }

private:
    QKnxNetIpHeader m_header;
    QKnxNetIpPayload m_payload;
};

QT_END_NAMESPACE

#endif
