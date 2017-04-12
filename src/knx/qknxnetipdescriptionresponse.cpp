/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdescriptionresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDescriptionResponse::QKnxNetIpDescriptionResponse(const QKnxNetIpDeviceDIB &deviceHardware,
        const QKnxNetIpServiceFamiliesDIB &supportedFamilies)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::DescriptionResponse))
{
    QKnxNetIpPayload payload;
    payload.setBytes(deviceHardware.bytes());
    payload.appendBytes(supportedFamilies.bytes());
    setPayload(payload);
}

QKnxNetIpDeviceDIB QKnxNetIpDescriptionResponse::deviceHardware() const
{
    return QKnxNetIpDeviceDIB::fromBytes(payloadRef(), 0);
}

QKnxNetIpServiceFamiliesDIB QKnxNetIpDescriptionResponse::supportedFamilies() const
{
    return QKnxNetIpServiceFamiliesDIB::fromBytes(payloadRef(), 54);
}

namespace QKnxPrivate
{
    static QKnxNetIpStructRef::Type codeToType(quint8 code)
    {
        switch (QKnxNetIp::DescriptionTypeCode(code)) {
        case QKnxNetIp::DescriptionTypeCode::DeviceInfo:
            return QKnxNetIpStructRef::Type::QKnxNetIpDeviceDIB;
        case QKnxNetIp::DescriptionTypeCode::SupportedServiceFamilies:
            return QKnxNetIpStructRef::Type::QKnxNetIpServiceFamiliesDIB;
        case QKnxNetIp::DescriptionTypeCode::IpConfiguration:
            return QKnxNetIpStructRef::Type::QKnxNetIpConfigDIB;
        case QKnxNetIp::DescriptionTypeCode::CurrentIpConfiguration:
            return QKnxNetIpStructRef::Type::QKnxNetIpCurrentConfigDIB;
        case QKnxNetIp::DescriptionTypeCode::KnxAddresses:
            return QKnxNetIpStructRef::Type::QKnxNetIpKnxAddressesDIB;
        case QKnxNetIp::DescriptionTypeCode::ManufactorData:
            return QKnxNetIpStructRef::Type::QKnxNetIpManufacturerDIB;
        default:
            break;
        }
        return QKnxNetIpStructRef::Type::Null;
    };
}

QVector<QKnxNetIpStructRef> QKnxNetIpDescriptionResponse::optionalDibs() const
{
    const auto &ref = payloadRef();
    auto header = QKnxNetIpStructHeader::fromBytes(ref, 0);
    quint16 index = header.totalSize(); // total size of device DIB

    header = QKnxNetIpStructHeader::fromBytes(ref, index);
    index += header.totalSize(); // advance of total size of families DIB

    QVector<QKnxNetIpStructRef> dibs;
    while (index < ref.size()) {
        header = QKnxNetIpStructHeader::fromBytes(ref, index);
        dibs.push_back(QKnxNetIpStructRef(payloadRef(index), QKnxPrivate::codeToType(header.code())));
        index += header.totalSize(); // advance of total size of last read DIB
    }
    return dibs;
}

bool QKnxNetIpDescriptionResponse::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() >= 64;
}

QT_END_NAMESPACE
