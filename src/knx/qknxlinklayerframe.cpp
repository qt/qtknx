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

#include "qknxlinklayerframe.h"

QT_BEGIN_NAMESPACE

// List of Message code for Tunneling from 3.8.4 paragraph 2.2.1

/*!
    \class QKnxLinkLayerFrame

    \inmodule QtKnx
    \brief The QKnxLinkLayerFrame is a frame meant to be sent via a
    \l QKnxLinkLayerConnection between a client and a KNXnet/IP server.

    Following the KNXnet/IP tunneling specifications, only the
    \l QKnxLinkLayerFrame::MessageCode listed below are valid QKnxLinkLayerFrame
    message code to be sent via KNXnet/IP tunnel connection:

    \list
        \li DataRequest (L_Data.req)
        \li DataConfirmation (L_Data.con)
        \li DataIndication (L_Data.ind)
        \li BusmonitorIndication (L_Busmon.ind)
        \li RawRequest (L_Raw.req)
        \li RawIndication (L_Raw.ind)
        \li RawConfirmation (L_Raw.con)
        \li ResetRequest (M_Reset.req)
    \endlist

    For the moment, the QtKnx module is only implementing KNXnet/IP tunnel
    connection, so only the previously mentioned message code should be used.

    The message code is also to be chosen depending on the application service
    (encoded with the \l QKnxTpdu::ApplicationControlField) hold in the \l QKnxTpdu.
    In the \l QKnxTpduFactory, the application services are split into categories
    depending on the addressing method.

    The most basic functionalities are to be addressed with set of services built
    in \l QKnxTpduFactory::Multicast. For those services one should use
    DataRequest (L_Data.req), DataConfirmation (L_Data.con) or DataIndication
    (L_Data.ind) as QKnxLinkLayerFrame message code.
*/

/*!
    \enum QKnxLinkLayerFrame::MessageCode
    This enum describes the different message codes of the LinkLayer frame.

    \value Unknown
    \value BusmonitorIndication                       L_Busmon.ind
    \value DataRequest                                L_Data.req
    \value DataConfirmation                           L_Data.con
    \value DataIndication                             L_Data.ind
    \value RawRequest                                 L_Raw.req
    \value RawIndication                              L_Raw.ind
    \value RawConfirmation                            L_Raw.con
    \value PollDataRequest                            L_Poll_Data.req
    \value PollDataConfirmation                       L_Poll_Data.con
    \value DataConnectedRequest                       T_Data_Connected.req
    \value DataConnectedIndication                    T_Data_Connected.ind
    \value DataIndividualRequest                      T_Data_Individual.req
    \value DataIndividualIndication                   T_Data_Individual.ind
*/

/*!
    Constructs a LinkLayer frame starting with \a messageCode.

    \note The LinkLayer frame will be other wise empty and needs to be set by hand.
*/
QKnxLinkLayerFrame::QKnxLinkLayerFrame(QKnx::MediumType mediumType,
    QKnxLinkLayerFrame::MessageCode messageCode)
    : m_code(messageCode)
    , m_mediumType(mediumType)
{}

/*!
    Constructs a LinkLayer frame starting with \a messageCode and with a \l QKnxLinkLayerPayload \a payload.
*/
QKnxLinkLayerFrame::QKnxLinkLayerFrame(QKnx::MediumType mediumType,
    QKnxLinkLayerFrame::MessageCode messageCode, const QKnxLinkLayerPayload &payload)
    : m_code(messageCode)
    , m_mediumType(mediumType)
    , m_serviceInformation(payload)
{}

/*!
    Constructs an empty LinkLayer frame. The MediumType is set.
*/
QKnxLinkLayerFrame::QKnxLinkLayerFrame(QKnx::MediumType mediumType)
    : m_mediumType(mediumType)
{}

