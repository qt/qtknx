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

#ifndef QKNXNETIPKNXADDRESSESDIB_H
#define QKNXNETIPKNXADDRESSESDIB_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpKnxAddressesDib final : public QKnxNetIpDescriptionTypeStruct
{
public:
    QKnxNetIpKnxAddressesDib() = default;
    ~QKnxNetIpKnxAddressesDib() override = default;

    explicit QKnxNetIpKnxAddressesDib(const QKnxAddress &address);
    explicit QKnxNetIpKnxAddressesDib(const QVector<QKnxAddress> &addresses);

    template <typename T> static QKnxNetIpKnxAddressesDib fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpStructHelper::fromBytes(bytes, index,
            QKnxNetIp::DescriptionType::KnxAddresses);
    }

    QKnxNetIp::DescriptionType descriptionType() const;
    template <typename T> auto individualAddresses() const -> decltype(T())
    {
        static_assert(is_type<T, QVector<QKnxAddress>, std::deque<QKnxAddress>,
            std::vector<QKnxAddress>>::value, "Type not supported.");

        const QKnxNetIpPayload &load = payload();
        T addresses;
        for (quint16 i = 0; i < load.size(); i += 2)
            addresses.push_back({ QKnxAddress::Type::Individual, load.bytes<QByteArray>(i, 2) });
        return addresses;

    }

    bool isValid() const override;

private:
    QKnxNetIpKnxAddressesDib(const QKnxNetIpDescriptionTypeStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpKnxAddressesDib, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
