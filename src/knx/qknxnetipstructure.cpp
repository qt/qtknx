/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipstructure.h"

QT_BEGIN_NAMESPACE

quint8 QKnxNetIpStructure::code() const
{
    return m_code;
}

/*
    03_08_02 Core v01.05.01 AS.pdf
    7.5.4.1 Use, format and general requirements: The structure shall
    always have an even number of octets which may have to be achieved by
    padding with 00h in the last octet of the structure.
*/
void QKnxNetIpStructure::padData(int length)
{
    if (m_size % 2 || m_size < length) {
        m_size++;
        m_data.resize(length >= 0 ? length : m_size);
    }
}

qint32 QKnxNetIpStructure::dataSize() const
{
    return m_size;
}

qint32 QKnxNetIpStructure::rawSize() const
{
    return m_size > 0xfc ? m_size + 4 : m_size + 2;
}

QT_END_NAMESPACE
