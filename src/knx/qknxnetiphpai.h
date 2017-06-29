/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXNETIPHPAI_H
#define QKNXNETIPHPAI_H

#include <QtCore/qbytearray.h>
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

    template <typename T> static QKnxNetIpHpai fromBytes(const T &bytes, quint16 index)
    {
        auto code = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>::fromBytes(bytes, index).code();
        if (!QKnxNetIp::isStructType(code))
            return {};
        return QKnxNetIpStructHelper::fromBytes(bytes, index, code);
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
