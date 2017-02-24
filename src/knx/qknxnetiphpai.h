/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstructure.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpHPAI final : public QKnxNetIpStructure
{
public:
    QKnxNetIpHPAI() = default;

    QKnxNetIpHPAI(const QHostAddress &address, quint16 port);
    QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QHostAddress &address, quint16 port);

    QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QByteArray &data);
    QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QVector<quint8> &data);

    static QKnxNetIpHPAI fromRawData(const QByteArray &rawData, qint32 offset);
    static QKnxNetIpHPAI fromRawData(const QVector<quint8> &rawData, qint32 offset);

    QHostAddress address() const;
    quint16 port() const;

    bool isValid() const;

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::hostProtocolCode;

private:
    QKnxNetIpHPAI(const QKnxNetIpStructure &other)
        : QKnxNetIpStructure(other)
    {}
};
Q_DECLARE_TYPEINFO(QKnxNetIpHPAI, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
