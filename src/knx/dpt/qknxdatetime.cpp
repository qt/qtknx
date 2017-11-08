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

#include "qknxdatetime.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE


// -- QKnxTimeOfDay

QKnxTimeOfDay::QKnxTimeOfDay()
    : QKnxTimeOfDay(QKnxTime(0,0,0))
{}

QKnxTimeOfDay::QKnxTimeOfDay(const QKnxTime &time)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
{
    setDescription(tr("Time of day"));
    setRangeText(tr("No day, 00:00:00"), tr("Sunday, 23:59:59"));
    setRange(QVariant::fromValue(QKnxTime(00, 00, 00)), QVariant::fromValue(QKnxTime(23, 59, 59)));
    setValue(time);
}

QKnxTimeOfDay::QKnxTimeOfDay(quint8 h, quint8 m, quint8 s, QKnxTime::DayOfWeek day)
    : QKnxTimeOfDay()
{
    setValue({ h, m, s, day });
}

QKnxTime QKnxTimeOfDay::value() const
{
    return { quint8(byte(0) & 0x1f), byte(1), byte(2),
        static_cast<QKnxTime::DayOfWeek> ((byte(0) & 0xe0) >> 5u) };
}

bool QKnxTimeOfDay::setValue(const QKnxTime &time)
{
    if (!time.isValid())
        return false;

    return setByte(2, quint8(time.second()))
        && setByte(1, quint8(time.minute()))
        && setByte(0, quint8(time.hour()) | quint8(time.dayOfWeek()) << 5u);
}

quint8 QKnxTimeOfDay::hour() const
{
    return byte(0) & 0x1f;
}

bool QKnxTimeOfDay::setHour(quint8 hour)
{
    if (hour < 24)
        return setByte(0, quint8(byte(0) &0xe0) | hour);
    return false;
}

quint8 QKnxTimeOfDay::minute() const
{
    return byte(1);
}

bool QKnxTimeOfDay::setMinute(quint8 minute)
{
    if (minute < 60)
       return setByte(1, minute);
    return false;
}

quint8 QKnxTimeOfDay::second() const
{
    return byte(2);
}

bool QKnxTimeOfDay::setSecond(quint8 second)
{
    if (second < 60)
       return setByte(2, second);
    return false;
}

QKnxTime::DayOfWeek QKnxTimeOfDay::dayOfDay() const
{
    return QKnxTime::DayOfWeek(quint8(byte(0) & 0xe0) >> 5u);
}

bool QKnxTimeOfDay::setDayOfWeek(QKnxTime::DayOfWeek day)
{
    return setByte(0, quint8(day) << 5u | quint8(byte(0) & 0x1f));
}

bool QKnxTimeOfDay::isValid() const
{
    auto time = value();
    return QKnxDatapointType::isValid() && time.isValid()
        && (time >= minimum().value<QKnxTime>()) && (time <= maximum().value<QKnxTime>());
}


// -- QKnxDate

QKnxDate::QKnxDate()
    : QKnxDate(QDate(2000, 0, 0))
{
    setDescription(tr("Date"));
    setRange(QDate(1990, 1, 1), QDate(2089, 12, 31));
    setRangeText(tr("Monday, 1990-01-01"), tr("Saturday, 2089-12-31"));
}

QKnxDate::QKnxDate(const QDate &date)
    : QKnxFixedSizeDatapointType(MainType, SubType, TypeSize)
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

bool QKnxDate::setValue(const QDate &date)
{
    if (date.isValid() && (date >= minimum().toDate()) && (date <= maximum().toDate())) {
        return setByte(0, date.day())
            && setByte(1, date.month())
            && setByte(2, quint8(date.year() % 100));
    }
    return false;
}

quint16 QKnxDate::year() const
{
    quint16 year = byte(2);
    return year + quint16(year < 90 ? 2000 : 1900);
}

