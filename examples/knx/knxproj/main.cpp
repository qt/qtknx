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

#include <QtCore/qdebug.h>
#include <QtCore/qvector.h>
#include <QtCore/qversionnumber.h>
#include <QtCore/qxmlstream.h>
#include <QtKnx/private/qknxinstallation_p.h>
#include <QtKnx/private/qknxprojectroot_p.h>
#include <QtKnx/private/qknxprojectinformation_p.h>

#include <QtKnx/private/qzipreader_p.h>
#include <QtKnx/private/qzipwriter_p.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
        return EXIT_FAILURE;

    QZipReader zipReader(argv[1]);
    if (!zipReader.isReadable() || zipReader.status() != QZipReader::NoError)
        return EXIT_FAILURE;

    int projectId = -1, nullXmlId = -1;
    auto fileInfos = zipReader.fileInfoList();
    for (int i = 0; i < fileInfos.count(); ++i) {
        if (fileInfos[i].filePath.endsWith(QStringLiteral("project.xml"))) {
            projectId = i;
            if (projectId >= 0 && nullXmlId >= 0)
                break;
        }

        if (fileInfos[i].filePath.endsWith(QStringLiteral("0.xml"))) {
            nullXmlId = i;
            if (projectId >= 0 && nullXmlId >= 0)
                break;
        }
    }

    if (projectId < 0 || nullXmlId < 0)
        return EXIT_FAILURE;

    QXmlStreamReader xmlReader(zipReader.fileData(fileInfos[projectId].filePath));
    struct Trace {
        Trace(QXmlStreamReader *xmlReader)
            : reader(xmlReader)
        {}
        ~Trace()
        {
            if (reader->hasError())
                qDebug() << reader->errorString();
        }

    private:
        QXmlStreamReader *reader;
    } tracer(&xmlReader);

    if (!xmlReader.readNextStartElement())
        return EXIT_FAILURE;

    QKnxProjectRoot projectXml;
    if (!projectXml.parseElement(&xmlReader, true))
        return EXIT_FAILURE;

    xmlReader.clear();
    xmlReader.addData(zipReader.fileData(fileInfos[nullXmlId].filePath));

    if (!xmlReader.readNextStartElement())
        return EXIT_FAILURE;

    QKnxProjectRoot nullXml;
    if (!nullXml.parseElement(&xmlReader, true))
        return EXIT_FAILURE;

    auto project = nullXml.Project.value(0);
    auto installation = project.Installations.value(0);
    auto groupAddresses = installation.GroupAddresses.value(0);
    auto groupRanges = groupAddresses.GroupRanges.value(0);

    return EXIT_SUCCESS;
};
