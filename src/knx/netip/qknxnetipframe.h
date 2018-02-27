/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetippackage.h>

QT_BEGIN_NAMESPACE

using QKnxNetIpFrame = QKnxNetIpPackage<QKnxNetIp::ServiceType, QKnxNetIpFrameHeader>;

struct QKnxNetIpFrameHelper
{
    static QKnxNetIpFrame fromBytes(const QKnxByteArray &bytes, quint16 index, QKnxNetIp::ServiceType sType)
    {
        auto header = QKnxNetIpFrameHeader::fromBytes(bytes, index);
        if (!header.isValid() || header.code() != sType)
            return {};

        return QKnxNetIpFrame(header, QKnxNetIpPayload::fromBytes(bytes, index + header.size(),
            header.payloadSize()));
    }
};

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpFrame &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpFrame &package);

QT_END_NAMESPACE

#endif
