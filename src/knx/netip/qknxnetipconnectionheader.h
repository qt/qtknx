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

#ifndef QKNXNETIPCONNECTIONHEADER_H
#define QKNXNETIPCONNECTIONHEADER_H

#include <QtCore/qstring.h>

#include <QtKnx/qknxbytearray.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConnectionHeader final
{
public:
    QKnxNetIpConnectionHeader() = default;
    ~QKnxNetIpConnectionHeader() = default;

    QKnxNetIpConnectionHeader(quint8 channelId, quint8 seqNumber);
    QKnxNetIpConnectionHeader(quint8 channelId, quint8 seqNumber, quint8 serviceTypeSpecificValue);

    bool isNull() const;
    bool isValid() const;

    quint8 size() const;
    QString toString() const;

    quint8 channelId() const;
    void setChannelId(quint8 channelId);

    quint8 sequenceNumber() const;
    void setSequenceNumber(quint8 seqNumber);

    quint8 serviceTypeSpecificValue() const;
    void setServiceTypeSpecificValue(quint8 value);

    QKnxByteArray connectionTypeSpecificHeaderItems() const;
    void setConnectionTypeSpecificHeaderItems(const QKnxByteArray &items);

    quint8 byte(quint8 index) const;
    QKnxByteArray bytes() const;

    static QKnxNetIpConnectionHeader fromBytes(const QKnxByteArray &bytes, quint16 index = 0);

    bool operator==(const QKnxNetIpConnectionHeader &other) const;
    bool operator!=(const QKnxNetIpConnectionHeader &other) const;

private:
    QKnxByteArray m_bytes { 0x00, 0x00, 0x00, 0x00 };
};

QT_END_NAMESPACE

#endif
