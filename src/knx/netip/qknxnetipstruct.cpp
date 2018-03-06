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

static QDebug stream(QDebug debug, const QKnxByteArray &bytes)
{
    return debug.nospace().noquote() << "0x" << bytes.toHex();
}

static QDataStream &stream(QDataStream &out, const QKnxByteArray &bytes)
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
    return package.isValid() ? stream(debug, package.bytes())
        : debug.nospace().noquote() << "0x1nv4l1d";
}

/*!
    \relates QKnxNetIpHostProtocolStruct

    Writes a KNX \a QKnxNetIpHostProtocolStruct to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxNetIpHostProtocolStruct &package)
{
    return package.isValid() ? stream(out, package.bytes()) : out;
}

/*!
    \relates QKnxNetIpConnectionTypeStruct

    Writes the KNX \a QKnxNetIpConnectionTypeStruct to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpConnectionTypeStruct &package)
{
    QDebugStateSaver _(debug);
    return package.isValid() ? stream(debug, package.bytes())
        : debug.nospace().noquote() << "0x1nv4l1d";
}

/*!
    \relates QKnxNetIpConnectionTypeStruct

    Writes a KNX \a QKnxNetIpConnectionTypeStruct to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxNetIpConnectionTypeStruct &package)
{
    return package.isValid() ? stream(out, package.bytes()) : out;
}

/*!
    \relates QKnxNetIpDescriptionTypeStruct

    Writes the KNX \a QKnxNetIpDescriptionTypeStruct to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpDescriptionTypeStruct &package)
{
    QDebugStateSaver _(debug);
    return package.isValid() ? stream(debug, package.bytes())
        : debug.nospace().noquote() << "0x1nv4l1d";
}

/*!
    \relates QKnxNetIpDescriptionTypeStruct

    Writes a KNX \a QKnxNetIpDescriptionTypeStruct to the stream \a out and returns a reference to the
    stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxNetIpDescriptionTypeStruct &package)
{
    return package.isValid() ? stream(out, package.bytes()) : out;
}

QT_END_NAMESPACE