bool QKnxDate::setYear(quint16 year)
{
    if (year >= 1990 && year <= 2089)
        return setByte(2, quint8(year % 100));
    return false;
}

quint8 QKnxDate::month() const
{
    return byte(1);
}

bool QKnxDate::setMonth(quint8 month)
{
    if (month <= 12 && month > 0)
        return setByte(1, month);
    return false;
}

quint8 QKnxDate::day() const
{
    return byte(0);
}

bool QKnxDate::setDay(quint8 day)
{
    if (day <= 31 && day > 0)
        return setByte(0, day);
    return false;
}

bool QKnxDate::isValid() const
{
    auto date = value();
    return QKnxDatapointType::isValid() && date.isValid()
        && (date >= minimum().toDate()) && (date <= maximum().toDate());
}


// -- QKnxDateTime

QKnxDateTime::QKnxDateTime()
    : QKnxDateTime(QDate(2000, 0, 0), QKnxTime24(0, 0), Attributes(),
    ClockQuality::WithoutExtendedSyncSignal)
{}

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

bool QKnxDateTime::setValue(const QDate &date, const QKnxTime24 &time, Attributes attributes,
        ClockQuality quality)
{
    setAttributes(attributes);
    setClockQuality(quality);
    return setDate(date) && setTime(time);
}

QDate QKnxDateTime::date() const
{
    return { byte(0) + 1900, byte(1), byte(2) };
}

bool QKnxDateTime::setDate(const QDate &date)
{
    if (date.isValid() && (date.year() >= 1900) && (date.year() <= 2155)) {
        return setByte(0, quint8(date.year() - 1900))
            && setByte(1, date.month())
            && setByte(2, date.day());
    }
    return false;
}

QKnxTime24 QKnxDateTime::time() const
{
    return { byte(3), byte(4), byte(5) };
}

bool QKnxDateTime::setTime(const QKnxTime24 &time)
{
    if (time.isValid()) {
        return setByte(3, quint8(time.hour()) | quint8(time.dayOfWeek()) << 5u)
            && setByte(4, quint8(time.minute()))
            && setByte(5, quint8(time.second()));
    }
    return false;
}

QKnxDateTime::Attributes QKnxDateTime::attributes() const
{
    return Attributes().setFlag(Attribute::Fault, QKnxDatapointType::testBit(byte(7), 7))
        .setFlag(Attribute::WorkingDay, QKnxDatapointType::testBit(byte(7), 6))
        .setFlag(Attribute::WorkingDay, QKnxDatapointType::testBit(byte(7), 5))
        .setFlag(Attribute::YearInvalild, QKnxDatapointType::testBit(byte(7), 4))
        .setFlag(Attribute::DateInvalid, QKnxDatapointType::testBit(byte(7), 3))
        .setFlag(Attribute::DayOfWeekInvalid, QKnxDatapointType::testBit(byte(7), 2))
        .setFlag(Attribute::TimeInvalid, QKnxDatapointType::testBit(byte(7), 1))
        .setFlag(Attribute::StandardSummerTime, QKnxDatapointType::testBit(byte(7), 0));
}

void QKnxDateTime::setAttributes(Attributes attributes)
{
    quint8 byte = 0x00;
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::Fault), 7);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::WorkingDay), 6);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::WorkingDayInvalid), 5);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::YearInvalild), 4);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::DateInvalid), 3);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::DayOfWeekInvalid), 2);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::TimeInvalid), 1);
    byte = QKnxDatapointType::setBit(byte, attributes.testFlag(Attribute::StandardSummerTime), 0);
    setByte(6, byte);
}

QKnxDateTime::ClockQuality QKnxDateTime::clockQuality() const
{
    return ClockQuality(byte(7));
}

void QKnxDateTime::setClockQuality(ClockQuality quality)
{
    setByte(7, QKnxDatapointType::setBit(byte(7), bool(quality), 7));
}

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

