/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qtypeinfo.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxAddress final
{
public:
    enum struct Type : quint8 {
        Group = 0x00,
        Individual = 0x01
    };
    QKnxAddress::Type type() const;

    enum struct Notation : quint8 {
        TwoLevel = 0x00,
        ThreeLevel = 0x01,
    };
    QKnxAddress::Notation notation() const;

    QKnxAddress() = default;
    QKnxAddress(QKnxAddress::Type type, quint16 address);
    QKnxAddress(QKnxAddress::Type type, const QString &address);
    QKnxAddress(QKnxAddress::Type type, const QByteArray &address);
    QKnxAddress(QKnxAddress::Type type, const QVector<quint8> &address);

    static QKnxAddress createGroup(quint8 mainGroup, quint16 subGroup);
    static QKnxAddress createGroup(quint8 mainGroup, quint8 middleGroup, quint8 subGroup);

    static QKnxAddress createIndividual(quint8 area, quint8 line, quint8 busDevice);

    bool isValid() const;
    bool isBroadcast() const;

    bool isRouter() const;
    bool isUnregistered() const;

    QString toString() const;
    template <typename T> auto rawData() const -> decltype(T())
    {
        static_assert(IsType<T, QByteArray, QVector<quint8>>::value,
            "Only QByteArray or QVector<quint8> are supported as return type.");

        if (m_type != QKnxAddress::Type::Group && m_type != QKnxAddress::Type::Individual)
            return {};
        T t(2, Qt::Uninitialized); t[0] = quint8(m_address >> 8), t[1] = quint8(m_address);
        return t;
    }

    struct Q_KNX_EXPORT Group { static QKnxAddress Broadcast; };

private:
    QKnxAddress(QKnxAddress::Type type, quint16 sec1, quint16 *sec2, quint16 sec3);

#ifdef Q_QDOC
public: template <typename T> auto rawData() const; private:
#endif
    template <typename T, typename ... Ts> struct IsType
    {
        enum { value = false };
    };
    template <typename T, typename T1, typename ... Ts> struct IsType<T, T1, Ts...>
    {
        enum { value = std::is_same<T, T1>::value || IsType<T, Ts...>::value };
    };

private:
    qint32 m_address = -1;
    QKnxAddress::Type m_type = static_cast<QKnxAddress::Type>(0xff);
    QKnxAddress::Notation m_notation = QKnxAddress::Notation::ThreeLevel;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxAddress &address);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxAddress &address);

Q_DECLARE_TYPEINFO(QKnxAddress, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxAddress::Type, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxAddress::Notation, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
