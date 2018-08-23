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
    \ingroup qtknx-netip

    \brief The QKnxNetIpStructHeader class is a KNXnet/IP structure header.

    The first octet of the structure is the length of the structure and the
    second octet is an identifier that specifies the type of the structure.

    If the structure's amount of data exceeds 252 octets, the first octet is
    \c 0xff and the next two octets contain the length as a 16 bit value. The
    fourth octet is then the identifier specifying the type of the structure.

    \sa {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStructHeader<CodeType>::QKnxNetIpStructHeader()

    Constructs an empty invalid header object.

    \sa isNull(), isValid()
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStructHeader<CodeType>::~QKnxNetIpStructHeader()

    Destroys the header object and releases all allocated resources.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStructHeader<CodeType>::QKnxNetIpStructHeader(CodeType code)

    Constructs a valid header object and sets the KNXnet/IP structure identifier
    to \a code.

    \sa isValid(), code(), setCode()
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStructHeader<CodeType>::QKnxNetIpStructHeader(CodeType code, quint16 dataSize)

    Constructs a valid header object and sets the KNXnet/IP structure identifier
    to \a code and the data size to \a dataSize and updates the header size and
    total size accordingly.

    \sa isValid(), code(), setCode(), dataSize(), setDataSize()
*/

/*!
    \fn template <typename CodeType> bool QKnxNetIpStructHeader<CodeType>::isNull() const

    Returns \c true if this is a default constructed header, otherwise returns
    \c false. A header is considered null if it contains no initialized values.

    \sa isValid()
*/

/*!
    \fn template <typename CodeType> bool QKnxNetIpStructHeader<CodeType>::isValid() const

    Returns \c true if the KNXnet/IP structure header contains initialized
    values and is in itself valid, otherwise returns \c false. A valid KNXnet/IP
    structure header consist of a given total size and a known \l code
    identifier.

    \sa isNull()
*/

/*!
    \fn template <typename CodeType> quint8 QKnxNetIpStructHeader<CodeType>::size() const

    Returns the size of the KNXnet/IP structure header.

    \sa isNull(), isValid()
*/

/*!
    \fn template <typename CodeType> quint16 QKnxNetIpStructHeader<CodeType>::totalSize() const

    Returns the total size of the KNXnet/IP structure if the header is valid,
    otherwise returns \c null. The total size includes the size of the header
    and the size of the KNXnet/IP structure data.

    \sa isNull(), isValid(), size(), dataSize()
*/

/*!
    \fn template <typename CodeType> quint16 QKnxNetIpStructHeader<CodeType>::dataSize() const

    Returns the size of the KNXnet/IP structure data if the header is valid,
    otherwise returns \c null.

    The data size excludes the size of the KNXnet/IP structure header.

    \sa isNull(), isValid(), size(), totalSize()
*/

/*!
    \fn template <typename CodeType> void QKnxNetIpStructHeader<CodeType>::setDataSize(quint16 dataSize)

    Sets the KNXnet/IP structure data size to \a dataSize and updates the
    header and total size accordingly.
*/

/*!
    \fn template <typename CodeType> CodeType QKnxNetIpStructHeader<CodeType>::code() const

    Returns the KNXnet/IP structure's \l code identifier.
*/

/*!
    \fn template <typename CodeType> void QKnxNetIpStructHeader<CodeType>::setCode(CodeType code)

    Sets the KNXnet/IP structure's code identifier to \a code.
*/

/*!
    \fn template <typename CodeType> quint8 QKnxNetIpStructHeader<CodeType>::byte(quint8 index) const

    Returns the byte at position \a index in the header.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStructHeader<CodeType>::bytes() const

    Returns an array of bytes that represent the KNXnet/IP structure header.
*/

/*!
    \fn template <typename CodeType> static QKnxNetIpStructHeader<CodeType>::fromBytes(const QKnxByteArray &bytes, quint16 index = 0)

    Constructs the KNXnet/IP structure header from the byte array \a bytes
    starting at the position \a index inside the array.

    \sa isNull(), isValid()
*/

/*!
    \fn template <typename CodeType> bool QKnxNetIpStructHeader<CodeType>::operator==(const QKnxNetIpStructHeader &other) const

    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/

/*!
    \fn template <typename CodeType> bool QKnxNetIpStructHeader<CodeType>::operator!=(const QKnxNetIpStructHeader &other) const

    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/

/*!
    \since 5.12
    \fn template <typename CodeType> bool QKnxNetIpStructHeader<CodeType>::isMandatory() const

    Returns \c true if the mandatory bit is set; otherwise returns \c false.
*/

/*!
    \since 5.12
    \fn template <typename CodeType> void QKnxNetIpStructHeader<CodeType>::setMandatory(bool value)

    Sets the KNXnet/IP structure's mandatory bit to \a value.
*/

/*!
    \relates QKnxNetIpStructHeader
    \fn template<typename CodeType> QDebug operator<<(QDebug debug, const QKnxNetIpStructHeader<CodeType> &header)

    Writes the KNXnet/IP structure header \a header to the \a debug stream.
*/

QT_END_NAMESPACE
