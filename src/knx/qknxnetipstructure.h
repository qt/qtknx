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

QT_BEGIN_NAMESPACE

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

    qint32 dataSize() const;
    template <typename T> auto data() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        T t(m_size, Qt::Uninitialized);
        std::copy(m_data.constBegin(), m_data.constEnd(), t.begin());
        return t;
    }

    template <typename T> auto data(int start, int length) const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        if (m_data.size() < start + length)
            return T();

        T t(length, Qt::Uninitialized);
        std::copy(m_data.constBegin() + start, m_data.constBegin() + start + length, t.begin());
        return t;
    }

    qint32 rawSize() const;
    template <typename T> auto rawData() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        T t(m_size > 0xfc ? 4 : 2, Qt::Uninitialized);
        if (m_size > 0xfc) {
            t[0] = quint8(0xff);
            t[1] = quint8(rawSize() >> 8);
            t[2] = quint8(rawSize());
            t[3] = m_code;
        } else {
            t[0] = quint8(rawSize());
            t[1] = m_code;
        }
        return t + data<T>();
    }

protected:
    QKnxNetIpStructure() = default;
    virtual ~QKnxNetIpStructure() = default;

    virtual QString toString() const;
    void resize(int size, bool makeEven = false);

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

    template <typename T> QKnxNetIpStructure(quint8 code, const T &data)
    {
        setRawData(code, data);
    }

    template <typename T> QKnxNetIpStructure(quint8 code, const T &rawData, qint32 offset)
    {
        setRawData(code, rawData, offset);
    }

    template <typename T> void setRawData(quint8 code, const T &data)
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        m_code = code;
        m_size = data.size();
        m_data.resize(m_size);
        std::copy(data.constBegin(), data.constEnd(), m_data.begin());
    }

    template <typename T> void setRawData(quint8 code, const T &rawData, qint32 offset)
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        qint32 availableSize = rawData.size() - offset;
        if (availableSize <= 0)
            return;

        quint16 size = quint8(rawData[offset]);
        qint32 codeOffset = offset + (size < 0xff ? 2 : 3);
        if (availableSize < codeOffset || rawData[codeOffset] != code)
            return;

        if (size == 0xff)
            size = quint16(rawData[offset + 1]) << 8 | rawData[offset + 2];

        if (size > availableSize)
            return;
        setRawData(rawData[codeOffset], rawData.mid(codeOffset + 1, size - offset + 1));
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
