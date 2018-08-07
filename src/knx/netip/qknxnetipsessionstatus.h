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

#ifndef QKNXNETIPSESSIONSTATUS_H
#define QKNXNETIPSESSIONSTATUS_H

#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetip.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpSessionStatusProxy final
{
public:
    QKnxNetIpSessionStatusProxy() = delete;
    ~QKnxNetIpSessionStatusProxy() = default;

    QKnxNetIpSessionStatusProxy(const QKnxNetIpFrame &&) = delete;
    explicit QKnxNetIpSessionStatusProxy(const QKnxNetIpFrame &frame);

    bool isValid() const;
    QKnxNetIp::SecureSessionStatus status() const;

    class Q_KNX_EXPORT Builder final
    {
    public:
        Builder &setStatus(QKnxNetIp::SecureSessionStatus status);

        QKnxNetIpFrame create() const;

    private:
        QKnxNetIp::SecureSessionStatus m_status = QKnxNetIp::SecureSessionStatus::Unknown;
    };
    static QKnxNetIpSessionStatusProxy::Builder builder();

private:
    const QKnxNetIpFrame &m_frame;
};

QT_END_NAMESPACE

#endif
