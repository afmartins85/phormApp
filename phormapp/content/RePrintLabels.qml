import QtQuick 2.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3

Dialog {
    width: 240
    height: 175
    title: "Re impressão de rótulo"

    property int pixelSize: 22

    onVisibleChanged: {
        countValue.value = 1;
    }

    contentItem: Rectangle {
        implicitWidth: 240
        implicitHeight: 175

        color: "#E3E2D2"

        FontLoader { id: fixedFont; name: "Droid Sans" }

        SpinBox {
            id: countValue
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
            font.pixelSize: 28
            font.family: fixedFont.name
            minimumValue: 1
            prefix: "Qtd."
        }

        Rectangle {
            height: 45
            width: parent.width
            anchors.bottom: parent.bottom
            color: "#212126"

            Button {
                id: printBt
                anchors.bottom: parent.bottom
                anchors.right: cancelBt.left
                anchors.margins: 10
                text: "Imprimir"
                onClicked: {
                    connDatabase.orderHandler.loadPrintLabel(
                    connDatabase.orderHandler.getOrderId(),
                                countValue.value)

                    rePrintLabelDialog.close();
                }
            }

            Button {
                id: cancelBt
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 10
                text: "Cancelar"
                onClicked: {
                    rePrintLabelDialog.close();
                }
            }
        }
    }
}

