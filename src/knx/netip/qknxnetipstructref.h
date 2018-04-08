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

#ifndef QKNXNETIPSTRUCTREF_H
#define QKNXNETIPSTRUCTREF_H

#include <QtKnx/qknxnetipconfigdib.h>
#include <QtKnx/qknxnetipcrd.h>
#include <QtKnx/qknxnetipcurrentconfigdib.h>
#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>

QT_BEGIN_NAMESPACE

namespace QKnxPrivate
{
    template<typename T> struct QKnxNetIpStructTypeHelper;
}

class Q_KNX_EXPORT QKnxNetIpStructRef final
{
    friend class QKnxNetIpDescriptionResponse;
    template<typename T> friend struct QKnxPrivate::QKnxNetIpStructTypeHelper;

public:
    enum class Type : quint8
    {
        Null,
        QKnxNetIpConfigDib,
        QKnxNetIpCurrentConfigDib,
        QKnxNetIpDeviceDib
    };

    QKnxNetIpStructRef() = default;
    ~QKnxNetIpStructRef() = default;

    explicit QKnxNetIpStructRef(QKnxNetIpConfigDib *ipConfigDib)
        : a(ipConfigDib)
        , m_type(Type::QKnxNetIpConfigDib)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCurrentConfigDib *ipCurrentConfigDib)
        : d(ipCurrentConfigDib)
        , m_type(Type::QKnxNetIpCurrentConfigDib)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpDeviceDib *deviceDib)
        : e(deviceDib)
        , m_type(Type::QKnxNetIpDeviceDib)
    {}

    QKnxNetIpStructRef(const QKnxNetIpStructRef &) = default;
    QKnxNetIpStructRef &operator=(const QKnxNetIpStructRef &) = default;

    bool isNull() const { return m_type == Type::Null; }
    bool isIpConfigDib() const { return m_type == Type::QKnxNetIpConfigDib; }
    bool isIpCurrentConfigDib() const { return m_type == Type::QKnxNetIpCurrentConfigDib; }
    bool isDeviceDib() const { return m_type == Type::QKnxNetIpDeviceDib; }

    template <typename T> T toType() const
    {
        return QKnxPrivate::QKnxNetIpStructTypeHelper<T>::invoke(*this);
    }
    QKnxNetIpStructRef::Type type() const { return m_type; }

private:
    explicit QKnxNetIpStructRef(QKnxNetIpDib *)
    {}

    QKnxNetIpStructRef(const QKnxByteArray &ref, QKnxNetIpStructRef::Type type)
        : m_type(type)
        , m_byteStoreRef(ref)
    {}

private:
    union
    {
        QKnxNetIpConfigDib *a;
        QKnxNetIpCurrentConfigDib *d;
        QKnxNetIpDeviceDib *e;
    };

    Type m_type = Type::Null;
    QKnxByteArray m_byteStoreRef;
};

namespace QKnxPrivate
{
#define TYPE_CHECK_RETURN_FROM_BYTES(REF_TYPE) \
    do { \
        if (ref.m_type != QKnxNetIpStructRef::Type::REF_TYPE) \
            return {}; \
        if (ref.m_byteStoreRef.size() != 0) \
            return REF_TYPE::fromBytes(ref.m_byteStoreRef, 0); \
    } while (0)

    template<typename T> struct QKnxNetIpStructTypeHelper {};

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpConfigDib>
    {
        static QKnxNetIpConfigDib invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpConfigDib);
            return QKnxNetIpConfigDib(*(ref.a));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpCurrentConfigDib>
    {
        static QKnxNetIpCurrentConfigDib invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpCurrentConfigDib);
            return QKnxNetIpCurrentConfigDib(*(ref.d));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpDeviceDib>
    {
        static QKnxNetIpDeviceDib invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpDeviceDib);
            return QKnxNetIpDeviceDib(*(ref.e));
        }
    };

#undef TYPE_CHECK_RETURN_FROM_BYTES
}

QT_END_NAMESPACE

#endif // QKNXNETIPSTRUCTREF_H
