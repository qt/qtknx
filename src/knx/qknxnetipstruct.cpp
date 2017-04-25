/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipstruct.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpStruct

    \inmodule QtKnx
    \brief The QKnxNetIpStruct class represents the base for all KNXnet/IP
    structures.
*/

/*!
    Creates a new QKnxNetIpStruct with the given \a code.
*/

/*!
    Creates a new QKnxNetIpStruct with the given \a header and \a payload.
    The \a header is expected to be fully setup and all values describing the
    payload match the given \a payload.
    \note No adjustments are made to the function arguments.
*/

/*!
    Returns the generic code stored in the KNXnet/IP structures header.
*/

/*!
    Sets the generic code stored in the KNXnet/IP structures header.
*/

/*!
    Returns the number of bytes representing the KNXnet/IP structure including
    the header and the payload.
*/

/*!
    \relates QKnxNetIpStruct

    Writes the KNX \a NetIpStruct to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpStruct &package)
{
    QDebugStateSaver _(debug);
    if (package.isValid()) {
        QDebug &dbg = debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2)
                                                << qSetPadChar('0');
        const auto bytes = package.bytes<QVector<quint8>>();
        for (quint8 byte : qAsConst(bytes))
            dbg << byte;
    } else {
        debug.nospace().noquote() << "0x1nv4l1d";
    }
    return debug;
}

/*!
    \relates QKnxNetIpStruct

    Writes a KNX \a NetIpStruct to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxNetIpStruct &package)
{
    if (!package.isValid())
        return out;

    const auto bytes = package.bytes<QByteArray>();
    for (quint8 byte : qAsConst(bytes))
        out << byte;
    return out;
}

QT_END_NAMESPACE
