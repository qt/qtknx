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
#include <QtCore/qdebug.h>

#include <QtKnx/qtknxglobal.h>

#include <initializer_list>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxByteArray
{
public:
    inline QKnxByteArray() = default;
    inline ~QKnxByteArray() = default;

    QKnxByteArray(int size, quint8 ch);
    QKnxByteArray(int size, Qt::Initialization);

    QKnxByteArray(const char *data, int size);
    QKnxByteArray(const quint8 *data, int size);

    QKnxByteArray(std::initializer_list<quint8> args);

    inline QKnxByteArray(const QKnxByteArray &other) Q_DECL_NOTHROW
        : m_bytes(other.m_bytes)
    {}
    QKnxByteArray &operator=(const QKnxByteArray &other) Q_DECL_NOTHROW;

    inline QKnxByteArray(QKnxByteArray &&other) Q_DECL_NOTHROW
        : m_bytes(other.m_bytes)
    {}
    inline QKnxByteArray &operator=(QKnxByteArray &&other) Q_DECL_NOTHROW
    {
        m_bytes.operator=(other.m_bytes);
        return *this;
    }

    inline void swap(QKnxByteArray &other) Q_DECL_NOTHROW { m_bytes.swap(other.m_bytes); }

    const QByteArray &toByteArray() const;
    static QKnxByteArray fromByteArray(const QByteArray &ba);

    bool isNull() const;
    inline bool isEmpty() const { return size() == 0; }

    inline int size() const { return m_bytes.size(); }

    void clear();
    void resize(int size);

    inline quint8 at(int i) const { return m_bytes.at(i); }
    inline void set(int i, quint8 value) { m_bytes[i] = value; }

    inline void setValue(int i, quint8 value)
    {
        if (i >= 0 && i < m_bytes.size()) m_bytes[i] = value;
    }
    inline quint8 value(int i, quint8 defaultValue = {}) const
    {
        return (uint(i) >= uint(size()) ? defaultValue : quint8(m_bytes.at(i)));
    }

    QKnxByteArray repeated(int times) const;
    QKnxByteArray &fill(quint8 ch, int size = -1);

    Q_REQUIRED_RESULT QKnxByteArray left(int len) const;
    Q_REQUIRED_RESULT QKnxByteArray right(int len) const;
    Q_REQUIRED_RESULT QKnxByteArray mid(int index, int len = -1) const;
    Q_REQUIRED_RESULT QKnxByteArray chop(int len) const {
        Q_ASSERT(len >= 0); if (len >= size()) return {}; return left(size() - len);
    }

    QKnxByteArray &prepend(quint8 ch);
    QKnxByteArray &prepend(const QKnxByteArray &ba);
    inline QKnxByteArray &prepend(int count, quint8 ch) { return insert(0, count, ch); }

    QKnxByteArray &insert(int i, quint8 ch);
    QKnxByteArray &insert(int i, int count, quint8 ch);
    QKnxByteArray &insert(int i, const QKnxByteArray &ba);

    QKnxByteArray &append(quint8 ch);
    QKnxByteArray &append(const QKnxByteArray &ba);
    inline QKnxByteArray &append(int count, quint8 ch) { return insert(size(), count, ch); }

    QKnxByteArray &replace(int index, int len, const QKnxByteArray &s);
    QKnxByteArray &replace(quint8 before, const QKnxByteArray &after);
    QKnxByteArray &replace(const QKnxByteArray &before, const QKnxByteArray &after);
    QKnxByteArray &replace(quint8 before, quint8 after);

    QKnxByteArray &remove(int index, int len);

    inline quint8 *data() { return reinterpret_cast <quint8 *> (m_bytes.data()); }
    inline const quint8 *data() const {
        return reinterpret_cast <const quint8 *> (m_bytes.data()); }
    inline const quint8 *constData() const {
        return reinterpret_cast <const quint8 *> (m_bytes.constData()); }

    int indexOf(quint8 ch, int from = 0) const;
    int indexOf(const QKnxByteArray &ba, int from = 0) const;
    int lastIndexOf(quint8 ch, int from = -1) const;
    int lastIndexOf(const QKnxByteArray &ba, int from = -1) const;

    inline bool contains(quint8 ch) const { return indexOf(ch) != -1; }
    inline bool contains(const QKnxByteArray &ba) const { return indexOf(ba) != -1; }

    bool startsWith(quint8 ch) const;
    bool startsWith(const QKnxByteArray &ba) const;

    bool endsWith(quint8 ch) const;
    bool endsWith(const QKnxByteArray &ba) const;

    QKnxByteArray toHex(quint8 separator = '\0') const;
    static QKnxByteArray fromHex(const QByteArray &hexEncoded);
    static QKnxByteArray fromHex(const QKnxByteArray &hexEncoded);

    typedef quint8 *iterator;
    typedef const quint8 *const_iterator;
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    inline iterator begin() { return reinterpret_cast <quint8 *> (m_bytes.begin()); }
    inline const_iterator begin() const { return reinterpret_cast <const quint8 *> (m_bytes.begin()); }
    inline const_iterator cbegin() const { return reinterpret_cast <const quint8 *> (m_bytes.cbegin()); }
    inline const_iterator constBegin() const { return reinterpret_cast <const quint8 *> (m_bytes.constBegin()); }
    inline iterator end() { return reinterpret_cast <quint8 *> (m_bytes.end()); }
    inline const_iterator end() const { return reinterpret_cast <const quint8 *> (m_bytes.end()); }
    inline const_iterator cend() const { return reinterpret_cast <const quint8 *> (m_bytes.cend()); }
    inline const_iterator constEnd() const { return reinterpret_cast <const quint8 *> (m_bytes.constEnd()); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

    // stl compatibility
    typedef int size_type;
    typedef qptrdiff difference_type;
    typedef const quint8 &const_reference;
    typedef quint8 &reference;
    typedef quint8 *pointer;
    typedef const quint8 *const_pointer;
    typedef quint8 value_type;

    inline QKnxByteArray &operator+=(quint8 ch) { return append(ch); }
    inline QKnxByteArray &operator+=(const QKnxByteArray &ba) { return append(ba); }

private:
    QByteArray m_bytes;
};

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
inline const QKnxByteArray operator+(const QKnxByteArray &ba, quint8 ch)
{
    return QKnxByteArray(ba) += ch;
}
inline const QKnxByteArray operator+(quint8 ch, const QKnxByteArray &ba)
{
    return QKnxByteArray(1, ch) += ba;
}

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxByteArray &);
Q_KNX_EXPORT Q_DECL_PURE_FUNCTION uint qHash(const QKnxByteArray &ba, uint seed = 0) Q_DECL_NOTHROW;

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QKnxByteArray)

#endif
