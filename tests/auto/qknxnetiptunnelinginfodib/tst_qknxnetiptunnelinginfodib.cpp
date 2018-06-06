/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#include <QtCore/qdebug.h>
#include <QtTest/qtest.h>
#include <QtKnx/qknxnetiptunnelinginfodib.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}
QT_BEGIN_NAMESPACE

char *toString(const QKnxByteArray &ba)
{
    using QTest::toString;
    return toString("QKnxByteArray(" + ba.toByteArray() + ')');
}

QT_END_NAMESPACE
class tst_QKnxNetIpTunnelingInfoDib : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultDib()
    {
        QKnxNetIpDib dib;

        QKnxNetIpTunnelingInfoDibProxy proxy(dib);
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
        QCOMPARE(proxy.maximumInterfaceApduLength(), 0x0000);
        QCOMPARE(proxy.tunnelingSlotInfo(), QKnxNetIpTunnelingSlotInfo());
        QCOMPARE(proxy.optionalSlotInfos(), QVector<QKnxNetIpTunnelingSlotInfo>());

        dib = QKnxNetIpTunnelingInfoDibProxy::builder().create();
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.isValid(), false);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::Unknown);
        QCOMPARE(proxy.maximumInterfaceApduLength(), 0x0000);
        QCOMPARE(proxy.tunnelingSlotInfo(), QKnxNetIpTunnelingSlotInfo());
        QCOMPARE(proxy.optionalSlotInfos(), QVector<QKnxNetIpTunnelingSlotInfo>());
    }

    void testValidDib()
    {
        QKnxAddress address { QKnxAddress::Type::Individual, 4353 };
        QKnxNetIpTunnelingSlotInfo::Status status { QKnxNetIpTunnelingSlotInfo::State::Available };
        QKnxNetIpTunnelingSlotInfo slotInfo { address, status };
        auto infoDib = QKnxNetIpTunnelingInfoDibProxy::builder()
            .setMaximumInterfaceApduLength(4096)
            .setTunnelingSlotInfo(slotInfo)
            .setOptionalSlotInfos({ slotInfo })
            .create();
        auto bytes = QKnxByteArray::fromHex("0c0710001101ffff1101ffff");

        QKnxNetIpTunnelingInfoDibProxy proxy(infoDib);
        QCOMPARE(proxy.isValid(), true);
        QCOMPARE(proxy.descriptionType(), QKnxNetIp::DescriptionType::TunnelingInfo);
        QCOMPARE(proxy.maximumInterfaceApduLength(), 0x1000);
        QCOMPARE(proxy.tunnelingSlotInfo(), slotInfo);
        QCOMPARE(proxy.optionalSlotInfos().count(), 1);

        auto slot = proxy.optionalSlotInfos().first();
        QCOMPARE(slot.isValid(), true);
        QCOMPARE(slot.individualAddress(), address);
        QCOMPARE(slot.status(), status);
        QCOMPARE(slot, slotInfo);
        QCOMPARE(slot.bytes(), QKnxByteArray::fromHex("1101ffff"));

        QKnxNetIpDib infoDib2 = QKnxNetIpDib::fromBytes(bytes);
        QCOMPARE(infoDib2, infoDib);
        QKnxNetIpTunnelingInfoDibProxy proxy2(infoDib2);
        QCOMPARE(proxy2.isValid(), true);
        QCOMPARE(proxy2.descriptionType(), QKnxNetIp::DescriptionType::TunnelingInfo);
        QCOMPARE(proxy2.maximumInterfaceApduLength(), 0x1000);
        QCOMPARE(proxy.tunnelingSlotInfo(), slotInfo);
        QCOMPARE(proxy2.optionalSlotInfos().count(), 1);

        auto slot2 = proxy2.optionalSlotInfos().first();
        QCOMPARE(slot2.isValid(), true);
        QCOMPARE(slot2.individualAddress(), address);
        QCOMPARE(slot2.status(), status);

        QCOMPARE(slot, slot2);
        QCOMPARE(slot2, slotInfo);
        QCOMPARE(slot2.bytes(), QKnxByteArray::fromHex("1101ffff"));
    }

    void testTunnelingSlotInfo()
    {
        QKnxNetIpTunnelingSlotInfo info;
        QCOMPARE(info.isValid(), false);
        QCOMPARE(info.individualAddress(), QKnxAddress());
        QCOMPARE(info.status(), QKnxNetIpTunnelingSlotInfo::State::Invalid);
        QCOMPARE(info.bytes(), QKnxByteArray());

        info.setIndividualAddress({ QKnxAddress::Type::Individual, 4353 });
        QCOMPARE(info.isValid(), false);
        QCOMPARE(info.individualAddress(), QKnxAddress(QKnxAddress::Type::Individual, 4353));
        QCOMPARE(info.status(), QKnxNetIpTunnelingSlotInfo::State::Invalid);
        QCOMPARE(info.bytes(), QKnxByteArray());

        info.setStatus(QKnxNetIpTunnelingSlotInfo::State::Available);
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.individualAddress(), QKnxAddress(QKnxAddress::Type::Individual, 4353));
        QCOMPARE(info.status(), QKnxNetIpTunnelingSlotInfo::State::Available);
        QCOMPARE(info.bytes(), QKnxByteArray::fromHex("1101ffff"));

        info = QKnxNetIpTunnelingSlotInfo({ QKnxAddress::Type::Individual, 4353 });
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.individualAddress(), QKnxAddress(QKnxAddress::Type::Individual, 4353));
        QCOMPARE(info.status(), QKnxNetIpTunnelingSlotInfo::State::NotAvailable);
        QCOMPARE(info.bytes(), QKnxByteArray::fromHex("1101fff8"));

        info = QKnxNetIpTunnelingSlotInfo({ QKnxAddress::Type::Individual, 4353 },
            QKnxNetIpTunnelingSlotInfo::State::Free);
        QCOMPARE(info.isValid(), true);
        QCOMPARE(info.individualAddress(), QKnxAddress(QKnxAddress::Type::Individual, 4353));
        QCOMPARE(info.status(), QKnxNetIpTunnelingSlotInfo::State::Free);
        QCOMPARE(info.bytes(), QKnxByteArray::fromHex("1101fff9"));
    }

    void testDebugStream()
    {
        struct DebugHandler
        {
            explicit DebugHandler(QtMessageHandler newMessageHandler)
                : oldMessageHandler(qInstallMessageHandler(newMessageHandler)) {}
            ~DebugHandler() {
                qInstallMessageHandler(oldMessageHandler);
            }
            QtMessageHandler oldMessageHandler;
        } _(myMessageHandler);

        qDebug() << QKnxNetIpTunnelingInfoDibProxy::builder().create();
        QCOMPARE(s_msg, QString::fromLatin1("0x0207"));

        qDebug() << QKnxNetIpTunnelingInfoDibProxy::builder()
            .setMaximumInterfaceApduLength(4096)
            .setTunnelingSlotInfo(
                QKnxNetIpTunnelingSlotInfo::fromBytes(QKnxByteArray::fromHex("1101ffff"), 0)
            ).setOptionalSlotInfos({ { { QKnxAddress::Type::Individual, 4353 },
                QKnxNetIpTunnelingSlotInfo::State::Available } })
            .create();
        QCOMPARE(s_msg, QString::fromLatin1("0x0c0710001101ffff1101ffff"));
    }
};

QTEST_APPLESS_MAIN(tst_QKnxNetIpTunnelingInfoDib)

#include "tst_qknxnetiptunnelinginfodib.moc"
