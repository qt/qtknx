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
#include <QtKnx/qknxtraits.h>

#include <bitset>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxExtendedControlField final
{
public:
    QKnxExtendedControlField() = default;
    explicit QKnxExtendedControlField(quint8 data);
    explicit QKnxExtendedControlField(const QByteArray &data);
    explicit QKnxExtendedControlField(const QVector<quint8> &data);

    enum class DestinationAddressType : quint8
    {
        Individual = 0x00,
        Group = 0x01
    };
    QKnxExtendedControlField::DestinationAddressType destinationAddressType() const;
    void setDestinationAddressType(QKnxExtendedControlField::DestinationAddressType address);

    quint8 hopCount() const;
    void setHopCount(quint8 hopCount);

    enum class ExtendedFrameFormat
    {
        Standard = 0x00,
        Lte = 0x01
    };
    QKnxExtendedControlField::ExtendedFrameFormat format() const;
    void setFormat(QKnxExtendedControlField::ExtendedFrameFormat format);

    quint8 rawSize() const { return 1; }
    template <typename T> auto rawData() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(1, Qt::Uninitialized); t[0] = quint8(m_ctrl2.to_ulong());
        return t;
    }
    quint8 rawData() const { return quint8(m_ctrl2.to_ulong()); }

    QString toString() const;

private:
    std::bitset<8> m_ctrl2 = 0;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxExtendedControlField &ctrl);
Q_KNX_EXPORT QDataStream &operator>>(QDataStream &stream, QKnxExtendedControlField &ctrl);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxExtendedControlField &ctrl);

Q_DECLARE_TYPEINFO(QKnxExtendedControlField, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxExtendedControlField::DestinationAddressType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxExtendedControlField::ExtendedFrameFormat, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
