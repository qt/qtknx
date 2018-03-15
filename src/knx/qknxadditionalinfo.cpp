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

#include "qknxadditionalinfo.h"
#include "qknxutils.h"

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
    \value ManufacturerSpecificData       Manufacturer specific data, including
                                        manufacturer ID (2 byte) and
                                        Subfunction ID (1 byte).
    \omitvalue EscCode
*/

/*!
    \fn QKnxAdditionalInfo::QKnxAdditionalInfo()

    Constructs an new, empty, invalid additional info object.
*/

/*!
    \fn QKnxAdditionalInfo::~QKnxAdditionalInfo()

    Destroys the additional info object and releases all allocated resources.
*/

/*!
    Constructs an new additional info object and sets its \l Type to \a type
    and data to \a data.

    \note The KNX specification limits the size of the data to a maximum
    of 252 bytes. The class implementation acknowledges this and silently
    truncates the data stored in the additional info object.

    \sa isNull(), isValid()
*/
QKnxAdditionalInfo::QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QKnxByteArray &data)
{
    m_bytes[0] = quint8(type);
    m_bytes[1] = quint8(qMin(data.size(), 252));
    m_bytes += data.mid(0, m_bytes[1]);
}

/*!
    If this is an additional info object that is constructed by default,
    returns \c true, otherwise returns \c false. An additional info's object
    is considered null if it contains no initialized type value.

    \sa isValid()
*/
bool QKnxAdditionalInfo::isNull() const
{
    return (m_bytes[0] == 0x00);
}

/*!
    Returns \c true if this is a valid additional info object; \c false
    otherwise.
*/
bool QKnxAdditionalInfo::isValid() const
{
    if (isNull())
        return false;

    if ((size() != m_bytes.size()) || (size() > 254))
        return false;

    const auto type = QKnxAdditionalInfo::Type(m_bytes[0]);
    const auto expectedSize = expectedDataSize(type);
    if (expectedSize < 0)
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
        return dataSize() == quint8(expectedSize);
    case QKnxAdditionalInfo::Type::RfFastAckInformation:
        return (dataSize() >= quint8(expectedSize)) && ((dataSize() % 2) == 0);
    case QKnxAdditionalInfo::Type::ManufacturerSpecificData:
        return dataSize() >= quint8(expectedSize);
    case QKnxAdditionalInfo::Type::Reserved:
    case QKnxAdditionalInfo::Type::EscCode:
        break;
    }
    return false;
}

/*!
    Returns the number of bytes representing the additional info, including the
    byte for \l Type id and the byte for length information.
*/
quint8 QKnxAdditionalInfo::size() const
{
    if (isNull())
        return 0;
    return m_bytes[1] + 2; // 2 -> type and length byte
}

/*!
    Returns the additional info \l Type.
*/
QKnxAdditionalInfo::Type QKnxAdditionalInfo::type() const
{
    return QKnxAdditionalInfo::Type(m_bytes[0]);
}

/*!
    Sets the additional info type to \a type.
*/
void QKnxAdditionalInfo::setType(QKnxAdditionalInfo::Type type)
{
    m_bytes[0] = quint8(type);
}

/*!
    Returns the additional info's object data.
*/
QKnxByteArray QKnxAdditionalInfo::data() const
{
    return m_bytes.mid(2);
}

/*!
    Sets the additional info's object data to \a data.

    \note The KNX specification limits the size of the data to a maximum
    of 252 bytes. The class implementation acknowledges this and silently
    truncates the data stored in the additional info object.
*/
void QKnxAdditionalInfo::setData(const QKnxByteArray &data)
{
    m_bytes[1] = quint8(qMin(data.size(), 252));
    m_bytes.resize(2);
    m_bytes += data.mid(0, m_bytes[1]);
}

/*!
    Returns the number of bytes representing the additional info's data.
*/
quint8 QKnxAdditionalInfo::dataSize() const
{
    return m_bytes[1];
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
        \li \l QKnxAdditionalInfo::ManufacturerSpecificData - three bytes minimum.
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
        table[int(QKnxAdditionalInfo::Type::ManufacturerSpecificData)] = 3;
        return table;
    }();
    if (isFixedSize)
        *isFixedSize = (type < QKnxAdditionalInfo::Type::RfFastAckInformation);
    return table[int(type)];
}

/*!
    Returns the byte at position \a index in the additional info's object.
*/
quint8 QKnxAdditionalInfo::byte(quint8 index) const
{
    Q_ASSERT_X(index < size(), "QKnxAdditionalInfo::byte", "index out of range");
    return m_bytes[index];
}

/*!
    Returns an array of bytes that represent the additional info's object.
*/
QKnxByteArray QKnxAdditionalInfo::bytes() const
{
    if (!isValid())
        return {};
    return m_bytes;
}

/*!
    Constructs the additional info object from the byte array \a bytes starting
    at position \a index inside the array.

    \sa isNull(), isValid()
*/
QKnxAdditionalInfo QKnxAdditionalInfo::fromBytes(const QKnxByteArray &bytes, quint16 index)
{
    const qint32 availableSize = bytes.size() - index;
    if (availableSize < 2)
        return {}; // size missing

    quint16 size = QKnxUtils::QUint8::fromBytes(bytes, index + 1) + 2; // type + size => 2
    if (availableSize < size)
        return {};

    return { QKnxAdditionalInfo::Type(bytes[index]), bytes.mid(index + 2, bytes[index + 1]) };
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
            << qSetPadChar(QLatin1Char('0'));
        const auto rawData = info.bytes();
        for (quint8 byte : qAsConst(rawData))
            dbg << byte;
    } else {
         debug.nospace().noquote() << "0x1nv4l1d";
    }
    return debug;
}

QT_END_NAMESPACE
