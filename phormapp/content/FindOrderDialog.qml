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
    id: findOrderDialog

    width: 840
    height: 620
    title: "Pesquisar Ordem"

    property var rePrintLabelDialog: RePrintLabels {}
    property bool featuresPrintVisible: false
    property bool expand: false
    property int orderID: 0
    property string clientID: ""
    property string phoneID: ""
    property string purchaseDate: ""

    signal findFromOrder
    signal findFromCostumer
    signal findFromPhone
    signal findFromDate


    Keys.onReleased: {
        if (event.key === Qt.Key_Escape) {
            item.source = "../images/delegate.png";
            clientText.color = "black"
            phoneText.color = "black"
            orderText.color = "black"
            dateTimeText.color = "black"
        }
    }

    ListModel {
        id: orderModel
    }

    onFindFromOrder: {

        orderModel.clear();

        if (connDatabase.orderHandler.findOrderFromID(orderID) === true) {
            orderModel.append({
                                  "title": connDatabase.orderHandler.composedFound(),
                                  "client": connDatabase.orderHandler.costumerFound(),
                                  "phone": connDatabase.orderHandler.phoneFound(),
                                  "date": connDatabase.orderHandler.dateTimeFound()})
        } else {
            messageDialog.setText("ORDEM NÃO LOCALIZADO!!");
            messageDialog.show();
            findOrderDialog.close();
        }
    }

    onFindFromCostumer: {

        orderModel.clear();
        if (connDatabase.orderHandler.findOrderFromCostumer(clientID) === true) {
            for (var i=0 ; i< connDatabase.orderHandler.costumerListCount(); ++i)
                orderModel.append({
                                      "title": connDatabase.orderHandler.orderListAt(i),
                                      "client": connDatabase.orderHandler.costumerFound(),
                                      "phone": connDatabase.orderHandler.phoneFound(),
                                      "date": connDatabase.orderHandler.datetimeListAt(i)})
        } else {
            messageDialog.setText("ORDENS NÃO LOCALIZADO!!");
            messageDialog.show();
            findOrderDialog.close();
        }
    }

    onFindFromPhone: {

        orderModel.clear();

        if (connDatabase.orderHandler.findOrderFromPhone(phoneID) === true) {
            for (var i=0 ; i< connDatabase.orderHandler.costumerListCount(); ++i)
                orderModel.append({
                                      "title": connDatabase.orderHandler.orderListAt(i),
                                      "client": connDatabase.orderHandler.costumerFound(),
                                      "phone": connDatabase.orderHandler.phoneFound(),
                                      "date": connDatabase.orderHandler.datetimeListAt(i)})
        } else {
            messageDialog.setText("CLIENTE NÃO LOCALIZADO!!");
            messageDialog.show();
            findOrderDialog.close();
        }
    }

    onFindFromDate: {

        orderModel.clear();

        if (connDatabase.orderHandler.findOrderFromDate(purchaseDate) === true) {
            for (var i=0 ; i< connDatabase.orderHandler.phoneListCount(); ++i)
                orderModel.append({
                                      "title": connDatabase.orderHandler.orderListAt(i),
                                      "client": connDatabase.orderHandler.costumerListAt(i),
                                      "phone": connDatabase.orderHandler.phoneListAt(i),
                                      "date": connDatabase.orderHandler.dateTimeFound()})
        } else {
            messageDialog.setText("ORDENS NÃO LOCALIZADO!!");
            messageDialog.show();
            findOrderDialog.close();
        }
    }

    contentItem: Rectangle {

        implicitWidth: 840
        implicitHeight: 620
        color: "#E3E2D2"

        //![view]
        ListView {
            id: listview
            model: orderModel
            delegate: listItemDelegate
            anchors.top: parent.top
            anchors.bottom: footer.top
            width: parent.width
            clip: true
        }
        //![view]

        BorderImage {
            id: footer

            width: parent.width
            anchors.bottom: parent.bottom
            source: "../images/delegate.png"
            border.left: 5; border.top: 5
            border.right: 5; border.bottom: 5

            Rectangle {
                y: -1 ; height: 1
                width: parent.width
                color: "#bbb"
            }
            Rectangle {
                y: 0 ; height: 1
                width: parent.width
                color: "white"
            }
        }

        Component {
            id: listItemDelegate

            BorderImage {
                id: item

                width: parent.width ; height: 70
                source: mouse.pressed ? "../images/delegate_pressed.png" : "../images/delegate.png"
                border.left: 5; border.top: 5
                border.right: 5; border.bottom: 5

                property real wrapperOpacity: 0
                property string wrapperBackground: "White"
                property string wrapperForeground: "Black"

                Image {
                    id: shadow
                    anchors.top: parent.bottom
                    width: parent.width
                    visible: mouse.pressed
                    source: "../images/shadow.png"
                }

                MouseArea {
                    id: mouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {

                        if (expand === false) {
                            parent.state = "expanded"
                            item.source = "../images/delegate_selected.png";
                            clientText.color = "white"
                            phoneText.color = "white"
                            orderText.color = "white"
                            dateTimeText.color = "white"
                            expand = true;
                        } else {
                            parent.state = ""
                            expand = false;
                            item.source = "../images/delegate.png";
                            clientText.color = "black"
                            phoneText.color = "black"
                            orderText.color = "black"
                            dateTimeText.color = "black"
                        }
                    }
                }

                //![delegate-properties]
                Text {
                    id: clientText
                    text: client
                    font.pixelSize: 14
                    color: "#333"

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.rightMargin: 40
                    elide: Text.ElideRight
                }

                Text {
                    id: phoneText
                    text: phone
                    font.pixelSize: 10
                    color: "#333"

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.top: clientText.bottom
                    anchors.bottomMargin: 12
                    anchors.leftMargin: 12
                    anchors.rightMargin: 40
                    elide: Text.ElideRight
                }

                Text {
                    id: orderText
                    text: title
                    font.pixelSize: title.length > 40 ? 10 : 16
                    color: "#333"

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: clientText.right
                    anchors.leftMargin: 12
                    anchors.rightMargin: 40
                    elide: Text.ElideRight
                }

                Text {
                    id: dateTimeText
                    text: date
                    font.pixelSize: 14
                    color: "#333"

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.leftMargin: 12
                    anchors.rightMargin: 40
                    elide: Text.ElideRight
                }
                //![delegate-properties]

                Item {
                    id: expanded

                    width: parent.width
                    opacity: item.wrapperOpacity
                    enabled: item.wrapperOpacity

                    Row {

                        spacing: 10
                        Button {
                            id: reOrder

                            height: 32

                            text: "Repetir Ordem..."
                            style: buttonStyle

                            onClicked: {
                                if (connDatabase.orderHandler.loadPurchase (
                                            orderModel.get(index).title,
                                            orderModel.get(index).client,
                                            orderModel.get(index).date) === true) {

                                    newOrder.visible = true;
                                    newOrder.isRecord = true;
                                    newOrder.editOrder();

                                    nameFieldFirstStage.text = orderModel.get(index).client;
                                    visibleClientList = false;
                                    obsField.text = connDatabase.orderHandler.description;
                                }
                            }
                        }

                        Button {
                            id: reprintLabel

                            height: 32

                            text: "Reimprimir Rótulo..."
                            style: buttonStyle

                            onClicked: {
                                connDatabase.orderHandler.loadPurchase (
                                            orderModel.get(index).title,
                                            orderModel.get(index).client,
                                            orderModel.get(index).date)

                                rePrintLabelDialog.open();
                            }
                        }

                        Button {
                            id: orderEdit

                            height: 32

                            text: "Reimprimir Ordem..."
                            style: buttonStyle

                            onClicked: {
                                connDatabase.orderHandler.loadPurchase (
                                            orderModel.get(index).title,
                                            orderModel.get(index).client,
                                            orderModel.get(index).date)

                                connDatabase.orderHandler.loadPrintOrder(
                                connDatabase.orderHandler.getOrderId());
                                findOrderDialog.close();
                            }
                        }
                    }
                }

                states: [
                    State {
                        name: "expanded"

                        PropertyChanges { target: item; height: listview.height/6 }
                        PropertyChanges { target: item; wrapperOpacity: 1; x: 0 }
//                        PropertyChanges { target: closeButton; opacity: 1 }
//                        PropertyChanges { target: wrapper.ListView.view; contentY: wrapper.y; interactive: false }
                    }
                ]

                transitions: [
                    Transition {
                        NumberAnimation {
                            duration: 200;
                            properties: "height,width,anchors.rightMargin,anchors.topMargin,opacity,contentY"
                        }
                    }
                ]
            }
        }

        Button {
            id: closeFindWindowButtom
            height: 40
            anchors {
                bottom: parent.bottom
                bottomMargin: 10
                right: parent.right
                rightMargin: 10
            }
            text: "Fechar"
            style: buttonStyle
            onClicked: {
                findOrderDialog.close();
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

    MessageDialog {
        id: messageDialog
        title: qsTr("Erro!")

        detailedText: "NÃO FOI POSSÍVEL ENCONTRAR AS INFORMAÇÕES NO BANCO DE DADOS!!"

        icon: StandardIcon.Warning

        function show() {
            messageDialog.open();
        }
    }
}
