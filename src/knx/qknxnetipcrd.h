/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPCRD_H
#define QKNXNETIPCRD_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpCRD : public QKnxNetIpConnectionTypeStruct
{
public:
    QKnxNetIpCRD() = default;
    ~QKnxNetIpCRD() override = default;

    explicit QKnxNetIpCRD(QKnxNetIp::ConnectionType connectionType);

    template <typename T> static QKnxNetIpCRD fromBytes(const T &bytes, quint16 index)
    {
        auto code = QKnxNetIpStructHeader<QKnxNetIp::ConnectionType>::fromBytes(bytes, index).code();
        if (!QKnxNetIp::isStructType(code))
            return {};
        return QKnxNetIpStructHelper::fromBytes(bytes, index, code);
    }

    QKnxNetIp::ConnectionType connectionType() const;
    void setConnectionType(QKnxNetIp::ConnectionType connectionType);

    bool isValid() const override;

private:
    QKnxNetIpCRD(const QKnxNetIpConnectionTypeStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpCRD, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
