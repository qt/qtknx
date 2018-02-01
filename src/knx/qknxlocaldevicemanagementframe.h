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

#ifndef QKNXLOCALDEVICEMANAGEMENTFRAME_H
#define QKNXLOCALDEVICEMANAGEMENTFRAME_H

#include <QtKnx/qknxcemiframe.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxinterfaceobjectproperty.h>
#include <QtKnx/qknxinterfaceobjecttype.h>
#include <QtKnx/qknxnamespace.h>

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

    QKnx::CemiServer::Error error() const;
    void setError(QKnx::CemiServer::Error error);

    QKnx::CemiServer::ReturnCode returnCode() const;
    void setReturnCode(QKnx::CemiServer::ReturnCode code);

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
