/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Copyright (C) 2016 Intel Corporation.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QKNXBYTEARRAY_H
#define QKNXBYTEARRAY_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>

#include <QtKnx/qknxglobal.h>

#include <initializer_list>

QT_BEGIN_NAMESPACE

class QKnxByteRef;
template <typename T> class QVector;

class Q_KNX_EXPORT QKnxByteArray
{
private:
    using Data = QTypedArrayData<quint8>;
    using QKnxByteArrayData = QArrayData;

public:
    inline QKnxByteArray() Q_DECL_NOTHROW;
    inline ~QKnxByteArray();

    QKnxByteArray(int size, quint8 c);
    QKnxByteArray(int size, Qt::Initialization);

    QKnxByteArray(const char *data, int size);
    QKnxByteArray(const quint8 *data, int size);

    QKnxByteArray(std::initializer_list<quint8> args);
    inline QKnxByteArray(const QKnxByteArray &) Q_DECL_NOTHROW;

    QKnxByteArray &operator=(const QKnxByteArray &) Q_DECL_NOTHROW;
    inline QKnxByteArray(QKnxByteArray && other) Q_DECL_NOTHROW
        : d(other.d)
    {
        other.d = Data::sharedNull();
    }
    inline QKnxByteArray &operator=(QKnxByteArray && other) Q_DECL_NOTHROW
    {
        qSwap(d, other.d);
        return *this;
    }

    inline void swap(QKnxByteArray &other) Q_DECL_NOTHROW { qSwap(d, other.d); }

    inline int size() const;
    inline bool isEmpty() const;

    void resize(int size);
    QKnxByteArray &fill(quint8 c, int size = -1);

    inline int capacity() const;
    inline void reserve(int size);
    inline void squeeze();

    explicit operator QByteArray() const;
    static QKnxByteArray fromByteArray(const QByteArray &ba);

    inline quint8 *data();
    inline const quint8 *data() const;
    inline const quint8 *constData() const;

    inline void detach();
    void clear();

    inline quint8 at(int i) const;
    inline quint8 value(int i) const;
    inline quint8 value(int i, quint8 defaultValue) const;

    inline quint8 operator[](int i) const;
    inline quint8 operator[](uint i) const;
    inline QKnxByteRef operator[](int i);
    inline QKnxByteRef operator[](uint i);

    Q_REQUIRED_RESULT inline QKnxByteRef front();
    Q_REQUIRED_RESULT inline QKnxByteRef back();

    Q_REQUIRED_RESULT quint8 front() const { return at(0); }
    Q_REQUIRED_RESULT quint8 back() const { return at(size() - 1); }

    int indexOf(quint8 c, int from = 0) const;
    int indexOf(const QKnxByteArray &a, int from = 0) const;
    int lastIndexOf(quint8 c, int from = -1) const;
    int lastIndexOf(const QKnxByteArray &a, int from = -1) const;

    inline bool contains(quint8 c) const;
    inline bool contains(const QKnxByteArray &a) const;

    int count(quint8 c) const;
    int count(const QKnxByteArray &a) const;

    Q_REQUIRED_RESULT QKnxByteArray left(int len) const;
    Q_REQUIRED_RESULT QKnxByteArray right(int len) const;
    Q_REQUIRED_RESULT QKnxByteArray mid(int index, int len = -1) const;
    Q_REQUIRED_RESULT QKnxByteArray chopped(int len) const
    {
        Q_ASSERT(len >= 0); Q_ASSERT(len <= size()); return left(size() - len);
    }

    bool startsWith(quint8 c) const;
    bool startsWith(const QKnxByteArray &a) const;

    bool endsWith(quint8 c) const;
    bool endsWith(const QKnxByteArray &a) const;

    void truncate(int pos);
    void chop(int n);

    Q_REQUIRED_RESULT QKnxByteArray leftJustified(int width, quint8 fill = ' ', bool truncate = false) const;
    Q_REQUIRED_RESULT QKnxByteArray rightJustified(int width, quint8 fill = ' ', bool truncate = false) const;

    QKnxByteArray &prepend(quint8 c);
    QKnxByteArray &prepend(const QKnxByteArray &a);
    QKnxByteArray &prepend(const quint8 *s, int len);
    inline QKnxByteArray &prepend(int count, quint8 c);

    QKnxByteArray &append(quint8 c);
    QKnxByteArray &append(const QKnxByteArray &a);
    QKnxByteArray &append(const quint8 *s, int len);
    inline QKnxByteArray &append(int count, quint8 c);

    QKnxByteArray &insert(int i, quint8 c);
    QKnxByteArray &insert(int i, int count, quint8 c);
    QKnxByteArray &insert(int i, const QKnxByteArray &a);
    QKnxByteArray &insert(int i, const quint8 *s, int len);

    QKnxByteArray &remove(int index, int len);

    QKnxByteArray &replace(int index, int len, const quint8 *s, int alen);
    QKnxByteArray &replace(int index, int len, const QKnxByteArray &s);
    QKnxByteArray &replace(quint8 before, const QKnxByteArray &after);
    QKnxByteArray &replace(const quint8 *before, int bsize, const quint8 *after, int asize);
    QKnxByteArray &replace(const QKnxByteArray &before, const QKnxByteArray &after);
    QKnxByteArray &replace(quint8 before, quint8 after);

