/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxnetipstructure.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpCRD final : public QKnxNetIpStructure
{
public:
    QKnxNetIpCRD() = default;

    QKnxNetIpCRD(ConnectionTypeCode type, const QByteArray &data);
    QKnxNetIpCRD(ConnectionTypeCode type, const QVector<quint8> &data);

    QKnxNetIpCRD fromRawData(const QByteArray &rawData, qint32 offset);
    QKnxNetIpCRD fromRawData(const QVector<quint8> &rawData, qint32 offset);

    bool isValid() const;

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::connectionTypeCode;

private:
    QKnxNetIpCRD(const QKnxNetIpStructure &other)
        : QKnxNetIpStructure(other)
    {}
};
Q_DECLARE_TYPEINFO(QKnxNetIpCRD, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
