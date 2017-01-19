/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxadditionalinfo.h"

#include <array>

QT_BEGIN_NAMESPACE

/*!
    \class QKnxAdditionalInfo

    \inmodule QtKnx
    \brief The QKnxAdditionalInfo class represents the additional information
    that can be placed inside a KNX cEMI frame.
*/

/*!
    \enum QKnxAdditionalInfo::Type

    \omitvalue Reserved
    \value PlMediumInformation          Domain Address used by PL medium.
    \value RfMediumInformation          RF-Info byte (formerly named
                                        RF-Ctrl) and KNX Serial Number/DoA
                                        and Data Link Layer Frame Number (LFN).
    \value BusmonitorStatusInfo         Busmonitor Error Flags.
    \value TimestampRelative            Relative timestamp; e.g. for L_Raw.ind.
    \value TimeDelayUntilSending        Time delay; e.g. for L_Raw.req.
    \value ExtendedRelativeTimestamp    Device independent time stamp, e.g.
                                        for L_Raw.ind or L_Busmon.ind.
    \value BiBatInformation             Contains b7-b4 of the RF KNX-Ctrl
                                        field and BiBat Block-number.
    \value RfMultiInformation           RF Multi frequency, call channel and
                                        Fast Ack number.
    \value PreambleAndPostamble         Preamble and postamble length.
    \value RfFastAckInformation         Status and information about each
                                        expected number of Fast Ack (N)
    \value ManufactorSpecificData       Manufacturer specific data, including
                                        manufacturer ID (2 byte) and
                                        Subfunction ID (1 byte).
    \omitvalue EscCode
*/

/*!
    Returns the additional info \l Type.
*/
QKnxAdditionalInfo::Type QKnxAdditionalInfo::type() const
{
    return m_type;
}

/*!
    \fn QKnxAdditionalInfo::QKnxAdditionalInfo()

    Constructs an new, empty, invalid additional info object.
*/

/*!
    Constructs an new additional info object and sets its \l Type to \a type and
    data to \a data.
*/
QKnxAdditionalInfo::QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QByteArray &data)
{
    if (isValid(type, data)) {
        m_type = type;
        m_data.resize(data.size());
        std::copy(data.constBegin(), data.constEnd(), m_data.begin());
    }
}

/*!
    Constructs an new additional info object and sets its \l Type to \a type and
    data to \a data.
*/
QKnxAdditionalInfo::QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QVector<quint8> &data)
{
    if (isValid(type, data)) {
        m_type = type;
        m_data = data;
    }
}

/*!
    \overload

    Returns \c true if this is a valid additional info object; \c false
    otherwise.
*/
bool QKnxAdditionalInfo::isValid() const
{
    return isValid(m_type, m_data);
}

/*!
    \fn bool QKnxAdditionalInfo::isValid(QKnxAdditionalInfo::Type type, const T &data)

    Returns \c true if \a type is one of the know enumeration in \l Type and
    \a data has the expected length for \a type.

    \note The Additional Information field included in a cEMI message cannot
    contain a value larger then 254, value 255 is reserved for future extension.
    So the maximum valid data size cannot exceed 252 bytes, the two remaining
    bytes are reserved for the \l Type and length information.

    \sa expectedDataSize()
*/

/*!
    Returns the number of bytes representing the additional info, including the
    byte for \l Type id and the byte for length information.
*/
qint32 QKnxAdditionalInfo::size() const
{
    if (m_data.isEmpty())
        return 0;
    return m_data.size() + 2;
}

/*!
    Returns the number of bytes representing the additional info, excluding the
    byte for \l Type id and the byte for length information.
*/
qint32 QKnxAdditionalInfo::dataSize() const
{
    return m_data.size();
}

