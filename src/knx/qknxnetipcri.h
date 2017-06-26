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

#ifndef QKNXNETIPCRI_H
#define QKNXNETIPCRI_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpCri : public QKnxNetIpConnectionTypeStruct
{
public:
    QKnxNetIpCri() = default;
    ~QKnxNetIpCri() override = default;

    explicit QKnxNetIpCri(QKnxNetIp::TunnelingLayer layer);
    explicit QKnxNetIpCri(QKnxNetIp::ConnectionType connectionType); // TODO: review
    explicit QKnxNetIpCri(QKnxNetIpServiceFamiliesDib::ServiceFamilieId serviceType);

    template <typename T> static QKnxNetIpCri fromBytes(const T &bytes, quint16 index)
    {
        auto code = QKnxNetIpStructHeader<QKnxNetIp::ConnectionType>::fromBytes(bytes, index).code();
        if (!QKnxNetIp::isStructType(code))
            return {};
        return QKnxNetIpStructHelper::fromBytes(bytes, index, code);
    }

    QKnxNetIp::ConnectionType connectionType() const;
    void setConnectionType(QKnxNetIp::ConnectionType connectionType);

    bool isValid() const override;

    QKnxNetIp::TunnelingLayer tunnelingLayer() const;
    bool setTunnelingLayer(QKnxNetIp::TunnelingLayer layer);

private:
    QKnxNetIpCri(const QKnxNetIpConnectionTypeStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpCri, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
