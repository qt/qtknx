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

#ifndef QKNXNETIPFRAME_H
#define QKNXNETIPFRAME_H

#include <QtCore/qshareddata.h>

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetippayload.h>
#include <QtKnx/qknxnetipconnectionheader.h>
#include <QtKnx/qknxnetipframeheader.h>

QT_BEGIN_NAMESPACE

class QKnxNetIpFrameExPrivate;
class Q_KNX_EXPORT QKnxNetIpFrameEx
{
public:
    QKnxNetIpFrameEx();
    ~QKnxNetIpFrameEx();

    QKnxNetIpFrameEx(QKnxNetIp::ServiceType type, const QKnxByteArray &data = {});
    QKnxNetIpFrameEx(QKnxNetIp::ServiceType type,
        const QKnxNetIpConnectionHeader &connectionHeader, const QKnxByteArray &data = {});
    QKnxNetIpFrameEx(const QKnxNetIpFrameHeader &header,
        const QKnxNetIpConnectionHeader &connectionHeader, const QKnxByteArray &data = {});

    bool isNull()const;
    bool isValid() const;

    quint16 size() const;
    quint16 dataSize() const;

    QKnxNetIpFrameHeader header() const;
    void setHeader(const QKnxNetIpFrameHeader &header);

    quint8 protocolVersion() const;

    QKnxNetIp::ServiceType serviceType() const;
    void setServiceType(QKnxNetIp::ServiceType type);

    QKnxNetIpConnectionHeader connectionHeader() const;
    void setConnectionHeader(const QKnxNetIpConnectionHeader &header);

    quint8 channelId() const;
    quint8 sequenceNumber() const;
    quint8 serviceTypeSpecificValue() const;
    QKnxByteArray connectionTypeSpecificHeaderItems() const;

    QKnxByteArray data() const;
    const QKnxByteArray &constData() const;
    void setData(const QKnxByteArray &data);

    QKnxByteArray bytes() const;
    static QKnxNetIpFrameEx fromBytes(const QKnxByteArray &bytes, quint16 index = 0);

    QKnxNetIpFrameEx(const QKnxNetIpFrameEx &other);
    QKnxNetIpFrameEx &operator=(const QKnxNetIpFrameEx &other);

    QKnxNetIpFrameEx(QKnxNetIpFrameEx &&other) Q_DECL_NOTHROW;
    QKnxNetIpFrameEx &operator=(QKnxNetIpFrameEx &&other) Q_DECL_NOTHROW;

    void swap(QKnxNetIpFrameEx &other) Q_DECL_NOTHROW;

    bool operator==(const QKnxNetIpFrameEx &other) const;
    bool operator!=(const QKnxNetIpFrameEx &other) const;

private:
    explicit QKnxNetIpFrameEx(QKnxNetIpFrameExPrivate &dd);

private:
    QSharedDataPointer<QKnxNetIpFrameExPrivate> d_ptr;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpFrameEx &frame);

class Q_KNX_EXPORT QKnxNetIpFrame
{
    friend struct QKnxNetIpFrameHelper;
    friend struct QKnxNetIpConnectionHeaderFrameHelper;

public:
    virtual quint16 size() const
    {
        return m_header.totalSize();
    }

    virtual QKnxNetIpFrameHeader header() const
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

    QKnxByteArray bytes() const
    {
        return m_header.bytes() + m_payload.bytes();
    }

    virtual ~QKnxNetIpFrame() = default;

protected:
    QKnxNetIpFrame() = default;

    explicit QKnxNetIpFrame(QKnxNetIp::ServiceType code)
        : m_header(code)
    {}

    QKnxNetIpFrame(const QKnxNetIpFrameHeader &header, const QKnxNetIpPayload &payload)
        : m_header(header)
        , m_payload(payload)
    {}

    QKnxNetIp::ServiceType code() const
    {
        return m_header.serviceType();
    }

    void setCode(QKnxNetIp::ServiceType type)
    {
        m_header.setServiceType(type);
    }

    quint16 dataSize() const
    {
        return m_header.dataSize();
    }

    void setDataSize(quint16 dataSize)
    {
        m_header.setDataSize(dataSize);
    }

    virtual void setPayload(const QKnxNetIpPayload &payload)
    {
        m_payload = payload;
        setDataSize(payload.size());
    }

private:
    QKnxNetIpFrameHeader m_header;
    QKnxNetIpPayload m_payload;
};

struct QKnxNetIpFrameHelper
{
    static QKnxNetIpFrame fromBytes(const QKnxByteArray &bytes, quint16 index, QKnxNetIp::ServiceType sType)
    {
        auto header = QKnxNetIpFrameHeader::fromBytes(bytes, index);
        if (!header.isValid() || header.serviceType() != sType)
            return {};

        return QKnxNetIpFrame(header, QKnxNetIpPayload::fromBytes(bytes, index + header.size(),
            header.dataSize()));
    }
};

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpFrame &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpFrame &package);

QT_END_NAMESPACE

#endif
