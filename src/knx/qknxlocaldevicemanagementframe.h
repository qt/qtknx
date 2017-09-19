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

#ifndef QKNXLOCALDEVICEMANAGEMENTFRAME_H
#define QKNXLOCALDEVICEMANAGEMENTFRAME_H

#include <QtKnx/qknxcemi.h>
#include <QtKnx/qknxcemiframe.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxinterfaceobjectproperty.h>
#include <QtKnx/qknxinterfaceobjecttype.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxLocalDeviceManagementFrame final : public QKnxCemiFrame
{
    friend class QKnxLocalDeviceManagementFrameFactory;

public:
    QKnxLocalDeviceManagementFrame() = default;
    ~QKnxLocalDeviceManagementFrame() override = default;

    explicit QKnxLocalDeviceManagementFrame(QKnxCemiFrame::MessageCode code);

    bool isValid() const override;
    bool isNegativeConfirmation() const;

    QKnxInterfaceObjectType objectType() const;
    void setObjectType(QKnxInterfaceObjectType type);

    quint8 objectInstance() const;
    void setObjectInstance(quint8 instance);

    QKnxInterfaceObjectProperty property() const;
    void setProperty(QKnxInterfaceObjectProperty pid);

    quint8 numberOfElements() const;
    void setNumberOfElements(quint8 count);

    quint16 startIndex() const;
    void setStartIndex(quint16 index);

    QKnxCemi::Server::Error error() const;
    void setError(QKnxCemi::Server::Error error);

    QKnxCemi::Server::ReturnCode returnCode() const;
    void setReturnCode(QKnxCemi::Server::ReturnCode code);

    template <typename T = QByteArray> auto data() const -> decltype(T())
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        auto ref = serviceInformationRef(6);

        T t(ref.size(), 0);
        std::copy(std::begin(ref), std::end(ref), std::begin(t));
        return t;
    }

    template <typename T, std::size_t S = 0> void setData(const T &newData)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, QKnxByteStoreRef, std::deque<quint8>,
            std::vector<quint8>, std::array<quint8, S>>::value, "Type not supported.");

        auto sf = serviceInformation();
        sf.resize(6);
        sf.appendBytes(newData);
        setServiceInformation(sf);
    }

    QKnxLocalDeviceManagementFrame(const QKnxCemiFrame &other);

private:
    template <typename T = QByteArray> QKnxLocalDeviceManagementFrame(QKnxCemiFrame::MessageCode code,
            QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
            quint8 noe, quint16 index, const T &payload = {})
        : QKnxLocalDeviceManagementFrame(code)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, QKnxByteStoreRef, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        auto si = serviceInformation();
        si.setBytes(QKnxUtils::QUint16::bytes(quint16(type)));
        si.setByte(2, instance);
        si.setByte(3, pid);
        si.replaceBytes(4, QKnxUtils::QUint16::bytes((quint16(noe) << 12) | index));
        si.appendBytes(payload);
        setServiceInformation(si);
    }

    template <typename T = QByteArray> QKnxLocalDeviceManagementFrame(QKnxCemiFrame::MessageCode code,
            QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
            const T &payload = {})
        : QKnxLocalDeviceManagementFrame(code)
    {
        static_assert(is_type<T, QByteArray, QVector<quint8>, QKnxByteStoreRef, std::deque<quint8>,
            std::vector<quint8>>::value, "Type not supported.");

        auto si = serviceInformation();
        si.setBytes(QKnxUtils::QUint16::bytes(quint16(type)));
        si.setByte(2, instance);
        si.setByte(3, pid);
        si.appendBytes(payload);
        setServiceInformation(si);
    }
};

QT_END_NAMESPACE

#endif
