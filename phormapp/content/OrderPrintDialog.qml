/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
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

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import PhormApp 1.0
import "../content"

Dialog {
    id: printOrderDialog

    width: 620
    height: 420

    property bool featuresPrintVisible: false
    title: "Imprimir Ordem"

    function finishPurshase() {

        console.log("FinishPurshase\n");

        connDatabase.orderHandler.isDelivery = deliveryCheck.checked;
        connDatabase.orderHandler.costDelivery = deliveryCostField.displayText;
        connDatabase.orderHandler.description = obsField.displayText;
        connDatabase.orderHandler.client = nameFieldFirstStage.displayText;
        connDatabase.orderHandler.totalCost = Number(finishCost + deliveryCost).toFixed(2);

        if (connDatabase.orderHandler.finishPurchase() === true) {
            visibleClientList = false;
            deliveryCheck.checkedState = 1
            deliveryCostField.text = "";
            sumDeliveryText.text = "R$ 0.00";
            obsField.text = "";
            nameFieldFirstStage.text = "";
            visibleClientList = false;
            finishCost = 0.00;
            deliveryCost = 0.00;

            orderModel.clear();
            orderTable.currentRow = 0;
        }
    }



    contentItem: Rectangle {

        implicitWidth: 620
        implicitHeight: 420
        color: "#E3E2D2"

        Text {

            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter

            color: "black"
            font.pixelSize: 24
            text: "FINALIZAR ORDEM"

        }

        Rectangle {
            id: iconsRec

            width: 120
            height: 120

            color: "#E3E2D2"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Row {

                spacing: 40

                Row {
                    id: row1

                    spacing: 40

                    Rectangle {
                        height: 64
                        width: 64
                        color: "#E3E2D2"
                        radius: 10

                        Image {
                            id: saveImage
                            anchors {
                                verticalCenter: parent.verticalCenter
                                horizontalCenter: parent.horizontalCenter
                            }
                            source: "../images/save.svg"
                            fillMode: Image.PreserveAspectFit;
                            width: 48
                            height: 48
                            Behavior on width {
                                    NumberAnimation { duration: 100 }
                                }
                            Behavior on height {
                                    NumberAnimation { duration: 100 }
                                }
                            MouseArea {
                                id: mouseSave
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: {
                                    parent.width = 96;
                                    parent.height = 96;
                                }
                                onExited: {
                                    parent.width = 64;
                                    parent.height = 64;
                                }
                                onClicked: {
                                    connDatabase.orderHandler.orderStatus = 'P';
                                    connDatabase.orderHandler.printlabelPreview(false);
                                    connDatabase.orderHandler.printPreview(false);
                                    finishPurshase();
                                }
                            }
                        }

                        Text {
                            anchors.top: saveImage.bottom
                            color: "black"
                            font.pixelSize: 16
                            text: "SALVAR"
                        }
                    }
                }

                Row {

                    id: rowPrint
                    spacing: 40

                    Rectangle {
                        id: printRec

                        height: 64
                        width: 64
                        color: "#E3E2D2"
                        radius: 10

                        Image {
                            id: printImage
                            anchors {
                                verticalCenter: parent.verticalCenter
                                horizontalCenter: parent.horizontalCenter
                            }
                            source: "../images/printer.svg"
                            fillMode: Image.PreserveAspectFit;
                            width: 48
                            height: 48
                            Behavior on width {
                                    NumberAnimation { duration: 100 }
                                }
                            Behavior on height {
                                    NumberAnimation { duration: 100 }
                                }
                            MouseArea {
                                id: mousePrint
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: {
                                    parent.width = 96;
                                    parent.height = 96;
                                }
                                onExited: {
                                    parent.width = 64;
                                    parent.height = 64;
                                }
                                onClicked: {
                                    connDatabase.orderHandler.orderStatus = 'E';
                                    featuresPrintVisible = true;
                                }
                            }
                        }

                        Text {
                            anchors.top: printImage.bottom
                            color: "black"
                            font.pixelSize: 16
                            text: "IMPRIMIR"
                        }
                    }
                }
            }
        }

        Rectangle {
            id: featuresPrint
            width: 220
            height: 128

            color: featuresPrintVisible? "white" : "transparent"
            border.color: "black"
            border.width: 3
            radius:10
            visible: featuresPrintVisible? true : false
            anchors.top: iconsRec.bottom
            anchors.topMargin: 20
            anchors.left: iconsRec.right

            //Quantidade

            CheckBox {
                id: labelCheck
                anchors {
                    left: parent.left
                    leftMargin: 5
                    top: parent.top
                    topMargin: 6
                }
                implicitWidth: columnWidth
                checked: true
                style: CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 24
                        implicitHeight: 24
                        radius: 3
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        border.width: 1
                        Rectangle {
                            visible: control.checked
                            color: "#555"
                            border.color: "#333"
                            radius: 1
                            anchors.margins: 4
                            anchors.fill: parent
                        }
                    }
                    label: Text {
                        text: qsTr("Rótulo")
                        font.pixelSize: 18
                        font.family: fixedFont.name
                        font.bold: true
                    }
                }
                onClicked: {

                }
            }

            CheckBox {
                id: orderCheck
                anchors {
                    left: parent.left
                    leftMargin: 5
                    top: labelCheck.bottom
                    topMargin: 3
                }

                implicitWidth: columnWidth
                checked: true
                style: CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 24
                        implicitHeight: 24
                        radius: 3
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        border.width: 1
                        Rectangle {
                            visible: control.checked
                            color: "#555"
                            border.color: "#333"
                            radius: 1
                            anchors.margins: 4
                            anchors.fill: parent
                        }
                    }
                    label: Text {
                        text: qsTr("Ordem")
                        font.pixelSize: 18
                        font.family: fixedFont.name
                        font.bold: true
                    }
                }
                onClicked: {
                }
            }

            Button {
                id: printExec
                height: 30
                width: 65
                anchors {
                    bottom: parent.bottom
                    bottomMargin: 10
                    right: parent.right
                    rightMargin: 5
                }
                text: "OK"
                style: buttonStyle
                onClicked: {

                    featuresPrintVisible = false;
                    finishPurshase();

                    if (labelCheck.checked === true) {
//                        connDatabase.orderHandler.labelPrintCount = Number(qtdLabel).toFixed(2);
                        connDatabase.orderHandler.printlabelPreview(true);
                    }

                    if (orderCheck.checked === true) {
                        connDatabase.orderHandler.printPreview(true);
                    }
                    printOrderDialog.close();
                }
            }
        }
    }

    property Component buttonStyle: ButtonStyle {
        background: Rectangle {
            implicitHeight: 22
            implicitWidth: columnWidth
            color: control.pressed ? "darkGray" : control.activeFocus ? "#cdd" : "#ccc"
            antialiasing: true
            border.color: "gray"
            radius: height/2
            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"
                antialiasing: true
                visible: !control.pressed
                border.color: "#aaffffff"
                radius: height/2
            }
        }
    }
}
