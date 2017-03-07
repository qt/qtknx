/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetip.h>
#include <QtKnx/qknxnetipframeheader.h>
#include <QtKnx/qknxnetippayload.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpFrame
{
public:
    quint8 code() const;
    void setCode(quint8 code);

    quint16 size() const;
    QKnxNetIpFrameHeader header() const;

    QKnxNetIpPayload payload() const;
    void setPayload(const QKnxNetIpPayload &payload);

    virtual bool isValid() const;
    virtual QString toString() const;
    virtual ~QKnxNetIpFrame() = default;

protected:
    QKnxNetIpFrame() = default;

private:
    QKnxNetIpFrameHeader m_header;
    QKnxNetIpPayload m_payload;
};

QT_END_NAMESPACE
