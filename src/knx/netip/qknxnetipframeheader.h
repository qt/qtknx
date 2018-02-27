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

#include <QtCore/qstring.h>

#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxnetip.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpFrameHeader final
{
    friend struct QKnxNetIpFrameHelper;

public:
    QKnxNetIpFrameHeader() = default;
    ~QKnxNetIpFrameHeader() = default;

    explicit QKnxNetIpFrameHeader(QKnxNetIp::ServiceType type);
    QKnxNetIpFrameHeader(QKnxNetIp::ServiceType type, quint16 dataSize);

    static constexpr const quint8 HeaderSize10 = 0x06;
    static constexpr const quint8 KnxNetIpVersion10 = 0x10;

    bool isNull() const;
    bool isValid() const;

    quint8 size() const;
    quint8 setSize(quint8 size);

    quint8 protocolVersion() const;
    void setProtocolVersion(quint8 version);

    quint16 totalSize() const;

    quint16 dataSize() const;
    void setDataSize(quint16 dataSize);

    QKnxNetIp::ServiceType serviceType() const;
    void setServiceType(QKnxNetIp::ServiceType type);

    QString toString() const;

    quint8 byte(quint8 index) const;
    QKnxByteArray bytes() const;

    static QKnxNetIpFrameHeader fromBytes(const QKnxByteArray &bytes, quint16 index = 0);

private:
    quint8 m_bytes[6] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
};

QT_END_NAMESPACE

#endif
