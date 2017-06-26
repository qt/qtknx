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
    Returns the KNXnet/IP structs total size and generic code as string. Code
    and total size are formatted in hexadecimal notation.
*/

QT_END_NAMESPACE
