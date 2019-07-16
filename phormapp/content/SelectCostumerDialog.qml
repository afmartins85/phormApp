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
    id: selectCostumerDialog

    width: 720
    height: 480
    title: "Lista de clientes"

//    property var findOrderDialog: FindOrderDialog{}
    property bool featuresPrintVisible: false
    property string costumerName: ""

    signal showListCostumer


    ListModel {
        id: costumerModel
    }

    onShowListCostumer: {

        costumerModel.clear();

        if (connDatabase.orderHandler.loadCostumerList(costumerName) === true) {
            for (var i=0 ; i< connDatabase.orderHandler.costumerListCount(); ++i)
                costumerModel.append({"title": connDatabase.orderHandler.costumerListAt(i)})
        } else {
            messageDialog.setText("CLIENTE NÃO LOCALIZADO!!");
            messageDialog.show();
            selectCostumerDialog.close();
        }
    }

    contentItem: Rectangle {

        implicitWidth: 720
        implicitHeight: 480
        color: "#E3E2D2"

        //![view]
        ListView {
            id: listview
            model: costumerModel
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

                Image {
                    id: shadow
                    anchors.top: parent.bottom
                    width: parent.width
                    visible: !mouse.pressed
                    source: "../images/shadow.png"
                }

                MouseArea {
                    id: mouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        findOrderDialog.visible = true;
                        findOrderDialog.clientID = costumerModel.get(index).title;
                        findOrderDialog.findFromCostumer();
                        selectCostumerDialog.close();
                    }
                }

                //![delegate-properties]
                Text {
                    id: costumerText
                    text: title
                    font.pixelSize: 14
                    color: "#333"

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 12
                    anchors.rightMargin: 40
                    elide: Text.ElideRight
                }
                //![delegate-properties]
            }
        }

        Button {
            id: closeSelCostumerWindowButtom
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
                selectCostumerDialog.close();
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

        detailedText: "NÃO FOI POSSÍVEL LOCALIZAR O CLIENTE INFORMADO AO BANCO DE DADOS!!"

        icon: StandardIcon.Warning

        function show() {
            messageDialog.open();
        }
    }
}
