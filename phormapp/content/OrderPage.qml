import QtQuick 2.0
import QtQuick 2.3
import QtQml.Models 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import PhormApp 1.0
import "../content"

Item {
    width: parent.width
    height: parent.height
    focus: true

    property var orderPrintDialog: OrderPrintDialog{}
    property var clientsDataDialog: NewClientsDialog{}
    property var newOrder: NewOrderDialog{}
    property var findOrderDialog: FindOrderDialog{}
    property var selectCostumerDialog: SelectCostumerDialog{}

    property int columnWidth: parent.width / 9
    property int  selectSupDelegate: 0

    property bool visibleClientList: false
    property real progress: 0
    property int pixelSize: 22
    property string itemRemove
    property real finishCost: 0.00
    property real deliveryCost: 0.00
    property alias orderCurrentRow: orderTable.currentRow

    property var locale: Qt.locale()
    property string dateString: new Date().toLocaleDateString(Qt.locale("pt_BR"))

    signal updFinishCostEmit
    signal subFinishCostEmit

    function checkOrder() {

        if (nameFieldFirstStage.displayText === "") {
            return -1;
        } else if (orderModel.count === 0) {
            return -2;
        }

        return 0;
    }

    onUpdFinishCostEmit: {

        finishCost += Number(connDatabase.orderHandler.finalCost);
    }

    onSubFinishCostEmit: {
        finishCost -= Number(connDatabase.orderHandler.getOrderCost());
    }

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

    Keys.onReleased: {
        if (event.key === Qt.Key_Insert) {
            newOrder.visible = true;
            newOrder.isRecord = true;
            newOrder.newOrderEmit(orderCurrentRow);
        } else if (event.key === Qt.Key_F2) {

            switch (checkOrder()) {

            case 0:
                orderPrintDialog.visible = true;
                break;

            case -1:
                messageDialog.setText("Cliente não informado!!");
                messageDialog.show();
                break;

            case -2:
                messageDialog.setText("Não há composto na lista!");
                messageDialog.show();
                break;
            }
        }
    }

    MouseArea {
         anchors.fill: parent
         onClicked: {
             orderTable.selection.clear();
             visibleClientList = false;
             console.log("button clicked");
         }
     }

    Component.onCompleted: {
        console.log("Carrega a lista de clientes\n");
        connDatabase.clientHandler.loadClientList();
        connDatabase.localHandler.loadStateList();
    }

    ListModel {
        id: orderModel
        onRowsInserted: {
            orderTable.currentRow++;
        }
    }

    FontLoader { id: fixedFont; name: "Droid Sans" }

    Rectangle {

        anchors {
            right: parent.right
            bottom: parent.bottom
        }

        width: parent.width - 55
        height: parent.height

        color: "transparent"

        Text {
            id: titleFind
            anchors {
                topMargin: 10
                leftMargin: 10
            }
            text: "Pesquisar"
            font.bold: true
            font.pixelSize: 28
            font.family: fixedFont.name
        }

        Rectangle {
            id: searchButtons
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.leftMargin: 20
            anchors.topMargin: 40

            width: 800
            height: 200

            color: "transparent"

            //PESQUISA POR CODIGO
            Image {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: borderOrder.left
                    rightMargin: 10
                }
                source: "../images/find.svg"
                fillMode: Image.PreserveAspectFit;
                width: 32
                height: 32
                enabled: findOrder.text.length
                opacity: enabled ? 1 : 0.5

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Pesquisa por codigo de ordens");
                    }
                }
            }

            BorderImage {
                id: borderOrder
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 16
                source:"../images/textfield.png"
                border.left: 14 ; border.right: 14 ; border.top: 8 ; border.bottom: 8

                TextInput{
                    id: findOrder
                    anchors.fill: parent
                    clip: true
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 22
                    Text {
                        id: placeholderOrder
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        visible: !(parent.text.length || parent.inputMethodComposing)
                        font: parent.font
                        text: "Código..."
                        color: "#aaa"
                    }

                    Image {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                            rightMargin: 1
                        }
                        enabled: findOrder.text.length
                        source: "../images/clear.svg"
                        fillMode: Image.PreserveAspectFit;
                        width: 32
                        height: 32
                        opacity: enabled ? 1 : 0.5

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                findOrder.text = "";
                            }
                        }
                    }

                    onAccepted: {

                        findOrderDialog.visible = true;
                        findOrderDialog.orderID = findOrder.text;
                        findOrderDialog.findFromOrder();

                    }
                }
            }

            //PESQUISA POR CLIENTE
            Image {
                anchors {
                    verticalCenter: borderClient.verticalCenter
                    right: borderClient.left
                    rightMargin: 10
                }
                source: "../images/find.svg"
                fillMode: Image.PreserveAspectFit;
                width: 32
                height: 32
                enabled: findClient.text.length
                opacity: enabled ? 1 : 0.5

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Pesquisa por Cliente");
                    }
                }
            }

            BorderImage {
                id: borderClient
                anchors.left: parent.left
                anchors.top: borderOrder.bottom
                anchors.margins: 16
                source:"../images/textfield.png"
                border.left: 14 ; border.right: 14 ; border.top: 8 ; border.bottom: 8

                TextInput{
                    id: findClient
                    anchors.fill: parent
                    clip: true
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 22
                    Text {
                        id: placeholderClient
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        visible: !(parent.text.length || parent.inputMethodComposing)
                        font: parent.font
                        text: "Cliente..."
                        color: "#aaa"
                    }

                    Image {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                            rightMargin: 1
                        }
                        enabled: findClient.text.length
                        source: "../images/clear.svg"
                        fillMode: Image.PreserveAspectFit;
                        width: 32
                        height: 32
                        opacity: enabled ? 1 : 0.5

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                findClient.text = "";
                            }
                        }
                    }

                    onAccepted: {

                        selectCostumerDialog.visible = true;
                        selectCostumerDialog.costumerName = findClient.text;
                        selectCostumerDialog.showListCostumer();
                    }
                }
            }

            //PESQUISA POR TELEFONE DE CLIENTE
            Image {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: borderPhone.left
                    rightMargin: 10
                }
                source: "../images/find.svg"
                fillMode: Image.PreserveAspectFit;
                width: 32
                height: 32
                enabled: findPhone.text.length
                opacity: enabled ? 1 : 0.5

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Pesquisa por telefone de cliente");
                    }
                }
            }

            BorderImage {
                id: borderPhone
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 16
                source:"../images/textfield.png"
                border.left: 14 ; border.right: 14 ; border.top: 8 ; border.bottom: 8

                TextInput{
                    id: findPhone
                    anchors.fill: parent
                    clip: true
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 22
                    Text {
                        id: placeholderPhone
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        visible: !(parent.text.length || parent.inputMethodComposing)
                        font: parent.font
                        text: "Telefone..."
                        color: "#aaa"
                    }

                    Image {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                            rightMargin: 1
                        }
                        enabled: findPhone.text.length
                        source: "../images/clear.svg"
                        fillMode: Image.PreserveAspectFit;
                        width: 32
                        height: 32
                        opacity: enabled ? 1 : 0.5

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                findPhone.text = "";
                            }
                        }
                    }

                    onAccepted: {
                        findOrderDialog.visible = true;
                        findOrderDialog.phoneID = findPhone.text;
                        findOrderDialog.findFromPhone();
                    }
                }
            }


            //PESQUISA POR DATA DE PEDIDO
            Image {
                anchors {
                    verticalCenter: borderDate.verticalCenter
                    right: borderDate.left
                    rightMargin: 10
                }
                source: "../images/find.svg"
                fillMode: Image.PreserveAspectFit;
                width: 32
                height: 32
                enabled: findDate.text.length
                opacity: enabled ? 1 : 0.5

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Pesquisa por data");
                    }
                }
            }

            BorderImage {
                id: borderDate
                anchors.right: parent.right
                anchors.top: borderPhone.bottom
                anchors.margins: 16
                source:"../images/textfield.png"
                border.left: 14 ; border.right: 14 ; border.top: 8 ; border.bottom: 8

                TextInput{
                    id: findDate
                    anchors.fill: parent
                    clip: true
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 22
                    inputMask: "99/99/99"
                    text: connDatabase.orderHandler.getCurrentDate()
                    Text {
                        id: placeholderDate
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        visible: !(parent.text.length || parent.inputMethodComposing)
                        font: parent.font
                        color: "#aaa"
                    }

                    Image {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                            rightMargin: 1
                        }
                        enabled: findDate.text.length
                        source: "../images/clear.svg"
                        fillMode: Image.PreserveAspectFit;
                        width: 32
                        height: 32
                        opacity: enabled ? 1 : 0.5

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                findDate.text = "";
                            }
                        }
                    }

                    onAccepted: {
                        findOrderDialog.visible = true;
                        findOrderDialog.purchaseDate = findDate.text;
                        findOrderDialog.findFromDate();
                    }
                }
            }
        }

        Image {
            id: divider
            width: parent.width
            anchors {
                top: searchButtons.bottom
                topMargin: 10
            }

            source: "../images/divider-form.png"
            height: 2
        }

        //INFORMAÇÕES DO CLIENTE
        Text {
            id: infoClient
            anchors {
                top: divider.bottom
                topMargin: 10
                leftMargin: 10
            }
            text: "Informações de Clientes"
            font.bold: true
            font.pixelSize: 28
            font.family: fixedFont.name
        }

        Row {
            id: row1
            spacing: 10

            anchors {
                top: infoClient.bottom
                topMargin: 5
                leftMargin: 10
            }

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

                onDisplayTextChanged: {
                    visibleClientList = true
                }
            }

            Image {

                enabled: nameFieldFirstStage.text.length
                source: "../images/edit.svg"
                fillMode: Image.PreserveAspectFit;
                width: 32
                height: 32
                opacity: enabled ? 1 : 0.5

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        clientsDataDialog.name = nameFieldFirstStage.displayText;
                        clientsDataDialog.visible = true;
                    }
                }
            }
        }

        Rectangle {
            id: listClient

            anchors.top: row1.bottom
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 85

            width: columnWidth + 250
            height: 300
            z: 1

            color: "black"
            enabled: visibleClientList
            visible: visibleClientList
            opacity: visibleClientList ? 1 : 0

            Keys.onReleased: {
                if (event.key === Qt.Key_Return) {
                    doctorField.text = proxyModel.get(clientTable.currentRow).name;
                    visibleClientList = false
                }
            }

            onOpacityChanged: SequentialAnimation {
                PropertyAction { target: listClient; property: "height"; value: 0 }
                NumberAnimation { target: listClient; property: "height"; to: 60 * 5; duration: 250; easing.type: Easing.InOutQuad }
            }

            TableView {
                id: clientTable
                anchors.fill: parent

                model: SortFilterProxyModel {
                    id: proxyModel
                    source: clientsModel.count > 0 ? clientsModel : null

                    sortOrder: clientTable.sortIndicatorOrder
                    sortCaseSensitivity: Qt.CaseInsensitive
                    sortRole: doctorsModel.count > 0 ? clientTable.getColumn(clientTable.sortIndicatorColumn).role : ""

                    filterString: "*" + nameFieldFirstStage.text + "*"
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
                        id: selectedClient
                        anchors.fill: parent
                        source: "../images/selectedrow.png"
                        visible: styleData.selected
                        border{left:2; right:2; top:2; bottom:2}
                        SequentialAnimation {
                            running: true; loops: Animation.Infinite
                            NumberAnimation { target:selectedClient; property: "opacity"; to: 1.0; duration: 900}
                            NumberAnimation { target:selectedClient; property: "opacity"; to: 0.5; duration: 900}
                        }
                    }
                }

                onClicked: {
                    nameFieldFirstStage.text = proxyModel.get(clientTable.currentRow).name;
                    visibleClientList = false
                }
            }
        }

        Row {
            id: row2
            spacing: 10

            anchors {
                top: row1.bottom
                topMargin: 5
                leftMargin: 10
            }

            Text {
                anchors.margins: 20
                anchors.verticalCenter: parent.verticalCenter
                text: "Observação:"
                font.pixelSize: pixelSize
                font.family: fixedFont.name
            }

            TextField {
                id: obsField
                anchors.margins: 20
                style: textfieldStyle
                implicitWidth: 520
            }
        }

        //#### TABELA DE VISUALIZACAO DE ITEMS
        TableView {
            id: orderTable
            width: parent.width
            height: 200
            anchors.top: row2.bottom
            anchors.topMargin: 5

            frameVisible: true
            headerVisible: true
            alternatingRowColors: true

            property int currentRow: 0
            property int margins: Qt.platform.os === "osx" ? 16 : 0

            TableViewColumn {
                role: "check"
                title: ""
                width: 32
            }

            TableViewColumn {
                role: "composite"
                title: "Composição"
                width: (parent.width / 3) + (parent.width / 4)
            }

            TableViewColumn {
                role: "cost"
                title: "Custo"
                width: parent.width / 4
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
            model: orderModel

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
                    x: 900

                    visible: styleData.selected
                    Button {
                        id: updateButtom
                        text: "EDITAR"
                        implicitWidth: columnWidth
                        onClicked: {

//                            console.log(orderModel.get(orderTable.currentRow).composite);

                            if (connDatabase.orderHandler.editPurchase(
                                        orderTable.currentRow,
                                        orderModel.get(orderTable.currentRow).composite,
                                        orderTable.rowCount) === true) {
                                newOrder.visible = true;
                                newOrder.isRecord = false;
                                newOrder.editOrder();
                            }
                            orderTable.selection.clear();
                        }
                    }
                    Button {
                        id: removeButtom
                        text: "EXCLUIR"
                        implicitWidth: columnWidth
                        onClicked: {
                            if(connDatabase.orderHandler.itemCancelPurchase(orderTable.currentRow) === true) {
                                orderModel.remove(orderTable.currentRow);
                                orderTable.selection.clear();
                                subFinishCostEmit();
                            }
                        }
                    }
                }
            }

            onClicked: {
                console.log("Index: " + orderTable.currentRow);
            }
        }

        Column {
            id: column1
            spacing: 10

            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: orderTable.bottom
            anchors.topMargin: 5

            Rectangle {
                width: 200
                height: 100
                color: "transparent"
                border.color: "black"
                border.width: 3
                radius: 10

                Column {
                    spacing: 10

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    Row {
                        spacing: 30

                        CheckBox {
                            id: deliveryCheck
                            anchors.margins: 20
                            implicitWidth: columnWidth
                            checked: false
                            style: CheckBoxStyle {
                                indicator: Rectangle {
                                    implicitWidth: 32
                                    implicitHeight: 32
                                    radius: 3
                                    border.color: control.activeFocus ? "darkblue" : "gray"
                                    border.width: 1
                                    Rectangle {
                                        visible: control.checked
                                        color: "#555"
                                        border.color: "#333"
                                        radius: 1
                                        anchors.margins: 4
                                        anchors.fill: parent
                                    }
                                }
                                label: Text {
                                    text: qsTr("Tele-Entrega")
                                    font.pixelSize: 22
                                    font.family: fixedFont.name
                                    font.bold: true
                                }
                            }
                            onClicked: {
                                if (deliveryCheck.checked === false) {
                                    deliveryCostField.text = "0.00"
                                    deliveryCost = Number(deliveryCostField.displayText).toFixed(2);
                                    sumDeliveryText.text = 'R$' + Number(deliveryCostField.displayText).toFixed(2);
                                }
                            }
                        }
                    }

                    Row {
                        spacing: 10

                        Text {
                            anchors.margins: 20
                            anchors.verticalCenter: parent.verticalCenter
                            text: "R$"
                            font.pixelSize: 22
                            font.family: fixedFont.name
                            font.bold: true
                            opacity: deliveryCheck.checked ? 1 : 0.5
                        }

                        TextField {
                            id: deliveryCostField
                            anchors.margins: 20
                            style: textfieldStyle
                            implicitWidth: columnWidth - 70
                            enabled: deliveryCheck.checked ? 1 : 0
                            opacity: deliveryCheck.checked ? 1 : 0.5
                            onAccepted: {
                                deliveryCost = Number(deliveryCostField.displayText).toFixed(2);
                                sumDeliveryText.text = 'R$' + Number(deliveryCostField.displayText).toFixed(2);
                            }
                        }
                    }
                }
            }
        }

        Column {
            id: column2
            spacing: 10

            anchors.right: parent.right
            anchors.rightMargin: 40
            anchors.top: orderTable.bottom
            anchors.topMargin: 5

            Rectangle {
                id: rec1
                width: 200
                height: 100
                color: "white"
                border.color: "black"
                border.width: 3
                radius:10

                Row {
                    id: rowSum
                    spacing: 10

                    anchors.top: rowSum.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 5

                    Text {
                        id: sumResume
                        text: "Soma:"
                        font.pixelSize: 20
                        font.family: fixedFont.name
                        font.bold: true
                        color: "black"
                    }

                    Text {
                        id: sumResumeText
                        text: 'R$' + Number(finishCost).toFixed(2)
                        font.pixelSize: 18
                        font.family: fixedFont.name
                        font.bold: true
                        color: "black"
                    }
                }

                Row {
                    id: rowDelivery
                    spacing: 10

                    anchors.top: rowSum.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 5

                    Text {
                        id: sumDelivery
                        text: "Tele:"
                        font.pixelSize: 20
                        font.family: fixedFont.name
                        font.bold: true
                        color: "black"
                    }

                    Text {
                        id: sumDeliveryText
                        text: "R$ 0.00"
                        font.pixelSize: 18
                        font.family: fixedFont.name
                        font.bold: true
                        color: "black"
                    }
                }

                Row {
                    spacing: 10

                    anchors.top: rowDelivery.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 5

                    Text {
                        id: total
                        text: "Total:"
                        font.pixelSize: 28
                        font.family: fixedFont.name
                        font.bold: true
                        color: "black"
                    }

                    Text {
                        id: totalText
                        text: 'R$' + Number(finishCost + deliveryCost).toFixed(2)
                        font.pixelSize: 24
                        font.family: fixedFont.name
                        font.bold: true
                        color: "black"
                    }
                }
            }
        }

        Button {
            id: cancelOrderButtom
            height: 40
            anchors {
                bottom: orderTable.top
                bottomMargin: 10
                right: newOrderButtom.left
                rightMargin: 10
            }
            opacity: orderModel.count > 0 ? 1 : 0
            enabled: orderModel.count > 0 ? 1 : 0
            text: "[Del] Cancelar"
            style: buttonStyle
            onClicked: {
                connDatabase.orderHandler.cancelPurchase();
                orderModel.clear();
                orderTable.selection.clear();
                orderTable.currentRow = 0;
                visibleClientList = false;
                deliveryCheck.checkedState = 1
                deliveryCostField.text = "";
                sumDeliveryText.text = "R$ 0.00";
                obsField.text = "";
                nameFieldFirstStage.text = "";
                visibleClientList = false;
                finishCost = 0.00;
                deliveryCost = 0.00;
            }
        }

        Button {
            id: newOrderButtom
            height: 40
            anchors {
                bottom: orderTable.top
                bottomMargin: 10
                right: endOrderButtom.left
                rightMargin: 10
            }
            text: "[Ins] Nova Ordem"
            style: buttonStyle
            onClicked: {
                newOrder.visible = true
                newOrder.isRecord = true;
                newOrder.newOrderEmit(orderCurrentRow);
            }
        }

        Button {
            id: endOrderButtom
            height: 40
            anchors {
                bottom: orderTable.top
                bottomMargin: 10
                right: parent.right
                rightMargin: 10
            }
            text: "[F2] Finalizar"
            style: buttonStyle
            onClicked: {

                switch (checkOrder()) {

                case 0:
                    orderPrintDialog.visible = true;
                    break;

                case -1:
                    messageDialog.setText("Cliente não informado!!");
                    messageDialog.show();
                    break;

                case -2:
                    messageDialog.setText("Não há composto na lista!");
                    messageDialog.show();
                    break;
                }
            }
        }
    }

    property Component textfieldStyle: TextFieldStyle {
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
        background: Rectangle {
            implicitHeight: 22
            implicitWidth: columnWidth
            color: control.pressed ? "darkGray" : control.activeFocus ? "#cdd" : "#ccc"
            antialiasing: true
            border.color: "gray"
            radius: height/2
            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"
                antialiasing: true
                visible: !control.pressed
                border.color: "#aaffffff"
                radius: height/2
            }
        }
    }

    property Component buttonRecStyle: ButtonStyle {
        background: Rectangle {
            implicitHeight: 22
            implicitWidth: columnWidth
            color: control.pressed ? "darkGray" : control.activeFocus ? "#cdd" : "#ccc"
            antialiasing: true
            border.color: "gray"
            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"
                antialiasing: true
                visible: !control.pressed
                border.color: "#aaffffff"
            }
        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("Erro!")

        detailedText: "HÁ CAMPOS OBRIGATÓRIOS NÃO PREENCHIDOS!!"

        icon: StandardIcon.Warning

        function show() {
            messageDialog.open();
        }
    }
}

