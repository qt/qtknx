/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXBYTESTORE_H
#define QKNXBYTESTORE_H

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxbytestoreref.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxByteStore
{
    friend class QKnxByteStoreRef;

public:
    quint16 size() const
    {
        return quint16(m_bytes.size());
    }

    void resize(quint16 size, const quint8 value = 0)
    {
        auto oldSize = m_bytes.size();
        if (size > oldSize)
            m_bytes += QKnxByteArray(size - oldSize, value);
        else
            m_bytes.resize(size);
    }

    quint8 byte(quint16 index) const
    {
        return m_bytes.value(index);
    }

    void setByte(quint16 index, quint8 byte)
    {
        if (size() <= index)
            resize(index + 1);
        m_bytes[index] = byte;
    }

    virtual QString toString() const
    {
        QString bytesString;
        for (quint8 byte : m_bytes)
            bytesString += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
        bytesString.chop(2);
        return QStringLiteral("Bytes { %1 }").arg(bytesString);
    }

    QKnxByteArray bytes() const
    {
        return m_bytes;
    }

    QKnxByteArray bytes(quint16 start, quint16 count) const
    {
        if (size() < start + count)
            return {};
        return m_bytes.mid(start, count);
    }

    void setBytes(const QKnxByteStoreRef &storeRef);

    void setBytes(const QKnxByteArray &sourceBytes)
    {
        m_bytes = sourceBytes;
    }

    void setBytes(QKnxByteArray::const_iterator sourceBegin, QKnxByteArray::const_iterator sourceEnd)
    {
        m_bytes.resize(std::distance(sourceBegin, sourceEnd));
        std::copy(sourceBegin, sourceEnd, std::begin(m_bytes));
    }

    void appendBytes(const QKnxByteArray &bytesToAppend)
    {
        m_bytes += bytesToAppend;
    }

    void insertBytes(quint16 pos, const QKnxByteArray &bytesToInsert)
    {
        m_bytes.insert(pos, bytesToInsert);
    }

    void replaceBytes(quint16 pos, const QKnxByteArray &replacement);

    QKnxByteStoreRef ref(quint16 index = 0) const;

protected:
    QKnxByteStore() = default;
    virtual ~QKnxByteStore() = default;

    explicit QKnxByteStore(quint8 byte);
    QKnxByteStore(const quint8 *bytes, quint16 size);

    const quint8 *data() const { return m_bytes.data(); }

private:
    QKnxByteArray m_bytes;
};

QT_END_NAMESPACE

#endif
