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

#ifndef QKNXTUNNELFRAMEFACTORY_H
#define QKNXTUNNELFRAMEFACTORY_H

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtunnelframe.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxTunnelFrameFactory final
{
public:
    QKnxTunnelFrameFactory() = delete;

    struct Q_KNX_EXPORT GroupValue
    {
        GroupValue() = delete;

        static QKnxTunnelFrame createReadRequest(const QKnxAddress &src, const QKnxAddress &dest);
        static QKnxTunnelFrame createReadRequest(const QKnxAddress &src, const QKnxAddress &dest,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxTunnelFrame createReadConfirmation(const QKnxAddress &src, const QKnxAddress &dest,
                            QKnxControlField::Confirm status = QKnxControlField::Confirm::NoError);
        static QKnxTunnelFrame createReadConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QKnxControlField &ctrl,
                            const QKnxExtendedControlField &extCtrl);

        static QKnxTunnelFrame createReadIndication(const QKnxAddress &src,
                            const QKnxAddress &dest);
        static QKnxTunnelFrame createReadIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QKnxControlField &ctrl,
                            const QKnxExtendedControlField &extCtrl);

        static QKnxTunnelFrame createResponseRequest(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data);
        static QKnxTunnelFrame createResponseRequest(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxTunnelFrame createResponseConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            QKnxControlField::Confirm status = QKnxControlField::Confirm::NoError);
        static QKnxTunnelFrame createResponseConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxTunnelFrame createResponseIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data);
        static QKnxTunnelFrame createResponseIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxTunnelFrame createWriteRequest(const QKnxAddress &src, const QKnxAddress &dest,
                            const QVector<quint8> &data);
        static QKnxTunnelFrame createWriteRequest(const QKnxAddress &src, const QKnxAddress &dest,
                            const QVector<quint8> &data, const QKnxControlField &ctrl,
                            const QKnxExtendedControlField &extCtrl);

        static QKnxTunnelFrame createWriteConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            QKnxControlField::Confirm status = QKnxControlField::Confirm::NoError);
        static QKnxTunnelFrame createWriteConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxTunnelFrame createWriteIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data);
        static QKnxTunnelFrame createWriteIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxControlField createRequestControlField(
            QKnxControlField::Acknowledge acknowledge = QKnxControlField::Acknowledge::NotRequested,
            QKnxControlField::Priority priority = QKnxControlField::Priority::Normal);

        static QKnxControlField createConfirmationControlField(QKnxControlField::Confirm status,
            QKnxControlField::Acknowledge acknowledge = QKnxControlField::Acknowledge::NotRequested,
            QKnxControlField::Priority priority = QKnxControlField::Priority::Normal);

        static QKnxControlField createIndicationControlField(
            QKnxControlField::Priority priority = QKnxControlField::Priority::Normal);

        static QKnxExtendedControlField createExtentedControlField(quint8 hopCount = 6,
            QKnxExtendedControlField::ExtendedFrameFormat format =
                QKnxExtendedControlField::ExtendedFrameFormat::Standard);
    };
};

QT_END_NAMESPACE

#endif // QKNXTUNNELFRAMEFACTORY_H
