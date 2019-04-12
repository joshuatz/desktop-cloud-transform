import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "../"

Item {
    id: root
    anchors.fill: parent
    anchors.margins: 20
    property var cancelAction
    Rectangle {
        anchors.fill: parent
        color: ThemeColors.darkAccent
        Row {
            width: parent.width
            Item {
                width: parent.width * 0.05
                height: parent.height
            }
            Label {
                width: parent.width * 0.3
                wrapMode: Text.Wrap
                text: "Cloudinary Cloud Name"
                anchors.verticalCenter: parent.verticalCenter
            }
            TextField {
                id: cloudinaryCloudNameInput
                width: parent.width * 0.6
                placeholderText: "pw3abc8ac"
            }
        }
        Row {}
        Row {
            width: parent.width
            anchors.bottom: parent.bottom
            leftPadding: (parent.width / 3) / 3
            spacing: (parent.width / 3) / 3
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                id: cancelButton
                text: qsTr("Cancel")
                width: parent.width / 3
                onClicked: {
                    cancelAction();
                }
                Material.background: ThemeColors.darkPrimary
            }
            Button {
                id: saveButton
                text: qsTr("Save")
                width: parent.width / 3
                onClicked: {
                    submitForm();
                }
                Material.background: ThemeColors.darkPrimary
            }
        }
    }
    property var submitForm: (function(){
        //
    })
}