/*!
  Returns true if the message code is valid.

  \note This function needs to be extended.
*/
bool QKnxLinkLayerFrame::isValid() const
{
    // TODO, adapt so that it's not only valid for the netIptunnel frame
    if (!isMessageCodeValid())
    return false;

    // Tpdu is valid
    if (! tpdu().isValid())
        return false;

    // For the moment we only check for netIp Tunnel
    // TP and PL send L_Data_Standard and L_Data_Extended
    // TODO: Make the check more general (maybe other MediumType could use the following check)
    if (m_mediumType == QKnx::MediumType::NetIP) {
        // TODO: Make sure all constraints from 3.3.2 paragraph 2.2 L_Data is checked here

        //Extended control field destination address type corresponds to the destination address
        if (destinationAddress().type() != extendedControlField().destinationAddressType())
            return false;

        switch (messageCode()) {
        // L_Data
        case MessageCode::DataRequest:
        case MessageCode::DataConfirmation:
        case MessageCode::DataIndication:
            // From 3.3.2 paragraph 2.2.1
            if (sourceAddress().type() != QKnxAddress::Type::Individual)
                return false;
        default:
            break;
        }
        // TODO: check NPDU/ TPDU size, several cases need to be taken into account:
        // 1; Information-Length (max. value is 255); number of TPDU octets, TPCI octet not included!
        if (controlField().frameType() == QKnxControlField::FrameType::Extended
            && tpdu().size() > 256)
            return false;
        // Low Priority is Mandatory for long frame 3.3.2 paragraph 2.2.3
        if (tpdu().size() > 16 && controlField().priority() != QKnxControlField::Priority::Low)
            return false;
        // 2; Check presence of Pl/RF medium information in the additional info -> size always needs
        //    to be greater then 15 bytes because both need additional information.
        //    03_06_03 EMI_IMI v01.03.03 AS.pdf page 76 Table(Use of flags in control field)
        // 3; RF frames do not include a length field at all, it is supposed to be set to 0x00.
        //    03_06_03 EMI_IMI v01.03.03 AS.pdf page 75 NOTE 1
        // 4; 03_03_02 Data Link Layer General v01.02.02 AS.pdf page 12 paragraph 2.2.5
        // control field frame type standard -> max. length value is 15
        if (controlField().frameType() == QKnxControlField::FrameType::Standard
            && tpdu().byte(0) > 15)
            return false;
        //  control field frame type extended -> max. length value is 255
        return true;
    }

    // TODO: implement checks for other medium type
    return false;

}

/*!
    Return \c true if the Message Code of the LinkLayer frame is valid; \c false otherwise.

    \note It only checks that the given code is a correct LinkLayer frame code. It
    does not check the validity of the payload.
*/
bool QKnxLinkLayerFrame::isMessageCodeValid() const
{
    // TODO: extend the medium type check
    // TODO: some message code could be valide for other MediumType as well.
    // Adapt the function when other Medium type get implemented.

    switch (m_code) {
    case MessageCode::BusmonitorIndication:
    case MessageCode::DataRequest:
    case MessageCode::DataConfirmation:
    case MessageCode::DataIndication:
    case MessageCode::RawRequest:
    case MessageCode::RawIndication:
    case MessageCode::RawConfirmation:
    case MessageCode::ResetRequest:
         // For the moment the QKnxLinkLayerFrameFactory is not setting the MediumType
        if (m_mediumType == QKnx::MediumType::Unknown)
            return (guessMediumType(m_code) == QKnx::MediumType::NetIP);
        return (m_mediumType == QKnx::MediumType::NetIP);
    case MessageCode::PollDataRequest:
    case MessageCode::PollDataConfirmation:
    case MessageCode::DataConnectedRequest:
    case MessageCode::DataConnectedIndication:
    case MessageCode::DataIndividualRequest:
    case MessageCode::DataIndividualIndication:
    default:
        break;
    }
    return false;
}

QKnxControlField QKnxLinkLayerFrame::controlField() const
{
    return QKnxControlField { serviceInformationRef(additionalInfosSize() + 1).byte(0) };
}

