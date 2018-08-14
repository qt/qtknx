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

#ifndef QKNX2BYTEFLOAT_H
#define QKNX2BYTEFLOAT_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qtknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnx2ByteFloat : public QKnxFixedSizeDatapointType
{
public:

    QKnx2ByteFloat();
    explicit QKnx2ByteFloat(float value);

    static const constexpr int TypeSize = 0x02;
    static const constexpr int MainType = 0x09;

    virtual float value() const;
    virtual bool setValue(float value);

    bool isValid() const override;

protected:
    QKnx2ByteFloat(int subType, float value);
};

#define CREATE_CLASS_DECLARATION(CLASS, SUB_TYPE) \
class Q_KNX_EXPORT CLASS : public QKnx2ByteFloat \
{ \
public: \
    CLASS(); \
    explicit CLASS(float value); \
    static const constexpr int TypeSize = 0x02; \
    static const constexpr int MainType = 0x09; \
    static const constexpr int SubType = SUB_TYPE; \
};

CREATE_CLASS_DECLARATION(QKnxTemperatureCelsius, 0x01)
CREATE_CLASS_DECLARATION(QKnxTemperatureKelvin, 0x02)
CREATE_CLASS_DECLARATION(QKnxTemperatureChange, 0x03)
CREATE_CLASS_DECLARATION(QKnxValueLux, 0x04)
CREATE_CLASS_DECLARATION(QKnxWindSpeed, 0x05)
CREATE_CLASS_DECLARATION(QKnxPressure, 0x06)
CREATE_CLASS_DECLARATION(QKnxHumidity, 0x07)
CREATE_CLASS_DECLARATION(QKnxAirQuality, 0x08)
CREATE_CLASS_DECLARATION(QKnxAirFlow, 0x09)
CREATE_CLASS_DECLARATION(QKnxTimeSecond, 0x0a)
CREATE_CLASS_DECLARATION(QKnxTimeMilliSecond, 0x0b)
CREATE_CLASS_DECLARATION(QKnxVoltage, 0x14)
CREATE_CLASS_DECLARATION(QKnxCurrent, 0x15)
CREATE_CLASS_DECLARATION(QKnxPowerDensity, 0x16)
CREATE_CLASS_DECLARATION(QKnxKelvinPerPercent, 0x17)
CREATE_CLASS_DECLARATION(QKnxPower, 0x18)
CREATE_CLASS_DECLARATION(QKnxVolumeFlow, 0x19)
CREATE_CLASS_DECLARATION(QKnxAmountRain, 0x1a)
CREATE_CLASS_DECLARATION(QKnxTemperatureFahrenheit, 0x1b)
CREATE_CLASS_DECLARATION(QKnxWindSpeedKmPerHour, 0x1c)
CREATE_CLASS_DECLARATION(QKnxValueAbsoluteHumidity, 0x1d)
CREATE_CLASS_DECLARATION(QKnxConcentration, 0x1e)

#undef CREATE_CLASS_DECLARATION

QT_END_NAMESPACE

#endif
