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

#include "qknxdptfactory.h"

QT_BEGIN_NAMESPACE

// Just for testing
QKnxDataPoint QKnxDptFactory::setBits(bool value, QKnxDpt::TypesB1 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(1);
    quint8 temp = 0;
    if (value)
        temp = 1;
    dpt.setByte(0, temp);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_BINARY_INFORMATION"), info.usage);
    return {};
}

QKnxDataPoint QKnxDptFactory::Switch::On = setBits(true, QKnxDpt::TypesB1::Switch);
QKnxDataPoint QKnxDptFactory::Switch::Off = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Switch);
QKnxDataPoint QKnxDptFactory::Bool::False = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Bool);
QKnxDataPoint QKnxDptFactory::Bool::True = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Bool);
QKnxDataPoint QKnxDptFactory::Enable::No = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Enable);
QKnxDataPoint QKnxDptFactory::Enable::Yes = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Enable);
QKnxDataPoint QKnxDptFactory::Ramp::No = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Ramp);
QKnxDataPoint QKnxDptFactory::Ramp::Yes = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Ramp);
QKnxDataPoint QKnxDptFactory::Alarm::No = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Alarm);
QKnxDataPoint QKnxDptFactory::Alarm::Yes = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Alarm);
QKnxDataPoint QKnxDptFactory::BinaryValue::Low = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::BinaryValue);
QKnxDataPoint QKnxDptFactory::BinaryValue::High = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::BinaryValue);
QKnxDataPoint QKnxDptFactory::Step::Decrease = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Step);
QKnxDataPoint QKnxDptFactory::Step::Increase = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Step);
QKnxDataPoint QKnxDptFactory::Motion::Up = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::UpDown);
QKnxDataPoint QKnxDptFactory::Motion::Down = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::UpDown);
QKnxDataPoint QKnxDptFactory::Action::Open = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::OpenClose);
QKnxDataPoint QKnxDptFactory::Action::Close = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::OpenClose);
QKnxDataPoint QKnxDptFactory::Action::Stop = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Start);
QKnxDataPoint QKnxDptFactory::Action::Start = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Start);
QKnxDataPoint QKnxDptFactory::State::Inactive = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::State);
QKnxDataPoint QKnxDptFactory::State::Active = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::State);
QKnxDataPoint QKnxDptFactory::Inverted::No = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Invert);
QKnxDataPoint QKnxDptFactory::Inverted::Yes = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Invert);
QKnxDataPoint QKnxDptFactory::DimingStyle::StartStop = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::DimSendStyle);
QKnxDataPoint QKnxDptFactory::DimingStyle::Cyclical = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::DimSendStyle);
QKnxDataPoint QKnxDptFactory::InputSource::Fixed = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::InputSource);
QKnxDataPoint QKnxDptFactory::InputSource::Calculated = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::InputSource);
QKnxDataPoint QKnxDptFactory::Reset::No = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Reset);
QKnxDataPoint QKnxDptFactory::Reset::Yes = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Reset);
QKnxDataPoint QKnxDptFactory::Acknowledge::No = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Ack);
QKnxDataPoint QKnxDptFactory::Acknowledge::YesTrigger = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Ack);
QKnxDataPoint QKnxDptFactory::Trigger::Firering = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Trigger);
QKnxDataPoint QKnxDptFactory::Trigger::FireringAlso = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Trigger);
QKnxDataPoint QKnxDptFactory::Occupied::No = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::Occupancy);
QKnxDataPoint QKnxDptFactory::Occupied::Yes = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::Occupancy);
QKnxDataPoint QKnxDptFactory::WindowDoor::Closed = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::WindowDoor);
QKnxDataPoint QKnxDptFactory::WindowDoor::Open = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::WindowDoor);
QKnxDataPoint QKnxDptFactory::LogicalFunction::Or = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::LogicalFunction);
QKnxDataPoint QKnxDptFactory::LogicalFunction::And = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::LogicalFunction);
QKnxDataPoint QKnxDptFactory::SelectScene::A = QKnxDptFactory::SimpleBinary::setBits(false,
    QKnxDpt::TypesB1::SceneAB);
QKnxDataPoint QKnxDptFactory::SelectScene::B = QKnxDptFactory::SimpleBinary::setBits(true,
    QKnxDpt::TypesB1::SceneAB);
QKnxDataPoint QKnxDptFactory::ShutterBlindsMode::Shutter = QKnxDptFactory::SimpleBinary::setBits(
    false, QKnxDpt::TypesB1::ShutterBlindsMode);
QKnxDataPoint QKnxDptFactory::ShutterBlindsMode::Blind = QKnxDptFactory::SimpleBinary::setBits(
    true, QKnxDpt::TypesB1::ShutterBlindsMode);

bool QKnxDptFactory::SimpleBinary::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1 || QKnxUtils::QUint8::fromBytes(bytes) > 1)
        return false;
    return true;
}

QKnxDptFactory::SimpleBinary::Content QKnxDptFactory::SimpleBinary::getContent(
    const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    Content content;
    content.contentIsValid = true;
    if (QKnxUtils::QUint8::fromBytes(bytes) == 1)
        content.value = true;
    return content;
}

QKnxDataPoint QKnxDptFactory::SimpleBinary::setBits(bool value, QKnxDpt::TypesB1 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(1);
    dpt.setMainType(QKnxDpt::Types::B1);
    quint8 temp = 0;
    if (value)
        temp = 1;
    dpt.setByte(0, temp);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_BINARY_INFORMATION"), info.usage);
    return dpt;
}

QKnxDataPoint QKnxDptFactory::SwitchOn::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Switch);
QKnxDataPoint QKnxDptFactory::SwitchOn::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Switch);
QKnxDataPoint QKnxDptFactory::SwitchOff::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Switch);
QKnxDataPoint QKnxDptFactory::SwitchOff::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Switch);
QKnxDataPoint QKnxDptFactory::BoolTrue::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Bool);
QKnxDataPoint QKnxDptFactory::BoolTrue::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Bool);
QKnxDataPoint QKnxDptFactory::BoolFalse::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Bool);
QKnxDataPoint QKnxDptFactory::BoolFalse::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Bool);
QKnxDataPoint QKnxDptFactory::EnableNo::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Enable);
QKnxDataPoint QKnxDptFactory::EnableNo::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Enable);
QKnxDataPoint QKnxDptFactory::EnableYes::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Enable);
QKnxDataPoint QKnxDptFactory::EnableYes::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Enable);
QKnxDataPoint QKnxDptFactory::RampNo::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Ramp);
QKnxDataPoint QKnxDptFactory::RampNo::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Ramp);
QKnxDataPoint QKnxDptFactory::RampYes::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Ramp);
QKnxDataPoint QKnxDptFactory::RampYes::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Ramp);
QKnxDataPoint QKnxDptFactory::AlarmNo::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Alarm);
QKnxDataPoint QKnxDptFactory::AlarmNo::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Alarm);
QKnxDataPoint QKnxDptFactory::AlarmYes::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Alarm);
QKnxDataPoint QKnxDptFactory::AlarmYes::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Alarm);
QKnxDataPoint QKnxDptFactory::BinaryValueLow::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::BinaryValue);
QKnxDataPoint QKnxDptFactory::BinaryValueLow::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::BinaryValue);
QKnxDataPoint QKnxDptFactory::BinaryValueHigh::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::BinaryValue);
QKnxDataPoint QKnxDptFactory::BinaryValueHigh::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::BinaryValue);
QKnxDataPoint QKnxDptFactory::StepDecrease::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Step);
QKnxDataPoint QKnxDptFactory::StepDecrease::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Step);
QKnxDataPoint QKnxDptFactory::StepIncrease::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Step);
QKnxDataPoint QKnxDptFactory::StepIncrease::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Step);
QKnxDataPoint QKnxDptFactory::MotionUp::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Direction1);
QKnxDataPoint QKnxDptFactory::MotionUp::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Direction1);
QKnxDataPoint QKnxDptFactory::MotionDown::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Direction1);
QKnxDataPoint QKnxDptFactory::MotionDown::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Direction1);
QKnxDataPoint QKnxDptFactory::ActionOpen::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Direction2);
QKnxDataPoint QKnxDptFactory::ActionOpen::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Direction2);
QKnxDataPoint QKnxDptFactory::ActionClose::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Direction2);
QKnxDataPoint QKnxDptFactory::ActionClose::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Direction2);
QKnxDataPoint QKnxDptFactory::ActionStop::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Start);
QKnxDataPoint QKnxDptFactory::ActionStop::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Start);
QKnxDataPoint QKnxDptFactory::ActionStart::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Start);
QKnxDataPoint QKnxDptFactory::ActionStart::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Start);
QKnxDataPoint QKnxDptFactory::StateInactive::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::State);
QKnxDataPoint QKnxDptFactory::StateInactive::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::State);
QKnxDataPoint QKnxDptFactory::StateActive::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::State);
QKnxDataPoint QKnxDptFactory::StateActive::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::State);
QKnxDataPoint QKnxDptFactory::InvertedNo::ControlOff = setBits(false, Status::Off, QKnxDpt::TypesB2::Invert);
QKnxDataPoint QKnxDptFactory::InvertedNo::ControlOn = setBits(false, Status::On, QKnxDpt::TypesB2::Invert);
QKnxDataPoint QKnxDptFactory::InvertedYes::ControlOff = setBits(true, Status::Off, QKnxDpt::TypesB2::Invert);
QKnxDataPoint QKnxDptFactory::InvertedYes::ControlOn = setBits(true, Status::On, QKnxDpt::TypesB2::Invert);

bool QKnxDptFactory::Control::validate(const QByteArray &bytes)
{
    return (bytes.size() == 1) && (quint8(bytes[0]) <= 3);
}

