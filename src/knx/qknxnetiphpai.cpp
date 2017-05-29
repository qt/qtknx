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

QKnxNetIpHPAI::QKnxNetIpHPAI(const QKnxNetIpHostProtocolStruct &other)
    : QKnxNetIpHostProtocolStruct(other)
{}

QKnxNetIpHPAI::QKnxNetIpHPAI(const QHostAddress &address, quint16 port)
    : QKnxNetIpHPAI(QKnxNetIp::HostProtocol::IpV4_Udp, address, port)
{}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIp::HostProtocol hpc, const QHostAddress &address, quint16 port)
    : QKnxNetIpHostProtocolStruct(hpc)
{
    setHPAI(hpc, address, port);
}

QKnxNetIp::HostProtocol QKnxNetIpHPAI::hostProtocol() const
{
    return QKnxNetIp::HostProtocol(code());
}

void QKnxNetIpHPAI::setHostProtocol(QKnxNetIp::HostProtocol code)
{
    if (isValid())
        setCode(code);
    else
        setHPAI(code, QHostAddress(), 0);
}

QHostAddress QKnxNetIpHPAI::address() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef());
}

void QKnxNetIpHPAI::setAddress(QHostAddress hostAddress)
{
    if (isValid())
        setHPAI(hostProtocol(), hostAddress, port());
    else
        setHPAI(QKnxNetIp::HostProtocol::IpV4_Udp, hostAddress, 0);
}

quint16 QKnxNetIpHPAI::port() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef(), 4);
}

void QKnxNetIpHPAI::setPort(quint16 port)
{
    if (isValid())
        setHPAI(hostProtocol(), address(), port);
    else
        setHPAI(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress(), port);
}

void QKnxNetIpHPAI::setHPAI(QKnxNetIp::HostProtocol code, QHostAddress address, quint16 port)
{
    setCode(code);
    QKnxNetIpPayload payload;

    // TODO: Review this part - It might make more sense to set the address to AnyIPv4
    // to indicate NAT traversal, see for example 8.6.3.5 Network Address Translation (NAT)
    payload.setBytes(QKnxUtils::HostAddress::bytes(address
        .isNull() ? QHostAddress::LocalHost : address));

    payload.appendBytes(QKnxUtils::QUint16::bytes(port));
    setPayload(payload);
}

bool QKnxNetIpHPAI::isValid() const
{
    bool validHostProtocolCode = hostProtocol() == QKnxNetIp::HostProtocol::IpV4_Udp
        || hostProtocol() == QKnxNetIp::HostProtocol::IpV4_Tcp;
    return QKnxNetIpHostProtocolStruct::isValid() && size() == 8
        && validHostProtocolCode;
}

QT_END_NAMESPACE
