/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxnetipstructure.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpKnxAddressesDIB final : public QKnxNetIpStructure
{
public:
    QKnxNetIpKnxAddressesDIB() = default;

    explicit QKnxNetIpKnxAddressesDIB(const QKnxAddress &address);
    explicit QKnxNetIpKnxAddressesDIB(const QVector<QKnxAddress> &addresses);

    explicit QKnxNetIpKnxAddressesDIB(const QByteArray &data);
    explicit QKnxNetIpKnxAddressesDIB(const QVector<quint8> &data);

    QKnxNetIpKnxAddressesDIB fromRawData(const QByteArray &rawData, qint32 offset);
    QKnxNetIpKnxAddressesDIB fromRawData(const QVector<quint8> &rawData, qint32 offset);

    QVector<QKnxAddress> individualAddresses() const;

    bool isValid() const;

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::descriptionTypeCode;

private:
    QKnxNetIpKnxAddressesDIB(const QKnxNetIpStructure &other)
        : QKnxNetIpStructure(other)
    {}
};
Q_DECLARE_TYPEINFO(QKnxNetIpKnxAddressesDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