QKnxDptFactory::Control::Content QKnxDptFactory::Control::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};

    Content content;
    content.contentIsValid = true;

    quint8 temp = quint8(bytes[0]);
    if (temp > 1)
        content.control = Status::On;
    else
        content.control = Status::Off;
    if (temp == 1 || temp == 3)
        content.value = true;
    return content;
}

QKnxDataPoint QKnxDptFactory::Control::setBits(bool lowBit, Status control, QKnxDpt::TypesB2 type)
{
    if (control == Status::Unknown)
        return {};

    bool highBit = true;
    if (control == Status::Off)
        highBit = false;

    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(2);
    dpt.setMainType(QKnxDpt::Types::B2);
    quint8 temp = 0;
    if (lowBit)
        temp = 1;
    if (highBit)
        temp |= (1 << 1);
    dpt.setByte(0, temp);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_GENERIC_01"), info.usage);
    return dpt;
}

bool QKnxDptFactory::TypeB1U3::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1 || QKnxUtils::QUint8::fromBytes(bytes) > 15)
        return false;
    return true;
}

QKnxDptFactory::TypeB1U3::Content QKnxDptFactory::TypeB1U3::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    quint8 temp = QKnxUtils::QUint8::fromBytes(bytes);
    Content content;
    content.contentIsValid = true;
    content.stepCode = (temp & 0x07);
    quint8 binary = (temp & 0x08) >> 3;
    if (binary == 1)
        content.value = true;
    content.numberOfInterval = quint8(qPow(2, content.stepCode - 1));
    return content;
}

QKnxDataPoint QKnxDptFactory::TypeB1U3::setBits(Content content,
    QKnxDpt::TypesB1U3 type)
{
    return setBits(content.value, content.numberOfInterval, type);
}

QKnxDataPoint QKnxDptFactory::TypeB1U3::setBits(bool highBit, quint8 stepCode,
    QKnxDpt::TypesB1U3 type)
{
    if (stepCode > 7)
        return {};
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(QKnxDpt::Types::B1U3));
    dpt.setMainType(QKnxDpt::Types::B1U3);
    if (highBit)
        stepCode |= (1 << 3);
    dpt.setByte(0, stepCode);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_GENERIC_01"), info.usage);
    return dpt;
}

QKnxDataPoint QKnxDptFactory::ControlDimming::increaseWithNumberOfInternal(quint8 stepCode)
{
    if (stepCode == 0)
        return {};
    return setBits(1, stepCode, QKnxDpt::TypesB1U3::Dimming);
}

QKnxDataPoint QKnxDptFactory::ControlDimming::decreaseWithNumberOfInternal(quint8 stepCode)
{
    if (stepCode == 0)
        return {};
    return setBits(false, stepCode, QKnxDpt::TypesB1U3::Dimming);
}

QKnxDataPoint QKnxDptFactory::ControlDimming::BreakDecrease =
QKnxDptFactory::ControlDimming::setBits(false, 0, QKnxDpt::TypesB1U3::Dimming);

QKnxDataPoint QKnxDptFactory::ControlDimming::BreakIncrease =
QKnxDptFactory::ControlDimming::setBits(true, 0, QKnxDpt::TypesB1U3::Dimming);

QKnxDataPoint QKnxDptFactory::ControlBlinds::moveUpWithNumberOfInternal(quint8 numberInterval)
{
    if (numberInterval == 0)
        return {};
    return setBits(false, numberInterval, QKnxDpt::TypesB1U3::Blinds);
}

QKnxDataPoint QKnxDptFactory::ControlBlinds::moveDownWithNumberOfInternal(quint8 numberInterval)
{
    if (numberInterval == 0)
        return {};
    return setBits(true, numberInterval, QKnxDpt::TypesB1U3::Blinds);
}

QKnxDataPoint QKnxDptFactory::ControlBlinds::BreakMoveDown =
QKnxDptFactory::ControlBlinds::setBits(true, 0, QKnxDpt::TypesB1U3::Blinds);

QKnxDataPoint QKnxDptFactory::ControlBlinds::BreakMoveUp =
QKnxDptFactory::ControlBlinds::setBits(false, 0, QKnxDpt::TypesB1U3::Blinds);

bool QKnxDptFactory::TypeU8::validate(const QByteArray &bytes, QKnxDpt::TypesU8 type)
{
    if (bytes.size() != 1)
        return false;
    // DPT_Tariff 255 is reserved and shall not be used
    if (type == QKnxDpt::TypesU8::Tariff && QKnxUtils::QUint8::fromBytes(bytes) > 254)
        return false;
    return true;
}

QKnxDptFactory::TypeU8::Content QKnxDptFactory::TypeU8::getContent(const QByteArray &bytes,
    QKnxDpt::TypesU8 type)
{
    Content content;
    if (!validate(bytes, type))
        return content;
    content.value = QVariant(QKnxUtils::QUint8::fromBytes(bytes));
    content.contentIsValid = true;
    return content;
}

QKnxDataPoint QKnxDptFactory::Scaling::setPercent(double percent) // max 100% resolution 0.4%
{
    if (percent > 100 || percent < 0)
        return {};
    quint8 value = quint8(qRound(percent*2.55));
    return setBits(value, QKnxDpt::TypesU8::Scaling);
}

bool QKnxDptFactory::Scaling::validate(const QByteArray &bytes)
{
    return QKnxDptFactory::TypeU8::validate(bytes, QKnxDpt::TypesU8::Scaling);
}

QKnxDptFactory::Scaling::Content QKnxDptFactory::Scaling::getContent(const QByteArray &bytes)
{
    Content content = QKnxDptFactory::TypeU8::getContent(bytes, QKnxDpt::TypesU8::Angle);
    double temp = quint8(content.value.toInt()) * 100 / 255.;
    content.value = QVariant(temp);
    return content;
}

QKnxDataPoint QKnxDptFactory::Angle::setDegree(double angleDegree)
{
    if (angleDegree > 360 || angleDegree < 0)
        return {};
    quint8 value = quint8(angleDegree * 255. / 360);
    return setBits(value, QKnxDpt::TypesU8::Angle);
}

bool QKnxDptFactory::Angle::validate(const QByteArray &bytes)
{
    return QKnxDptFactory::TypeU8::validate(bytes, QKnxDpt::TypesU8::Angle);
}

QKnxDptFactory::Angle::Content QKnxDptFactory::Angle::getContent(const QByteArray &bytes)
{
    Content content = QKnxDptFactory::TypeU8::getContent(bytes, QKnxDpt::TypesU8::Angle);
    double temp = quint8(content.value.toInt()) * 360 / 255.;
    content.value = QVariant(temp);
    return content;
}

QKnxDataPoint QKnxDptFactory::PercentUnsigned::setPercent(quint8 percent) // max 255% resolution 1%
{
    return setBits(percent, QKnxDpt::TypesU8::Percent);
}

bool QKnxDptFactory::PercentUnsigned::validate(const QByteArray &bytes)
{
    return QKnxDptFactory::TypeU8::validate(bytes, QKnxDpt::TypesU8::Percent);
}

QKnxDptFactory::PercentUnsigned::Content QKnxDptFactory::PercentUnsigned::getContent(
    const QByteArray &bytes)
{
    return QKnxDptFactory::TypeU8::getContent(bytes, QKnxDpt::TypesU8::Percent);
}

QKnxDataPoint QKnxDptFactory::DecimalFactor::setRatio(double ratio)
{
    if (ratio > 1 || ratio < 0)
        return {};
    quint8 value = quint8(ratio * 255);
    return setBits(value, QKnxDpt::TypesU8::DecimalFactor);
}

bool QKnxDptFactory::DecimalFactor::validate(const QByteArray &bytes)
{
    return QKnxDptFactory::TypeU8::validate(bytes, QKnxDpt::TypesU8::DecimalFactor);
}

QKnxDptFactory::DecimalFactor::Content QKnxDptFactory::DecimalFactor::getContent(
    const QByteArray &bytes)
{
    Content content = QKnxDptFactory::TypeU8::getContent(bytes, QKnxDpt::TypesU8::Angle);
    double temp = quint8(content.value.toInt()) / 255.;
    content.value = QVariant(temp);
    return content;
}

QKnxDataPoint QKnxDptFactory::Uint8::setValue(quint8 value)
{
    return setBits(value, QKnxDpt::TypesU8::Value);
}

bool QKnxDptFactory::Uint8::validate(const QByteArray &bytes)
{
    return QKnxDptFactory::TypeU8::validate(bytes, QKnxDpt::TypesU8::Value);
}

QKnxDptFactory::Uint8::Content QKnxDptFactory::Uint8::getContent(const QByteArray &bytes)
{
    return QKnxDptFactory::TypeU8::getContent(bytes, QKnxDpt::TypesU8::Value);
}

QKnxDataPoint QKnxDptFactory::Tariff::setValue(quint8 tariff)
{
    if (tariff > 254) //255 reserved, shall not be used (3/7/2 p27)
        return {};
    return setBits(tariff, QKnxDpt::TypesU8::Tariff);
}

bool QKnxDptFactory::Tariff::validate(const QByteArray &bytes)
{
    return QKnxDptFactory::TypeU8::validate(bytes, QKnxDpt::TypesU8::Tariff);
}

QKnxDptFactory::Tariff::Content QKnxDptFactory::Tariff::getContent(const QByteArray &bytes)
{
    return QKnxDptFactory::TypeU8::getContent(bytes, QKnxDpt::TypesU8::Tariff);
}

QKnxDataPoint QKnxDptFactory::TypeU8::setBits(quint8 value, QKnxDpt::TypesU8 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    dpt.setByte(0, value);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_UNSIGNED_CHAR"),
        info.usage);
    return dpt;
}
bool QKnxDptFactory::TypeV8::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1)
        return false;
    return true;
}

