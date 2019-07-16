
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import PhormApp 1.0

Item {
    width: parent.width
    height: parent.height

    property var clientsDataDialog: NewClientsDialog{}

    property int columnWidth: parent.width / 9
    property int  selectSupDelegate: 0

    property real progress: 0
    property int pixelSize: 22
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
        id: clientsForm
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
                        clientsDataDialog.name = nameFieldFirstStage.displayText;
                        clientsDataDialog.visible = true;
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
        id: tableClients
        y: 200
        width: parent.width
        height: parent.height
        frameVisible: true
        headerVisible: true
        alternatingRowColors: true
        selectionMode: SelectionMode.SingleSelection

        property int margins: Qt.platform.os === "osx" ? 16 : 0

        TableViewColumn {
            role: "name"
            title: "Nome"
            width: 420
        }
        TableViewColumn {
            role: "fone_1"
            title: "Telefone 1"
            width: 120
        }
        TableViewColumn {
            role: "fone_2"
            title: "Telefone 2"
            width: 120
        }

        model: SortFilterProxyModel {
           id: proxyModel
           source: clientsModel.count > 0 ? clientsModel : null

           sortOrder: tableClients.sortIndicatorOrder
           sortCaseSensitivity: Qt.CaseInsensitive
           sortRole: clientsModel.count > 0 ? tableClients.getColumn(tableClients.sortIndicatorColumn).role : ""

           filterString: "*" + nameFieldFirstStage.text + "*"
           filterSyntax: SortFilterProxyModel.Wildcard
           filterCaseSensitivity: Qt.CaseInsensitive
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
                        clientsDataDialog.name = proxyModel.get(styleData.row).name;
                        clientsDataDialog.visible = true;

                        clientsDataDialog.editForm();
                        clientsDataDialog.isUpdate = true;
                    }
                }
                Button {
                    id: removeButtom
                    text: "EXCLUIR"
                    implicitWidth: columnWidth
                    onClicked: {

                        console.log("REMOVE Nome: " + proxyModel.get(styleData.row).name);
                        console.log("REMOVE index: " + styleData.row);
                        itemRemove = proxyModel.get(styleData.row).name;
                        connDatabase.clientHandler.remove(itemRemove);

                        //TODO: Criar um método de reload
                        clientsModel.clear();
                        connDatabase.clientHandler.loadClientList();

                        for (var i=0 ; i< connDatabase.clientHandler.clientListCount() ; ++i)
                            clientsModel.append({"name":connDatabase.clientHandler.clientListAt(i),
                                                    "fone_1": connDatabase.clientHandler.firstPhoneAt(i),
                                                    "fone_2": connDatabase.clientHandler.secPhoneAt(i)})
                    }
                }
            }
        }

        onClicked: {
            console.log("linha: " + row)
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

    property Component buttonStyle: ButtonStyle {
        panel: Item {
            implicitHeight: 50
            BorderImage {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                antialiasing: true
                border.bottom: 8
                border.top: 8
                border.left: 8
                border.right: 8
                source: selectSupDelegate ? "../images/enter_pressed.png" : "../images/button_default.png"
                Text {
                    text: control.text
                    anchors.centerIn: parent
                    color: "Black"
                    font.pixelSize: 20
                    font.bold: true
                    renderType: Text.NativeRendering
                }
            }
        }
    }
}
