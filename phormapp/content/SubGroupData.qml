import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3

Rectangle {
    id: root
    width: 100
    height: 62

    ListModel {
        id: subGroupsModel
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadSubGroupList(connDatabase.maintenanceHandler.getFirstGroupId());
            for (var i=0 ; i< connDatabase.maintenanceHandler.subGroupListCount(); ++i)
                subGroupsModel.append({"title": connDatabase.maintenanceHandler.subGroupListAt(i)})
        }
    }

    ListModel {
        id: groupsBoxModel
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadGroupList();
            for (var i=0 ; i< connDatabase.maintenanceHandler.groupListCount(); ++i)
                groupsBoxModel.append({"title": connDatabase.maintenanceHandler.groupListAt(i)})
        }
    }

    Rectangle {
        id: header
        anchors.top: parent.top
        width: parent.width
        height: 70
        color: "white"

        Box {
            id: stateBox
            boxHeight: parent.height
            anchors.margins: 10
            implicitWidth: parent.width
            model: groupsBoxModel

            onIndexChange: {
                //Limpa a lista
                subGroupsModel.clear();
                //Carrega o id do grupo selecionado
                connDatabase.maintenanceHandler.groupId = groupsBoxModel.get(stateBox.currentIndex).title;
                //Carrega os items de sub-grupo relacionados ao grupo
                connDatabase.maintenanceHandler.loadSubGroupList(connDatabase.maintenanceHandler.getGroupId());
                for (var i=0 ; i< connDatabase.maintenanceHandler.subGroupListCount(); ++i)
                    subGroupsModel.append({"title": connDatabase.maintenanceHandler.subGroupListAt(i)})
            }
        }

        Rectangle {
            width: parent.width ; height: 1
            anchors.bottom: parent.bottom
            color: "#bbb"
        }

        Component.onCompleted: {
            console.log("ComboBox pronto");
        }
    }

    //![view]
    ListView {
        id: listview
        model: subGroupsModel
        delegate: listItemDelegate
        anchors.top: header.bottom
        anchors.bottom: footer.top
        width: parent.width
        clip: true

        // Animations
        add: Transition { NumberAnimation { properties: "y"; from: root.height; duration: 250 } }
        removeDisplaced: Transition { NumberAnimation { properties: "y"; duration: 150 } }
        remove: Transition { NumberAnimation { property: "opacity"; to: 0; duration: 150 } }
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

        //![append]

        BorderImage {

            anchors.left: parent.left
            anchors.right: addButton.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 16
            source:"../images/textfield.png"
            border.left: 14 ; border.right: 14 ; border.top: 8 ; border.bottom: 8

            TextInput{
                id: textInput
                anchors.fill: parent
                clip: true
                anchors.leftMargin: 14
                anchors.rightMargin: 14
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 22
                Text {
                    id: placeholderText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    visible: !(parent.text.length || parent.inputMethodComposing)
                    font: parent.font
                    text: "Novo Sub Grupo..."
                    color: "#aaa"
                }
                onAccepted: {
                    subGroupsModel.append({"title": textInput.text, "completed": false})

                    connDatabase.maintenanceHandler.groupId =
                            connDatabase.maintenanceHandler.groupListAt(stateBox.currentIndex);

                    connDatabase.maintenanceHandler.subGroup = textInput.text;
                    console.log(textInput.text);
                    textInput.text = ""
                }
            }
        }

        Item {
            id: addButton

            width: 40 ; height: 40
            anchors.margins: 20
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            enabled: textInput.text.length
            Image {
                source: addMouseArea.pressed ? "../images/add_icon_pressed.png" : "../images/add_icon.png"
                anchors.centerIn: parent
                opacity: enabled ? 1 : 0.5
            }
            MouseArea {
                id: addMouseArea
                anchors.fill: parent
                onClicked: textInput.accepted()
            }
        }
    }
    //![append]

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

            //![setProperty]
            MouseArea {
                id: mouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    if (index !== -1 && _synced) {
                        subGroupsModel.setProperty(index, "completed", !completed)
                    }
                }
            }

            //![delegate-properties]
            Text {
                id: todoText
                text: title
                font.pixelSize: 26
                color: "#333"

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 12
                anchors.rightMargin: 40
                elide: Text.ElideRight
            }
            //![delegate-properties]

            // Show a delete button when the mouse is over the delegate
            //![sync]
            Image {
                id: removeIcon

                source: removeMouseArea.pressed ? "../images/delete_icon_pressed.png" : "../images/delete_icon.png"
                anchors.margins: 20
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                opacity: enabled ? 1 : 0.5
                Behavior on opacity {NumberAnimation{duration: 100}}
                //![remove]
                MouseArea {
                    id: removeMouseArea
                    anchors.fill: parent
                    onClicked: {
                        connDatabase.maintenanceHandler.subGroupListRemove(subGroupsModel.get(index).title);
                        subGroupsModel.remove(index)
                    }
                }
                //![remove]
            }
            //![sync]
        }
    }
}

