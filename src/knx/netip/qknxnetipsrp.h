/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#ifndef QKNXNETIPSRP_H
#define QKNXNETIPSRP_H

#include <QtCore/qvector.h>
#include <QtCore/qshareddata.h>
#include <QtKnx/qtknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>

QT_BEGIN_NAMESPACE

struct QKnxServiceInfo;
namespace SrpBuilders {

class Q_KNX_EXPORT ProgrammingMode final
{
public:
    ProgrammingMode();
    ~ProgrammingMode();

    ProgrammingMode(const ProgrammingMode &other);
    ProgrammingMode &operator=(const ProgrammingMode &other);

    ProgrammingMode &setMandatory(bool value = true);

    QKnxNetIpSrp create() const;

private:
    class ProgrammingModePrivate;
    QSharedDataPointer<ProgrammingModePrivate> d_ptr;
};

class Q_KNX_EXPORT MacAddress final
{
public:
    MacAddress();
    ~MacAddress();

    MacAddress(const MacAddress &other);
    MacAddress &operator=(const MacAddress &other);

    MacAddress &setMac(const QKnxByteArray &macAdd);
    MacAddress &setMandatory(bool value = true);

    QKnxNetIpSrp create() const;

private:
    class MacAddressPrivate;
    QSharedDataPointer<MacAddressPrivate> d_ptr;
};

class Q_KNX_EXPORT SupportedFamily final
{
public:
    SupportedFamily();
    ~SupportedFamily();

    SupportedFamily(const SupportedFamily &other);
    SupportedFamily &operator=(const SupportedFamily &other);

    SupportedFamily &setServiceInfos(const QVector<QKnxServiceInfo> &infos);
    SupportedFamily &setMandatory(bool value = true);

    QKnxNetIpSrp create() const;

private:
    class SupportedFamilyPrivate;
    QSharedDataPointer<SupportedFamilyPrivate> d_ptr;
};

class Q_KNX_EXPORT RequestDibs final
{
public:
    RequestDibs();
    ~RequestDibs();

    RequestDibs(const RequestDibs &other);
    RequestDibs &operator=(const RequestDibs &other);

    RequestDibs &setDescriptionTypes(const QVector<QKnxNetIp::DescriptionType> &types);
    RequestDibs &setMandatory(bool value = true);

    QKnxNetIpSrp create() const;

private:
    class RequestDibsPrivate;
    QSharedDataPointer<RequestDibsPrivate> d_ptr;
};

}

QT_END_NAMESPACE

#endif

