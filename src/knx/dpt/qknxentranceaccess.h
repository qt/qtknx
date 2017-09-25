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

#ifndef QKNXENTRANCEACCESS_H
#define QKNXENTRANCEACCESS_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxEntranceAccess : public QKnxDatapointType
{
    Q_GADGET

public:
    enum Attribute
    {
        Error = 0x01,
        PermissionAccepted = 0x02,
        ReadRightToLeft = 0x04,
        Encrypted = 0x08
    };
    Q_ENUM(Attribute)
    Q_DECLARE_FLAGS(Attributes, Attribute)

    QKnxEntranceAccess();
    QKnxEntranceAccess(quint32 idCode, Attributes attributes, quint8 index);

    static const constexpr int TypeSize = 0x04;
    static const constexpr int MainType = 0x0f;

    bool setValue(quint32 idCode, Attributes attributes, quint8 index);

    qint32 idCode()const;
    bool setIdCode(quint32 idCode);

    qint8 digit(quint8 x) const;
    bool setDigit(quint8 x, quint8 digit);

    Attributes attributes() const;
    bool isSet(Attribute attibute) const;
    void setAttributes(Attributes attributes);
    void setAttribute(Attribute attribute);
    void removeAttribute(Attribute attribute);

    quint8 index() const;
    bool setIndex(quint8 index);

    bool isValid() const override;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxEntranceAccess::Attributes)

QT_END_NAMESPACE

#endif
