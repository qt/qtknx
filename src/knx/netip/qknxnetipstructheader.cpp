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

#include "qknxnetipstructheader.h"

QT_BEGIN_NAMESPACE

/*
    2.1.3 Structures:

    All KNXnet/IP structures follow a common rule: the first octet shall always
    be the length of the complete structure (as some structures may have fields
    of variable length e.g. strings) and the second octet shall always be an
    identifier that shall specify the type of the structure. From the third
    octet on the structure data shall follow.
*/

/*!
    \class QKnxNetIpStructHeader

    \inmodule QtKnx

    \brief The QKnxNetIpStructHeader class is a KNXnet/IP structure header.

    The first octet of the structure is the length of the structure and the
    second octet is an identifier that specifies the type of the structure.
    The data follows from the third octet on.
*/

/*!
    \fn QString QKnxNetIpStructHeader::toString() const

    Returns the KNXnet/IP struct's total size and generic code as a string. The
    code and total size are formatted in hexadecimal notation.
*/

QT_END_NAMESPACE
