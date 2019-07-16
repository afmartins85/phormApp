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

Dialog {
    id: clientsDataDialog

    width: 1024
    height: 600
    title: "Cadastro de Fornecedores"

    property alias name: nameField.text
    property alias cnpj: cnpjInput.text

    //    contentItem:
    Rectangle {
        implicitWidth: 1024
        implicitHeight: 600
        color: "#E3E2D2"

        FontLoader { id: fixedFont; name: "Droid Sans" }

        ListModel {
            id: typeModel

            ListElement {
                name: "FORNECEDOR"
            }
            ListElement {
                name: "SERVIÇO"
            }
        }

        ListModel {
            id: cityModel

            Component.onCompleted: {
                cityModel.append({"title": ".."})
                connDatabase.localHandler.loadCityList(connDatabase.localHandler.getFirstStateId());
                for (var i=0 ; i< connDatabase.localHandler.cityListCount(); ++i)
                    cityModel.append({"title": connDatabase.localHandler.cityListAt(i)})

                if (cityModel.count === 1) {
                    console.log("Apenas um item")
                    addCityField.opacity = 1
                    addCityField.enabled = true
                }
            }
        }

        ListModel {
            id: localModel

            Component.onCompleted: {
                connDatabase.localHandler.loadStateList();
                for (var i=0 ; i< connDatabase.localHandler.statesCount(); ++i)
                    localModel.append({"title": connDatabase.localHandler.stateListAt(i)})
            }
        }

        //######################################################################################
        // INICIO DO FORMULARIO

        Column {
            id: suppliersForm
            spacing: 10
            anchors {
                top: parent.top
                topMargin: 10
            }
            width: parent.width

            Row {
                spacing: 10

                Text {
                    id: typeText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Tipo:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                Box {
                    id: typeBox
                    boxHeight: boxHeightLarge
                    anchors.left: typeText.right
                    anchors.leftMargin: 50
                    implicitWidth: boxWidthLarge
                    model: typeModel
                }
            }

            Row {
                spacing: 10

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Razão Social:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: nameField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 550
                }
            }

            Row {
                spacing: 10

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "CNPJ:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: cnpjInput
                    anchors.margins: 20
                    inputMask: "999.999.999/9999-99"
                    style: textfieldStyle
                    implicitWidth: columnWidth + 200
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Insc. Estadual:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: stateRegField
                    anchors.margins: 20
//                    inputMask: "999.999.999/9999-99"
                    style: textfieldStyle
                    implicitWidth: columnWidth + 200
                }
            }

            Row {

                spacing: 15

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Represent.:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: nameContatctInput
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 280
                }
                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Tel. Contato:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: phoneField
                    anchors.margins: 20
                    inputMask: "(00)000000000"
                    style: textfieldStyle
                    implicitWidth: 200
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Fax:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: faxField
                    anchors.margins: 20
                    inputMask: "(00)000000000"
                    style: textfieldStyle
                    implicitWidth: 200
                }
            }

            Row {
                spacing: 15

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Endereço:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }


                TextField {
                    id: addressField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 350
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Bairro:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: districtField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 120
                }

            }

            Row {
                spacing: 20

                Text {
                    id: cityText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Cidade:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: addCityField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 140
                    enabled: false
                    opacity: 0.5
                }

                Item {
                    id: addCity

                    anchors.leftMargin: 10
                    width: 40 ; height: 40
                    enabled: addCityField.text.length
                    Image {
                        source: addMouseArea.pressed ? "../images/add_icon_pressed.png" : "../images/add_icon.png"
                        anchors.centerIn: parent
                        opacity: enabled ? 1 : 0.5
                    }
                    MouseArea {
                        id: addMouseArea
                        anchors.fill: parent
                        onClicked: {
                            connDatabase.localHandler.stateId = localModel.get(localBox.currentIndex).title;
                            connDatabase.localHandler.insertCity = addCityField.displayText;
                            //Limpa a lista
                            cityModel.clear();
                            cityModel.append({"title": ".."})
                            //Carrega os items de cidades relacionados ao estado
                            connDatabase.localHandler.loadCityList(connDatabase.localHandler.getStateId());
                            for (var i=0 ; i< connDatabase.localHandler.cityListCount(); ++i)
                                cityModel.append({"title": connDatabase.localHandler.cityListAt(i)})

                            addCityField.text = ""
                        }
                    }
                }

                Box {
                    id: cityBox
                    boxHeight: boxHeightLarge
                    anchors.left: addCity.right
                    anchors.leftMargin: 50
                    implicitWidth: boxWidthLarge

                    model: cityModel

                     onIndexChange: {
                         if (cityBox.currentIndex === 0) {
                             addCityField.enabled = true
                             addCityField.opacity = 1
                         } else {
                             addCityField.enabled = false
                             addCityField.opacity = 0.5
                         }
                     }
                }
            }

            Row {
                spacing: 300

                Text {
                    id: localText
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Estado:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                Box {
                    id: localBox
                    boxHeight: boxHeightLarge
                    anchors.left: localText.right
                    anchors.leftMargin: 50
                    implicitWidth: boxWidthLarge

                    model: localModel

                    onIndexChange: {
                        //Limpa a lista
                        cityModel.clear();
                        cityModel.append({"title": ".."})
                        //Carrega o id do grupo selecionado
                        connDatabase.localHandler.stateId = localModel.get(localBox.currentIndex).title;
                        //Carrega os items de cidades relacionados ao estado
                        connDatabase.localHandler.loadCityList(connDatabase.localHandler.getStateId());
                        for (var i=0 ; i< connDatabase.localHandler.cityListCount(); ++i)
                            cityModel.append({"title": connDatabase.localHandler.cityListAt(i)})
                   }
                }
            }

            Row {
                spacing: 15

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Complemento:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }


                TextField {
                    id: compField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 400
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

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Banco nº:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: bankInput
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 60
                }
            }

            Row {
                spacing: 15

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Agência:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: agInput
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 60
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Nº Conta:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: ccInput
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: columnWidth + 60
                }
            }

            Image {
                id: dividerBank
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
                    text: "INSERIR"
                    style: buttonStyle
                    implicitWidth: columnWidth
                    onClicked: {
                        connDatabase.suppliersHandler.type = typeModel.get(typeBox.currentIndex).name;
                        connDatabase.suppliersHandler.socialName = nameField.displayText;
                        connDatabase.suppliersHandler.cnpj = cnpjInput.displayText;
                        connDatabase.suppliersHandler.stateRegistration = stateRegField.displayText;
                        connDatabase.suppliersHandler.contactName = nameContatctInput.displayText;
                        connDatabase.suppliersHandler.phone = phoneField.displayText;
                        connDatabase.suppliersHandler.fax = faxField.displayText;

                        connDatabase.localHandler.address = addressField.displayText;
                        connDatabase.localHandler.district = districtField.displayText;
                        connDatabase.localHandler.complement = compField.displayText;
                        connDatabase.localHandler.state = localModel.get(localBox.currentIndex).title;
                        connDatabase.localHandler.city = cityModel.get(cityBox.currentIndex).title;

                        connDatabase.suppliersHandler.codeBank = bankInput.displayText;
                        connDatabase.suppliersHandler.agencia = agInput.displayText;
                        connDatabase.suppliersHandler.cc = ccInput.displayText;

                        if (connDatabase.suppliersHandler.checkDependence() === false) {
                            statusEnroll.state= "FAIL";
                            messageDialog.show();
                        } else {
                            if (connDatabase.suppliersHandler.insert() === true) {

                                if (connDatabase.localHandler.insert(3, nameField.displayText) === true) {
                                    suppliersModel.append({"name": nameField.displayText,
                                                        "cnpj": cnpjInput.displayText,
                                                        "fone_1": phoneField.displayText});

                                    nameField.text = "";
                                    cnpjInput.text = "";
                                    stateRegField.text = "";
                                    nameContatctInput.text = "";
                                    phoneField.text = "";
                                    faxField.text = "";
                                    addressField.text = "";
                                    districtField.text = "";
                                    compField.text = "";
                                    bankInput.text = "";
                                    agInput.text = "";
                                    ccInput.text = "";

                                    nameFieldFirstStage.text = "";
                                    statusEnroll.state= "SUCCESS";
                                } else {
                                    statusEnroll.state= "FAIL";
                                }
                            } else {
                                statusEnroll.state= "FAIL";
                            }
                        }
                    }
                }

                Button {

                    id: cleanButtom
                    anchors.rightMargin: 10
                    text: "LIMPAR"
                    style: buttonStyle
                    implicitWidth: columnWidth

                    onClicked: {
                        nameField.text = "";
                        cnpjInput.text = "";
                        stateRegField.text = "";
                        nameContatctInput.text = "";
                        phoneField.text = "";
                        faxField.text = "";
                        addressField.text = "";
                        districtField.text = "";
                        compField.text = "";
                        bankInput.text = "";
                        agInput.text = "";
                        ccInput.text = "";

                        nameFieldFirstStage.text = "";
                        statusEnroll.state= "NOT";
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
