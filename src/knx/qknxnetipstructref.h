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
        IpConfigDIB,
        CRD,
        CRI,
        IpCurrentConfigDIB,
        DeviceDIB,
        HPAI,
        KnxAddressesDIB,
        ManufacturerDIB,
        ServiceFamiliesDIB
    };

    QKnxNetIpStructRef() = default;
    ~QKnxNetIpStructRef() = default;

    explicit QKnxNetIpStructRef(QKnxNetIpConfigDIB *ipConfigDIB)
        : a(ipConfigDIB)
        , m_type(Type::IpConfigDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCRD *crd)
        : b(crd)
        , m_type(Type::CRD)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCRI *cri)
        : c(cri)
        , m_type(Type::CRI)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpCurrentConfigDIB *ipCurrentConfigDIB)
        : d(ipCurrentConfigDIB)
        , m_type(Type::IpCurrentConfigDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpDeviceDIB *deviceDIB)
        : e(deviceDIB)
        , m_type(Type::DeviceDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpHPAI *hpai)
        : f(hpai)
        , m_type(Type::HPAI)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpKnxAddressesDIB *knxAddressesDIB)
        : g(knxAddressesDIB)
        , m_type(Type::KnxAddressesDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpManufacturerDIB *manufacturerDIB)
        : h(manufacturerDIB)
        , m_type(Type::ManufacturerDIB)
        , m_isByteStoreRef(false)
    {}

    explicit QKnxNetIpStructRef(QKnxNetIpServiceFamiliesDIB *serviceFamiliesDIB)
        : i(serviceFamiliesDIB)
        , m_type(Type::ServiceFamiliesDIB)
        , m_isByteStoreRef(false)
    {}

    QKnxNetIpStructRef(const QKnxNetIpStructRef &) = default;
    QKnxNetIpStructRef &operator=(const QKnxNetIpStructRef &) = default;

    bool isNull() const { return m_type == Type::Null; }
    bool isIpConfigDIB() const { return m_type == Type::IpConfigDIB; }
    bool isCrd() const { return m_type == Type::CRD; }
    bool isCri() const { return m_type == Type::CRI; }
    bool isIpCurrentConfigDIB() const { return m_type == Type::IpCurrentConfigDIB; }
    bool isDeviceDIB() const { return m_type == Type::DeviceDIB; }
    bool isHpai() const { return m_type == Type::HPAI; }
    bool isKnxAddressesDIB() const { return m_type == Type::KnxAddressesDIB; }
    bool isManufacturerDIB() const { return m_type == Type::ManufacturerDIB; }
    bool isServiceFamiliesDIB() const { return m_type == Type::ServiceFamiliesDIB; }

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
    template<typename T> struct QKnxNetIpStructTypeHelper {};

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpConfigDIB>
    {
        static QKnxNetIpConfigDIB invoke(const QKnxNetIpStructRef &ref)
        {
            if (ref.m_isByteStoreRef && ref.m_type == QKnxNetIpStructRef::Type::IpConfigDIB)
                return QKnxNetIpConfigDIB::fromBytes(ref.byteStoreRef, 0);
            return QKnxNetIpConfigDIB(*(ref.a));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpCRD>
    {
        static QKnxNetIpCRD invoke(const QKnxNetIpStructRef &ref)
        {
            if (ref.m_isByteStoreRef && ref.m_type == QKnxNetIpStructRef::Type::CRD)
                return QKnxNetIpCRD::fromBytes(ref.byteStoreRef, 0);
            return QKnxNetIpCRD(*(ref.b));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpCRI>
    {
        static QKnxNetIpCRI invoke(const QKnxNetIpStructRef &ref)
        {
            if (ref.m_isByteStoreRef && ref.m_type == QKnxNetIpStructRef::Type::CRI)
                return QKnxNetIpCRI::fromBytes(ref.byteStoreRef, 0);
            return QKnxNetIpCRI(*(ref.c));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpCurrentConfigDIB>
    {
        static QKnxNetIpCurrentConfigDIB invoke(const QKnxNetIpStructRef &ref)
        {
            if (ref.m_isByteStoreRef && ref.m_type == QKnxNetIpStructRef::Type::IpCurrentConfigDIB)
                return QKnxNetIpCurrentConfigDIB::fromBytes(ref.byteStoreRef, 0);
            return QKnxNetIpCurrentConfigDIB(*(ref.d));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpDeviceDIB>
    {
        static QKnxNetIpDeviceDIB invoke(const QKnxNetIpStructRef &ref)
        {
            if (ref.m_isByteStoreRef && ref.m_type == QKnxNetIpStructRef::Type::DeviceDIB)
                return QKnxNetIpDeviceDIB::fromBytes(ref.byteStoreRef, 0);
            return QKnxNetIpDeviceDIB(*(ref.e));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpHPAI>
    {
        static QKnxNetIpHPAI invoke(const QKnxNetIpStructRef &ref)
        {
            if (ref.m_isByteStoreRef && ref.m_type == QKnxNetIpStructRef::Type::HPAI)
                return QKnxNetIpHPAI::fromBytes(ref.byteStoreRef, 0);
            return QKnxNetIpHPAI(*(ref.f));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpKnxAddressesDIB>
    {
        static QKnxNetIpKnxAddressesDIB invoke(const QKnxNetIpStructRef &ref)
        {
            if (ref.m_isByteStoreRef && ref.m_type == QKnxNetIpStructRef::Type::KnxAddressesDIB)
                return QKnxNetIpKnxAddressesDIB::fromBytes(ref.byteStoreRef, 0);
            return QKnxNetIpKnxAddressesDIB(*(ref.g));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpManufacturerDIB>
    {
        static QKnxNetIpManufacturerDIB invoke(const QKnxNetIpStructRef &ref)
        {
            if (ref.m_isByteStoreRef && ref.m_type == QKnxNetIpStructRef::Type::ManufacturerDIB)
                return QKnxNetIpManufacturerDIB::fromBytes(ref.byteStoreRef, 0);
            return QKnxNetIpManufacturerDIB(*(ref.h));
        }
    };

    template<> struct QKnxNetIpStructTypeHelper<QKnxNetIpServiceFamiliesDIB>
    {
        static QKnxNetIpServiceFamiliesDIB invoke(const QKnxNetIpStructRef &ref)
        {
            if (ref.m_isByteStoreRef && ref.m_type == QKnxNetIpStructRef::Type::ServiceFamiliesDIB)
                return QKnxNetIpServiceFamiliesDIB::fromBytes(ref.byteStoreRef, 0);
            return QKnxNetIpServiceFamiliesDIB(*(ref.i));
        }
    };
}

QT_END_NAMESPACE

#endif // QKNXNETIPSTRUCTREF_H
