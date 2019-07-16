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


import QtQuick 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2

Dialog {
    id: elementDataDialog

    width: 1228
    height: 768
    title: "Cadastro de Insumos"

    property alias name: nameField.text
    property bool isUpdate: false

    signal editForm

    function clear ()
    {
        loteField.text = "";
        nicknameField.text = "";
        concentrationField.text = "";
        factorField.text = "";
        doseMinField.text = "";
        doseMaxField.text = "";
        obsField.text = "";

        subGroupBox.currentIndex = 0;
        groupBox.currentIndex = 0;
        unitManBox.currentIndex = 0;
        unitStockBox.currentIndex = 0;
        uiConvBox.currentIndex = 0;
        utrConvBox.currentIndex = 0;

        costFiled.text = "";
        factorCostField.text = "";
        densidyField.text = "";
        stockMinField.text = "";
        stockMaxField.text = "";
        loteSupplierField.text = "";

        nameFieldFirstStage.text = "";
        statusEnroll.state= "NOT";
    }

    onEditForm: {
        connDatabase.elementHandler.select = nameField.text;

        loteField.text = connDatabase.elementHandler.lote;
        groupBox.currentIndex = connDatabase.maintenanceHandler.groupIndexOf(
                    connDatabase.elementHandler.getGroupName());

        subGroupBox.currentIndex = connDatabase.maintenanceHandler.subGroupIndexOf(
                    connDatabase.elementHandler.getSubGroupElement());

        nicknameField.text = connDatabase.elementHandler.nickname;
        concentrationField.text = connDatabase.elementHandler.concentration;
        factorField.text = connDatabase.elementHandler.factor;
        unitManBox.currentIndex = connDatabase.maintenanceHandler.unitIndexOf(
                    connDatabase.elementHandler.getUnitMan());
        unitStockBox.currentIndex = connDatabase.maintenanceHandler.unitIndexOf(
                    connDatabase.elementHandler.getUnitStock());
        doseMinField.text = connDatabase.elementHandler.doseMin;
        doseMaxField.text = connDatabase.elementHandler.doseMax;
        obsField.text = connDatabase.elementHandler.description;

        uiBase.text = connDatabase.elementHandler.uiBase;
        uiEquivalent.text = connDatabase.elementHandler.uiEquivalent;
        uiConvBox.currentIndex = connDatabase.maintenanceHandler.unitIndexOf(
                    connDatabase.elementHandler.getUiConv());

        utrBase.text = connDatabase.elementHandler.utrBase;
        utrEquivalent.text = connDatabase.elementHandler.utrEquivalent;
        utrConvBox.currentIndex = connDatabase.maintenanceHandler.unitIndexOf(
                    connDatabase.elementHandler.getUtrConv());

        costFiled.text = connDatabase.elementHandler.cost;
        factorCostField.text = connDatabase.elementHandler.costFactor;
        densidyField.text = connDatabase.elementHandler.density;
        stockMinField.text = connDatabase.elementHandler.stockMin;
        stockMaxField.text = connDatabase.elementHandler.stockMax;
        loteSupplierField.text = connDatabase.elementHandler.loteSupplier;
    }

//    contentItem:
        Rectangle {
        implicitWidth: 1228
        implicitHeight: 768
        color: "#E3E2D2"

        FontLoader { id: fixedFont; name: "Droid Sans" }

        ListModel {
            id: unitModel
            Component.onCompleted: {
                connDatabase.maintenanceHandler.loadUnitsList();
                for (var i=0 ; i< connDatabase.maintenanceHandler.unitListCount(); ++i)
                    unitModel.append({"title": connDatabase.maintenanceHandler.unitsListAt(i)})
            }
        }

        ListModel {
            id: subGroupsModel
            Component.onCompleted: {
                connDatabase.maintenanceHandler.loadSubGroupList(connDatabase.maintenanceHandler.getFirstGroupId());
                for (var i=0 ; i< connDatabase.maintenanceHandler.subGroupListCount(); ++i)
                    subGroupsModel.append({"title": connDatabase.maintenanceHandler.subGroupListAt(i)})
            }
        }

        ListModel {
            id: groupsBoxModel
            Component.onCompleted: {
                connDatabase.maintenanceHandler.loadGroupList();
                for (var i=0 ; i< connDatabase.maintenanceHandler.groupListCount(); ++i)
                    groupsBoxModel.append({"title": connDatabase.maintenanceHandler.groupListAt(i)})
            }
        }

        //######################################################################################
        // INICIO DO FORMULARIO
        Column {
            id: elementForm
            spacing: 10
            anchors {
                top: parent.top
                topMargin: 10
            }
            width: parent.width

            Row {
                spacing: 20
                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Nº Lote:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: loteField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + columnWidth
                }
            }
            //DADOS DE INSUMOS
            Row {
                spacing: 300

                Text {
                    id: groupText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Grupo:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Box {
                    id: groupBox
                    boxHeight: boxHeightLarge
                    anchors.left: groupText.right
                    anchors.leftMargin: 50
                    //                anchors.margins: 10
                    implicitWidth: boxWidthLarge

                    model: groupsBoxModel

                    onIndexChange: {
                        //Limpa a lista
                        subGroupsModel.clear();
                        //Carrega o id do grupo selecionado
                        connDatabase.maintenanceHandler.groupId = groupsBoxModel.get(groupBox.currentIndex).title;
                        //Carrega os items de sub-grupo relacionados ao grupo
                        connDatabase.maintenanceHandler.loadSubGroupList(connDatabase.maintenanceHandler.getGroupId());
                        for (var i=0 ; i< connDatabase.maintenanceHandler.subGroupListCount(); ++i)
                            subGroupsModel.append({"title": connDatabase.maintenanceHandler.subGroupListAt(i)})
                    }
                }

                Text {
                    id: subGroupText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Sub grupo:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Box {
                    id: subGroupBox
                    boxHeight: boxHeightLarge
                    anchors.left: subGroupText.right
                    anchors.margins: 10
                    implicitWidth: boxWidthLarge

                    model: subGroupsModel
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
                    id: nameField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 520
                    onTextChanged: {
                        statusEnroll.state= "NOT";
                    }
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Apelido:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: nicknameField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 250
                }
            }

            Row {

                spacing: 150

                Text {
                    id: unitManText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Unidade de Manipulação:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Box {
                    id: unitManBox
                    boxHeight: boxHeightSmall
                    anchors.left: unitManText.right
                    anchors.margins: 10
                    implicitWidth: boxWidthSmall

                    model: unitModel
                }

                Text {
                    id: unitStockText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Unidade de Estoque:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Box {
                    id: unitStockBox
                    boxHeight: boxHeightSmall
                    anchors.left: unitStockText.right
                    anchors.margins: 10
                    implicitWidth: boxWidthSmall

                    model: unitModel
                }

                Row {
                    spacing: 15

                    Text {
                        id: concentrationText
                        anchors.margins: 20
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Concentração:"
                        font.pixelSize: pixelSize
                        font.family: fixedFont.name
                    }

                    TextField {
                        id: concentrationField
                        anchors.margins: 10
                        style: textfieldStyle
                        implicitWidth: columnWidth - 30
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        text: "%"
                        font.pixelSize: pixelSize
                        font.family: fixedFont.name
                    }
                }

            }

            Row {
                spacing: 15

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Fator Correção:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: factorField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Dose mín.:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: doseMinField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: connDatabase.maintenanceHandler.unitsListAt(unitManBox.currentIndex)
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Dose máx.:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: doseMaxField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: connDatabase.maintenanceHandler.unitsListAt(unitManBox.currentIndex)
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }
            }

            Row {
                spacing: 15

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
                    implicitWidth: columnWidth + 600
                }
            }

            //DADOS DE CONVERSAO
            Row {
                Text {
                    anchors.margins: 10
                    text: "DADOS DE CONVERSÃO"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

            }

            Image {

                id: dividerConversion
                width: parent.width

                source: "../images/divider-form.png"
                height: 2

            }


            Row {
                spacing: 70

                TextField {
                    id: uiBase
                    style: textfieldStyle
                    implicitWidth: columnWidth
                    placeholderText: "1,00000"
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "ui -->"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: uiEquivalent
                    style: textfieldStyle
                    implicitWidth: columnWidth
                    placeholderText: "1,00000"
                }

                Box {
                    id: uiConvBox
                    boxHeight: boxHeightSmall
                    anchors.left: uiEquivalent.right
                    anchors.margins: 10
                    implicitWidth: boxWidthSmall

                    model: unitModel
                }

                TextField {
                    id: utrBase
                    anchors.margins: 60
                    style: textfieldStyle
                    implicitWidth: columnWidth
                    placeholderText: "1,00000"
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "utr --->"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: utrEquivalent
                    style: textfieldStyle
                    implicitWidth: columnWidth
                    placeholderText: "1,00000"
                }

                Box {
                    id: utrConvBox
                    boxHeight: boxHeightSmall
                    anchors.left: utrEquivalent.right
                    anchors.margins: 10
                    implicitWidth: boxWidthSmall

                    model: unitModel
                }
            }

            //DADOS DE CUSTO

            Row {
                Text {
                    anchors.margins: 10
                    text: "DADOS DE CUSTO"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }
            }

            Image {

                id: dividerCost
                width: parent.width

                source: "../images/divider-form.png"
                height: 2

            }

            Row {
                spacing: 15

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Custo:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: costFiled
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth - 20
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text:{ "/" + connDatabase.maintenanceHandler.unitsListAt(unitStockBox.currentIndex)}
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Fator preço:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: factorCostField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth - 20
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Densidade:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: densidyField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "g/ml"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }
            }

            Row {
                spacing: 15

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Estoque Min:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: stockMinField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "/" + connDatabase.maintenanceHandler.unitsListAt(unitStockBox.currentIndex)
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Estoque Máx:"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: stockMaxField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "/" + connDatabase.maintenanceHandler.unitsListAt(unitStockBox.currentIndex)
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Lote (fornecedor):"
                    font.pixelSize: pixelSize
                    font.family: fixedFont.name
                }

                TextField {
                    id: loteSupplierField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth - 50
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

            Row {
                spacing: 15

                Button {
                    id: applyButtom
                    anchors.rightMargin: 10
                    text: isUpdate ? "ATUALIZAR" : "INSERIR"
                    style: actionButtonsStyle
                    implicitWidth: columnWidth
                    onClicked: {
                        connDatabase.elementHandler.lote = loteField.displayText;
                        connDatabase.elementHandler.subGroupId = subGroupsModel.get(subGroupBox.currentIndex).title;
                        connDatabase.elementHandler.element = nameField.displayText;
                        connDatabase.elementHandler.nickname = nicknameField.displayText;
                        connDatabase.elementHandler.unitmanip = unitModel.get(unitManBox.currentIndex).title;
                        connDatabase.elementHandler.unitstock = unitModel.get(unitStockBox.currentIndex).title;
                        connDatabase.elementHandler.concentration = concentrationField.displayText;
                        connDatabase.elementHandler.factor = factorField.displayText;
                        connDatabase.elementHandler.doseMin =  doseMinField.displayText;
                        connDatabase.elementHandler.doseMax =  doseMaxField.displayText;
                        connDatabase.elementHandler.description = obsField.displayText;

                        connDatabase.elementHandler.uiBase = uiBase.displayText;
                        connDatabase.elementHandler.uiEquivalent = uiEquivalent.displayText;
                        connDatabase.elementHandler.uiUnit = unitModel.get(uiConvBox.currentIndex).title;

                        connDatabase.elementHandler.utrBase = utrBase.displayText;
                        connDatabase.elementHandler.utrEquivalent = utrEquivalent.displayText;
                        connDatabase.elementHandler.utrUnit = unitModel.get(utrConvBox.currentIndex).title;

                        connDatabase.elementHandler.cost = costFiled.displayText;
                        connDatabase.elementHandler.costFactor = factorCostField.displayText;
                        connDatabase.elementHandler.density = densidyField.displayText;
                        connDatabase.elementHandler.stockMin = stockMinField.displayText;
                        connDatabase.elementHandler.stockMax = stockMaxField.displayText;
                        connDatabase.elementHandler.loteSupplier = loteSupplierField.displayText;

                        if (connDatabase.elementHandler.checkDependence() === false) {
                            statusEnroll.state= "FAIL";
                            messageDialog.show();
                        } else {

                            //Testa se eh uma atualizacao ou insersao
                            if (isUpdate === true) {
                                isUpdate = false;

                                if (connDatabase.elementHandler.update() === true) {
                                    nameField.text = "";
                                    clear();
                                    nameFieldFirstStage.text = "";
                                    statusEnroll.state= "SUCCESS";
                                }

                            } else {

                                if (connDatabase.elementHandler.insert() === true) {

                                    elementModel.append({"name": nameField.displayText,
                                                         "group": groupsBoxModel.get(groupBox.currentIndex).title,
                                                         "subgroup": subGroupsModel.get(subGroupBox.currentIndex).title});

                                    nameField.text = "";
                                    clear();
                                    nameFieldFirstStage.text = "";
                                    statusEnroll.state= "SUCCESS";
                                }
                            }
                        }
                    }
                }

                Button {

                    id: cleanButtom
                    anchors.rightMargin: 10
                    text: "LIMPAR"
                    style: actionButtonsStyle
                    implicitWidth: columnWidth

                    onClicked: {
                        clear();
                        nameField.text = "";
                    }
                }
            }

            Rectangle {
                id: statusEnroll
                height: 50; width: parent.width
                state: "NOT"

                Text {
                    id: description
                    anchors.centerIn: parent
                    color: "white"
                    font.pixelSize: 22
                    font.bold: true
                }

                states: [
                    State {
                        name: "NOT"
                        PropertyChanges {target: statusEnroll; visible: false}
                    },
                    State {
                        name: "SUCCESS"
                        PropertyChanges {target: statusEnroll; color: "green"}
                        PropertyChanges {target: description; text: "ITEM CADASTRADO COM SUCESSO!"}
                    },
                    State {
                        name: "FAIL"
                        PropertyChanges {target: statusEnroll; color: "red"}
                        PropertyChanges {target: description; text: "NÃO FOI POSSÍVEL CADASTRAR O ITEM!"}
                    }
                ]
            }
        }

        MessageDialog {
            id: messageDialog
            title: qsTr("Erro!")

            informativeText: "Campos não preenchidos!!"
            detailedText: "Verifique se todos os campos foram preenchidos corretamente!"

            icon: StandardIcon.Critical

            function show() {
    //            messageDialog.text = caption;
                messageDialog.open();
            }
        }
    }
}