QKnxDptFactory::TypeV8::Content QKnxDptFactory::TypeV8::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};

    Content content;
    content.contentIsValid = true;
    content.value = qint8(QKnxUtils::QUint8::fromBytes(bytes));
    return content;
}

QKnxDataPoint QKnxDptFactory::PercentSigned::setPercent(qint8 percent)
{
    return setBits(percent, QKnxDpt::TypesV8::Percent);
}

QKnxDataPoint QKnxDptFactory::Int8::setValue(qint8 counterPulses)
{
    return setBits(counterPulses, QKnxDpt::TypesV8::Value);
}

QKnxDataPoint QKnxDptFactory::TypeV8::setBits(qint8 value, QKnxDpt::TypesV8 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);

    QKnxDataPoint dpt;
    dpt.setByte(0, quint8(value));
    dpt.setMainType(info.mainType);
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_CHAR"), info.usage);
    return dpt;
}

bool QKnxDptFactory::StatusMode3::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1)
        return false;
    return true;
}

bool QKnxDptFactory::StatusMode3::validate(Content content)
{
    if (content.A == Status::Unknown || content.B == Status::Unknown
        || content.C == Status::Unknown || content.D == Status::Unknown
        || content.E == Status::Unknown || content.mode == ActiveMode::Unknown)
        return false;
    return true;
}

QKnxDptFactory::StatusMode3::Content QKnxDptFactory::StatusMode3::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    quint8 temp = QKnxUtils::QUint8::fromBytes(bytes);
    Content content;
    quint8 tempA = (temp & 0x80) >> 7;
    quint8 tempB = (temp & 0x40) >> 6;
    quint8 tempC = (temp & 0x20) >> 5;
    quint8 tempD = (temp & 0x10) >> 4;
    quint8 tempE = (temp & 0x8) >> 3;
    quint8 tempMode = (temp & 0x7);

    switch (tempMode) {
    case 1:
        content.mode = ActiveMode::M0;
        break;
    case 2:
        content.mode = ActiveMode::M1;
        break;
    case 4:
        content.mode = ActiveMode::M2;
        break;
    default:
        content.mode = ActiveMode::Unknown;
        break;
    }

    if (tempA == 1)
        content.A = Status::Clear;
    else if (tempA == 0)
        content.A = Status::Set;
    if (tempB == 1)
        content.B = Status::Clear;
    else if (tempB == 0)
        content.B = Status::Set;
    if (tempC == 1)
        content.C = Status::Clear;
    else if (tempC == 0)
        content.C = Status::Set;
    if (tempD == 1)
        content.D = Status::Clear;
    else if (tempD == 0)
        content.D = Status::Set;
    if (tempE == 1)
        content.E = Status::Clear;
    else if (tempE == 0)
        content.E = Status::Set;
    return content;
}

QKnxDataPoint QKnxDptFactory::StatusMode3::setBits(Content content)
{
    if (!validate(content))
        return {};
    QKnxDpt::TypesB5N3 type = QKnxDpt::TypesB5N3::StatusMode3;
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);

    quint8 temp = 0;
    switch (content.mode) {
    case ActiveMode::M0:
        temp = 1;
        break;
    case ActiveMode::M1:
        temp = 2;
        break;
    case ActiveMode::M2:
        temp = 4;
        break;
    case ActiveMode::Unknown:
        return {};
    default:
        break;
    }

    if (content.A == Status::Clear)
        temp |= 1 << 7;
    if (content.B == Status::Clear)
        temp |= 1 << 6;
    if (content.C == Status::Clear)
        temp |= 1 << 5;
    if (content.D == Status::Clear)
        temp |= 1 << 4;
    if (content.E == Status::Clear)
        temp |= 1 << 3;

    dpt.setByte(0, temp);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_GENERIC_01"), info.usage);
    return dpt;
}

QKnxDptFactory::TypeU16::Content QKnxDptFactory::TypeU16::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    Content content;
    content.value = QVariant(QKnxUtils::QUint16::fromBytes(bytes));
    content.contentIsValid = true;
    return content;
}

bool QKnxDptFactory::TypeU16::validate(const QByteArray &bytes)
{
    if (bytes.size() != 2)
        return false;
    return true;
}

QKnxDataPoint QKnxDptFactory::TypeU16::setBits(quint16 value, QKnxDpt::TypesU16 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    dpt.setBytes(QKnxUtils::QUint16::bytes(value));
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_UNSIGNED_INT"), info.usage);
    return dpt;
}

QKnxDataPoint QKnxDptFactory::Uint16::setValue(quint16 pulse)
{
    return setBits(pulse, QKnxDpt::TypesU16::Value);
}

QKnxDataPoint QKnxDptFactory::PropertyDataType::setValue(quint16 value)
{
    return setBits(value, QKnxDpt::TypesU16::PropertyDataType);
}

QKnxDataPoint QKnxDptFactory::TimePeriodMilliSec::setValue(quint16 periodMs)
{
    return setBits(periodMs, QKnxDpt::TypesU16::TimePeriodMilliSec);
}

QKnxDataPoint QKnxDptFactory::TimePeriod10MilliSec::setValue(quint32 periodMs)
{
    if (periodMs / 10. > 65535) {
        qDebug() << "Please try using setTimePeriod100Msec";
        return {};
    }
    return setBits(quint16(qRound(periodMs / 10.)), QKnxDpt::TypesU16::TimePeriod10MilliSec);
}

QKnxDptFactory::TimePeriod10MilliSec::Content
QKnxDptFactory::TimePeriod10MilliSec::getContent(const QByteArray &bytes)
{
    Content content = QKnxDptFactory::TypeU16::getContent(bytes);
    quint32 value = content.value.toUInt();
    content.value = QVariant(value * 10);
    return content;
}

QKnxDataPoint QKnxDptFactory::TimePeriod100MilliSec::setValue(quint32 periodMs)
{
    if (periodMs / 100. > 65535) {
        qDebug() << "Please try using setTimePeriodSec";
        return {};
    }
    return setBits(quint16(qRound(periodMs / 100.)), QKnxDpt::TypesU16::TimePeriod100MilliSec);
}

QKnxDptFactory::TimePeriod100MilliSec::Content
QKnxDptFactory::TimePeriod100MilliSec::getContent(const QByteArray &bytes)
{
    Content content = QKnxDptFactory::TypeU16::getContent(bytes);
    quint32 value = content.value.toUInt();
    content.value = QVariant(value * 100);
    return content;
}

QKnxDataPoint QKnxDptFactory::TimePeriodSec::setValue(quint16 periodSec)
{
    return setBits(periodSec, QKnxDpt::TypesU16::TimePeriodSec);
}

QKnxDataPoint QKnxDptFactory::TimePeriodMin::setValue(quint16 periodMin)
{
    return setBits(periodMin, QKnxDpt::TypesU16::TimePeriodMin);
}

QKnxDataPoint QKnxDptFactory::TimePeriodHrs::setValue(quint16 periodHrs)
{
    return setBits(periodHrs, QKnxDpt::TypesU16::TimePeriodHrs);
}

QKnxDataPoint QKnxDptFactory::Length::setValue(quint16 value)
{
    return setBits(value, QKnxDpt::TypesU16::Length);
}

QKnxDataPoint QKnxDptFactory::UEICurrent::setValue(quint16 value)
{
    return setBits(value, QKnxDpt::TypesU16::UEICurrent);
}

QKnxDataPoint QKnxDptFactory::Brightness::setValue(quint16 lux)
{
    return setBits(lux, QKnxDpt::TypesU16::Brightness);
}

QKnxDptFactory::TypeV16::Content QKnxDptFactory::TypeV16::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    Content content;
    content.value = QVariant(qint16(QKnxUtils::QUint16::fromBytes(bytes)));
    content.contentIsValid = true;
    return content;
}

bool QKnxDptFactory::TypeV16::validate(const QByteArray &bytes)
{
    if (bytes.size() != 2)
        return false;
    return true;
}

QKnxDataPoint QKnxDptFactory::TypeV16::setBits(qint16 value, QKnxDpt::TypesV16 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    dpt.setBytes(QKnxUtils::QUint16::bytes(quint16(value)));
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_INT"), info.usage);
    return dpt;
}

QKnxDataPoint QKnxDptFactory::Int16::setValue(qint16 pulse)
{
    return setBits(pulse, QKnxDpt::TypesV16::Value);
}

QKnxDataPoint QKnxDptFactory::PercentSigned16::setValue(double percent)
{
    if (percent > 327.67 || percent < -327.68)
        return {};
    return setBits(qint16(percent * 100), QKnxDpt::TypesV16::Percent);
}

QKnxDptFactory::PercentSigned16::Content QKnxDptFactory::PercentSigned16::getContent(
    const QByteArray &bytes)
{
    Content content = QKnxDptFactory::TypeV16::getContent(bytes);
    double percent = content.value.toInt() / 100.;
    content.value = QVariant(percent);
    return content;
}

QKnxDataPoint QKnxDptFactory::DeltaTimeMilliSec::setValue(qint16 deltaMs)
{
    return setBits(deltaMs, QKnxDpt::TypesV16::DeltaTimeMilliSec);
}

QKnxDptFactory::DeltaTime10MilliSec::Content
QKnxDptFactory::DeltaTime10MilliSec::getContent(const QByteArray &bytes)
{
    Content content = QKnxDptFactory::TypeV16::getContent(bytes);
    qint32 value = content.value.toInt() * 10;
    content.value = QVariant(value);
    return content;
}

