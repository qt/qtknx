/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipheader.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpFrame
{
public:
    QKnxNetIpHeader header() { return m_header; }
    void setHeader(const QKnxNetIpHeader &header) { m_header = header; }

    quint16 size() const { return m_header.headerSize() + m_frameSize; }

private:
    virtual ~QKnxNetIpFrame() = 0;

private:
    QKnxNetIpHeader m_header;

    quint16 m_frameSize = 0;
};

QT_END_NAMESPACE
