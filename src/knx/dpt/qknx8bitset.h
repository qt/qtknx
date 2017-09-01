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

#ifndef QKNX8BITSET_H
#define QKNX8BITSET_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx8BitSet : public QKnxDatapointType
{
public:
    QKnx8BitSet();
    explicit QKnx8BitSet(quint8 byte);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x15;

    bool bit(int index) const;
    void setBit(bool value, int index);

    quint8 byte() const;
    void setByte(quint8 value);

    bool isValid() const override;

protected:
    explicit QKnx8BitSet(int subType);
    using QKnxDatapointType::setBit;
};

class Q_KNX_EXPORT QKnxGeneralStatus : public QKnx8BitSet
{
    Q_GADGET

public:
    enum Attribute : quint8
    {
        OutOfService = 0x01,
        Fault = 0x02,
        Overridden = 0x04,
        InAlarm = 0x08,
        AlarmUnacknowledged = 0x10
    };
    Q_ENUM(Attribute)
    Q_DECLARE_FLAGS(Attributes, Attribute)

    QKnxGeneralStatus();
    explicit QKnxGeneralStatus(Attributes attributes);

    static const constexpr int SubType = 0x01;

    Attributes value() const;
    void setValue(Attributes attributes);

    bool isSet(Attribute attribute) const;

    void setAttribute(Attribute attribute);
    void removeAttribute(Attribute attribute);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxGeneralStatus::Attributes)

QT_END_NAMESPACE

#endif
