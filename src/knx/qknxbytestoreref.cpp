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

#include "qknxbytestore.h"
#include "qknxbytestoreref.h"

QT_BEGIN_NAMESPACE

QKnxByteStoreRef::QKnxByteStoreRef(QKnxByteStore *store, quint16 index)
{
    if (store && index < store->size()) {
        m_index = index;
        m_store = store;
    }
}

quint16 QKnxByteStoreRef::size() const
{
    if (m_store)
        return m_store->size() - m_index;
    return 0;
}

const quint8 *QKnxByteStoreRef::bytes() const
{
    if (m_store)
        return &(*std::next(m_store->data(), m_index));
    return nullptr;
}

QT_END_NAMESPACE
