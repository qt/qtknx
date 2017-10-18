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

#ifndef QKNX8BITUNSIGNEDVALUE_H
#define QKNX8BITUNSIGNEDVALUE_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx8BitUnsignedValue : public QKnxDatapointType
{
public:
    QKnx8BitUnsignedValue();
    explicit QKnx8BitUnsignedValue(double value);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x05;

    double value() const;
    bool setValue(double value);

protected:
    QKnx8BitUnsignedValue(int subType, double value);
};

class Q_KNX_EXPORT QKnxTariff : public QKnx8BitUnsignedValue
{
public:
    QKnxTariff();
    explicit QKnxTariff(double value);
    static const constexpr int SubType = 0x06;

    bool isValid() const override;
};

#define CREATE_CLASS_DECLARATION(CLASS, SUB_TYPE) \
class Q_KNX_EXPORT CLASS : public QKnx8BitUnsignedValue \
{ \
public: \
    CLASS(); \
    explicit CLASS(double value); \
    static const constexpr int SubType = SUB_TYPE; \
};

CREATE_CLASS_DECLARATION(QKnxScaling, 0x01)
CREATE_CLASS_DECLARATION(QKnxAngle, 0x03)
CREATE_CLASS_DECLARATION(QKnxPercentU8, 0x04)
CREATE_CLASS_DECLARATION(QKnxDecimalFactor, 0x05)
CREATE_CLASS_DECLARATION(QKnxValue1Ucount, 0x0a)

#undef CREATE_CLASS_DECLARATION

QT_END_NAMESPACE

#endif
