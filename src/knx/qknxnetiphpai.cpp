/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetiphpai.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpHPAI

    \inmodule QtKnx
    \brief The QKnxNetIpHPAI class represents the structure containing the
    information that is necessary to uniquely identify an Internet Protocol
    transport connection endpoint. It includes the Network Layer address
    and the Transport Layer identifier called Port number.
*/

QKnxNetIpHPAI::QKnxNetIpHPAI(const QHostAddress &address, quint16 port)
    : QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode::IpV4_Udp, address, port)
{}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QHostAddress &address,
        quint16 port)
    : QKnxNetIpStructure(quint8(hpc))
{
    auto data = QKnxUtils::HostAddress::toArray<QByteArray>(address
        .isNull() ? QHostAddress::LocalHost : address);
    data += QKnxUtils::Integer::quint16ToArray<QByteArray>(port);

    setData(data);
}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QByteArray &data)
    : QKnxNetIpStructure(quint8(hpc), data)
{
}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(hpc), data)
{
}

QKnxNetIpHPAI QKnxNetIpHPAI::fromRawData(const QByteArray &rawData, qint32 offset)
{
    return QKnxNetIpStructure::fromRawData(rawData, offset);
}

QKnxNetIpHPAI QKnxNetIpHPAI::fromRawData(const QVector<quint8> &rawData, qint32 offset)
{
    return QKnxNetIpStructure::fromRawData(rawData, offset);
}

QHostAddress QKnxNetIpHPAI::address() const
{
    return QKnxUtils::HostAddress::fromArray(data<QVector<quint8>>(0, 4));
}

quint16 QKnxNetIpHPAI::port() const
{
    return QKnxUtils::Integer::quint16FromArray(data<QVector<quint8>>(4, 2));
}

bool QKnxNetIpHPAI::isValid() const
{
    auto hpc = hostProtocolCode();
    return QKnxNetIpStructure::isValid() && dataSize() == 6
        && (hpc == HostProtocolCode::IpV4_Udp || hpc == HostProtocolCode::IpV4_Tcp);
}

QT_END_NAMESPACE
