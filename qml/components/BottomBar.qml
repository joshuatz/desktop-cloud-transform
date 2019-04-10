import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

Item {
    id: root
    width: parent.width
    height: 50
    Rectangle {
        anchors.fill: parent
        color: Material.color(Material.primary)
        Button {
            id: uploadFileSelectButton
            text: qsTr("Upload")
        }
        FileDropArea {
            anchors.left: uploadFileSelectButton.right
            width: 200
            height: 40
        }

    }
}
