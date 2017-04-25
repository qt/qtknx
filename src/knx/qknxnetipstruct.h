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

Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxNetIpStruct &package);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &out, const QKnxNetIpStruct &package);

QT_END_NAMESPACE

#endif
