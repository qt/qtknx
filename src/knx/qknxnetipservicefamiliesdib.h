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
#include <QtCore/qmap.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpServiceFamiliesDIB final : private QKnxNetIpStruct
{
public:
    enum class ServiceFamilieId : quint8
    {
        Core = 0x02,
        DeviceManagement = 0x03,
        IpTunneling = 0x04,
        IpRouting = 0x05,
        RemoteLogging = 0x06,
        RemoteConfigAndDiagnosis = 0x07,
        ObjectServer = 0x08
    };

    QKnxNetIpServiceFamiliesDIB() = default;
    ~QKnxNetIpServiceFamiliesDIB() override = default;

    QKnxNetIpServiceFamiliesDIB(ServiceFamilieId id, quint8 versions);
    QKnxNetIpServiceFamiliesDIB(const QMap<ServiceFamilieId, quint8> &families);
    QKnxNetIpServiceFamiliesDIB(const QVector<ServiceFamilieId> &ids, const QVector<quint8> &versions);

    template <typename T> static QKnxNetIpServiceFamiliesDIB fromBytes(const T &bytes, qint32 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

    QKnxNetIp::DescriptionTypeCode descriptionTypeCode() const;
    // TODO: add access functions for family and version if needed

    void add(ServiceFamilieId id, quint8 versions);
    void add(const QMap<ServiceFamilieId, quint8> &families);
    void add(const QVector<ServiceFamilieId> &ids, const QVector<quint8> &versions);

    bool isValid() const override;

    using QKnxNetIpStruct::size;
    using QKnxNetIpStruct::bytes;
    using QKnxNetIpStruct::payload;
    using QKnxNetIpStruct::toString;

private:
    QKnxNetIpServiceFamiliesDIB(const QKnxNetIpStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpServiceFamiliesDIB, Q_MOVABLE_TYPE);

QT_END_NAMESPACE
