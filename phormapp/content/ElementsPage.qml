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


import QtQuick 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2
import PhormApp 1.0

Item {
    width: parent.width
    height: parent.height

    property var elementDataDialog: NewElementDialog{}

    property int columnWidth: parent.width / 9
    property int  selectSupDelegate: 0

    property real progress: 0
    property int pixelSize: 22
    //Valores para rodar em resolução baixa
    property int boxHeightSmall: 40
    property int boxWidthSmall: 60
    property int boxHeightLarge: 40
    property int boxWidthLarge: 220
    property string itemRemove


    SequentialAnimation on progress {
        loops: Animation.Infinite
        running: true
        NumberAnimation {
            from: 0
            to: 1
            duration: 3000
        }
        NumberAnimation {
            from: 1
            to: 0
            duration: 3000
        }
    }

    FontLoader { id: fixedFont; name: "Droid Sans" }

    Keys.onReleased: {
        if ((event.key === Qt.Key_Down) || (event.key === Qt.Key_Up)) {
            console.log("Key Down or Key Up pressed");
        }
    }

    //######################################################################################
    // INICIO DO FORMULARIO

    Column {
        id: elementForm
        spacing: 10
        width: parent.width
        anchors {
            top: parent.top
            topMargin: 10
        }

        Row {
            spacing: 10

            Text {
                anchors.margins: 20
                anchors.verticalCenter: parent.verticalCenter
                text: "Nome:"
                font.pixelSize: pixelSize
                font.family: fixedFont.name
            }

            TextField {
                id: nameFieldFirstStage
                anchors.margins: 20
                style: textfieldStyle
                implicitWidth: 520
            }

            Item {
                id: applyButtom

                anchors.rightMargin: 10
                width: 40 ; height: 40
                enabled: nameFieldFirstStage.text.length
                Image {
                    source: addMouseArea.pressed ? "../images/add_icon_pressed.png" : "../images/add_icon.png"
                    anchors.centerIn: parent
                    opacity: enabled ? 1 : 0.5
                }
                MouseArea {
                    id: addMouseArea
                    anchors.fill: parent
                    onClicked: {
                        elementDataDialog.name = nameFieldFirstStage.displayText;
                        elementDataDialog.visible = true;
                        elementDataDialog.clear();
                        elementDataDialog.isUpdate = false;
                    }
                }
            }

        }

        Image {
            id: divider
            width: parent.width
            anchors {
                right: parent.right
            }

            source: "../images/divider-form.png"
            height: 2
        }
    }

    TableView {
        id: tableSuppliers
        y: 200
        width: parent.width
        height: (parent.height / 3) + 200
        frameVisible: true
        headerVisible: true
        alternatingRowColors: true

        property int margins: Qt.platform.os === "osx" ? 16 : 0

        TableViewColumn {
            role: "name"
            title: "Nome"
            width: 420
        }
        TableViewColumn {
            role: "group"
            title: "Grupo"
            width: 120
        }
        TableViewColumn {
            role: "subgroup"
            title: "SubGrupo"
            width: 220
        }

        headerDelegate: BorderImage {
            source: "../images/header.png"
            border{left:2;right:2;top:2;bottom:2}
            Text {
                text: styleData.value
                anchors.centerIn:parent
                color:"#333"
            }
        }

        model: SortFilterProxyModel {
            id: proxyModel
            source: elementModel.count > 0 ? elementModel : null

            sortOrder: tableSuppliers.sortIndicatorOrder
            sortCaseSensitivity: Qt.CaseInsensitive
            sortRole: elementModel.count > 0 ? tableSuppliers.getColumn(tableSuppliers.sortIndicatorColumn).role : ""

            filterString: "*" + nameFieldFirstStage.text + "*"
            filterSyntax: SortFilterProxyModel.Wildcard
            filterCaseSensitivity: Qt.CaseInsensitive
        }

        rowDelegate: Rectangle {
            height: 30
            Behavior on height{ NumberAnimation{} }

            color: styleData.selected ? "#448" : (styleData.alternate? "#eee" : "#fff")
            BorderImage{
                id: selected
                anchors.fill: parent
                source: "../images/selectedrow.png"
                visible: styleData.selected
                border{left:2; right:2; top:2; bottom:2}
                SequentialAnimation {
                    running: true; loops: Animation.Infinite
                    NumberAnimation { target:selected; property: "opacity"; to: 1.0; duration: 900}
                    NumberAnimation { target:selected; property: "opacity"; to: 0.5; duration: 900}
                }
            }

            Row {
                z: 1
                x: 750

                visible: styleData.selected
                Button {
                    id: updateButtom
                    text: "EDITAR"
                    implicitWidth: columnWidth
                    onClicked: {
                        elementDataDialog.name = proxyModel.get(styleData.row).name;
                        elementDataDialog.visible = true;

                        elementDataDialog.editForm();
                        elementDataDialog.isUpdate = true;
                    }
                }
                Button {
                    id: removeButtom
                    text: "EXCLUIR"
                    implicitWidth: columnWidth
                    onClicked: {

                        console.log("indice: " + styleData.row);
                        console.log(proxyModel.get(styleData.row).name);

                        itemRemove = proxyModel.get(styleData.row).name;
                        proxyModel.remove(styleData.row, 1);
                        connDatabase.elementHandler.remove(itemRemove);
                    }
                }
            }
        }

        onClicked: {
            console.log("Linha 1: " + elementModel.get(tableSuppliers.currentRow).name);
            console.log("num: ", currentRow);

        }
    }

    property  Component textfieldStyle: TextFieldStyle {
        textColor: "black"
        font.pixelSize: pixelSize
        background: Item {
            implicitHeight: 50
            BorderImage {
                source: "../images/textinput.png"
                border.left: 8
                border.right: 8
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

            }
        }
    }

    property Component actionButtonsStyle: ButtonStyle {
        background: Rectangle {
            implicitHeight: 30
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
