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
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import PhormApp 1.0
import "content"


ApplicationWindow {
    id: applicationWindow1
//    visible: true
//    visibility: Window.FullScreen
//    visibility: Window.Windowed

    property real opacity_background: 0
    property bool itemSelected: false
    property bool menuSelect: false
    property bool isSelected: true

    property var splashWindow: Splash {
        onTimeout: {
//            applicationWindow1.visible = true;
            applicationWindow1.visibility = Window.FullScreen;
        }
    }

    property var elementModel: ListModel {
        Component.onCompleted: {
            connDatabase.elementHandler.loadElementList();

            for (var i=0 ; i< connDatabase.elementHandler.elementListCount() ; ++i)
                elementModel.append({
                                        "name":connDatabase.elementHandler.elementListAt(i),
                                        "group": connDatabase.elementHandler.elementGroupAt(i),
                                        "subgroup": connDatabase.elementHandler.elementSubGroupAt(i)
                                    })
        }
    }

    property var phormModel: ListModel {
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadPhormList();
            for (var i=0 ; i< connDatabase.maintenanceHandler.phormListCount(); ++i)
                phormModel.append({"title": connDatabase.maintenanceHandler.phormListAt(i)})
        }
    }

    property var excipentModel: ListModel {
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadExcipientList(connDatabase.maintenanceHandler.getFirstPhormId());
            for (var i=0 ; i< connDatabase.maintenanceHandler.excipientCount(); ++i)
                excipentModel.append({"title": connDatabase.maintenanceHandler.excipientListAt(i)})
        }
    }

    property var specCapsModel: ListModel {
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadAllSpecificationCapsList();
            for (var i=0 ; i< connDatabase.maintenanceHandler.specificationCapsCount(); ++i) {
                specCapsModel.append({
                     "title": connDatabase.maintenanceHandler.specCapsListAt(i)
                })
            }
        }
    }

    property var capsForPacksModel: ListModel {
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadCapsForPackList(connDatabase.maintenanceHandler.getFirstPhormId());
            for (var i=0 ; i< connDatabase.maintenanceHandler.capsForPackCount(); ++i) {

                capsForPacksModel.append({
                     "title": connDatabase.maintenanceHandler.capacityListAt(i),
                     "capsula": connDatabase.maintenanceHandler.capsForPackListAt(i)
                })
            }
        }
    }

    property var doctorsModel: ListModel {
        Component.onCompleted: {
            connDatabase.doctorHandler.loadDoctorList();

            for (var i=0 ; i< connDatabase.doctorHandler.doctorListCount() ; ++i)
                doctorsModel.append({
                                        "name":connDatabase.doctorHandler.doctorListAt(i),
                                        "crm": connDatabase.doctorHandler.crmListAt(i),
                                        "fone_1": connDatabase.doctorHandler.phoneListAt(i)
                                    })
        }
    }

    property var clientsModel: ListModel {
        Component.onCompleted: {
            connDatabase.clientHandler.loadClientList();

            for (var i=0 ; i< connDatabase.clientHandler.clientListCount() ; ++i)
                clientsModel.append({"name":connDatabase.clientHandler.clientListAt(i),
                                        "fone_1": connDatabase.clientHandler.firstPhoneAt(i),
                                        "fone_2": connDatabase.clientHandler.secPhoneAt(i)})
        }
    }

    property var unitModel: ListModel {
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadUnitsList();
            for (var i=0 ; i< connDatabase.maintenanceHandler.unitListCount(); ++i)
                unitModel.append({"title": connDatabase.maintenanceHandler.unitsListAt(i)})
        }
    }

    property var validateModel: ListModel {
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadValidateList();
            for (var i=0 ; i< connDatabase.maintenanceHandler.validateCount(); ++i)
                validateModel.append({"title": connDatabase.maintenanceHandler.validateListAt(i)})
        }
    }

    property var printerModel: ListModel {
        Component.onCompleted: {
            connDatabase.ptrSetup.printersLoadList();
            for (var i=0 ; i< connDatabase.ptrSetup.printersCount(); ++i)
                printerModel.append({"printer": connDatabase.ptrSetup.printerListAt(i)})
        }
    }

    property var printerSelectDialog: PrinterSelectDialog {}
    property var menuWindow: MaintenancePage {}
    property var netConfigWindow: NetConfigPage {}
    property var connDatabase: ConnectionDatabase {
        maintenanceHandler: MaintenanceHandler {}
        clientHandler: ClientHandler {}
        doctorHandler: DoctorHandler {}
        elementHandler: ElementHandler {}
        suppliersHandler: SuppliersHandler {}
        localHandler: LocalHandler {}
        orderHandler: OrderHandler {}
        ptrSetup: PrinterSetup {}

        Component.onCompleted: {
            orderHandler.loadPrintConfig();
        }
    }

    Rectangle {
        color: "#F5F4E2"
        anchors.fill: parent
        Image {
            anchors {
                horizontalCenter: parent.horizontalCenter
                topMargin: 20
            }
            width: 152
            height: 161
            fillMode: Image.PreserveAspectFit
            source: "images/logo.png"
        }
    }

    Rectangle {
        color: "#000168"
        height: parent.height
        width: 55
    } //Barra lateral

    toolBar: BorderImage {
        id: borderImage1
        border.bottom: 8
        source: "images/toolbar.png"
        width: parent.width
        height: 50

        Rectangle {
            color: "#000168"
            anchors.fill: parent
            Rectangle {
                id: backButton
                width: opacity ? 60 : 0
                anchors.left: parent.left
                anchors.leftMargin: 20
                opacity: stackView.depth > 1 ? 1 : 0
                anchors.verticalCenter: parent.verticalCenter
                antialiasing: true
                height: 60
                radius: 4
                color: backmouse.pressed ? "#222" : "transparent"
                Behavior on opacity { NumberAnimation{} }
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source: "images/navigation_previous_item.png"
                }
                MouseArea {
                    id: backmouse
                    anchors.fill: parent
                    anchors.margins: -10
                    onClicked:  {
                        stackView.pop();
                        itemSelected = false;
                    }
                }
            }

            Rectangle {
                id: menuButtom

                width: 250
                height: 48
                anchors.margins: 5
                anchors.right: parent.right
                radius: 8
                color: isSelected ? "transparent" : "#212126"
                Behavior on opacity { NumberAnimation{} }
                Image {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                        rightMargin: 10
                    }

                    width: 36
                    height: 36
                    source: "images/menu.png"
                }
                MouseArea {
                    id: menumouse
                    anchors.fill: parent
                    onClicked: {
                        if (isSelected === true) {
                            menuSelect = true;
                            isSelected = false;
                        } else {
                            menuSelect = false;
                            isSelected = true;
                        }
                    }
                }
            }

            Rectangle {
                id: menuConfig

                anchors.top: menuButtom.bottom
                anchors.right: parent.right
                anchors.margins: 5
                width: menuButtom.width
                height: 60 * 6 //Quantidade de botões
                radius: 8
                color: "#212126"
                clip: true
                opacity: menuSelect ? 1 : 0

                onOpacityChanged: SequentialAnimation {
                    PropertyAction { target: menuConfig; property: "height"; value: 0 }
                    NumberAnimation { target: menuConfig; property: "height"; to: 60 * 6; duration: 250; easing.type: Easing.InOutQuad }
                }
                Button {
                    id: netcfgButtom
                    anchors.top: parent.top
                    text: "Configurações de Rede"
                    style: touchStyle
                    onClicked: {
                        netConfigWindow.visible = !netConfigWindow.visible
                        menuSelect = false;
                        isSelected = true;
                    }
                    enabled: menuSelect ? 1 : 0
                }

                Button {
                    id: updButtom
                    anchors.top: netcfgButtom.bottom
                    text: "Atualização de Software"
                    style: touchStyle
                    enabled: menuSelect ? 1 : 0
                }

                Button {
                    id: maintenanceButtom
                    anchors.top: updButtom.bottom
                    text: "Manutenção de tabelas"
                    style: touchStyle
                    onClicked: {
                        menuWindow.visible = !menuWindow.visible;
                        menuSelect = false;
                        isSelected = true;
                    }
                    enabled: menuSelect ? 1 : 0
                }

                Button {
                    id: printerButtom
                    anchors.top: maintenanceButtom.bottom
                    text: "Impressora"
                    style: touchStyle
                    onClicked: {
                        printerSelectDialog.visible = !printerSelectDialog.visible;
                        menuSelect = false;
                        isSelected = true;
                    }
                    enabled: menuSelect ? 1 : 0
                }

                Button {
                    id: aboutButtom
                    anchors.top: printerButtom.bottom
                    text: "Sobre"
                    style: touchStyle
                    enabled: menuSelect ? 1 : 0
                }
                Button {
                    id: powerButtom
                    anchors.top: aboutButtom.bottom
                    text: "Desligar"
                    style: touchStyle
                    onClicked: {
                        Qt.quit();
                    }
                    enabled: menuSelect ? 1 : 0
                }
            }
        }
    } //toolbar

    Component {
        id: touchStyle
        ButtonStyle {
            panel: Item {
                implicitHeight: 60
                implicitWidth: menuConfig.width
                BorderImage {
                    anchors.fill: parent
                    antialiasing: true
                    border.bottom: 8
                    border.top: 8
                    border.left: 8
                    border.right: 8
                    anchors.margins: control.pressed ? -2 : 2
                    source: control.pressed ? "images/button_pressed.png" : "images/button_default.png"
                    Text {
                        text: control.text
                        anchors.centerIn: parent
                        color: "white"
                        font.pixelSize: 18
                        renderType: Text.NativeRendering
                    }
                }
            }
        }
    }


    ListModel {
        id: pageModel
        ListElement {
            title: "Cadastro"
            page: "content/EnrollPage.qml"
            icon: "images/note.svg"
            active: true
        }
        ListElement {
            title: "Pedidos"
            page: "content/OrderPage.qml"
            icon: "images/cart.svg"
            active: true
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             itemSelected = false;
                             stackView.pop();
                             event.accepted = true;
                         }

        initialItem: Item {
            width: parent.width
            height: parent.height

            ListView {
                model: pageModel
                anchors.fill: parent
                anchors.margins: 140
//                orientation: ListView.Horizontal
                delegate: AndroidDelegate {
                    text: title
                    iconImage: icon
                    onClicked: {
                        if (itemSelected == false) {
                            stackView.push(Qt.resolvedUrl(page));
                            itemSelected = true;
                        }
                    }
                }
            }
        }
    }

    statusBar: BorderImage {
        border.top: 0
        source: "images/toolbar.png"
        width: parent.width
        height: 40

        Text {
            font.pixelSize: 12
            Behavior on x { NumberAnimation{ easing.type: Easing.OutCubic} }
            x: backButton.x + backButton.width + 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            color: "white"
            text: "powered by truetech-systems.com"
        }
    }
}
