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

#include "qknxdatapointtype.h"
#include "qknxdatapointtype_p.h"

QT_BEGIN_NAMESPACE

QKnxDatapointType::Type QKnxDatapointType::type() const
{
    return static_cast<Type> (d_ptr->m_type);
}

QKnxDatapointType::~QKnxDatapointType()
{}

QKnxDatapointType::QKnxDatapointType(Type type, int size)
    : d_ptr(new QKnxDatapointTypePrivate)
{
    auto number = QString::number(int(type));
    if (number.size() < 6)
        return;

    bool okMain = false, okSub = false;

    // Datapoint Type shall be identified by a 16 bit main number separated
    // by a dot from a 16 bit subnumber. The assumption being made is that
    // QKnxDatapointType::Type is encoded in that way while omitting the dot.
    int mainType = number.left(number.size() - 5).toInt(&okMain);
    int subType = number.right(5).toInt(&okSub);

    if (okMain && okSub)
        d_ptr->setup(mainType, subType, quint32(type), size);
}

QKnxDatapointType::QKnxDatapointType(const QString &dptId, int size)
    : d_ptr(new QKnxDatapointTypePrivate)
{
    auto match = d_ptr->m_dpt.match(dptId);
    if (!match.hasMatch())
        return;

    QString subType = QStringLiteral("0");
    auto mainType = match.captured(QStringLiteral("MainOnly"));
    if (mainType.isEmpty()) {
        subType = match.captured(QStringLiteral("SubType"));
        mainType = match.captured(QStringLiteral("MainType"));
    }

    quint32 tmp;
    if (QKnxDatapointTypePrivate::toType(mainType, subType, &tmp))
        d_ptr->setup(mainType.toUInt(), subType.toUInt(), tmp, size);
}

QKnxDatapointType::QKnxDatapointType(quint16 mainType, quint16 subType, int size)
    : d_ptr(new QKnxDatapointTypePrivate)
{
    quint32 tmp;
    if (QKnxDatapointTypePrivate::toType(mainType, subType, &tmp))
        d_ptr->setup(mainType, subType, tmp, size);
}

int QKnxDatapointType::size() const
{
    return d_ptr->m_bytes.size();
}

int QKnxDatapointType::subType() const
{
    return d_ptr->m_subType;
}

int QKnxDatapointType::mainType() const
{
    return d_ptr->m_mainType;
}

QVariant QKnxDatapointType::minimum() const
{
    return d_ptr->m_minimum;
}

void QKnxDatapointType::setMinimum(const QVariant &minimum)
{
    d_ptr->m_minimum = minimum;
}

QVariant QKnxDatapointType::maximum() const
{
    return d_ptr->m_maximum;
}

void QKnxDatapointType::setMaximum(const QVariant &maximum)
{
    d_ptr->m_maximum = maximum;
}

QString QKnxDatapointType::minimumText() const
{
    return d_ptr->m_minimumText;
}

void QKnxDatapointType::setMinimumText(const QString &minimumText)
{
    d_ptr->m_minimumText = minimumText;
}

QString QKnxDatapointType::maximumText() const
{
    return d_ptr->m_maximumText;
}

void QKnxDatapointType::setMaximumText(const QString &maximumText)
{
    d_ptr->m_maximumText = maximumText;
}

void QKnxDatapointType::setRange(const QVariant &minimum, const QVariant &maximum)
{
    d_ptr->m_minimum = minimum;
    d_ptr->m_maximum = maximum;
}

void QKnxDatapointType::setRangeText(const QString &minimumText, const QString &maximumText)
{
    d_ptr->m_minimumText = minimumText;
    d_ptr->m_maximumText = maximumText;
}

QString QKnxDatapointType::unit() const
{
    return d_ptr->m_unit;
}

void QKnxDatapointType::setUnit(const QString &unit)
{
    d_ptr->m_unit = unit;
}

QString QKnxDatapointType::description() const
{
    return d_ptr->m_descrition;
}

void QKnxDatapointType::setDescription(const QString &description)
{
    d_ptr->m_descrition = description;
}

bool QKnxDatapointType::isValid() const
{
    return (d_ptr->m_type != 0) && (d_ptr->m_bytes.size() > 0);
}

QString QKnxDatapointType::toString() const
{
    return QLatin1String(QByteArray((const char*) d_ptr->m_bytes.constData(), d_ptr->m_bytes
        .size()).toHex());
}

quint8 &QKnxDatapointType::operator[](int i)
{
    Q_ASSERT_X(i >= 0 && i < d_ptr->m_bytes.size(), "QKnxDatapointType::operator[]",
        "index out of range");
    return d_ptr->m_bytes[i];
}

const quint8 &QKnxDatapointType::operator[](int i) const
{
    Q_ASSERT_X(i >= 0 && i < d_ptr->m_bytes.size(), "QKnxDatapointType::operator[]",
        "index out of range");
    return d_ptr->m_bytes[i];
}

quint8 *QKnxDatapointType::data()
{
    return d_ptr->m_bytes.data();
}

const quint8 *QKnxDatapointType::data() const
{
    return d_ptr->m_bytes.data();
}

const quint8 *QKnxDatapointType::constData() const
{
    return d_ptr->m_bytes.constData();
}

quint8 QKnxDatapointType::byte(quint16 index) const
{
    return (index < d_ptr->m_bytes.size() ? d_ptr->m_bytes[index] : quint8{});
}

bool QKnxDatapointType::setByte(quint16 index, quint8 bytes)
{
    if (index < d_ptr->m_bytes.size())
        d_ptr->m_bytes[index] = bytes;
    return (index < d_ptr->m_bytes.size());
}

QKnxDatapointType::QKnxDatapointType(const QKnxDatapointType &other)
    : d_ptr(other.d_ptr)
{}

QKnxDatapointType &QKnxDatapointType::operator=(const QKnxDatapointType &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

#ifdef Q_COMPILER_RVALUE_REFS
QKnxDatapointType &QKnxDatapointType::operator=(QKnxDatapointType &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}
#endif

void QKnxDatapointType::swap(QKnxDatapointType &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

bool QKnxDatapointType::operator==(const QKnxDatapointType &other) const
{
    return d_ptr == other.d_ptr
        || (d_ptr->m_subType == other.d_ptr->m_subType
            && d_ptr->m_mainType == other.d_ptr->m_mainType
            && d_ptr->m_bytes == other.d_ptr->m_bytes
            && d_ptr->m_unit == other.d_ptr->m_unit
            && d_ptr->m_descrition == other.d_ptr->m_descrition
            && d_ptr->m_minimum == other.d_ptr->m_minimum
            && d_ptr->m_maximum == other.d_ptr->m_maximum
            && d_ptr->m_minimumText == other.d_ptr->m_minimumText
            && d_ptr->m_maximumText == other.d_ptr->m_maximumText);
}

bool QKnxDatapointType::operator!=(const QKnxDatapointType &other) const
{
    return !operator==(other);
}


// -- private

QKnxDatapointType::QKnxDatapointType(QKnxDatapointTypePrivate &dd)
    : d_ptr(new QKnxDatapointTypePrivate(dd))
{}

#include "moc_qknxdatapointtype.cpp"

QT_END_NAMESPACE
