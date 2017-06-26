/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxnetipframe.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpFrame

    \inmodule QtKnx
    \brief The QKnxNetIpFrame class represents the base for all KNXnet/IP
    frames.
*/

/*!
    Creates a new QKnxNetIpFrame with the given \a code.
*/

/*!
    Creates a new QKnxNetIpFrame with the given \a header and \a payload.
    The \a header is expected to be fully setup and all values describing the
    payload match the given \a payload.
    \note No adjustments are made to the function arguments.
*/

/*!
    Returns the generic code stored in the KNXnet/IP frame header.
*/

/*!
    Sets the generic code stored in the KNXnet/IP frame header.
*/

/*!
    Returns the number of bytes representing the KNXnet/IP frame including
    the header and the payload.
*/

/*!
    \relates QKnxNetIpFrame

    Writes the KNX \a NetIpFrame to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpFrame &package)
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
    \relates QKnxNetIpFrame

    Writes a KNX \a NetIpFrame to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxNetIpFrame &package)
{
    if (!package.isValid())
        return out;

    const auto bytes = package.bytes<QByteArray>();
    for (quint8 byte : qAsConst(bytes))
        out << byte;
    return out;
}

QT_END_NAMESPACE