QKnxDataPoint QKnxDptFactory::DeltaTime10MilliSec::setValue(qint32 deltaMs)
{
    if (qRound(deltaMs / 10.) > 32767 || qRound(deltaMs / 10.) < -32768) {
        qDebug() << "Please try using setTimePeriod100Msec";
        return {};
    }
    return setBits(qint16(deltaMs / 10), QKnxDpt::TypesV16::DeltaTime10MilliSec);
}

QKnxDptFactory::DeltaTime100MilliSec::Content
QKnxDptFactory::DeltaTime100MilliSec::getContent(const QByteArray &bytes)
{
    Content content = QKnxDptFactory::TypeV16::getContent(bytes);
    qint32 value = content.value.toInt() * 100;
    content.value = QVariant(value);
    return content;
}

QKnxDataPoint QKnxDptFactory::DeltaTime100MilliSec::setValue(qint32 deltaMs)
{
    if (qRound(deltaMs / 100.) > 32767 || qRound(deltaMs / 100.) < -32768) {
        qDebug() << "Please try using setTimePeriodSec";
        return {};
    }
    return setBits(qint16(deltaMs / 100.), QKnxDpt::TypesV16::DeltaTime100MilliSec);
}

QKnxDataPoint QKnxDptFactory::DeltaTimeSec::setValue(qint16 deltaSec)
{
    return setBits(deltaSec, QKnxDpt::TypesV16::DeltaTimeSec);
}

QKnxDataPoint QKnxDptFactory::DeltaTimeMin::setValue(qint16 deltaMin)
{
    return setBits(deltaMin, QKnxDpt::TypesV16::DeltaTimeMin);
}

QKnxDataPoint QKnxDptFactory::DeltaTimeHrs::setValue(qint16 deltaHrs)
{
    return setBits(deltaHrs, QKnxDpt::TypesV16::DeltaTimeHrs);
}

QKnxDataPoint QKnxDptFactory::RotationAngle::setValue(qint16 angleDegree)
{
    return setBits(angleDegree, QKnxDpt::TypesV16::RotationAngle);
}

QKnxDataPoint QKnxDptFactory::TemperatureCelcius::setValue(float value)
{
    if (value < -273 || value > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::TemperatureCelcius);
}

QKnxDataPoint QKnxDptFactory::TemperatureKelvin::setValue(float value)
{
    if (float(qFabs(qreal(value))) > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::TempertureKelvin);
}

QKnxDataPoint QKnxDptFactory::TemperatureChange::setValue(float value)
{
    if (float(qFabs(qreal(value))) > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::TemperatureChangeKelvinPerHrs);
}

QKnxDataPoint QKnxDptFactory::BrightnessFloat::setValue(float value)
{
    if (value < 0 || value > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::Brightness);
}

QKnxDataPoint QKnxDptFactory::SpeedMeterPerSec::setValue(float value)
{
    if (value < 0 || value > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::SpeedMeterPerSec);
}

QKnxDataPoint QKnxDptFactory::Pression::setValue(float value)
{
    if (value < 0 || value > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::Pression);
}

QKnxDataPoint QKnxDptFactory::Humidity::setValue(float value)
{
    if (value < 0 || value > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::Humidity);
}

QKnxDataPoint QKnxDptFactory::AirQuality::setValue(float value)
{
    if (value < 0 || value > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::AirQuality);
}

QKnxDataPoint QKnxDptFactory::TimeSec::setValue(float value)
{
    if (float(qFabs(qreal(value))) > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::TimeSec);
}

QKnxDataPoint QKnxDptFactory::TimeMilliSec::setValue(float value)
{
    if (float(qFabs(qreal(value))) > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::TimeMilliSec);
}

QKnxDataPoint QKnxDptFactory::Voltage::setValue(float value)
{
    if (float(qFabs(qreal(value))) > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::Voltage);
}

QKnxDataPoint QKnxDptFactory::Current::setValue(float value)
{
    if (float(qFabs(qreal(value))) > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::Current);
}

QKnxDataPoint QKnxDptFactory::PowerDensity::setValue(float value)
{
    if (float(qFabs(qreal(value))) > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::PowerDensity);
}

QKnxDataPoint QKnxDptFactory::KelvinPerPercent::setValue(float value)
{
    if (float(qFabs(qreal(value))) > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::KelvinPerPercent);
}

QKnxDataPoint QKnxDptFactory::Power::setValue(float value)
{
    if (value < -671088.64f || value > 670760.96f)//kW
        return {};
    return setBits(value, QKnxDpt::TypesF16::Power);
}

QKnxDataPoint QKnxDptFactory::VolumeFlow::setValue(float value)
{
    if (float(qFabs(qreal(value))) > 670760)
        return {};
    return setBits(value, QKnxDpt::TypesF16::VolumeFlow);
}

QKnxDataPoint QKnxDptFactory::RainAmount::setValue(float value)
{
    if (value < -671088.64f || value > 670760.96f)
        return {};
    return setBits(value, QKnxDpt::TypesF16::RainAmount);
}

QKnxDataPoint QKnxDptFactory::TemperatureFahrenheit::setValue(float value)
{
    if (value < -459.6f || value > 670760.96f)
        return {};
    return setBits(value, QKnxDpt::TypesF16::TemperatureFahrenheit);
}

QKnxDataPoint QKnxDptFactory::SpeedKiloMeterPerHrs::setValue(float value)
{
    if (value < 0 || value > 670760.96f)
        return {};
    return setBits(value, QKnxDpt::TypesF16::SpeedKiloMeterPerHrs);
}

QKnxDataPoint QKnxDptFactory::TypeF16::setBits(float value, QKnxDpt::TypesF16 type)
{
    value = float(qRound(value * 100) / 100.);
    if (value > 670760.96f || value < -671088.64f)
        return {};
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    quint8 E = 0;
    if (qAbs(qreal(value)) > 20.48)
        E = quint8(qFloor(qLn(qAbs(qreal(value) * 100 / 2048.)) / qLn(2) + 1));
    qint32 M = qint32(qRound((value*float(qPow(2, -E)) * 100)));
    if (E > 15 || M > 2047 || M < -2048)
        return {};// should never happen considering the ranges of value.
    quint16 encodedM = quint16(M);
    if (value < 0)
        encodedM &= 0x87ff;
    encodedM |= E << 11;
    dpt.setBytes(QKnxUtils::QUint16::bytes(encodedM));
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_KNX_FLOAT"), info.usage);
    return dpt;
}

bool QKnxDptFactory::TypeF16::validate(QKnxDptFactory::TypeF16::Content content)
{
    float value = content.value;
    if (value < -671088.64f || value > 670760.96f)
        return false;
    return true;
}

QKnxDptFactory::TypeF16::Content QKnxDptFactory::TypeF16::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    Content content;
    quint16 temp = QKnxUtils::QUint16::fromBytes(bytes);
    quint16 encodedM = (temp & 0x87ff);
    // Turning on bits reserved for E.
    // Only needed for reinterpretation of negative values
    if (encodedM > 2047)
        encodedM += 0x7800;

    qint16 M = qint16(encodedM);
    quint8 E = (temp & 0x7800) >> 11;
    float value = float(0.01*(M) *qPow(2, qreal(E)));
    content.value = value;
    content.contentIsValid = validate(content);
    return content;
}

bool QKnxDptFactory::TypeF16::validate(const QByteArray &bytes)
{
    if (bytes.size() != 2)
        return false;
    return true;
}

QKnxDataPoint QKnxDptFactory::TimeOfDay::setTimeOfDay(QKnxDptFactory::TimeOfDay::Content content)
{
    if (!validate(content))
        return {};
    QKnxDpt::TypesN3U5r2U6r2U6 type = QKnxDpt::TypesN3U5r2U6r2U6::TimeOfDay;
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    quint8 temp = content.hour;
    temp |= content.dayOfTheWeek << 5;
    dpt.setByte(0, temp);
    dpt.setByte(1, content.min);
    dpt.setByte(2, content.sec);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_TIME"), info.usage);
    return dpt;
}

QKnxDptFactory::TimeOfDay::Content QKnxDptFactory::TimeOfDay::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    Content content;
    content.sec = quint8(bytes[2]);
    content.min = quint8(bytes[1]);
    content.hour = (quint8(bytes[0]) & 0x1f);
    content.dayOfTheWeek = (quint8(bytes[0]) & 0xe0) >> 5;
    content.contentIsValid = validate(content);
    return content;
}

bool QKnxDptFactory::TimeOfDay::validate(const QByteArray &bytes)
{
    if (bytes.size() != 3)
        return false;
    if (quint8(bytes[1]) > 63 || quint8(bytes[2]) > 63)
        return false;
    return true;
}

bool QKnxDptFactory::TimeOfDay::validate(QKnxDptFactory::TimeOfDay::Content content)
{
    quint8 day = content.dayOfTheWeek;
    quint8 hour = content.hour;
    quint8 min = content.min;
    quint8 sec = content.sec;
    if (day > 7 || hour > 23 || min > 59 || sec > 59)
        return false;
    return true;
}

QKnxDptFactory::Date::Content QKnxDptFactory::Date::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    Content content;
    quint16 year = quint16(bytes[2]);
    if (year > 89)
        year += 1900;
    else
        year += 2000;
    content.year = year;
    content.month = quint8(bytes[1]);
    content.day = (quint8(bytes[0]));
    content.contentIsValid = validate(content);
    return content;
}

bool QKnxDptFactory::Date::validate(const QByteArray &bytes)
{
    if (bytes.size() != 3)
        return false;
    if (quint8(bytes[0]) > 31 || quint8(bytes[1]) > 15 || quint8(bytes[2]) > 127)
        return false;
    return true;
}

