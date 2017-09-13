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

#include "qknxtime.h"

QT_BEGIN_NAMESPACE

bool QKnxTime::isValid() const
{
    return QKnxTimeBase<QKnxTime>::ds() > NullTime
        && QKnxTimeBase<QKnxTime>::ds() < MsecPerDay;
}
bool QKnxTime::isValid(quint8 h, quint8 m, quint8 s)
{
    return quint32(h) < 24 && quint32(m) < 60 && quint32(s) < 60;
}

bool QKnxTime24::isValid() const
{
    return QKnxTimeBase<QKnxTime24>::ds() > NullTime
        && QKnxTimeBase<QKnxTime24>::ds() <= MsecPerDay;
}
bool QKnxTime24::isValid(quint8 h, quint8 m, quint8 s)
{
    return quint32(h) <= 24 && quint32(m) < 60 && quint32(s) < 60;
}

QT_END_NAMESPACE
