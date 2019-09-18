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
#include <QtKnx/qknxnetipcri.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpCri : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorTunnelingLayer();
    void testConstructorConnectionType();
    void testAdditionalData();
    void testDebugStream();
};

void tst_QKnxNetIpCri::testDefaultConstructor()
{
    QKnxNetIpCri cri;
    QCOMPARE(cri.isValid(), false);
    QCOMPARE(cri.size(), quint16(0));
    QCOMPARE(cri.bytes(), QKnxByteArray {});
    QCOMPARE(cri.data().size(), quint16(0));
    QCOMPARE(cri.data(), QKnxByteArray {});
    QCOMPARE(QKnxNetIpCriProxy(cri).connectionType(), QKnxNetIp::ConnectionType::Unknown);
}

void tst_QKnxNetIpCri::testConstructorTunnelingLayer()
{
    {
        auto builder = QKnxNetIpCriProxy::builder();
        auto cri = builder
            .setTunnelLayer(QKnxNetIp::TunnelLayer::Unknown)
            .create();

        QCOMPARE(cri.isValid(), true);
        QCOMPARE(cri.size(), quint16(2));
        QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0204"));
        QCOMPARE(cri.data().size(), quint16());
        QCOMPARE(cri.data(), QKnxByteArray {});

        QKnxNetIpCriProxy view(cri);
        QCOMPARE(view.isValid(), false);
        QCOMPARE(view.connectionType(), QKnxNetIp::ConnectionType::Unknown);
        QCOMPARE(view.tunnelLayer(), QKnxNetIp::TunnelLayer::Unknown);

        // make the Cri valid by setting the QKnxNetIp::TunnelLayer
        cri = builder.setTunnelLayer(QKnxNetIp::TunnelLayer::Busmonitor)
            .create();
        QCOMPARE(cri.isValid(), true);
        QCOMPARE(cri.size(), quint16(4));
        QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("04048000"));
        QCOMPARE(cri.data().size(), quint16(2));
        QCOMPARE(cri.data(), QKnxByteArray({ 0x80, 0x00 }));
        QCOMPARE(view.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(view.tunnelLayer(), QKnxNetIp::TunnelLayer::Busmonitor);
    }

    {
        auto cri = QKnxNetIpCriProxy::builder()
            .setTunnelLayer(QKnxNetIp::TunnelLayer::Raw)
            .create();
        QCOMPARE(cri.isValid(), true);
        QCOMPARE(cri.size(), quint16(4));
        QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("04040400"));
        QCOMPARE(cri.data().size(), quint16(2));
        QCOMPARE(cri.data(), QKnxByteArray({ 0x04, 0x00 }));

        QKnxNetIpCriProxy view(cri);
        QCOMPARE(view.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(view.tunnelLayer(), QKnxNetIp::TunnelLayer::Raw);
    }
}

void tst_QKnxNetIpCri::testConstructorConnectionType()
{
    QKnxNetIpCri cri;
    QCOMPARE(cri.isValid(), false);
    QCOMPARE(cri.size(), quint16(0));
    QCOMPARE(cri.bytes(), QKnxByteArray {});
    QCOMPARE(cri.data().size(), quint16(0));
    QCOMPARE(cri.data(), QKnxByteArray {});

    QKnxNetIpCriProxy view(cri);
    QCOMPARE(view.connectionType(), QKnxNetIp::ConnectionType::Unknown);

    auto builder = QKnxNetIpCriProxy::builder();
    cri = builder
        .setConnectionType(QKnxNetIp::ConnectionType::DeviceManagement)
        .create();
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0203"));
    QCOMPARE(cri.data().size(), quint16(0));
    QCOMPARE(cri.data(), QKnxByteArray {});
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.connectionType(), QKnxNetIp::ConnectionType::DeviceManagement);

    cri = builder
        .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
        .create();
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0204"));
    QCOMPARE(cri.data().size(), quint16());
    QCOMPARE(cri.data(), QKnxByteArray {});
    QCOMPARE(view.isValid(), false);
    QCOMPARE(view.connectionType(), QKnxNetIp::ConnectionType::Unknown);

    cri = builder
        .setConnectionType(QKnxNetIp::ConnectionType::RemoteLogging)
        .create();
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0206"));
    QCOMPARE(cri.data().size(), quint16(0));
    QCOMPARE(cri.data(), QKnxByteArray {});
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.connectionType(), QKnxNetIp::ConnectionType::RemoteLogging);

    cri = builder
        .setConnectionType(QKnxNetIp::ConnectionType::RemoteConfiguration)
        .create();
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0207"));
    QCOMPARE(cri.data().size(), quint16(0));
    QCOMPARE(cri.data(), QKnxByteArray {});
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.connectionType(), QKnxNetIp::ConnectionType::RemoteConfiguration);

    cri = builder
        .setConnectionType(QKnxNetIp::ConnectionType::ObjectServer)
        .create();
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0208"));
    QCOMPARE(cri.data().size(), quint16(0));
    QCOMPARE(cri.data(), QKnxByteArray {});
    QCOMPARE(view.isValid(), true);
    QCOMPARE(view.connectionType(), QKnxNetIp::ConnectionType::ObjectServer);
}

void tst_QKnxNetIpCri::testAdditionalData()
{
    auto frame = QKnxNetIpCriProxy::builder()
        .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
        .setTunnelLayer(QKnxNetIp::TunnelLayer::Link)
        .create();
    auto frame2 = QKnxNetIpCriProxy::builder()
        .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
        .setAdditionalData({ quint8(QKnxNetIp::TunnelLayer::Link), 0x00 })
        .create();
    QCOMPARE(frame, frame2);

    QKnxNetIpCriProxy cri(frame);
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.isExtended(), false);
    QCOMPARE(cri.individualAddress(), QKnxAddress{});

    QKnxAddress address { QKnxAddress::Type::Individual, 2013 };
    frame = QKnxNetIpCriProxy::builder()
        .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
        .setTunnelLayer(QKnxNetIp::TunnelLayer::Link)
        .setIndividualAddress(address)
        .create();
    frame2 = QKnxNetIpCriProxy::builder()
        .setConnectionType(QKnxNetIp::ConnectionType::Tunnel)
        .setAdditionalData(QKnxByteArray { quint8(QKnxNetIp::TunnelLayer::Link), 0x00 }
            + address.bytes())
        .create();
    QCOMPARE(frame, frame2);

    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.isExtended(), true);
    QCOMPARE(cri.individualAddress(), address);
}

void tst_QKnxNetIpCri::testDebugStream()
{
    struct DebugHandler
    {
        explicit DebugHandler(QtMessageHandler newMessageHandler)
            : oldMessageHandler(qInstallMessageHandler(newMessageHandler))
        {}
        ~DebugHandler()
        {
            qInstallMessageHandler(oldMessageHandler);
        }
        QtMessageHandler oldMessageHandler;
    } _(myMessageHandler);

    qDebug() << QKnxNetIpCriProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0204"));

    qDebug() << QKnxNetIpCri::fromBytes(QKnxByteArray::fromHex("04048000"), 0);
    QCOMPARE(s_msg, QString::fromLatin1("0x04048000"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCri)

#include "tst_qknxnetipcri.moc"
