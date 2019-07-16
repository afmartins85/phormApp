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
import QtQml.Models 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import "../content"


Item {
    id: tabBar
    width: parent.width
    height: parent.height

    function checkView (i) {
        if (view.current === i) {
            return true;
        }
        return false;
    }

    property int columnWidth: parent.width / 6

    Rectangle {
        id: selectionAbe
        x: 980
        z: 1
        width: columnWidth
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0

        Rectangle {
            anchors.fill: parent
            color: "#333333"

            Image {
                height: parent.height
                anchors.right: parent.right
                source: "../images/border_resume_abe.png"
            }
        }

        Item {
            id: listResume
            width: selectionAbe.width
            height: 350
            anchors {
                right: parent.right
                rightMargin: 5
            }

            ListView {
                id: resumeView
                width: parent.width
                height: parent.height
                anchors.top: parent.top
                anchors.topMargin: 45
                clip: true


                model: ListModel {
                    id:pageModel

                    ListElement {
                        name: "Clientes"
                        page: "content/ClientsPage.qml"
                    }

                    ListElement {
                        name: "Médicos"
                        page: "content/DoctorsPage.qml"
                    }

                    ListElement {
                        name: "Fornecedores"
                        page: "content/SuppliersPage.qml"
                    }

                    ListElement {
                        name: "Insumos"
                        page: "content/ElementsPage.qml"
                    }
                }
                delegate: itemDelegate

                spacing: 5
                highlight: highlightComponent
                focus: true
            }

            Component {
                    id: highlightComponent

                Rectangle {
                    width: resumeView.width
                    color: "green"
                    radius: 10
                    y: resumeView.currentItem.y
                    Behavior on y {
                        SpringAnimation {
                            spring: 3
                            damping: 0.2
                        }
                    }
                }
            }

            Component {
                id: itemDelegate
                Item {
                    width: resumeView.width
                    height: 80

                    Keys.onReleased: {
                        if (event.key === Qt.Key_Down) {
                            view.current = index;
                            event.accepted = true;
                        } else if (event.key === Qt.Key_Up) {
                            view.current = index;
                            event.accepted = true;
                        }
                    }
                    Text {
                        text: name
//                        anchors.centerIn: parent.center
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        color: "white"
                        font.bold: false
                        font.pixelSize: 26
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            parent.ListView.view.currentIndex = index
                            view.current = index
                        }
                    }
                }
            }
        }
    }

    TabWidget {
        id: view
        width: parent.width - selectionAbe.width
        height: parent.height
        anchors {
            left: selectionAbe.right
            leftMargin: 2
            bottomMargin: 3
        }

        Rectangle {
            anchors.fill: parent
            color: "#E3E2D2"

            Rectangle {
                anchors.fill: parent; anchors.margins: 20
                color: "#F5F4E2"

                Image {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                    }
                    fillMode: Image.PreserveAspectFit
                    source: "../images/logo.png"
                    opacity: 0.2
                }
                ClientsPage {
                    enabled: checkView(0);
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "#E3E2D2"

            Rectangle {
                anchors.fill: parent; anchors.margins: 20
                color: "#F5F4E2"
                Image {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                    }
                    fillMode: Image.PreserveAspectFit
                    source: "../images/logo.png"
                    opacity: 0.2
                }
                DoctorsPage {
                    enabled: checkView(1);
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "#E3E2D2"

            Rectangle {
                anchors.fill: parent; anchors.margins: 20
                color: "#F5F4E2"
                Image {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                    }
                    fillMode: Image.PreserveAspectFit
                    source: "../images/logo.png"
                    opacity: 0.2
                }
                SuppliersPage {
                    enabled: checkView(2);
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "#E3E2D2"

            Rectangle {
                anchors.fill: parent; anchors.margins: 20
                color: "#F5F4E2"

                Image {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                    }
                    fillMode: Image.PreserveAspectFit
                    source: "../images/logo.png"
                    opacity: 0.2
                }
                ElementsPage {
                    enabled: checkView(3);
                }
            }
        }
    }
}

