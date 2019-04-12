import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

Item {
    id: root
    anchors.fill: parent
    anchors.margins: 20
    Rectangle {
        anchors.fill: parent
//        color: Material.color(Material.primary)
        Row {
            width: parent.width;
            Label {
                width: parent.width * 0.3
                wrapMode: Text.Wrap
                text: "Cloudinary Cloud Name"
            }
            TextField {
                id: cloudinaryCloudNameInput;
                width: parent.width * 0.6
                placeholderText: "pw3abc8ac"
            }
        }
        Row {}
        Row {
            anchors.bottom: parent.bottom
            Button {
                text: qsTr("Cancel")
                width: parent.width / 3
            }
            Button {
                text: qsTr("Save")
                width: parent.width / 3
            }
        }
    }
}
