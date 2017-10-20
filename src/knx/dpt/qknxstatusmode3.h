/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#ifndef QKNXSTATUSMODE3_H
#define QKNXSTATUSMODE3_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxStatusMode3 : public QKnxDatapointType
{
    Q_GADGET

public:
    enum class Mode
    {
        Zero = 0x01,
        One = 0x02,
        Two = 0x04,
        Unknown = 0x08
    };
    Q_ENUM(Mode)

    enum Status
    {
        A = 0x01,
        B = 0x02,
        C = 0x04,
        D = 0x08,
        E = 0x0a
    };
    Q_ENUM(Status)
    Q_DECLARE_FLAGS(StatusFlags, Status)

    QKnxStatusMode3();
    explicit QKnxStatusMode3(Mode mode, StatusFlags statusFlags);

    static const constexpr int TypeSize = 0x01;
    static const constexpr int MainType = 0x06;
    static const constexpr int SubType = 0x14;

    Mode mode() const;
    bool setMode(Mode mode);

    StatusFlags statusFlags() const;
    bool setStatusFlags(StatusFlags statusFlags);

    bool isSet(Status status) const;
    bool setStatus(Status status);
    bool removeStatus(Status status);

    bool isValid() const override;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxStatusMode3::StatusFlags)

QT_END_NAMESPACE

#endif