bool QKnxDptFactory::Date::validate(QKnxDptFactory::Date::Content content)
{
    quint8 day = content.day;
    quint8 month = content.month;
    quint16 year = content.year;
    if (day < 1 || day > 31 || month < 1 || month > 12 || year > 2089 || year < 1990)
        return false;
    return true;
}

QKnxDataPoint QKnxDptFactory::Date::setDate(Content content)
{
    if (!validate(content))
        return {};
    QKnxDpt::Typesr3U5r4U4r1U7 type = QKnxDpt::Typesr3U5r4U4r1U7::Date;
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    quint16 year = quint16(content.year);
    if (year > 1999)
        year = year - 2000;
    else
        year = year - 1900;
    dpt.setByte(0, content.day);
    dpt.setByte(1, content.month);
    dpt.setByte(2, quint8(year));
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_DATE"), info.usage);
    return dpt;
}

QKnxDptFactory::Uint32::Content QKnxDptFactory::Uint32::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};

    Content content;
    content.value = QKnxUtils::QUint32::fromBytes(bytes);
    content.contentIsValid = true;
    return content;
}

bool QKnxDptFactory::Uint32::validate(const QByteArray &bytes)
{
    if (bytes.size() != 4)
        return false;
    return true;
}

QKnxDataPoint QKnxDptFactory::Uint32::setValue(quint32 value)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(QKnxDpt::TypesU32::Value);

    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    dpt.setBytes(QKnxUtils::QUint32::bytes(quint32(value)));
    dpt.setSpecifics(quint8(QKnxDpt::TypesU32::Value), info.typeName,
        QString::fromLatin1("PDT_UNSIGNED_LONG"), info.usage);
    return dpt;
}

QKnxDataPoint QKnxDptFactory::Int32::setValue(qint32 value)
{
    return setBits(value, QKnxDpt::TypesV32::Value);
}

QKnxDataPoint QKnxDptFactory::FlowRate::setValue(qint32 value)
{
    return setBits(value, QKnxDpt::TypesV32::FlowRate);
}

QKnxDataPoint QKnxDptFactory::ActiveEnergyWattHrs::setValue(qint32 value)
{
    return setBits(value, QKnxDpt::TypesV32::ActiveEnergyWattHrs);
}

QKnxDataPoint QKnxDptFactory::ApparentEnergyVoltAmpHrs::setValue(qint32 value)
{
    return setBits(value, QKnxDpt::TypesV32::ApparentEnergyVoltAmpHrs);
}

QKnxDataPoint QKnxDptFactory::ReactiveEnergyVARHrs::setValue(qint32 value)
{
    return setBits(value, QKnxDpt::TypesV32::ReactiveEvnergyVARHrs);
}

QKnxDataPoint QKnxDptFactory::ActiveEnergyKiloWattHrs::setValue(qint32 value)
{
    return setBits(value, QKnxDpt::TypesV32::ActiveEnergyKiloWattHrs);
}

QKnxDataPoint QKnxDptFactory::ApparentEnergyKiloVoltAmpHrs::setValue(qint32 value)
{
    return setBits(value, QKnxDpt::TypesV32::ApparentEnergyKiloVoltAmpHrs);
}

QKnxDataPoint QKnxDptFactory::ReactiveEnergyKiloVARHrs::setValue(qint32 value)
{
    return setBits(value, QKnxDpt::TypesV32::ReactiveEvnergyKiloVARHrs);
}

QKnxDataPoint QKnxDptFactory::DeltaTime::setValue(qint32 value)
{
    return setBits(value, QKnxDpt::TypesV32::DeltaTimeSec);
}

QKnxDataPoint QKnxDptFactory::TypeV32::setBits(qint32 value, QKnxDpt::TypesV32 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);

    QKnxDataPoint dpt;
    dpt.setMainType(info.mainType);
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setBytes(QKnxUtils::QUint32::bytes(value));
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_LONG"), info.usage);
    return dpt;
}

QKnxDptFactory::TypeV32::Content QKnxDptFactory::TypeV32::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};

    Content content;
    content.contentIsValid = true;
    content.value = qint32(QKnxUtils::QUint32::fromBytes(bytes));
    return content;
}

bool QKnxDptFactory::TypeV32::validate(const QByteArray &bytes)
{
    if (bytes.size() != 4)
        return false;
    return true;
}

QKnxDataPoint QKnxDptFactory::TypeF32::setBits(float value, QKnxDpt::TypesF32 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);

    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);

    quint32 tmp;
    memcpy(&tmp, &value, sizeof(value));
    dpt.setBytes(QKnxUtils::QUint32::bytes(tmp));

    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_FLOAT"), info.usage);
    return dpt;
}

QKnxDptFactory::TypeF32::Content QKnxDptFactory::TypeF32::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};

    quint32 value = QKnxUtils::QUint32::fromBytes(bytes);

    Content content;
    content.contentIsValid = true;
    memcpy(&content.value, &value, sizeof(content.value));
    return content;
}

bool QKnxDptFactory::TypeF32::validate(const QByteArray &bytes)
{
    if (bytes.size() != 4)
        return false;
    return true;
}

bool QKnxDptFactory::AccessData::validate(const QByteArray &bytes)
{
    if (bytes.size() != 4)
        return false;
    return true;
}

bool QKnxDptFactory::AccessData::validate(Content content)
{
    if (content.d1 > 9 || content.d2 > 9 || content.d3 > 9 || content.d4 > 9 || content.d5 > 9
        || content.d6 > 9 || content.error == ErrorStatus::Unknown
        || content.permission == PermissionStatus::Unknown
        || content.reading == ReadDirection::Unknown || content.encryption == Encryption::Unknown
        || content.index > 15)
        return false;
    return true;
}

QKnxDptFactory::AccessData::Content QKnxDptFactory::AccessData::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    Content content;
    content.d6 = quint8((quint8(bytes[0]) & 0xf0) >> 4);
    content.d5 = quint8(quint8(bytes[0]) & 0x0f);
    content.d4 = quint8((quint8(bytes[1]) & 0xf0) >> 4);
    content.d3 = quint8(quint8(bytes[1]) & 0x0f);
    content.d2 = quint8((quint8(bytes[2]) & 0xf0) >> 4);
    content.d1 = quint8(quint8(bytes[2]) & 0x0f);
    content.index = quint8(quint8(bytes[3]) & 0x0f);
    if (quint8((quint8(bytes[3]) & 0x80) >> 7) == 1)
        content.error = ErrorStatus::ReadingUnsuccessful;
    else
        content.error = ErrorStatus::NoError;
    if (quint8((quint8(bytes[3]) & 0x40) >> 6) == 1)
        content.permission = PermissionStatus::Accepted;
    else
        content.permission = PermissionStatus::NotAccepted;
    if (quint8((quint8(bytes[3]) & 0x20) >> 5) == 1)
        content.reading = ReadDirection::RightToLeft;
    else
        content.reading = ReadDirection::LeftToRight;
    if (quint8((quint8(bytes[3]) & 0x10) >> 4) == 1)
        content.encryption = Encryption::Yes;
    else
        content.encryption = Encryption::No;
    content.contentIsValid = validate(content);
    return content;
}

QKnxDataPoint QKnxDptFactory::AccessData::setValue(Content content)
{
    if (!validate(content))
        return {};
    QKnxDpt::TypesU4U4U4U4U4U4B4N4 type = QKnxDpt::TypesU4U4U4U4U4U4B4N4::AccessData;
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    quint8 d6d5 = quint8(content.d6 << 4);
    d6d5 |= content.d5;
    quint8 d4d3 = quint8(content.d4 << 4);
    d4d3 |= content.d3;
    quint8 d2d1 = quint8(content.d2 << 4);
    d2d1 |= content.d1;
    quint8 temp = content.index;
    if (content.error == ErrorStatus::ReadingUnsuccessful)
        temp |= quint8(1 << 7);
    if (content.permission == PermissionStatus::Accepted)
        temp |= quint8(1 << 6);
    if (content.reading == ReadDirection::RightToLeft)
        temp |= quint8(1 << 5);
    if (content.encryption == Encryption::Yes)
        temp |= quint8(1 << 4);
    dpt.setByte(0, d6d5);
    dpt.setByte(1, d4d3);
    dpt.setByte(2, d2d1);
    dpt.setByte(3, temp);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_GENERIC_04"),
        info.usage);
    return dpt;
}

QKnxDataPoint QKnxDptFactory::SceneNumber::setValue(quint8 sceneNumber)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(QKnxDpt::Typesr2U6::SceneNumber);
    return setBits(sceneNumber, Control::NotApplicable, Info::NotApplicable, info.mainType,
        info.subType, info.typeName, info.usage);
}

QKnxDataPoint QKnxDptFactory::SceneControl::activate(quint8 sceneNumber)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(QKnxDpt::TypesB1r1U6::SceneControl);
    return setBits(sceneNumber, Control::ActivateScene, Info::NotApplicable, info.mainType,
        info.subType, info.typeName, info.usage);
}

QKnxDataPoint QKnxDptFactory::SceneControl::learn(quint8 sceneNumber)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(QKnxDpt::TypesB1r1U6::SceneControl);
    return setBits(sceneNumber, Control::LearnScene, Info::NotApplicable, info.mainType,
        info.subType, info.typeName, info.usage);
}

QKnxDataPoint QKnxDptFactory::SceneInfo::activeScene(quint8 sceneNumber)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(QKnxDpt::Typesr1B1U6::SceneInfo);
    return setBits(sceneNumber, Control::NotApplicable, Info::ActiveScene, info.mainType,
        info.subType, info.typeName, info.usage);
}

