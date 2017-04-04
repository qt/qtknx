/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPFRAME_H
#define QKNXNETIPFRAME_H

#include <QtKnx/qknxnetippackage.h>

QT_BEGIN_NAMESPACE

using QKnxNetIpFrame = QKnxNetIpPackage<quint16, QKnxNetIpFrameHeader>;

QT_END_NAMESPACE

#endif
