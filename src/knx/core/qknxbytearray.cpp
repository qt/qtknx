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

#include "qknxbytearray.h"
#include "private/qknxbytearraymatcher_p.h"

#define IS_RAW_DATA(d) ((d)->offset != sizeof(QKnxByteArrayData))

QT_BEGIN_NAMESPACE

int qFindByteArray(const quint8 *haystack0, int haystackLen, int from, const quint8 *needle0,
    int needleLen);

QKnxByteArray &QKnxByteArray::operator=(const QKnxByteArray &other) Q_DECL_NOTHROW
{
    other.d->ref.ref();
    if (!d->ref.deref())
        Data::deallocate(d);
    d = other.d;
    return *this;
}

void QKnxByteArray::truncate(int pos)
{
    if (pos < d->size)
        resize(pos);
}

void QKnxByteArray::chop(int n)
{
    if (n > 0)
        resize(d->size - n);
}

QKnxByteArray::QKnxByteArray(int size, quint8 ch)
{
    if (size <= 0) {
        d = Data::allocate(0);
    } else {
        d = Data::allocate(uint(size) + 1u);
        Q_CHECK_PTR(d);
        d->size = size;
        memset(d->data(), ch, size);
        d->data()[size] = '\0';
    }
}

QKnxByteArray::QKnxByteArray(int size, Qt::Initialization)
{
    d = Data::allocate(uint(size) + 1u);
    Q_CHECK_PTR(d);
    d->size = size;
    d->data()[size] = '\0';
}

QKnxByteArray::QKnxByteArray(const char *data, int size)
    : QKnxByteArray((const quint8*) data, size)
{}

QKnxByteArray::QKnxByteArray(const quint8 *data, int size)
{
    if (!data) {
        d = Data::sharedNull();
    } else {
        if (size < 0)
            size = 0;
        if (!size) {
            d = Data::allocate(0);
        } else {
            d = Data::allocate(uint(size) + 1u);
            Q_CHECK_PTR(d);
            d->size = size;
            memcpy(d->data(), data, size);
            d->data()[size] = '\0';
        }
    }
}

QKnxByteArray::QKnxByteArray(std::initializer_list<quint8> args)
{
    if (args.size() > 0) {
        d = Data::allocate(args.size() + 1u);
        Q_CHECK_PTR(d);
        d->size = int(args.size());
        memcpy(d->data(), args.begin(), (args.end() - args.begin()) * sizeof(quint8));
        d->data()[d->size] = '\0';
    } else {
        d = Data::allocate(0);
    }
}

void QKnxByteArray::resize(int size)
{
    if (size < 0)
        size = 0;

    if (IS_RAW_DATA(d) && !d->ref.isShared() && size < d->size) {
        d->size = size;
        return;
    }

    if (size == 0 && !d->capacityReserved) {
        Data *x = Data::allocate(0);
        if (!d->ref.deref())
            Data::deallocate(d);
        d = x;
    } else if (d->size == 0 && d->ref.isStatic()) {
        //
        // Optimize the idiom:
        //    QKnxByteArray a;
        //    a.resize(sz);
        //    ...
        // which is used in place of the Qt 3 idiom:
        //    QKnxByteArray a(sz);
        //
        Data *x = Data::allocate(uint(size) + 1u);
        Q_CHECK_PTR(x);
        x->size = size;
        x->data()[size] = '\0';
        d = x;
    } else {
        if (d->ref.isShared() || uint(size) + 1u > d->alloc
            || (!d->capacityReserved && size < d->size
                && uint(size) + 1u < uint(d->alloc >> 1)))
            reallocData(uint(size) + 1u, d->detachFlags() | Data::Grow);
        if (d->alloc) {
            d->size = size;
            d->data()[size] = '\0';
        }
    }
}

QKnxByteArray &QKnxByteArray::fill(quint8 ch, int size)
{
    resize(size < 0 ? d->size : size);
    if (d->size)
        memset(d->data(), ch, d->size);
    return *this;
}

