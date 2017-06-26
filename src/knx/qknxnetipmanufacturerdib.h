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

#ifndef QKNXNETIPMANUFACTURERDIB_H
#define QKNXNETIPMANUFACTURERDIB_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpManufacturerDib final : public QKnxNetIpDescriptionTypeStruct
{
public:
    QKnxNetIpManufacturerDib() = default;
    ~QKnxNetIpManufacturerDib() override = default;

    explicit QKnxNetIpManufacturerDib(quint16 manufacturerId);
    QKnxNetIpManufacturerDib(quint16 manufacturerId, const QByteArray &manufacturerData);
    QKnxNetIpManufacturerDib(quint16 manufacturerId, const QVector<quint8> &manufacturerData);

    template <typename T> static QKnxNetIpManufacturerDib fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpStructHelper::fromBytes(bytes, index,
            QKnxNetIp::DescriptionType::ManufactorData);
    }

    QKnxNetIp::DescriptionType descriptionType() const;
    quint16 manufacturerId() const;
    template <typename T = QByteArray> auto manufacturerData() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");
        const auto &ref = payloadRef();
        return ref.bytes<T>(2, ref.size() - 2);
    }

    bool isValid() const override;

private:
    QKnxNetIpManufacturerDib(const QKnxNetIpDescriptionTypeStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpManufacturerDib, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
