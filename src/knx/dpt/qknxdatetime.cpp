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

#include "qknxdatetime.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxTimeOfDay
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxTimeOfDay class is a datapoint type for the time of day.

    This is a fixed size datapoint type with the length of 3 bytes.

    The left byte of the data holds the day as 3 bits and the hour as 5 bits,
    the middle byte holds the minutes as 5 bits, and the right byte holds the
    seconds as 5 bits.

    Valid times of day are from \c {No day, 00:00:00} to \c {Sunday, 23:59:59}.

    \sa QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

// -- QKnxTimeOfDay

/*!
    Creates a fixed size datapoint type with the time of day set to \c {0,0,0}.
*/
QKnxTimeOfDay::QKnxTimeOfDay()
    : QKnxTimeOfDay(QKnxTime(0,0,0))
{}

/*!
    Creates a fixed size datapoint type with the time of day set to \a time.
*/
QKnxTimeOfDay::QKnxTimeOfDay(const QKnxTime &time)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Time of day"));
    setRangeText(tr("No day, 00:00:00"), tr("Sunday, 23:59:59"));
    setRange(QVariant::fromValue(QKnxTime(00, 00, 00)), QVariant::fromValue(QKnxTime(23, 59, 59)));
    setValue(time);
}

/*!
    Creates a fixed size datapoint type with the hour set to \a h, minutes set
    to \a m, seconds set to \a s, and the day of the week set to \a day.
*/
QKnxTimeOfDay::QKnxTimeOfDay(quint8 h, quint8 m, quint8 s, QKnxTime::DayOfWeek day)
    : QKnxTimeOfDay()
{
    setValue({ h, m, s, day });
}

/*!
    Returns the time of day stored in the datapoint type.
*/
QKnxTime QKnxTimeOfDay::value() const
{
    return { quint8(byte(0) & 0x1f), byte(1), byte(2),
        static_cast<QKnxTime::DayOfWeek> ((byte(0) & 0xe0) >> 5u) };
}

/*!
    Sets the time of day stored in the datapoint type to \a time.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxTimeOfDay::setValue(const QKnxTime &time)
{
    if (!time.isValid())
        return false;

    return setByte(2, quint8(time.second()))
        && setByte(1, quint8(time.minute()))
        && setByte(0, quint8(time.hour()) | quint8(time.dayOfWeek()) << 5u);
}

/*!
    Returns the hour stored in the datapoint type.
*/
quint8 QKnxTimeOfDay::hour() const
{
    return byte(0) & 0x1f;
}

/*!
    Sets the hour to \a hour.

    If the value is outside the allowed range (from 0 to 23 included), returns
    \c false and does not set the value.
*/
bool QKnxTimeOfDay::setHour(quint8 hour)
{
    if (hour < 24)
        return setByte(0, quint8(byte(0) &0xe0) | hour);
    return false;
}

/*!
    Returns the minutes stored in the datapoint type.
*/
quint8 QKnxTimeOfDay::minute() const
{
    return byte(1);
}

/*!
    Sets the minute to \a minute.

    If the value is outside the allowed range (from 0 to 59 included), returns
    \c false and does not set the value.
*/
bool QKnxTimeOfDay::setMinute(quint8 minute)
{
    if (minute < 60)
       return setByte(1, minute);
    return false;
}

/*!
    Returns the seconds stored in the datapoint type.
*/
quint8 QKnxTimeOfDay::second() const
{
    return byte(2);
}

/*!
    Sets the seconds to \a second.

    If the value is outside the allowed range (from 0 to 59 included), returns
    \c false and does not set the value.
*/
bool QKnxTimeOfDay::setSecond(quint8 second)
{
    if (second < 60)
       return setByte(2, second);
    return false;
}

/*!
    Returns the day of the week stored in the datapoint type.
*/
QKnxTime::DayOfWeek QKnxTimeOfDay::dayOfDay() const
{
    return QKnxTime::DayOfWeek(quint8(byte(0) & 0xe0) >> 5u);
}

/*!
    Sets the day of the week to \a day.

    If the value is outside the allowed range , returns \c false and does not
    set the value.
*/
bool QKnxTimeOfDay::setDayOfWeek(QKnxTime::DayOfWeek day)
{
    return setByte(0, quint8(day) << 5u | quint8(byte(0) & 0x1f));
}

/*!
    \reimp
*/
bool QKnxTimeOfDay::isValid() const
{
    auto time = value();
    return QKnxDatapointType::isValid() && time.isValid()
        && (time >= minimum().value<QKnxTime>()) && (time <= maximum().value<QKnxTime>());
}

