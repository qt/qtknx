/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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
