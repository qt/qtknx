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
#include <QtKnx/qknxnetipframeheader.h>
#include <QtKnx/qknxnetippayload.h>
#include <QtKnx/qknxnetipstructheader.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

template <typename CodeType, typename QKnxNetIpHeader> class Q_KNX_EXPORT QKnxNetIpPackage
{
public:
    quint16 size() const
    {
        return header().totalSize();
    }

    QKnxNetIpHeader header() const
    {
        return m_header;
    }

    QKnxNetIpPayload payload() const
    {
        return m_payload;
    }

    QKnxByteStoreRef payloadRef(quint16 index = 0) const
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

    virtual void setPayload(const QKnxNetIpPayload &payload)
    {
        m_payload = payload;
    }

    template <typename T = std::vector<quint8>> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        const auto &headr = header();
        T t(headr.totalSize(), 0);

        auto headrBytes = headr.bytes();
        std::copy(std::begin(headrBytes), std::end(headrBytes), std::begin(t));

        const auto &ref = payloadRef();
        std::copy_n(ref.bytes(), ref.size(), std::next(std::begin(t), headr.size()));

        return t;
    }

private:
    QKnxNetIpHeader m_header;
    QKnxNetIpPayload m_payload;
};

QT_END_NAMESPACE

#endif