void QKnxByteArray::reallocData(uint alloc, Data::AllocationOptions options)
{
    if (d->ref.isShared() || IS_RAW_DATA(d)) {
        Data *x = Data::allocate(alloc, options);
        Q_CHECK_PTR(x);
        x->size = qMin(int(alloc) - 1, d->size);
        ::memcpy(x->data(), d->data(), x->size);
        x->data()[x->size] = '\0';
        if (!d->ref.deref())
            Data::deallocate(d);
        d = x;
    } else {
        Data *x = Data::reallocateUnaligned(d, alloc, options);
        Q_CHECK_PTR(x);
        d = x;
    }
}

void QKnxByteArray::expand(int i)
{
    resize(qMax(i + 1, d->size));
}

QKnxByteArray QKnxByteArray::nulTerminated() const
{
    // is this fromRawData?
    if (!IS_RAW_DATA(d))
        return *this;           // no, then we're sure we're zero terminated

    QKnxByteArray copy(*this);
    copy.detach();
    return copy;
}

QKnxByteArray &QKnxByteArray::prepend(const QKnxByteArray &ba)
{
    if (d->size == 0 && d->ref.isStatic() && !IS_RAW_DATA(ba.d)) {
        *this = ba;
    } else if (ba.d->size != 0) {
        QKnxByteArray tmp = *this;
        *this = ba;
        append(tmp);
    }
    return *this;
}

QKnxByteArray &QKnxByteArray::prepend(const quint8 *str, int len)
{
    if (str) {
        if (d->ref.isShared() || uint(d->size + len) + 1u > d->alloc)
            reallocData(uint(d->size + len) + 1u, d->detachFlags() | Data::Grow);
        memmove(d->data() + len, d->data(), d->size);
        memcpy(d->data(), str, len);
        d->size += len;
        d->data()[d->size] = '\0';
    }
    return *this;
}

QKnxByteArray &QKnxByteArray::prepend(quint8 ch)
{
    if (d->ref.isShared() || uint(d->size) + 2u > d->alloc)
        reallocData(uint(d->size) + 2u, d->detachFlags() | Data::Grow);
    memmove(d->data() + 1, d->data(), d->size);
    d->data()[0] = ch;
    ++d->size;
    d->data()[d->size] = '\0';
    return *this;
}

QKnxByteArray &QKnxByteArray::append(const QKnxByteArray &ba)
{
    if (d->size == 0 && d->ref.isStatic() && !IS_RAW_DATA(ba.d)) {
        *this = ba;
    } else if (ba.d->size != 0) {
        if (d->ref.isShared() || uint(d->size + ba.d->size) + 1u > d->alloc)
            reallocData(uint(d->size + ba.d->size) + 1u, d->detachFlags() | Data::Grow);
        memcpy(d->data() + d->size, ba.d->data(), ba.d->size);
        d->size += ba.d->size;
        d->data()[d->size] = '\0';
    }
    return *this;
}

QKnxByteArray &QKnxByteArray::append(const quint8 *str, int len)
{
    if (len < 0)
        len = 0;
    if (str && len) {
        if (d->ref.isShared() || uint(d->size + len) + 1u > d->alloc)
            reallocData(uint(d->size + len) + 1u, d->detachFlags() | Data::Grow);
        memcpy(d->data() + d->size, str, len); // include null terminator
        d->size += len;
        d->data()[d->size] = '\0';
    }
    return *this;
}

QKnxByteArray& QKnxByteArray::append(quint8 ch)
{
    if (d->ref.isShared() || uint(d->size) + 2u > d->alloc)
        reallocData(uint(d->size) + 2u, d->detachFlags() | Data::Grow);
    d->data()[d->size++] = ch;
    d->data()[d->size] = '\0';
    return *this;
}

