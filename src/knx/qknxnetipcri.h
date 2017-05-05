/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

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

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpCRI : public QKnxNetIpConnectionTypeStruct
{
public:
    QKnxNetIpCRI() = default;
    ~QKnxNetIpCRI() override = default;

    explicit QKnxNetIpCRI(QKnxNetIp::ConnectionType connectionType);

    template <typename T> static QKnxNetIpCRI fromBytes(const T &bytes, quint16 index)
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
    QKnxNetIpCRI(const QKnxNetIpConnectionTypeStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpCRI, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
