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

#ifndef QKNXDATAPOINTTYPE_H
#define QKNXDATAPOINTTYPE_H

#include <QtCore/qcoreapplication.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtCore/qshareddata.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

struct QKnxDatapointTypePrivate;

class Q_KNX_EXPORT QKnxDatapointType
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(QKnxDatapointType)

public:
    enum class Type : quint32
    {
        Unknown = 0,
        Dpt1_1Bit = 100000,
            DptSwitch, DptBool, DptEnable, DptRamp, DptAlarm, DptBinaryValue, DptStep, DptUpDown,
            DptOpenClose, DptStart, DptState, DptInvert, DptDimSendStyle, DptInputSource, DptReset,
            DptAck, DptTrigger, DptOccupancy, DptWindowDoor, DptLogicalFunction = 100021, DptSceneAB,
            DptShutterBlindsMode, DptHeatCool = 100100,
        Dpt2_1BitControlled = 200000,
            DptSwitchControl, DptBoolControl, DptEnableControl, DptRampControl, DptAlarmControl,
            DptBinaryValueControl, DptStepControl, DptDirection1Control, DptDirection2Control,
            DptStartControl, DptStateControl, DptInvertControl,
        Dpt3_3BitControlled = 300000,
            DptControlDimming = 300007, DptControlBlinds = 300008,
        Dpt4_Character = 400000,
            DptCharAscii, DptChar8859,
        Dpt5_8bitUnsigned = 500000,
            DptScaling, DptAngle = 500003, DptPercentU8, DptDecimalFactor, DptValue1Ucount = 500010,
        Dpt6_8bitSigned = 600000,
            DptPercentV8, DptValue1Count = 600010, DptStatusMode3 = 600020,
        Dpt7_2ByteUnsigned = 700000,
            DptPropertyDataType = 700010,
        Dpt8_2ByteSigned = 800000,
        Dpt9_2ByteFloat = 900000,
        Dpt10_TimeOfDay = 1000000,
            DptTimeOfDay = 1000001,
        Dpt11_Date = 1100000,
            DptDate = 1100001,
        Dpt12_4ByteUnsigned = 1200000,
            DptValue4Ucount,
        Dpt13_4ByteSigned = 1300000,
        Dpt14_4ByteFloat = 1400000,
        Dpt15_EntranceAccess = 1500000,
        Dpt16_CharacterString = 1600000,
        Dpt17_SceneNumber = 1700000,
        Dpt18_SceneControl = 1800000,
        Dpt19_DataTime = 1900000,
            DptDateTime = 1900001,
        Dpt20_1Byte = 2000000,
        Dpt21_8BitSet = 2100000,
            DptGeneralStatus = 2100010,
        Dpt22_16BitSet = 2200000,
        Dpt23_2BitSet = 2300000,
        Dpt24_String = 2400000,
            DptVariableString8859 = 2400001,
        Dpt25_2NibbleSet = 2500000,
        Dpt26_8BitSet = 2600000,
        Dpt27_32BitSet = 2700000,
        Dpt28_StringUtf8 = 2800000,
            DptUtf8 = 2800001,
        Dpt29_ElectricalEnergy = 2900000,
        Dpt30_24TimesChannelActivation = 3000000,
        DPT217_DatapointTypeVersion = 21700000,
            DptVersion = 21700001,
        Dpt225_ScalingSpeed = 22500000,
        Dpt232_3ByteColourRGB = 23200000,
            DptColourRGB = 23200600
    };
    Q_ENUMS(Type)
    Type type() const;

    virtual ~QKnxDatapointType();

    QKnxDatapointType(Type type, int size);
    QKnxDatapointType(const QString &dptId, int size);
    QKnxDatapointType(quint16 mainType, quint16 subType, int size);

    int subType() const;
    int mainType() const;

    int size() const;

    QVariant minimum() const;
    void setMinimum(const QVariant &minimum);

    QVariant maximum() const;
    void setMaximum(const QVariant &maximum);

    QString minimumText() const;
    void setMinimumText(const QString &minimumText);

    QString maximumText() const;
    void setMaximumText(const QString &maximumText);

    void setRange(const QVariant &minimum, const QVariant &maximum);
    void setRangeText(const QString &minimumText, const QString &maximumText);

    QString unit() const;
    void setUnit(const QString &unit);

    QString description() const;
    void setDescription(const QString &description);

    virtual bool isValid() const;
    virtual QString toString() const;

    quint8 &operator[](int i);
    const quint8 &operator[](int i) const;

    quint8 *data();
    const quint8 *data() const;
    const quint8 *constData() const;

    quint8 byte(quint16 index) const;
    template <typename T = QByteArray> auto bytes() const -> decltype(T())
    {
        T t(size(), 0x00);
        std::copy_n(constData(), size(), std::begin(t));
        return t;
    }

    bool setByte(quint16 index, quint8 bytes);
    template <typename T> void setBytes(const T &bytesToSet, quint16 index, quint16 count)
    {
        if (((bytesToSet.size() - index) < count) || (size() != count))
            return;
        std::copy(std::begin(bytesToSet), std::end(bytesToSet), data());
    }

    QKnxDatapointType(const QKnxDatapointType &other);
    QKnxDatapointType &operator=(const QKnxDatapointType &other);

#ifdef Q_COMPILER_RVALUE_REFS
    QKnxDatapointType &operator=(QKnxDatapointType &&other) Q_DECL_NOTHROW;
#endif

    void swap(QKnxDatapointType &other) Q_DECL_NOTHROW;
    bool operator==(const QKnxDatapointType &other) const;
    bool operator!=(const QKnxDatapointType &other) const;

    static inline bool testBit(quint8 byteToTest, quint8 bit)
    {
        return (byteToTest & (quint8(1) << bit)) != 0;
    }

    static inline void setBit(quint8 *byteToSet, bool value, quint8 bit)
    {
        value ? (*byteToSet |= quint8(1) << bit) : (*byteToSet &= ~(quint8(1) << bit));
    }

    static const constexpr int SubType = 0x00;

private:
    QKnxDatapointType() = delete;
    explicit QKnxDatapointType(QKnxDatapointTypePrivate &dd);

private:
    QSharedDataPointer<QKnxDatapointTypePrivate> d_ptr;
};

QT_END_NAMESPACE

#endif
