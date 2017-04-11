/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXBYTESTOREREF_H
#define QKNXBYTESTOREREF_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class QKnxByteStore;
class Q_KNX_EXPORT QKnxByteStoreRef final
{
public:
    QKnxByteStoreRef() = default;
    ~QKnxByteStoreRef() = default;

    explicit QKnxByteStoreRef(const QKnxByteStore *store);

    QKnxByteStoreRef(const QKnxByteStoreRef &) = default;
    QKnxByteStoreRef &operator=(const QKnxByteStoreRef &) = default;

    quint16 size() const;
    quint8 byte(quint16 index) const;
    const quint8 *bytes() const;

    template <typename T = std::vector<quint8>> auto bytes() const -> decltype(T())
    {
        if (!m_store)
            return {};
        T t(size(), 0);
        std::copy_n(bytes(), size(), std::begin(t));
        return t;
    }

    template <typename T = std::vector<quint8>>
        auto bytes(quint16 start, quint16 size) const -> decltype(T())
    {
        if (!m_store)
            return {};
        T t(size, 0);
        std::copy_n(std::next(bytes(), start), size, std::begin(t));
        return t;
    }

private:
    const QKnxByteStore *m_store = nullptr;
};

QT_END_NAMESPACE

#endif
