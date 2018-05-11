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

#include "qknxtime.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxTimeBase

    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \inherits QAbstractKnxTime
    \brief The QKnxTimeBase class is a base class for datapoint types that
    contain time information.

    \sa QKnxTime, QKnxTime24, {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnxTimeBase::TimeFlag

    This enum holds the time flag.

    \value NullTime
    \value MsecPerDay
    \value SecondsPerHour
    \value MsecPerHour
    \value SecondsPerMinute
    \value MsecPerMinute
*/

/*!
    \class QAbstractKnxTime

    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QAbstractKnxTime class is an abstract base class for datapoint
    types that contain time information.

    \sa QKnxTimeBase, {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QAbstractKnxTime::DayOfWeek

    This enum holds the day of the week.

    \value Ignore
    \value Monday
    \value Tuesday
    \value Wednesday
    \value Thursday
    \value Friday
    \value Saturday
    \value Sunday
*/

/*!
    \fn template<typename Derived>QKnxTimeBase<Derived>::QKnxTimeBase()

    Creates a datapoint type for storing time information.
*/

/*!
    \fn template<typename Derived>QKnxTimeBase<Derived>::QKnxTimeBase(quint8 h, quint8 m, quint8 s = 0, QAbstractKnxTime::DayOfWeek day = DayOfWeek::Ignore)

    Creates a datapoint type with hours set to \a h, minutes to \a m, seconds to
    \a s, and the day of week to \a day.
*/

/*!
    \fn template<typename Derived>bool QKnxTimeBase<Derived>::isNull() const

    Returns \c true if the time is null (that is, the time object was
    constructed using the default constructor); otherwise returns \c false.
    A null time is also an invalid time.

    \sa isValid()
*/

/*!
    \fn template<typename Derived>bool QKnxTimeBase<Derived>::isValid(qint8 h, qint8 m, qint8 s)

    Returns \c true if the values specified for hours \a h, minutes \a m, and
    seconds \a s are valid; otherwise returns \c false.
*/

/*!
    \fn template<typename Derived>bool QKnxTimeBase<Derived>::isValid() const

    Returns \c true if the time is valid; otherwise returns \c false.

    \sa isNull()
*/

/*!
    \fn template<typename Derived>qint8 QKnxTimeBase<Derived>::hour() const

    Returns the hour specified as a part of the time information.

    \sa minute(), second()
*/

/*!
    \fn template<typename Derived>qint8 QKnxTimeBase<Derived>::minute() const

    Returns the minutes specified as a part of the time information.

    \sa hour(), second()
*/

/*!
    \fn template<typename Derived>qint8 QKnxTimeBase<Derived>::second() const

    Returns the seconds specified as a part of the time information.

    \sa hour(), minute()
*/

/*!
    \fn template<typename Derived>void QKnxTimeBase<Derived>::setHMS(quint8 h, quint8 m, quint8 s)

    Sets the time to the hour \a h, minutes \a m, and seconds \a s if the given
    arguments are valid.

    \sa isValid()
*/

/*!
    \fn template<typename Derived>DayOfWeek QKnxTimeBase<Derived>::dayOfWeek() const

    Returns the day of the week.
*/

/*!
     \fn template<typename Derived>void QKnxTimeBase<Derived>::setDayOfWeek(DayOfWeek day)

    Sets the day of the week to \a day.
*/

/*!
    \fn template<typename Derived>int QKnxTimeBase<Derived>::ds() const

    Returns the month, day, and seconds.
*/

/*!
    \fn template<typename Derived>bool QKnxTimeBase<Derived>::operator==(const QKnxTimeBase &o) const

    Returns \c true if this time is equal to \a o; otherwise returns \c false.
*/

/*!
    \fn template<typename Derived>bool QKnxTimeBase<Derived>::operator!=(const QKnxTimeBase &o) const

    Returns \c true if this time is different from \a o; otherwise returns
    \c false.
*/

/*!
    \fn template<typename Derived>bool QKnxTimeBase<Derived>::operator< (const QKnxTimeBase &o) const

    Returns \c true if this time is earlier than \a o; otherwise returns
    \c false.
*/

/*!
    \fn template<typename Derived>bool QKnxTimeBase<Derived>::operator<=(const QKnxTimeBase &o) const

    Returns \c true if this time is earlier than or equal to \a o; otherwise
    returns \c false.
*/

/*!
    \fn template<typename Derived>bool QKnxTimeBase<Derived>::operator> (const QKnxTimeBase &o) const

    Returns \c true if this time is later than \a o; otherwise returns \c false.
*/

/*!
    \fn template<typename Derived>bool QKnxTimeBase<Derived>::operator>=(const QKnxTimeBase &o) const

    Returns \c true if this time is equal to or later than \a o; otherwise
    returns \c false.
*/

/*!
    \class QKnxTime

    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxTime class holds time information.

    Hour values must be less than \c 24. Minute and second values must be less
    than \c 59.

    \sa QKnxTimeBase, QKnxTime24, {Qt KNX Datapoint Type Classes}
*/

/*!
    \class QKnxTime24

    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxTime24 class holds time information.

    Hour values can be up to \c 24. Minute and second values can be up to \c 59.

    \sa QKnxTimeBase, QKnxTime, {Qt KNX Datapoint Type Classes}
*/

QAbstractKnxTime::~QAbstractKnxTime()
{}

/*!
    Returns \c true if the time is valid; otherwise returns \c false.
    For example, the time 23:30:55.746 is valid, but 24:12:30 is invalid.

    \sa QKnxTimeBase::isNull()
*/
bool QKnxTime::isValid() const
{
    return QKnxTimeBase<QKnxTime>::ds() > NullTime
        && QKnxTimeBase<QKnxTime>::ds() < MsecPerDay;
}

/*!
    Returns \c true if the values specified for hours \a h, minutes \a m, and
    seconds \a s are valid; otherwise returns \c false.

    \sa QKnxTimeBase::isNull()
*/
bool QKnxTime::isValid(quint8 h, quint8 m, quint8 s)
{
    return quint32(h) < 24 && quint32(m) < 60 && quint32(s) < 60;
}

/*!
    Returns \c true if the time is valid; otherwise returns \c false. For
    example, the time 24:12:30 is valid, but 24:60:30 is invalid.

    \sa QKnxTimeBase::isNull()
*/
bool QKnxTime24::isValid() const
{
    return QKnxTimeBase<QKnxTime24>::ds() > NullTime
        && QKnxTimeBase<QKnxTime24>::ds() <= MsecPerDay;
}

/*!
    Returns \c true if the values specified for hours \a h, minutes \a m, and
    seconds \a s are valid; otherwise returns \c false.

    \sa QKnxTimeBase::isNull()
*/
bool QKnxTime24::isValid(quint8 h, quint8 m, quint8 s)
{
    return quint32(h) <= 24 && quint32(m) < 60 && quint32(s) < 60;
}

#include "moc_qknxtime.cpp"

QT_END_NAMESPACE
