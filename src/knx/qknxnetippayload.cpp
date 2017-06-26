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

#include "qknxnetippayload.h"

QT_BEGIN_NAMESPACE

QKnxNetIpPayload::QKnxNetIpPayload(quint8 byte)
    : QKnxByteStore(byte)
{}

QKnxNetIpPayload::QKnxNetIpPayload(const quint8 *data, quint16 size)
    : QKnxByteStore(data, size)
{}

QKnxNetIpPayloadRef QKnxNetIpPayload::ref(quint16 index) const
{
    return QKnxByteStore::ref(index);
}

QT_END_NAMESPACE
