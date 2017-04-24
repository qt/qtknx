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

class Q_KNX_EXPORT QKnxNetIpCRD : public QKnxNetIpStruct
{
public:
    QKnxNetIpCRD() = default;
    ~QKnxNetIpCRD() override = default;

    template <typename T> static QKnxNetIpCRD fromBytes(const T &bytes, qint32 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

    QKnxNetIp::ConnectionTypeCode connectionTypeCode() const;

    bool isValid() const override;

private:
    QKnxNetIpCRD(const QKnxNetIpStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpCRD, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
