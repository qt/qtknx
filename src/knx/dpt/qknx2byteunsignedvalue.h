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

#ifndef QKNX2BYTEUNSIGNEDVALUE_H
#define QKNX2BYTEUNSIGNEDVALUE_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx2ByteUnsignedValue : public QKnxDatapointType
{
public:
    QKnx2ByteUnsignedValue();
    explicit QKnx2ByteUnsignedValue(quint32 value);

    static const constexpr int TypeSize = 0x02;
    static const constexpr int MainType = 0x07;

    quint32 value() const;
    bool setValue(quint32 value);

    bool isValid() const override;

protected:
    QKnx2ByteUnsignedValue(int subType, quint32 value);
};


#define CREATE_CLASS_DECLARATION(CLASS, SUB_TYPE) \
class Q_KNX_EXPORT CLASS : public QKnx2ByteUnsignedValue \
{ \
public: \
    CLASS(); \
    explicit CLASS(quint32 value); \
    static const constexpr int SubType = SUB_TYPE; \
};

CREATE_CLASS_DECLARATION(QKnxValue2Ucount, 0x01)
CREATE_CLASS_DECLARATION(QKnxPropDataType, 0x0a)
CREATE_CLASS_DECLARATION(QKnxTimePeriodMsec, 0x02)
CREATE_CLASS_DECLARATION(QKnxTimePeriod10Msec, 0x03)
CREATE_CLASS_DECLARATION(QKnxTimePeriod100Msec, 0x04)
CREATE_CLASS_DECLARATION(QKnxTimePeriodSec, 0x05)
CREATE_CLASS_DECLARATION(QKnxTimePeriodMin, 0x06)
CREATE_CLASS_DECLARATION(QKnxTimePeriodHrs, 0x07)
CREATE_CLASS_DECLARATION(QKnxLengthMilliMeter, 0x0b)
CREATE_CLASS_DECLARATION(QKnxUEICurrentMilliA, 0x0c)
CREATE_CLASS_DECLARATION(QKnxBrightness, 0x0d)

#undef CREATE_CLASS_DECLARATION

QT_END_NAMESPACE

#endif
