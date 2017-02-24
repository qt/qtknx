/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxnetipstructure.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpCRI final : public QKnxNetIpStructure
{
public:
    QKnxNetIpCRI() = default;

    QKnxNetIpCRI(ConnectionTypeCode type, const QByteArray &data);
    QKnxNetIpCRI(ConnectionTypeCode type, const QVector<quint8> &data);

    QKnxNetIpCRI fromRawData(const QByteArray &rawData, qint32 offset);
    QKnxNetIpCRI fromRawData(const QVector<quint8> &rawData, qint32 offset);

    bool isValid() const;

    using QKnxNetIpStructure::toString;
    using QKnxNetIpStructure::connectionTypeCode;

private:
    QKnxNetIpCRI(const QKnxNetIpStructure &other)
        : QKnxNetIpStructure(other)
    {}
};
Q_DECLARE_TYPEINFO(QKnxNetIpCRI, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
