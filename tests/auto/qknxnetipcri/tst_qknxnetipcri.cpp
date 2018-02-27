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
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpCri::testDefaultConstructor()
{
    QKnxNetIpCri cri;
    QCOMPARE(cri.isValid(), false);
    QCOMPARE(cri.size(), quint16(0));
    QCOMPARE(cri.bytes(), QKnxByteArray {});
    QCOMPARE(cri.payload().size(), quint16(0));
    QCOMPARE(cri.payload().bytes(), QKnxByteArray {});
    QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x00 }, Code { 0x00 }, "
        "Bytes {  }"));
    QCOMPARE(quint8(cri.connectionType()), quint8(0));
}

void tst_QKnxNetIpCri::testConstructorTunnelingLayer()
{
    {
        QKnxNetIpCri cri(QKnxNetIp::TunnelingLayer::Unknown);
        QCOMPARE(cri.isValid(), false);
        QCOMPARE(cri.size(), quint16(2));
        QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0204"));
        QCOMPARE(cri.payload().size(), quint16());
        QCOMPARE(cri.payload().bytes(), QKnxByteArray {});
        QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x04 }, "
            "Bytes {  }"));
        QCOMPARE(cri.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(cri.tunnelingLayer(), QKnxNetIp::TunnelingLayer::Unknown);

        // make the Cri valid by setting the QKnxNetIp::TunnelingLayer

        cri.setTunnelingLayer(QKnxNetIp::TunnelingLayer::Busmonitor);
        QCOMPARE(cri.isValid(), true);
        QCOMPARE(cri.size(), quint16(4));
        QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("04048000"));
        QCOMPARE(cri.payload().size(), quint16(2));
        QCOMPARE(cri.payload().bytes(), QKnxByteArray({ 0x80, 0x00 }));
        QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x04 }, Code { 0x04 }, "
            "Bytes { 0x80, 0x00 }"));
        QCOMPARE(cri.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(cri.tunnelingLayer(), QKnxNetIp::TunnelingLayer::Busmonitor);
    }

    {
        QKnxNetIpCri cri(QKnxNetIp::TunnelingLayer::Raw);
        QCOMPARE(cri.isValid(), true);
        QCOMPARE(cri.size(), quint16(4));
        QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("04040400"));
        QCOMPARE(cri.payload().size(), quint16(2));
        QCOMPARE(cri.payload().bytes(), QKnxByteArray({ 0x04, 0x00 }));
        QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x04 }, Code { 0x04 }, "
            "Bytes { 0x04, 0x00 }"));
        QCOMPARE(cri.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(cri.tunnelingLayer(), QKnxNetIp::TunnelingLayer::Raw);
    }
}

void tst_QKnxNetIpCri::testConstructorConnectionType()
{
    QKnxNetIpCri cri(QKnxNetIp::ConnectionType::Unknown);
    QCOMPARE(cri.isValid(), false);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0200"));
    QCOMPARE(cri.payload().size(), quint16(0));
    QCOMPARE(cri.payload().bytes(), QKnxByteArray {});
    QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x00 }, "
        "Bytes {  }"));
    QCOMPARE(cri.connectionType(), QKnxNetIp::ConnectionType::Unknown);

    cri = QKnxNetIpCri(QKnxNetIp::ConnectionType::DeviceManagement);
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0203"));
    QCOMPARE(cri.payload().size(), quint16(0));
    QCOMPARE(cri.payload().bytes(), QKnxByteArray {});
    QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x03 }, "
        "Bytes {  }"));
    QCOMPARE(cri.connectionType(), QKnxNetIp::ConnectionType::DeviceManagement);

    cri = QKnxNetIpCri(QKnxNetIp::ConnectionType::Tunnel);
    QCOMPARE(cri.isValid(), false);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0204"));
    QCOMPARE(cri.payload().size(), quint16());
    QCOMPARE(cri.payload().bytes(), QKnxByteArray {});
    QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x04 }, "
        "Bytes {  }"));
    QCOMPARE(cri.connectionType(), QKnxNetIp::ConnectionType::Tunnel);

    cri = QKnxNetIpCri(QKnxNetIp::ConnectionType::RemoteLogging);
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0206"));
    QCOMPARE(cri.payload().size(), quint16(0));
    QCOMPARE(cri.payload().bytes(), QKnxByteArray {});
    QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x06 }, "
        "Bytes {  }"));
    QCOMPARE(cri.connectionType(), QKnxNetIp::ConnectionType::RemoteLogging);

    cri = QKnxNetIpCri(QKnxNetIp::ConnectionType::RemoteConfiguration);
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0207"));
    QCOMPARE(cri.payload().size(), quint16(0));
    QCOMPARE(cri.payload().bytes(), QKnxByteArray {});
    QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x07 }, "
        "Bytes {  }"));
    QCOMPARE(cri.connectionType(), QKnxNetIp::ConnectionType::RemoteConfiguration);

    cri = QKnxNetIpCri(QKnxNetIp::ConnectionType::ObjectServer);
    QCOMPARE(cri.isValid(), true);
    QCOMPARE(cri.size(), quint16(2));
    QCOMPARE(cri.bytes(), QKnxByteArray::fromHex("0208"));
    QCOMPARE(cri.payload().size(), quint16(0));
    QCOMPARE(cri.payload().bytes(), QKnxByteArray {});
    QCOMPARE(cri.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x08 }, "
        "Bytes {  }"));
    QCOMPARE(cri.connectionType(), QKnxNetIp::ConnectionType::ObjectServer);
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

    qDebug() << QKnxNetIpCri();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpCri::fromBytes(QKnxByteArray::fromHex("04048000"), 0);
    QCOMPARE(s_msg, QString::fromLatin1("0x04048000"));

}

void tst_QKnxNetIpCri::testDataStream()
{
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpCri(QKnxNetIp::ConnectionType::DeviceManagement);
        QCOMPARE(byteArray, QByteArray::fromHex("0203"));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpCri(QKnxNetIp::TunnelingLayer::Busmonitor);
        QCOMPARE(byteArray, QByteArray::fromHex("04048000"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCri)

#include "tst_qknxnetipcri.moc"
