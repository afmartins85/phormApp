import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2

Rectangle {
    id: root
    width: 100
    height: 62

    property var packPriceUpDialog: PacksPriceUpDialog {}

    ListModel {
        id: packageModel
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadPackList(connDatabase.maintenanceHandler.getFirstPhormId());
            for (var i=0 ; i< connDatabase.maintenanceHandler.packCount(); ++i) {
                packageModel.append({
                     "title": connDatabase.maintenanceHandler.packListAt(i),
                     "volume": connDatabase.maintenanceHandler.volumePackListAt(i),
                     "rotulo": connDatabase.maintenanceHandler.labelSizeListAt(i)
                })
            }
        }
    }

    ListModel {
        id: phormModel
        Component.onCompleted: {
            connDatabase.maintenanceHandler.loadPhormList();
            for (var i=0 ; i< connDatabase.maintenanceHandler.phormListCount(); ++i)
              phormModel.append({"title": connDatabase.maintenanceHandler.phormListAt(i)})
        }
    }

    ListModel {
        id: labelSizeModel
        Component.onCompleted: {
              labelSizeModel.append({"title": "PEQ."});
              labelSizeModel.append({"title": "MED."});
              labelSizeModel.append({"title": "GR."});
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
            model: phormModel

            onIndexChange: {
                //Limpa a lista
                packageModel.clear();
                //Carrega o id da formula selecionada
                connDatabase.maintenanceHandler.phormId = phormModel.get(stateBox.currentIndex).title;
                //Carrega os items de sub-grupo relacionados ao grupo
                connDatabase.maintenanceHandler.loadPackList(connDatabase.maintenanceHandler.getPhormId());
                for (var i=0 ; i< connDatabase.maintenanceHandler.packCount(); ++i) {
                    packageModel.append({
                        "title": connDatabase.maintenanceHandler.packListAt(i),
                        "volume": connDatabase.maintenanceHandler.volumePackListAt(i),
                        "rotulo": connDatabase.maintenanceHandler.labelSizeListAt(i)
                    })
                }
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
        model: packageModel
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

        Row {

            spacing: 15
            anchors.verticalCenter: parent.verticalCenter
            //![append]
            BorderImage {

                id: nameInfo
                anchors.margins: 16
                source:"../images/textfield.png"
                border.left: 14 ; border.right: 14 ; border.top: 8 ; border.bottom: 8

                TextInput{
                    id: textInputName
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
                        text: "Nova embalagem..."
                        color: "#aaa"
                    }
                    onAccepted: {

                        if (textInputVolume.text !== "") {

                            packageModel.append({
                                 "title": textInputName.text,
                                 "volume": textInputVolume.text,
                                 "rotulo": labelSizeModel.get(labelBox.currentIndex).title

                            })
                            connDatabase.maintenanceHandler.phormId = phormModel.get(stateBox.currentIndex).title;

                            connDatabase.maintenanceHandler.labelSize = labelBox.currentIndex;
                            connDatabase.maintenanceHandler.VolumePack = textInputVolume.text;
                            connDatabase.maintenanceHandler.packageName = textInputName.text;

                            console.log(textInputName.text);
                            console.log(textInputVolume.text);
                            console.log(labelSizeModel.get(labelBox.currentIndex).title);
                            textInputName.text = ""
                            textInputVolume.text = ""
                            labelBox.currentIndex = 0;

                        } else {
                            messageError.show();
                        }
                    }
                }
            }

            BorderImage {

                id: volumeInfo
                width: 130
                anchors.margins: 16
                source:"../images/textfield.png"
                border.left: 18 ; border.right: 18 ; border.top: 8 ; border.bottom: 8

                TextInput{
                    id: textInputVolume
                    anchors.fill: parent
                    clip: true
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 14
                    Text {
                        id: placeholderTextVolume
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        visible: !(parent.text.length || parent.inputMethodComposing)
                        font: parent.font
                        text: "Vol.(ml)..."
                        color: "#aaa"
                    }
                    onAccepted: {

                        if (textInputName.text !== "") {

                            packageModel.append({
                                 "title": textInputName.text,
                                 "volume": textInputVolume.text,
                                 "rotulo": labelSizeModel.get(labelBox.currentIndex).title

                            })

                            connDatabase.maintenanceHandler.phormId = phormModel.get(stateBox.currentIndex).title;

                            connDatabase.maintenanceHandler.labelSize = labelBox.currentIndex;
                            connDatabase.maintenanceHandler.VolumePack = textInputVolume.text;
                            connDatabase.maintenanceHandler.packageName = textInputName.text;

                            console.log(textInputName.text);
                            console.log(textInputVolume.text);
                            console.log(labelSizeModel.get(labelBox.currentIndex).title);
                            textInputName.text = ""
                            textInputVolume.text = ""
                            labelBox.currentIndex = 0;

                        } else {
                            messageError.show();
                        }
                    }
                }
            }

            Box {
                id: labelBox
                anchors.margins: 20
                boxHeight: 40; implicitWidth: 60
                anchors.left: volumeInfo.right
                model: labelSizeModel
            }
        }

        Item {
            id: addButton

            width: 40 ; height: 40
            anchors.margins: 20
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            enabled: textInputName.text.length
            Image {
                source: addMouseArea.pressed ? "../images/add_icon_pressed.png" : "../images/add_icon.png"
                anchors.centerIn: parent
                opacity: enabled ? 1 : 0.5
            }
            MouseArea {
                id: addMouseArea
                anchors.fill: parent
                onClicked: textInputName.accepted()
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
                    if (index !== -1) {
                        packPriceUpDialog.item = packageModel.get(index).title;
                        packPriceUpDialog.visible = !packPriceUpDialog.visible;
                    }
                }
            }

            //![delegate-properties]
            Text {
                id: todoText
                text: title
                font.pixelSize: 22
                color: "#333"

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 12
                anchors.rightMargin: 40
                elide: Text.ElideRight
            }

            Text {
                text: "Vol.: " + volume + " ml" + "  Rotulo: " + rotulo
                font.pixelSize: 14
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
                        connDatabase.maintenanceHandler.packRemove(packageModel.get(index).title);
                        packageModel.remove(index)
                    }
                }
                //![remove]
            }
            //![sync]
        }
    }

    MessageDialog {
        id: messageError
        title: qsTr("Erro!")

        informativeText: "Campos não preenchidos!!"
        detailedText: "Verifique se todos os campos foram preenchidos corretamente!"

        icon: StandardIcon.Critical

        function show() {
            messageError.open();
        }
    }
}

