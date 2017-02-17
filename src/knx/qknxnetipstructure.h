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
#include <QtKnx/qknxtypecheck.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

struct QKnxNetIpUtils
{
    template <typename T, std::size_t S = 0> static QHostAddress fromArray(const T &data)
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>();

        if (data.size() < 4)
            return {};
        return QHostAddress(quint32(data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]));
    }

    template <typename T> static auto toArray(const QHostAddress &address) -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>();

        T t(4, Qt::Uninitialized);
        auto addr = address.toIPv4Address();
        t[0] = quint8(addr >> 24);
        t[1] = quint8(addr >> 16);
        t[2] = quint8(addr >> 8);
        t[3] = quint8(addr);
        return t;
    }
};

/*
    03_08_02 Core v01.05.01 AS.pdf

    2.1.3 Structures: All KNXnet/IP structures follow a common rule: the first
    octet shall always be the length of the complete structure (as some
    structures may have fields of variable length e.g. strings) and the second
    octet shall always be an identifier that shall specify the type of the
    structure. From the third octet on the structure data shall follow. If the
    amount of data exceeds 252 octets, the length octet shall be FFh and the
    next tow octets shall contain the length as a 16 bit value. Then the
    structure data shall start at the fifth octet.
*/
struct Q_KNX_EXPORT QKnxNetIpStructure
{
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

    template <typename T> auto header(quint8 code, quint16 dataSize) const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>();

        T t((dataSize > 0xfc ? 4 : 2), Qt::Uninitialized);
        if (dataSize > 0xfc) {
            t[0] = quint8(0xff);
            t[1] = quint8(quint16(dataSize + 4) >> 8);
            t[2] = quint8(dataSize + 4);
            t[3] = code;
        } else {
            t[0] = quint8(dataSize + 2);
            t[1] = code;
        }
        return t;
    }

    qint32 dataSize() const;
    template <typename T> auto data() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>();

        T t(m_size, Qt::Uninitialized);
        std::copy(std::begin(m_data), std::end(m_data), std::begin(t));
        return t;
    }

    template <typename T> auto data(int start, int length) const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>();

        if (m_data.size() < start + length)
            return T();

        T t(length, Qt::Uninitialized);
        auto begin = std::next(std::begin(m_data), start);
        std::copy(begin, begin + length, std::begin(t));
        return t;
    }

    qint32 rawSize() const;
    template <typename T> auto rawData() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>();

        T t(rawSize(), Qt::Uninitialized);
        auto h = header<T>(m_code, m_size);
        std::copy(std::begin(h), std::end(h), std::begin(t));
        std::copy(std::begin(m_data), std::end(m_data), std::next(std::begin(t), h.size()));
        return t;
    }

protected:
    QKnxNetIpStructure() = default;
    virtual ~QKnxNetIpStructure() = default;

    virtual QString toString() const;
    void resizeData(int size, bool makeEven = false);
    void appendData(const QVector<quint8> additinalData);

    QKnxNetIpStructure::HostProtocolCode hostProtocolCode() const
    {
        return QKnxNetIpStructure::HostProtocolCode(m_code);
    };

    QKnxNetIpStructure::ConnectionTypeCode connectionTypeCode() const
    {
        return QKnxNetIpStructure::ConnectionTypeCode(m_code);
    };

    QKnxNetIpStructure::DescriptionTypeCode descriptionTypeCode() const
    {
        return QKnxNetIpStructure::DescriptionTypeCode(m_code);
    };

    QKnxNetIpStructure(quint8 code, int size, bool makeEven = false)
    {
        m_code = code;
        resizeData(size, makeEven);
    }

    template <typename T> QKnxNetIpStructure(quint8 code, const T &data)
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>();
        setData(code, data);
    }

    template <typename T> void setData(quint8 code, const T &data)
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>();

        auto h = header<T>(code, quint16(data.size()));
        T t(h.size() + data.size(), Qt::Uninitialized);
        std::copy(std::begin(h), std::end(h), std::begin(t));
        std::copy(std::begin(data), std::end(data), std::next(std::begin(t), h.size()));
        setRawData(code, t, 0);
    }

    template <typename T> void setRawData(quint8 code, const T &rawData, qint32 offset)
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>();

        const qint32 availableSize = rawData.size() - offset;
        if (availableSize < 1) return; // first byte for the length information

        quint16 size = quint8(rawData[offset]);
        const quint8 headerSize = size == 0xff ? 4 : 2;
        if (availableSize < headerSize) return; // length information and code information

        if (headerSize == 4)
            size = quint16(rawData[offset + 1]) << 8 | rawData[offset + 2];

        if (availableSize < size || rawData[offset + headerSize - 1] != code)
            return;

        m_code = code;
        m_size = size - headerSize;
        m_data.resize(size - headerSize);

        auto begin = std::next(std::begin(rawData), offset + headerSize);
        std::copy(begin, begin + m_size, std::begin(m_data));
    }

private:
    quint8 m_code = 0;
    quint16 m_size = 0;
    QVector<quint8> m_data;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpStructure &structure);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxNetIpStructure &structure);

Q_DECLARE_TYPEINFO(QKnxNetIpStructure::HostProtocolCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIpStructure::ConnectionTypeCode, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIpStructure::DescriptionTypeCode, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
