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

#include <QtCore/qglobal.h>

#if !defined(Q_OS_WIN)
# if defined(Q_CC_CLANG)
#  if defined(__apple_build_version__)
#    if __apple_build_version__ == 8020041
#     error "Apple LLVM version 8.1.0 (clang-802.0.41)"
#   endif
#  endif
# elif defined(Q_CC_GNU)
#  if Q_CC_GNU < 409
#   error "GCC version less than 4.9.0"
#  endif
# endif
#else
# if defined(Q_CC_MSVC)
#  if Q_CC_MSVC < 1900
#   error "Visual C++ compiler less than version 19."
#  endif
# endif
#endif

struct ConstExpr
{
    static constexpr const quint16 DefaultPort = 3671;
    static constexpr const char *MulticastAddress = "224.0.23.12";
};

int main(int /*argc*/, char** /*argv*/)
{
    auto port = ConstExpr::DefaultPort;
    Q_UNUSED(port)
    auto address = ConstExpr::MulticastAddress;
    Q_UNUSED(address)

    return 0;
}
