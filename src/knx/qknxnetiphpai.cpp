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
{
}

QKnxNetIpHPAI::QKnxNetIpHPAI(QKnxNetIpStructure::HostProtocolCode hpc, const QHostAddress &address,
        quint16 port)
    : QKnxNetIpStructure(quint8(hpc), 6)
{
    QByteArray data(0, Qt::Uninitialized);
    auto addr = address.isNull() ? QHostAddress::LocalHost : address;
    data.insert(0, QKnxUtils::HostAddress::toArray<QByteArray>(addr));
    data.insert(4, QKnxUtils::Integer::quint16ToArray<QByteArray>(port));
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
    hpai.resizeData(6); // size enforced 8.6.2 Host Protocol Address Information
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
    hpai.resizeData(6); // size enforced 8.6.2 Host Protocol Address Information
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
    return QKnxUtils::HostAddress::fromArray(data<QVector<quint8>>(0, 4));
}

quint16 QKnxNetIpHPAI::port() const
{
    return QKnxUtils::Integer::quint16FromArray(data<QVector<quint8>>(4, 2));
}

QT_END_NAMESPACE
