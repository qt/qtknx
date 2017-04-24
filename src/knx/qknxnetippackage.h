/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPPACKAGE_H
#define QKNXNETIPPACKAGE_H
#include <iostream>

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

    virtual bool isValid() const
    {
        const auto &headr = header();
        return headr.isValid() && size() == (headr.size() + payload().size());
    }

    virtual QString toString() const
    {
        return QStringLiteral("%1, %2").arg(header().toString(), payload().toString());
    }

    QKnxByteStoreRef payloadRef(quint16 index = 0) const
    {
        return m_payload.ref(index);
    }

    template <typename T = std::vector<quint8>> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        const auto &headr = header();
        T t(headr.totalSize(), 0);

        auto bytesGet = headr.bytes();
        std::copy(std::begin(bytesGet), std::end(bytesGet), std::begin(t));

        bytesGet = payload().bytes();
        std::copy(std::begin(bytesGet), std::end(bytesGet), std::next(std::begin(t), headr.size()));

        return t;
    }

    template <typename T, std::size_t S = 0>
        static QKnxNetIpPackage fromBytes(const T &bytes, quint16 index)
    {
        auto header = QKnxNetIpHeader::fromBytes(bytes, index);
        if (!header.isValid())
            return {};

        return QKnxNetIpPackage(header, QKnxNetIpPayload::fromBytes(bytes, index + header.size(),
            header.payloadSize()));
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
        return header().code();
    }

    void setCode(CodeType code)
    {
        m_header = QKnxNetIpHeader(code, header().size());
    }

    void setPayload(const QKnxNetIpPayload &payload)
    {
        m_payload = payload;
        m_header = QKnxNetIpHeader(header().code(), payload.size());
    }

private:
    QKnxNetIpHeader m_header;
    QKnxNetIpPayload m_payload;
};

QT_END_NAMESPACE

#endif