void QKnxLinkLayerFrame::setControlField(const QKnxControlField &controlField)
{
    auto payload = serviceInformation();
    payload.setByte(additionalInfosSize() + 1, controlField.byte());
    setServiceInformation(payload);
}

QKnxExtendedControlField QKnxLinkLayerFrame::extendedControlField() const
{
    return QKnxExtendedControlField { serviceInformationRef(additionalInfosSize() + 1).byte(1) };
}

void QKnxLinkLayerFrame::setExtendedControlField(const QKnxExtendedControlField &controlFieldEx)
{
    auto payload = serviceInformation();
    payload.setByte(additionalInfosSize() + 2, controlFieldEx.byte());
    setServiceInformation(payload);
}

quint8 QKnxLinkLayerFrame::additionalInfosSize() const
{
    auto size = serviceInformationRef().byte(0);
    return (size < 0xff ? size : 0u); // 0xff is reserved for future use
}

void QKnxLinkLayerFrame::addAdditionalInfo(const QKnxAdditionalInfo &info)
{
    auto payload = serviceInformation();

    quint8 size = payload.byte(0);
    if (size + info.size() > 0xfe)
        return; // maximum size would be exceeded, 0xff is reserved for future use

    quint8 index = 1;
    if (size > 0) {
        while (index < size) {
            if (QKnxAdditionalInfo::Type(payload.byte(index)) >= info.type())
                break;
            index += payload.byte(index + 1) + 2; // type + size => 2
        }
    }

    payload.insertBytes((index > size ? size + 1 : index), info.bytes());
    payload.setByte(0, size + info.size());
    setServiceInformation(payload);
}

void QKnxLinkLayerFrame::removeAdditionalInfo(QKnxAdditionalInfo::Type type)
{
    quint8 oldSize = additionalInfosSize();
    if (oldSize == 0)
        return;

    QKnxLinkLayerPayload payload(oldSize);

    auto infos = additionalInfos();
    for (auto &info : qAsConst(infos)) {
        if (info.type() == type)
            continue;
        payload.appendBytes(info.bytes());
    }

    payload.setByte(0, payload.size() - 1);
    payload.appendBytes(serviceInformationRef(oldSize + 1).bytes(0));

    setServiceInformation(payload);
}

void QKnxLinkLayerFrame::removeAdditionalInfo(const QKnxAdditionalInfo &info)
{
    quint8 oldSize = additionalInfosSize();
    if (oldSize == 0)
        return;

    QKnxLinkLayerPayload payload(oldSize);

    auto infos = additionalInfos();
    for (auto &tmp : qAsConst(infos)) {
        if (tmp.type() == info.type()) {
            if (tmp.bytes() == info.bytes())
                continue;
        }
        payload.appendBytes(tmp.bytes());
    }

    payload.setByte(0, payload.size() - 1);
    payload.appendBytes(serviceInformationRef(oldSize + 1).bytes(0));

    setServiceInformation(payload);
}

void QKnxLinkLayerFrame::clearAdditionalInfos()
{
    quint8 oldSize = additionalInfosSize();
    if (oldSize == 0)
        return;

    QKnxLinkLayerPayload payload(0x00);
    payload.appendBytes(serviceInformationRef(oldSize + 1).bytes(0));
    setServiceInformation(payload);
}

const QKnxAddress QKnxLinkLayerFrame::sourceAddress() const
{
    return { QKnxAddress::Type::Individual, serviceInformationRef(additionalInfosSize() + 1)
        .bytes(2, 2) };
}

void QKnxLinkLayerFrame::setSourceAddress(const QKnxAddress &source)
{
    auto payload = serviceInformation();
    payload.replaceBytes(additionalInfosSize() + 3, source.bytes());
    setServiceInformation(payload);
}

const QKnxAddress QKnxLinkLayerFrame::destinationAddress() const
{
    return { extendedControlField().destinationAddressType(),
        serviceInformationRef(additionalInfosSize() + 1).bytes(4, 2) };
}

