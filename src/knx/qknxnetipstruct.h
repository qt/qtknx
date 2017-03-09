/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetippayload.h>
#include <QtKnx/qknxnetipstructheader.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpStruct
{
public:
    quint8 code() const;
    void setCode(quint8 code);

    quint16 size() const;
    QKnxNetIpStructHeader header() const;

    QKnxNetIpPayload payload() const;
    void setPayload(const QKnxNetIpPayload &payload);

    virtual bool isValid() const;
    virtual QString toString() const;
    virtual ~QKnxNetIpStruct() = default;

protected:
    QKnxNetIpStruct() = default;

    explicit QKnxNetIpStruct(quint8 code);
    QKnxNetIpStruct(const QKnxNetIpStructHeader &header, const QKnxNetIpPayload &payload);

    QKnxNetIpStruct(const QKnxNetIpStruct& ) = default;
    QKnxNetIpStruct &operator=(const QKnxNetIpStruct& ) = default;

    template <typename T = std::vector<quint8>> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(m_header.totalSize(), Qt::Uninitialized);

        auto bytes = m_header.bytes();
        std::copy(std::begin(bytes), std::end(bytes), std::begin(t));

        bytes = m_payload.bytes();
        std::copy(std::begin(bytes), std::end(bytes), std::next(std::begin(t), m_header.size()));

        return t;
    }

    template <typename T, std::size_t S = 0>
        static QKnxNetIpStruct fromBytes(const T &bytes, quint16 index)
    {
        auto header = QKnxNetIpStructHeader::fromBytes(bytes, index);
        if (!header.isValid())
            return {};

        return QKnxNetIpStruct(header, QKnxNetIpPayload::fromBytes(bytes, index + header.size(),
            header.payloadSize()));
    }

private:
    QKnxNetIpStructHeader m_header;
    QKnxNetIpPayload m_payload;
};

QT_END_NAMESPACE
