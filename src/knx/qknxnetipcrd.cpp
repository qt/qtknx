/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipcrd.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCRD::QKnxNetIpCRD(ConnectionTypeCode type,const QByteArray &data)
    : QKnxNetIpStructure(quint8(type), data)
{}

QKnxNetIpCRD::QKnxNetIpCRD(ConnectionTypeCode type,const QVector<quint8> &data)
    : QKnxNetIpStructure(quint8(type), data)
{}

QKnxNetIpCRD QKnxNetIpCRD::fromRawData(const QByteArray &rawData, qint32 offset)
{
    return QKnxNetIpStructure::fromRawData(rawData, offset);
}

QKnxNetIpCRD QKnxNetIpCRD::fromRawData(const QVector<quint8> &rawData, qint32 offset)
{
    return QKnxNetIpStructure::fromRawData(rawData, offset);
}

bool QKnxNetIpCRD::isValid() const
{
    switch (connectionTypeCode()) {
        case ConnectionTypeCode::DeviceManagementConnection:
        case ConnectionTypeCode::TunnelConnection :
        case ConnectionTypeCode::RemoteLoggingConnection :
        case ConnectionTypeCode::RemoteConfigurationConnection :
        case ConnectionTypeCode::ObjectServerConnection :
            break;
        default:
            return false;
    }
    return QKnxNetIpStructure::isValid();
}

QT_END_NAMESPACE
