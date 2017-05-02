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

class Q_KNX_EXPORT QKnxNetIpHPAI final : public QKnxNetIpStruct
{
public:
    enum class HostProtocol : quint8
    {
        IpV4_Udp = 0x01,
        IpV4_Tcp = 0x02
    };

    QKnxNetIpHPAI() = default;
    ~QKnxNetIpHPAI() override = default;

    QKnxNetIpHPAI(const QHostAddress &address, quint16 port);
    QKnxNetIpHPAI(QKnxNetIpHPAI::HostProtocol hpc, const QHostAddress &address, quint16 port);

    template <typename T> static QKnxNetIpHPAI fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

    QKnxNetIpHPAI::HostProtocol hostProtocol() const;
    void setHostProtocol(QKnxNetIpHPAI::HostProtocol code);

    QHostAddress address() const;
    void setAddress(QHostAddress hostAddress);

    quint16 port() const;
    void setPort(quint16 port);

    void setHPAI(QKnxNetIpHPAI::HostProtocol code, QHostAddress hostAddress, quint16 port);

    bool isValid() const override;

private:
    QKnxNetIpHPAI(const QKnxNetIpStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpHPAI, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIpHPAI::HostProtocol, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
