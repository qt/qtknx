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
#include <QtKnx/qknxdevicemanagementframe.h>
#include <QtKnx/qknxdevicemanagementframefactory.h>
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
        QKnxDeviceManagementFrame frame(QKnxDeviceManagementFrame::MessageCode::PropertyReadRequest);
        frame.setObjectType(QKnxInterfaceObjectType::System::KnxNetIpParameter);
        frame.setObjectInstance(1);
        frame.setProperty(QKnxInterfaceObjectProperty::KnxNetIpParameter::CurrentIpAddress);
        frame.setNumberOfElements(14);
        frame.setStartIndex(25);
        frame.setData(QKnxByteArray { 0x01, 0x02, 0x03, 0x04, 0x05 });

        QCOMPARE(frame.messageCode(), QKnxDeviceManagementFrame::MessageCode::PropertyReadRequest);
        QCOMPARE(quint16(frame.objectType()), quint16(QKnxInterfaceObjectType::KnxNetIpParameter));
        QCOMPARE(frame.objectInstance(), quint8(1));
        QCOMPARE(quint8(frame.property()), quint8(QKnxInterfaceObjectProperty::CurrentIpAddress));
        QCOMPARE(frame.numberOfElements(), quint8(14));
        QCOMPARE(frame.startIndex(), quint16(25));
        QCOMPARE(frame.data(), QKnxByteArray({ 0x01, 0x02, 0x03, 0x04, 0x05 }));

        // TODO: extend the test with the following methods

        // isValid()
        // isNegativeConfirmation()
        // error()
        // setError(...)
        // returnCode()
        // setReturnCode(...)
        // QKnxDeviceManagementFrame(const QKnxDeviceManagementFrame&)
        // size()
        // serviceInformation()
        // setMessageCode(...)
    }

    void testFromBytes()
    {
        auto objectTypeProperty = QKnxByteArray { 0xfc, 0x00, 0x08, 0x01, 0x01, 0x10, 0x01 };
        QKnxDeviceManagementFrame frame = QKnxDeviceManagementFrame::fromBytes(objectTypeProperty,
            0, objectTypeProperty.size());

        QCOMPARE(frame.messageCode(), QKnxDeviceManagementFrame::MessageCode::PropertyReadRequest);
        QCOMPARE(quint16(frame.objectType()), quint16(QKnxInterfaceObjectType::CemiServer));
        QCOMPARE(frame.objectInstance(), quint8(1));
        QCOMPARE(quint8(frame.property()), quint8(QKnxInterfaceObjectProperty::ObjectType));
        QCOMPARE(frame.numberOfElements(), quint8(1));
        QCOMPARE(frame.startIndex(), quint16(1));

        auto bytes = QKnxByteArray { 0xfc, 0x00, 0x0b, 0x01, 0x4c, 0x10, 0x00 };
        frame = QKnxDeviceManagementFrame::fromBytes(bytes, 0, objectTypeProperty.size());
        frame.setNumberOfElements(15);
        bytes = frame.bytes();  // TODO: Fix this, what was the idea to test here?
    }

    void testFactory()
    {
        auto propRead = QKnxDeviceManagementFrame::propertyReadBuilder()
            .setObjectType(QKnxInterfaceObjectType::KnxNetIpParameter)
            .setObjectInstance(3)
            .setProperty(QKnxInterfaceObjectProperty::FriendlyName)
            .setNumberOfElements(1)
            .setStartIndex(0).createRequest();
        QCOMPARE(propRead.messageCode(), QKnxDeviceManagementFrame::MessageCode::PropertyReadRequest);
        QCOMPARE(propRead.objectType(), QKnxInterfaceObjectType::KnxNetIpParameter);
        QCOMPARE(propRead.objectInstance(), 3);
        QCOMPARE(propRead.property(), QKnxInterfaceObjectProperty::FriendlyName);
        QCOMPARE(propRead.numberOfElements(), 1);
        QCOMPARE(propRead.startIndex(), 0);

        auto propConf = QKnxDeviceManagementFrame::propertyReadBuilder()
            .setObjectType(QKnxInterfaceObjectType::KnxNetIpParameter)
            .setObjectInstance(12)
            .setProperty(QKnxInterfaceObjectProperty::KnxNetIpParameter::DefaultGateway)
            .setNumberOfElements(10)
            .setStartIndex(100).createConfirmation(QKnxByteArray(10, 0xff));
        QCOMPARE(propConf.messageCode(), QKnxDeviceManagementFrame::MessageCode::PropertyReadConfirmation);
        QCOMPARE(propConf.objectType(), QKnxInterfaceObjectType::KnxNetIpParameter);
        QCOMPARE(propConf.objectInstance(), 12);
        QCOMPARE(propConf.property(), QKnxInterfaceObjectProperty::DefaultGateway);
        QCOMPARE(propConf.numberOfElements(), 10);
        QCOMPARE(propConf.startIndex(), 100);
        QCOMPARE(propConf.data(), QKnxByteArray(10, 0xff));

        propConf = QKnxDeviceManagementFrame::propertyReadBuilder()
            .createConfirmation(QKnxByteArray(10, 0xff), propRead);
        QCOMPARE(propConf.messageCode(), QKnxDeviceManagementFrame::MessageCode::PropertyReadConfirmation);
        QCOMPARE(propConf.objectType(), QKnxInterfaceObjectType::KnxNetIpParameter);
        QCOMPARE(propConf.objectInstance(), 3);
        QCOMPARE(propConf.property(), QKnxInterfaceObjectProperty::FriendlyName);
        QCOMPARE(propConf.numberOfElements(), 1);
        QCOMPARE(propConf.startIndex(), 0);
        QCOMPARE(propConf.data(), QKnxByteArray(10, 0xff));

        propConf = QKnxDeviceManagementFrame::propertyReadBuilder()
            .createNegativeConfirmation(QKnx::NetIp::CemiServer::Error::IllegalCommand, propRead);
        QCOMPARE(propConf.messageCode(), QKnxDeviceManagementFrame::MessageCode::PropertyReadConfirmation);
        QCOMPARE(propConf.objectType(), QKnxInterfaceObjectType::KnxNetIpParameter);
        QCOMPARE(propConf.objectInstance(), 3);
        QCOMPARE(propConf.property(), QKnxInterfaceObjectProperty::FriendlyName);
        QCOMPARE(propConf.numberOfElements(), 0);
        QCOMPARE(propConf.startIndex(), 0);
        QCOMPARE(propConf.data(), QKnxByteArray(1, 0x06));
        QCOMPARE(propConf.error(), QKnx::NetIp::CemiServer::Error::IllegalCommand);
        QCOMPARE(propConf.isNegativeConfirmation(), true);

        // TODO: add some checks against above created cEMI

        auto cemi = QKnxDeviceManagementFrame::functionPropertyCommandBuilder()
            .setObjectType(QKnxInterfaceObjectType::KnxNetIpParameter)
            .setObjectInstance(1)
            .setProperty(QKnxInterfaceObjectProperty::FriendlyName)
            .createRequest({ 0x00, 0x01, 0x02 });
        // TODO: add some checks against above created cEMI
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
};

QTEST_MAIN(tst_QKnxLocalDeviceManagementFrame)

#include "tst_qknxdevicemanagementframe.moc"
