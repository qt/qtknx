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

#ifndef QKNXNETIPHPAI_H
#define QKNXNETIPHPAI_H

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

using QKnxNetIpHpai = QKnxNetIpStruct<QKnxNetIp::HostProtocol>;
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpHpai &package);

class Q_KNX_EXPORT QKnxNetIpHpaiView final
{
public:
    QKnxNetIpHpaiView() = delete;
    ~QKnxNetIpHpaiView() = default;

    QKnxNetIpHpaiView(const QKnxNetIpHpai &&) = delete;
    explicit QKnxNetIpHpaiView(const QKnxNetIpHpai &hpai);

    bool isValid() const;

    QKnxNetIp::HostProtocol hostProtocol() const;
    QHostAddress hostAddress() const;
    quint16 port() const;

    class Q_KNX_EXPORT Builder final
    {
    public:
        Builder &setHostProtocol(QKnxNetIp::HostProtocol code);
        Builder &setHostAddress(const QHostAddress &address);
        Builder &setPort(quint16 port);

        QKnxNetIpHpai create() const;

    private:
        QKnxNetIp::HostProtocol m_code { QKnxNetIp::HostProtocol::UDP_IPv4 };
        QHostAddress m_address { QHostAddress::LocalHost };
        quint16 m_port { 0 };
    };
    static QKnxNetIpHpaiView::Builder builder();

private:
     const QKnxNetIpHpai &m_hpai;
};

QT_END_NAMESPACE

#endif
