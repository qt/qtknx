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
#include "qknxlinklayerframebuilder.h"

QT_BEGIN_NAMESPACE

// List of Message code for Tunneling from 3.8.4 paragraph 2.2.1

/*!
    \class QKnxLinkLayerFrame

    \inmodule QtKnx
    \ingroup qtknx-tunneling

    \brief The QKnxLinkLayerFrame class is a frame that is sent over a
    communication channel established between a KNXnet/IP client and a
    KNXnet/IP server.

    Following the KNXnet/IP tunneling specifications, only the \l MessageCode
    values listed below are valid link layer frame message codes to be sent via
    a KNXnet/IP tunnel connection:

    \list
        \li \c DataRequest (\c {L_Data.req})
        \li \c DataConfirmation (\c {L_Data.con})
        \li \c DataIndication (\c {L_Data.ind})
        \li \c BusmonitorIndication (\c {L_Busmon.ind})
        \li \c RawRequest (\c {L_Raw.req})
        \li \c RawIndication (\c {L_Raw.ind})
        \li \c RawConfirmation (\c {L_Raw.con})
        \li \c ResetRequest (\c {M_Reset.req})
    \endlist

    The Qt KNX module implements only KNXnet/IP tunnel connections, and
    therefore only the above message codes can be used.

    The message code is also to be chosen depending on the application service
    (encoded with the \l QKnxTpdu::ApplicationControlField) held in \l QKnxTpdu.
    The application services are split into categories according to the
    addressing method.

    \sa {Qt KNX Tunneling Classes}
*/

/*!
    \enum QKnxLinkLayerFrame::MessageCode
    This enum holds the message code of the link layer frame sent via a
    KNXnet/IP tunnel connection.

    \value Unknown
           An unknown message code.
    \value BusmonitorIndication
           A \c {L_Busmon.ind} message code that is used in server-to-client
           communication on KNX bus monitor.
    \value DataRequest
           A \c {L_Data.req} message code that is used in client-to-server
           communication on KNX data link layer.
    \value DataConfirmation
           A \c l{L_Data.con} message code that is used in server-to-client
           communication on KNX data link layer.
    \value DataIndication
           A \c {L_Data.ind} message code that is used in server-to-client
           communication on KNX data link layer.
    \value RawRequest
           A \c {L_Raw.req} message code that is used in client-to-server
           communication in common external message interface (cEMI) raw mode.
    \value RawIndication
           A \c {L_Raw.ind} message code that is used in server-to-client
           communication in cEMI raw mode.
    \value RawConfirmation
           A \c {L_Raw.con} message code that is used in server-to-client
           communication in cEMI raw mode.
    \value ResetRequest
           A \c {M_Reset.req} message code that is used in client-to-server
           communication on KNX data link layer and in cEMI raw mode.

    \omitvalue PollDataRequest
    \omitvalue PollDataConfirmation
    \omitvalue DataConnectedRequest
    \omitvalue DataConnectedIndication
    \omitvalue DataIndividualRequest
    \omitvalue DataIndividualIndication
*/

class QKnxLinkLayerFramePrivate : public QSharedData
{
public:
    QKnxLinkLayerFramePrivate() = default;
    ~QKnxLinkLayerFramePrivate() = default;

    QKnxLinkLayerFrame::MessageCode m_code { QKnxLinkLayerFrame::MessageCode::Unknown };
    QKnx::MediumType m_mediumType { QKnx::MediumType::NetIP };
    QKnxAddress m_srcAddress;
    QKnxAddress m_dstAddress;
    QKnxTpdu m_tpdu;
    QKnxControlField m_ctrl;
    QKnxExtendedControlField m_extCtrl;
    quint8 m_additionalInfoSize { 0 };
    mutable bool m_additionalInfosSorted { true };
    mutable QVector<QKnxAdditionalInfo> m_additionalInfos;
};

/*!
    Constructs an empty link layer frame with the medium type set to
    \l QKnx::MediumType \c NetIP.

    \sa setMediumType()
*/
QKnxLinkLayerFrame::QKnxLinkLayerFrame()
    : d_ptr(new QKnxLinkLayerFramePrivate)
{}

