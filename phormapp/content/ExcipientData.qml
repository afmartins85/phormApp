import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import PhormApp 1.0

Rectangle {
    id: root
    width: 100
    height: 62

    property bool visibleElementList: false
    signal excicpShow

    onExcicpShow: {
        connDatabase.maintenanceHandler.loadExcipientList(connDatabase.maintenanceHandler.getFirstPhormId());
        for (var i=0 ; i< connDatabase.maintenanceHandler.excipientCount(); ++i)
            excipentModel.append({"title": connDatabase.maintenanceHandler.excipientListAt(i)})

        connDatabase.maintenanceHandler.loadPhormList();
        for (i=0 ; i< connDatabase.maintenanceHandler.phormListCount(); ++i)
            phormModel.append({"title": connDatabase.maintenanceHandler.phormListAt(i)})

        connDatabase.elementHandler.loadElementList();
        for (i=0 ; i< connDatabase.elementHandler.elementListCount() ; ++i)
            elementModel.append({"name":connDatabase.elementHandler.elementListAt(i)})
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
                excipentModel.clear();
                //Carrega o id da formula selecionada
                connDatabase.maintenanceHandler.phormId = phormModel.get(stateBox.currentIndex).title;
                //Carrega os items de sub-grupo relacionados ao grupo
                connDatabase.maintenanceHandler.loadExcipientList(connDatabase.maintenanceHandler.getPhormId());
                for (var i=0 ; i< connDatabase.maintenanceHandler.excipientCount(); ++i)
                    excipentModel.append({"title": connDatabase.maintenanceHandler.excipientListAt(i)})
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
        model: excipentModel
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
                    text: "Novo Excipiente..."
                    color: "#aaa"
                }
                onTextChanged: {

                    if (textInput.text === "") {
                        visibleElementList = false
                    } else {
                        visibleElementList = true
                    }
                }

                onAccepted: {
                    excipentModel.append({"title": textInput.text, "completed": false})

                    connDatabase.maintenanceHandler.phormId =
                            connDatabase.maintenanceHandler.phormListAt(stateBox.currentIndex);

                    connDatabase.maintenanceHandler.excipient = textInput.text;
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
//                    if (index !== -1 && _synced) {
//                        excipentModel.setProperty(index, "completed", !completed)
//                    }
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
                        connDatabase.maintenanceHandler.excipientRemove(excipentModel.get(index).title);
                        excipentModel.remove(index)
                    }
                }
                //![remove]
            }
            //![sync]
        }
    }

    Rectangle {
        id: listElementBase

        anchors.bottom: footer.top

        width: 250
        height: 100
        z: 1

        color: "black"
        enabled: visibleElementList
        visible: visibleElementList
        opacity: visibleElementList ? 1 : 0

        Keys.onReleased: {
            if (event.key === Qt.Key_Return) {
                textInput.text = elementProxyModel.get(elementTable.currentRow).name;
                visibleElementList = false;
            }
        }

        onOpacityChanged: SequentialAnimation {
            PropertyAction { target: listElementBase; property: "height"; value: 0 }
            NumberAnimation { target: listElementBase; property: "height"; to: 60 * 5; duration: 250; easing.type: Easing.InOutQuad }
        }

        TableView {
            id: elementTable
            anchors.fill: parent

            model: SortFilterProxyModel {
                id: elementProxyModel
                source: elementModel.count > 0 ? elementModel : null

                sortOrder: elementTable.sortIndicatorOrder
                sortCaseSensitivity: Qt.CaseInsensitive
                sortRole: elementModel.count > 0 ? elementTable.getColumn(elementTable.sortIndicatorColumn).role : ""

                filterString: "*" + textInput.text + "*"
                filterSyntax: SortFilterProxyModel.Wildcard
                filterCaseSensitivity: Qt.CaseInsensitive
            }

            TableViewColumn {
                role: "name"
                width: parent.width
            }

            rowDelegate: Rectangle {
                height: 30
                Behavior on height{ NumberAnimation{} }

                color: styleData.selected ? "#448" : (styleData.alternate? "#eee" : "#fff")
                BorderImage{
                    id: selected3
                    anchors.fill: parent
                    source: "../images/selectedrow.png"
                    visible: styleData.selected
                    border{left:2; right:2; top:2; bottom:2}
                    SequentialAnimation {
                        running: true; loops: Animation.Infinite
                        NumberAnimation { target:selected3; property: "opacity"; to: 1.0; duration: 900}
                        NumberAnimation { target:selected3; property: "opacity"; to: 0.5; duration: 900}
                    }
                }
            }

            onClicked: {
                textInput.text = elementProxyModel.get(elementTable.currentRow).name;
                visibleElementList = false
            }
        }
    }
}

