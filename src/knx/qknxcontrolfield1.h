/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtCore/qdatastream.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtypecheck.h>

#include <bitset>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxControlField1 final
{
public:
    QKnxControlField1() = default;
    explicit QKnxControlField1(quint8 data);
    explicit QKnxControlField1(const QByteArray &data);
    explicit QKnxControlField1(const QVector<quint8> &data);

    enum class Type : quint8 {
        Extended = 0x00,
        Standard = 0x01
    };
    QKnxControlField1::Type type() const { return static_cast<Type> (m_ctrl1[7]); }
    void setType(QKnxControlField1::Type type) { m_ctrl1[7] = static_cast<int> (type); }

    enum class Repeat : quint8 {
        Repeat = 0x00,
        DoNotRepeat = 0x01
    };
    QKnxControlField1::Repeat repeat() const { return static_cast<Repeat> (m_ctrl1[6]); }
    void setRepeat(QKnxControlField1::Repeat repeat) { m_ctrl1[6] = static_cast<int> (repeat); }

    enum class Broadcast : quint8 {
        System = 0x00,
        Domain = 0x01
    };
    QKnxControlField1::Broadcast broadcast() const { return static_cast<Broadcast> (m_ctrl1[5]); }
    void setBroadcast(QKnxControlField1::Broadcast bcst) { m_ctrl1[5] = static_cast<int> (bcst); }

    enum class Priority : quint8 {
        System = 0x00,
        Normal = 0x01,
        Urgent = 0x02,
        Low = 0x03
    };
    QKnxControlField1::Priority priority() const;
    void setPriority(QKnxControlField1::Priority priority);

    enum class Acknowledge : quint8 {
        NotRequested = 0x00,
        Requested = 0x01
    };
    QKnxControlField1::Acknowledge acknowledge() const { return Acknowledge(m_ctrl1[1]); }
    void setAcknowledge(QKnxControlField1::Acknowledge ack) { m_ctrl1[1] = static_cast<int> (ack); }

    enum class Confirm : quint8 {
        NoError = 0x00,
        Error = 0x01
    };
    QKnxControlField1::Confirm confirm() const { return static_cast<Confirm> (m_ctrl1[0]); }
    void setConfirm(QKnxControlField1::Confirm confirm) { m_ctrl1[0] = static_cast<int> (confirm); }

    QString toString() const;
    quint8 rawData() const { return quint8(m_ctrl1.to_ulong()); }

#ifndef Q_QDOC
    template <typename T> auto rawData() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();
        T t(1, Qt::Uninitialized); t[0] = quint8(m_ctrl1.to_ulong());
        return t;
    }
#else
    template <typename T> auto rawData() const;
#endif

private:
    std::bitset<8> m_ctrl1 = 0;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxControlField1 &ctrl);
Q_KNX_EXPORT QDataStream &operator>>(QDataStream &stream, QKnxControlField1 &ctrl);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxControlField1 &ctrl);

Q_DECLARE_TYPEINFO(QKnxControlField1, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField1::Type, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField1::Repeat, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField1::Broadcast, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField1::Priority, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField1::Acknowledge, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField1::Confirm, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
