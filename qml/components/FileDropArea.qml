import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "."

Item {
    id: root
    property alias background: dropAreaBackground.background
    DashedRectangle {
        id: dropAreaBackground
        background: "white"
        anchors.fill: parent
        DropArea {
            anchors.fill: parent
            Rectangle {
                anchors.centerIn: parent
                Text {
                    text: qsTr("Drop to Upload")
                    anchors.centerIn: parent
                    opacity: 0.5
                    color: "white"
                }
            }

//            MouseArea {
//                id: hoverArea
//                anchors.fill: parent
//                hoverEnabled: true
//            }
        }
    }
}
