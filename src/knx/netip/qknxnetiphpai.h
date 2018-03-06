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

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpHpai final : public QKnxNetIpHostProtocolStruct
{
public:
    QKnxNetIpHpai() = default;
    ~QKnxNetIpHpai() override = default;

    QKnxNetIpHpai(const QHostAddress &address, quint16 port);
    QKnxNetIpHpai(QKnxNetIp::HostProtocol hpc, const QHostAddress &address, quint16 port);

    static QKnxNetIpHpai fromBytes(const QKnxByteArray &bytes, quint16 index)
    {
        auto code = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>::fromBytes(bytes, index).code();
        if (!QKnxNetIp::isStructType(code))
            return {};
        return QKnxNetIpStruct::fromBytes(bytes, index, code);
    }

    QKnxNetIp::HostProtocol hostProtocol() const;
    void setHostProtocol(QKnxNetIp::HostProtocol code);

    QHostAddress address() const;
    void setAddress(const QHostAddress &hostAddress);

    quint16 port() const;
    void setPort(quint16 port);

    void setHpai(QKnxNetIp::HostProtocol code, const QHostAddress &hostAddress, quint16 port);

    bool isValid() const override;

private:
    QKnxNetIpHpai(const QKnxNetIpHostProtocolStruct &other);
};

QT_END_NAMESPACE

#endif
