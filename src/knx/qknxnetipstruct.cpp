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

static QDebug stream(QDebug debug, const QVector<quint8> &bytes)
{
    QDebug &dbg = debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar('0');
    for (quint8 byte : qAsConst(bytes))
        dbg << byte;
    return debug;
}

static QDataStream &stream(QDataStream &out, const QByteArray &bytes)
{
    for (quint8 byte : qAsConst(bytes))
        out << byte;
    return out;
}

/*!
    \relates QKnxNetIpHostProtocolStruct

    Writes the KNX \a QKnxNetIpHostProtocolStruct to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpHostProtocolStruct &package)
{
    QDebugStateSaver _(debug);
    return package.isValid() ? stream(debug, package.bytes<QVector<quint8>>())
        : debug.nospace().noquote() << "0x1nv4l1d";
}

/*!
    \relates QKnxNetIpHostProtocolStruct

    Writes a KNX \a QKnxNetIpHostProtocolStruct to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxNetIpHostProtocolStruct &package)
{
    return package.isValid() ? stream(out, package.bytes<QByteArray>()) : out;
}

/*!
    \relates QKnxNetIpConnectionTypeStruct

    Writes the KNX \a QKnxNetIpConnectionTypeStruct to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpConnectionTypeStruct &package)
{
    QDebugStateSaver _(debug);
    return package.isValid() ? stream(debug, package.bytes<QVector<quint8>>())
        : debug.nospace().noquote() << "0x1nv4l1d";
}

/*!
    \relates QKnxNetIpConnectionTypeStruct

    Writes a KNX \a QKnxNetIpConnectionTypeStruct to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxNetIpConnectionTypeStruct &package)
{
    return package.isValid() ? stream(out, package.bytes<QByteArray>()) : out;
}

/*!
    \relates QKnxNetIpDescriptionTypeStruct

    Writes the KNX \a QKnxNetIpDescriptionTypeStruct to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpDescriptionTypeStruct &package)
{
    QDebugStateSaver _(debug);
    return package.isValid() ? stream(debug, package.bytes<QVector<quint8>>())
        : debug.nospace().noquote() << "0x1nv4l1d";
}

/*!
    \relates QKnxNetIpDescriptionTypeStruct

    Writes a KNX \a QKnxNetIpDescriptionTypeStruct to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxNetIpDescriptionTypeStruct &package)
{
    return package.isValid() ? stream(out, package.bytes<QByteArray>()) : out;
}

QT_END_NAMESPACE
