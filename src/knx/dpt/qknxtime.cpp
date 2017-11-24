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

#include "qknxtime.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxTime

    \inmodule QtKnx
    \brief The QKnxTime class holds time information.

    Hour values must be less than 24.
*/

/*!
    \class QKnxTime24

    \inmodule QtKnx
    \brief The QKnxTime24 class holds time information.

    Hour values can be up to 24.
*/

QAbstractKnxTime::~QAbstractKnxTime()
{}

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

#include "moc_qknxtime.cpp"

QT_END_NAMESPACE
