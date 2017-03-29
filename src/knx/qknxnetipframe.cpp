/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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

QT_END_NAMESPACE
