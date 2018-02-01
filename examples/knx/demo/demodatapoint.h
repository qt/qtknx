/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DEMODATAPOINT_H
#define DEMODATAPOINT_H

#include <QtCore/QTimer>
#include <QtGui/QColor>
#include <QtKnx/QKnxLinkLayerFrame>

class DemoDataPoint
{
public:
    virtual ~DemoDataPoint() = default;

    virtual QVector<quint8> bytes() const { return QVector<quint8>(); }
    virtual bool updateDataPointState(const QKnxLinkLayerFrame &frame) = 0;

    int position() const { return m_position; }
    void setPosition(int pos) { m_position = pos; }

protected:
    int m_position { 0 };

    static const QVector<quint8> BytesOn;
    static const QVector<quint8> BytesOff;
};

class DemoSwitchDataPoint : public DemoDataPoint
{
public:
    DemoSwitchDataPoint() = default;
    ~DemoSwitchDataPoint() override = default;

    QVector<quint8> bytes() const override;
    bool updateDataPointState(const QKnxLinkLayerFrame &frame) override;

    void uiToggle();
    bool isOn() const;

private:
    bool m_state;
};

class DemoColorLed : public QObject, public DemoDataPoint
{
    Q_OBJECT

public:
    DemoColorLed() = default;
    ~DemoColorLed() override = default;

    bool updateDataPointState(const QKnxLinkLayerFrame &frame) override;

    QColor currentColor() const;
    void setCurrentColor(QColor color);

    QVector<quint8> redBytes() const;
    QVector<quint8> greenBytes() const;
    QVector<quint8> blueBytes() const;

    bool isOn() const;

Q_SIGNALS:
    void colorChange(QColor color);

private:
    QColor m_stateColor;
    quint8 numMsgConfirmation { 0 };
};

class DemoRockerDataPoint : public QObject, public DemoDataPoint
{
    Q_OBJECT
public:
    DemoRockerDataPoint() = default;
    virtual ~DemoRockerDataPoint() override = default;

    bool updateDataPointState(const QKnxLinkLayerFrame &frame) override;

Q_SIGNALS:
    void rockerChange(int position);
};

class DemoBlindDataPoint : public QObject, public DemoDataPoint
{
    Q_OBJECT
public:
    DemoBlindDataPoint() = default;
    ~DemoBlindDataPoint() override = default;

    bool updateDataPointState(const QKnxLinkLayerFrame &frame) override;

    QVector<quint8> moveUpBytes() const;
    QVector<quint8> moveDownBytes() const;
    QVector<quint8> stopBytes() const;

public slots:
    void moveBlindUp();
    void moveBlindDown();
    void stopBlind();

Q_SIGNALS:
    void blindChange(int position);
    void blindFullyUp();
    void blindFullyDown();

private:
    void chooseTimeoutHandle(bool blindUp);
    void reenableTimer();
    QTimer m_timer;
};

#endif // DEMODATAPOINT_H