static inline QKnxByteArray &QKnxByteArray_insert(QKnxByteArray *ba,
    int pos, const quint8 *arr, int len)
{
    Q_ASSERT(pos >= 0);

    if (pos < 0 || len <= 0 || arr == 0)
        return *ba;

    int oldsize = ba->size();
    ba->resize(qMax(pos, oldsize) + len);
    quint8 *dst = ba->data();
    if (pos > oldsize)
        ::memset(dst + oldsize, 0x20, pos - oldsize);
    else
        ::memmove(dst + pos + len, dst + pos, oldsize - pos);
    memcpy(dst + pos, arr, len);
    return *ba;
}

QKnxByteArray &QKnxByteArray::insert(int i, const QKnxByteArray &ba)
{
    QKnxByteArray copy(ba);
    return QKnxByteArray_insert(this, i, copy.d->data(), copy.d->size);
}

QKnxByteArray &QKnxByteArray::insert(int i, const quint8 *str, int len)
{
    return QKnxByteArray_insert(this, i, str, len);
}

QKnxByteArray &QKnxByteArray::insert(int i, quint8 ch)
{
    return QKnxByteArray_insert(this, i, &ch, 1);
}

QKnxByteArray &QKnxByteArray::insert(int i, int count, quint8 ch)
{
    if (i < 0 || count <= 0)
        return *this;

    int oldsize = size();
    resize(qMax(i, oldsize) + count);
    quint8 *dst = d->data();
    if (i > oldsize)
        ::memset(dst + oldsize, 0x20, i - oldsize);
    else if (i < oldsize)
        ::memmove(dst + i + count, dst + i, oldsize - i);
    ::memset(dst + i, ch, count);
    return *this;
}

QKnxByteArray &QKnxByteArray::remove(int pos, int len)
{
    if (len <= 0 || uint(pos) >= uint(d->size))
        return *this;
    detach();
    if (len >= d->size - pos) {
        resize(pos);
    } else {
        memmove(d->data() + pos, d->data() + pos + len, d->size - pos - len);
        resize(d->size - len);
    }
    return *this;
}

QKnxByteArray &QKnxByteArray::replace(int pos, int len, const QKnxByteArray &after)
{
    if (len == after.d->size && (pos + len <= d->size)) {
        detach();
        memmove(d->data() + pos, after.d->data(), len * sizeof(quint8));
        return *this;
    } else {
        QKnxByteArray copy(after);
        // ### optimize me
        remove(pos, len);
        return insert(pos, copy);
    }
}

QKnxByteArray &QKnxByteArray::replace(int pos, int len, const quint8 *after, int alen)
{
    if (len == alen && (pos + len <= d->size)) {
        detach();
        memcpy(d->data() + pos, after, len * sizeof(quint8));
        return *this;
    } else {
        remove(pos, len);
        return QKnxByteArray_insert(this, pos, after, alen);
    }
}

QKnxByteArray &QKnxByteArray::replace(const QKnxByteArray &before, const QKnxByteArray &after)
{
    if (isNull() || before.d == after.d)
        return *this;

    QKnxByteArray aft = after;
    if (after.d == d)
        aft.detach();

    return replace(before.constData(), before.size(), aft.constData(), aft.size());
}

