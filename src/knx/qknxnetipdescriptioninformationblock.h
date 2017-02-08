/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpDescriptionInformationBlock
{
public:
    enum class DescriptionTypeCode : quint8 {
        DeviceInfo = 0x01,
        SupportedServiceFamilies = 0x02,
        IpConfiguration = 0x03,
        CurrentIpConfiguration = 0x04,
        KnxAddresses = 0x05,
        ManufactorData = 0xfe,
        NotUsed = 0xff
    };
    QKnxNetIpDescriptionInformationBlock::DescriptionTypeCode descriptionTypeCode() const {
        return m_code;
    };
    void setDescriptionTypeCode(QKnxNetIpDescriptionInformationBlock::DescriptionTypeCode code) {
        m_code = code;
    }

    QKnxNetIpDescriptionInformationBlock() = default;
    QKnxNetIpDescriptionInformationBlock(DescriptionTypeCode code, const QByteArray &data)
        : QKnxNetIpDescriptionInformationBlock(data)
    {
        m_code = code;
    }
    QKnxNetIpDescriptionInformationBlock(DescriptionTypeCode code, const QVector<quint8> &data)
        : QKnxNetIpDescriptionInformationBlock(data)
    {
        m_code = code;
    }

    qint32 dataSize() const { return m_data.size(); }
    template <typename T> auto data() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        T t(m_data.size(), Qt::Uninitialized);
        std::copy(m_data.constBegin(), m_data.constEnd(), t.begin());
        return t;
    }

    qint32 rawSize() const { return 2 + m_data.size(); }
    template <typename T> auto rawData() const -> decltype(T())
    {
        QKnxTypeCheck::FailIfNot<T, QByteArray, QVector<quint8>>();

        T t(2, Qt::Uninitialized);
        t[0] = rawSize(), t[1] = quint8(m_code);
        return t + data<T>();
    }

private:
    template <typename T> QKnxNetIpDescriptionInformationBlock(const T &data)
    {
        int size = data.size();
        if (size % 2)
            size += 1;
        m_data.resize(size);
        m_data[size - 1] = 0x00u;
        std::copy(data.constBegin(), data.constEnd(), m_data.begin());
    }

private:
    QVector<quint8> m_data;
    QKnxNetIpDescriptionInformationBlock::DescriptionTypeCode m_code = DescriptionTypeCode::NotUsed;
};

Q_DECLARE_TYPEINFO(QKnxNetIpDescriptionInformationBlock, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxNetIpDescriptionInformationBlock::DescriptionTypeCode, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