/*!
    \class QKnxDate
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxDate class is a datapoint type for a date.

    This is a fixed size datapoint type with the length of 3 bytes.

    The left byte of the data holds the day as 5 bits, the middle byte holds the
    month as 4 bits, and the right byte holds the year as 7 bits.

    Valid dates are from \c {Monday, 1990-01-01} to \c {Saturday, 2089-12-31}.

    \sa QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

// -- QKnxDate

/*!
    Creates a fixed size datapoint type with the date set to \c {2000, 0, 0}.
*/
QKnxDate::QKnxDate()
    : QKnxDate(QDate(2000, 0, 0))
{
    setDescription(tr("Date"));
    setRange(QDate(1990, 1, 1), QDate(2089, 12, 31));
    setRangeText(tr("Monday, 1990-01-01"), tr("Saturday, 2089-12-31"));
}

/*!
    Creates a fixed size datapoint type with the date set to \a date.
*/
QKnxDate::QKnxDate(const QDate &date)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setValue(date);
}

/*!
    Creates a fixed size datapoint type with the year set to \a year, month to
    \a month, and day to \a day.
*/
QKnxDate::QKnxDate(quint16 year, quint8 month, quint8 day)
    : QKnxDate()
{
    setValue({ year, month, day });
}

/*!
    Returns the date stored in the datapoint type.
*/
QDate QKnxDate::value() const
{
    return { year(), month(), day() };
}

/*!
    Sets the date stored in the datapoint type to \a date.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxDate::setValue(const QDate &date)
{
    if (date.isValid() && (date >= minimum().toDate()) && (date <= maximum().toDate())) {
        return setByte(0, date.day())
            && setByte(1, date.month())
            && setByte(2, quint8(date.year() % 100));
    }
    return false;
}

/*!
    Returns the year stored in the datapoint type.
*/
quint16 QKnxDate::year() const
{
    quint16 year = byte(2);
    return year + quint16(year < 90 ? 2000 : 1900);
}

/*!
    Sets the year to \a year.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxDate::setYear(quint16 year)
{
    if (year >= 1990 && year <= 2089)
        return setByte(2, quint8(year % 100));
    return false;
}

/*!
    Returns the month stored in the datapoint type.
*/
quint8 QKnxDate::month() const
{
    return byte(1);
}

/*!
    Sets the month to \a month.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxDate::setMonth(quint8 month)
{
    if (month <= 12 && month > 0)
        return setByte(1, month);
    return false;
}

/*!
    Returns the day stored in the datapoint type.
*/
quint8 QKnxDate::day() const
{
    return byte(0);
}

/*!
    Sets the day to \a day.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxDate::setDay(quint8 day)
{
    if (day <= 31 && day > 0)
        return setByte(0, day);
    return false;
}

/*!
    \reimp
*/
bool QKnxDate::isValid() const
{
    auto date = value();
    return QKnxDatapointType::isValid() && date.isValid()
        && (date >= minimum().toDate()) && (date <= maximum().toDate());
}

/*!
    \class QKnxDateTime
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx
    \ingroup qtknx-datapoint-types

    \brief The QKnxDateTime class is a datapoint type for the date and time.

    This is a fixed size datapoint type with the length of 8 bytes.

    Valid dates are from \c {Monday, 1900-01-01} to \c {Wednesday, 2155-12-31},
    and valid times are from \c {Any day, 00:00:00} to \c {Sunday, 24:00:00}.

    \sa QKnxDatapointType, {Qt KNX Datapoint Type Classes}
*/

/*!
    \enum QKnxDateTime::Attribute

    This enum type holds the date attributes.

    \value Fault              Set if one or more supported fields of the date
        and time information are corrupted.
    \value WorkingDay         Set to indicate a working day.
    \value NoWorkingDay       Set if the Working day field is not valid.
    \value NoYear             Set if the Year field is not valid.
    \value NoDate             Set if the Month and Day of the Month field are
        not valid.
    \value NoDayOfWeek        Set if the Day of week field is not valid.
    \value NoTime             Set if the Hour of day, Minutes and Seconds
        fields are not valid.
    \value StandardSummerTime Standard Summer Time is only an attribute for
        information and visualization. In the hour field, summer-time correction
        is already considered. Therefore, no hour offset shall be added by the
        receiver if Standard Summer Time Information is set.
*/

/*!
    \enum QKnxDateTime::ClockQuality

    This enum type holds whether the device sending date and time information
    sends signals which are synchronized (time to time) with an external date
    and time information source.

    \value WithoutExtendedSyncSignal  Clock without an external synchronization
        signal. The device sending date & time information has a local clock,
        which can be inaccurate.
    \value WithExtendedSyncSignal     Clock with an external synchronization
        signal (like DCF77, videotext). Also an externally synchronized
        clock should send \c {CLQ = 0} after start-up (until reception of first
        synchronization signal) or after a synchronization timeout.
*/

// -- QKnxDateTime

/*!
    Creates a fixed size datapoint type with the date set to \c {2000, 0, 0},
    the time set to \c {0 ,0}, the clock quality set to
    \c WithoutExtendedSyncSignal, and an empty list of attributes.
*/
QKnxDateTime::QKnxDateTime()
    : QKnxDateTime(QDate(2000, 0, 0), QKnxTime24(0, 0), Attributes(),
    ClockQuality::WithoutExtendedSyncSignal)
{}

