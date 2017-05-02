/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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

class Q_KNX_EXPORT QKnxNetIpHPAI final : public QKnxNetIpHostProtocolStruct
{
public:
    QKnxNetIpHPAI() = default;
    ~QKnxNetIpHPAI() override = default;

    QKnxNetIpHPAI(const QHostAddress &address, quint16 port);
    QKnxNetIpHPAI(QKnxNetIp::HostProtocol hpc, const QHostAddress &address, quint16 port);

    template <typename T> static QKnxNetIpHPAI fromBytes(const T &bytes, quint16 index)
    {
        auto code = QKnxNetIpStructHeader<QKnxNetIp::HostProtocol>::fromBytes(bytes, index).code();
        if (!QKnxNetIp::isStructType(code))
            return {};
        return QKnxNetIpStructHelper::fromBytes(bytes, index, code);
    }

    QKnxNetIp::HostProtocol hostProtocol() const;
    void setHostProtocol(QKnxNetIp::HostProtocol code);

    QHostAddress address() const;
    void setAddress(QHostAddress hostAddress);

    quint16 port() const;
    void setPort(quint16 port);

    void setHPAI(QKnxNetIp::HostProtocol code, QHostAddress hostAddress, quint16 port);

    bool isValid() const override;

private:
    QKnxNetIpHPAI(const QKnxNetIpHostProtocolStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpHPAI, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
