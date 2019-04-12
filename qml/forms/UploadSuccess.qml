import QtQuick 2.0
import "../"

Item {
    property var closeFunction
    property var uploadResult
    id: root
    Rectangle {
        anchors.fill: parent
        color: ThemeColors.darkAccentLight
        Rectangle {
            id: titleBar
            width: parent.width * 0.8
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.1
            anchors.top: parent.top
            anchors.topMargin: 20
            height: 50
            radius: 20
            color: Qt.rgba(255/255,255/255,255/255,0.8)
            Text {
                anchors.centerIn: parent
                text: qsTr("Upload success!")
                color: ThemeColors.darkPrimary
                font.pixelSize: 30
            }
        }
        Rectangle {
            id: mainMessage
            width: parent.width * 0.6
            x: parent.width * 0.2
            anchors.top: titleBar.bottom
            anchors.topMargin: 20
            Text {
                anchors.centerIn: parent
                text: qsTr("URL has been copied to clipboard. Details below:")
                color: ThemeColors.darkPrimary
                font.pixelSize: 20
            }
        }
        // @TODO use listview and delegate to provide nice fields to copy paste out of
        Text {
            anchors.top: mainMessage.bottom
            anchors.topMargin: 50
            text: root.uploadResult.url
        }
    }
}
