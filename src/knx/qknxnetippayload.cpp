/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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
