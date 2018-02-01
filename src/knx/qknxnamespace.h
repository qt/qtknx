/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXNAMESPACE_H
#define QKNXNAMESPACE_H

#include <QtCore/qmetaobject.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

namespace QKnx
{
    //Q_NAMESPACE

    enum class MediumType : quint8
    {
        Unknown = 0x00,
        TP = 0x01,
        PL = 0x04,
        RF = 0x10,
        NetIP = 0x20
    };
    //Q_ENUM_NS(MediumType)

    struct CemiServer final
    {
        enum class ReturnCode : quint8
        {
            NoError = 0x00,
            Error = 0x01
        };

        enum class Error : quint8
        {
            Unspecified = 0x00,
            OutOfRange = 0x01,
            OutOfMaxRange = 0x02,
            OutOfMinRange = 0x03,
            Memory = 0x04,
            ReadOnly = 0x05,
            IllegalCommand = 0x06,
            NonExistingProperty = 0x07,
            TypeConflict = 0x08,
            PropertyIndexRangeError = 0x09,
            TemporaryNotWritable = 0x0a,
            None = 0xff  // Qt extension
        };
        // See 03_06_03 EMI_IMI v01.03.03 AS.pdf Paragraph 4.1.7.3.7.2 (Table 12)
    };

}

QT_END_NAMESPACE

#endif
