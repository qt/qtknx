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

#ifndef QKNXEXTENDEDCONTROLFIELD_H
#define QKNXEXTENDEDCONTROLFIELD_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxExtendedControlField final
{
    Q_GADGET

public:
    QKnxExtendedControlField() = default;
    explicit QKnxExtendedControlField(quint8 data);
    explicit QKnxExtendedControlField(const QByteArray &data);
    explicit QKnxExtendedControlField(const QVector<quint8> &data);

    QKnxAddress::Type destinationAddressType() const;
    void setDestinationAddressType(QKnxAddress::Type address);

    quint8 hopCount() const;
    void setHopCount(quint8 hopCount);

    enum class ExtendedFrameFormat
    {
        Standard = 0x00,
        Lte = 0x01
    };
    Q_ENUM(ExtendedFrameFormat)
    QKnxExtendedControlField::ExtendedFrameFormat format() const;
    void setFormat(QKnxExtendedControlField::ExtendedFrameFormat format);

    quint8 bytes() const { return quint8(m_ctrl2.to_ulong()); }
    template <typename T = QByteArray> auto bytes() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        T t(1, 0); t[0] = quint8(m_ctrl2.to_ulong());
        return t;
    }

    QString toString() const;
    quint8 size() const { return 1; }

private:
    std::bitset<8> m_ctrl2 = 0;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxExtendedControlField &ctrl);
Q_KNX_EXPORT QDataStream &operator>>(QDataStream &stream, QKnxExtendedControlField &ctrl);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxExtendedControlField &ctrl);

Q_DECLARE_TYPEINFO(QKnxExtendedControlField::ExtendedFrameFormat, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