    inline QKnxByteArray &operator+=(quint8 c);
    inline QKnxByteArray &operator+=(const QKnxByteArray &a);

    QKnxByteArray repeated(int times) const;
    QVector<QKnxByteArray> split(quint8 sep) const;

    QKnxByteArray &setRawData(const quint8 *a, int n);
    static QKnxByteArray fromRawData(const quint8 *, int size);

    QKnxByteArray toHex(quint8 separator = '\0') const;

    static QKnxByteArray fromHex(const QByteArray &hexEncoded);
    static QKnxByteArray fromHex(const QKnxByteArray &hexEncoded);

    typedef quint8 *iterator;
    typedef const quint8 *const_iterator;
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    inline iterator begin();
    inline const_iterator begin() const;
    inline const_iterator cbegin() const;
    inline const_iterator constBegin() const;
    inline iterator end();
    inline const_iterator end() const;
    inline const_iterator cend() const;
    inline const_iterator constEnd() const;
    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }
    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(begin());
    }

    // stl compatibility
    typedef int size_type;
    typedef qptrdiff difference_type;
    typedef const quint8 & const_reference;
    typedef quint8 & reference;
    typedef quint8 *pointer;
    typedef const quint8 *const_pointer;
    typedef quint8 value_type;
    inline void push_back(quint8 c);
    inline void push_back(const QKnxByteArray &a);
    inline void push_front(quint8 c);
    inline void push_front(const QKnxByteArray &a);
    void shrink_to_fit()
    {
        squeeze();
    }

    inline int count() const
    {
        return d->size;
    }
    int length() const
    {
        return d->size;
    }
    bool isNull() const;

private:
    inline QKnxByteArray(QKnxByteArrayData *ptr)
        : d(static_cast<Data *>(ptr))
    {}

    operator QNoImplicitBoolCast() const;
    Data *d;
    void reallocData(uint alloc, Data::AllocationOptions options);
    void expand(int i);
    QKnxByteArray nulTerminated() const;

    friend class QKnxByteRef;
};

inline QKnxByteArray::QKnxByteArray() Q_DECL_NOTHROW
    : d(Data::sharedNull())
{}

inline QKnxByteArray::~QKnxByteArray()
{
    if (!d->ref.deref()) Data::deallocate(d);
}
inline int QKnxByteArray::size() const
{
    return d->size;
}

inline quint8 QKnxByteArray::at(int i) const
{
    Q_ASSERT(uint(i) < uint(size())); return d->data()[i];
}
inline quint8 QKnxByteArray::value(int i) const
{
    if (uint(i) >= uint(d->size))
        return {};
    return d->data()[i];
}
inline quint8 QKnxByteArray::value(int i, quint8 defaultValue) const
{
    return (uint(i) >= uint(d->size) ? defaultValue : d->data()[i]);
}

inline quint8 QKnxByteArray::operator[](int i) const
{
    Q_ASSERT(uint(i) < uint(size())); return d->data()[i];
}
inline quint8 QKnxByteArray::operator[](uint i) const
{
    Q_ASSERT(i < uint(size())); return d->data()[i];
}

inline bool QKnxByteArray::isEmpty() const
{
    return d->size == 0;
}
inline quint8 *QKnxByteArray::data()
{
    detach(); return d->data();
}
inline const quint8 *QKnxByteArray::data() const
{
    return d->data();
}
inline const quint8 *QKnxByteArray::constData() const
{
    return d->data();
}
inline void QKnxByteArray::detach()
{
    if (d->ref.isShared() || (d->offset != sizeof(QKnxByteArrayData)))
        reallocData(uint(d->size) + 1u, d->detachFlags());
}
inline QKnxByteArray::QKnxByteArray(const QKnxByteArray &a) Q_DECL_NOTHROW
    : d(a.d)
{
    d->ref.ref();
}

inline int QKnxByteArray::capacity() const
{
    return d->alloc ? d->alloc - 1 : 0;
}

inline void QKnxByteArray::reserve(int asize)
{
    if (d->ref.isShared() || uint(asize) + 1u > d->alloc) {
        reallocData(qMax(uint(size()), uint(asize)) + 1u, d->detachFlags() | Data::CapacityReserved);
    } else {
        // cannot set unconditionally, since d could be the shared_null or
        // otherwise static
        d->capacityReserved = true;
    }
}

inline void QKnxByteArray::squeeze()
{
    if (d->ref.isShared() || uint(d->size) + 1u < d->alloc) {
        reallocData(uint(d->size) + 1u, d->detachFlags() & ~Data::CapacityReserved);
    } else {
        // cannot set unconditionally, since d could be shared_null or
        // otherwise static.
        d->capacityReserved = false;
    }
}

