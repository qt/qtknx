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
    quint16 index = 54;
    const auto &ref = payloadRef();
    auto header = QKnxNetIpStructHeader::fromBytes(ref, index);

    QVector<QKnxNetIpStructRef> dibs;
    while (index < ref.size()) {
        header = QKnxNetIpStructHeader::fromBytes(ref, index += header.totalSize());
        // Ugly hack to get a pointer to the real byte store data.
        dibs.push_back(QKnxNetIpStructRef(&(*std::next(ref.bytes(), index)),
            QKnxPrivate::codeToType(header.code())));
    }
    return dibs;
}

bool QKnxNetIpDescriptionResponse::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() >= 64;
}

QT_END_NAMESPACE
