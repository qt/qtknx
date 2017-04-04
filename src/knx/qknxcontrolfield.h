/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXCONTROLFIELD_H
#define QKNXCONTROLFIELD_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxControlField final
{
public:
    QKnxControlField() = default;
    explicit QKnxControlField(quint8 data);
    explicit QKnxControlField(const QByteArray &data);
    explicit QKnxControlField(const QVector<quint8> &data);

    enum class FrameType : quint8
    {
        Extended = 0x00,
        Standard = 0x01
    };
    QKnxControlField::FrameType frameType() const { return static_cast<FrameType> (m_ctrl1[7]); }
    void setFrameType(QKnxControlField::FrameType type) { m_ctrl1[7] = static_cast<int> (type); }

    enum class Repeat : quint8
    {
        Repeat = 0x00,
        DoNotRepeat = 0x01
    };
    QKnxControlField::Repeat repeat() const { return static_cast<Repeat> (m_ctrl1[6]); }
    void setRepeat(QKnxControlField::Repeat repeat) { m_ctrl1[6] = static_cast<int> (repeat); }

    enum class Broadcast : quint8
    {
        System = 0x00,
        Domain = 0x01
    };
    QKnxControlField::Broadcast broadcast() const { return static_cast<Broadcast> (m_ctrl1[5]); }
    void setBroadcast(QKnxControlField::Broadcast bcst) { m_ctrl1[5] = static_cast<int> (bcst); }

    enum class Priority : quint8
    {
        System = 0x00,
        Normal = 0x01,
        Urgent = 0x02,
        Low = 0x03
    };
    QKnxControlField::Priority priority() const;
    void setPriority(QKnxControlField::Priority priority);

    enum class Acknowledge : quint8
    {
        NotRequested = 0x00,
        Requested = 0x01
    };
    QKnxControlField::Acknowledge acknowledge() const { return Acknowledge(m_ctrl1[1]); }
    void setAcknowledge(QKnxControlField::Acknowledge ack) { m_ctrl1[1] = static_cast<int> (ack); }

    enum class Confirm : quint8
    {
        NoError = 0x00,
        Error = 0x01
    };
    QKnxControlField::Confirm confirm() const { return static_cast<Confirm> (m_ctrl1[0]); }
    void setConfirm(QKnxControlField::Confirm confirm) { m_ctrl1[0] = static_cast<int> (confirm); }

    quint8 rawSize() const { return 1; }
    template <typename T = std::vector<quint8>> auto rawData() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(1, 0); t[0] = quint8(m_ctrl1.to_ulong());
        return t;
    }
    quint8 rawData() const { return quint8(m_ctrl1.to_ulong()); }

    QString toString() const;

private:
    std::bitset<8> m_ctrl1 = 0;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxControlField &ctrl);
Q_KNX_EXPORT QDataStream &operator>>(QDataStream &stream, QKnxControlField &ctrl);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxControlField &ctrl);

Q_DECLARE_TYPEINFO(QKnxControlField, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::FrameType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Repeat, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Broadcast, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Priority, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Acknowledge, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Confirm, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
