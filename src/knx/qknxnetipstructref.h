/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPSTRUCTREF_H
#define QKNXNETIPSTRUCTREF_H

#include <QtKnx/qknxnetippackage.h>
#include <QtKnx/qknxnetipconfigdib.h>
#include <QtKnx/qknxnetipcrd.h>
#include <QtKnx/qknxnetipcri.h>
#include <QtKnx/qknxnetipcurrentconfigdib.h>
#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipknxaddressesdib.h>
#include <QtKnx/qknxnetipmanufacturerdib.h>
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
        QKnxNetIpConfigDIB,
        QKnxNetIpCRD,
        QKnxNetIpCRI,
        QKnxNetIpCurrentConfigDIB,
        QKnxNetIpDeviceDIB,
        QKnxNetIpHPAI,
        QKnxNetIpKnxAddressesDIB,
        QKnxNetIpManufacturerDIB,
        QKnxNetIpServiceFamiliesDIB
    };

    QKnxNetIpStructRef() = default;
    ~QKnxNetIpStructRef() = default;

    explicit QKnxNetIpStructRef(QKnxNetIpConfigDIB *ipConfigDIB)
        : a(ipConfigDIB)
        , m_type(Type::QKnxNetIpConfigDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCRD *crd)
        : b(crd)
        , m_type(Type::QKnxNetIpCRD)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCRI *cri)
        : c(cri)
        , m_type(Type::QKnxNetIpCRI)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCurrentConfigDIB *ipCurrentConfigDIB)
        : d(ipCurrentConfigDIB)
        , m_type(Type::QKnxNetIpCurrentConfigDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpDeviceDIB *deviceDIB)
        : e(deviceDIB)
        , m_type(Type::QKnxNetIpDeviceDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpHPAI *hpai)
        : f(hpai)
        , m_type(Type::QKnxNetIpHPAI)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpKnxAddressesDIB *knxAddressesDIB)
        : g(knxAddressesDIB)
        , m_type(Type::QKnxNetIpKnxAddressesDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpManufacturerDIB *manufacturerDIB)
        : h(manufacturerDIB)
        , m_type(Type::QKnxNetIpManufacturerDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpServiceFamiliesDIB *serviceFamiliesDIB)
        : i(serviceFamiliesDIB)
        , m_type(Type::QKnxNetIpServiceFamiliesDIB)
        , m_isByteStoreRef(false)
    {}

    QKnxNetIpStructRef(const QKnxNetIpStructRef &) = default;
    QKnxNetIpStructRef &operator=(const QKnxNetIpStructRef &) = default;

    bool isNull() const { return m_type == Type::Null; }
    bool isIpConfigDIB() const { return m_type == Type::QKnxNetIpConfigDIB; }
    bool isCrd() const { return m_type == Type::QKnxNetIpCRD; }
    bool isCri() const { return m_type == Type::QKnxNetIpCRI; }
    bool isIpCurrentConfigDIB() const { return m_type == Type::QKnxNetIpCurrentConfigDIB; }
    bool isDeviceDIB() const { return m_type == Type::QKnxNetIpDeviceDIB; }
    bool isHpai() const { return m_type == Type::QKnxNetIpHPAI; }
    bool isKnxAddressesDIB() const { return m_type == Type::QKnxNetIpKnxAddressesDIB; }
    bool isManufacturerDIB() const { return m_type == Type::QKnxNetIpManufacturerDIB; }
    bool isServiceFamiliesDIB() const { return m_type == Type::QKnxNetIpServiceFamiliesDIB; }

    template <typename T> T toType() const
    {
        return QKnxPrivate::QKnxNetIpStructTypeHelper<T>::invoke(*this);
    }
    QKnxNetIpStructRef::Type type() const { return m_type; }

private:
    explicit QKnxNetIpStructRef(QKnxNetIpStruct *)
    {}

    QKnxNetIpStructRef(const quint8 *ref, QKnxNetIpStructRef::Type type)
        : byteStoreRef(ref)
        , m_type(type)
        , m_isByteStoreRef(true)
    {}

private:
    union
    {
        QKnxNetIpConfigDIB *a;
        QKnxNetIpCRD *b;
        QKnxNetIpCRI *c;
        QKnxNetIpCurrentConfigDIB *d;
        QKnxNetIpDeviceDIB *e;
        QKnxNetIpHPAI *f;
        QKnxNetIpKnxAddressesDIB *g;
        QKnxNetIpManufacturerDIB *h;
        QKnxNetIpServiceFamiliesDIB *i;
        const quint8 *byteStoreRef;
    };
    Type m_type = Type::Null;
    bool m_isByteStoreRef = false;
};

namespace QKnxPrivate
{
#define TYPE_CHECK_RETURN_FROM_BYTES(REF_TYPE) \
    do { \
            if (ref.m_type != QKnxNetIpStructRef::Type::REF_TYPE) \
                return {}; \
            if (ref.m_isByteStoreRef) \
                return REF_TYPE::fromBytes(ref.byteStoreRef, 0); \
    } while (0)

    template<typename T> struct QKnxNetIpStructTypeHelper {};

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpConfigDIB>
    {
        static QKnxNetIpConfigDIB invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpConfigDIB);
            return QKnxNetIpConfigDIB(*(ref.a));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpCRD>
    {
        static QKnxNetIpCRD invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpCRD);
            return QKnxNetIpCRD(*(ref.b));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpCRI>
    {
        static QKnxNetIpCRI invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpCRI);
            return QKnxNetIpCRI(*(ref.c));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpCurrentConfigDIB>
    {
        static QKnxNetIpCurrentConfigDIB invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpCurrentConfigDIB);
            return QKnxNetIpCurrentConfigDIB(*(ref.d));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpDeviceDIB>
    {
        static QKnxNetIpDeviceDIB invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpDeviceDIB);
            return QKnxNetIpDeviceDIB(*(ref.e));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpHPAI>
    {
        static QKnxNetIpHPAI invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpHPAI);
            return QKnxNetIpHPAI(*(ref.f));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpKnxAddressesDIB>
    {
        static QKnxNetIpKnxAddressesDIB invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpKnxAddressesDIB);
            return QKnxNetIpKnxAddressesDIB(*(ref.g));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpManufacturerDIB>
    {
        static QKnxNetIpManufacturerDIB invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpManufacturerDIB);
            return QKnxNetIpManufacturerDIB(*(ref.h));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpServiceFamiliesDIB>
    {
        static QKnxNetIpServiceFamiliesDIB invoke(const QKnxNetIpStructRef &ref)
        {
            TYPE_CHECK_RETURN_FROM_BYTES(QKnxNetIpServiceFamiliesDIB);
            return QKnxNetIpServiceFamiliesDIB(*(ref.i));
        }
    };
#undef TYPE_CHECK_RETURN_FROM_BYTES
}

QT_END_NAMESPACE

#endif // QKNXNETIPSTRUCTREF_H
