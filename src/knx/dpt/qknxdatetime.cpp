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

#include "qknxdatetime.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE


// -- QKnxTimeOfDay

QKnxTimeOfDay::QKnxTimeOfDay()
    : QKnxDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Time of day"));
    setRangeText(tr("No day, 00:00:00"), tr("Sunday, 23:59:59"));
    setRange(QVariant::fromValue(QKnxTime(00, 00, 00)), QVariant::fromValue(QKnxTime(23, 59, 59)));
}

QKnxTimeOfDay::QKnxTimeOfDay(const QKnxTime &time)
    : QKnxTimeOfDay()
{
    setValue(time);
}

QKnxTimeOfDay::QKnxTimeOfDay(quint8 h, quint8 m, quint8 s, QKnxTime::DayOfWeek day)
    : QKnxTimeOfDay()
{
    setValue({ h, m, s, day });
}

QKnxTime QKnxTimeOfDay::value() const
{
    return { quint8(operator[](0) & 0x1f), operator[](1), operator[](2),
        static_cast<QKnxTime::DayOfWeek> ((operator[](0) & 0xe0) >> 5u) };
}

void QKnxTimeOfDay::setValue(const QKnxTime &time)
{
    if (!time.isValid())
        return;

    operator[](2) = quint8(time.second());
    operator[](1) = quint8(time.minute());
    operator[](0) = quint8(time.hour()) | quint8(time.dayOfWeek()) << 5u;
}

quint8 QKnxTimeOfDay::hour() const
{
    return operator[](0) & 0x1f;
}

void QKnxTimeOfDay::setHour(quint8 hour)
{
    if (hour < 24)
        operator[](0) = quint8(operator[](0) &0xe0) | hour;
}

quint8 QKnxTimeOfDay::minute() const
{
    return operator[](1);
}

void QKnxTimeOfDay::setMinute(quint8 minute)
{
    if (minute < 60)
        operator[](1) = minute;
}

quint8 QKnxTimeOfDay::second() const
{
    return operator[](2);
}

void QKnxTimeOfDay::setSecond(quint8 second)
{
    if (second < 60)
        operator[](2) = second;
}

QKnxTime::DayOfWeek QKnxTimeOfDay::dayOfDay() const
{
    return QKnxTime::DayOfWeek(quint8(operator[](0) & 0xe0) >> 5u);
}

void QKnxTimeOfDay::setDayOfWeek(QKnxTime::DayOfWeek day)
{
    operator[](0) = quint8(day) << 5u | quint8(operator[](0) & 0x1f);
}

bool QKnxTimeOfDay::isValid() const
{
    auto time = value();
    return mainType() == MainType && size() == TypeSize && time.isValid()
        && (time >= minimum().value<QKnxTime>()) && (time <= maximum().value<QKnxTime>());
}


// -- QKnxDate

QKnxDate::QKnxDate()
    : QKnxDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Date"));
    setRange(QDate(1990, 1, 1), QDate(2089, 12, 31));
    setRangeText(tr("Monday, 1990-01-01"), tr("Saturday, 2089-12-31"));
}

QKnxDate::QKnxDate(const QDate &date)
    : QKnxDate()
{
    setValue(date);
}

QKnxDate::QKnxDate(quint16 year, quint8 month, quint8 day)
    : QKnxDate()
{
    setValue({ year, month, day });
}

QDate QKnxDate::value() const
{
    return { year(), month(), day() };
}

void QKnxDate::setValue(const QDate &date)
{
    if (date.isValid() && (date >= minimum().toDate()) && (date <= maximum().toDate())) {
        operator[](0) = date.day();
        operator[](1) = date.month();
        operator[](2) = quint8(date.year() % 100);
    }
}

quint16 QKnxDate::year() const
{
    quint16 year = operator[](2);
    return year + quint16(year < 90 ? 2000 : 1900);
}

void QKnxDate::setYear(quint16 year)
{
    if (year >= 1990 && year <= 2089)
        operator[](2) = quint8(year % 100);
}

quint8 QKnxDate::month() const
{
    return quint8(operator[](1));
}

void QKnxDate::setMonth(quint8 month)
{
    if (month <= 12)
        operator[](1) = month;
}

quint8 QKnxDate::day() const
{
    return quint8(operator[](0));
}

void QKnxDate::setDay(quint8 day)
{
    if (day <= 31)
        operator[](0) = day;
}

bool QKnxDate::isValid() const
{
    auto date = value();
    return mainType() == MainType && size() == TypeSize && date.isValid()
        && (date >= minimum().toDate()) && (date <= maximum().toDate());
}