QKnxByteArray &QKnxByteArray::replace(const quint8 *before, int bsize, const quint8 *after, int asize)
{
    if (isNull() || (before == after && bsize == asize))
        return *this;

    // protect against before or after being part of this
    const quint8 *a = after;
    const quint8 *b = before;
    if (after >= d->data() && after < d->data() + d->size) {
        quint8 *copy = (quint8 *) malloc(asize);
        Q_CHECK_PTR(copy);
        memcpy(copy, after, asize);
        a = copy;
    }
    if (before >= d->data() && before < d->data() + d->size) {
        quint8 *copy = (quint8 *) malloc(bsize);
        Q_CHECK_PTR(copy);
        memcpy(copy, before, bsize);
        b = copy;
    }

    QKnxByteArrayMatcher matcher(before, bsize);
    int index = 0;
    int len = d->size;
    quint8 *d = data();

    if (bsize == asize) {
        if (bsize) {
            while ((index = matcher.indexIn(*this, index)) != -1) {
                memcpy(d + index, after, asize);
                index += bsize;
            }
        }
    } else if (asize < bsize) {
        uint to = 0;
        uint movestart = 0;
        uint num = 0;
        while ((index = matcher.indexIn(*this, index)) != -1) {
            if (num) {
                int msize = index - movestart;
                if (msize > 0) {
                    memmove(d + to, d + movestart, msize);
                    to += msize;
                }
            } else {
                to = index;
            }
            if (asize) {
                memcpy(d + to, after, asize);
                to += asize;
            }
            index += bsize;
            movestart = index;
            num++;
        }
        if (num) {
            int msize = len - movestart;
            if (msize > 0)
                memmove(d + to, d + movestart, msize);
            resize(len - num*(bsize - asize));
        }
    } else {
        // the most complex case. We don't want to lose performance by doing repeated
        // copies and reallocs of the string.
        while (index != -1) {
            uint indices[4096];
            uint pos = 0;
            while (pos < 4095) {
                index = matcher.indexIn(*this, index);
                if (index == -1)
                    break;
                indices[pos++] = index;
                index += bsize;
                // avoid infinite loop
                if (!bsize)
                    index++;
            }
            if (!pos)
                break;

            // we have a table of replacement positions, use them for fast replacing
            int adjust = pos*(asize - bsize);
            // index has to be adjusted in case we get back into the loop above.
            if (index != -1)
                index += adjust;
            int newlen = len + adjust;
            int moveend = len;
            if (newlen > len) {
                resize(newlen);
                len = newlen;
            }
            d = this->d->data();

            while (pos) {
                pos--;
                int movestart = indices[pos] + bsize;
                int insertstart = indices[pos] + pos*(asize - bsize);
                int moveto = insertstart + asize;
                memmove(d + moveto, d + movestart, (moveend - movestart));
                if (asize)
                    memcpy(d + insertstart, after, asize);
                moveend = movestart - bsize;
            }
        }
    }

    if (a != after)
        ::free(const_cast<quint8 *>(a));
    if (b != before)
        ::free(const_cast<quint8 *>(b));

    return *this;
}

QKnxByteArray &QKnxByteArray::replace(quint8 before, const QKnxByteArray &after)
{
    quint8 b[2] = { before, '\0' };
    QKnxByteArray cb = fromRawData(b, 1);
    return replace(cb, after);
}

QKnxByteArray &QKnxByteArray::replace(quint8 before, quint8 after)
{
    if (d->size) {
        quint8 *i = data();
        quint8 *e = i + d->size;
        for (; i != e; ++i)
            if (*i == before)
                * i = after;
    }
    return *this;
}

QVector<QKnxByteArray> QKnxByteArray::split(quint8 sep) const
{
    QVector<QKnxByteArray> list;
    int start = 0;
    int end;
    while ((end = indexOf(sep, start)) != -1) {
        list.append(mid(start, end - start));
        start = end + 1;
    }
    list.append(mid(start));
    return list;
}

QKnxByteArray QKnxByteArray::repeated(int times) const
{
    if (d->size == 0)
        return *this;

    if (times <= 1) {
        if (times == 1)
            return *this;
        return QKnxByteArray();
    }

    const int resultSize = times * d->size;

    QKnxByteArray result;
    result.reserve(resultSize);
    if (result.d->alloc != uint(resultSize) + 1u)
        return QKnxByteArray(); // not enough memory

    memcpy(result.d->data(), d->data(), d->size);

    int sizeSoFar = d->size;
    quint8 *end = result.d->data() + sizeSoFar;

    const int halfResultSize = resultSize >> 1;
    while (sizeSoFar <= halfResultSize) {
        memcpy(end, result.d->data(), sizeSoFar);
        end += sizeSoFar;
        sizeSoFar <<= 1;
    }
    memcpy(end, result.d->data(), resultSize - sizeSoFar);
    result.d->data()[resultSize] = '\0';
    result.d->size = resultSize;
    return result;
}

