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
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import PhormApp 1.0

Dialog {
    id: newOrderDialog

    width: 1224
    height: 760
    title: "Nova Ordem de Produção"
    modality: Qt.ApplicationModal
    property var endOrder: EndOrderDialog{}

    property int boxHeightSmall: 40
    property int boxWidthSmall: 60
    property int boxHeightLarge: 40
    property int boxWidthLarge: 220

    property bool visibleUnitList: false
    property bool visibleElementList: false
    property bool isNewOrder: true
    property bool isRecord: true

    property int calcBaseQtd: 0
    property bool fractionChanged: false
    property real acum: 0.0
    property real acumtemp: 0.0
    property int orderCurrentRow: 0
    signal newOrderEmit(int currentRow)
    signal editOrder
    signal goBack

    function clearState () {
        acum = 0.0;
        connDatabase.orderHandler.clearState();
        if (isRecord === true) {
            connDatabase.orderHandler.cancel();
        }
    }

    function checkInputData(step) {

        //Testa para ver se a verificacao eh executado
        //durante uma inclusao de um item ou na finalizao do composto
        if (step === "I") {
            if (phormModel.get(formBox.currentIndex).title === "..." ) {
                return -1;
            } else if (phormQtdField.displayText === "" ) {
                return -2;
            } else if (unitField.displayText === "" ) {
                return -3;
            } else if (elementField.displayText === "" ) {
                return -4;
            } else if (qtdField.displayText === "" ) {
                return -5;
            } else if (excipentModel.get(excipBox.currentIndex).title === "..." ) {
                return -7;
            }

            return 0;
        } else {
            if (componentTable.rowCount === 0 ) {
                return -6;
            }
        }

        if (isNewOrder === true) {
            return 0;
        }

        return 1;
    }

    function funcAlterOrder(rec) {

        var result = checkInputData("I");

        switch (result) {

        case 0:
            addElementToCart(rec);
            break;

        case -1:
            messageDialog.setText("É preciso informar uma phorma farmaceutica!");
            messageDialog.show();
            break;

        case -2:
            messageDialog.setText("Quantidade não informada!");
            messageDialog.show();
            break;

        case -3:
            messageDialog.setText("É preciso informar a unidade da phorma!");
            messageDialog.show();
            break;

        case -4:
            messageDialog.setText("O tipo de insumo deve ser informado!");
            messageDialog.show();
            break;

        case -5:
            messageDialog.setText("Não foi possível identificar a quantidade de insumo!");
            messageDialog.show();
            break;

        case -7:
            messageDialog.setText("É preciso informar um excipiente!");
            messageDialog.show();
            break;

        default:
            messageDialog.setText("Não foi possível identificar o erro!");
            messageDialog.show();
            break;
        }
    }

    function addElementToCart(rec) {

        connDatabase.orderHandler.phorm = phormModel.get(formBox.currentIndex).title;
        connDatabase.orderHandler.phormQtd = phormQtdField.displayText;
        connDatabase.orderHandler.phormQtdUnit = unitField.displayText;
        connDatabase.orderHandler.element = elementField.displayText;
        connDatabase.orderHandler.elementQtd = qtdField.displayText;
        connDatabase.orderHandler.elementQtdUnit = unitModel.get(unitBox.currentIndex).title;
        connDatabase.orderHandler.excip = excipentModel.get(excipBox.currentIndex).title;
        connDatabase.orderHandler.labelPrintCount = countValue.value;

        console.log("ADICIONA UM NOVO ELEMENTO");
        if (connDatabase.orderHandler.addElementToCart(rec) === true) {

            componentModel.append({
                  "item": elementField.displayText,
                  "qtd": connDatabase.orderHandler.qtdBase() + unitModel.get(unitBox.currentIndex).title,
                  "prepare": connDatabase.orderHandler.qtdPrepare() + " g",
                  "cost": "R$" + connDatabase.orderHandler.cost()
                  })

            qtdField.text = "";
            elementField.text = "";
            elementField.selectAll();

            acum = acum + connDatabase.orderHandler.costListAt(componentModel.count - 1);

        } else {
            messageDialog.setText("Erro durante os calculos");
            messageDialog.show();
        }
    }

    function updatePhormList() {

        acum = 0.0;

        phormModel.clear();
        phormModel.append({"title": "..."})
        for (var i=0 ; i< connDatabase.maintenanceHandler.phormListCount(); ++i)
            phormModel.append({"title": connDatabase.maintenanceHandler.phormListAt(i)})

        //Limpa a lista
        excipentModel.clear();
        //Carrega o id da formula selecionada
        connDatabase.maintenanceHandler.phormId = phormModel.get(formBox.currentIndex).title;
        //Carrega os items de sub-grupo relacionados ao grupo
        connDatabase.maintenanceHandler.loadExcipientList(connDatabase.maintenanceHandler.getPhormId());
        if (connDatabase.maintenanceHandler.excipientCount() === 0) {
            excipentModel.append({"title": "..."});
        }
        for (i=0 ; i< connDatabase.maintenanceHandler.excipientCount(); ++i) {
            excipentModel.append({"title": connDatabase.maintenanceHandler.excipientListAt(i)})
        }
    }

    function updateComponent(idx) {

        acumtemp = connDatabase.orderHandler.costListAt(idx);
        connDatabase.orderHandler.labelPrintCount = countValue.value;

        if (connDatabase.orderHandler.update(idx,
                                         phormQtdField.displayText,
                                         elementField.displayText,
                                         qtdField.displayText,
                                         unitModel.get(unitBox.currentIndex).title) === true) {

            componentModel.set(idx, {
                  "item": elementField.displayText,
                  "qtd": connDatabase.orderHandler.qtdBase() + unitModel.get(unitBox.currentIndex).title,
                  "prepare": connDatabase.orderHandler.qtdPrepare() + " g",
                  "cost": "R$" + connDatabase.orderHandler.cost()
                  })

            qtdField.text = "";
            elementField.text = "";
            elementField.selectAll();

            acum = acum - acumtemp;
            acum = acum + connDatabase.orderHandler.costListAt(idx);

        } else {
            messageDialog.setText("Erro durante os calculos");
            messageDialog.show();
        }
    }

    //AÇÃO PARA RESTABELECER O RESULTADO DA FORMULA SEM O EXCIPIENTE
    onGoBack: {
        acum = acum - Number(connDatabase.orderHandler.removeExcip());
    }

    onNewOrderEmit: {

        orderCurrentRow = currentRow;
        clearState();

        elementField.text = "";
        qtdField.text = "";
        unitField.text = "";
        phormQtdField.text = "";
        componentModel.clear();
        excipBox.currentIndex = 0;
        formBox.currentIndex = 0;
        countValue.value = 1;

        isNewOrder = true;
        visibleUnitList = false;
        visibleElementList = false;

        unitTable.selection.clear();
        elementTable.selection.clear();
        componentTable.selection.clear();

        connDatabase.orderHandler.setNextComposedID();
        updatePhormList();
    }

    onEditOrder: {

        acum = 0.0;
        qtdField.text = "";
        elementField.text = "";
        componentModel.clear();
        excipBox.currentIndex = 0;
        formBox.currentIndex = 0;
        countValue.value = 1;

        isNewOrder = false;
        visibleElementList = false;

        unitTable.selection.clear();
        elementTable.selection.clear();
        componentTable.selection.clear();

        if (isRecord === true) {
            connDatabase.orderHandler.setNextComposedID();
        }

        updatePhormList();

        for (var j = 0; j < connDatabase.orderHandler.compositionCount(); ++j) {
            if (connDatabase.orderHandler.fillOrderFromComposition() === true) {

                phormQtdField.text = connDatabase.orderHandler.phormQtd;
                unitField.text = connDatabase.orderHandler.phormQtdUnit;
                visibleUnitList = false;
                excipBox.currentIndex = (connDatabase.maintenanceHandler.excipientIndexOf(
                            connDatabase.orderHandler.excip) + 1);
                formBox.currentIndex = (connDatabase.maintenanceHandler.phormIndexOf(
                            connDatabase.orderHandler.phorm) + 1);

                if (connDatabase.orderHandler.addElementToCart(isRecord) === true) {

                    componentModel.append({
                          "item": connDatabase.orderHandler.element,
                          "qtd": connDatabase.orderHandler.qtdBase() + unitModel.get(unitBox.currentIndex).title,
                          "prepare": connDatabase.orderHandler.qtdPrepare() + " g",
                          "cost": "R$" + connDatabase.orderHandler.cost()
                          })

                    acum = acum + connDatabase.orderHandler.costListAt(componentModel.count - 1);
                }
            }
        }
    }

    contentItem: Rectangle {
        implicitWidth: 1224
        implicitHeight: 760
        color: "#E3E2D2"

        Keys.onReleased: {
            if (event.key === Qt.Key_Escape) {
                acum = 0.0;
                connDatabase.orderHandler.clearState();
                if (isRecord === true) {
                    connDatabase.orderHandler.cancel();
                }
                newOrderDialog.close();
            } else if (event.key === Qt.Key_Insert) {

                funcAlterOrder(true);

            } else if (event.key === Qt.Key_F2) {

                var end_result = checkInputData("F");

                switch (end_result) {

                case 0:
                    endOrder.visible = true
                    endOrder.doctor = ""
                    endOrder.usage = "CONFORME ORIENTAÇÃO MÉDICA."
                    endOrder.observation = ""
                    endOrder.endOrderEmit(orderCurrentRow);
                    break;

                case 1:
                    endOrder.visible = true
                    endOrder.doctor = connDatabase.orderHandler.doctor
                    endOrder.usage = connDatabase.orderHandler.posologia
                    endOrder.observation = connDatabase.orderHandler.obs
                    endOrder.endOrderEmit(orderCurrentRow);
                    break;

                case -6:
                    messageDialog.setText("Nenhum item adicionado a lista!");
                    messageDialog.show();
                    break;

                case -7:
                    messageDialog.setText("Nenhum excipiente informado!");
                    messageDialog.show();
                    break;
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("Out off mouse area clicked");
                visibleUnitList = false;
                visibleElementList = false;

                unitTable.selection.clear();
                elementTable.selection.clear();
                componentTable.selection.clear();
            }
        }

        FontLoader { id: fixedFont; name: "Droid Sans" }

        //inicializa a tabela de componentes
        ListModel {
            id: componentModel
        }

        //Lista de Phormas farmaceuticas
        ListModel {
            id: phormModel
        }

        //Lista de excipientes
        ListModel {
            id: excipentModel
        }

        ListModel {
            id: usageModel
            Component.onCompleted: {
                usageModel.append({"title": "INTERNO"})
                usageModel.append({"title": "EXTERNO"})
            }
        }

        //Lista de opções de validade
        ListModel {
            id: validateModel
            Component.onCompleted: {
                connDatabase.maintenanceHandler.loadValidateList();
                for (var i=0 ; i< connDatabase.maintenanceHandler.validateCount(); ++i)
                    validateModel.append({"title": connDatabase.maintenanceHandler.validateListAt(i)})
            }
        }
        //######################################################################################
        // INICIO DO FORMULARIO

        Column {
            id: column1
            spacing: 40
            anchors {
                top: parent.top
                topMargin: 10
            }

            Row {
                id: row1
                spacing: 20

                Text {
                    id: formText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Forma Farm.:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                Box {
                    id: formBox
                    boxHeight: boxHeightLarge
                    anchors.left: formText.right
                    anchors.leftMargin: 50
                    implicitWidth: boxWidthLarge
                    model: phormModel

                    onIndexChange: {
                        //Limpa a lista
                        excipentModel.clear();
                        //Carrega o id da formula selecionada
                        connDatabase.maintenanceHandler.phormId = phormModel.get(formBox.currentIndex).title;
                        //Carrega os items de sub-grupo relacionados ao grupo
                        connDatabase.maintenanceHandler.loadExcipientList(connDatabase.maintenanceHandler.getPhormId());

                        if (connDatabase.maintenanceHandler.excipientCount() === 0) {
                            excipentModel.append({"title": "..."});
                        }

                        for (var i=0 ; i< connDatabase.maintenanceHandler.excipientCount(); ++i)
                            excipentModel.append({"title": connDatabase.maintenanceHandler.excipientListAt(i)})
                    }
                }
            }
        }

        Column {
            id: column2

            anchors.left: column1.right

            anchors.top: parent.top
            anchors.topMargin: 1
            anchors.leftMargin: 300

            Row {
                spacing: 10

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Quantidade:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: phormQtdField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth / 2
                    onTextChanged: {
                        if (fractionChanged === false) {
                            calcBaseQtd = Number(phormQtdField.text);
                            connDatabase.orderHandler.phormQtd = phormQtdField.displayText;
                        }
                        fractionChanged = false;
                    }
                    onAccepted: {
                        phormQtdField.text = calcBaseQtd * countValue.value;
                        for (var i=0 ; i < componentTable.rowCount; ++i) {
                            elementField.text = connDatabase.orderHandler.elementAt(i);
                            qtdField.text = connDatabase.orderHandler.elementQtdAt(i);
                            updateComponent(i);
                        }
                    }
                }

                TextField {
                    id: unitField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth / 2
                    onDisplayTextChanged: {
                        if (unitField.displayText !== "") {
                            visibleUnitList = true
                        } else {
                            visibleUnitList = false
                        }

                    }
                }
            }
        }

        Rectangle {
            id: listUnit

            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 602

            width: columnWidth / 2
            height: 300
            z: 1

            color: "black"
            enabled: visibleUnitList
            visible: visibleUnitList
            opacity: visibleUnitList ? 1 : 0

            Keys.onReleased: {
                if (event.key === Qt.Key_Return) {
                    unitField.text = unitPproxyModel.get(unitTable.currentRow).title;
                    visibleUnitList = false;
                }
            }

            onOpacityChanged: SequentialAnimation {
                PropertyAction { target: listUnit; property: "height"; value: 0 }
                NumberAnimation { target: listUnit; property: "height"; to: 60 * 5; duration: 250; easing.type: Easing.InOutQuad }
            }

            TableView {
                id: unitTable
                anchors.fill: parent

                model: SortFilterProxyModel {
                    id: unitPproxyModel
                    source: unitModel.count > 0 ? unitModel : null

                    sortOrder: unitTable.sortIndicatorOrder
                    sortCaseSensitivity: Qt.CaseInsensitive
                    sortRole: unitModel.count > 0 ? unitTable.getColumn(unitTable.sortIndicatorColumn).role : ""

                    filterString: "*" + unitField.text + "*"
                    filterSyntax: SortFilterProxyModel.Wildcard
                    filterCaseSensitivity: Qt.CaseInsensitive
                }

                TableViewColumn {
                    role: "title"
                    width: parent.width
                }

                rowDelegate: Rectangle {
                    height: 30
                    Behavior on height{ NumberAnimation{} }

                    color: styleData.selected ? "#448" : (styleData.alternate? "#eee" : "#fff")
                    BorderImage{
                        id: selected2
                        anchors.fill: parent
                        source: "../images/selectedrow.png"
                        visible: styleData.selected
                        border{left:2; right:2; top:2; bottom:2}
                        SequentialAnimation {
                            running: true; loops: Animation.Infinite
                            NumberAnimation { target:selected2; property: "opacity"; to: 1.0; duration: 900}
                            NumberAnimation { target:selected2; property: "opacity"; to: 0.5; duration: 900}
                        }
                    }
                }

                onClicked: {
                    unitField.text = unitPproxyModel.get(unitTable.currentRow).title;
                    visibleUnitList = false;
                }
            }
        }

        Column {
            id: column3
            spacing: 40
            anchors.top: column1.bottom
            anchors.topMargin: 30

            Row {
                spacing: 20

                Text {
                    id: excipText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Excip/Veic.:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                Box {
                    id: excipBox
                    boxHeight: boxHeightLarge
                    anchors.left: excipText.right
                    anchors.leftMargin: 67
                    implicitWidth: boxWidthLarge
                    model: excipentModel
                    onIndexChange: {
                    }
                }
            }
        }

        Column {
            id: column4
            spacing: 40

            anchors.left: column2.right

            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.leftMargin: 100

            Row {
                spacing: 20

                Text {
                    id: valText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Validade:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                Box {
                    id: valBox
                    boxHeight: boxHeightLarge
                    anchors.left: valText.right
                    anchors.leftMargin: 50
                    implicitWidth: boxWidthLarge

                    model: validateModel
                    onIndexChange: {
                    }
                }
            }

            Row {
                spacing: 20

                Text {
                    id: usageText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Uso:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                Box {
                    id: usageBox
                    boxHeight: boxHeightLarge
                    anchors.left: usageText.right
                    anchors.leftMargin: 88
                    implicitWidth: boxWidthLarge

                    model: usageModel
                    onIndexChange: {
                    }
                }
            }
        }

        Image {
            id: divider
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

            anchors.top: divider.bottom
            anchors.topMargin: 10

            Row {
                spacing: 10

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Insumo:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: elementField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 200
                    onDisplayTextChanged: {
                        if (elementField.displayText !== "") {
                            visibleElementList = true
                        } else {
                            visibleElementList = false
                        }
                    }
                    Keys.onDownPressed: {
                        visibleElementList = true
                        elementTable.currentRow++;
                    }
                    Keys.onEnterPressed: {
                        elementField.text = elementProxyModel.get(elementTable.currentRow).name;
                        visibleElementList = false
                    }
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Qtd.:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: qtdField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth / 2
                }

                Box {
                    id: unitBox
                    boxHeight: boxHeightSmall
                    anchors.left: qtdField.right
                    anchors.leftMargin: 42
                    implicitWidth: boxWidthSmall

                    model: unitModel
                }
            }
        }

        Column {
            id: column6
            spacing: 40

            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.top: divider.bottom
            anchors.topMargin: 10

            Row {
                spacing: 30

                Button {
                    id: addButtom
                    height: 40
                    text: "[Ins] Incluir"
                    style: buttonStyle
                    onClicked: {
                        funcAlterOrder(true);
                    }
                }

                Button {
                    id: doneButtom
                    height: 40
                    text: "[F2] Finalizar"
                    style: buttonStyle
                    onClicked: {
                        var end_result = checkInputData("F");

                        switch (end_result) {

                        case 0:
                            endOrder.visible = true
                            endOrder.doctor = ""
                            endOrder.usage = "CONFORME ORIENTAÇÃO MÉDICA."
                            endOrder.observation = ""
                            if (isRecord === true) {
                                endOrder.isUpdate = false;
                            } else {
                                endOrder.isUpdate = true;
                            }

                            endOrder.endOrderEmit(orderCurrentRow);
                            break;

                        case 1:
                            endOrder.visible = true
                            endOrder.doctor = connDatabase.orderHandler.doctor
                            endOrder.usage = connDatabase.orderHandler.posologia
                            endOrder.observation = connDatabase.orderHandler.obs
                            if (isRecord === true) {
                                endOrder.isUpdate = false;
                            } else {
                                endOrder.isUpdate = true;
                            }

                            endOrder.endOrderEmit(orderCurrentRow);
                            break;

                        case -6:
                            messageDialog.setText("Nenhum item adicionado a lista!");
                            messageDialog.show();
                            break;

                        case -7:
                            messageDialog.setText("Nenhum excipiente informado!");
                            messageDialog.show();
                            break;
                        }
                    }
                }

            }

        }

        Rectangle {
            id: listElementBase

            anchors.top: column5.bottom
            anchors.topMargin: 1
            anchors.left: parent.left
            anchors.leftMargin: 80

            width: columnWidth + 200
            height: 300
            z: 1

            color: "black"
            enabled: visibleElementList
            visible: visibleElementList
            opacity: visibleElementList ? 1 : 0

            Keys.onReleased: {
                if (event.key === Qt.Key_Return) {
                    elementField.text = elementProxyModel.get(elementTable.currentRow).name;
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

                    filterString: "*" + elementField.text + "*"
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
                    elementField.text = elementProxyModel.get(elementTable.currentRow).name;
                    visibleElementList = false
                }
            }
        }

        //#### TABELA DE VISUALIZACAO DE ITEMS
        TableView {
            id: componentTable
            width: parent.width
            height: 300
            anchors.top: column5.bottom
            anchors.left: parent.left
            anchors.topMargin: 25

            frameVisible: true
            headerVisible: true
            alternatingRowColors: true

            property int margins: Qt.platform.os === "osx" ? 16 : 0

            TableViewColumn {
                role: "item"
                title: "Item"
//                width: (parent.width / 6) + 28
                width: (parent.width / 5) - 30
            }
            TableViewColumn {
                role: "qtd"
                title: "Qtd. unit"
                width: (parent.width / 5) - 30
            }
            TableViewColumn {
                role: "prepare"
                title: "Manipular"
                width: (parent.width / 5) - 30
            }
            TableViewColumn {
                role: "cost"
                title: "Custo"
                width: (parent.width / 5) - 30
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
            model: componentModel

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
                        text: "ATUALIZAR"
                        implicitWidth: columnWidth
                        onClicked: {
                            updateComponent(componentTable.currentRow);
                        }
                    }
                    Button {
                        id: removeButtom
                        text: "EXCLUIR"
                        implicitWidth: columnWidth
                        onClicked: {
                            acumtemp = connDatabase.orderHandler.costListAt(componentTable.currentRow);

                            if (connDatabase.orderHandler.remove(componentTable.currentRow) === true) {
                                componentModel.remove(componentTable.currentRow);

                                acum = acum - acumtemp;
                            }
                        }
                    }
                }
            }

            onClicked: {
                unitBox.currentIndex = connDatabase.maintenanceHandler.unitIndexOf(
                                    connDatabase.orderHandler.getElementQtdUnit(componentTable.currentRow));
                qtdField.text = connDatabase.orderHandler.elementQtdAt(componentTable.currentRow);
                elementField.text = connDatabase.orderHandler.elementAt(componentTable.currentRow);
                visibleElementList = false;
            }
        }

        //Fator Multiplicador
        Rectangle {
            id: featuresPrint
            width: 220
            height: 128

            color: "white"
            border.color: "black"
            border.width: 3
            radius:10
            visible: true
            anchors.top: componentTable.bottom
            anchors.left: parent.left
            anchors.topMargin: 20

            //
            Column {

                id: countColumn
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20

                Column {
                    spacing: 10

                    Text {
                        id: textSum
                        text: "Qtd. Fração:"
                        font.pixelSize: 18
                        font.family: fixedFont.name
                    }

                    SpinBox {
                        id: countValue
                        font.pixelSize: 26
                        font.family: fixedFont.name
                        minimumValue: 1
                        maximumValue: 9999
                        prefix: "x"
                        onValueChanged: {
                            fractionChanged = true;
                            phormQtdField.text = calcBaseQtd * countValue.value;
                            for (var i=0 ; i < componentTable.rowCount; ++i) {
                                elementField.text = connDatabase.orderHandler.elementAt(i);
                                qtdField.text = connDatabase.orderHandler.elementQtdAt(i);
                                updateComponent(i);
                            }
                        }
                    }
                }
            }
        }

        //Resumo
        Rectangle {
            id: resume

            width: 200
            height: 100

            anchors.top: componentTable.bottom
            anchors.right: parent.right

            color: "#eee"

            Text {
                id: totalTitle
                anchors {
                    left: parent.left
                    margins: 10
                }

                text: "TOTAL:"
                font.pixelSize: 20
                font.family: fixedFont.name
                font.bold: true
                color: "black"
            }

            Text {
                id: resumeCostText
                anchors {
                    left: totalTitle.right
                    margins: 10
                }

                text: 'R$' + Number(acum).toFixed(2)
                font.pixelSize: 20
                font.family: fixedFont.name
                font.bold: true
                color: "black"
            }
        }

        Column {
            id: column7
            spacing: 40

            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.top: resume.bottom
            anchors.topMargin: 10

            Row {
                spacing: 30

                Button {
                    id: cancelButtom
                    height: 40
                    text: "[Esc] Cancelar"
                    style: buttonStyle
                    onClicked: {
                        clearState();
                        newOrderDialog.close();
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