// -- QKnxDateTime

QKnxDateTime::QKnxDateTime()
    : QKnxDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Date Time"));

    setMinimumText(tr("Monday, 1900-01-01; Any day, 00:00:00"));
    setMaximumText(tr("Wednesday, 2155-12-31; Sunday, 24:00:00"));
    setMinimum(QVariant({ QDate(1900, 01, 01), QVariant::fromValue(QKnxTime24(00, 00, 00)) }));
    setMaximum(QVariant({ QDate(2155, 12, 31), QVariant::fromValue(QKnxTime24(24, 00, 00)) }));
}

QKnxDateTime::QKnxDateTime(const QDate &date, const QKnxTime24 &time, Attributes attributes,
        ClockQuality quality)
    : QKnxDatapointType()
{
    setValue(date, time, attributes, quality);
}

void QKnxDateTime::setValue(const QDate &date, const QKnxTime24 &time, Attributes attributes,
        ClockQuality quality)
{
    setDate(date);
    setTime(time);
    setAttributes(attributes);
    setClockQuality(quality);
}

QDate QKnxDateTime::date() const
{
    return { operator[](0) + 1900, operator[](1), operator[](2) };
}

void QKnxDateTime::setDate(const QDate &date)
{
    if (date.isValid() && (date.year() >= 1900) && (date.year() <= 2155)) {
        operator[](0) = quint8(date.year() - 1900);
        operator[](1) = date.month();
        operator[](2) = date.day();
    }
}

QKnxTime24 QKnxDateTime::time() const
{
    return { operator[](3), operator[](4), operator[](5) };
}

void QKnxDateTime::setTime(const QKnxTime24 &time)
{
    if (time.isValid()) {
        operator[](3) = quint8(time.hour()) | quint8(time.dayOfWeek()) << 5u;
        operator[](4) = quint8(time.minute());
        operator[](5) = quint8(time.second());
    }
}

QKnxDateTime::Attributes QKnxDateTime::attributes() const
{
    return Attributes().setFlag(Attribute::Fault, testBit(operator[](7), 7))
        .setFlag(Attribute::WorkingDay, testBit(operator[](7), 6))
        .setFlag(Attribute::WorkingDay, testBit(operator[](7), 5))
        .setFlag(Attribute::YearInvalild, testBit(operator[](7), 4))
        .setFlag(Attribute::DateInvalid, testBit(operator[](7), 3))
        .setFlag(Attribute::DayOfWeekInvalid, testBit(operator[](7), 2))
        .setFlag(Attribute::TimeInvalid, testBit(operator[](7), 1))
        .setFlag(Attribute::StandardSummerTime, testBit(operator[](7), 0));
}

void QKnxDateTime::setAttributes(Attributes attributes)
{
    quint8 byte = 0x00;
    setBit(byte, attributes.testFlag(Attribute::Fault), 7);
    setBit(byte, attributes.testFlag(Attribute::WorkingDay), 6);
    setBit(byte, attributes.testFlag(Attribute::WorkingDayInvalid), 5);
    setBit(byte, attributes.testFlag(Attribute::YearInvalild), 4);
    setBit(byte, attributes.testFlag(Attribute::DateInvalid), 3);
    setBit(byte, attributes.testFlag(Attribute::DayOfWeekInvalid), 2);
    setBit(byte, attributes.testFlag(Attribute::TimeInvalid), 1);
    setBit(byte, attributes.testFlag(Attribute::StandardSummerTime), 0);
    operator[](6) = byte;
}

QKnxDateTime::ClockQuality QKnxDateTime::clockQuality() const
{
    return ClockQuality(operator[](7));
}

void QKnxDateTime::setClockQuality(ClockQuality quality)
{
    setBit(operator[](7), bool(quality), 7);
}

bool QKnxDateTime::isValid() const
{
    auto tmpDate = date(); auto tmpTime = time();
    return mainType() == MainType && size() == TypeSize && tmpDate.isValid() && tmpTime.isValid()
        && (tmpDate >= minimum().toList()[0].toDate())
        && (tmpDate <= maximum().toList()[0].toDate())
        && (tmpTime >= minimum().toList()[1].value<QKnxTime24>())
        && (tmpTime <= maximum().toList()[1].value<QKnxTime24>())
        && ((operator[](7) == 0x00) || (operator[](7) == 0x80));
}

#include "moc_qknxdatetime.cpp"

QT_END_NAMESPACE

