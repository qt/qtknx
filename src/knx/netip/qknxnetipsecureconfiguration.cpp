/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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
****************************************************************************/

#include "qknxcryptographicengine.h"
#include "qknxnetipsecureconfiguration.h"

#include <QtCore/qfile.h>

#include "private/qknxkeyring_p.h"
#include "private/qknxnetipsecureconfiguration_p.h"

QT_BEGIN_NAMESPACE

namespace QKnxPrivate
{
    static QKnxNetIpSecureConfiguration fromInterface(const QKnx::Ets::Keyring::QKnxInterface &iface,
        const QKnxByteArray &pwHash, const QKnxByteArray &createdHash)
    {
        QKnxNetIpSecureConfiguration s;
        s.setUserId(QKnxNetIp::SecureUserId(iface.UserID));
        s.setPrivateKey(QKnxSecureKey::generatePrivateKey());
        s.setHost({ QKnxAddress::Type::Individual, iface.Host });
        s.setUserPassword(QKnxCryptographicEngine::decodeAndDecryptPassword(pwHash,
            createdHash, iface.Password).toByteArray());
        s.setIndividualAddress({ QKnxAddress::Type::Individual, iface.IndividualAddress });
        s.setDeviceAuthenticationCode(QKnxCryptographicEngine::decodeAndDecryptPassword(pwHash,
            createdHash, iface.Authentication).toByteArray());
        return s;
    }

    static QKnxNetIpSecureConfiguration fromDevice(const QKnx::Ets::Keyring::QKnxDevice &device,
        const QKnxByteArray &pwHash, const QKnxByteArray &createdHash)
    {
        QKnxNetIpSecureConfiguration s;
        s.setUserId(QKnxNetIp::SecureUserId::Management);
        s.setPrivateKey(QKnxSecureKey::generatePrivateKey());
        s.setHost({ QKnxAddress::Type::Individual, device.IndividualAddress });
        s.setUserPassword(QKnxCryptographicEngine::decodeAndDecryptPassword(pwHash,
            createdHash, device.ManagementPassword).toByteArray());
        s.setIndividualAddress({ QKnxAddress::Type::Individual, device.IndividualAddress });
        s.setDeviceAuthenticationCode(QKnxCryptographicEngine::decodeAndDecryptPassword(pwHash,
            createdHash, device.Authentication).toByteArray());
        return s;
    }

    static QVector<QKnxNetIpSecureConfiguration> fromKeyring(QKnxNetIpSecureConfiguration::Type type,
        const QKnxAddress &ia, const QString &filePath, const QByteArray &password, bool validate)
    {
        QFile file;
        file.setFileName(filePath);
        if (!file.open(QIODevice::ReadOnly))
            return {};

        QXmlStreamReader reader(&file);
        QKnx::Ets::Keyring::QKnxKeyring keyring;

        const auto pwHash = QKnxCryptographicEngine::keyringPasswordHash(password);
        if (validate) {
            if (!keyring.validate(&reader, pwHash))
                return {};
            file.seek(0);
            reader.setDevice(&file);
        }

        if (!keyring.parseElement(&reader, true))
            return {};
        const auto createdHash = QKnxCryptographicEngine::hashSha256(keyring.Created.toUtf8());

        auto iaString = ia.toString();
        QVector<QKnxNetIpSecureConfiguration> results;

        if (type == QKnxNetIpSecureConfiguration::Type::Tunneling) {
            if (keyring.Interface.isEmpty())
                return {};

            if (!iaString.isEmpty()) { // only a single interface is requested
                for (const auto iface : qAsConst(keyring.Interface)) {
                    if (iaString != iface.IndividualAddress)
                        continue;
                    return { QKnxPrivate::fromInterface(iface, pwHash, createdHash) };
                }
            } else {
                for (const auto iface : qAsConst(keyring.Interface))
                    results.append(QKnxPrivate::fromInterface(iface, pwHash, createdHash));
            }
        }

        if (type == QKnxNetIpSecureConfiguration::Type::DeviceManagement) {
            if (keyring.Devices.isEmpty())
                return {};

            const auto devices = keyring.Devices.value(0).Device;
            if (!iaString.isEmpty()) { // only a single device is requested
                for (const auto device : devices) {
                    if (iaString != device.IndividualAddress)
                        continue;
                    return { QKnxPrivate::fromDevice(device, pwHash, createdHash) };
                }
            } else {
                for (const auto device : devices)
                    results.append(QKnxPrivate::fromDevice(device, pwHash, createdHash));
            }
        }

        return results;
    }
}

/*!
    \since 5.13
    \inmodule QtKnx

    \class QKnxNetIpSecureConfiguration
    \ingroup qtknx-general-classes

    \brief The QKnxNetIpSecureConfiguration class holds configuration
    options used for the secure session authentication process.

    It holds information such as secure key, user ID and password, device
    authentication code, and so on.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.
*/

