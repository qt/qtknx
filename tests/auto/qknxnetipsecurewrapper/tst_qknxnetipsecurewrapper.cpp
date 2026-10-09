/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtCore/QLoggingCategory>
#include <QtKnx/QKnx1Bit>
#include <QtKnx/QKnx8BitUnsignedValue>
#include <QtKnx/QKnxCryptographicEngine>
#include <QtKnx/QKnxLinkLayerFrameBuilder>
#include <QtKnx/QKnxNetIpSecureWrapperProxy>
#include <QtKnx/QKnxNetIpTunnelingRequestProxy>
#include <QtKnx/QKnxTpdu>
#include <QtTest/QTest>

QT_BEGIN_NAMESPACE

char *toString(const QKnxByteArray &ba)
{
    using QTest::toString;
    return toString("QKnxByteArray(" + ba.toByteArray() + ')');
}

QT_END_NAMESPACE

class tst_qknxnetipsecurewrapper : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QLoggingCategory::setFilterRules("qt.network.ssl=false");
    }

    void testEncryptDecryptPayload_data();
    void testEncryptDecryptPayload();
};

void tst_qknxnetipsecurewrapper::testEncryptDecryptPayload_data()
{
    QTest::addColumn<QKnxByteArray>("data");
    QTest::addColumn<QKnxByteArray>("mac");

    // Row 1: GroupValueWrite carrying switch On (DPT 1.001)
    QKnxSwitch switchDpt(QKnxSwitch::State::On);
    QTest::newRow("switch") << switchDpt.bytes() << QKnxByteArray::fromHex("50c56f8a6df62fa53b0f675e903be3cc");

    // Row 2: GroupValueWrite carrying scaling at max (DPT 5.001 = 100% -> 0xFF)
    // when sending QKnxScaling the length of the messages is exact multiple of 16 which is the size of the chiper block used.
    QKnxScaling scalingDpt(100.0);
    QTest::newRow("scaling") << scalingDpt.bytes() << QKnxByteArray::fromHex("19b7135209c9e60cf6f4b3f790f0bf20");
}

void tst_qknxnetipsecurewrapper::testEncryptDecryptPayload()
{
    if (QKnxCryptographicEngine::sslLibraryVersionNumber() < 0x1010000fL)
        return;

    QFETCH(QKnxByteArray, data);
    QFETCH(QKnxByteArray, mac);

    const quint16 channelId = 0x0001;
    const quint16 sessionId = 0x0001;
    const quint48 sequenceNumber = 0x000000000010;
    const auto serialNumber = QKnxByteArray::fromHex("00fa12345678");
    const quint16 messageTag = 0xaffe;
    const auto sessionKey = QKnxByteArray::fromHex("289426c2912535ba98279a4d1843c487");

    QKnxTpdu tpdu(
        QKnxTpdu::TransportControlField::DataGroup,
        QKnxTpdu::ApplicationControlField::GroupValueWrite,
        data);

    auto frame = QKnxLinkLayerFrame::builder()
        .setMedium(QKnx::MediumType::NetIP)
        .setSourceAddress(QKnxAddress(QKnxAddress::Type::Individual, "0.0.1"))
        .setDestinationAddress(QKnxAddress(QKnxAddress::Type::Group, "0/0/1"))
        .setTpdu(tpdu)
        .createFrame();

    auto cemi = QKnxNetIpTunnelingRequestProxy::builder()
        .setChannelId(channelId)
        .setSequenceNumber(1)
        .setCemi(frame)
        .create();

    auto secureWrapper = QKnxNetIpSecureWrapperProxy::secureBuilder()
        .setSecureSessionId(sessionId)
        .setSequenceNumber(sequenceNumber)
        .setSerialNumber(serialNumber)
        .setMessageTag(messageTag)
        .setEncapsulatedFrame(cemi)
        .create(sessionKey);

    QVERIFY(secureWrapper.isValid());

    const QKnxNetIpSecureWrapperProxy proxy(secureWrapper);
    QVERIFY(proxy.isValid());
    QCOMPARE(proxy.secureSessionId(), sessionId);
    QCOMPARE(proxy.sequenceNumber(), sequenceNumber);
    QCOMPARE(proxy.serialNumber(), serialNumber);
    QCOMPARE(proxy.messageTag(), messageTag);

    // Decrypt and verify the result matches the original inner frame bytes
    const auto decryptedData = QKnxCryptographicEngine::decryptSecureWrapperPayload(sessionKey,
        proxy.encapsulatedFrame(), sequenceNumber, serialNumber, messageTag);
    const auto decMac = QKnxCryptographicEngine::decryptMessageAuthenticationCode(sessionKey,
            proxy.messageAuthenticationCode(), sequenceNumber, serialNumber, messageTag);

    QCOMPARE(decryptedData, cemi.bytes());
    QCOMPARE(decMac, mac);
}

QTEST_APPLESS_MAIN(tst_qknxnetipsecurewrapper)

#include "tst_qknxnetipsecurewrapper.moc"
