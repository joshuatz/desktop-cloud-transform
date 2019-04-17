import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "../"

Item {
    id: root
    width: parent.width
    height: 80
    property var settingsPopup
    property alias background : container.color
    property var newButtonCallback
    Rectangle {
        id: container
        color: Material.primary
        anchors.fill: parent
        property int numButtons: 2
        property int spacing: Math.round((root.width - addNewTrButton.width - settingsButton.width) / (numButtons + 1))

        Button {
            id: addNewTrButton
            text: qsTr("+ New Configuration")
            anchors.left: parent.left
            anchors.leftMargin: container.spacing
            anchors.top: parent.top
            anchors.topMargin: (parent.height - height) / 2
            Material.background: ThemeColors.darkAccent
            onClicked: {
                root.newButtonCallback();
            }
        }
        Button {
            id: settingsButton
            text: qsTr("Settings")
            anchors.left: addNewTrButton.right
            anchors.leftMargin: container.spacing
            anchors.top: parent.top
            anchors.topMargin: (parent.height - height) / 2
            Material.background: ThemeColors.darkAccent
            onClicked: {
                settingsPopup.open();
            }
        }
        RoundButton {
            id: helpButton
            text: "?"
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: (parent.height - height) / 2
            Material.background: ThemeColors.darkAccent
        }
    }
}
