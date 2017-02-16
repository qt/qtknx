/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

QT_BEGIN_NAMESPACE

#include <deque>
#include <vector>

class QKnxTypeCheck
{
public:
    template <typename T, typename ... Args> static void FailIfNot()
    {
        static_assert(IsType<T, Args...>::value, "Unsupported argument type.");
    }

private:
    template <typename T, typename ... Ts> struct IsType
    {
        enum { value = false };
    };

    template <typename T, typename T1, typename ... Ts> struct IsType<T, T1, Ts...>
    {
        enum { value = std::is_same<T, T1>::value || IsType<T, Ts...>::value };
    };
};

QT_END_NAMESPACE
