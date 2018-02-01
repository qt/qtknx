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

#ifndef QKNXLINKLAYERFRAME_H
#define QKNXLINKLAYERFRAME_H

#include <QtKnx/qknxadditionalinfo.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxcemiframe.h>
#include <QtKnx/qknxcontrolfield.h>
#include <QtKnx/qknxextendedcontrolfield.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxtpdu.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxLinkLayerFrame final : public QKnxCemiFrame
{
public:
    QKnxLinkLayerFrame() = default;
    ~QKnxLinkLayerFrame() override = default;

    explicit QKnxLinkLayerFrame(QKnxLinkLayerFrame::MessageCode messageCode);

    bool isValid() const override;

    QKnxControlField controlField() const;
    void setControlField(const QKnxControlField &field);

    QKnxExtendedControlField extendedControlField() const;
    void setExtendedControlField(const QKnxExtendedControlField &field);

    quint8 additionalInfosSize() const;

    void addAdditionalInfo(const QKnxAdditionalInfo &info);
    template <typename T = QVector<QKnxAdditionalInfo>> auto additionalInfos() const -> decltype(T())
    {
        static_assert(is_type<T, QVector<QKnxAdditionalInfo>, std::deque<QKnxAdditionalInfo>,
            std::vector<QKnxAdditionalInfo>>::value, "Type not supported.");

        const auto &store = serviceInformationRef();
        if (store.size() < 1)
            return {};

        quint8 size = store.byte(0);
        if (size < 0x02 || size == 0xff)
            return {};

        T infos;
        quint8 index = 1;
        while (index < size) {
            infos.push_back(QKnxAdditionalInfo::fromBytes(store, index));
            index += store.byte(index + 1) + 2; // type + size => 2
        }
        return infos;
    }

    void removeAdditionalInfo(QKnxAdditionalInfo::Type type);
    void removeAdditionalInfo(const QKnxAdditionalInfo &info);

    void clearAdditionalInfos();

    const QKnxAddress sourceAddress() const;
    void setSourceAddress(const QKnxAddress &address);

    const QKnxAddress destinationAddress() const;
    void setDestinationAddress(const QKnxAddress &address);

    QKnxTpdu tpdu() const;
    void setTpdu(const QKnxTpdu &tpdu);

    QKnxLinkLayerFrame(const QKnxCemiFrame &other);
};

QT_END_NAMESPACE

#endif
