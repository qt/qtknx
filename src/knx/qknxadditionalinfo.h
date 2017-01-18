/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#pragma once

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxAdditionalInfo final
{
public:
    enum struct Type : quint8 {
        Reserved = 0x00,
        PlMediumInformation = 0x01,
        RfMediumInformation = 0x02,
        BusmonitorStatusInfo = 0x03,
        TimestampRelative = 0x04,
        TimeDelayUntilSending = 0x05,
        ExtendedRelativeTimestamp = 0x06,
        BiBatInformation = 0x07,
        RfMultiInformation = 0x08,
        PreambleAndPostamble = 0x09,
        RfFastAckInformation = 0x0a,
        ManufactorSpecificData = 0xfe,
        EscCode = 0xff
    };
    QKnxAdditionalInfo::Type type() const;

    QKnxAdditionalInfo() = default;
    QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QVector<quint8> &data);

    bool isValid() const;

    qint32 size() const;
    qint32 dataSize() const;

    QString toString() const;
    QVector<quint8> rawData() const;

    static bool isValid(QKnxAdditionalInfo::Type type, const QVector<quint8> &data);
    static qint32 expectedDataSize(QKnxAdditionalInfo::Type type, bool *isFixedSize = nullptr);

#ifndef Q_QDOC
    friend Q_KNX_EXPORT QDataStream &operator>>(QDataStream &, QKnxAdditionalInfo &);
    friend Q_KNX_EXPORT QDataStream &operator<<(QDataStream &, const QKnxAdditionalInfo &);
#endif

private:
    QVector<quint8> m_data;
    QKnxAdditionalInfo::Type m_type = QKnxAdditionalInfo::Type::EscCode;
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxAdditionalInfo &info);
#ifdef Q_QDOC
Q_KNX_EXPORT QDataStream &operator>>(QDataStream &stream, QKnxAdditionalInfo &info);
Q_KNX_EXPORT QDataStream &operator<<(QDataStream &stream, const QKnxAdditionalInfo &info);
#endif

Q_DECLARE_TYPEINFO(QKnxAdditionalInfo, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QKnxAdditionalInfo::Type, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE
