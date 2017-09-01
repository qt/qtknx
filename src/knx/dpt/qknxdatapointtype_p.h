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

#ifndef QKNXDATAPOINTTYPE_P_H
#define QKNXDATAPOINTTYPE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qregularexpression.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qvariant.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxDatapointTypePrivate : public QSharedData
{
    QKnxDatapointTypePrivate() = default;
    ~QKnxDatapointTypePrivate() = default;

    int m_subType { 0 };
    int m_mainType { 0 };
    quint32 m_type { 0 };
    QVector<quint8> m_bytes;
    QString m_unit, m_descrition;
    QVariant m_minimum, m_maximum;
    QString m_minimumText, m_maximumText;
    const QRegularExpression m_dpt { QStringLiteral("^DPT-(?<MainOnly>\\d{1,5})$"
        "|^(DPST-)?(?<MainType>\\d{1,5})(\\.|-)(?<SubType>\\d{1,5})$"),
            QRegularExpression::CaseInsensitiveOption };

    static bool toType(quint16 main, quint16 sub, quint32 *type) {
        return toType(QString::number(main), QString::number(sub), type);
    }
    static bool toType(const QString &main, const QString &sub, quint32 *type) {
        bool ok = true;
        if (main.isEmpty() || sub.isEmpty())
            return !ok;

        *type = QString(main.rightJustified(5, QLatin1Char('0')) + sub
            .rightJustified(5, QLatin1Char('0'))).toUInt(&ok);
        return ok;
    }
    void setup(quint16 mainType, quint16 subType, quint32 type, int size)
    {
        m_type = type;
        m_bytes.resize(size);
        m_subType = subType;
        m_mainType = mainType;
    }
};

QT_END_NAMESPACE

#endif
