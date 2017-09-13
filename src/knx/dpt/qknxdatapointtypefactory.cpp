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

#include "qknxdatapointtype_p.h"
#include "qknxdatapointtypefactory.h"

#include "qknx1bit.h"
#include "qknx1bitcontrolled.h"
#include "qknx8bitset.h"
#include "qknxdatetime.h"

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

    registerType<QKnx8BitSet>();
    registerType<QKnxGeneralStatus>();

    registerType<QKnxTimeOfDay>();
    registerType<QKnxDate>();
    registerType<QKnxDateTime>();
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
