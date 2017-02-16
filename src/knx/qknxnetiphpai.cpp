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

QKnxNetIpHPAI::QKnxNetIpHPAI(const QHostAddress &address, quint16 port)
    : QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode::IpV4_Udp, address, port)
{
}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QHostAddress &address,
        quint16 port)
    : QKnxNetIpStructure(quint8(hpc), 6)
{
    QByteArray data(6, Qt::Uninitialized);
    auto addr = (address.isNull() ? QHostAddress::LocalHost : address).toIPv4Address();
    data[0] = quint8(addr >> 24);
    data[1] = quint8(addr >> 16);
    data[2] = quint8(addr >> 8);
    data[3] = quint8(addr);
    data[4] = quint8(port >> 8);
    data[5] = quint8(port);
    setData(quint8(hpc), data);
}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QByteArray &data)
    : QKnxNetIpStructure(quint8(hpc), data)
{
    resizeData(6); // size enforced 8.6.2 Host Protocol Address Information
}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(hpc), data)
{
    resizeData(6); // size enforced 8.6.2 Host Protocol Address Information
}

QKnxNetIpHPAI QKnxNetIpHPAI::fromRawData(const QByteArray &rawData, qint32 offset)
{
    QKnxNetIpHPAI hpai;
    qint32 availableSize = rawData.size() - offset;
    if (availableSize <= 2) // at least size and code are to be expected
        return hpai;

    auto hpc = QKnxNetIpStructure::HostProtocolCode(rawData[offset + 1]);
    if (hpc == QKnxNetIpStructure::HostProtocolCode::IpV4_Udp
        || hpc == QKnxNetIpStructure::HostProtocolCode::IpV4_Tcp) {
        hpai.setRawData(quint8(hpc), rawData, offset);
        hpai.resizeData(6); // size enforced 8.6.2 Host Protocol Address Information
    }
    return hpai;
}

QKnxNetIpHPAI QKnxNetIpHPAI::fromRawData(const QVector<quint8> &rawData, qint32 offset)
{
    QKnxNetIpHPAI hpai;
    qint32 availableSize = rawData.size() - offset;
    if (availableSize <= 2) // at least size and code are to be expected
        return hpai;

    auto hpc = QKnxNetIpStructure::HostProtocolCode(rawData[offset + 1]);
    if (hpc == QKnxNetIpStructure::HostProtocolCode::IpV4_Udp
        || hpc == QKnxNetIpStructure::HostProtocolCode::IpV4_Tcp) {
        hpai.setRawData(quint8(hpc), rawData, offset);
        hpai.resizeData(6); // size enforced 8.6.2 Host Protocol Address Information
    }
    return hpai;
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
