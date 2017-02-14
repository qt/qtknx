/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetiphpai.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpHPAI

    \inmodule QtKnx
    \brief The QKnxNetIpHPAI class represents the structure containing the
    information that is necessary to uniquely identify an Internet Protocol
    transport connection endpoint. It includes the Network Layer address
    and the Transport Layer identifier called Port number.
*/

QKnxNetIpHPAI::QKnxNetIpHPAI(const QByteArray &rawData, qint32 offset)
{
    qint32 availableSize = rawData.size() - offset;
    if (availableSize <= 2) // at least size and code are to be expected
        return;

    auto hpc = QKnxNetIpStructure::HostProtocolCode(rawData[offset + 1]);
    if (hpc == QKnxNetIpStructure::HostProtocolCode::IpV4_Udp
        || hpc == QKnxNetIpStructure::HostProtocolCode::IpV4_Tcp) {
        setRawData(quint8(hpc), rawData, offset);
    }
    resize(6); // size enforced 8.6.2 Host Protocol Address Information
}

QKnxNetIpHPAI::QKnxNetIpHPAI(const QVector<quint8> &rawData, qint32 offset)
{
    qint32 availableSize = rawData.size() - offset;
    if (availableSize <= 2) // at least size and code are to be expected
        return;

    auto hpc = QKnxNetIpStructure::HostProtocolCode(rawData[offset + 1]);
    if (hpc == QKnxNetIpStructure::HostProtocolCode::IpV4_Udp
        || hpc == QKnxNetIpStructure::HostProtocolCode::IpV4_Tcp) {
        setRawData(quint8(hpc), rawData, offset);
    }
    resize(6); // size enforced 8.6.2 Host Protocol Address Information
}

QKnxNetIpHPAI::QKnxNetIpHPAI(const QHostAddress &address, quint16 port)
    : QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode::IpV4_Udp, address, port)
{
}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QHostAddress &address,
    quint16 port)
{
    QByteArray rawData(6, Qt::Uninitialized);
    auto addr = (address.isNull() ? QHostAddress::LocalHost : address).toIPv4Address();
    rawData[0] = quint8(addr >> 24);
    rawData[1] = quint8(addr >> 16);
    rawData[2] = quint8(addr >> 8);
    rawData[3] = quint8(addr);
    rawData[4] = quint8(port >> 8);
    rawData[5] = quint8(port);
    setRawData(quint8(hpc), rawData);
}

QHostAddress QKnxNetIpHPAI::address() const
{
    auto address = data<QVector<quint8>>(0, 4);
    return QHostAddress(quint32(address[0] << 24 | address[1] << 16 | address[2] << 8 | address[3]));
}

quint16 QKnxNetIpHPAI::port() const
{
    auto port = data<QVector<quint8>>(4, 2);
    return quint16(port[0] << 8 | port[1]);
}

QT_END_NAMESPACE
