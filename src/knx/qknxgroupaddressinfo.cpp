/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qknxgroupaddressinfo.h"

QT_BEGIN_NAMESPACE

//-- QKnxGroupAddressInfoPrivate

class QKnxGroupAddressInfoPrivate : public QSharedData
{
public:
    QKnxGroupAddressInfoPrivate() = default;
    ~QKnxGroupAddressInfoPrivate() = default;

    QKnxAddress groupAddress {};
    QKnxGroupAddress groupAddressEntry;
    QKnxDatapointType::Type datapointType = QKnxDatapointType::Type::Unknown;
};

static QKnxDatapointType::Type dptNumbersFromQString(const QString &dptType)
{
    // Format: DPT-mainType-subType
    if (dptType.size() < 7)
        return {};

    auto mainSubType = dptType.split(QString::fromLatin1("-"));
    if (mainSubType.size() != 3)
        return {};

    return QKnxDatapointType::Type(100000 * mainSubType.value(1).toUInt() + mainSubType.value(2)
        .toUInt());
}


//-- QKnxGroupAddressInfo

QKnxGroupAddressInfo::QKnxGroupAddressInfo()
    : d(new QKnxGroupAddressInfoPrivate)
{}

QKnxGroupAddressInfo::~QKnxGroupAddressInfo()
{}

QKnxGroupAddressInfo::QKnxGroupAddressInfo(const QKnxGroupAddress &groupAddressFromParser)
    : d(new QKnxGroupAddressInfoPrivate)
{
    d->groupAddressEntry = groupAddressFromParser;
    d->groupAddress = { QKnxAddress::Type::Group, quint16(d->groupAddressEntry.Address) };
    d->datapointType = dptNumbersFromQString(groupAddressFromParser.DatapointType);
}

QKnxGroupAddressInfo::QKnxGroupAddressInfo(const QString &id, const QString &address,
        const QString &name, const QString &description, const QString &dataPointType,
        const QString &puid)
    : QKnxGroupAddressInfo({ id, address.toUShort(), name, puid.toInt() })
{
    d->groupAddressEntry.Description = description;
    d->groupAddressEntry.DatapointType = dataPointType;
    d->datapointType = dptNumbersFromQString(d->groupAddressEntry.DatapointType);
}
QString QKnxGroupAddressInfo::id() const
{
    return d->groupAddressEntry.Id;
}

QString QKnxGroupAddressInfo::address() const
{
    return d->groupAddress.toString();
}

QString QKnxGroupAddressInfo::name() const
{
    return d->groupAddressEntry.Name;
}

QString QKnxGroupAddressInfo::description() const
{
    return d->groupAddressEntry.Description;
}

QString QKnxGroupAddressInfo::dataPointNumber() const
{
    return d->groupAddressEntry.DatapointType;
}

QString QKnxGroupAddressInfo::puid() const
{
    return QString::number(d->groupAddressEntry.Puid);
}

QKnxAddress QKnxGroupAddressInfo::groupAddress() const
{
    return d->groupAddress;
}

QKnxDatapointType::Type QKnxGroupAddressInfo::dataPointType() const
{
    return d->datapointType;
}

QKnxGroupAddressInfo::QKnxGroupAddressInfo(const QKnxGroupAddressInfo &other)
    : d(other.d)
{}

QKnxGroupAddressInfo &QKnxGroupAddressInfo::operator=(const QKnxGroupAddressInfo &other)
{
    d = other.d;
    return *this;
}

QKnxGroupAddressInfo &QKnxGroupAddressInfo::operator=(QKnxGroupAddressInfo &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

bool QKnxGroupAddressInfo::operator==(const QKnxGroupAddressInfo &other) const
{
    return d == other.d || [&]() -> bool {
        return d->groupAddress == other.d->groupAddress
            && d->groupAddressEntry == d->groupAddressEntry
            && d->datapointType == d->datapointType;
    }();
}

bool QKnxGroupAddressInfo::operator!=(const QKnxGroupAddressInfo &other) const
{
    return !operator==(other);
}

void QKnxGroupAddressInfo::swap(QKnxGroupAddressInfo &other) Q_DECL_NOTHROW
{
    d.swap(other.d);
}

QKnxGroupAddressInfo::QKnxGroupAddressInfo(QKnxGroupAddressInfoPrivate &dd)
    : d(new QKnxGroupAddressInfoPrivate(dd))
{}

QT_END_NAMESPACE