QKnxDataPoint QKnxDptFactory::SceneInfo::inactiveScene(quint8 sceneNumber)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(QKnxDpt::Typesr1B1U6::SceneInfo);
    return setBits(sceneNumber, Control::NotApplicable, Info::InactiveScene, info.mainType,
        info.subType, info.typeName, info.usage);
}

bool QKnxDptFactory::SceneInfo::validate(Content content)
{
    if (content.info == Info::NotApplicable || content.info == Info::Unknown
        || content.control != Control::NotApplicable || content.sceneNumber > 63)
        return false;
    return true;
}

bool QKnxDptFactory::SceneControl::validate(Content content)
{
    if (content.control == Control::NotApplicable || content.control == Control::Unknown
        || content.info != Info::NotApplicable || content.sceneNumber > 63)
        return false;
    return true;
}

bool QKnxDptFactory::SceneNumber::validate(Content content)
{
    if (content.control != Control::NotApplicable || content.info != Info::NotApplicable
        || content.sceneNumber > 63)
        return false;
    return true;
}

bool QKnxDptFactory::SceneNumber::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1)
        return false;
    if (quint8(bytes[0]) > 63)
        return false;
    return true;
}

bool QKnxDptFactory::SceneControl::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1)
        return false;
    if (quint8(bytes[0]) > 191)
        return false;
    return true;
}

bool QKnxDptFactory::SceneInfo::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1)
        return false;
    if (quint8(bytes[0]) > 127)
        return false;
    return true;
}

QKnxDptFactory::Scene::Content QKnxDptFactory::SceneNumber::getContent(const QByteArray &bytes)
{
    if (!QKnxDptFactory::Scene::validate(bytes))
        return {};
    Content content = QKnxDptFactory::Scene::getContent(bytes);
    if (content.control == Control::ActivateScene && content.info == Info::ActiveScene) {
        content.control = Control::NotApplicable;
        content.info = Info::NotApplicable;
    }
    // if Control and Info bits are not 0, this content cannot be a SceneNumber content
    content.contentIsValid = validate(content);
    return content;
}

QKnxDptFactory::Scene::Content QKnxDptFactory::SceneInfo::getContent(const QByteArray &bytes)
{
    if (!QKnxDptFactory::Scene::validate(bytes))
        return {};
    Content content = QKnxDptFactory::Scene::getContent(bytes);
    if (content.control == Control::ActivateScene)
        content.control = Control::NotApplicable;
    // if Control bit is not 0, this content cannot be a SceneInfo content
    content.contentIsValid = validate(content);
    return content;
}

QKnxDptFactory::Scene::Content QKnxDptFactory::SceneControl::getContent(const QByteArray &bytes)
{
    if (!QKnxDptFactory::Scene::validate(bytes))
        return {};
    Content content = QKnxDptFactory::Scene::getContent(bytes);
    if (content.info == Info::ActiveScene)
        content.info = Info::NotApplicable;
    // if Info bit is not 0, this content cannot be a SceneControl content
    content.contentIsValid = validate(content);
    return content;
}

bool QKnxDptFactory::Scene::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1)
        return false;
    return true;
}

QKnxDptFactory::Scene::Content QKnxDptFactory::Scene::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    Content content;

    content.sceneNumber = quint8((quint8(bytes[0]) & 0x3f));
    if (quint8((quint8(bytes[0]) & 0x40) >> 6) == 1)
        content.info = Info::InactiveScene;
    else
        content.info = Info::ActiveScene;
    if (quint8((quint8(bytes[0]) & 0x80) >> 7) == 1)
        content.control = Control::LearnScene;
    else
        content.control = Control::ActivateScene;
    return content;
}

QKnxDataPoint QKnxDptFactory::Scene::setBits(quint8 sceneNumber, Control control, Info info,
    QKnxDpt::Types mainType, quint16 dptSubNumber, const QString &dptName, QKnxDpt::Usage usage)
{
    if (sceneNumber > 63)
        return {};
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(mainType));
    dpt.setMainType(mainType);
    if (control == Control::LearnScene)
        sceneNumber |= quint8(1 << 7);
    if (info == Info::InactiveScene)
        sceneNumber |= quint8(1 << 6);
    dpt.setByte(0, sceneNumber);
    dpt.setSpecifics(quint8(dptSubNumber), dptName, QString::fromLatin1("PDT_GENERIC_01"), usage);
    return dpt;
}

bool QKnxDptFactory::DateTime::validate(const QByteArray &bytes)
{
    if (bytes.size() != 8)
        return false;
    if (quint8(bytes[1]) > 15 || quint8(bytes[2]) > 31 || quint8(bytes[4]) > 63
        || quint8(bytes[5]) > 63 || quint8(quint8(bytes[7]) & 0x7f) != 0)
        return false;
    return true;
}

QKnxDptFactory::DateTime::Content QKnxDptFactory::DateTime::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    Content content;
    content.year = quint8(bytes[0]) + 1900;
    content.month = quint8(quint8(bytes[1]) & 0x0f);
    content.day = quint8(quint8(bytes[2]) & 0x1f);
    content.dayOfWeek = quint8((quint8(bytes[3]) & 0xe0) >> 5);
    content.hourOfDay = quint8(quint8(bytes[3]) & 0x1f);
    content.minutes = quint8(quint8(bytes[4]) & 0x3f);
    content.seconds = quint8(quint8(bytes[5]) & 0x3f);
    if (quint8((quint8(bytes[6]) & 0x80) >> 7) == 1)
        content.fault = Fault::Yes;
    else
        content.fault = Fault::No;
    if (quint8((quint8(bytes[6]) & 0x40) >> 6) == 1)
        content.workingDay = WorkingDay::Yes;
    else
        content.workingDay = WorkingDay::No;
    if (quint8((quint8(bytes[6]) & 0x20) >> 5) == 1)
        content.workingDayField = WorkingDayField::NotValid;
    else
        content.workingDayField = WorkingDayField::Valid;
    if (quint8((quint8(bytes[6]) & 0x10) >> 4) == 1)
        content.yearField = YearField::NotValid;
    else
        content.yearField = YearField::Valid;
    if (quint8((quint8(bytes[6]) & 0x08) >> 3) == 1)
        content.dateField = DateField::NotValid;
    else
        content.dateField = DateField::Valid;
    if (quint8((quint8(bytes[6]) & 0x04) >> 2) == 1)
        content.dayOfWeekField = DayOfWeekField::NotValid;
    else
        content.dayOfWeekField = DayOfWeekField::Valid;
    if (quint8((quint8(bytes[6]) & 0x02) >> 1) == 1)
        content.timeField = TimeField::NotValid;
    else
        content.timeField = TimeField::Valid;
    if (quint8(quint8(bytes[6]) & 0x01) == 1)
        content.summerTime = SummerTime::Yes;
    else
        content.summerTime = SummerTime::No;
    if (quint8(quint8(bytes[7]) & 0x80) == 1)
        content.clockQuality = ClockQuality::WithExtendedSyncSignal;
    else
        content.clockQuality = ClockQuality::WithoutExtendedSyncSignal;

    content.contentIsValid = validate(content);

    return content;
}

bool QKnxDptFactory::DateTime::validate(Content content)
{
    if (content.year > 2155 || content.year < 1900 || content.month < 1 || content.month > 12
        || content.day < 1 || content.day > 31 || content.dayOfWeek > 7 || content.hourOfDay > 24
        || content.minutes > 59 || content.seconds > 59 || content.fault == Fault::Unknown
        || content.workingDay == WorkingDay::Unknown
        || content.workingDayField == WorkingDayField::Unknown
        || content.yearField == YearField::Unknown
        || content.dateField == DateField::Unknown
        || content.dayOfWeekField == DayOfWeekField::Unknown
        || content.timeField == TimeField::Unknown
        || content.summerTime == SummerTime::Unknown
        || content.clockQuality == ClockQuality::Unknown)
        return false;
    return true;
};

QKnxDataPoint QKnxDptFactory::DateTime::setDateTime(Content content)
{
    if (!validate(content))
        return {};
    if (content.hourOfDay == 24) {
        content.minutes = 0;
        content.seconds = 0;
    }

    QKnxDpt::TypesU8r4U4r3U5U3U5r2U6r2U6B16 type =
        QKnxDpt::TypesU8r4U4r3U5U3U5r2U6r2U6B16::DateTime;
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);

    dpt.setByte(0, quint8(content.year - 1900));
    dpt.setByte(1, content.month);
    dpt.setByte(2, content.day);
    quint8 dayHour = content.hourOfDay;
    dayHour |= quint8(content.dayOfWeek << 5);
    dpt.setByte(3, dayHour);
    dpt.setByte(4, content.minutes);
    dpt.setByte(5, content.seconds);
    quint8 flags = 0;
    if (content.fault == Fault::Yes)
        flags |= quint8(1 << 7);
    if (content.workingDay == WorkingDay::Yes)
        flags |= quint8(1 << 6);
    if (content.workingDayField == WorkingDayField::NotValid)
        flags |= quint8(1 << 5);
    if (content.yearField == YearField::NotValid)
        flags |= quint8(1 << 4);
    if (content.dateField == DateField::NotValid)
        flags |= quint8(1 << 3);
    if (content.dayOfWeekField == DayOfWeekField::NotValid)
        flags |= quint8(1 << 2);
    if (content.timeField == TimeField::NotValid)
        flags |= quint8(1 << 1);
    if (content.summerTime == SummerTime::Yes)
        flags |= quint8(1);
    dpt.setByte(6, flags);
    flags = 0;
    if (content.clockQuality == ClockQuality::WithExtendedSyncSignal)
        flags |= quint8(1 << 7);
    dpt.setByte(7, flags);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_DATE_TIME"), info.usage);
    return dpt;
}

QKnxDataPoint QKnxDptFactory::ScloMode::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::ScloMode);
}

