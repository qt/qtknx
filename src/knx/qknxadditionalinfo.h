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
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxAdditionalInfo final
{
public:
    enum class Type : quint8
    {
        Reserved = 0x00,
        PlMediumInformation = 0x01,
        RfMediumInformation = 0x02,
        BusmonitorStatusInfo = 0x03,
        TimestampRelative = 0x04,
        TimeDelayUntilSending = 0x05,
        ExtendedRelativeTimestamp = 0x06,
        BiBatInformation = 0x07,
        RfMultiInformation = 0x08,
        PreambleAndPostamble = 0x09,
        RfFastAckInformation = 0x0a,
        ManufactorSpecificData = 0xfe,
        EscCode = 0xff
    };
    QKnxAdditionalInfo::Type type() const;

    QKnxAdditionalInfo() = default;
    QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QByteArray &data);
    QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QVector<quint8> &data);

    qint32 dataSize() const;
    template <typename T = std::vector<quint8>> auto data() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(m_data.size(), Qt::Uninitialized);
        std::copy(std::begin(m_data), std::end(m_data), std::begin(t));
        return t;
    }

    qint32 rawSize() const;
    template <typename T = std::vector<quint8>> auto rawData() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        if (!isValid())
            return {};

        T t(2, Qt::Uninitialized);
        t[0] = quint8(m_type), t[1] = quint8(m_data.size());
        return t + data<T>();
    }

    bool isValid() const;

    template<typename T, std::size_t S = 0>
        static bool isValid(QKnxAdditionalInfo::Type type, const T &data)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        const int size = data.size();
        if (size > 252)
            return false;

        switch (type) {
        case QKnxAdditionalInfo::Type::PlMediumInformation:
        case QKnxAdditionalInfo::Type::RfMediumInformation:
        case QKnxAdditionalInfo::Type::BusmonitorStatusInfo:
        case QKnxAdditionalInfo::Type::TimestampRelative:
        case QKnxAdditionalInfo::Type::TimeDelayUntilSending:
        case QKnxAdditionalInfo::Type::ExtendedRelativeTimestamp:
        case QKnxAdditionalInfo::Type::BiBatInformation:
        case QKnxAdditionalInfo::Type::RfMultiInformation:
        case QKnxAdditionalInfo::Type::PreambleAndPostamble:
            return size == expectedDataSize(type);
        case QKnxAdditionalInfo::Type::RfFastAckInformation:
            return (size >= expectedDataSize(type)) && ((size % 2) == 0);
        case QKnxAdditionalInfo::Type::ManufactorSpecificData:
            return size >= expectedDataSize(type);
        default:
            break;
        }
        return false;
    }
    static qint32 expectedDataSize(QKnxAdditionalInfo::Type type, bool *isFixedSize = nullptr);

    QString toString() const;

private:
    QVector<quint8> m_data;
    QKnxAdditionalInfo::Type m_type = QKnxAdditionalInfo::Type::EscCode;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxAdditionalInfo &info);
Q_KNX_EXPORT QDataStream &operator>>(QDataStream &stream, QKnxAdditionalInfo &info);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxAdditionalInfo &info);

Q_DECLARE_TYPEINFO(QKnxAdditionalInfo, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxAdditionalInfo::Type, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
