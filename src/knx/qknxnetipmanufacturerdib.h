/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstructure.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpManufacturerDIB final : public QKnxNetIpStructure
{
public:
    QKnxNetIpManufacturerDIB() = default;

    explicit QKnxNetIpManufacturerDIB(quint16 manufacturerId);
    QKnxNetIpManufacturerDIB(quint16 manufacturerId, const QByteArray &manufacturerData);
    QKnxNetIpManufacturerDIB(quint16 manufacturerId, const QVector<quint8> &manufacturerData);

    explicit QKnxNetIpManufacturerDIB(const QByteArray &data);
    explicit QKnxNetIpManufacturerDIB(const QVector<quint8> &data);

    quint16 manufacturerId() const;
    template <typename T> auto manufacturerData() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>();
        return data<T>(2, dataSize() - 2);
    }

    QKnxNetIpManufacturerDIB fromRawData(const QByteArray &rawData, qint32 offset);
    QKnxNetIpManufacturerDIB fromRawData(const QVector<quint8> &rawData, qint32 offset);

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::descriptionTypeCode;
};
Q_DECLARE_TYPEINFO(QKnxNetIpManufacturerDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
