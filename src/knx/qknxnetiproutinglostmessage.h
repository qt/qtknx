/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPROUTINGLOSTMESSAGE_H
#define QKNXNETIPROUTINGLOSTMESSAGE_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtraits.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpRoutingLostMessage final : private QKnxNetIpFrame
{
public:
    QKnxNetIpRoutingLostMessage() = default;
    ~QKnxNetIpRoutingLostMessage() override = default;
};

QT_END_NAMESPACE

#endif
