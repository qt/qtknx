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
#include <QtKnx/qknxlocaldevicemanagementframe.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxLocalDeviceManagementFrame : public QObject
{
    Q_OBJECT

private slots:
    void testClass()
    {
        QKnxLocalDeviceManagementFrame frame(QKnxLocalDeviceManagementFrame::MessageCode::PropertyReadRequest);
        frame.setObjectType(QKnxInterfaceObjectType::System::KnxNetIpParameter);
        frame.setObjectInstance(1);
        frame.setProperty(QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentIpAddress);
        frame.setNumberOfElements(14);
        frame.setStartIndex(25);
        frame.setData(QByteArray::fromHex("0102030405"));

        QCOMPARE(frame.messageCode(), QKnxLocalDeviceManagementFrame::MessageCode::PropertyReadRequest);
        QCOMPARE(quint16(frame.objectType()), quint16(QKnxInterfaceObjectType::KnxNetIpParameter));
        QCOMPARE(frame.objectInstance(), quint8(1));
        QCOMPARE(quint8(frame.property()), quint8(QKnxInterfaceObjectProperty::CurrentIpAddress));
        QCOMPARE(frame.numberOfElements(), quint8(14));
        QCOMPARE(frame.startIndex(), quint16(25));
        QCOMPARE(frame.data(), QByteArray::fromHex("0102030405"));
    }

    void testFactory()
    {
        auto objectTypeProperty = QByteArray::fromHex("fc000801011001");
        QKnxLocalDeviceManagementFrame frame = QKnxLocalDeviceManagementFrame::fromBytes(objectTypeProperty,
            0, objectTypeProperty.size());

        QCOMPARE(frame.messageCode(), QKnxCemiFrame::MessageCode::PropertyReadRequest);
        QCOMPARE(quint16(frame.objectType()), quint16(QKnxInterfaceObjectType::CemiServer));
        QCOMPARE(frame.objectInstance(), quint8(1));
        QCOMPARE(quint8(frame.property()), quint8(QKnxInterfaceObjectProperty::ObjectType));
        QCOMPARE(frame.numberOfElements(), quint8(1));
        QCOMPARE(frame.startIndex(), quint16(1));

        auto bytes = QByteArray::fromHex("fc000b014c1000");
        frame = QKnxLocalDeviceManagementFrame::fromBytes(bytes, 0, objectTypeProperty.size());
        frame.setNumberOfElements(15);
        bytes = frame.bytes().toHex();
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

        // TODO: Implement.
    }

    void testDataStream()
    {
        // TODO: Implement.
    }
};

QTEST_MAIN(tst_QKnxLocalDeviceManagementFrame)

#include "tst_qknxdevicemanagementframe.moc"
