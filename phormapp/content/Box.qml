import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.3

Item {
    id: root

    property alias boxHeight: boxComponent.height
    property alias implicitWidth: boxComponent.implicitWidth
    property alias model: boxComponent.model
    property alias currentIndex: boxComponent.currentIndex
    property int sizeText: boxHeight / 3

    signal indexChange

    ComboBox {
        id: boxComponent
        currentIndex: 0
        activeFocusOnPress: true
        style: ComboBoxStyle {
            id: comboBox
            background: Rectangle {
                id: rectCategory
                radius: 5
                border.width: 2
//                color: "#fff"
                color: "darkBlue"
            }
            label: Text {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: sizeText
                font.bold: true
                font.family: "Droid Sans"
                font.capitalization: Font.SmallCaps
                color: "white"
                text: control.currentText
            }

            // drop-down customization here
            property Component __dropDownStyle: MenuStyle {
                __maxPopupHeight: 600
                __menuItemType: "comboboxitem"

                frame: Rectangle {              // background
                    color: "#fff"
                    border.width: 2
                    radius: 5
                }

                itemDelegate.label:             // an item text
                    Text {
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: sizeText
                    font.bold: true
                    font.family: "Droid Sans"
                    font.capitalization: Font.SmallCaps
                    color: styleData.selected ? "white" : "black"
                    text: styleData.text
                }

                itemDelegate.background: Rectangle {  // selection of an item
                    radius: 2
                    color: styleData.selected ? "darkGray" : "transparent"
                }

                __scrollerStyle: ScrollViewStyle { }
            }

            property Component __popupStyle: Style {
                property int __maxPopupHeight: 400
                property int submenuOverlap: 0

                property Component frame: Rectangle {
                    width: (parent ? parent.contentWidth : 0)
                    height: (parent ? parent.contentHeight : 0) + 2
                    border.color: "black"
                    property real maxHeight: 500
                    property int margin: 1
                }

                property Component menuItemPanel: Text {
                    text: "NOT IMPLEMENTED"
                    color: "red"
                    font {
                        pixelSize: 14
                        bold: true
                    }
                }

                property Component __scrollerStyle: null
            }
        }

        onCurrentIndexChanged: {
            root.indexChange();
        }
    }
}
