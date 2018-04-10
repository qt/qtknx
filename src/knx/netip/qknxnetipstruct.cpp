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

// TODO: remove
static QDebug stream(QDebug debug, const QKnxByteArray &bytes)
{
    return debug.nospace().noquote() << "0x" << bytes.toHex();
}

/*!
    \relates QKnxNetIpStruct

    Writes the KNXnet/IP host protocol information structure \a hpai to the
    \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpStruct<QKnxNetIp::HostProtocol> &hpai)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << hpai.bytes().toHex();
}

/*!
    \relates QKnxNetIpStruct

    Writes the KNXnet/IP connection information structure \a cr to the \a debug
    stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpStruct<QKnxNetIp::ConnectionType> &cr)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << cr.bytes().toHex();
}

/*!
    \relates QKnxNetIpStruct

    Writes the KNXnet/IP description information block structure \a dib to the
    \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpStruct<QKnxNetIp::DescriptionType> &dib)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << dib.bytes().toHex();
}

QT_END_NAMESPACE
