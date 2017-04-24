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

class Q_KNX_EXPORT QKnxNetIpCRI : public QKnxNetIpStruct
{
public:
    QKnxNetIpCRI() = default;
    ~QKnxNetIpCRI() override = default;

    template <typename T> static QKnxNetIpCRI fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpStruct::fromBytes(bytes, index);
    }

    QKnxNetIp::ConnectionTypeCode connectionTypeCode() const;

    bool isValid() const override;

private:
    QKnxNetIpCRI(const QKnxNetIpStruct &other);
};
Q_DECLARE_TYPEINFO(QKnxNetIpCRI, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
