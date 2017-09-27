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

#include "qknxdatapointtype_p.h"
#include "qknxdatapointtypefactory.h"

#include "qknx1bit.h"
#include "qknx1bitcontrolled.h"
#include "qknx2bytefloat.h"
#include "qknx3bitcontrolled.h"
#include "qknx8bitset.h"
#include "qknx8bitsignedvalue.h"
#include "qknx8bitunsignedvalue.h"
#include "qknxdatetime.h"
#include "qknxentranceaccess.h"

QT_BEGIN_NAMESPACE

QKnxDatapointType *QKnxDatapointTypeFactory::createType(int mainType, int subType) const
{
    const auto main = factoryTable().constFind(mainType);
    if (main == factoryTable().constEnd())
        return nullptr;

    auto sub = (*main).constFind(subType);
    if (sub == (*main).constEnd())
        sub = (*main).constFind(0); // try base, e.g. 1.00[0]

    if (sub == (*main).constEnd())
        return nullptr;

    return (*sub)();
}

QKnxDatapointType *QKnxDatapointTypeFactory::createType(QKnxDatapointType::Type type) const
{
    auto number = QString::number(int(type));
    if (number.size() < 6)
        return nullptr;

    bool okMain = false, okSub = false;

    // Datapoint Type shall be identified by a 16 bit main number separated by a dot from a 16 bit
    // subnumber. The assumption being made is that QKnxDatapointType::Type is encoded in that way
    // while omitting the dot.
    int mainType = number.left(number.size() - 5).toInt(&okMain);
    int subType = number.right(5).toInt(&okSub);

    if (okMain && okSub)
        return createType(mainType, subType);
    return nullptr;
}

int QKnxDatapointTypeFactory::typeSize(int mainType)
{
    return sizeTable().value(mainType);
}

QList<int> QKnxDatapointTypeFactory::mainTypes() const
{
    return factoryTable().keys();
}

bool QKnxDatapointTypeFactory::containsMainType(int mainType) const
{
    return factoryTable().contains(mainType);
}

QList<int> QKnxDatapointTypeFactory::subTypes(int mainType) const
{
    const auto main = factoryTable().constFind(mainType);
    if (main != factoryTable().constEnd())
        return (*main).keys();
    return {};
}

bool QKnxDatapointTypeFactory::containsSubType(int mainType, int subType) const
{
    const auto main = factoryTable().constFind(mainType);
    if (main != factoryTable().constEnd())
        return (*main).constFind(subType) != (*main).constEnd();
    return false;
}

QKnxDatapointTypeFactory::QKnxDatapointTypeFactory()
{
    registerType<QKnx1Bit>();
    registerType<QKnxSwitch>();

    registerType<QKnx1BitControlled>();
    registerType<QKnxSwitchControl>();

    registerType<QKnx3BitControlled>();
    registerType<QKnxControlDimming>();
    registerType<QKnxControlBlinds>();

    registerType<QKnx8BitSignedValue>();
    registerType<QKnxPercentV8>();
    registerType<QKnxValue1Count>();

    registerType<QKnx2ByteFloat>();
    registerType<QKnxTemperatureCelcius>();
    registerType<QKnxTemperatureKelvin>();
    registerType<QKnxTemperatureChange>();
    registerType<QKnxBrightness>();
    registerType<QKnxWindSpeed>();
    registerType<QKnxPressure>();
    registerType<QKnxHumidity>();
    registerType<QKnxAirQuality>();
    registerType<QKnxTimeSecond>();
    registerType<QKnxTimeMilliSecond>();
    registerType<QKnxVoltage>();
    registerType<QKnxCurrent>();
    registerType<QKnxPowerDensity>();
    registerType<QKnxKelvinPerPercent>();
    registerType<QKnxPower>();
    registerType<QKnxVolumeFlow>();
    registerType<QKnxAmountRain>();
    registerType<QKnxTemperatureFahrenheit>();
    registerType<QKnxWindSpeedKmPerHour>();

    registerType<QKnx8BitUnsignedValue>();
    registerType<QKnxScaling>();
    registerType<QKnxAngle>();
    registerType<QKnxPercentU8>();
    registerType<QKnxDecimalFactor>();
    registerType<QKnxValue1Ucount>();
    registerType<QKnxTariff>();

    registerType<QKnx8BitSet>();
    registerType<QKnxGeneralStatus>();

    registerType<QKnxTimeOfDay>();
    registerType<QKnxDate>();

    registerType<QKnxEntranceAccess>();

    registerType<QKnxDateTime>();

    registerType<QKnx8BitSet>();
    registerType<QKnxGeneralStatus>();
}

QHash<int, int> &QKnxDatapointTypeFactory::sizeTable()
{
    static QHash<int, int> _instance;
    return _instance;
}

void QKnxDatapointTypeFactory::setTypeSize(int mainType, int size)
{
    sizeTable().insert(mainType, size);
}

QT_END_NAMESPACE
