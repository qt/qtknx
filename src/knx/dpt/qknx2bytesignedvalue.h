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

#ifndef QKNX2BYTESIGNEDVALUE_H
#define QKNX2BYTESIGNEDVALUE_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx2ByteSignedValue : public QKnxFixedSizeDatapointType
{
public:
    QKnx2ByteSignedValue();
    explicit QKnx2ByteSignedValue(double value);

    static const constexpr int TypeSize = 0x02;
    static const constexpr int MainType = 0x08;

    double value() const;
    bool setValue(double value);

    bool isValid() const override;

protected:
    QKnx2ByteSignedValue(int subType, double value);
};

#define CREATE_CLASS_DECLARATION(CLASS, SUB_TYPE) \
class Q_KNX_EXPORT CLASS : public QKnx2ByteSignedValue \
{ \
public: \
    CLASS(); \
    explicit CLASS(double value); \
    static const constexpr int SubType = SUB_TYPE; \
};

CREATE_CLASS_DECLARATION(QKnxValue2Count, 0x01)
CREATE_CLASS_DECLARATION(QKnxPercentV16, 0x0a)
CREATE_CLASS_DECLARATION(QKnxDeltaTimeMsec, 0x02)
CREATE_CLASS_DECLARATION(QKnxDeltaTime10Msec, 0x03)
CREATE_CLASS_DECLARATION(QKnxDeltaTime100Msec, 0x04)
CREATE_CLASS_DECLARATION(QKnxDeltaTimeSec, 0x05)
CREATE_CLASS_DECLARATION(QKnxDeltaTimeMin, 0x06)
CREATE_CLASS_DECLARATION(QKnxDeltaTimeHrs, 0x07)
CREATE_CLASS_DECLARATION(QKnxRotationAngle, 0x0b)

#undef CREATE_CLASS_DECLARATION

QT_END_NAMESPACE

#endif
