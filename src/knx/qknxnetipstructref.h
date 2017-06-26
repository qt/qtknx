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

#ifndef QKNXNETIPSTRUCTREF_H
#define QKNXNETIPSTRUCTREF_H

#include <QtKnx/qknxnetippackage.h>
#include <QtKnx/qknxnetipconfigdib.h>
#include <QtKnx/QKnxNetIpCrd.h>
#include <QtKnx/QKnxNetIpCri.h>
#include <QtKnx/QKnxNetIpCurrentConfigDib.h>
#include <QtKnx/QKnxNetIpDeviceDib.h>
#include <QtKnx/QKnxNetIpHpai.h>
#include <QtKnx/QKnxNetIpKnxAddressesDib.h>
#include <QtKnx/QKnxNetIpManufacturerDib.h>
#include <QtKnx/QKnxNetIpServiceFamiliesDib.h>

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
        QKnxNetIpCrd,
        QKnxNetIpCri,
        QKnxNetIpCurrentConfigDib,
        QKnxNetIpDeviceDib,
        QKnxNetIpHpai,
        QKnxNetIpKnxAddressesDib,
        QKnxNetIpManufacturerDib,
        QKnxNetIpServiceFamiliesDib
    };

    QKnxNetIpStructRef() = default;
    ~QKnxNetIpStructRef() = default;

    explicit QKnxNetIpStructRef(QKnxNetIpConfigDib *ipConfigDib)
        : a(ipConfigDib)
        , m_type(Type::QKnxNetIpConfigDib)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCrd *crd)
        : b(crd)
        , m_type(Type::QKnxNetIpCrd)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCri *cri)
        : c(cri)
        , m_type(Type::QKnxNetIpCri)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCurrentConfigDib *ipCurrentConfigDib)
        : d(ipCurrentConfigDib)
        , m_type(Type::QKnxNetIpCurrentConfigDib)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpDeviceDib *deviceDib)
        : e(deviceDib)
        , m_type(Type::QKnxNetIpDeviceDib)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpHpai *hpai)
        : f(hpai)
        , m_type(Type::QKnxNetIpHpai)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpKnxAddressesDib *knxAddressesDib)
        : g(knxAddressesDib)
        , m_type(Type::QKnxNetIpKnxAddressesDib)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpManufacturerDib *manufacturerDib)
        : h(manufacturerDib)
        , m_type(Type::QKnxNetIpManufacturerDib)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpServiceFamiliesDib *serviceFamiliesDib)
        : i(serviceFamiliesDib)
        , m_type(Type::QKnxNetIpServiceFamiliesDib)
        , m_isByteStoreRef(false)
    {}

    QKnxNetIpStructRef(const QKnxNetIpStructRef &) = default;
    QKnxNetIpStructRef &operator=(const QKnxNetIpStructRef &) = default;

    bool isNull() const { return m_type == Type::Null; }
    bool isIpConfigDib() const { return m_type == Type::QKnxNetIpConfigDib; }
    bool isCrd() const { return m_type == Type::QKnxNetIpCrd; }
    bool isCri() const { return m_type == Type::QKnxNetIpCri; }
    bool isIpCurrentConfigDib() const { return m_type == Type::QKnxNetIpCurrentConfigDib; }
    bool isDeviceDib() const { return m_type == Type::QKnxNetIpDeviceDib; }
    bool isHpai() const { return m_type == Type::QKnxNetIpHpai; }
    bool isKnxAddressesDib() const { return m_type == Type::QKnxNetIpKnxAddressesDib; }
    bool isManufacturerDib() const { return m_type == Type::QKnxNetIpManufacturerDib; }
    bool isServiceFamiliesDib() const { return m_type == Type::QKnxNetIpServiceFamiliesDib; }

    template <typename T> T toType() const
    {
        return QKnxPrivate::QKnxNetIpStructTypeHelper<T>::invoke(*this);
    }
    QKnxNetIpStructRef::Type type() const { return m_type; }

private:
    explicit QKnxNetIpStructRef(QKnxNetIpHostProtocolStruct *)
    {}
    explicit QKnxNetIpStructRef(QKnxNetIpConnectionTypeStruct *)
    {}
    explicit QKnxNetIpStructRef(QKnxNetIpDescriptionTypeStruct *)
    {}

    QKnxNetIpStructRef(const QKnxByteStoreRef &ref, QKnxNetIpStructRef::Type type)
        : m_type(type)
        , m_isByteStoreRef(true)
        , m_byteStoreRef(ref)
    {}

private:
    union
    {
        QKnxNetIpConfigDib *a;
        QKnxNetIpCrd *b;
        QKnxNetIpCri *c;
        QKnxNetIpCurrentConfigDib *d;
        QKnxNetIpDeviceDib *e;
        QKnxNetIpHpai *f;
        QKnxNetIpKnxAddressesDib *g;
        QKnxNetIpManufacturerDib *h;
        QKnxNetIpServiceFamiliesDib *i;
    };
    Type m_type = Type::Null;
    bool m_isByteStoreRef = false;
    QKnxByteStoreRef m_byteStoreRef;
};

namespace QKnxPrivate
{
#define TYPE_CHECK_RETURN_FROM_BYTES(REF_TYPE) \
    do { \
        if (ref.m_type != QKnxNetIpStructRef::Type::REF_TYPE) \
            return {}; \
        if (ref.m_isByteStoreRef) \
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

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpCrd>
    {
        static QKnxNetIpCrd invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpCrd);
            return QKnxNetIpCrd(*(ref.b));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpCri>
    {
        static QKnxNetIpCri invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpCri);
            return QKnxNetIpCri(*(ref.c));
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

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpHpai>
    {
        static QKnxNetIpHpai invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpHpai);
            return QKnxNetIpHpai(*(ref.f));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpKnxAddressesDib>
    {
        static QKnxNetIpKnxAddressesDib invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpKnxAddressesDib);
            return QKnxNetIpKnxAddressesDib(*(ref.g));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpManufacturerDib>
    {
        static QKnxNetIpManufacturerDib invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpManufacturerDib);
            return QKnxNetIpManufacturerDib(*(ref.h));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpServiceFamiliesDib>
    {
        static QKnxNetIpServiceFamiliesDib invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpServiceFamiliesDib);
            return QKnxNetIpServiceFamiliesDib(*(ref.i));
        }
    };
#undef TYPE_CHECK_RETURN_FROM_BYTES
}

QT_END_NAMESPACE

#endif // QKNXNETIPSTRUCTREF_H
