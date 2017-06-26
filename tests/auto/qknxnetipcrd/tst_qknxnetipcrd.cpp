/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
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

class tst_QKnxNetIpCRD : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorKnxAddress();
    void testConstructorConnectionType();
    void testDebugStream();
    void testDataStream();
};

void tst_QKnxNetIpCRD::testDefaultConstructor()
{
    QKnxNetIpCRD crd;
    QCOMPARE(crd.isValid(), false);
    QCOMPARE(crd.size(), quint16(0));
    QCOMPARE(crd.bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(crd.payload().size(), quint16(0));
    QCOMPARE(crd.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x00 }, Code { 0x00 }, "
        "Bytes {  }"));
    QCOMPARE(quint8(crd.connectionType()), quint8(0));
}

void tst_QKnxNetIpCRD::testConstructorKnxAddress()
{
    {
        QKnxNetIpCRD crd(QKnxAddress {});
        QCOMPARE(crd.isValid(), false);
        QCOMPARE(crd.size(), quint16(2));
        QCOMPARE(crd.bytes<QByteArray>(), QByteArray::fromHex("0204"));
        QCOMPARE(crd.payload().size(), quint16());
        QCOMPARE(crd.payload().bytes<QByteArray>(), QByteArray());
        QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x04 }, "
            "Bytes {  }"));
        QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(crd.individualAddress().toString(), QStringLiteral("0.0.0"));

        // make the CRD valid by setting an QKnxAddress

        crd.setIndividualAddress({ QKnxAddress::Type::Individual, QVector<quint8> { 0x11, 0x01 } });
        QCOMPARE(crd.isValid(), true);
        QCOMPARE(crd.size(), quint16(4));
        QCOMPARE(crd.bytes<QByteArray>(), QByteArray::fromHex("04041101"));
        QCOMPARE(crd.payload().size(), quint16(2));
        QCOMPARE(crd.payload().bytes<QVector<quint8>>(), QVector<quint8>({ 0x11, 0x01 }));
        QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x04 }, Code { 0x04 }, "
            "Bytes { 0x11, 0x01 }"));
        QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(crd.individualAddress().toString(), QStringLiteral("1.1.1"));
    }

    {
        QKnxNetIpCRD crd({ QKnxAddress::Type::Individual, QVector<quint8> { 0x11, 0x01 } });
        QCOMPARE(crd.isValid(), true);
        QCOMPARE(crd.size(), quint16(4));
        QCOMPARE(crd.bytes<QByteArray>(), QByteArray::fromHex("04041101"));
        QCOMPARE(crd.payload().size(), quint16(2));
        QCOMPARE(crd.payload().bytes<QVector<quint8>>(), QVector<quint8>({ 0x11, 0x01 }));
        QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x04 }, Code { 0x04 }, "
            "Bytes { 0x11, 0x01 }"));
        QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Tunnel);
        QCOMPARE(crd.individualAddress().toString(), QStringLiteral("1.1.1"));
    }
}

void tst_QKnxNetIpCRD::testConstructorConnectionType()
{
    QKnxNetIpCRD crd(QKnxNetIp::ConnectionType::Unknown);
    QCOMPARE(crd.isValid(), false);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes<QByteArray>(), QByteArray::fromHex("0200"));
    QCOMPARE(crd.payload().size(), quint16(0));
    QCOMPARE(crd.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x00 }, "
        "Bytes {  }"));
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Unknown);

    crd = QKnxNetIpCRD(QKnxNetIp::ConnectionType::DeviceManagement);
    QCOMPARE(crd.isValid(), true);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes<QByteArray>(), QByteArray::fromHex("0203"));
    QCOMPARE(crd.payload().size(), quint16(0));
    QCOMPARE(crd.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x03 }, "
        "Bytes {  }"));
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::DeviceManagement);

    crd = QKnxNetIpCRD(QKnxNetIp::ConnectionType::Tunnel);
    QCOMPARE(crd.isValid(), false);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes<QByteArray>(), QByteArray::fromHex("0204"));
    QCOMPARE(crd.payload().size(), quint16());
    QCOMPARE(crd.payload().bytes<QByteArray>(), QByteArray());
    QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x04 }, "
        "Bytes {  }"));
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::Tunnel);

    crd = QKnxNetIpCRD(QKnxNetIp::ConnectionType::RemoteLogging);
    QCOMPARE(crd.isValid(), true);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes<QByteArray>(), QByteArray::fromHex("0206"));
    QCOMPARE(crd.payload().size(), quint16(0));
    QCOMPARE(crd.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x06 }, "
        "Bytes {  }"));
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::RemoteLogging);

    crd = QKnxNetIpCRD(QKnxNetIp::ConnectionType::RemoteConfiguration);
    QCOMPARE(crd.isValid(), true);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes<QByteArray>(), QByteArray::fromHex("0207"));
    QCOMPARE(crd.payload().size(), quint16(0));
    QCOMPARE(crd.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x07 }, "
        "Bytes {  }"));
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::RemoteConfiguration);

    crd = QKnxNetIpCRD(QKnxNetIp::ConnectionType::ObjectServer);
    QCOMPARE(crd.isValid(), true);
    QCOMPARE(crd.size(), quint16(2));
    QCOMPARE(crd.bytes<QByteArray>(), QByteArray::fromHex("0208"));
    QCOMPARE(crd.payload().size(), quint16(0));
    QCOMPARE(crd.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(crd.toString(), QString::fromLatin1("Total size { 0x02 }, Code { 0x08 }, "
        "Bytes {  }"));
    QCOMPARE(crd.connectionType(), QKnxNetIp::ConnectionType::ObjectServer);
}

void tst_QKnxNetIpCRD::testDebugStream()
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

    qDebug() << QKnxNetIpCRD();
    QCOMPARE(s_msg, QString::fromLatin1("0x1nv4l1d"));

    qDebug() << QKnxNetIpCRD::fromBytes(QByteArray::fromHex("04041101"), 0);
    QCOMPARE(s_msg, QString::fromLatin1("0x04041101"));
}

void tst_QKnxNetIpCRD::testDataStream()
{
    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpCRD(QKnxNetIp::ConnectionType::DeviceManagement);
        QCOMPARE(byteArray, QByteArray::fromHex("0203"));
    }

    {
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << QKnxNetIpCRD({ QKnxAddress::Type::Individual, QVector<quint8> { 0x11, 0x01 } });
        QCOMPARE(byteArray, QByteArray::fromHex("04041101"));
    }
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpCRD)

#include "tst_qknxnetipcrd.moc"
