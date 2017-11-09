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

#ifndef QKNX8BYTESIGNEDVALUE_H
#define QKNX8BYTESIGNEDVALUE_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxElectricalEnergy : public QKnxFixedSizeDatapointType
{
public:
    QKnxElectricalEnergy();
    explicit QKnxElectricalEnergy(qint64 value);

    static const constexpr int TypeSize = 0x08;
    static const constexpr int MainType = 0x1d;

    qint64 value() const;
    bool setValue(qint64 value);

    bool isValid() const override;

protected:
    QKnxElectricalEnergy(int subType, qint64 value);
};

#define CREATE_CLASS_DECLARATION(CLASS, SUB_TYPE) \
class Q_KNX_EXPORT CLASS : public QKnxElectricalEnergy \
{ \
public: \
    CLASS(); \
    explicit CLASS(qint64 value); \
    static const constexpr int SubType = SUB_TYPE; \
};

CREATE_CLASS_DECLARATION(QKnxActiveEnergyV64, 0x0a)
CREATE_CLASS_DECLARATION(QKnxApparentEnergyV64, 0x0b)
CREATE_CLASS_DECLARATION(QKnxReactiveEnergyV64, 0x0c)

#undef CREATE_CLASS_DECLARATION

QT_END_NAMESPACE

#endif
