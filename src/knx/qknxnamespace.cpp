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

#include "qknxnamespace.h"

QT_BEGIN_NAMESPACE

/*!
    \namespace QKnx

    \inmodule QtKnx
    \brief Contains miscellaneous identifiers used throughout the QtKnx library.
*/

/*!
    \enum QKnx::MediumType

    This enum describes the various medium types supported by KNX bus.

    \value Unknown      The supported medium type is unknown.
    \value TP           The supported medium type is twisted pair 1.
    \value PL           The supported medium type is power-line 110.
    \value RF           The supported medium type is radio frequency.
    \value NetIP        The supported medium type is KNXnet/IP.
*/

/*!
    \enum QKnx::NetIp::CemiServer

    This enum describes the possible error codes returned by the remote cEMI
    Server after cEMI function properties service requests.

    \value NoError The function was successfully executed. The return code
                   indicates the positive result of the function.
    \value Error   The function was not successfully executed. The return code
                   indicates the negative result of the function. Can be any
                   number larger than \c NoError.
*/

/*!
    \enum QKnx::NetIp::Error : quint8

    This enum describes the possible error codes returned by the remote cEMI
    Server after cEMI service management requests.

    \value Unspecified
           The error that happened is unknown. Can occur in negative read/ write
           confirmation frames.
    \value OutOfRange
           Denotes a general write error if OutOfMaxRange or OutOfMinRange are
           not applicable. Can occur in negative write confirmation frames.
    \value OutOfMaxRange
           The value to write was to high. Can occur in negative write
           confirmation frames.
    \value OutOfMinRange
           The value to write was to low. Can occur in negative write
           confirmation frames.
    \value Memory
           The memory can not be written or only with fault(s). Can occur in
           negative write confirmation frames.
    \value ReadOnly
           Write access to a read-only or a write protected property. Can occur
           in negative write confirmation frames.
    \value IllegalCommand
           The used command is not valid or not supported by the cEMI server.
    \value NonExistingProperty
           Read or write access to an non existing Property. Can occur in
           negative read/ write confirmation frames.
    \value TypeConflict
           Write access with a wrong data type (datapoint length).
    \value PropertyIndexRangeError
           Read or write access to a non existing property-array index. Can
           occur in negative read/ write confirmation frames.
    \value TemporaryNotWritable
           The Property exists but cannot be set to a new value at the time.
           Can occur in negative write confirmation frames.
    \value None     No error happened. Please do not use.
*/

QT_END_NAMESPACE