int QKnxByteArray::indexOf(const QKnxByteArray &ba, int from) const
{
    const int ol = ba.d->size;
    if (ol == 0)
        return from;
    if (ol == 1)
        return indexOf(*ba.d->data(), from);

    const int l = d->size;
    if (from > d->size || ol + from > l)
        return -1;
    return qFindByteArray(d->data(), d->size, from, ba.d->data(), ol);
}

int QKnxByteArray::indexOf(quint8 ch, int from) const
{
    if (from < 0)
        from = qMax(from + d->size, 0);
    if (from < d->size) {
        const quint8 *n = d->data() + from - 1;
        const quint8 *e = d->data() + d->size;
        while (++n != e)
            if (*n == ch)
                return  n - d->data();
    }
    return -1;
}

static int lastIndexOfHelper(const quint8 *haystack, int l, const quint8 *needle, int ol, int from)
{
    int delta = l - ol;
    if (from < 0)
        from = delta;
    if (from < 0 || from > l)
        return -1;
    if (from > delta)
        from = delta;

    const quint8 *end = haystack;
    haystack += from;
    const uint ol_minus_1 = ol - 1;
    const quint8 *n = needle + ol_minus_1;
    const quint8 *h = haystack + ol_minus_1;
    uint hashNeedle = 0, hashHaystack = 0;
    int idx;
    for (idx = 0; idx < ol; ++idx) {
        hashNeedle = ((hashNeedle << 1) + *(n - idx));
        hashHaystack = ((hashHaystack << 1) + *(h - idx));
    }
    hashHaystack -= *haystack;

#define REHASH(a) \
    if (ol_minus_1 < sizeof(uint) * CHAR_BIT) \
        hashHaystack -= (a) << ol_minus_1; \
    hashHaystack <<= 1

    while (haystack >= end) {
        hashHaystack += *haystack;
        if (hashHaystack == hashNeedle && memcmp(needle, haystack, ol) == 0)
            return haystack - end;
        --haystack;
        REHASH(*(haystack + ol));
    }
    return -1;

#undef REHASHS
}

int QKnxByteArray::lastIndexOf(const QKnxByteArray &ba, int from) const
{
    const int ol = ba.d->size;
    if (ol == 1)
        return lastIndexOf(*ba.d->data(), from);
    return lastIndexOfHelper(d->data(), d->size, ba.d->data(), ol, from);
}

int QKnxByteArray::lastIndexOf(quint8 ch, int from) const
{
    if (from < 0)
        from += d->size;
    else if (from > d->size)
        from = d->size - 1;
    if (from >= 0) {
        const quint8 *b = d->data();
        const quint8 *n = d->data() + from + 1;
        while (n-- != b)
            if (*n == ch)
                return  n - b;
    }
    return -1;
}

int QKnxByteArray::count(const QKnxByteArray &ba) const
{
    int num = 0;
    int i = -1;
    if (d->size > 500 && ba.d->size > 5) {
        QKnxByteArrayMatcher matcher(ba);
        while ((i = matcher.indexIn(*this, i + 1)) != -1)
            ++num;
    } else {
        while ((i = indexOf(ba, i + 1)) != -1)
            ++num;
    }
    return num;
}

int QKnxByteArray::count(quint8 ch) const
{
    int num = 0;
    const quint8 *i = d->data() + d->size;
    const quint8 *b = d->data();
    while (i != b)
        if (*--i == ch)
            ++num;
    return num;
}

bool QKnxByteArray::startsWith(const QKnxByteArray &ba) const
{
    if (d == ba.d || ba.d->size == 0)
        return true;
    if (d->size < ba.d->size)
        return false;
    return memcmp(d->data(), ba.d->data(), ba.d->size) == 0;
}