/*!
    Returns the number of expected bytes for \l Type id \a type. The additional
    \c bool parameter \a isFixedSize is set to \c true if the \a type expects
    a fixed size of bytes; otherwise to false. If the \a type is unknown, the
    function will return a negative value.

    Know types of variable size are:
    \list
        \li \l QKnxAdditionalInfo::RfFastAckInformation - a multiple of two
               bytes, two bytes minimum.
        \li \l QKnxAdditionalInfo::ManufactorSpecificData - three bytes minimum.
    \endlist
*/
qint32 QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type type, bool *isFixedSize)
{
    static const std::array<int, 0x100> table = []() {
        std::array<int, 0x100> table; table.fill(-1);
        table[int(QKnxAdditionalInfo::Type::PlMediumInformation)] = 2;
        table[int(QKnxAdditionalInfo::Type::RfMediumInformation)] = 8;
        table[int(QKnxAdditionalInfo::Type::BusmonitorStatusInfo)] = 1;
        table[int(QKnxAdditionalInfo::Type::TimestampRelative)] = 2;
        table[int(QKnxAdditionalInfo::Type::TimeDelayUntilSending)] = 4;
        table[int(QKnxAdditionalInfo::Type::ExtendedRelativeTimestamp)] = 4;
        table[int(QKnxAdditionalInfo::Type::BiBatInformation)] = 2;
        table[int(QKnxAdditionalInfo::Type::RfMultiInformation)] = 4;
        table[int(QKnxAdditionalInfo::Type::PreambleAndPostamble)] = 3;
        table[int(QKnxAdditionalInfo::Type::RfFastAckInformation)] = 2;
        table[int(QKnxAdditionalInfo::Type::ManufactorSpecificData)] = 3;
        return table;
    }();
    if (isFixedSize)
        *isFixedSize = (type < QKnxAdditionalInfo::Type::RfFastAckInformation);
    return table[int(type)];
}

/*!
    Returns the additional info's type, length and the data as string. Type,
    length and data are formatted in hexadecimal notation. If the additional
    info is invalid, an empty string is returned.
*/
QString QKnxAdditionalInfo::toString() const
{
    if (!isValid())
        return QString();

    QString data;
    foreach (quint8 byte, m_data)
        data += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
    data.chop(2);

    return QStringLiteral("Type { 0x%1 }, Size { 0x%2 }, Data { %3 }")
        .arg(static_cast<quint8> (m_type), 2, 16, QLatin1Char('0'))
        .arg(m_data.size(), 2, 16, QLatin1Char('0')).arg(data);
}

/*!
    \fn auto QKnxAdditionalInfo::rawData() const

    Returns the additional info as vector of bytes if the information is valid;
    otherwise an empty vector. The vector includes the type id, the size of the
    actual data and the data itself.

    \note Only QByteArray and QVector<quint8> are supported as return type.
*/

/*!
    \relates QKnxAdditionalInfo

    Writes the KNX cEMI frame's additional \a info to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxAdditionalInfo &info)
{
    QDebugStateSaver _(debug);
    if (info.isValid()) {
        QDebug &dbg = debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2)
            << qSetPadChar('0');
        foreach (quint8 byte, info.rawData<QVector<quint8>>())
            dbg << byte;
    } else {
         debug.nospace().noquote() << "0x1nv4l1d";
    }
    return debug;
}

/*!
    \relates QKnxAdditionalInfo

    Reads the KNX cEMI frame's additional \a info from the \a stream and returns
    a reference to the \a stream.
*/
QDataStream &operator>>(QDataStream &stream, QKnxAdditionalInfo &info)
{
    quint8 type, size;
    stream >> type >> size;

    QByteArray ba(size, Qt::Uninitialized);
    stream.readRawData(ba.data(), size);
    info = QKnxAdditionalInfo(QKnxAdditionalInfo::Type(type), ba);
    return stream;
}

/*!
    \relates QKnxAdditionalInfo

    Writes the KNX cEMI frame's additional \a info to the \a stream and returns
    a reference to the \a stream.
*/
QDataStream &operator<<(QDataStream &stream, const QKnxAdditionalInfo &info)
{
    if (!info.isValid())
        return stream;
    for (quint8 byte : info.rawData<QByteArray>())
        stream << static_cast<quint8> (byte);
    return stream;
}

QT_END_NAMESPACE
