/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import PhormApp 1.0

Dialog {
    id: endOrderDialog

    width: 800
    height: 520
    title: "Nova Ordem de Produção"
    modality: Qt.ApplicationModal

    property int boxHeightSmall: 40
    property int boxWidthSmall: 60
    property int boxHeightLarge: 40
    property int boxWidthLarge: 220

    property bool visibleDoctorList: false
    property int sumPercent: 0
    property int subPercent: 0
    property real total: acum
    property string doctor: ""
    property string usage: ""
    property string observation: ""
    property bool isUpdate: false
    property int orderCurrentRow: 0

    signal endOrderEmit(int currentRow)

    function checkEndOrder() {

        if (posField.text === "") {
            return -1;
        }

        return 0;
    }

    function addCompositeToCart() {

        connDatabase.orderHandler.specCaps = specCapsModel.get(capsBox.currentIndex).title;
        connDatabase.orderHandler.package = packageModel.get(packBox.currentIndex).title;
        connDatabase.orderHandler.validate = validateModel.get(valBox.currentIndex).title;
        connDatabase.orderHandler.usage = usageModel.get(usageBox.currentIndex).title;
        connDatabase.orderHandler.posologia = posField.text;
        connDatabase.orderHandler.doctor = doctorField.displayText === "" ? "." : doctorField.displayText;
        connDatabase.orderHandler.obs = observationField.displayText;
        connDatabase.orderHandler.sum = Number(sumPercent);
        connDatabase.orderHandler.sub = Number(subPercent);
        connDatabase.orderHandler.finalCost = total;

        if (isUpdate === false) {

            if (connDatabase.orderHandler.addComposedToCart(orderCurrentRow) === true) {
                orderModel.append({"index": "",
                                   "composite": connDatabase.orderHandler.composedName(),
                                      "cost": "R$" + connDatabase.orderHandler.finalCost
                                  })
            }
        } else {
            if (connDatabase.orderHandler.updateComposedToCart() === true) {
                orderModel.set(orderTable.currentRow, {
                       "check": "",
                       "composite": connDatabase.orderHandler.composedName(),
                          "cost": "R$" + connDatabase.orderHandler.finalCost
                      })
            }
        }

        endOrderDialog.close();
        connDatabase.orderHandler.clearState();
        newOrderDialog.close();

        doctorField.text = "";
        visibleDoctorList = false;
        specCapsModel.clear();

        updFinishCostEmit();
    }

    function endPurchaseExec() {

        sumPercent = 0;
        subPercent = 0;

        specCapsModel.clear();
        packageModel.clear();

        //TODO: Calcular aqui o excipiente
        connDatabase.orderHandler.endPurchaseProcess();

        acum += connDatabase.orderHandler.getCostExcip();
        if (acum > connDatabase.orderHandler.getMinPrice()) {
            total = acum;
        } else {
            total = connDatabase.orderHandler.getMinPrice();
        }

        specCapsModel.append({"title": "..."});

        if ((phormModel.get(formBox.currentIndex).title === "CÁPSULAS") ||
            (phormModel.get(formBox.currentIndex).title === "SACHÊS")) {

            connDatabase.maintenanceHandler.loadSpecificationCapsList(
                        phormModel.get(formBox.currentIndex).title);
            for (var i=0 ; i< connDatabase.maintenanceHandler.specificationCapsCount(); ++i)
                specCapsModel.append({"title": connDatabase.maintenanceHandler.specCapsListAt(i)})

            for (i = 0; i <= connDatabase.maintenanceHandler.specificationCapsCount(); i++) {
                if (specCapsModel.get(i).title === connDatabase.orderHandler.getCapsSpecDetected()) {
                    capsBox.currentIndex = i;
                    break;
                }
            }
        }

        connDatabase.maintenanceHandler.loadPackList(connDatabase.maintenanceHandler.getFirstPhormId());
        for (i=0 ; i< connDatabase.maintenanceHandler.packCount(); ++i) {
            packageModel.append({"title": connDatabase.maintenanceHandler.packListAt(i)})
        }
    }

    onEndOrderEmit: {

        orderCurrentRow = currentRow;
        observationField.text = observation;
        posField.text = usage;
        doctorField.text = doctor;
        visibleDoctorList = false;
        endPurchaseExec();
    }

    //    contentItem:
    contentItem: Rectangle {
        implicitWidth: 800
        implicitHeight: 520

        color: "#E3E2D2"

        Keys.onReleased: {
            if (event.key === Qt.Key_F1) {
                endOrderDialog.close();
            } else if (event.key === Qt.Key_Enter) {

                switch (checkEndOrder()) {

                case 0:
                    addCompositeToCart();
                    break;

                case -1:
                    messageDialog.setText("Posologia é um item obrigatório!");
                    messageDialog.show();
                    break;
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("Out off mouse area clicked in End Order Dialog");
                visibleDoctorList = false;
            }
        }

        FontLoader { id: fixedFont; name: "Droid Sans" }

        //Carrega a lista de especificação de capsulas
        ListModel {
            id: specCapsModel
        }

        //Carrega a lista de embalagens
        ListModel {
            id: packageModel
        }

        //######################################################################################
        // INICIO DO FORMULARIO

        Column {
            id: column1
            spacing: 10
            anchors {
                top: parent.top
                topMargin: 10
            }
            width: parent.width

            Row {
                spacing: 350

                Text {
                    id: capsText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Capsula:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                Box {
                    id: capsBox
                    boxHeight: boxHeightLarge
                    anchors.left: capsText.right
                    anchors.leftMargin: 88
                    implicitWidth: boxWidthLarge
                    model: specCapsModel
                    onIndexChange: {
                    }
                }

                Text {
                    id: packText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Embalagem:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                Box {
                    id: packBox
                    boxHeight: boxHeightLarge
                    anchors.left: packText.right
                    anchors.leftMargin: 25
                    implicitWidth: boxWidthLarge
                    model: packageModel
                    onIndexChange: {
                    }
                }
            }
        }

        Image {
            id: divider
            width: parent.width
            anchors {
                top: column1.bottom
                topMargin: 30
            }

            source: "../images/divider-form.png"
            height: 2
        }


        Column {
            id: column2
            spacing: 40

            anchors.top: divider.bottom
            anchors.topMargin: 10

            Row {
                spacing: 20

                Text {
                    id: posText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Posologia:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: posField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 300
                }

                Button {
                    id: clearPosButtom
                    height: 40
                    text: "Limpar"
                    style: buttonStyle
                    onClicked: {
                        posField.text = "";
                    }
                }
            }
        }

        Column {
            id: column3
            spacing: 40
            anchors.top: column2.bottom
            anchors.topMargin: 30

            Row {
                spacing: 20

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Médico:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: doctorField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 250

                    onDisplayTextChanged: {
                        visibleDoctorList = true
                    }
                }
            }
        }

        Rectangle {
            id: listDoctor

            anchors.top: column3.bottom
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 85

            width: columnWidth + 250
            height: 300
            z: 1

            color: "black"
            enabled: visibleDoctorList
            visible: visibleDoctorList
            opacity: visibleDoctorList ? 1 : 0

            Keys.onReleased: {
                if (event.key === Qt.Key_Return) {
                    doctorField.text = proxyModel.get(docTable.currentRow).name;
                    visibleDoctorList = false
                }
            }

            onOpacityChanged: SequentialAnimation {
                PropertyAction { target: listDoctor; property: "height"; value: 0 }
                NumberAnimation { target: listDoctor; property: "height"; to: 60 * 5; duration: 250; easing.type: Easing.InOutQuad }
            }

            TableView {
                id: docTable
                anchors.fill: parent

                model: SortFilterProxyModel {
                    id: proxyModel
                    source: doctorsModel.count > 0 ? doctorsModel : null

                    sortOrder: docTable.sortIndicatorOrder
                    sortCaseSensitivity: Qt.CaseInsensitive
                    sortRole: doctorsModel.count > 0 ? docTable.getColumn(docTable.sortIndicatorColumn).role : ""

                    filterString: "*" + doctorField.text + "*"
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

                onClicked: {
                    doctorField.text = proxyModel.get(docTable.currentRow).name;
                    visibleDoctorList = false
                }
            }
        }

        Column {

            id: column4
            spacing: 40
            anchors.top: column3.bottom
            anchors.topMargin: 30
            Row {
                spacing: 20

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Observação:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: observationField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 600
                }
            }
        }

        Image {
            id: divider2
            width: parent.width
            anchors {
                top: column4.bottom
                topMargin: 30
            }

            source: "../images/divider-form.png"
            height: 2
        }

        Column {
            id: column5
            spacing: 40

            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.top: resume.bottom
            anchors.topMargin: 10

            Row {
                spacing: 30

                Button {
                    id: addButtom
                    height: 40
                    text: "[F1] Voltar"
                    style: buttonStyle
                    onClicked: {
                        newOrder.goBack();
                        endOrderDialog.close();
                    }
                }

                Button {
                    id: doneButtom
                    height: 40
                    text: "[F2] Concluir"
                    style: buttonStyle
                    onClicked: {
                        switch (checkEndOrder()) {

                        case 0:
                            addCompositeToCart();
                            break;

                        case -1:
                            messageDialog.setText("Posologia é um item obrigatório!");
                            messageDialog.show();
                            break;
                        }
                    }
                }

            }

        }

        //Resumo
        Rectangle {
            id: resume

            width: parent.width
            height: 120

            anchors.top: column4.bottom
            anchors.right: parent.right

            color: "#eee"

            Row {
                id: costRow

                spacing: 10

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    id: totalTitle
                    text: "TOTAL:"
                    font.pixelSize: 30
                    font.family: fixedFont.name
                    font.bold: true
                    color: "black"
                }

                Text {
                    id: priceText
                    text: 'R$' + Number(total).toFixed(2)
                    font.pixelSize: 32
                    font.family: fixedFont.name
                    font.bold: true
                    color: "black"
                }
            }

            //Acrescimo
            Column {

                id: sumColumn

                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20

                Row {
                    spacing: 10

                    Text {
                        id: textSum
                        anchors.margins: 20
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Acrescimo:"
                        font.pixelSize: 18
                        font.family: fixedFont.name
                    }

                    Image {
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                        source: "../images/sun.svg"
                        fillMode: Image.PreserveAspectFit;
                        width: 32
                        height: 32
                        enabled: subPercent ? 0 : 1
                        opacity: enabled ? 1 : 0.5

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                sumPercent++;
                                total = acum + ((acum * sumPercent) / 100);
                            }
                        }
                    }

                    Text {
                        id: sumValue
                        anchors.verticalCenter: parent.verticalCenter
                        text: Number(sumPercent) + "%"
                        font.pixelSize: 22
                        font.family: fixedFont.name
                    }

                    Image {
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                        source: "../images/less.svg"
                        fillMode: Image.PreserveAspectFit;
                        width: 32
                        height: 32
                        enabled: sumPercent ? 1 : 0
                        opacity: enabled ? 1 : 0.5

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (sumPercent > 0) {
                                    sumPercent--;
                                    total = acum - ((acum * sumPercent) / 100);
                                } else {
                                    total = acum;
                                }
                            }
                        }
                    }
                }
            }

            //            //Desconto
            Column {

                anchors.top: sumColumn.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20

                Row {
                    spacing: 10

                    Text {
                        id: discSum
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Descontos:"
                        font.pixelSize: 18
                        font.family: fixedFont.name
                    }

                    Image {
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                        source: "../images/sun.svg"
                        fillMode: Image.PreserveAspectFit;
                        width: 32
                        height: 32
                        enabled: sumPercent ? 0 : 1
                        opacity: enabled ? 1 : 0.5

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                subPercent++;
                                total = acum - ((acum * subPercent) / 100);
                            }
                        }
                    }

                    Text {
                        id: discValue
                        anchors.verticalCenter: parent.verticalCenter
                        text: Number(subPercent) + "%"
                        font.pixelSize: 22
                        font.family: fixedFont.name
                    }

                    Image {
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                        source: "../images/less.svg"
                        fillMode: Image.PreserveAspectFit;
                        width: 32
                        height: 32
                        enabled: subPercent ? 1 : 0
                        opacity: enabled ? 1 : 0.5

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (subPercent > 0) {
                                    subPercent--;
                                    total = acum + ((acum * subPercent) / 100);
                                } else {
                                    total = acum;
                                }
                            }
                        }

                    }
                }
            }
        }

        MessageDialog {
            id: messageDialog
            title: qsTr("Erro!")

            detailedText: "Há campos obrigatórios não preenchidos!!"

            icon: StandardIcon.Warning

            function show() {
                messageDialog.open();
            }
        }
    }
}
