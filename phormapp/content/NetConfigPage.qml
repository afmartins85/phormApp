import QtQuick 2.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import PhormApp 1.0

Dialog {
    width: 800
    height: 600
    title: "Manutenção de Tabelas"

    //property int test: 1
    Rectangle {
        id: mainDialog
        implicitWidth: 800
        implicitHeight: 600
        color: "#212126"

        Rectangle {
            id: ipForm
            width: 600
            height: 400
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            //color: "#191930"
            color: "#E3E2D2"

            BorderImage {
                id: name
                source: "../images/delegate.png"
                anchors.fill: parent
                border.left: 5; border.top: 5
                border.right: 5; border.bottom: 5
            }

            Column {
                spacing: 5
                anchors.horizontalCenter: parent.horizontalCenter

                Row {
                    spacing: 20
                    //anchors.horizontalCenter: parent.horizontalCenter

                    Column {
                        spacing: 5

                        Text {
                            text: "IP do Servidor: "
                            color: "black"
                            font.pixelSize: 22
                            font.bold: true
                        }

                        TextField {
                            id: ipInput
                            anchors.margins: 20
                            style: textfieldStyle
                            inputMask: "999.999.999.999"
                            implicitWidth: 230
                        }
                    }

                    Column {
                        spacing: 5

                        Text {
                            text: "Porta: "
                            color: "black"
                            font.pixelSize: 22
                            font.bold: true
                        }

                        TextField {
                            id: portInput
                            style: textfieldStyle
                            implicitWidth: 80
                        }
                    }
                }

                Text {
                    text: "BaseName: "
                    color: "black"
                    font.pixelSize: 22
                    font.bold: true
                }

                TextField {
                    id: baseNameInput
                    style: textfieldStyle
                    implicitWidth: 250
                }

                Text {
                    text: "Login: "
                    color: "black"
                    font.pixelSize: 22
                    font.bold: true
                }

                TextField {
                    id: loginInput
                    style: textfieldStyle
                    implicitWidth: 250
                }

                Text {
                    text: "Password: "
                    color: "black"
                    font.pixelSize: 22
                    font.bold: true
                }

                TextField {
                    id: passInput
                    style: textfieldStyle
                    echoMode: TextInput.Password
                    implicitWidth: 250
                }
            }

            Button {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 5
                text: "Aplicar"
                onClicked: {

                    connDatabase.databaseName = baseNameInput.text;
                    connDatabase.userName = loginInput.text;
                    connDatabase.password = passInput.text;
                    connDatabase.hostName = ipInput.text;
                    connDatabase.port = portInput.text;

                    if (connDatabase.assertConn() === true) {
                        statusConn.state= "SUCCESS";
                    } else {
                        statusConn.state= "FAIL";
                    }
                }
            }

            Component.onCompleted: {

                if (connDatabase.isConfigExists === true) {
                    ipInput.text = connDatabase.hostName;
                    portInput.text = connDatabase.port;
                    baseNameInput.text = connDatabase.databaseName;
                    loginInput.text = connDatabase.userName;
                    passInput.text = connDatabase.password;
                }

                if (connDatabase.isAssertConn === true &&
                        connDatabase.isConfigExists === true) {
                    statusConn.state= "SUCCESS";
                } else if (connDatabase.isConfigExists === false) {
                    statusConn.state= "NOT";
                } else {
                    statusConn.state= "FAIL";
                }
            }
        }

        Rectangle {
            id: statusConn
            height: 50; width: ipForm.width
            anchors.top: ipForm.bottom
            anchors.horizontalCenter: parent.horizontalCenter
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
                    PropertyChanges {target: statusConn; visible: false}
                },
                State {
                    name: "SUCCESS"
                    PropertyChanges {target: statusConn; color: "green"}
                    PropertyChanges {target: description; text: "Conexão realizada com sucesso!"}
                },
                State {
                    name: "FAIL"
                    PropertyChanges {target: statusConn; color: "red"}
                    PropertyChanges {target: description; text: "Falha ao tentar se conectar a base de dados!"}
                }
            ]
        }

    }

    property  Component textfieldStyle: TextFieldStyle {
        textColor: "black"
        font.pixelSize: 22
        font.bold: true
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

