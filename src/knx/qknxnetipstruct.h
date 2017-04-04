/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXNETIPSTRUCT_H
#define QKNXNETIPSTRUCT_H

#include <QtKnx/qknxnetippackage.h>

QT_BEGIN_NAMESPACE

using QKnxNetIpStruct = QKnxNetIpPackage<quint8, QKnxNetIpStructHeader>;

QT_END_NAMESPACE

#endif
