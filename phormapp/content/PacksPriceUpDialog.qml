import QtQuick 2.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3

Dialog {
    width: 480
    height: 350
    title: "Custo da embalagem"

    property int pixelSize: 22
    property alias item: itemTitle.text

    onVisibleChanged: {

        if (connDatabase.maintenanceHandler.getPackPriceUpData(item) === true) {
            factorCostPack.text = connDatabase.maintenanceHandler.packFactor;
            packCost.text = connDatabase.maintenanceHandler.costPack;
        } else {
            factorCostPack.text = "";
            packCost.text = "";
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
                    text: "Preço da embalagem: R$"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: packCost
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
                    text: "Fator preço:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: factorCostPack
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

                    connDatabase.maintenanceHandler.packFactor = factorCostPack.text
                    connDatabase.maintenanceHandler.costPack = packCost.text

                    if ((connDatabase.maintenanceHandler.packPriceUPInsert = item) === true) {
                        factorCostPack.text = "";
                        packCost.text = "";
                        packPriceUpDialog.close();

                    } else {
                        packPriceUpDialog.close();
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

