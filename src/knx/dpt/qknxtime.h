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

#ifndef QKNXTIME_H
#define QKNXTIME_H

#include <QtCore/qmetatype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QAbstractKnxTime
{
    Q_GADGET

public:
    enum DayOfWeek : quint8
    {
        Ignore = 0,
        Monday = 1,
        Tuesday = 2,
        Wednesday = 3,
        Thursday = 4,
        Friday = 5,
        Saturday = 6,
        Sunday = 7
    };
    Q_ENUM(DayOfWeek)
    virtual ~QAbstractKnxTime() = 0;
};

template<typename Derived> class QKnxTimeBase : public QAbstractKnxTime
{
public:
    QKnxTimeBase() = default;
    QKnxTimeBase(quint8 h, quint8 m, quint8 s = 0, DayOfWeek day = DayOfWeek::Ignore)
    { setHMS(h, m, s); m_day = day; }

    bool isNull() const { return mds == NullTime; }

    static bool isValid(qint8 h, qint8 m, qint8 s) { return Derived::isValid(h, m, s); }
    bool isValid() const { return static_cast<const Derived *> (this)->isValid(); }

    qint8 hour() const { return (isValid() ? ds() / MsecPerHour : -1); }
    qint8 minute() const { return (isValid() ? (ds() % MsecPerHour) / MsecPerMinute : -1); }
    qint8 second() const { return (isValid() ? (ds() / 1000) % SecondsPerMinute : -1); }

    void setHMS(quint8 h, quint8 m, quint8 s)
    {
        if (isValid(h, m, s))
            mds = (h * SecondsPerHour + m * SecondsPerMinute + s) * 1000;
    }

    DayOfWeek dayOfWeek() const { return m_day; }
    void setDayOfWeek(DayOfWeek day) { m_day = day; }

    bool operator==(const QKnxTimeBase &o) const { return mds == o.mds && m_day == o.m_day; }
    bool operator!=(const QKnxTimeBase &o) const { return mds != o.mds && m_day != o.m_day; }
    bool operator< (const QKnxTimeBase &o) const { return mds < o.mds && m_day < o.m_day; }
    bool operator<=(const QKnxTimeBase &o) const { return mds <= o.mds && m_day <= o.m_day; }
    bool operator> (const QKnxTimeBase &o) const { return mds > o.mds && m_day > o.m_day; }
    bool operator>=(const QKnxTimeBase &o) const { return mds >= o.mds && m_day >= o.m_day; }

protected:
    enum TimeFlag
    {
        NullTime = -1,
        MsecPerDay = 86400000,
        SecondsPerHour = 3600,
        MsecPerHour = 3600000,
        SecondsPerMinute = 60,
        MsecPerMinute = 60000
    };
    inline int ds() const { return mds; }

private:
    int mds { TimeFlag::NullTime };
    DayOfWeek m_day { DayOfWeek::Ignore };
};

class Q_KNX_EXPORT QKnxTime : public QKnxTimeBase<QKnxTime>
{
public:
    using QKnxTimeBase::QKnxTimeBase;

    bool isValid() const;
    static bool isValid(quint8 h, quint8 m, quint8 s);
};

class Q_KNX_EXPORT QKnxTime24 : public QKnxTimeBase<QKnxTime24>
{
public:
    using QKnxTimeBase::QKnxTimeBase;

    bool isValid() const;
    static bool isValid(quint8 h, quint8 m, quint8 s);
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QKnxTime)
Q_DECLARE_METATYPE(QKnxTime24)

#endif
