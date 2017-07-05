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

#ifndef QKNXTUNNELFRAME_H
#define QKNXTUNNELFRAME_H

#include <QtKnx/qknxadditionalinfo.h>
#include <QtKnx/qknxaddress.h>
#include <QtKnx/qknxcemiframe.h>
#include <QtKnx/qknxcontrolfield.h>
#include <QtKnx/qknxextendedcontrolfield.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

struct QKnxNpdu
{
    QByteArray bytes() const { return {}; }
    template <typename T> static QKnxNpdu fromBytes(const T &, quint16) { return {}; }
};

class Q_KNX_EXPORT QKnxTunnelFrame final : public QKnxCemiFrame
{
public:
    QKnxTunnelFrame() = default;
    ~QKnxTunnelFrame() override = default;

    explicit QKnxTunnelFrame(QKnxTunnelFrame::MessageCode messageCode);

    QKnxTunnelFrame (const QKnxCemiFrame &other)
        : QKnxCemiFrame(other)
    {}

    bool isValid() const override;

    QKnxControlField controlField() const;
    void setControlField(const QKnxControlField &field);

    QKnxExtendedControlField extendedControlField() const;
    void setExtendedControlField(const QKnxExtendedControlField &field);

    quint8 additionalInfosSize() const;
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

    void addAdditionalInfo(const QKnxAdditionalInfo &info);
    void removeAdditionalInfo(QKnxAdditionalInfo::Type type);

    const QKnxAddress sourceAddress() const;
    void setSourceAddress(const QKnxAddress &address);

    const QKnxAddress destionationAddress() const;
    void setDestionationAddress(const QKnxAddress &address);

    QKnxNpdu networkLayerProtocolDataUnit() const;
    void setNetworkLayerProtocolDataUnit(const QKnxNpdu &npdu);
};

QT_END_NAMESPACE

#endif
