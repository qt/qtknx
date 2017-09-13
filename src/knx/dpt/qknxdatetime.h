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

#ifndef QKNXDATETIME_H
#define QKNXDATETIME_H

#include <QtCore/qdatetime.h>
#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtime.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxTimeOfDay : public QKnxDatapointType
{
public:
    QKnxTimeOfDay();
    explicit QKnxTimeOfDay(const QKnxTime &time);
    QKnxTimeOfDay(quint8 hour, quint8 minute, quint8 second = 0,
                  QKnxTime::DayOfWeek day = QKnxTime::DayOfWeek::Ignore);

    static const constexpr int TypeSize = 0x03;
    static const constexpr int MainType = 0x0a;
    static const constexpr int SubType = 0x01;

    QKnxTime value() const;
    void setValue(const QKnxTime &time);

    quint8 hour() const;
    void setHour(quint8 hour);

    quint8 minute() const;
    void setMinute(quint8 minute);

    quint8 second() const;
    void setSecond(quint8 second);

    QKnxTime::DayOfWeek dayOfDay() const;
    void setDayOfWeek(QKnxTime::DayOfWeek day);

    bool isValid() const override;
};

class Q_KNX_EXPORT QKnxDate : public QKnxDatapointType
{
public:
    QKnxDate();
    explicit QKnxDate(const QDate &date);
    QKnxDate(quint16 year, quint8 month, quint8 day);

    static const constexpr int TypeSize = 0x03;
    static const constexpr int MainType = 0x0b;
    static const constexpr int SubType = 0x01;

    QDate value() const;
    void setValue(const QDate &date);

    quint16 year() const;
    void setYear(quint16 year);

    quint8 month() const;
    void setMonth(quint8 month);

    quint8 day() const;
    void setDay(quint8 day);

    bool isValid() const override;
};

class Q_KNX_EXPORT QKnxDateTime : public QKnxDatapointType
{
    Q_GADGET

public:
    enum Attribute
    {
        Fault = 0x00,
        WorkingDay = 0x01,
        WorkingDayInvalid = 0x02,
        YearInvalild = 0x04,
        DateInvalid = 0x08,
        DayOfWeekInvalid = 0x10,
        TimeInvalid = 0x20,
        StandardSummerTime = 0x40
    };
    Q_ENUM(Attribute)
    Q_DECLARE_FLAGS(Attributes, Attribute)

    enum class ClockQuality
    {
        WithoutExtendedSyncSignal,
        WithExtendedSyncSignal = 0x01
    };
    QKnxDateTime();
    QKnxDateTime(const QDate &date, const QKnxTime24 &time, Attributes attr, ClockQuality quality);

    static const constexpr int TypeSize = 0x08;
    static const constexpr int MainType = 0x13;
    static const constexpr int SubType = 0x01;

    void setValue(const QDate &date, const QKnxTime24 &time, Attributes attr, ClockQuality quality);

    QDate date() const;
    void setDate(const QDate &date);

    QKnxTime24 time() const;
    void setTime(const QKnxTime24 &time);

    Attributes attributes() const;
    void setAttributes(Attributes attributes);

    ClockQuality clockQuality() const;
    void setClockQuality(ClockQuality quality);

    bool isValid() const override;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxDateTime::Attributes)

QT_END_NAMESPACE

#endif
