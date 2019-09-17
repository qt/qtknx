/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
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
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtCore/QCommandLineParser>
#include <QtCore/qdebug.h>
#include <QtCore/qvector.h>
#include <QtCore/qversionnumber.h>
#include <QtCore/qxmlstream.h>
#include <QtKnx/qknxgroupaddressinfos.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("knxproj");

    QCommandLineParser cliParser;
    cliParser.setApplicationDescription("KNX Project file parser");
    cliParser.addHelpOption();
    QCommandLineOption verboseOption({"v","verbose"},
                                     QCoreApplication::translate(
                                         "main", "Show more details of the project file."));
    cliParser.addOption(verboseOption);
    QCommandLineOption projFilePathOption(QStringList() << "p" << "project",
              QCoreApplication::translate("main", "Path to the project file to parse."),
              QCoreApplication::translate("main", "path"));
    cliParser.addOption(projFilePathOption);
    cliParser.process(app);

    bool verbose = cliParser.isSet(verboseOption);

    if (!cliParser.isSet(projFilePathOption)) {
        qInfo() << "Error: Missing project file name" << Qt::endl;
        cliParser.showHelp(EXIT_FAILURE);
    }

    QKnxGroupAddressInfos infos(cliParser.value(projFilePathOption));
    infos.parse();

    qInfo().noquote() << QString::fromLatin1("Opening project file: %1")
                         .arg(infos.projectFile());
    QString status = infos.errorString().isEmpty() ? "No errors" : infos.errorString();
    qInfo().noquote() << QString::fromLatin1("Status parsing project: %1")
                         .arg(status);
    qInfo().noquote() << QString::fromLatin1("Project ids found: %1")
                         .arg(infos.projectIds().size());

    if (infos.status() != QKnxGroupAddressInfos::Status::NoError) {
        qInfo() << "ERROR:" <<  infos;
        return EXIT_FAILURE;
    }

    for (const auto &projId: infos.projectIds()) {
        auto installations = infos.installations(projId);
        qInfo()<< " # project"<<infos.projectName(projId);
        qInfo().noquote() << QString::fromLatin1("     found %1 installation(s)")
                             .arg(installations.size());
        for (const QString &installation: installations) {
            auto groupAddresses = infos.addressInfos(projId, installation);
            qInfo().noquote() << QString::fromLatin1("  - Installation \"%1\"")
                                 .arg(installation);
            qInfo().noquote() << QString::fromLatin1("     found %1 Group address(es)")
                       .arg(groupAddresses.size());
            for (const QKnxGroupAddressInfo &addInfo: groupAddresses) {
                if (!addInfo.isValid())
                    return EXIT_FAILURE;
                if (verbose)
                    qInfo().noquote() << QString::fromLatin1("      %1: %2")
                                         .arg(addInfo.name()).arg(addInfo.address().toString());
            }
            qInfo() << Qt::endl;
        }
    }

    return EXIT_SUCCESS;
};
