import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3

Rectangle {
    id: root
    width: 100
    height: 62

    //Carrega a lista de embalagens
    ListModel {
        id: packagesBoxModel
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadAllCapsList();
            for (var i=0 ; i< connDatabase.maintenanceHandler.capsCount(); ++i)
                packagesBoxModel.append({"title": connDatabase.maintenanceHandler.packListAt(i)})
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
            model: packagesBoxModel

            onIndexChange: {
                //Limpa a lista
                capsForPacksModel.clear();
                //Carrega o id do grupo selecionado
                connDatabase.maintenanceHandler.packId = packagesBoxModel.get(stateBox.currentIndex).title;
                //Carrega os items de sub-grupo relacionados ao grupo
                connDatabase.maintenanceHandler.loadCapsForPackList(connDatabase.maintenanceHandler.getPackId());
                for (var i=0 ; i< connDatabase.maintenanceHandler.capsForPackCount(); ++i) {

                    capsForPacksModel.append({
                         "title": connDatabase.maintenanceHandler.capacityListAt(i),
                         "capsula": connDatabase.maintenanceHandler.capsForPackListAt(i)
                    })
                }
            }
        }

        Rectangle {
            width: parent.width ; height: 1
            anchors.bottom: parent.bottom
            color: "#bbb"
        }
    }

    //![view]
    ListView {
        id: listview
        model: capsForPacksModel
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

            id: capsInfo
            width: 130
            anchors.right: addButton.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 16
            source:"../images/textfield.png"
            border.left: 22 ; border.right: 22 ; border.top: 8 ; border.bottom: 8

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
                    text: "Qtd. (un)..."
                    color: "#aaa"
                }
                onAccepted: {

                    capsForPacksModel.append({
                         "title": textInput.text,
                         "capsula": specCapsModel.get(capsBox.currentIndex).title
                    })

                    connDatabase.maintenanceHandler.packId = packagesBoxModel.get(stateBox.currentIndex).title;

                    connDatabase.maintenanceHandler.qtdCaps = textInput.text;
                    connDatabase.maintenanceHandler.capsForPack = specCapsModel.get(capsBox.currentIndex).title;

                    console.log(textInput.text);
                    console.log(specCapsModel.get(capsBox.currentIndex).title);
                    textInput.text = ""
                }
            }
        }

        BorderImage {

            id: capsBoxInfo
            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.top: parent.top
            anchors.verticalCenter: parent.verticalCenter

            Box {
                id: capsBox
                anchors.margins: 16
                anchors.top: parent.top
                boxHeight: 40; implicitWidth: 360
                model: specCapsModel
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
                        capsForPacksModel.setProperty(index, "completed", !completed)
                    }
                }
            }

            //![delegate-properties]
            Text {
                id: todoText
                text: capsula
                font.pixelSize: 18
                color: "#333"

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 12
                anchors.rightMargin: 40
                elide: Text.ElideRight
            }

            Text {
                text: title + " un"
                font.pixelSize: 20
                color: "#333"

                anchors.verticalCenter: parent.verticalCenter
                anchors.right: removeIcon.left
                anchors.leftMargin: 10
                anchors.rightMargin: 10
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
                        connDatabase.maintenanceHandler.capsForPackListRemove(capsForPacksModel.get(index).capsula);
                        capsForPacksModel.remove(index)
                    }
                }
                //![remove]
            }
            //![sync]
        }
    }
}
