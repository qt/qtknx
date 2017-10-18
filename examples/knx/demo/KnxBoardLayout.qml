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
import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

Rectangle {
    property real xScaleFactor: (width / Screen.width).toFixed(2)

    function toggleLight(num)
    {
        knxDemo.toggleLight(num);
    }

    function enableBox(num, checked)
    {
        if (num == 1)
            e1Scheme.lightE1.on = checked;
        else if (num == 2)
            e2Scheme.lightE2.on = checked;
        else if (num == 3)
            e3e4Scheme.lightE3.on = checked;
        else if (num == 4)
            e3e4Scheme.lightE4.on = checked;
        else if (num == 5)
            e5e6Scheme.lightE5.on = checked;
        else if (num == 6)
            e5e6Scheme.lightE6.on = checked;
        else if (num == 7)
            e7e8Scheme.lightE7.on = checked;
        else if (num == 8)
            e7e8Scheme.lightE8.on = checked;
        else if (num == 9)
            e9Scheme.lightE9.on = checked;
        else if (num == 10)
            e10Scheme.lightE10.on = checked;
        else if (num == 11)
            e11Scheme.lightE11.on = checked;
        else if (num == 12)
            e12Scheme.lightE12.on = checked;
        else if (num == 14)
            changeLedColor(middleRightLed.led, checked ? "Yellow" : "#394251");
        return;
    }

    function changeColorLeftLed(color)
    {
        changeLedColor(middleLeftLed.led, color);
    }

    function changeLedColor(led, color)
    {
        if (Qt.colorEqual(color, "black"))
            led.color = "#394251";
        else
            led.color = color;
    }

    function rotateLogo(rocker)
    {
        logo.rotation = (rocker - 1000) * 360 / 1000;
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        RowLayout {
            spacing: 0
            E1Scheme { id: e1Scheme }
            E2Scheme { id: e2Scheme }
            E3E4Scheme { id: e3e4Scheme }
        }
        RowLayout {
            spacing: 0
            E5E6Scheme { id: e5e6Scheme }
            E13Led { id: middleLeftLed }
            E12RedLed { id: e12Scheme }
            E14Led { id: middleRightLed }
            E7E8Scheme { id: e7e8Scheme }
        }
        RowLayout {
            spacing: 0
            E9RedLed { id: e9Scheme }
            ColumnLayout {
                spacing: 0
                ColorSlider {}
                E10RedLed { id: e10Scheme }
            }
            E11RedLed { id: e11Scheme }
        }
    }
}
