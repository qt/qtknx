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
    explicit QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QByteArray &data)
        : QKnxNetIpStructure(quint8(hpc), data)
    {
        resize(6); // size enforced 8.6.2 Host Protocol Address Information
    }
    explicit QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QVector<quint8> &data)
        : QKnxNetIpStructure(quint8(hpc), data)
    {
        resize(6); // size enforced 8.6.2 Host Protocol Address Information
    }

    QKnxNetIpHPAI(const QByteArray &rawData, qint32 offset);
    QKnxNetIpHPAI(const QVector<quint8> &rawData, qint32 offset);

    QKnxNetIpHPAI(const QHostAddress &address, quint16 port);
    QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QHostAddress &address, quint16 port);

    QHostAddress address() const;
    quint16 port() const;

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::hostProtocolCode;
};
Q_DECLARE_TYPEINFO(QKnxNetIpHPAI, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
