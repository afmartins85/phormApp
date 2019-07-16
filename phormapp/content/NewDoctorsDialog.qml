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
    id: doctorsDataDialog

    width: 1024
    height: 600
    title: "Cadastro de Médicos"

    property alias name: nameField.text
    property alias crm: crmFieldInput.text

    //    contentItem:
    Rectangle {
        implicitWidth: 1024
        implicitHeight: 600
        color: "#E3E2D2"

        FontLoader { id: fixedFont; name: "Droid Sans" }

        ListModel {
            id: specModel

            ListElement {
                name: "CLINICO GERAL"
            }
            ListElement {
                name: "CARDIOLOGISTA"
            }
            ListElement {
                name: "DERMATOLOGISTA"
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
            id: doctorForm
            spacing: 20
            anchors {
                top: parent.top
                topMargin: 10
            }
            Row {
                spacing: 10

                Text {
                    id: specTitle
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Especialidade:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                Box {
                    id: specialityBox
                    boxHeight: boxHeightLarge
                    anchors.left: specTitle.right
                    anchors.leftMargin: 50
                    implicitWidth: boxWidthLarge

                    model: specModel
                }
            }

            Row {
                spacing: 10

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Nome:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: nameField
                    anchors.margins: 20
                    style: textfieldStyle
                    implicitWidth: 420
                }

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "CRM:"
                    font.pixelSize: 18
                    font.family: fixedFont.name
                }

                TextField {
                    id: crmFieldInput
                    anchors.margins: 20
//                    inputMask: "000000000000000"
                    style: textfieldStyle
                    implicitWidth: columnWidth + 160
                }
            }

            Row {

                spacing: 15

                Text {
                    anchors.margins: 20
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Telefone:"
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
                    implicitWidth: columnWidth + 180
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

                Button {
                    id: applyButtom
                    anchors.rightMargin: 10
                    text: "INSERIR"
                    style: buttonStyle
                    implicitWidth: columnWidth
                    onClicked: {
                        connDatabase.doctorHandler.name = nameField.displayText;
                        connDatabase.doctorHandler.crm = crmFieldInput.displayText;
                        connDatabase.doctorHandler.speciality = specModel.get(specialityBox.currentIndex).name;
//                        connDatabase.doctorHandler.firstPhone = parseInt(firstPhoneField.displayText);
                        connDatabase.doctorHandler.phone = phoneField.displayText;
                        connDatabase.doctorHandler.fax = faxField.displayText;

                        connDatabase.localHandler.address = addressField.displayText;
                        connDatabase.localHandler.district = districtField.displayText;
                        connDatabase.localHandler.complement = compField.displayText;
                        connDatabase.localHandler.state = localModel.get(localBox.currentIndex).title;
                        connDatabase.localHandler.city = cityModel.get(cityBox.currentIndex).title;

                        if (connDatabase.doctorHandler.checkDependence() === false) {
                            statusEnroll.state= "FAIL";
                            messageDialog.show();
                        } else {
                            if (connDatabase.doctorHandler.insert() === true) {

                                if (connDatabase.localHandler.insert(2, crmFieldInput.displayText) === true) {
                                    doctorsModel.append({"name": nameField.displayText,
                                                            "crm": crmFieldInput.displayText,
                                                            "fone_1": phoneField.displayText});

                                    nameField.text = "";
                                    crmFieldInput.text = "";
                                    phoneField.text = "";
                                    faxField.text = "";
                                    addressField.text = "";
                                    districtField.text = "";
                                    compField.text = "";

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
                        firstPhoneField.text;
                        secPhoneField.text = "";


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
