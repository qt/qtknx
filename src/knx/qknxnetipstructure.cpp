/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipstructure.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpStructure

    \inmodule QtKnx
    \brief The QKnxNetIpStructure class represents the base for all KNXnet/IP
    structures.
*/

/*!
    TODO: Add description the enum values.

    \enum QKnxNetIpStructure::HostProtocolCode

    \value IpV4_Udp
    \value IpV4_Tcp

    \enum QKnxNetIpStructure::ConnectionTypeCode

    \value DeviceManagementConnection
    \value TunnelConnection
    \value RemoteLoggingConnection
    \value RemoteConfigurationConnection
    \value ObjectServerConnection

    \enum QKnxNetIpStructure::DescriptionTypeCode

    \value DeviceInfo
    \value SupportedServiceFamilies
    \value IpConfiguration
    \value CurrentIpConfiguration
    \value KnxAddresses
    \value ManufactorData
    \value NotUsed
*/

/*!
    Sets the size of the KNXnet/IP structure to \a size. If \a size is greater
    than the current size, elements are added to the end; the new elements are
    initialized with a \l{default-constructed value}. If \a size is less than
    the current size, elements are removed from the end.

    \note This function shall always be called for Description Information Block
    structures with \a makeEven set to \c true, see for reference:

    03_08_02 Core v01.05.01 AS.pdf paragraph 7.5.4.1: Use, format and general
    requirements: The structure shall always have an even number of octets which
    may have to be achieved by padding with 00h in the last octet of the structure.

*/
void QKnxNetIpStructure::resize(int size, bool makeEven)
{
    if (makeEven)
        size += size % 2;

    if (m_size != size) {
        m_size = size;
        m_data.resize(size);
    }
}

/*!
    Returns the number of bytes representing the KNXnet/IP structure's data,
    excluding the bytes for the structure size and code.
*/
qint32 QKnxNetIpStructure::dataSize() const
{
    return m_size;
}

/*!
    Returns the number of bytes representing the KNXnet/IP structure, including
    the bytes for the structure size and code.
*/
qint32 QKnxNetIpStructure::rawSize() const
{
    return m_size > 0xfc ? m_size + 4 : m_size + 2;
}

/*!
    Returns the KNXnet/IP raw size. generic code and the data as string. Code,
    raw size and data are formatted in hexadecimal notation.
*/
QString QKnxNetIpStructure::toString() const
{
    QString data;
    for (quint8 byte : qAsConst(m_data))
        data += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
    data.chop(2);

    return QStringLiteral("Raw size { 0x%1 }, Code { 0x%2 }, Data { %3 }")
        .arg(rawSize(), 2, 16, QLatin1Char('0')).arg(m_code, 2, 16, QLatin1Char('0')).arg(data);
}

/*!
    \relates QKnxNetIpStructure

    Writes the KNXnet/IP \a structure to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpStructure &structure)
{
    QDebugStateSaver _(debug);
    QDebug &dbg = debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar('0');
    const auto rawData = structure.rawData<QVector<quint8>>();
    for (quint8 byte : qAsConst(rawData))
        dbg << byte;
    return debug;
}

/*!
    \relates QKnxNetIpStructure

    Writes the KNXnet/IP \a structure to the \a stream and  returns a reference
    to the \a stream.
*/
QDataStream &operator<<(QDataStream &stream, const QKnxNetIpStructure &structure)
{
    for (quint8 byte : structure.rawData<QByteArray>())
        stream << static_cast<quint8> (byte);
    return stream;
}

QT_END_NAMESPACE
