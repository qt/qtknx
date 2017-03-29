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

QT_END_NAMESPACE