QKnxDptFactory::ScloMode::Content QKnxDptFactory::ScloMode::getContent(const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 2)
        return {};
    Content content;
    content.contentIsValid = true;
    if (value == 0)
        content.value = Value::Autonomous;
    else if (value == 1)
        content.value = Value::Master;
    else
        content.value = Value::Slave;
    return content;
}

QKnxDataPoint QKnxDptFactory::BuildingMode::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::BuildingMode);
}

QKnxDptFactory::BuildingMode::Content QKnxDptFactory::BuildingMode::getContent(const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 2)
        return {};
    Content content;
    content.contentIsValid = true;
    if (value == 0)
        content.value = Value::InUse;
    else if (value == 1)
        content.value = Value::NotUsed;
    else
        content.value = Value::Protection;
    return content;
}

QKnxDataPoint QKnxDptFactory::OccMode::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::OccMode);
}

QKnxDptFactory::OccMode::Content QKnxDptFactory::OccMode::getContent(const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 2)
        return {};
    Content content;
    content.contentIsValid = true;
    if (value == 0)
        content.value = Value::Occupied;
    else if (value == 1)
        content.value = Value::Standby;
    else
        content.value = Value::NotOccupied;
    return content;
}

QKnxDataPoint QKnxDptFactory::Priority::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::Priority);
}

QKnxDptFactory::Priority::Content QKnxDptFactory::Priority::getContent(const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 3)
        return {};
    Content content;
    content.contentIsValid = true;
    if (value == 0)
        content.value = Value::High;
    else if (value == 1)
        content.value = Value::Medium;
    else if (value == 2)
        content.value = Value::Low;
    else
        content.value = Value::Void;
    return content;
}

QKnxDataPoint QKnxDptFactory::LightApplicationMode::setValue(Value value)
{
    Q_ASSERT_X(value < Value::ManufacturerValue, "QKnxDptFactory::LightApplicationMode::setValue",
        "For manufacturer values please use LightApplicationMode::setManufacturerValue(...)");
    return setBits(quint8(value), QKnxDpt::TypesN8::LightApplicationMode);
}

QKnxDataPoint QKnxDptFactory::LightApplicationMode::setManufacturerValue(quint8 value)
{
    if (value < 17)
        return {};
    return setBits(value, QKnxDpt::TypesN8::LightApplicationMode);
}

QKnxDptFactory::LightApplicationMode::Content QKnxDptFactory::LightApplicationMode::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 2 && value < 17)
        return {};
    Content content;
    content.contentIsValid = true;
    if (value == 0)
        content.value = Value::Normal;
    else if (value == 1)
        content.value = Value::PresenceSimulation;
    else if (value == 2)
        content.value = Value::NightRound;
    else {
        content.value = Value::ManufacturerValue;
        content.manufacturerValue = value;
    }
    return content;
}

QKnxDataPoint QKnxDptFactory::ApplicationArea::setValue(Value value)
{
    Q_ASSERT_X(value != Value::HvacReserved
        && value != Value::LightingReserved
        && value != Value::SecurityReserved
        && value != Value::LoadManagementReserved,
        "QKnxDptFactory::ApplicationArea::setValue",
        "For reserved values please use QKnxDptFactory::ApplicationArea::setReservedValue(...)");
    return setBits(quint8(value), QKnxDpt::TypesN8::ApplicationArea);
}

QKnxDataPoint QKnxDptFactory::ApplicationArea::setReservedValue(quint8 value, Value valueRange)
{
    if (valueRange != Value::HvacReserved && valueRange != Value::LightingReserved
        && valueRange != Value::SecurityReserved && valueRange != Value::LoadManagementReserved)
        return {};

    if (valueRange == Value::HvacReserved && !(value >= 15 && value <= 19))
        return {};
    if (valueRange == Value::LightingReserved && !(value >= 21 && value <= 29))
        return {};
    if (valueRange == Value::SecurityReserved && !(value >= 31 && value <= 39))
        return {};
    if (valueRange == Value::LoadManagementReserved && !(value >= 41 && value <= 49))
        return {};
    return setBits(quint8(value), QKnxDpt::TypesN8::ApplicationArea);
}

QKnxDptFactory::ApplicationArea::Content QKnxDptFactory::ApplicationArea::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if ((value > 1 && value < 10) || value > 50)
        return {};
    Content content;
    content.contentIsValid = true;

    if (value >= 15 && value <= 19) {
        content.value = Value::HvacReserved;
        content.reservedValue = value;
        return content;
    }
    if (value >= 21 && value <= 29) {
        content.value = Value::LightingReserved;
        content.reservedValue = value;
        return content;
    }
    if (value >= 31 && value <= 39) {
        content.value = Value::SecurityReserved;
        content.reservedValue = value;
        return content;
    }
    if (value >= 41 && value <= 49) {
        content.value = Value::LoadManagementReserved;
        content.reservedValue = value;
        return content;
    }

    if (value == 0)
        content.value = Value::NoFault;
    else if (value == 1)
        content.value = Value::SysAndFuncOfComInterest;
    else if (value == 10)
        content.value = Value::HvacGeneralFB;
    else if (value == 11)
        content.value = Value::HvacHotWaterHeating;
    else if (value == 12)
        content.value = Value::HvacDirectElectricalHeating;
    else if (value == 13)
        content.value = Value::HvacTerminalUnits;
    else if (value == 14)
        content.value = Value::HvacVac;
    else if (value == 20)
        content.value = Value::Lighting;
    else if (value == 30)
        content.value = Value::Security;
    else if (value == 40)
        content.value = Value::LoadManagement;
    else
        content.value = Value::ShuttersAndBlinds;
    return content;
}

QKnxDataPoint QKnxDptFactory::AlarmClassType::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::AlarmClassType);
}

QKnxDptFactory::AlarmClassType::Content QKnxDptFactory::AlarmClassType::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value < 1 || value > 3)
        return {};
    Content content;
    content.contentIsValid = true;

    if (value == 1)
        content.value = Value::Simple;
    else if (value == 2)
        content.value = Value::Basic;
    else
        content.value = Value::Extended;
    return content;
}

QKnxDataPoint QKnxDptFactory::PsuMode::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::PsuMode);
}

QKnxDptFactory::PsuMode::Content QKnxDptFactory::PsuMode::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 2)
        return {};
    Content content;
    content.contentIsValid = true;

    if (value == 0)
        content.value = Value::Disabled;
    else if (value == 1)
        content.value = Value::Enabled;
    else
        content.value = Value::Auto;
    return content;
}

QKnxDataPoint QKnxDptFactory::ErrorClassSystem::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::ErrorClassSystem);
}

QKnxDptFactory::ErrorClassSystem::Content QKnxDptFactory::ErrorClassSystem::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 18)
        return {};
    Content content;
    content.contentIsValid = true;

    if (value == 0)
        content.value = Value::NoFault;
    else if (value == 1)
        content.value = Value::GeneralDeviceFault;
    else if (value == 2)
        content.value = Value::CommunicationFault;
    else if (value == 3)
        content.value = Value::ConfigurationFault;
    else if (value == 4)
        content.value = Value::HardwareFault;
    else if (value == 5)
        content.value = Value::SoftwareFault;
    else if (value == 6)
        content.value = Value::InsufficientNonVolMem;
    else if (value == 7)
        content.value = Value::InsufficientVolMem;
    else if (value == 8)
        content.value = Value::MemAllocSizeZeroReceived;
    else if (value == 9)
        content.value = Value::CrcError;
    else if (value == 10)
        content.value = Value::WatchdogResetDetected;
    else if (value == 11)
        content.value = Value::InvalidOpcodeDetected;
    else if (value == 12)
        content.value = Value::GeneralProtectionFault;
    else if (value == 13)
        content.value = Value::MaxTableLengthExceeded;
    else if (value == 14)
        content.value = Value::UndefinedLoadCommandReceived;
    else if (value == 15)
        content.value = Value::GroupAddressTableNotSorted;
    else if (value == 16)
        content.value = Value::InvalidConnectionNumber;
    else if (value == 17)
        content.value = Value::InvalidGroupObjectNumber;
    else
        content.value = Value::GroupObjectTypeExceeded;
    return content;
}

QKnxDataPoint QKnxDptFactory::ErrorClassHvac::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::ErrorClassHvac);
}

QKnxDptFactory::ErrorClassHvac::Content QKnxDptFactory::ErrorClassHvac::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 4)
        return {};
    Content content;
    content.contentIsValid = true;

    if (value == 0)
        content.value = Value::NoFault;
    else if (value == 1)
        content.value = Value::SensorFault;
    else if (value == 2)
        content.value = Value::ProcessOrControllerFault;
    else if (value == 3)
        content.value = Value::ActuatorFault;
    else
        content.value = Value::OtherFault;
    return content;
}

QKnxDataPoint QKnxDptFactory::TimeDelay::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::TimeDelay);
}

