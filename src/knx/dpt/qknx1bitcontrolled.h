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

#ifndef QKNX1BITCONTROLLED_H
#define QKNX1BITCONTROLLED_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx1BitControlled : public QKnxDatapointType
{
public:
    QKnx1BitControlled();

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x02;

    bool valueBit() const;
    void setValueBit(bool value);

    bool controlBit() const;
    void setControlBit(bool control);

    bool isValid() const override;

protected:
    explicit QKnx1BitControlled(int subType);
    QKnx1BitControlled(int subType, const QKnxDatapointType &dpt);
};

class Q_KNX_EXPORT QKnxSwitchControl : public QKnx1BitControlled
{
    Q_GADGET

public:
    enum Attribute : quint8
    {
        Off = 0x00,
        On = 0x01,
        NoControl = 0x02,
        Controlled = 0x04
    };
    Q_ENUM(Attribute)
    Q_DECLARE_FLAGS(Attributes, Attribute)

    QKnxSwitchControl();
    explicit QKnxSwitchControl(Attributes attributes);

    static const constexpr int SubType = 0x01;

    void setValue(Attributes attributes);
    QKnxSwitchControl::Attributes value() const;

    bool isSet(Attribute flag) const;

    void setAttribute(Attribute option);
    void removeAttribute(Attribute option);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxSwitchControl::Attributes)

QT_END_NAMESPACE

#endif
