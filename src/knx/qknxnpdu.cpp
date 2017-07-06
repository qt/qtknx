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

#include "qknxnpdu.h"

QT_BEGIN_NAMESPACE

bool QKnxNpdu::isValid()
{
    return size() != 0;
}

void QKnxNpdu::appendAndUpdate(const QByteArray &data)
{
    appendBytes<QByteArray>(data);
    setByte(0, quint8(size() - 2));
}

QT_END_NAMESPACE
