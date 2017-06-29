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

#ifndef QKNXADDRESS_H
#define QKNXADDRESS_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxAddress final
{
public:
    enum class Type : quint8
    {
        Group = 0x00,
        Individual = 0x01
    };
    QKnxAddress::Type type() const;

    enum class Notation : quint8
    {
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
    static QKnxAddress createGroup(quint8 mainGroup, quint16 middleGroup, quint8 subGroup);

    static QKnxAddress createIndividual(quint8 area, quint16 line, quint8 sequentialNumber);

    bool isValid() const;
    bool isBroadcast() const;

    bool isUnregistered() const;
    bool isCouplerOrRouter() const;

    struct Q_KNX_EXPORT Group final { static QKnxAddress Broadcast; };
    struct Q_KNX_EXPORT Individual final { static QKnxAddress Unregistered; };

    quint8 size() const { return 2; }
    template <typename T = QByteArray> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        if (!isValid())
            return {};
        return QKnxUtils::QUint16::bytes<T>(quint16(m_address));
    }

    QString toString() const;

private:
    QKnxAddress(QKnxAddress::Type type, quint16 sec1, quint16 *sec2, quint16 sec3);

private:
    qint32 m_address = -1;
    QKnxAddress::Type m_type = static_cast<QKnxAddress::Type>(0xff);
    QKnxAddress::Notation m_notation = QKnxAddress::Notation::ThreeLevel;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxAddress &address);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxAddress &address);

Q_DECLARE_TYPEINFO(QKnxAddress::Type, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxAddress::Notation, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
