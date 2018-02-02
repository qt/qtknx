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

#ifndef QKNXLINKLAYERFRAMEFACTORY_H
#define QKNXLINKLAYERFRAMEFACTORY_H

#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxlinklayerframe.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxLinkLayerFrameFactory final
{
public:
    QKnxLinkLayerFrameFactory() = delete;

    static QKnxControlField createRequestControlField(
        QKnxControlField::Acknowledge acknowledge = QKnxControlField::Acknowledge::NotRequested,
        QKnxControlField::Priority priority = QKnxControlField::Priority::Normal);

    static QKnxControlField createConfirmationControlField(QKnxControlField::Confirm status,
        QKnxControlField::Acknowledge acknowledge = QKnxControlField::Acknowledge::NotRequested,
        QKnxControlField::Priority priority = QKnxControlField::Priority::Normal);

    static QKnxControlField createIndicationControlField(
        QKnxControlField::Priority priority = QKnxControlField::Priority::Normal);

    static QKnxExtendedControlField createExtentedControlField(QKnxAddress::Type type,
        quint8 hopCount = 6, QKnxExtendedControlField::ExtendedFrameFormat format =
            QKnxExtendedControlField::ExtendedFrameFormat::Standard);

    struct Q_KNX_EXPORT GroupValue
    {
        GroupValue() = delete;

        static QKnxLinkLayerFrame createReadRequest(const QKnxAddress &src, const QKnxAddress &dest);
        static QKnxLinkLayerFrame createReadRequest(const QKnxAddress &src, const QKnxAddress &dest,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxLinkLayerFrame createReadConfirmation(const QKnxAddress &src, const QKnxAddress &dest,
                            QKnxControlField::Confirm status = QKnxControlField::Confirm::NoError);
        static QKnxLinkLayerFrame createReadConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QKnxControlField &ctrl,
                            const QKnxExtendedControlField &extCtrl);

        static QKnxLinkLayerFrame createReadIndication(const QKnxAddress &src,
                            const QKnxAddress &dest);
        static QKnxLinkLayerFrame createReadIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QKnxControlField &ctrl,
                            const QKnxExtendedControlField &extCtrl);

        static QKnxLinkLayerFrame createResponseRequest(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data);
        static QKnxLinkLayerFrame createResponseRequest(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxLinkLayerFrame createResponseConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            QKnxControlField::Confirm status = QKnxControlField::Confirm::NoError);
        static QKnxLinkLayerFrame createResponseConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxLinkLayerFrame createResponseIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data);
        static QKnxLinkLayerFrame createResponseIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxLinkLayerFrame createWriteRequest(const QKnxAddress &src, const QKnxAddress &dest,
                            const QVector<quint8> &data);
        static QKnxLinkLayerFrame createWriteRequest(const QKnxAddress &src, const QKnxAddress &dest,
                            const QVector<quint8> &data, const QKnxControlField &ctrl,
                            const QKnxExtendedControlField &extCtrl);

        static QKnxLinkLayerFrame createWriteConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            QKnxControlField::Confirm status = QKnxControlField::Confirm::NoError);
        static QKnxLinkLayerFrame createWriteConfirmation(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);

        static QKnxLinkLayerFrame createWriteIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data);
        static QKnxLinkLayerFrame createWriteIndication(const QKnxAddress &src,
                            const QKnxAddress &dest, const QVector<quint8> &data,
                            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl);
    };

    struct Q_KNX_EXPORT Memory
    {
        Memory() = delete;
        static QKnxLinkLayerFrame createReadRequest(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, quint8 sequenceNumber = 0);
        static QKnxLinkLayerFrame createReadRequest(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QKnxControlField &ctrl,
            const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber = 0);

        static QKnxLinkLayerFrame createReadConfirmation(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, quint8 sequenceNumber = 0,
            QKnxControlField::Confirm status = QKnxControlField::Confirm::NoError);
        static QKnxLinkLayerFrame createReadConfirmation(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QKnxControlField &ctrl,
            const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber = 0);

        static QKnxLinkLayerFrame createReadIndication(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, quint8 sequenceNumber = 0);
        static QKnxLinkLayerFrame createReadIndication(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QKnxControlField &ctrl,
            const QKnxExtendedControlField &extCtrl, quint8 sequenceNumber = 0);

        static QKnxLinkLayerFrame createResponseRequest(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data,
            quint8 sequenceNumber = 0);
        static QKnxLinkLayerFrame createResponseRequest(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data,
            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl,
            quint8 sequenceNumber = 0);

        static QKnxLinkLayerFrame createResponseConfirmation(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data, quint8 sequenceNumber = 0,
            QKnxControlField::Confirm status = QKnxControlField::Confirm::NoError);
        static QKnxLinkLayerFrame createResponseConfirmation(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data,
            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl,
            quint8 sequenceNumber = 0);

        static QKnxLinkLayerFrame createResponseIndication(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data, quint8 sequenceNumber = 0);
        static QKnxLinkLayerFrame createResponseIndication(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data,
            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl,
            quint8 sequenceNumber = 0);

        static QKnxLinkLayerFrame createWriteRequest(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data,
            quint8 sequenceNumber = 0);
        static QKnxLinkLayerFrame createWriteRequest(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data,
            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl,
            quint8 sequenceNumber = 0);

        static QKnxLinkLayerFrame createWriteConfirmation(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data, quint8 sequenceNumber = 0,
            QKnxControlField::Confirm status = QKnxControlField::Confirm::NoError);
        static QKnxLinkLayerFrame createWriteConfirmation(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data,
            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl,
            quint8 sequenceNumber = 0);

        static QKnxLinkLayerFrame createWriteIndication(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data, quint8 sequenceNumber = 0);
        static QKnxLinkLayerFrame createWriteIndication(const QKnxAddress &src, const QKnxAddress &dest,
            quint16 memoryAddress, quint8 number, const QVector<quint8> &data,
            const QKnxControlField &ctrl, const QKnxExtendedControlField &extCtrl,
            quint8 sequenceNumber = 0);
    };
};

QT_END_NAMESPACE

#endif // QKNXLINKLAYERFRAMEFACTORY_H
