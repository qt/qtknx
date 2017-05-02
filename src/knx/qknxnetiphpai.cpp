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
    : QKnxNetIpHPAI(QKnxNetIpHPAI::HostProtocol::IpV4_Udp, address, port)
{}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIpHPAI::HostProtocol hpc, const QHostAddress &address,
        quint16 port)
    : QKnxNetIpStruct(quint8(hpc))
{
    setHPAI(hpc, address, port);
}

QKnxNetIpHPAI::HostProtocol QKnxNetIpHPAI::hostProtocol() const
{
    return QKnxNetIpHPAI::HostProtocol(code());
}

void QKnxNetIpHPAI::setHostProtocol(QKnxNetIpHPAI::HostProtocol code)
{
    if (isValid())
        setCode(quint8(code));
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
        setHPAI(QKnxNetIpHPAI::HostProtocol::IpV4_Udp, hostAddress, 0);
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
        setHPAI(QKnxNetIpHPAI::HostProtocol::IpV4_Udp, QHostAddress(), port);
}

void QKnxNetIpHPAI::setHPAI(QKnxNetIpHPAI::HostProtocol code, QHostAddress address, quint16 port)
{
    setCode(quint8(code));
    QKnxNetIpPayload payload;
    payload.setBytes(QKnxUtils::HostAddress::bytes(address
        .isNull() ? QHostAddress::LocalHost : address));
    payload.appendBytes(QKnxUtils::QUint16::bytes(port));
    setPayload(payload);
}

bool QKnxNetIpHPAI::isValid() const
{
    bool validHostProtocolCode = hostProtocol() == QKnxNetIpHPAI::HostProtocol::IpV4_Udp
        || hostProtocol() == QKnxNetIpHPAI::HostProtocol::IpV4_Tcp;
    return QKnxNetIpStruct::isValid() && size() == 8 && validHostProtocolCode;
}

QT_END_NAMESPACE
