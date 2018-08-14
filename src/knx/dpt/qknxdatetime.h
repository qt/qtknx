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

#ifndef QKNXDATETIME_H
#define QKNXDATETIME_H

#include <QtCore/qdatetime.h>
#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxtime.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxTimeOfDay : public QKnxFixedSizeDatapointType
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
    bool setValue(const QKnxTime &time);

    quint8 hour() const;
    bool setHour(quint8 hour);

    quint8 minute() const;
    bool setMinute(quint8 minute);

    quint8 second() const;
    bool setSecond(quint8 second);

    QKnxTime::DayOfWeek dayOfDay() const;
    bool setDayOfWeek(QKnxTime::DayOfWeek day);

    bool isValid() const override;
};

class Q_KNX_EXPORT QKnxDate : public QKnxFixedSizeDatapointType
{
public:
    QKnxDate();
    explicit QKnxDate(const QDate &date);
    QKnxDate(quint16 year, quint8 month, quint8 day);

    static const constexpr int TypeSize = 0x03;
    static const constexpr int MainType = 0x0b;
    static const constexpr int SubType = 0x01;

    QDate value() const;
    bool setValue(const QDate &date);

    quint16 year() const;
    bool setYear(quint16 year);

    quint8 month() const;
    bool setMonth(quint8 month);

    quint8 day() const;
    bool setDay(quint8 day);

    bool isValid() const override;
};

class Q_KNX_EXPORT QKnxDateTime : public QKnxFixedSizeDatapointType
{
    Q_GADGET

public:
    enum Attribute
    {
        Fault = 0x00,
        WorkingDay = 0x01,
        NoWorkingDay = 0x02,
        NoYear = 0x04,
        NoDate = 0x08,
        NoDayOfWeek = 0x10,
        NoTime = 0x20,
        StandardSummerTime = 0x40
    };
    Q_ENUM(Attribute)
    Q_DECLARE_FLAGS(Attributes, Attribute)

    enum class ClockQuality
    {
        WithoutExtendedSyncSignal,
        WithExtendedSyncSignal = 0x01
    };
    Q_ENUM(ClockQuality)

    QKnxDateTime();
    QKnxDateTime(const QDate &date, const QKnxTime24 &time, Attributes attr, ClockQuality quality);

    static const constexpr int TypeSize = 0x08;
    static const constexpr int MainType = 0x13;
    static const constexpr int SubType = 0x01;

    bool setValue(const QDate &date, const QKnxTime24 &time, Attributes attr, ClockQuality quality);

    QDate date() const;
    bool setDate(const QDate &date);

    QKnxTime24 time() const;
    bool setTime(const QKnxTime24 &time);

    Attributes attributes() const;
    void setAttributes(Attributes attributes);

    ClockQuality clockQuality() const;
    void setClockQuality(ClockQuality quality);

    bool isValid() const override;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxDateTime::Attributes)

QT_END_NAMESPACE

#endif