void QKnxLinkLayerFrame::setDestinationAddress(const QKnxAddress &destination)
{
    auto payload = serviceInformation();
    payload.replaceBytes(additionalInfosSize() + 5, destination.bytes());
    setServiceInformation(payload);
}

QKnxTpdu QKnxLinkLayerFrame::tpdu() const
{
    // TODO: In RF-Frames the length field is set to 0x00, figure out how this fits in here.
    // See 03_06_03 EMI_IMI, paragraph 4.1.5.3.1 Implementation and usage, page 75, Note 1,2,3

    // length field + ctrl + extCtrl + 2 * KNX address -> 7 bytes
    const quint8 tpduOffset = additionalInfosSize() + 7 + 1/* bytes */;
    return QKnxTpdu::fromBytes(serviceInformationRef().bytes(0),
        tpduOffset, (size() - 1) - tpduOffset);
}

void QKnxLinkLayerFrame::setTpdu(const QKnxTpdu &tpdu)
{
    auto info = serviceInformation();
    info.resize(additionalInfosSize() + 7); // length field + ctrl + extCtrl + 2 * KNX address
    info.setByte(additionalInfosSize() + 7, tpdu.dataSize());
    info.appendBytes(tpdu.bytes());
    setServiceInformation(info);
}

QKnxLinkLayerFrame::QKnxLinkLayerFrame(const QKnxLinkLayerFrame &other)
{
    m_code = other.messageCode();
    m_serviceInformation = other.serviceInformation();

}
/*!
    Returns the number of bytes of the LinkLayer frame.
*/
quint16 QKnxLinkLayerFrame::size() const
{
    return m_serviceInformation.size() + 1 /* message code */;
}

/*!
      Returns the \l QKnxLinkLayerPayload.
      This is the CEMI frame without the message code.
*/
QKnxLinkLayerPayload QKnxLinkLayerFrame::serviceInformation() const
{
    return m_serviceInformation;
}

/*!
    Returns a \l QKnxLinkLayerPayloadRef at the given \a index of the LinkLayer frame
    payload.
*/
QKnxLinkLayerPayloadRef QKnxLinkLayerFrame::serviceInformationRef(quint16 index) const
{
    return m_serviceInformation.ref(index);
}


/*!
    Returns a \l QString representing the bytes of the LinkLayer frame
*/
QString QKnxLinkLayerFrame::toString() const
{
    QString tmp;
    for (quint8 byte : m_serviceInformation.ref())
        tmp += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
    tmp.chop(2);

    return QStringLiteral("Message code: { 0x%1 }, Service information: { 0x%2 }")
        .arg(quint8(m_code), 2, 16, QLatin1Char('0')).arg(tmp);
}

/*!
    Sets the \l QKnxLinkLayerPayload \a serviceInformation of the LinkLayer frame.
*/
void QKnxLinkLayerFrame::setServiceInformation(const QKnxLinkLayerPayload &serviceInformation)
{
    m_serviceInformation = serviceInformation;
}

/*!
    Returns the message code of the LinkLayer frame.
*/
QKnxLinkLayerFrame::MessageCode QKnxLinkLayerFrame::messageCode() const
{
    return m_code;
}

/*!
    Sets the message code of the LinkLayer frame with \a code.
*/
void QKnxLinkLayerFrame::setMessageCode(QKnxLinkLayerFrame::MessageCode code)
{
    m_code = code;
}

/*!
    Returns the medium type to be used to send the LinkLayer frame.
*/
QKnx::MediumType QKnxLinkLayerFrame::mediumType() const
{
    return m_mediumType;
}

/*!
    Sets the medium type to be used to send the LinkLayer frame with \a type.
*/
void QKnxLinkLayerFrame::setMediumType(QKnx::MediumType type)
{
    m_mediumType = type;
}

QT_END_NAMESPACE
