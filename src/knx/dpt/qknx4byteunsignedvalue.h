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

#ifndef QKNX4BYTEUNSIGNEDVALUE_H
#define QKNX4BYTEUNSIGNEDVALUE_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx4ByteUnsignedValue : public QKnxFixedSizeDatapointType
{
public:

    QKnx4ByteUnsignedValue();
    explicit QKnx4ByteUnsignedValue(quint32 value);

    static const constexpr int TypeSize = 0x04;
    static const constexpr int MainType = 0x0c;

    quint32 value() const;
    bool setValue(quint32 value);

    bool isValid() const override;

protected:
    QKnx4ByteUnsignedValue(int subType, quint32 value);
};

class Q_KNX_EXPORT QKnxValue4UCount : public QKnx4ByteUnsignedValue
{
public :
    QKnxValue4UCount();
    explicit QKnxValue4UCount(quint32 value);
    static const constexpr int SubType = 0x01;
};

QT_END_NAMESPACE

#endif
