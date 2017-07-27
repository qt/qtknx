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
#include <QtKnx/qknxdevicemanagementframe.h>
#include <QtTest/qtest.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxDeviceManagementFrame : public QObject
{
    Q_OBJECT

private slots:
    void testConstructors()
    {
        QKnxDeviceManagementFrame frame(QKnxDeviceManagementFrame::MessageCode::PropertyReadRequest);
        frame.setObjectType(QKnxInterfaceObject::Type::System::KnxNetIpParameter);
        frame.setObjectInstance(1);
        frame.setProperty(QKnxInterfaceObject::Property::KnxNetIpParameter::CurrentIpAddress);
        frame.setNumberOfElements(14);
        frame.setStartIndex(25);
        frame.setData(QByteArray::fromHex("0102030405"));

        QCOMPARE(frame.messageCode(), QKnxDeviceManagementFrame::MessageCode::PropertyReadRequest);
        QCOMPARE(quint16(frame.objectType()), quint16(QKnxInterfaceObject::Type::KnxNetIpParameter));
        QCOMPARE(frame.objectInstance(), quint8(1));
        QCOMPARE(quint8(frame.property()), quint8(QKnxInterfaceObject::Property::CurrentIpAddress));
        QCOMPARE(frame.numberOfElements(), quint8(14));
        QCOMPARE(frame.startIndex(), quint16(25));

        // TODO: Extend the auto-test.
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
    }

    void testDataStream()
    {
    }
};

QTEST_MAIN(tst_QKnxDeviceManagementFrame)

#include "tst_qknxdevicemanagementframe.moc"
