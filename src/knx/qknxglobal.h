/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#ifndef QKNXGLOBAL_H
#define QKNXGLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_KNX_LIB)
#    define Q_KNX_EXPORT Q_DECL_EXPORT
#  else
#    define Q_KNX_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_KNX_EXPORT
#endif

QT_END_NAMESPACE

#endif
