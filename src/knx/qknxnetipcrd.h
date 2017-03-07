/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpCRD : private QKnxNetIpStruct
{
public:
    QKnxNetIpCRD() = default;

    template <typename T> QKnxNetIpCRD(QKnxNetIp::ConnectionTypeCode type, const T &bytes)
        : QKnxNetIpStruct(quint8(type), bytes)
    {}

    template <typename T> QKnxNetIpCRD fromBytes(const T &bytes, qint32 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

    QKnxNetIp::ConnectionTypeCode connectionTypeCode() const;

    bool isValid() const override;

    using QKnxNetIpStruct::size;
    using QKnxNetIpStruct::bytes;
    using QKnxNetIpStruct::payload;
    using QKnxNetIpStruct::toString;

private:
    QKnxNetIpCRD(const QKnxNetIpStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpCRD, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
