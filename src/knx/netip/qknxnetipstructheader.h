/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#ifndef QKNXNETIPSTRUCTHEADER_H
#define QKNXNETIPSTRUCTHEADER_H

#include <QtCore/qstring.h>

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

template <typename CodeType> class Q_KNX_EXPORT QKnxNetIpStructHeader final
{
    friend struct QKnxNetIpStructHelper;
    friend struct QKnxNetIpConnectionHeaderFrameHelper;

    static_assert(is_type<CodeType, QKnxNetIp::HostProtocol, QKnxNetIp::ConnectionType,
        QKnxNetIp::DescriptionType>::value, "Type not supported.");

public:
    QKnxNetIpStructHeader() = default;
    ~QKnxNetIpStructHeader() = default;

    explicit QKnxNetIpStructHeader(CodeType code)
        : QKnxNetIpStructHeader(code, 0)
    {}

    QKnxNetIpStructHeader(CodeType code, quint16 dataSize)
    {
        setCode(code);
        setDataSize(dataSize);
    }

    bool isNull() const
    {
        return bool(size() == 0);
    }

    bool isValid() const
    {
        if (isNull())
            return false;
        return QKnxNetIp::isStructType(code());
    }

    quint8 size() const
    {
        return m_bytes[0];
    }

    quint16 totalSize() const
    {
        if (size() == 2)
            return byte(0);

        if (size() == 4)
            return quint16(quint16(byte(1)) << 8 | byte(2));
        return 0;
    }

    quint16 dataSize() const
    {
        return totalSize() - size();
    }

    void setDataSize(quint16 dataSize)
    {
        // 2.1.3 Structures: If the amount of data exceeds 252 octets, the length octet shall
        // be 0xff and the next two octets shall contain the length as a 16 bit value. Then the
        // structure data shall start at the fifth octet.

        const auto tmp = code();
        setSize(dataSize > 0xfc ? 4 : 2);

        if (dataSize > 0xfc) {
            setByte(0, 0xff);
            setByte(1, quint8((dataSize + 4) >> 8));
            setByte(2, quint8(dataSize + 4));
        } else {
            setByte(0, quint8(dataSize + 2));
        }
        setCode(tmp);
    }

    CodeType code() const
    {
        if (size() == 0)
            return CodeType::Unknown;
        return CodeType(byte(size() - 1));
    }

    void setCode(CodeType code)
    {
        if (isNull()) {
            setSize(2);
            setByte(0, 2);
        }
        setByte(size() - 1, quint8(code));
    }

    QString toString() const
    {
        return QStringLiteral("Total size { 0x%1 }, Code { 0x%2 }")
            .arg(totalSize(), 2, 16, QLatin1Char('0')).arg(quint8(code()), 2, 16, QLatin1Char('0'));
    }

    quint8 byte(quint8 index) const
    {
        Q_ASSERT_X(index < size(), "QKnxNetIpStructHeader::byte", "index out of range");
        return m_bytes[index + 1];
    }

    QKnxByteArray bytes() const
    {
        if (!isNull()) {
            if (size() == 2)
                return { byte(0), byte(1) };

            if (size() == 4)
                return { byte(0), byte(1), byte(2), byte(3) };
        }
        return {};
    }

    static QKnxNetIpStructHeader fromBytes(const QKnxByteArray &bytes, quint16 index = 0)
    {
        const qint32 availableSize = bytes.size() - index;
        if (availableSize < 1)
            return {}; // total size missing

        quint16 totalSize = QKnxUtils::QUint8::fromBytes(bytes, index);
        const quint8 headerSize = totalSize == 0xff ? 4 : 2;
        if (availableSize < headerSize)
            return {}; // total size and code missing

        if (headerSize == 4)
            totalSize = QKnxUtils::QUint16::fromBytes(bytes, index + 1);

        auto code = CodeType(QKnxUtils::QUint8::fromBytes(bytes, index + headerSize - 1));
        if (!QKnxNetIp::isStructType(code))
            return {};
        return { code, quint16(totalSize - headerSize) };
    }

    bool operator==(const QKnxNetIpStructHeader &other) const
    {
        if (sizeof(m_bytes) != sizeof(other.m_bytes))
            return false;
        return memcmp(m_bytes, other.m_bytes, sizeof(m_bytes)) == 0;
    }

    bool operator!=(const QKnxNetIpStructHeader &other) const
    {
        return !operator==(other);
    }

private:
    void setSize(quint8 size)
    {
        m_bytes[0] = size;
    }

    void setByte(quint8 index, quint8 value)
    {
        m_bytes[index + 1] = value;
    }

private:
    // first byte denotes size, not part of the header
    quint8 m_bytes[5] { 0x00, 0x00, 0x00, 0x00, 0x00 };
};

QT_END_NAMESPACE

#endif
