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

class Q_KNX_EXPORT QKnxControlField2 final
{
public:
    QKnxControlField2() = default;
    explicit QKnxControlField2(quint8 data);
    explicit QKnxControlField2(const QByteArray &data);
    explicit QKnxControlField2(const QVector<quint8> &data);

    enum class DestinationAddress : quint8 {
        Individual = 0x00,
        Group = 0x01
    };
    QKnxControlField2::DestinationAddress destinationAddress() const;
    void setDestinationAddress(QKnxControlField2::DestinationAddress address);

    quint8 hopCount() const;
    void setHopCount(quint8 hopCount);

    enum class ExtendedFrameFormat {
        Standard = 0x00,
        Lte = 0x01
    };
    QKnxControlField2::ExtendedFrameFormat format() const;
    void setFormat(QKnxControlField2::ExtendedFrameFormat format);

    QString toString() const;
    quint8 rawData() const { return quint8(m_ctrl2.to_ulong()); }

#ifndef Q_QDOC
    template <typename T> auto rawData() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();
        T t(1, Qt::Uninitialized); t[0] = quint8(m_ctrl2.to_ulong());
        return t;
    }
#else
    template <typename T> auto rawData() const;
#endif

private:
    std::bitset<8> m_ctrl2 = 0;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxControlField2 &ctrl);
Q_KNX_EXPORT QDataStream &operator>>(QDataStream &stream, QKnxControlField2 &ctrl);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxControlField2 &ctrl);

Q_DECLARE_TYPEINFO(QKnxControlField2, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField2::DestinationAddress, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField2::ExtendedFrameFormat, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
