import QtQuick 2.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3

Dialog {
    id: printSetupDialog
    width: 480
    height: 350
    title: "Definir Impressoras"

    property int pixelSize: 22
    property alias item: itemTitle.text

    property int boxHeightSmall: 40
    property int boxWidthSmall: 60
    property int boxHeightLarge: 40
    property int boxWidthLarge: 220

    contentItem: Rectangle {
        implicitWidth: 480
        implicitHeight: 350

        color: "#E3E2D2"

        FontLoader { id: fixedFont; name: "Droid Sans" }

        Column {
            spacing: 20

            anchors {
                top: parent.top
                topMargin: 10
            }
            width: parent.width

            Text {
                id: itemTitle
                anchors.margins: 20
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Configuração de Impressão"
                font.pixelSize: 32
                font.family: fixedFont.name
            }

            Row {
                spacing: 15

                Text {
                    id: label
                    text: "Rótulos:  "
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Box {
                    id: orderBox
                    boxHeight: boxHeightLarge
                    anchors.left: label.right
                    implicitWidth: boxWidthLarge
                    model: printerModel

                    onIndexChange: {
                    }
                }
            }

            Row {
                spacing: 10

                Text {
                    id: tag
                    text: "Etiquetas: "
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Box {
                    id: labelBox
                    boxHeight: boxHeightLarge
                    anchors.left: tag.right
                    implicitWidth: boxWidthLarge
                    model: printerModel

                    onIndexChange: {
                    }
                }
            }
        }

        Rectangle {

            height: 45
            width: parent.width

            anchors.bottom: parent.bottom

            color: "#212126"

            Button {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 10
                text: "Fechar"
                onClicked: {

                    if (connDatabase.ptrSetup.setPrinters(printerModel.get(labelBox.currentIndex).printer,
                                                      printerModel.get(orderBox.currentIndex).printer) === false) {

                        messageDialog.setText("NÃO FOI POSSIVEL CONFIGURAR A IMPRESSORA!");
                        messageDialog.show();
                        printSetupDialog.close();
                    } else {

                        connDatabase.orderHandler.loadPrintConfig();
                        printerSelectDialog.visible = !printerSelectDialog.visible;
                        printerSelectDialog.close();

                    }
                }
            }
        }

        MessageDialog {
            id: messageDialog
            title: qsTr("Erro!")

            detailedText: "VERIFIQUE A CONEXÃO COM O SERVIDOR!"

            icon: StandardIcon.Warning

            function show() {
                messageDialog.open();
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
}