/*!
    \enum QKnxNetIpSecureConfiguration::Type

    This enum holds the type of secure configuration that can be constructed
    from an ETS exported keyring (*.knxkeys) file.

    \value Tunneling
            KNXnet/IP secure tunneling configuration.
    \value DeviceManagement
            KNXnet/IP secure device management configuration.
*/

/*!
    Constructs a new, empty, invalid secure configuration.

    \sa isNull(), isValid()
*/
QKnxNetIpSecureConfiguration::QKnxNetIpSecureConfiguration()
    : d(new QKnxNetIpSecureConfigurationPrivate)
{}

/*!
    Releases any resources held by the secure configuration.
*/
QKnxNetIpSecureConfiguration::~QKnxNetIpSecureConfiguration() = default;

/*!
    Constructs a vector of secure configurations for the given type
    \a type from an ETS exported \a keyring (*.knxkeys) file that was
    encrypted with the given password \a password. Set the \a validate
    argument to \c true to verify that all data in the keyring file is
    trustworthy, \c false to omit the check.

    \note If an error occurred, no or invalid information for \a type
     was found in the keyring file, the returned vector can be empty.
*/
QVector<QKnxNetIpSecureConfiguration> QKnxNetIpSecureConfiguration::fromKeyring(Type type,
    const QString &keyring, const QByteArray &password, bool validate)
{
    return QKnxPrivate::fromKeyring(type, {}, keyring, password, validate);
}

/*!
    Constructs a secure configurations for the given type \a type and the
    given individual address \a ia from an ETS exported \a keyring (*.knxkeys)
    file that was encrypted with the given password \a password.
    Set the \a validate argument to \c true to verify that all data in the
    keyring file is trustworthy, \c false to omit the check.

    \note If an error occurred, no or invalid information for \a type or \a ia
    was found in the keyring file;
    the function returns a \l {default-constructed value} which can be invalid.
*/
QKnxNetIpSecureConfiguration QKnxNetIpSecureConfiguration::fromKeyring(QKnxNetIpSecureConfiguration::Type type,
    const QKnxAddress &ia, const QString &keyring, const QByteArray &password, bool validate)
{
    return QKnxPrivate::fromKeyring(type, ia, keyring, password, validate).value(0, {});
}

/*!
    Returns \c true if this is a default constructed secure configuration;
    otherwise returns \c false. A secure configuration is considered \c null
    if it contains no initialized values.
*/
bool QKnxNetIpSecureConfiguration::isNull() const
{
    return d->privateKey.isNull() && d->userId == QKnxNetIp::SecureUserId::Reserved
        && d->userPassword.isNull() && d->deviceAuthenticationCode.isNull();
}

/*!
    Returns \c true if the secure configuration contains initialized values and
    is in itself valid, otherwise returns \c false.

    A valid secure configuration consists of at least a valid user ID,
    a valid \l {QKnxSecureKey} {secure key}, and sensible device authentication
    code.
*/
bool QKnxNetIpSecureConfiguration::isValid() const
{
    if (isNull())
        return false;

    return d->privateKey.type() == QKnxSecureKey::Type::Private && d->privateKey.isValid()
        && QKnxNetIp::isSecureUserId(d->userId) && (!d->deviceAuthenticationCode.isEmpty());
}

/*!
    Returns the host address of the secure KNX device this secure configuration
    targets. The host address can be empty and is not required to establish a
    secure session.

    The purpose of this field is more to help GUI applications to indicate
    which KNX secure device can be used with this secure configuration.
*/
QKnxAddress QKnxNetIpSecureConfiguration::host() const
{
    return d->host;
}

/*!
    Sets the secure configurations host address to \a hostAddress. The host
    address can be empty and is not required to establish a secure session.
*/
void QKnxNetIpSecureConfiguration::setHost(const QKnxAddress &hostAddress)
{
    d->host = hostAddress;
}

/*!
    Returns the public \l {QKnxSecureKey} {secure key} used to establish the
    secure session. The public key is derived from the given private key.
*/
QKnxSecureKey QKnxNetIpSecureConfiguration::publicKey() const
{
    return d->publicKey;
}

/*!
    Returns the private \l {QKnxSecureKey} {secure key} used to establish the
    secure session.
*/
QKnxSecureKey QKnxNetIpSecureConfiguration::privateKey() const
{
    return d->privateKey;
}

/*!
    Set the \l {QKnxSecureKey} {secure key} used to establish the secure
    connection to \a key and returns \c true on success; \c false otherwise.
*/
bool QKnxNetIpSecureConfiguration::setPrivateKey(const QKnxSecureKey &key)
{
    auto valid = key.isValid() && key.type() == QKnxSecureKey::Type::Private;
    if (valid) {
        d->privateKey = key;
        d->publicKey = QKnxSecureKey::publicKeyFromPrivate(key);
    }
    return valid;
}

/*!
    Returns the user ID used in the KNXnet/IP session authentication frame.
*/
QKnxNetIp::SecureUserId QKnxNetIpSecureConfiguration::userId() const
{
    return d->userId;
}