/*!
    Destroys the link layer management frame and frees all allocated resources.
*/
QKnxLinkLayerFrame::~QKnxLinkLayerFrame()
{}

/*!
    Constructs a link layer frame starting with \a messageCode. The medium type
    is set to \l QKnx::MediumType \c NetIP.

    \note The link layer frame will be otherwise empty and needs to be set
    manually.

    \sa setMediumType()
*/
QKnxLinkLayerFrame::QKnxLinkLayerFrame(QKnxLinkLayerFrame::MessageCode messageCode)
    : d_ptr(new QKnxLinkLayerFramePrivate)
{
    d_ptr->m_code = messageCode;
}

/*!
    Constructs a link layer frame starting with \a messageCode and sets the
    service information to \a serviceInfo.

    The medium type is set to \l QKnx::MediumType \c NetIP.

    \sa setMediumType()
*/
QKnxLinkLayerFrame::QKnxLinkLayerFrame(QKnxLinkLayerFrame::MessageCode messageCode,
        const QKnxByteArray &serviceInfo)
    : QKnxLinkLayerFrame(messageCode)
{
    setServiceInformation(serviceInfo);
}

/*!
    Returns \c true if the link layer frame is valid, returns \c false otherwise.

    \note Validation is only supported for \l QKnx::MediumType \c NetIP, for
    other medium types the function always returns \c false.

*/
bool QKnxLinkLayerFrame::isValid() const
{
    // TODO: This function needs to be extended with support for other medium types.

    if (!isMessageCodeValid())
        return false;

    if (!d_ptr->m_tpdu.isValid())
        return false;

    // For the moment we only check for netIp Tunnel
    // TP and PL send L_Data_Standard and L_Data_Extended
    // TODO: Make the check more general (maybe other MediumType could use the following check)
    if (d_ptr->m_mediumType == QKnx::MediumType::NetIP) {
        // TODO: Make sure all constraints from 3.3.2 paragraph 2.2 L_Data is checked here

        //Extended control field destination address type corresponds to the destination address
        if (d_ptr->m_dstAddress.type() != d_ptr->m_extCtrl.destinationAddressType())
            return false;

        switch (d_ptr->m_code) {
            // L_Data
        case MessageCode::DataRequest:
        case MessageCode::DataConfirmation:
        case MessageCode::DataIndication:
            // From 3.3.2 paragraph 2.2.1
            if (d_ptr->m_srcAddress.type() != QKnxAddress::Type::Individual)
                return false;
        default:
            break;
        }

        if (d_ptr->m_tpdu.dataSize() > 15) {
            if (d_ptr->m_tpdu.dataSize() > 255)
                return false;
            // Low Priority is mandatory for long frame 3.3.2 paragraph 2.2.3
            if (d_ptr->m_ctrl.priority() != QKnxControlField::Priority::Low)
                return false;
            if (d_ptr->m_ctrl.frameFormat() != QKnxControlField::FrameFormat::Extended)
                return false;
        } else {
            if (d_ptr->m_ctrl.frameFormat() != QKnxControlField::FrameFormat::Standard)
                return false;
        }

        // TODO: check NPDU/ TPDU size, several cases need to be taken into account:
        // 1; Check presence of Pl/RF medium information in the additional info -> size always needs
        //    to be greater then 15 bytes because both need additional information.
        //    03_06_03 EMI_IMI v01.03.03 AS.pdf page 76 Table(Use of flags in control field)
        // 2; RF frames do not include a length field at all, it is supposed to be set to 0x00.
        //    03_06_03 EMI_IMI v01.03.03 AS.pdf page 75 NOTE 1
        // 3; 03_03_02 Data Link Layer General v01.02.02 AS.pdf page 12 paragraph 2.2.5
        return true;
    }

    // TODO: implement checks for other medium type
    return false;
}

