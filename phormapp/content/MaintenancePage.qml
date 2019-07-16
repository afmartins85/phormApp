import QtQuick 2.3
import QtQml.Models 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.2


Dialog {
    id: maintenancePageDialog

    width: 800
    height: 600
    title: "Manutenção de Tabelas"

//    signal
    contentItem: Rectangle {
        implicitWidth: 800
        implicitHeight: 600
        color: "#212126"

        ListModel {
            id: maintenanceList
            ListElement {
                title: "Formas Farmaceuticas"
                page: "pharmaPage"
            }
            ListElement {
                title: "Unidades de Medida"
                page: "unitPage"
            }
            ListElement {
                title: "Grupos"
                page: "groupsPage"
            }
            ListElement {
                title: "SubGrupos"
                page: "subGroupsPage"
            }
            ListElement {
                title: "Validades"
                page: "validationPage"
            }
            ListElement {
                title: "Excipientes/Veículos"
                page: "excipientPage"
            }
            ListElement {
                title: "Especificação Capsula"
                page: "capSpecPage"
            }
            ListElement {
                title: "Caps. por embalagem"
                page: "capsForPacksPage"
            }
            ListElement {
                title: "Embalagens"
                page: "packsPage"
            }
            ListElement {
                title: "Composição de Preços"
                page: "priceUpPage"
            }
        }

        Rectangle {
            id: items
            anchors.left: parent.left
            width: 180; height: parent.height

            color: "#212126"

            Component {
                id: mainDelegate
                Item {
                    id: titleItem
                    width: items.width; height: 40
                    anchors.margins: 4
                    Text {
                        text: title
                        font.family: "Droid Sans"
                        font.pointSize: 12
                        color: "white"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            itemView.currentIndex = index
                            stack.push(page)
                        }
                    }
                }
            }

            ListView {
                id: itemView
                anchors.fill: parent
                model: maintenanceList
                delegate: mainDelegate
                highlight: Rectangle {
                    color: "green"
                    radius: 4
                    y: itemView.currentItem.y
                    Behavior on y {
                        SpringAnimation {
                            spring: 3
                            damping: 0.2
                        }
                    }
                }
                focus: true
            }
        } //Menu List

        Rectangle {
            width: 800 - items.width
            height: parent.height
            anchors.right: parent.right

            Rectangle {
                id: stack
                property Item currentPage

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: parent.height

                function push(item) {

                    var objId;
                    if (item === "pharmaPage") {
                        objId = pharmaPage
                    } else if (item === "unitPage") {
                        objId = unitPage
                    } else if (item === "groupsPage") {
                        objId = groupsPage
                    } else if (item === "subGroupsPage") {
                        objId = subGroupsPage
                    } else if (item === "validationPage") {
                        objId = validationPage
                    } else if (item === "excipientPage") {
                        objId = excipientPage
                    } else if (item === "capSpecPage") {
                        objId = capSpecPage
                    } else if (item === "capsForPacksPage") {
                        objId = capsForPacksPage
                    } else if (item === "packsPage") {
                        objId = packsPage
                    } else if (item === "priceUpPage") {
                        objId = priceUpPage
                    }


                    if(currentPage !== null)
                        currentPage.opacity = 0
                    currentPage = objId
                    objId.opacity = 1
                }

                function check (i) {
                    if (itemView.currentIndex === i) {
                        return true;
                    }
                    return false;
                }

                PhormPharmaData {
                    opacity: 0
                    id: pharmaPage
                    anchors.fill: stack
                    enabled: stack.check(0)
                }

                UnidadesData {
                    opacity: 0
                    id: unitPage
                    anchors.fill: stack
                    enabled: stack.check(1)
                }

                GroupsData {
                    opacity: 0
                    id: groupsPage
                    anchors.fill: stack
                    enabled: stack.check(2)
                }

                SubGroupData {
                    opacity: 0
                    id: subGroupsPage
                    anchors.fill: stack
                    enabled: stack.check(3)
                }

                ValidationData {
                    opacity: 0
                    id: validationPage
                    anchors.fill: stack
                    enabled: stack.check(4)
                }

                ExcipientData {
                    opacity: 0
                    id: excipientPage
                    anchors.fill: stack
                    enabled: stack.check(5)
                }

                CapSpecData {
                    opacity: 0
                    id: capSpecPage
                    anchors.fill: stack
                    enabled: stack.check(6)
                }

                CapsForPacksData {
                    opacity: 0
                    id: capsForPacksPage
                    anchors.fill: stack
                    enabled: stack.check(7)
                }

                PacksData {
                    opacity: 0
                    id: packsPage
                    anchors.fill: stack
                    enabled: stack.check(8)
                }

                PriceUpData {
                    opacity: 0
                    id: priceUpPage
                    anchors.fill: stack
                    enabled: stack.check(9)
                }

                Component.onCompleted: {
                    stack.push("pharmaPage")
                }
            }
        }

        Button {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.leftMargin: 20
            height: 30
            text: "      Fechar      "
            style: buttonStyle
            onClicked: {
                maintenancePageDialog.close();
            }
        }
    }

    property Component buttonStyle: ButtonStyle {
        background: Rectangle {
            implicitHeight: 22
            width: 60
//            implicitWidth: 60
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

