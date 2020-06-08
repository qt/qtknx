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

#ifndef QKNXADDRESS_H
#define QKNXADDRESS_H

#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxAddress final
{
public:
    enum class Type : quint8
    {
        Group = 0x01,
        Individual = 0x00
    };
    QKnxAddress::Type type() const;

    enum class Notation : quint8
    {
        TwoLevel = 0x00,
        ThreeLevel = 0x01,
    };

    QKnxAddress() = default;
    QKnxAddress(QKnxAddress::Type type, quint16 address);
    QKnxAddress(QKnxAddress::Type type, const QString &address);
    QKnxAddress(QKnxAddress::Type type, const QKnxByteArray &address);

    static QKnxAddress createGroup(quint8 mainGroup, quint16 subGroup);
    static QKnxAddress createGroup(quint8 mainGroup, quint16 middleGroup, quint8 subGroup);

    static QKnxAddress createIndividual(quint8 area, quint16 line, quint8 sequentialNumber);

    quint8 mainOrAreaSection() const;
    quint8 middleOrLineSection() const;
    quint16 subOrDeviceSection(Notation notation = Notation::ThreeLevel) const;

    bool isValid() const;
    bool isBroadcast() const;

    bool isUnregistered() const;
    bool isCouplerOrRouter() const;

    struct Q_KNX_EXPORT Group final { static QKnxAddress Broadcast; };
    struct Q_KNX_EXPORT Individual final { static QKnxAddress Unregistered; };

    quint8 size() const { return 2; }
    QKnxByteArray bytes() const
    {
        if (!isValid())
            return {};
        return QKnxUtils::QUint16::bytes(quint16(m_address));
    }

    QString toString(Notation notation = Notation::ThreeLevel) const;

    bool operator==(const QKnxAddress &other) const;
    bool operator!=(const QKnxAddress &other) const;

private:
    QKnxAddress(QKnxAddress::Type type, quint16 sec1, quint16 *sec2, quint16 sec3);

private:
    qint32 m_address = -1;
    QKnxAddress::Type m_type = static_cast<QKnxAddress::Type>(0xff);
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxAddress &address);
Q_KNX_EXPORT Q_DECL_PURE_FUNCTION size_t qHash(const QKnxAddress &key, uint seed = 0) Q_DECL_NOTHROW;

Q_DECLARE_TYPEINFO(QKnxAddress::Type, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxAddress::Notation, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
