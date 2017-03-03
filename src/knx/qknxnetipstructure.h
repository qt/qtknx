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

struct Q_KNX_EXPORT QKnxNetIpStructure
{
    virtual ~QKnxNetIpStructure() = default;

    enum class HostProtocolCode : quint8
    {
        IpV4_Udp = 0x01,
        IpV4_Tcp = 0x02
    };

    enum class ConnectionTypeCode : quint8
    {
        DeviceManagementConnection = 0x03,
        TunnelConnection = 0x04,
        RemoteLoggingConnection = 0x06,
        RemoteConfigurationConnection = 0x07,
        ObjectServerConnection = 0x08
    };

    enum class DescriptionTypeCode : quint8
    {
        DeviceInfo = 0x01,
        SupportedServiceFamilies = 0x02,
        IpConfiguration = 0x03,
        CurrentIpConfiguration = 0x04,
        KnxAddresses = 0x05,
        ManufactorData = 0xfe,
        NotUsed = 0xff
    };

    qint32 dataSize() const;
    template <typename T> auto data() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        if (!m_header.isValid())
            return {};

        T t(m_header.dataSize(), Qt::Uninitialized);
        std::copy(std::begin(m_data), std::end(m_data), std::begin(t));
        return t;
    }

    template <typename T> auto data(int start, int length) const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        if (!m_header.isValid() || m_data.size() < start + length)
            return {};

        T t(length, Qt::Uninitialized);
        auto begin = std::next(std::begin(m_data), start);
        std::copy(begin, begin + length, std::begin(t));
        return t;
    }

    qint32 rawSize() const;
    template <typename T> auto rawData() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        if (!m_header.isValid())
            return {};

        T t(rawSize(), Qt::Uninitialized);
        auto h = m_header.rawData<T>();
        std::copy(std::begin(h), std::end(h), std::begin(t));
        std::copy(std::begin(m_data), std::end(m_data), std::next(std::begin(t), h.size()));
        return t;
    }

protected:
    QKnxNetIpStructure() = default;

    virtual bool isValid() const;
    virtual QString toString() const;

    void resizeData(int size, bool makeEven = false);

    QKnxNetIpStructure::HostProtocolCode hostProtocolCode() const
    {
        return QKnxNetIpStructure::HostProtocolCode(m_header.code());
    };

    QKnxNetIpStructure::ConnectionTypeCode connectionTypeCode() const
    {
        return QKnxNetIpStructure::ConnectionTypeCode(m_header.code());
    };

    QKnxNetIpStructure::DescriptionTypeCode descriptionTypeCode() const
    {
        return QKnxNetIpStructure::DescriptionTypeCode(m_header.code());
    };

    explicit QKnxNetIpStructure(quint8 code)
        : m_header({ code })
    {}

    template <typename T> QKnxNetIpStructure(quint8 code, const T &data)
        : m_header({ code })
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");
        setData(data);
    }

    template <typename T, std::size_t S = 0> void setData(const T &data)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        m_header.setDataSize(quint16(data.size()));
        m_data.resize(m_header.dataSize());
        std::copy(std::begin(data), std::end(data), std::begin(m_data));
    }

    template <typename T, std::size_t S = 0> void appendData(const T &data)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        if (data.size() <= 0)
            return;

        m_header.setDataSize(m_header.dataSize() + quint16(data.size()));
        m_data.resize(m_header.dataSize());
        std::copy(std::begin(data), std::end(data), std::begin(m_data));
    }

    template <typename T, std::size_t S = 0>
        static QKnxNetIpStructure fromRawData(const T &rawData, qint32 offset)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        QKnxNetIpStructure structure;
        auto header = Header::fromRawData(rawData, offset);
        if (!header.isValid())
            return structure;

        structure.m_header = header;
        structure.m_data.resize(header.dataSize());

        auto begin = std::next(std::begin(rawData), offset + header.headerSize());
        std::copy(begin, begin + header.dataSize(), std::begin(structure.m_data));

        return structure;
    }

private:
        /*
        03_08_02 Core v01.05.01 AS.pdf,

        2.1.3 Structures - All KNXnet/IP structures follow a common rule: the
        first octet shall always be the length of the complete structure (as
        some structures may have fields of variable length e.g. strings) and
        the second octet shall always be an identifier that shall specify the
        type of the structure. From the third octet on the structure data shall
        follow. If the amount of data exceeds 252 octets, the length octet
        shall be FFh and the next two octets shall contain the length as a 16
        bit value. Then the structure data shall start at the fifth octet.
    */
    struct Header
    {
        Header() = default;
        explicit Header(quint8 code)
            : Header(code, 2)
        {}

        Header(quint8 code, quint16 rawSize)
            : m_code(code)
            , m_rawSize(rawSize)
            , m_headerSize(rawSize >= 0xff ? 4 : 2)
        {
            m_dataSize = rawSize - m_headerSize;
        }

        quint8 code() const { return m_code; }
        quint16 rawSize() const { return m_rawSize; }
        qint8 headerSize() const { return m_headerSize; }

        void setDataSize(quint16 dataSize)
        {
            m_headerSize = (dataSize > 0xfc) ? 4 : 2;
            m_dataSize = dataSize;
            m_rawSize = m_headerSize + m_dataSize;
        }
        quint16 dataSize() const { return m_dataSize; }

        bool isValid() const { return m_rawSize != 0 && m_code != 0 && m_headerSize != 0; }

        template <typename T> auto rawData() const -> decltype(T())
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>::value, "Type not supported.");
            return rawData<T>(m_code, m_rawSize);
        }

        template <typename T> static auto rawData(quint8 code, quint16 rawSize) -> decltype(T())
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>>::value, "Type not supported.");

            T t(rawSize >= 0xff ? 4 : 2, Qt::Uninitialized);
            if (rawSize >= 0xff) {
                t[0] = quint8(0xff);
                t[1] = quint8(quint16(rawSize) >> 8);
                t[2] = quint8(rawSize);
                t[3] = code;
            } else {
                t[0] = quint8(rawSize);
                t[1] = code;
            }
            return t;
        }

        template <typename T, std::size_t S = 0> static Header fromRawData(const T &data, qint32 offset)
        {
            static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
                std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

            const qint32 availableSize = data.size() - offset;
            if (availableSize < 1)
                return {}; // first byte for the length information required

            quint16 rawSize = quint8(data[offset]);
            const quint8 headerSize = rawSize == 0xff ? 4 : 2;
            if (availableSize < headerSize)
                return {}; // length information and code information required

            if (headerSize == 4)
                rawSize = quint16(data[offset + 1]) << 8 | data[offset + 2];

            return { quint8(data[offset + headerSize - 1]), rawSize };
        }

    private:
        quint8 m_code = 0x00u;
        quint16 m_rawSize = 0x0000u;
        quint16 m_dataSize = 0x0000u;
        qint8 m_headerSize = 0x00;
    };

private:
    Header m_header;
    QVector<quint8> m_data;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpStructure &structure);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxNetIpStructure &structure);

Q_DECLARE_TYPEINFO(QKnxNetIpStructure::HostProtocolCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIpStructure::ConnectionTypeCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIpStructure::DescriptionTypeCode, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
