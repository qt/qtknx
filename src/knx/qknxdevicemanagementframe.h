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

#ifndef QKNXDEVICEMANAGEMENTFRAME_H
#define QKNXDEVICEMANAGEMENTFRAME_H

#include <QtKnx/qknxcemiframe.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxinterfaceobject.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxDeviceManagementFrame final : public QKnxCemiFrame
{
public:
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

        Data = 0xfe, // Qt extension
        None = 0xff  // Qt extension
    };
    Error errorCode() const;

    QKnxDeviceManagementFrame() = default;
    ~QKnxDeviceManagementFrame() override = default;

    explicit QKnxDeviceManagementFrame(QKnxDeviceManagementFrame::MessageCode code);

    bool isValid() const override;
    bool isNegativeConfirmation() const;

    QKnxInterfaceObject::Type objectType() const;
    void setObjectType(QKnxInterfaceObject::Type type);

    quint8 objectInstance() const;
    void setObjectInstance(quint8 instance);

    QKnxInterfaceObject::Property property() const;
    void setProperty(QKnxInterfaceObject::Property pid);

    quint8 numberOfElements() const;
    void setNumberOfElements(quint8 count);

    quint16 startIndex() const;
    void setStartIndex(quint16 index);

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

    QKnxDeviceManagementFrame(const QKnxCemiFrame &other);

private:
    template <typename T = QByteArray> QKnxDeviceManagementFrame(QKnxCemiFrame::MessageCode code,
            QKnxInterfaceObject::Type type, quint8 instance, QKnxInterfaceObject::Property pid,
            quint8 noe, quint16 index, const T &payload = {})
        : QKnxDeviceManagementFrame(code)
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
};

QT_END_NAMESPACE

#endif
