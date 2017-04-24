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
    QKnxNetIpHPAI() = default;
    ~QKnxNetIpHPAI() override = default;

    QKnxNetIpHPAI(const QHostAddress &address, quint16 port);
    QKnxNetIpHPAI(QKnxNetIp::HostProtocolCode hpc, const QHostAddress &address, quint16 port);

    template <typename T> static QKnxNetIpHPAI fromBytes(const T &bytes, qint32 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

    QKnxNetIp::HostProtocolCode hostProtocolCode() const;
    void setHostProtocolCode(QKnxNetIp::HostProtocolCode code);

    QHostAddress address() const;
    quint16 port() const;

    bool isValid() const override;

private:
    QKnxNetIpHPAI(const QKnxNetIpStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpHPAI, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
