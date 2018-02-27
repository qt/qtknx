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

#ifndef QKNXNETIPFRAMEHEADER_H
#define QKNXNETIPFRAMEHEADER_H

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxbytestore.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpFrameHeader final : private QKnxByteStore
{
    friend struct QKnxNetIpFrameHelper;

public:
    QKnxNetIpFrameHeader() = default;
    ~QKnxNetIpFrameHeader() override = default;

    explicit QKnxNetIpFrameHeader(QKnxNetIp::ServiceType code);
    QKnxNetIpFrameHeader(QKnxNetIp::ServiceType code, quint16 payloadSize);

    bool isValid() const;
    quint16 totalSize() const;

    quint8 protocolVersion() const;

    quint16 payloadSize() const;
    void setPayloadSize(quint16 payloadSize);

    QKnxNetIp::ServiceType code() const;
    void setCode(QKnxNetIp::ServiceType code);

    QString toString() const override;

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;

    static constexpr const quint8 HeaderSize10 = 0x06;
    static constexpr const quint8 KnxNetIpVersion10 = 0x10;

    static QKnxNetIpFrameHeader fromBytes(const QKnxByteArray &bytes, quint16 index)
    {
        const qint32 availableSize = bytes.size() - index;
        if (availableSize < QKnxNetIpFrameHeader::HeaderSize10)
            return {};

        if (QKnxUtils::QUint8::fromBytes(bytes, index + 1) != QKnxNetIpFrameHeader::KnxNetIpVersion10)
            return {};

        const quint16 code = QKnxUtils::QUint16::fromBytes(bytes, index + 2);
        if (!QKnxNetIp::isFrameType(QKnxNetIp::ServiceType(code)))
            return {};

        return QKnxNetIpFrameHeader(QKnxNetIp::ServiceType(code),
            QKnxUtils::QUint16::fromBytes(bytes, index + 4) - QKnxNetIpFrameHeader::HeaderSize10);
    }
};

QT_END_NAMESPACE

#endif
