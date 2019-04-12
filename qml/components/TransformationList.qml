import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

Item {
    property alias background: background.color
    width: parent.width
    height: 200
    id: root
    Rectangle {
        id: background
        color: Material.primary
        anchors.fill: parent
    }
}
