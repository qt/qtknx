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

#ifndef QKNXTRAITS_H
#define QKNXTRAITS_H

#include <QtKnx/qknxglobal.h>

#include <array>
#include <bitset>
#include <deque>
#include <vector>

QT_BEGIN_NAMESPACE

template <typename T, typename ... Ts> struct is_type
{
    enum { value = false };
};

template <typename T, typename T1, typename ... Ts> struct is_type<T, T1, Ts...>
{
    enum { value = std::is_same<T, T1>::value || is_type<T, Ts...>::value };
};

QT_END_NAMESPACE

#endif
