/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

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
