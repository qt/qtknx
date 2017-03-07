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
    transport connection endpoint. It includes the Network Layer address and
    the Transport Layer identifier called Port number.
*/

QKnxNetIpHPAI::QKnxNetIpHPAI(const QKnxNetIpStruct &other)
    : QKnxNetIpStruct(other)
{}

QKnxNetIpHPAI::QKnxNetIpHPAI(const QHostAddress &address, quint16 port)
    : QKnxNetIpHPAI(QKnxNetIp::HostProtocolCode::IpV4_Udp, address, port)
{}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIp::HostProtocolCode hpc, const QHostAddress &address,
        quint16 port)
    : QKnxNetIpStruct(quint8(hpc))
{
    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::HostAddress::bytes<std::vector<quint8>>(address
        .isNull() ? QHostAddress::LocalHost : address));
    payload.appendBytes(QKnxUtils::QUint16::bytes<std::vector<quint8>>(port));
    setPayload(payload);
}

QKnxNetIp::HostProtocolCode QKnxNetIpHPAI::hostProtocolCode() const
{
    return QKnxNetIp::HostProtocolCode(code());
}

void QKnxNetIpHPAI::setHostProtocolCode(QKnxNetIp::HostProtocolCode code)
{
    setCode(quint8(code));
}

QHostAddress QKnxNetIpHPAI::address() const
{
    return QKnxUtils::HostAddress::fromBytes(payload().bytes<std::vector<quint8>>(0, 4));
}

quint16 QKnxNetIpHPAI::port() const
{
    return QKnxUtils::QUint16::fromBytes(payload().bytes<std::vector<quint8>>(4, 2));
}

bool QKnxNetIpHPAI::isValid() const
{
    bool validHostProtocolCode = hostProtocolCode() == QKnxNetIp::HostProtocolCode::IpV4_Udp
        || hostProtocolCode() == QKnxNetIp::HostProtocolCode::IpV4_Tcp;
    return QKnxNetIpStruct::isValid() && size() == 8 && validHostProtocolCode;
}

QT_END_NAMESPACE
