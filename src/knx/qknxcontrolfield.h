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

#ifndef QKNXCONTROLFIELD_H
#define QKNXCONTROLFIELD_H

#include <QtCore/qdebug.h>
#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qknxglobal.h>

#include <bitset>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxControlField final
{
    Q_GADGET

public:
    QKnxControlField() = default;
    explicit QKnxControlField(quint8 data);
    explicit QKnxControlField(const QKnxByteArray &data);

    enum class FrameFormat : quint8
    {
        Extended = 0x00,
        Standard = 0x01
    };
    Q_ENUM(FrameFormat)
    QKnxControlField::FrameFormat frameFormat() const { return static_cast<FrameFormat> (quint8(m_ctrl1[7])); }
    void setFrameFormat(QKnxControlField::FrameFormat type) { m_ctrl1[7] = static_cast<int> (type); }

    enum class Repeat : quint8
    {
        Repeat = 0x00,
        DoNotRepeat = 0x01
    };
    Q_ENUM(Repeat)
    QKnxControlField::Repeat repeat() const { return static_cast<Repeat> (quint8(m_ctrl1[5])); }
    void setRepeat(QKnxControlField::Repeat repeat) { m_ctrl1[5] = static_cast<int> (repeat); }

    enum class Broadcast : quint8
    {
        System = 0x00,
        Domain = 0x01
    };
    Q_ENUM(Broadcast)
    QKnxControlField::Broadcast broadcast() const { return static_cast<Broadcast> (quint8(m_ctrl1[4])); }
    void setBroadcast(QKnxControlField::Broadcast bcst) { m_ctrl1[4] = static_cast<int> (bcst); }

    enum class Priority : quint8
    {
        System = 0x00,
        Normal = 0x01,
        Urgent = 0x02,
        Low = 0x03
    };
    Q_ENUM(Priority)
    QKnxControlField::Priority priority() const;
    void setPriority(QKnxControlField::Priority priority);

    enum class Acknowledge : quint8
    {
        NotRequested = 0x00,
        Requested = 0x01
    };
    Q_ENUM(Acknowledge)
    QKnxControlField::Acknowledge acknowledge() const { return Acknowledge(quint8(m_ctrl1[1])); }
    void setAcknowledge(QKnxControlField::Acknowledge ack) { m_ctrl1[1] = static_cast<int> (ack); }

    enum class Confirm : quint8
    {
        NoError = 0x00,
        Error = 0x01
    };
    Q_ENUM(Confirm)
    QKnxControlField::Confirm confirm() const { return static_cast<Confirm> (quint8(m_ctrl1[0])); }
    void setConfirm(QKnxControlField::Confirm confirm) { m_ctrl1[0] = static_cast<int> (confirm); }

    quint8 byte() const { return quint8(m_ctrl1.to_ulong()); }
    QKnxByteArray bytes() const { return { byte() }; }

    quint8 size() const { return 1; }

private:
    std::bitset<8> m_ctrl1 = 0;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxControlField &ctrl);

Q_DECLARE_TYPEINFO(QKnxControlField::FrameFormat, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Repeat, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Broadcast, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Priority, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Acknowledge, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Confirm, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
