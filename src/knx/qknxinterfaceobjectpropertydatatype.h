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

#ifndef QKNXINTERFACEOBJECTPROPERTYDATATYPE_H
#define QKNXINTERFACEOBJECTPROPERTYDATATYPE_H

#include <QtCore/qobject.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxinterfaceobjectproperty.h>

QT_BEGIN_NAMESPACE

struct QKnxInterfaceObjectPropertyDataTypePrivate;

class Q_KNX_EXPORT QKnxInterfaceObjectPropertyDataType final
{
    Q_GADGET

public:
    enum class Unit : quint8
    {
        Array,
        Single
    };

    enum class Id : quint8
    {
        Control = 0x00,
        Char = 0x01,
        UnsignedChar = 0x02,
        Int = 0x03,
        UnsignedInt = 0x04,
        KnxFloat = 0x05,
        Date = 0x06,
        Time = 0x07,
        Long = 0x08,
        UnsignedLong = 0x09,
        Float = 0x0a,
        Double = 0x0b,
        CharBlock = 0x0c,
        PollGroupSettings = 0x0d,
        ShortCharBlock = 0x0e,
        DateTime = 0x0f,
        VariableLength = 0x10,
        Generic01 = 0x11,
        Generic02 = 0x12,
        Generic03 = 0x13,
        Generic04 = 0x14,
        Generic05 = 0x15,
        Generic06 = 0x16,
        Generic07 = 0x17,
        Generic08 = 0x18,
        Generic09 = 0x19,
        Generic10 = 0x1a,
        Generic11 = 0x1b,
        Generic12 = 0x1c,
        Generic13 = 0x1d,
        Generic14 = 0x1e,
        Generic15 = 0x1f,
        Generic16 = 0x20,
        Generic17 = 0x21,
        Generic18 = 0x22,
        Generic19 = 0x23,
        Generic20 = 0x24,
        Reserved01 = 0x25,
        Reserved02 = 0x26,
        Reserved03 = 0x27,
        Reserved04 = 0x28,
        Reserved05 = 0x29,
        Reserved06 = 0x2a,
        Reserved07 = 0x2b,
        Reserved08 = 0x2c,
        Reserved09 = 0x2d,
        Reserved10 = 0x2e,
        Utf8 = 0x2f,
        Version = 0x30,
        AlarmInfo = 0x31,
        BinaryInformation = 0x32,
        Bitset8 = 0x33,
        Bitset16 = 0x34,
        Enum8 = 0x35,
        Scaling = 0x36,
        Reserved11 = 0x37,
        Reserved12 = 0x38,
        Reserved13 = 0x39,
        Reserved14 = 0x3a,
        Reserved15 = 0x3b,
        NeVl = 0x3c,
        NeFl = 0x3d,
        Function = 0x3e,
        Escape = 0x3f,
        Invalid = 0xff
    };
    Q_ENUM(Id)

    QKnxInterfaceObjectPropertyDataType();
    QKnxInterfaceObjectPropertyDataType(QKnxInterfaceObjectPropertyDataType::Id id, quint8 size,
        Unit unit = Unit::Single);
    ~QKnxInterfaceObjectPropertyDataType();

    bool isValid() const;
    quint8 size(bool read = false) const;

    QKnxInterfaceObjectPropertyDataType::Id id() const;
    QKnxInterfaceObjectPropertyDataType::Unit unit() const;

    static quint8 size(QKnxInterfaceObjectPropertyDataType::Id id, bool read = false);
    static QVector<QKnxInterfaceObjectPropertyDataType> fromProperty(QKnxInterfaceObjectProperty p);

    QKnxInterfaceObjectPropertyDataType(const QKnxInterfaceObjectPropertyDataType &o);
    QKnxInterfaceObjectPropertyDataType &operator=(const QKnxInterfaceObjectPropertyDataType &o);

    void swap(QKnxInterfaceObjectPropertyDataType &other) Q_DECL_NOTHROW;
#ifdef Q_COMPILER_RVALUE_REFS
    QKnxInterfaceObjectPropertyDataType
        &operator=(QKnxInterfaceObjectPropertyDataType &&other) Q_DECL_NOTHROW;
#endif

private:
    explicit QKnxInterfaceObjectPropertyDataType(QKnxInterfaceObjectPropertyDataTypePrivate &dd);

private:
    QExplicitlySharedDataPointer<QKnxInterfaceObjectPropertyDataTypePrivate> d_ptr;
};

QT_END_NAMESPACE

#endif
