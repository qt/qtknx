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

#ifndef QKNXDATAPOINT_H
#define QKNXDATAPOINT_H

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxbytestore.h>
#include <QtKnx/qknxutils.h>
#include <QtKnx/qknxdpt.h>

QT_BEGIN_NAMESPACE

class QKnxDptFactory;

class Q_KNX_EXPORT QKnxDataPoint final: private QKnxByteStore
{
    friend class QKnxDptFactory;
public:

    QKnxDataPoint() = default;
    ~QKnxDataPoint() override = default;

    bool isValid() const;

    using QKnxByteStore::size;
    using QKnxByteStore::byte;
    using QKnxByteStore::bytes;
    using QKnxByteStore::toString;


    quint8 dataPointTypeIdMainNumber() const;
    quint8 dataPointTypeIdSubNumber() const;
    QString dataPointTypeName() const ;
    QString propertyDataTypeName() const;
    quint8 usage() const;
    // TODO:
    // is Valid function, checking the number of filled bits correspond the the type
    // maybe not here, could do it within the factory class

    // If need be
    // From 3/7/2 1.3.3

protected:
    using QKnxByteStore::resize;
    using QKnxByteStore::setByte;
    using QKnxByteStore::setBytes;
    using QKnxByteStore::appendBytes;
    using QKnxByteStore::insertBytes;

    //QKnxDpt::Info m_info;
    //void setInfo(QKnxDpt::Info info) {m_info = info;}

    void setMainType(QKnxDpt::Types mainType);
    void setMainNumber(quint8 mainNumber);
    void setSubNumber(quint8 subNumber);
    void setSpecifics(quint8 subNumber, const QString dptName, const QString pdtName,
        QKnxDpt::Usage usage);

    struct DptId
    {
        QKnxDpt::Types mainType = QKnxDpt::Types::Unknown;
        quint8 mainNumber = 0;
        quint8 subNumber = 0;
    };

    DptId m_dptId;
    QString m_dataPointTypeName;
    QString m_propertyDataTypeName;
    quint8 m_usage;
};

QT_END_NAMESPACE

#endif // QKNXDATAPOINT_H