bool QKnxByteArray::startsWith(quint8 ch) const
{
    if (d->size == 0)
        return false;
    return d->data()[0] == ch;
}

bool QKnxByteArray::endsWith(const QKnxByteArray &ba) const
{
    if (d == ba.d || ba.d->size == 0)
        return true;
    if (d->size < ba.d->size)
        return false;
    return memcmp(d->data() + d->size - ba.d->size, ba.d->data(), ba.d->size) == 0;
}

bool QKnxByteArray::endsWith(quint8 ch) const
{
    if (d->size == 0)
        return false;
    return d->data()[d->size - 1] == ch;
}

QKnxByteArray QKnxByteArray::left(int len)  const
{
    if (len >= d->size)
        return *this;
    if (len < 0)
        len = 0;
    return QKnxByteArray(d->data(), len);
}

QKnxByteArray QKnxByteArray::right(int len) const
{
    if (len >= d->size)
        return *this;
    if (len < 0)
        len = 0;
    return QKnxByteArray(d->data() + d->size - len, len);
}

QKnxByteArray QKnxByteArray::mid(int pos, int len) const
{
    using namespace QtPrivate;
    switch (QContainerImplHelper::mid(size(), &pos, &len)) {
    case QContainerImplHelper::Null:
        return QKnxByteArray();
    case QContainerImplHelper::Empty:
    {
        return QKnxByteArray(Data::allocate(0));
    }
    case QContainerImplHelper::Full:
        return *this;
    case QContainerImplHelper::Subset:
        return QKnxByteArray(d->data() + pos, len);
    }
    Q_UNREACHABLE();
    return QKnxByteArray();
}

void QKnxByteArray::clear()
{
    if (!d->ref.deref())
        Data::deallocate(d);
    d = Data::sharedNull();
}

#if !defined(QT_NO_DATASTREAM)

QDataStream &operator<<(QDataStream &out, const QKnxByteArray &ba)
{
    if (ba.isNull() && out.version() >= 6) {
        out << (quint32) 0xffffffff;
        return out;
    }
    return out.writeBytes((const char*) ba.constData(), ba.size());
}

QDataStream &operator >> (QDataStream &in, QKnxByteArray &ba)
{
    ba.clear();
    quint32 len;
    in >> len;
    if (len == 0xffffffff)
        return in;

    const quint32 Step = 1024 * 1024;
    quint32 allocated = 0;

    do {
        int blockSize = qMin(Step, len - allocated);
        ba.resize(allocated + blockSize);
        if (in.readRawData((char*) ba.data() + allocated, blockSize) != blockSize) {
            ba.clear();
            in.setStatus(QDataStream::ReadPastEnd);
            return in;
        }
        allocated += blockSize;
    } while (allocated < len);

    return in;
}

#endif // QT_NO_DATASTREAM

QKnxByteArray QKnxByteArray::leftJustified(int width, quint8 fill, bool truncate) const
{
    QKnxByteArray result;
    int len = d->size;
    int padlen = width - len;
    if (padlen > 0) {
        result.resize(len + padlen);
        if (len)
            memcpy(result.d->data(), d->data(), len);
        memset(result.d->data() + len, fill, padlen);
    } else {
        if (truncate)
            result = left(width);
        else
            result = *this;
    }
    return result;
}

QKnxByteArray QKnxByteArray::rightJustified(int width, quint8 fill, bool truncate) const
{
    QKnxByteArray result;
    int len = d->size;
    int padlen = width - len;
    if (padlen > 0) {
        result.resize(len + padlen);
        if (len)
            memcpy(result.d->data() + padlen, data(), len);
        memset(result.d->data(), fill, padlen);
    } else {
        if (truncate)
            result = left(width);
        else
            result = *this;
    }
    return result;
}

bool QKnxByteArray::isNull() const
{
    return d == QArrayData::sharedNull();
}

QKnxByteArray &QKnxByteArray::setRawData(const quint8 *data, int size)
{
    if (d->ref.isShared() || d->alloc) {
        *this = fromRawData(data, size);
    } else {
        if (data) {
            d->size = size;
            d->offset = data - reinterpret_cast<quint8 *>(d);
        } else {
            d->offset = sizeof(QKnxByteArrayData);
            d->size = 0;
            *d->data() = 0;
        }
    }
    return *this;
}

QKnxByteArray QKnxByteArray::fromRawData(const quint8 *data, int size)
{
    Data *x { nullptr };
    if (!data) {
        x = Data::sharedNull();
    } else if (!size) {
        x = Data::allocate(0);
    } else {
        x = Data::fromRawData(data, size);
        Q_CHECK_PTR(x);
    }
    return QKnxByteArray(x);
}

namespace QtKnxUtils {

Q_DECL_CONSTEXPR inline char toHexUpper(uint value) Q_DECL_NOTHROW
{
    return "0123456789ABCDEF"[value & 0xF];
}

Q_DECL_CONSTEXPR inline char toHexLower(uint value) Q_DECL_NOTHROW
{
    return "0123456789abcdef"[value & 0xF];
}

Q_DECL_CONSTEXPR inline int fromHex(uint c) Q_DECL_NOTHROW
{
    return ((c >= '0') && (c <= '9')) ? int(c - '0') :
           ((c >= 'A') && (c <= 'F')) ? int(c - 'A' + 10) :
           ((c >= 'a') && (c <= 'f')) ? int(c - 'a' + 10) :
           /* otherwise */              -1;
}

}

QKnxByteArray QKnxByteArray::toHex(quint8 separator) const
{
    if (!d->size)
        return QKnxByteArray();

    const int length = separator ? (d->size * 3 - 1) : (d->size * 2);
    QKnxByteArray hex(length, Qt::Uninitialized);
    quint8 *hexData = hex.data();
    const uchar *data = (const uchar *) d->data();
    for (int i = 0, o = 0; i < d->size; ++i) {
        hexData[o++] = QtKnxUtils::toHexLower(data[i] >> 4);
        hexData[o++] = QtKnxUtils::toHexLower(data[i] & 0xf);

        if ((separator) && (o < length))
            hexData[o++] = separator;
    }
    return hex;
}

QKnxByteArray QKnxByteArray::fromHex(const QByteArray &hexEncoded)
{
    return QKnxByteArray::fromHex(QKnxByteArray { hexEncoded.constData(), hexEncoded.size() });
}

QKnxByteArray QKnxByteArray::fromHex(const QKnxByteArray &hexEncoded)
{
    QKnxByteArray res((hexEncoded.size() + 1) / 2, Qt::Uninitialized);
    uchar *result = (uchar *) res.data() + res.size();

    bool odd_digit = true;
    for (int i = hexEncoded.size() - 1; i >= 0; --i) {
        uchar ch = uchar(hexEncoded.at(i));
        int tmp = QtKnxUtils::fromHex(ch);
        if (tmp == -1)
            continue;
        if (odd_digit) {
            --result;
            *result = tmp;
            odd_digit = false;
        } else {
            *result |= tmp << 4;
            odd_digit = true;
        }
    }

    res.remove(0, result - (const uchar *) res.constData());
    return res;
}

QKnxByteArray::operator QByteArray() const
{
    return { (const char*) data(), size() };
}

QKnxByteArray QKnxByteArray::fromByteArray(const QByteArray &ba)
{
    return QKnxByteArray(ba.constData(), ba.size());
}

/*!
    \relates QKnxByteArray

    Writes the QKnxByteArray \a byteArray to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxByteArray &byteArray)
{
    debug << static_cast<QByteArray> (byteArray); return debug;
}

uint qHash(const QKnxByteArray &ba, uint seed) Q_DECL_NOTHROW
{
    return qHash(static_cast<QByteArray> (ba), seed);
}

QT_END_NAMESPACE