QKnxDptFactory::TimeDelay::Content QKnxDptFactory::TimeDelay::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 25)
        return {};
    Content content;
    content.contentIsValid = true;

    if (value == 0)
        content.value = Value::NotActive;
    else if (value == 1)
        content.value = Value::OneSec;
    else if (value == 2)
        content.value = Value::TwoSec;
    else if (value == 3)
        content.value = Value::ThreeSec;
    else if (value == 4)
        content.value = Value::FiveSec;
    else if (value == 5)
        content.value = Value::TenSec;
    else if (value == 6)
        content.value = Value::FifteenSec;
    else if (value == 7)
        content.value = Value::TwentySec;
    else if (value == 8)
        content.value = Value::ThirtySec;
    else if (value == 9)
        content.value = Value::FortyFiveSec;
    else if (value == 10)
        content.value = Value::OneMin;
    else if (value == 11)
        content.value = Value::OneAndQuarterMin;
    else if (value == 12)
        content.value = Value::OneAndHalfMin;
    else if (value == 13)
        content.value = Value::TwoMin;
    else if (value == 14)
        content.value = Value::TwoAndHalfMin;
    else if (value == 15)
        content.value = Value::ThreeMin;
    else if (value == 16)
        content.value = Value::FiveMin;
    else if (value == 17)
        content.value = Value::FifteenMin;
    else if (value == 18)
        content.value = Value::TwentyMin;
    else if (value == 19)
        content.value = Value::ThirtyMin;
    else if (value == 20)
        content.value = Value::OneHrs;
    else if (value == 21)
        content.value = Value::TwoHrs;
    else if (value == 22)
        content.value = Value::ThreeHrs;
    else if (value == 23)
        content.value = Value::FiveHrs;
    else if (value == 24)
        content.value = Value::TwelveHrs;
    else
        content.value = Value::TwentyFourHrs;
    return content;
}

QKnxDataPoint QKnxDptFactory::BeaufortWindForceScale::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::BeaufortWindForceScale);
}

QKnxDptFactory::BeaufortWindForceScale::Content QKnxDptFactory::BeaufortWindForceScale::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 12)
        return {};
    Content content;
    content.contentIsValid = true;

    if (value == 0)
        content.value = Value::Calm;
    else if (value == 1)
        content.value = Value::LightAir;
    else if (value == 2)
        content.value = Value::LightBreeze;
    else if (value == 3)
        content.value = Value::GentleBreeze;
    else if (value == 4)
        content.value = Value::ModerateBreeze;
    else if (value == 5)
        content.value = Value::FreshBreeze;
    else if (value == 6)
        content.value = Value::StrongBreeze;
    else if (value == 7)
        content.value = Value::NearToModerateGale;
    else if (value == 8)
        content.value = Value::FreshGale;
    else if (value == 9)
        content.value = Value::StrongGale;
    else if (value == 10)
        content.value = Value::WholeGaleToStorm;
    else if (value == 11)
        content.value = Value::ViolentStorm;
    else
        content.value = Value::Hurricane;
    return content;
}

QKnxDataPoint QKnxDptFactory::SensorSelect::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::SensorSelect);
}

QKnxDptFactory::SensorSelect::Content QKnxDptFactory::SensorSelect::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 4)
        return {};
    Content content;
    content.contentIsValid = true;

    if (value == 0)
        content.value = Value::Inactive;
    else if (value == 1)
        content.value = Value::DigitalInputNotInverted;
    else if (value == 2)
        content.value = Value::DigitalInputInverted;
    else if (value == 3)
        content.value = Value::AnalogInput;
    else
        content.value = Value::TemperatureSensorInput;
    return content;
}

QKnxDataPoint QKnxDptFactory::ActuatorConnectType::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN8::ActuatorConnectType);
}

QKnxDptFactory::ActuatorConnectType::Content QKnxDptFactory::ActuatorConnectType::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN8::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN8::getContent(bytes).value;
    if (value > 2 || value < 1)
        return {};
    Content content;
    content.contentIsValid = true;

    if (value == 1)
        content.value = Value::SensorConnection;
    else
        content.value = Value::ControllerConnection;
    return content;
}

QKnxDptFactory::TypeN8::BasicContent QKnxDptFactory::TypeN8::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    BasicContent content;
    content.value = quint8(bytes[0]);
    content.contentIsValid = true;
    return content;
}

bool QKnxDptFactory::TypeN8::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1)
        return false;
    return true;
}

QKnxDataPoint QKnxDptFactory::TypeN8::setBits(quint8 value, QKnxDpt::TypesN8 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    dpt.setByte(0, value);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_ENUM8"), info.usage);
    return dpt;
}

bool QKnxDptFactory::TypeB8::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1)
        return false;
    if (quint8(bytes[0]) > 31)
        return false;
    return true;
}

QKnxDptFactory::TypeB8::BasicContent QKnxDptFactory::TypeB8::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    BasicContent content;
    content.contentIsValid = true;
    if (quint8(quint8(bytes[0]) & 0x01) == 1)
        content.b0 = true;
    if (quint8((quint8(bytes[0]) & 0x02) >> 1) == 1)
        content.b1 = true;
    if (quint8((quint8(bytes[0]) & 0x04) >> 2) == 1)
        content.b2 = true;
    if (quint8((quint8(bytes[0]) & 0x08) >> 3) == 1)
        content.b3 = true;
    if (quint8((quint8(bytes[0]) & 0x10) >> 4) == 1)
        content.b4 = true;
    return content;
}

QKnxDataPoint QKnxDptFactory::TypeB8::setBits(bool b0, bool b1, bool b2, bool b3, bool b4,
    QKnxDpt::TypesB8 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    quint8 temp = 0;
    if (b0)
        temp = 1;
    if (b1)
        temp |= (1 << 1);
    if (b2)
        temp |= (1 << 2);
    if (b3)
        temp |= (1 << 3);
    if (b4)
        temp |= (1 << 4);
    dpt.setByte(0, temp);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_BITSET8"), info.usage);
    return dpt;
}

QKnxDataPoint QKnxDptFactory::GeneralStatus::set(QKnxDptFactory::GeneralStatus::Content content)
{
    return setBits(content.outOfService, content.fault, content.overridden, content.inAlarm,
        content.alarmUnacknowledged, QKnxDpt::TypesB8::GeneralStatus);
}

QKnxDptFactory::GeneralStatus::Content QKnxDptFactory::GeneralStatus::getContent(
    const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    BasicContent basicContent = QKnxDptFactory::TypeB8::getContent(bytes);
    Content content;
    content.contentIsValid = basicContent.contentIsValid;
    content.outOfService = basicContent.b0;
    content.fault = basicContent.b1;
    content.overridden = basicContent.b2;
    content.inAlarm = basicContent.b3;
    content.alarmUnacknowledged = basicContent.b4;
    return content;
}

QKnxDataPoint QKnxDptFactory::DeviceControl::set(QKnxDptFactory::DeviceControl::Content content)
{
    return setBits(content.userStopped, content.ownIndividualAddress, content.verifyMode,
        false, false, QKnxDpt::TypesB8::DeviceControl);
}

QKnxDptFactory::DeviceControl::Content QKnxDptFactory::DeviceControl::getContent(
    const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    BasicContent basicContent = QKnxDptFactory::TypeB8::getContent(bytes);
    Content content;
    content.contentIsValid = basicContent.contentIsValid;
    content.userStopped = basicContent.b0;
    content.ownIndividualAddress = basicContent.b1;
    content.verifyMode = basicContent.b2;
    return content;
}

bool QKnxDptFactory::DeviceControl::validate(const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeB8::validate(bytes))
        return false;
    if (quint8(bytes[0]) > 7)
        return false;
    return true;
}

QKnxDptFactory::TypeN2::BasicContent QKnxDptFactory::TypeN2::getContent(const QByteArray &bytes)
{
    if (!validate(bytes))
        return {};
    BasicContent content;
    content.value = quint8(bytes[0]);
    content.contentIsValid = true;
    return content;
}

bool QKnxDptFactory::TypeN2::validate(const QByteArray &bytes)
{
    if (bytes.size() != 1)
        return false;
    if (quint8(bytes[0]) > 3)
        return false;
    return true;
}

QKnxDataPoint QKnxDptFactory::TypeN2::setBits(quint8 value, QKnxDpt::TypesN2 type)
{
    QKnxDpt::Info info = QKnxDpt::typeInfo(type);
    QKnxDataPoint dpt;
    dpt.setMainNumber(quint8(info.mainType));
    dpt.setMainType(info.mainType);
    dpt.setByte(0, value);
    dpt.setSpecifics(quint8(type), info.typeName, QString::fromLatin1("PDT_ENUM8"), info.usage);
    return dpt;
}

QKnxDataPoint QKnxDptFactory::OnOffAction::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN2::OnOffAction);
}

QKnxDptFactory::OnOffAction::Content QKnxDptFactory::OnOffAction::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN2::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN2::getContent(bytes).value;
    Content content;
    content.contentIsValid = true;
    if (value == 0)
        content.value = Value::Off;
    else if (value == 1)
        content.value = Value::On;
    else if (value == 2)
        content.value = Value::OffOn;
    else
        content.value = Value::OnOff;
    return content;
}

QKnxDataPoint QKnxDptFactory::AlarmReaction::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN2::AlarmReaction);
}

QKnxDptFactory::AlarmReaction::Content QKnxDptFactory::AlarmReaction::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN2::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN2::getContent(bytes).value;
    Content content;
    content.contentIsValid = true;
    if (value == 0)
        content.value = Value::NoAlarmUsed;
    else if (value == 1)
        content.value = Value::AlarmUp;
    else
        content.value = Value::AlarmDown;
    return content;
}

QKnxDataPoint QKnxDptFactory::UpDownAction::setValue(Value value)
{
    return setBits(quint8(value), QKnxDpt::TypesN2::UpDownAction);
}

QKnxDptFactory::UpDownAction::Content QKnxDptFactory::UpDownAction::getContent(
    const QByteArray &bytes)
{
    if (!QKnxDptFactory::TypeN2::validate(bytes))
        return {};
    quint8 value = QKnxDptFactory::TypeN2::getContent(bytes).value;
    Content content;
    content.contentIsValid = true;
    if (value == 0)
        content.value = Value::Up;
    else if (value == 1)
        content.value = Value::Down;
    else if (value == 2)
        content.value = Value::UpDown;
    else
        content.value = Value::DownUp;
    return content;
}

QT_END_NAMESPACE
