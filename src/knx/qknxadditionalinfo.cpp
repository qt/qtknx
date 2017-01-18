/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxadditionalinfo.h"

QT_BEGIN_NAMESPACE

struct {
    QKnxAdditionalInfo::Type type;
    qint32 expectedDataSize;
} gTypeSizeTable[] = {
    { QKnxAdditionalInfo::Type::Reserved, -1 },
    { QKnxAdditionalInfo::Type::PlMediumInformation, 2 },
    { QKnxAdditionalInfo::Type::RfMediumInformation, 8 },
    { QKnxAdditionalInfo::Type::BusmonitorStatusInfo, 1 },
    { QKnxAdditionalInfo::Type::TimestampRelative, 2 },
    { QKnxAdditionalInfo::Type::TimeDelayUntilSending, 4 },
    { QKnxAdditionalInfo::Type::ExtendedRelativeTimestamp, 4 },
    { QKnxAdditionalInfo::Type::BiBatInformation, 2 },
    { QKnxAdditionalInfo::Type::RfMultiInformation, 4 },
    { QKnxAdditionalInfo::Type::PreambleAndPostamble, 3 }
};

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
    Returns the additional info type.
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
    Constructs an new additional info object and sets its type to \a type and
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
    Returns \c true if \a type is one of the know enumeration in \l Type and
    \a data has the expected length for \a type.

    \sa expectedDataSize()
*/
bool QKnxAdditionalInfo::isValid(QKnxAdditionalInfo::Type type, const QVector<quint8> &data)
{
    const int size = data.size();
    if (size > 252)
        return false;

    switch (type) {
    case QKnxAdditionalInfo::Type::PlMediumInformation:
    case QKnxAdditionalInfo::Type::RfMediumInformation:
    case QKnxAdditionalInfo::Type::BusmonitorStatusInfo:
    case QKnxAdditionalInfo::Type::TimestampRelative:
    case QKnxAdditionalInfo::Type::TimeDelayUntilSending:
    case QKnxAdditionalInfo::Type::ExtendedRelativeTimestamp:
    case QKnxAdditionalInfo::Type::BiBatInformation:
    case QKnxAdditionalInfo::Type::RfMultiInformation:
    case QKnxAdditionalInfo::Type::PreambleAndPostamble:
        return size == expectedDataSize(type);
    case QKnxAdditionalInfo::Type::RfFastAckInformation:
        return (size >= expectedDataSize(type)) && ((size % 2) == 0);
    case QKnxAdditionalInfo::Type::ManufactorSpecificData:
        return size >= expectedDataSize(type);
    default:
        break;
    }
    return false;
}

/*!
    Returns the number of bytes representing the additional info, including the
    byte for type id and the byte for length information.
*/
qint32 QKnxAdditionalInfo::size() const
{
    if (m_data.isEmpty())
        return 0;
    return m_data.size() + 2;
}

/*!
    Returns the number of bytes representing the additional info, excluding the
    byte for type id and the byte for length information.
*/
qint32 QKnxAdditionalInfo::dataSize() const
{
    return m_data.size();
}

/*!
    Returns the number of expected bytes for \a type. The additional \c bool
    parameter \a isFixedSize is set to \c true if the \a type expects a fixed
    size of bytes; otherwise to false. If the ype is unknown, the function will
    return a negative value.

    Know types of variable size are:
    \list
        \li \l QKnxAdditionalInfo::RfFastAckInformation - a multiple of two
               bytes, two bytes minimum.
        \li \l QKnxAdditionalInfo::ManufactorSpecificData - three bytes minimum.
    \endlist
*/
qint32 QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type type, bool *isFixedSize)
{
    if ((type <= Type::Reserved || type > Type::RfFastAckInformation)
        && (type < Type::ManufactorSpecificData || type >= Type::EscCode)) {
        return -1;
    }

    if (isFixedSize)
        *isFixedSize = (type < QKnxAdditionalInfo::Type::RfFastAckInformation);

    if (type == QKnxAdditionalInfo::Type::RfFastAckInformation)
        return 2;
    if (type == QKnxAdditionalInfo::Type::ManufactorSpecificData)
        return 3;
    return gTypeSizeTable[static_cast<quint8> (type)].expectedDataSize;
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
    Returns the additional info as vector of bytes if the information is valid;
    otherwise an empty vector. The vector includes the type id, the size of the
    actual data and the data itself.
*/
QVector<quint8> QKnxAdditionalInfo::rawData() const
{
    if (!isValid())
        return QVector<quint8>();
    return QVector<quint8>{ quint8(m_type), quint8(m_data.size()) } + m_data;
}

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
        foreach (quint8 byte, info.rawData())
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
    return stream >> reinterpret_cast<quint8&> (info.m_type) >> info.m_data;
}

/*!
    \relates QKnxAdditionalInfo

    Writes the KNX cEMI frame's additional \a info to the \a stream and returns
    a reference to the \a stream.
*/
QDataStream &operator<<(QDataStream &stream, const QKnxAdditionalInfo &info)
{
    return stream << static_cast<quint8> (info.m_type) << info.m_data;
}

QT_END_NAMESPACE