/*!
    Sets the user ID used in the KNXnet/IP session authentication frame
    to \a userId and returns \c true on success; \c false otherwise.

    \note A userId() with the value \c QKnxNetIp::SecureUserId::Reserved or
    equal to or more than \c QKnxNetIp::SecureUserId::Invalid is considered
    invalid according to the KNX application note AN159.
*/
bool QKnxNetIpSecureConfiguration::setUserId(QKnxNetIp::SecureUserId userId)
{
    auto valid = QKnxNetIp::isSecureUserId(userId);
    if (valid)
        d->userId = userId;
    return valid;
}

/*!
    Returns the user password used to authenticate the user while establishing
    the secure session as an array of bytes.
*/
QByteArray QKnxNetIpSecureConfiguration::userPassword() const
{
    return d->userPassword;
}

/*!
    Sets the user password to authenticate the user while establishing the
    secure session to \a userPassword. Returns \c true on success; \c false
    otherwise.
*/
void QKnxNetIpSecureConfiguration::setUserPassword(const QByteArray &userPassword)
{
    d->userPassword = userPassword;
}

/*!
    Returns the requested individual address for the secure session.
*/
QKnxAddress QKnxNetIpSecureConfiguration::individualAddress() const
{
    return d->ia;
}

/*!
    Sets the requested individual address of the secure session to \a address.
    Returns \c true on success; \c false otherwise.

    \note To request any of the freely available addresses for the secure
    session, or to reset the requested one, pass an invalid \a address to
    the function.
*/
bool QKnxNetIpSecureConfiguration::setIndividualAddress(const QKnxAddress &address)
{
    if ((address.type() == QKnxAddress::Type::Individual) || (!address.isValid()))
        d->ia = address;
    return d->ia == address;
}

/*!
    Returns the device authentication code to establish the secure session
    as an array of bytes.
*/
QByteArray QKnxNetIpSecureConfiguration::deviceAuthenticationCode() const
{
    return d->deviceAuthenticationCode;
}

/*!
    Sets the device authentication code used to establish the secure session
    to \a authenticationCode. Returns \c true on success; \c false otherwise.

    \note The device authentication code cannot be empty.
*/
bool QKnxNetIpSecureConfiguration::setDeviceAuthenticationCode(const QByteArray &authenticationCode)
{
    auto valid = !authenticationCode.isEmpty();
    if (valid)
        d->deviceAuthenticationCode = authenticationCode;
    return valid;
}

/*!
    Returns \c true if the keep alive flag is set; \c false otherwise. By
    default this is set to \c false.
*/
bool QKnxNetIpSecureConfiguration::isSecureSessionKeepAliveSet() const
{
    return d->keepAlive;
}

/*!
    Determines whether the connection should be kept alive. Set \a keepAlive to
    \c true to keep a secure session alive even if there is no traffic for more
    than \l {QKnx::NetIp::SecureSessionTimeout} {60 seconds}.
*/
void QKnxNetIpSecureConfiguration::setKeepSecureSessionAlive(bool keepAlive)
{
    d->keepAlive = keepAlive;
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSecureConfiguration::QKnxNetIpSecureConfiguration(const QKnxNetIpSecureConfiguration &other)
    : d(other.d)
{}

/*!
    Assigns the specified \a other to this secure configuration.
*/
QKnxNetIpSecureConfiguration &QKnxNetIpSecureConfiguration::operator=(const QKnxNetIpSecureConfiguration &other)
{
    d = other.d;
    return *this;
}

/*!
    Move-constructs a secure configuration, making it point to the same secure
    configuration that \a other was pointing to.
*/
QKnxNetIpSecureConfiguration::QKnxNetIpSecureConfiguration(QKnxNetIpSecureConfiguration &&other) Q_DECL_NOTHROW
    : d(other.d)
{
    other.d = nullptr;
}

/*!
    Move-assigns \a other to this secure configuration.
*/
QKnxNetIpSecureConfiguration &
    QKnxNetIpSecureConfiguration::operator=(QKnxNetIpSecureConfiguration &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Swaps \a other with this secure configuration. This operation is very fast
    and never fails.
*/
void QKnxNetIpSecureConfiguration::swap(QKnxNetIpSecureConfiguration &other) Q_DECL_NOTHROW
{
    d.swap(other.d);
}

/*!
    Returns \c true if this secure configuration and the given \a other are
    equal; otherwise returns \c false.
*/
bool QKnxNetIpSecureConfiguration::operator==(const QKnxNetIpSecureConfiguration &other) const
{
    return d == other.d || (d->privateKey == other.d->privateKey
        && d->publicKey == other.d->publicKey
        && d->host == other.d->host
        && d->userId == other.d->userId
        && d->userPassword == other.d->userPassword
        && d->ia == other.d->ia
        && d->deviceAuthenticationCode == other.d->deviceAuthenticationCode
        && d->keepAlive == other.d->keepAlive);
}

/*!
    Returns \c true if this secure configuration and the given \a other are not
    equal; otherwise returns \c false.
*/
bool QKnxNetIpSecureConfiguration::operator!=(const QKnxNetIpSecureConfiguration &other) const
{
    return !operator==(other);
}

QT_END_NAMESPACE