class  Q_KNX_EXPORT  QKnxByteRef
{
    QKnxByteArray &a;
    int i;
    inline QKnxByteRef(QKnxByteArray &array, int idx)
        : a(array), i(idx)
    {
    }
    friend class QKnxByteArray;
public:
    inline operator quint8() const
    {
        return i < a.d->size ? a.d->data()[i] : quint8(0);
    }
    inline QKnxByteRef &operator=(quint8 c)
    {
        if (i >= a.d->size) a.expand(i); else a.detach();
        a.d->data()[i] = c;  return *this;
    }
    inline QKnxByteRef &operator=(const QKnxByteRef &c)
    {
        if (i >= a.d->size) a.expand(i); else a.detach();
        a.d->data()[i] = c.a.d->data()[c.i];  return *this;
    }
    inline bool operator==(quint8 c) const
    {
        return a.d->data()[i] == c;
    }
    inline bool operator!=(quint8 c) const
    {
        return a.d->data()[i] != c;
    }
    inline bool operator>(quint8 c) const
    {
        return a.d->data()[i] > c;
    }
    inline bool operator>=(quint8 c) const
    {
        return a.d->data()[i] >= c;
    }
    inline bool operator<(quint8 c) const
    {
        return a.d->data()[i] < c;
    }
    inline bool operator<=(quint8 c) const
    {
        return a.d->data()[i] <= c;
    }
};

inline QKnxByteRef QKnxByteArray::operator[](int i)
{
    Q_ASSERT(i >= 0); return QKnxByteRef(*this, i);
}
inline QKnxByteRef QKnxByteArray::operator[](uint i)
{
    return QKnxByteRef(*this, i);
}
inline QKnxByteRef QKnxByteArray::front()
{
    return operator[](0);
}
inline QKnxByteRef QKnxByteArray::back()
{
    return operator[](size() - 1);
}
inline QKnxByteArray::iterator QKnxByteArray::begin()
{
    detach(); return d->data();
}
inline QKnxByteArray::const_iterator QKnxByteArray::begin() const
{
    return d->data();
}
inline QKnxByteArray::const_iterator QKnxByteArray::cbegin() const
{
    return d->data();
}
inline QKnxByteArray::const_iterator QKnxByteArray::constBegin() const
{
    return d->data();
}
inline QKnxByteArray::iterator QKnxByteArray::end()
{
    detach(); return d->data() + d->size;
}
inline QKnxByteArray::const_iterator QKnxByteArray::end() const
{
    return d->data() + d->size;
}
inline QKnxByteArray::const_iterator QKnxByteArray::cend() const
{
    return d->data() + d->size;
}
inline QKnxByteArray::const_iterator QKnxByteArray::constEnd() const
{
    return d->data() + d->size;
}
inline QKnxByteArray &QKnxByteArray::append(int n, quint8 ch)
{
    return insert(d->size, n, ch);
}
inline QKnxByteArray &QKnxByteArray::prepend(int n, quint8 ch)
{
    return insert(0, n, ch);
}
inline QKnxByteArray &QKnxByteArray::operator+=(quint8 c)
{
    return append(c);
}
inline QKnxByteArray &QKnxByteArray::operator+=(const QKnxByteArray &a)
{
    return append(a);
}
inline void QKnxByteArray::push_back(quint8 c)
{
    append(c);
}
inline void QKnxByteArray::push_back(const QKnxByteArray &a)
{
    append(a);
}
inline void QKnxByteArray::push_front(quint8 c)
{
    prepend(c);
}
inline void QKnxByteArray::push_front(const QKnxByteArray &a)
{
    prepend(a);
}
inline bool QKnxByteArray::contains(const QKnxByteArray &a) const
{
    return indexOf(a) != -1;
}
inline bool QKnxByteArray::contains(quint8 c) const
{
    return indexOf(c) != -1;
}
inline bool operator==(const QKnxByteArray &a1, const QKnxByteArray &a2) Q_DECL_NOTHROW
{
    return (a1.size() == a2.size()) && (memcmp(a1.constData(), a2.constData(), a1.size()) == 0);
}
inline bool operator!=(const QKnxByteArray &a1, const QKnxByteArray &a2) Q_DECL_NOTHROW
{
    return !(a1 == a2);
}

inline const QKnxByteArray operator+(const QKnxByteArray &a1, const QKnxByteArray &a2)
{
    return QKnxByteArray(a1) += a2;
}
inline const QKnxByteArray operator+(const QKnxByteArray &a1, quint8 a2)
{
    return QKnxByteArray(a1) += a2;
}
inline const QKnxByteArray operator+(quint8 a1, const QKnxByteArray &a2)
{
    return QKnxByteArray(1, a1) += a2;
}

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxByteArray &);
Q_KNX_EXPORT Q_DECL_PURE_FUNCTION uint qHash(const QKnxByteArray &ba, uint seed = 0) Q_DECL_NOTHROW;

#if !defined(QT_NO_DATASTREAM)

Q_KNX_EXPORT QDataStream &operator>>(QDataStream &, QKnxByteArray &);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &, const QKnxByteArray &);

#endif

Q_DECLARE_SHARED(QKnxByteArray)

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QKnxByteArray)

#endif
