/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxnetiphpai.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpHpai

    \inmodule QtKnx
    \brief The QKnxNetIpHpai class represents the structure containing the
    information that is necessary to uniquely identify an Internet Protocol
    transport connection endpoint. It includes the Network Layer address and
    the Transport Layer identifier called Port number.
*/

QKnxNetIpHpai::QKnxNetIpHpai(const QKnxNetIpHostProtocolStruct &other)
    : QKnxNetIpHostProtocolStruct(other)
{}

QKnxNetIpHpai::QKnxNetIpHpai(const QHostAddress &address, quint16 port)
    : QKnxNetIpHpai(QKnxNetIp::HostProtocol::IpV4_Udp, address, port)
{}

QKnxNetIpHpai::QKnxNetIpHpai(QKnxNetIp::HostProtocol hpc, const QHostAddress &address, quint16 port)
    : QKnxNetIpHostProtocolStruct(hpc)
{
    setHpai(hpc, address, port);
}

QKnxNetIp::HostProtocol QKnxNetIpHpai::hostProtocol() const
{
    return code();
}

void QKnxNetIpHpai::setHostProtocol(QKnxNetIp::HostProtocol code)
{
    if (isValid())
        setCode(code);
    else
        setHpai(code, QHostAddress(), 0);
}

QHostAddress QKnxNetIpHpai::address() const
{
    return QKnxUtils::HostAddress::fromBytes(payloadRef());
}

void QKnxNetIpHpai::setAddress(const QHostAddress &hostAddress)
{
    if (isValid())
        setHpai(hostProtocol(), hostAddress, port());
    else
        setHpai(QKnxNetIp::HostProtocol::IpV4_Udp, hostAddress, 0);
}

quint16 QKnxNetIpHpai::port() const
{
    return QKnxUtils::QUint16::fromBytes(payloadRef(), 4);
}

void QKnxNetIpHpai::setPort(quint16 port)
{
    if (isValid())
        setHpai(hostProtocol(), address(), port);
    else
        setHpai(QKnxNetIp::HostProtocol::IpV4_Udp, QHostAddress(), port);
}

void QKnxNetIpHpai::setHpai(QKnxNetIp::HostProtocol code, const QHostAddress &address, quint16 port)
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

bool QKnxNetIpHpai::isValid() const
{
    bool validHostProtocolCode = hostProtocol() == QKnxNetIp::HostProtocol::IpV4_Udp
        || hostProtocol() == QKnxNetIp::HostProtocol::IpV4_Tcp;
    return QKnxNetIpHostProtocolStruct::isValid() && size() == 8 && validHostProtocolCode;
}

QT_END_NAMESPACE