/*!
    \internal

    Returns \c true if the \l MessageCode of the link layer frame is valid;
    \c false otherwise.

    \note This function checks only that the specified message code is a valid
    link layer frame code. It does not check the validity of the payload.
*/
bool QKnxLinkLayerFrame::isMessageCodeValid() const
{
    // TODO: extend the medium type check
    // TODO: some message code could be valid for other MediumType as well.
    // TODO: Adapt the function when other Medium type get implemented.

    switch (d_ptr->m_code) {
    case MessageCode::BusmonitorIndication:
    case MessageCode::DataRequest:
    case MessageCode::DataConfirmation:
    case MessageCode::DataIndication:
    case MessageCode::RawRequest:
    case MessageCode::RawIndication:
    case MessageCode::RawConfirmation:
    case MessageCode::ResetRequest:
        return (d_ptr->m_mediumType == QKnx::MediumType::NetIP);
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

/*!
    Returns the control field of the frame.
*/
QKnxControlField QKnxLinkLayerFrame::controlField() const
{
    return d_ptr->m_ctrl;
}

/*!
    Sets the control field of the frame to \a controlField.
*/
void QKnxLinkLayerFrame::setControlField(const QKnxControlField &controlField)
{
    d_ptr->m_ctrl = controlField;
}

/*!
    Returns the extended control field of the frame.
*/
QKnxExtendedControlField QKnxLinkLayerFrame::extendedControlField() const
{
    return d_ptr->m_extCtrl;
}

/*!
    Sets the extended control field of the frame to \a controlFieldEx.
*/
void QKnxLinkLayerFrame::setExtendedControlField(const QKnxExtendedControlField &controlFieldEx)
{
    d_ptr->m_extCtrl = controlFieldEx;
}

/*!
    Returns the source address.
*/
const QKnxAddress QKnxLinkLayerFrame::sourceAddress() const
{
    return d_ptr->m_srcAddress;
}

/*!
    Sets the source address to \a source.
*/
void QKnxLinkLayerFrame::setSourceAddress(const QKnxAddress &source)
{
    d_ptr->m_srcAddress = source;
}

/*!
    Returns the destination address.
*/
const QKnxAddress QKnxLinkLayerFrame::destinationAddress() const
{
    return d_ptr->m_dstAddress;
}


/*!
    Sets the destination address to \a destination.
*/
void QKnxLinkLayerFrame::setDestinationAddress(const QKnxAddress &destination)
{
    d_ptr->m_dstAddress = destination;
}

/*!
    Returns the TPDU of the frame. The TPDU is read by the network, transport,
    and application layer services.
*/
QKnxTpdu QKnxLinkLayerFrame::tpdu() const
{
    // TODO: In RF-Frames the length field is set to 0x00, figure out how this fits in here.
    // See 03_06_03 EMI_IMI, paragraph 4.1.5.3.1 Implementation and usage, page 75, Note 1,2,3
    // length field + ctrl + extCtrl + 2 * KNX address -> 7 bytes
    //    const quint8 tpduOffset = additionalInfosSize() + 7 + 1/* bytes */;
    //    return QKnxTpdu::fromBytes(m_serviceInformation, tpduOffset, (size() - 1) - tpduOffset);
    return d_ptr->m_tpdu;
}


/*!
    Sets the TPDU of the frame to \a tpdu. The TPDU is read by the network,
    transport, and application layer services.
*/
void QKnxLinkLayerFrame::setTpdu(const QKnxTpdu &tpdu)
{
    d_ptr->m_tpdu = tpdu;
}

/*!
    Returns the message code of the link layer frame.
*/
QKnxLinkLayerFrame::MessageCode QKnxLinkLayerFrame::messageCode() const
{
    return d_ptr->m_code;
}

/*!
    Sets the message code of the link layer frame to \a code.
*/
void QKnxLinkLayerFrame::setMessageCode(QKnxLinkLayerFrame::MessageCode code)
{
    d_ptr->m_code = code;
}

/*!
    Returns the medium type to be used to send the link layer frame.
*/
QKnx::MediumType QKnxLinkLayerFrame::mediumType() const
{
    return d_ptr->m_mediumType;
}

/*!
    Sets the medium type to be used to send the LinkLayer frame to \a type.
*/
void QKnxLinkLayerFrame::setMediumType(QKnx::MediumType type)
{
    d_ptr->m_mediumType = type;
}

/*!
    Returns the number of bytes of the link layer frame.
*/
quint16 QKnxLinkLayerFrame::size() const
{
    return bytes().size();
}

/*!
    Returns the service information the frame carries. This is the frame
    without the message code.
*/
QKnxByteArray QKnxLinkLayerFrame::serviceInformation() const
{
    return bytes().mid(1);
}

/*!
    Sets the service information based on a byte array with all the fields
    encoded into it to \a data.
*/
void QKnxLinkLayerFrame::setServiceInformation(const QKnxByteArray &data)
{
    if (data.size() < 1)
        return;

    int index = 1;
    quint8 addinfoLen = data.at(0);
    while (index < addinfoLen) {
        auto info = QKnxAdditionalInfo::fromBytes(data, index);
        addAdditionalInfo(info);
        index = index + info.size() + 1;
    }
    d_ptr->m_ctrl = QKnxControlField(data.at(index));
    ++index;
    d_ptr->m_extCtrl = QKnxExtendedControlField(data.at(index));
    ++index;
    d_ptr->m_srcAddress = { QKnxAddress::Type::Individual, data.mid(index, 2) };
    index += 2;
    d_ptr->m_dstAddress = { d_ptr->m_extCtrl.destinationAddressType(), data.mid(index, 2) };
    index += 2;
    // length doesn't include TPCI therefore add +1
    d_ptr->m_tpdu = QKnxTpdu::fromBytes(data, index + 1, data.at(index) + 1, d_ptr->m_mediumType);
}

/*!
    Returns an array of bytes that represent the link layer frame if it is
    valid; otherwise returns a \e {default-constructed} frame.
*/
QKnxByteArray QKnxLinkLayerFrame::bytes() const
{
    if (!isValid())
        return {};

    QKnxByteArray addAdditionalInfoBytes;
    for (const auto &info : d_ptr->m_additionalInfos)
        addAdditionalInfoBytes += info.bytes();

    return QKnxByteArray { quint8(d_ptr->m_code), d_ptr->m_additionalInfoSize }
        + addAdditionalInfoBytes
        + d_ptr->m_ctrl.bytes()
        + d_ptr->m_extCtrl.bytes()
        + d_ptr->m_srcAddress.bytes()
        + d_ptr->m_dstAddress.bytes()
        + QKnxUtils::QUint8::bytes(d_ptr->m_tpdu.dataSize())
        + d_ptr->m_tpdu.bytes();
}

/*!
    Constructs a link layer frame from the byte array \a data starting at the
    position \a index inside the array using the number of bytes specified by
    \a size. Sets the medium type of the frame to \a mediumType
*/
QKnxLinkLayerFrame QKnxLinkLayerFrame::fromBytes(const QKnxByteArray &data, quint16 index,
    quint16 size, QKnx::MediumType mediumType)
{
    // data is not big enough according to the given size to be read
    const qint32 availableSize = (data.size() - index) - size;
    if (availableSize < 0)
        return {};

    QKnxLinkLayerFrame frame(MessageCode(data.value(index)));
    frame.setMediumType(mediumType);
    frame.setServiceInformation(data.mid(index + 1, size - 1));
    return frame;
}

/*!
    Returns the size in bytes of the whole additional information field.
*/
quint8 QKnxLinkLayerFrame::additionalInfosSize() const
{
    return d_ptr->m_additionalInfoSize;
}

/*!
    Adds the KNX additional info parameter \a info to the additional information
    field. The information is sorted in ascending order based on its type.

    \sa QKnxAdditionalInfo, QKnxAdditionalInfo::type()
*/
void QKnxLinkLayerFrame::addAdditionalInfo(const QKnxAdditionalInfo &info)
{
    d_ptr->m_additionalInfos.append(info);
    d_ptr->m_additionalInfosSorted = false;
    d_ptr->m_additionalInfoSize += info.size();
}

/*!
    Returns a vector of additional information contained in the frame.

    \sa QKnxAdditionalInfo
*/
QVector<QKnxAdditionalInfo> QKnxLinkLayerFrame::additionalInfos() const
{
    if (!d_ptr->m_additionalInfosSorted) {
        std::sort(d_ptr->m_additionalInfos.begin(), d_ptr->m_additionalInfos.end(),
            [](const QKnxAdditionalInfo &lhs, const QKnxAdditionalInfo &rhs) {
            return lhs.type() < rhs.type();
        });
        d_ptr->m_additionalInfosSorted = true;
    }
    return d_ptr->m_additionalInfos;
}

/*!
    Removes all additional information of the type \a type.

    \sa QKnxAdditionalInfo, QKnxAdditionalInfo::Type
*/
void QKnxLinkLayerFrame::removeAdditionalInfo(QKnxAdditionalInfo::Type type)
{
    auto info = d_ptr->m_additionalInfos.begin();
    while (info != d_ptr->m_additionalInfos.end()) {
        if (info->type() == type) {
            d_ptr->m_additionalInfos.erase(info);
            d_ptr->m_additionalInfoSize -= info->size();
            break;
        }
        info++;
    }
}

/*!
    Removes the first occurrence of the additional information \a info from the
    frame's additional information field.

    \sa QKnxAdditionalInfo
*/
void QKnxLinkLayerFrame::removeAdditionalInfo(const QKnxAdditionalInfo &info)
{
    if (d_ptr->m_additionalInfos.removeOne(info))
        d_ptr->m_additionalInfoSize -= info.size();
}

/*!
    Removes all additional information from the frame.
*/
void QKnxLinkLayerFrame::clearAdditionalInfos()
{
    d_ptr->m_additionalInfoSize = 0;
    d_ptr->m_additionalInfos.clear();
}

/*!
    Constructs a copy of \a other.
*/
QKnxLinkLayerFrame::QKnxLinkLayerFrame(const QKnxLinkLayerFrame &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxLinkLayerFrame &QKnxLinkLayerFrame::operator=(const QKnxLinkLayerFrame &other)
{
  d_ptr = other.d_ptr;
  return *this;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxLinkLayerFrame::QKnxLinkLayerFrame(QKnxLinkLayerFrame &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = nullptr;
}

/*!
    Move-assigns \a other to this object instance.
*/
QKnxLinkLayerFrame &QKnxLinkLayerFrame::operator=(QKnxLinkLayerFrame &&other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
    return *this;
}

/*!
    Swaps \a other with this object. This operation is very fast and never fails.
*/
void QKnxLinkLayerFrame::swap(QKnxLinkLayerFrame &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxLinkLayerFrame::operator==(const QKnxLinkLayerFrame &other) const
{
    return  d_ptr == other.d_ptr
        || (d_ptr->m_code == other.d_ptr->m_code
            && d_ptr->m_mediumType == other.d_ptr->m_mediumType
            && d_ptr->m_srcAddress == other.d_ptr->m_srcAddress
            && d_ptr->m_dstAddress == other.d_ptr->m_dstAddress
            && d_ptr->m_tpdu == other.d_ptr->m_tpdu
            && d_ptr->m_ctrl == other.d_ptr->m_ctrl
            && d_ptr->m_extCtrl == other.d_ptr->m_extCtrl
            && d_ptr->m_additionalInfos == other.d_ptr->m_additionalInfos);
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxLinkLayerFrame::operator!=(const QKnxLinkLayerFrame &other) const
{
    return !operator==(other);
}

/*!
    Returns an instance of a generic link layer frame builder.
*/
QKnxLinkLayerFrame::Builder QKnxLinkLayerFrame::builder()
{
    return QKnxLinkLayerFrame::Builder();
}

/*!
    \relates QKnxLinkLayerFrame

    Writes the link layer frame \a frame to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxLinkLayerFrame &frame)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << frame.bytes().toHex();
}

QT_END_NAMESPACE