/*!
    Creates a fixed size datapoint type with the date set to \a date, time to
    \a time, attributes to \a attributes, and clock quality to \a quality.
*/
QKnxDateTime::QKnxDateTime(const QDate &date, const QKnxTime24 &time, Attributes attributes,
        ClockQuality quality)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Date Time"));
    setMinimumText(tr("Monday, 1900-01-01; Any day, 00:00:00"));
    setMaximumText(tr("Wednesday, 2155-12-31; Sunday, 24:00:00"));
    setMinimum(QVariant({ QDate(1900, 01, 01), QVariant::fromValue(QKnxTime24(00, 00, 00)) }));
    setMaximum(QVariant({ QDate(2155, 12, 31), QVariant::fromValue(QKnxTime24(24, 00, 00)) }));

    setValue(date, time, attributes, quality);
}

/*!
    Sets the date stored in the datapoint type to \a date, time to \a time, the
    list of attributes to \a attributes, and clock quality to \a quality.

    Returns \c true if the value was set; otherwise returns \c false.
*/
bool QKnxDateTime::setValue(const QDate &date, const QKnxTime24 &time, Attributes attributes,
        ClockQuality quality)
{
    setAttributes(attributes);
    setClockQuality(quality);
    return setDate(date) && setTime(time);
}

/*!
    Returns the date stored in the datapoint type.
*/
QDate QKnxDateTime::date() const
{
    return { byte(0) + 1900, byte(1), byte(2) };
}

/*!
    Sets the date stored in the datapoint type to \a date.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxDateTime::setDate(const QDate &date)
{
    if (date.isValid() && (date.year() >= 1900) && (date.year() <= 2155)) {
        return setByte(0, quint8(date.year() - 1900))
            && setByte(1, date.month())
            && setByte(2, date.day());
    }
    return false;
}

/*!
    Returns the time stored in the datapoint type.
*/
QKnxTime24 QKnxDateTime::time() const
{
    return { byte(3), byte(4), byte(5) };
}

/*!
    Sets the time stored in the datapoint type to \a time.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnxDateTime::setTime(const QKnxTime24 &time)
{
    if (time.isValid()) {
        return setByte(3, quint8(time.hour()) | quint8(time.dayOfWeek()) << 5u)
            && setByte(4, quint8(time.minute()))
            && setByte(5, quint8(time.second()));
    }
    return false;
}

/*!
    Returns the additional information stored in the datapoint type as a list
    of attributes.
*/
QKnxDateTime::Attributes QKnxDateTime::attributes() const
{
    return Attributes().setFlag(Attribute::Fault, QKnxDatapointType::testBit(byte(7), 7))
        .setFlag(Attribute::WorkingDay, QKnxDatapointType::testBit(byte(7), 6))
        .setFlag(Attribute::NoWorkingDay, QKnxDatapointType::testBit(byte(7), 5))
        .setFlag(Attribute::NoYear, QKnxDatapointType::testBit(byte(7), 4))
        .setFlag(Attribute::NoDate, QKnxDatapointType::testBit(byte(7), 3))
        .setFlag(Attribute::NoDayOfWeek, QKnxDatapointType::testBit(byte(7), 2))
        .setFlag(Attribute::NoTime, QKnxDatapointType::testBit(byte(7), 1))
        .setFlag(Attribute::StandardSummerTime, QKnxDatapointType::testBit(byte(7), 0));
}

/*!
    Sets the additional information of the datapoint type to the list of
    attributes specified by \a attributes.
*/
void QKnxDateTime::setAttributes(Attributes attributes)
{
    quint8 byte = 0x00;
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::Fault), 7);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::WorkingDay), 6);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::NoWorkingDay), 5);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::NoYear), 4);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::NoDate), 3);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::NoDayOfWeek), 2);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::NoTime), 1);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::StandardSummerTime), 0);
    setByte(6, byte);
}

/*!
    Returns the clock quality stored in the datapoint type.
*/
QKnxDateTime::ClockQuality QKnxDateTime::clockQuality() const
{
    return ClockQuality(byte(7));
}

/*!
    Sets the clock quality to \a quality.
*/
void QKnxDateTime::setClockQuality(ClockQuality quality)
{
    setByte(7, QKnxDatapointType::setBit(byte(7), bool(quality), 7));
}

/*!
    \reimp
*/
bool QKnxDateTime::isValid() const
{
    auto tmpDate = date(); auto tmpTime = time();
    return QKnxDatapointType::isValid() && tmpDate.isValid() && tmpTime.isValid()
        && (tmpDate >= minimum().toList()[0].toDate())
        && (tmpDate <= maximum().toList()[0].toDate())
        && (tmpTime >= minimum().toList()[1].value<QKnxTime24>())
        && (tmpTime <= maximum().toList()[1].value<QKnxTime24>())
        && ((byte(7) == 0x00) || (byte(7) == 0x80));
}

#include "moc_qknxdatetime.cpp"

QT_END_NAMESPACE

