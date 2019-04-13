import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import "../"

Item {
    property var closeFunction
    property var uploadResult
    id: root
    Rectangle {
        anchors.fill: parent
        color: ThemeColors.darkAccentLight
        Item {
            id: topBar
            width: parent.width
            height: 50
            anchors.top: parent.top
            anchors.topMargin: 20
            Rectangle {
                id: titleBar
                width: parent.width * 0.7
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.15
                height: parent.height
                radius: 20
                color: Qt.rgba(255/255,255/255,255/255,0.8)
                Text {
                    anchors.centerIn: parent
                    text: qsTr("Upload success!")
                    color: ThemeColors.darkPrimary
                    font.pixelSize: 30
                }
            }
            Item {
                anchors.left: titleBar.right
                width: parent.width * 0.15
                height: parent.height
                Button {
                    anchors.centerIn: parent
                    width: parent.width * 0.8
                    height: parent.height * 0.8
                    padding: 8
                    text: "X"
                    onClicked: {
                        root.closeFunction();
                    }
                    flat: true
                    Material.background: Material.color(Material.Red)
                }
            }
        }
        Rectangle {
            id: mainMessage
            width: parent.width * 0.6
            x: parent.width * 0.2
            anchors.top: topBar.bottom
            anchors.topMargin: 20
            Text {
                anchors.centerIn: parent
                text: qsTr("URL has been copied to clipboard. Details below:")
                color: ThemeColors.darkPrimary
                font.pixelSize: 20
            }
        }
        Item {
            id: copyableFieldsContainer
            width: parent.width * 0.9
            height: root.height - topBar.height - mainMessage.height
            x: parent.width * 0.05
            anchors.top: mainMessage.bottom
            anchors.topMargin: 10
            ListView {
                anchors.fill: parent
                anchors.topMargin: 10
                model: stringCopyFields
                delegate: Item {
                    width: parent.width
                    height: 50
                    Row {
                        width: parent.width
                        height: parent.height
                        Text {
                            id: fieldValName
                            text: fieldName
                            width: parent.width * 0.4
                        }
                        TextInput {
                            id: fieldValField
                            text: fieldVal
                            readOnly: true
                            width: parent.width * 0.4
                        }
                        Component.onCompleted: {
                            console.log(parent.height)
                            console.log(parent.width)
                        }
                    }
                }
            }
        }




        // @TODO use listview and delegate to provide nice fields to copy paste out of
        ListModel {
            id: stringCopyFields
            Component.onCompleted: {
                // Create the elements
                stringCopyFields.append([
                    {
                        "fieldName" : "URL",
                        "fieldVal" : uploadResult.url
                    },
                    {
                        "fieldName" : "id",
                        "fieldVal" : uploadResult.id
                    }
                ])
            }
        }
        ListModel {
            id: conditionalCopyFields
            ListElement {
                fieldName: "Local File Path"
                fieldVal: ""
            }
            ListElement {
                fieldName: "New Filename"
                fieldVal: ""
            }
            Component.onCompleted: {
//                conditionalCopyFields.append({})
            }
        }

        Text {
            anchors.top: mainMessage.bottom
            anchors.topMargin: 50
            text: root.uploadResult.url
        }
    }
}
