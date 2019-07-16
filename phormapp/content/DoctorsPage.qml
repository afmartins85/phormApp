import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2

Item {
    width: parent.width
    height: parent.height

    property var doctorsDataDialog: NewDoctorsDialog{}

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

//    ListModel {
//        id: doctorsModel
//        Component.onCompleted: {
//            connDatabase.doctorHandler.loadDoctorList();

//            for (var i=0 ; i< connDatabase.doctorHandler.doctorListCount() ; ++i)
//                doctorsModel.append({"name":connDatabase.doctorHandler.doctorListAt(i),
//                                        "crm": connDatabase.doctorHandler.crmListAt(i),
//                                        "fone_1": connDatabase.doctorHandler.phoneListAt(i)})
//        }
//    }

    //######################################################################################
    // INICIO DO FORMULARIO

    Column {
        id: doctorsForm
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
                text: "CRM:"
                font.pixelSize: 18
                font.family: fixedFont.name
            }

            TextField {
                id: crmField
                anchors.margins: 20
//                inputMask: "000000000000000"
                style: textfieldStyle
                implicitWidth: columnWidth + 160
            }

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
                inputMethodHints: Qt.ImhUppercaseOnly

                onTextChanged: {

                    doctorsModel.clear();
                    connDatabase.doctorHandler.filterDoctorList(nameFieldFirstStage.text);

                    if (connDatabase.doctorHandler.doctorListCountSwap() > 0) {

                        for (var i=0 ; i< connDatabase.doctorHandler.doctorListCountSwap() ; ++i)
                            doctorsModel.append({"name":connDatabase.doctorHandler.doctorListSwapAt(i),
                                                    "crm": connDatabase.doctorHandler.crmListSwapAt(i),
                                                    "fone_1": connDatabase.doctorHandler.phoneListSwapAt(i)})

                    } else {

                        for (var j=0 ; j< connDatabase.doctorHandler.doctorListCount() ; ++j)
                            doctorsModel.append({"name":connDatabase.doctorHandler.doctorListAt(j),
                                                    "crm": connDatabase.doctorHandler.crmListAt(j),
                                                    "fone_1": connDatabase.doctorHandler.phoneListAt(j)})
                    }
                }
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
                        doctorsDataDialog.crm = crmField.displayText;
                        doctorsDataDialog.name = nameFieldFirstStage.displayText;
                        doctorsDataDialog.visible = true;
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

        Button {
            id: editButtom
            anchors.rightMargin: 10
            text: "EDITAR"
            style: buttonStyle
            implicitWidth: columnWidth
            onClicked: {

                if (selectSupDelegate === 0) {
                    selectSupDelegate = 1;
                    editButtom.text = "OK"
                } else {
                    selectSupDelegate = 0;
                    editButtom.text = "EDITAR"
                }
            }
        }
    }

    TableView {
        id: tableDoctors
        model: doctorsModel
//        anchors.top: elementForm.bottom
        y: 200
        width: parent.width
//        height: (parent.height / 3) + 50
        height: parent.height
        frameVisible: true
        headerVisible: true
        alternatingRowColors: true

        property int margins: Qt.platform.os === "osx" ? 16 : 0

        TableViewColumn {
            role: "name"
            title: "Nome"
            width: 320
        }
        TableViewColumn {
            role: "crm"
            title: "CRM"
            width: 120
        }
        TableViewColumn {
            role: "fone_1"
            title: "Tel. Contato 1"
            width: 120
        }

        TableViewColumn {
            width: 32
            delegate: delDelegate
            visible: selectSupDelegate ? true : false
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
        }
    }

    Component {
        id: delDelegate
        Button {
            width: 32
            height: 32
            BorderImage{
                id: rmIcon
                source: "../images/remove.png"
            }
            onClicked: {
                console.log("Linha: " + doctorsModel.get(tableDoctors.currentRow).crm);
                itemRemove = doctorsModel.get(tableDoctors.currentRow).crm;
                doctorsModel.remove(tableDoctors.currentRow, 1);
                connDatabase.doctorHandler.remove(itemRemove);
            }
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
                //                anchors.margins: control.pressed ? -4 : 0
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

