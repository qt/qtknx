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
#include <QtKnx/qknxnetipcrd.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpCrd : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorKnxAddress();
    void testConstructorConnectionType();
    void testDebugStream();
};

void tst_QKnxNetIpCrd::testDefaultConstructor()
{
    QKnxNetIpCrd crd;
    QCOMPARE(crd.isValid(), false);
    QCOMPARE(crd.size(), quint16(0));
    QCOMPARE(crd.bytes(), QKnxByteArray {});
    QCOMPARE(crd.data().size(), quint16(0));
    QCOMPARE(crd.data(), QKnxByteArray {});
    QCOMPARE(quint8(crd.connectionType()), quint8(0));
}

void tst_QKnxNetIpCrd::testConstructorKnxAddress()
{
    {
        QKnxNetIpCrd crd(QKnxAddress {});
        QCOMPARE(crd.isValid(), false);
        QCOMPARE(crd.size(), quint16(2));
        QCOMPARE(crd.bytes(), QKnxByteArray::fromHex("0204"));
        QCOMPARE(crd.data().size(), quint16());
        QCOMPARE(crd.data(), QKnxByteArray {});
        QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(crd.individualAddress().toString(), QStringLiteral("0.0.0"));

        // make the CRD valid by setting an QKnxAddress

        crd.setIndividualAddress({ QKnxAddress::Type::Individual, QKnxByteArray { 0x11, 0x01 } });
        QCOMPARE(crd.isValid(), true);
        QCOMPARE(crd.size(), quint16(4));
        QCOMPARE(crd.bytes(), QKnxByteArray::fromHex("04041101"));
        QCOMPARE(crd.data().size(), quint16(2));
        QCOMPARE(crd.data(), QKnxByteArray({ 0x11, 0x01 }));
        QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(crd.individualAddress().toString(), QStringLiteral("1.1.1"));
    }

    {
        QKnxNetIpCrd crd({ QKnxAddress::Type::Individual, QKnxByteArray { 0x11, 0x01 } });
        QCOMPARE(crd.isValid(), true);
        QCOMPARE(crd.size(), quint16(4));
        QCOMPARE(crd.bytes(), QKnxByteArray::fromHex("04041101"));
        QCOMPARE(crd.data().size(), quint16(2));
        QCOMPARE(crd.data(), QKnxByteArray({ 0x11, 0x01 }));
        QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(crd.individualAddress().toString(), QStringLiteral("1.1.1"));
    }
}

void tst_QKnxNetIpCrd::testConstructorConnectionType()
{
    QKnxNetIpCrd crd(QKnxNetIp::ConnectionType::Unknown);
    QCOMPARE(crd.isValid(), false);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes(), QKnxByteArray::fromHex("0200"));
    QCOMPARE(crd.data().size(), quint16(0));
    QCOMPARE(crd.data(), QKnxByteArray {});
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Unknown);

    crd = QKnxNetIpCrd(QKnxNetIp::ConnectionType::DeviceManagement);
    QCOMPARE(crd.isValid(), true);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes(), QKnxByteArray::fromHex("0203"));
    QCOMPARE(crd.data().size(), quint16(0));
    QCOMPARE(crd.data(), QKnxByteArray {});
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::DeviceManagement);

    crd = QKnxNetIpCrd(QKnxNetIp::ConnectionType::Tunnel);
    QCOMPARE(crd.isValid(), false);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes(), QKnxByteArray::fromHex("0204"));
    QCOMPARE(crd.data().size(), quint16());
    QCOMPARE(crd.data(), QKnxByteArray {});
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Tunnel);

    crd = QKnxNetIpCrd(QKnxNetIp::ConnectionType::RemoteLogging);
    QCOMPARE(crd.isValid(), true);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes(), QKnxByteArray::fromHex("0206"));
    QCOMPARE(crd.data().size(), quint16(0));
    QCOMPARE(crd.data(), QKnxByteArray {});
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::RemoteLogging);

    crd = QKnxNetIpCrd(QKnxNetIp::ConnectionType::RemoteConfiguration);
    QCOMPARE(crd.isValid(), true);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes(), QKnxByteArray::fromHex("0207"));
    QCOMPARE(crd.data().size(), quint16(0));
    QCOMPARE(crd.data(), QKnxByteArray {});
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::RemoteConfiguration);

    crd = QKnxNetIpCrd(QKnxNetIp::ConnectionType::ObjectServer);
    QCOMPARE(crd.isValid(), true);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes(), QKnxByteArray::fromHex("0208"));
    QCOMPARE(crd.data().size(), quint16(0));
    QCOMPARE(crd.data(), QKnxByteArray {});
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::ObjectServer);
}

void tst_QKnxNetIpCrd::testDebugStream()
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

    qDebug() << QKnxNetIpCrd();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpCrd::fromBytes(QKnxByteArray::fromHex("04041101"), 0);
    QCOMPARE(s_msg, QString::fromLatin1("0x04041101"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCrd)

#include "tst_qknxnetipcrd.moc"
