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
#include <QtKnx/qknxnetipdeviceconfigurationrequest.h>
#include <QtKnx/qknxdevicemanagementframefactory.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpDeviceConfigurationRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
};

void tst_QKnxNetIpDeviceConfigurationRequest::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpDeviceConfigurationRequestProxy configuration(frame);

    QCOMPARE(configuration.isValid(), false);
}

void tst_QKnxNetIpDeviceConfigurationRequest::testConstructor()
{
    auto cemi = QKnxDeviceManagementFrame::propertyReadBuilder()
        .setObjectType(QKnxInterfaceObjectType::KnxNetIpParameter)
        .setObjectInstance(1)
        .setProperty(QKnxInterfaceObjectProperty::FriendlyName)
        .setNumberOfElements(1)
        .setStartIndex(0)
        .createRequest();
    auto frame = QKnxNetIpDeviceConfigurationRequestProxy::builder()
        .setChannelId(1)
        .setSequenceNumber(1)
        .setCemi(cemi)
        .create();

    QKnxNetIpDeviceConfigurationRequestProxy request(frame);
    QCOMPARE(request.isValid(), true);
    QCOMPARE(frame.size(), quint16(17));

    QCOMPARE(request.channelId(), quint8(1));
    QCOMPARE(request.sequenceNumber(), quint8(1));
    QCOMPARE(request.cemi().bytes(), cemi.bytes());
}

void tst_QKnxNetIpDeviceConfigurationRequest::testDebugStream()
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

    qDebug() << QKnxNetIpDeviceConfigurationRequestProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100310000b0400000000"));

    auto cemi = QKnxDeviceManagementFrame::propertyReadBuilder()
        .setObjectType(QKnxInterfaceObjectType::System::Device)
        .setObjectInstance(1)
        .setProperty(QKnxInterfaceObjectProperty::Device::DeviceDescriptor)
        .setNumberOfElements(1)
        .setStartIndex(1)
        .createRequest();
    qDebug() << QKnxNetIpDeviceConfigurationRequestProxy::builder()
        .setChannelId(2)
        .setSequenceNumber(0)
        .setCemi(cemi)
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x06100310001104020000fc000001531001"));
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpDeviceConfigurationRequest)

#include "tst_qknxnetipdeviceconfigurationrequest.moc"
