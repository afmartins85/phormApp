import QtQuick 2.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3

Dialog {
    width: 480
    height: 350
    title: "Composição de Preço"

    property int pixelSize: 22
    property alias item: itemTitle.text

    onVisibleChanged: {

        if (connDatabase.maintenanceHandler.getPriceUpData(item) === true) {

            qtdIni.text = connDatabase.maintenanceHandler.priceItemInit;
            qtdEnd.text = connDatabase.maintenanceHandler.priceItemEnd;
            cost.text = connDatabase.maintenanceHandler.costManipulation;
            factor.text = connDatabase.maintenanceHandler.profitFactor;
            minCost.text = connDatabase.maintenanceHandler.minPrice;
        } else {
            qtdIni.text = "";
            qtdEnd.text = "";
            cost.text = "";
            factor.text = "";
            minCost.text = "";
        }
    }

    contentItem: Rectangle {
        implicitWidth: 480
        implicitHeight: 350

        color: "#E3E2D2"

        FontLoader { id: fixedFont; name: "Droid Sans" }

        Text {
            id: itemTitle
            anchors.margins: 20
            anchors.horizontalCenter: parent.horizontalCenter
//            text: "TITULO"
            font.pixelSize: 32
            font.family: fixedFont.name
        }

        Column {
            id: priceUpForm
            spacing: 10
            anchors {
                top: parent.top
                topMargin: 40
                left: parent.left
                leftMargin: 20
            }
            width: parent.width

            Row {
                spacing: 10

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Nº Items: de"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: qtdIni
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 60
                    inputMask: "999"
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "a "
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: qtdEnd
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 60
                    inputMask: "999"
                }
            }

            Row {
                spacing: 10

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Custo Manip: R$"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: cost
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 100
                }
            }

            Row {
                spacing: 10

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Fator lucro:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: factor
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 100
                }
            }

            Row {
                spacing: 10

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Preço mín.: R$"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: minCost
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 100
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

                    connDatabase.maintenanceHandler.priceItemInit = qtdIni.text
                    connDatabase.maintenanceHandler.priceItemEnd = qtdEnd.text
                    connDatabase.maintenanceHandler.costManipulation = cost.text
                    connDatabase.maintenanceHandler.profitFactor = factor.text
                    connDatabase.maintenanceHandler.minPrice = minCost.text

                    if ((connDatabase.maintenanceHandler.priceUPInsert = item) === true) {

                        qtdIni.text = "";
                        qtdEnd.text = "";
                        cost.text = "";
                        factor.text = "";
                        minCost.text = "";
                        priceUpDialog.close();

                    } else {

                        priceUpDialog.close();
                    }
                }
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

