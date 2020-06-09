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

#include "qknxprojectroot_p.h"

QT_BEGIN_NAMESPACE

// -- QKnxAddinData

bool QKnxAddinData::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("AddinData")) {
        auto attrs = reader->attributes();

        // required attributes
        QStringView attr;
        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("AddinId"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("AddinId"), attr, &AddinId, reader, pedantic))
            return false;

        if (!QKnxProjectUtils::fetchAttr(attrs, QLatin1String("Name"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setString(QLatin1String("Name"), attr, 50, &Name, reader, pedantic))
            return false;

        reader->skipCurrentElement(); // attribute element only
    } else {
        reader->raiseError(tr("Expected element <AddinData>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxProject

bool QKnxProject::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QLatin1String("Project")) {
        QStringView attr; // required attribute
        if (!QKnxProjectUtils::fetchAttr(reader->attributes(), QLatin1String("Id"), &attr, reader))
            return false;
        if (!QKnxProjectUtils::setNCName(QLatin1String("Id"), attr, &Id, reader, pedantic))
            return false;

        // children
        while (!reader->atEnd() && !reader->hasError()) {
            auto tokenType = reader->readNext();
            if (tokenType == QXmlStreamReader::TokenType::StartElement) {
                if (reader->name() == QStringLiteral("ProjectInformation")) {
                    if (pedantic && ProjectInformation.size() >= 1) {
                        reader->raiseError(tr("Pedantic error: Encountered element "
                            "<ProjectInformation> more than once."));
                        return false;
                    }
                    QKnxProjectInformation info;
                    if (!info.parseElement(reader, pedantic))
                        return false;
                    ProjectInformation.append(info);
                } else if (reader->name() == QStringLiteral("Installations")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &Installations))
                        return false;
                    if (pedantic && Installations.size() > 16) {
                        reader->raiseError(tr("Pedantic error: Encountered element <Installation> "
                            "more than sixteen times."));
                        return false;
                    }
                } else if (reader->name() == QStringLiteral("UserFiles")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &UserFiles))
                        return false;
                } else if (reader->name() == QStringLiteral("AddinData")) {
                    if (!QKnxProjectUtils::parseChildElement(reader, pedantic, &AddinData))
                        return false;
                }
            } else if (tokenType == QXmlStreamReader::TokenType::EndElement) {
                if (reader->name() == QLatin1String("Project"))
                    break;
            }
        }
    } else {
        reader->raiseError(tr("Expected element <Project>, got: <%1>.").arg(reader->name()));
    }
    return !reader->hasError();
}


// -- QKnxProjectRoot

bool QKnxProjectRoot::parseElement(QXmlStreamReader *reader, bool pedantic)
{
    if (!reader || !reader->isStartElement())
        return false;

    if (reader->name() == QStringLiteral("KNX")) {
        auto attrs = reader->attributes();

        // optional attributes
        CreatedBy = attrs.value(QStringLiteral("CreatedBy")).toString();
        ToolVersion = attrs.value(QStringLiteral("ToolVersion")).toString();

        // children
        while (!reader->atEnd() && !reader->hasError() && reader->readNextStartElement()) {
            if (reader->name() == QStringLiteral("Project")) {
                QKnxProject project;
                if (!project.parseElement(reader, pedantic))
                    return false;
                Project.append(project);
            } else {
                reader->skipCurrentElement();
            }
        }
    } else {
        reader->raiseError(tr("Expected element <KNX>, got: <%1>.").arg(reader->name().toString()));
    }
    return !reader->hasError();
}

QT_END_NAMESPACE
