/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qknxnetipstructheader.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpStructHeader

    \inmodule QtKnx

    \brief The QKnxNetIpStructHeader class is a KNXnet/IP structure header.

    The first octet of the structure is the length of the structure and the
    second octet is an identifier that specifies the type of the structure.

    If the structure's amount of data exceeds 252 octets, the first octet is
    \c 0xff and the next two octets contain the length as a 16 bit value. The
    fourth octet is then the identifier specifying the type of the structure.
*/

/*!
    \fn QKnxNetIpStructHeader::QKnxNetIpStructHeader

    Constructs an empty invalid header object.

    \sa isNull(), isValid()
*/

/*!
    \fn QKnxNetIpStructHeader::~QKnxNetIpStructHeader()

    Destroys the header object and releases all allocated resources.
*/

/*!
    \fn QKnxNetIpStructHeader::QKnxNetIpStructHeader(CodeType code)

    Constructs a valid header object and sets the KNXnet/IP structure identifier
    to \a code.

    \sa isValid(), code(), setCode()
*/

/*!
    \fn QKnxNetIpStructHeader::QKnxNetIpStructHeader(CodeType code, quint16 dataSize)

    Constructs a valid header object and sets the KNXnet/IP struct identifier
    to \a code and the data size to \a dataSize and updates the header size and
    total size accordingly.

    \sa isValid(), code(), setCode(), dataSize(), setDataSize()
*/

/*!
    \fn bool QKnxNetIpStructHeader::isNull() const

    Returns \c true if this is a default constructed header, otherwise returns
    \c false. A header is considered null if it contains no initialized values.

    \sa isValid()
*/

/*!
    \fn bool QKnxNetIpStructHeader::isValid() const

    Returns \c true if the KNXnet/IP struct header contains initialized values
    and is in itself valid, otherwise returns \c false. A valid KNXnet/IP struct
    header consist of a given total size and a known \l code identifier.

    \sa isNull()
*/

/*!
    \fn quint8 QKnxNetIpStructHeader::size() const

    Returns the size of the KNXnet/IP struct's header.

    \sa isNull(), isValid()
*/

/*!
    \fn quint16 QKnxNetIpStructHeader::totalSize() const

    Returns the total size of the KNXnet/IP struct if the header is valid,
    otherwise returns \c null. The total size includes the size of the header
    and the size of the KNXnet/IP struct's data.

    \sa isNull(), isValid(), size(), dataSize()
*/

/*!
    \fn quint16 QKnxNetIpStructHeader::dataSize() const

    Returns the size of the KNXnet/IP struct's data if the header is valid,
    otherwise returns \c null.
    The data size excludes the size of the KNXnet/IP struct's header.

    \sa isNull(), isValid(), size(), totalSize()
*/

/*!
    \fn void QKnxNetIpStructHeader::setDataSize(quint16 dataSize)

    Sets the KNXnet/IP struct's data size to \a dataSize and updates the
    header and total size accordingly.
*/

/*!
    \fn CodeType QKnxNetIpStructHeader::code() const

    Returns the KNXnet/IP struct's \l code identifier.
*/

/*!
    \fn void QKnxNetIpStructHeader::setCode(CodeType code)

    Sets the KNXnet/IP struct's code identifier to \a code.
*/

/*!
    \fn QString QKnxNetIpStructHeader::toString() const

    Returns the KNXnet/IP struct's total size and generic code as a string. The
    code and total size are formatted in hexadecimal notation.
*/

/*!
    \fn quint8 QKnxNetIpStructHeader::byte(quint8 index) const

    Returns the byte at position \a index in the header.
*/

/*!
    \fn QKnxByteArray QKnxNetIpStructHeader::bytes() const

    Returns an array of bytes that represent the KNXnet/IP struct header.
*/

/*!
    \fn static QKnxNetIpStructHeader QKnxNetIpStructHeader::fromBytes(const QKnxByteArray &bytes, quint16 index = 0)

    Constructs the KNXnet/IP struct header from the byte array \a bytes starting
    at the position \a index inside the array.

    \sa isNull(), isValid()
*/

QT_END_NAMESPACE
