import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "../"

import QtQuick.Dialogs 1.2

Item {
    id: root
    anchors.fill: parent
    anchors.margins: 20
    property var cancelAction
    Rectangle {
        anchors.fill: parent
        color: ThemeColors.darkAccent
        Column {
            id: frmColumn
            width: parent.width
            height: parent.height - bottomActionBar.height
            anchors.top: parent.top
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
                    text: GlobalSettings.getCloudinaryCloudName();
                }
            }
            Row {
                width: parent.width
                Item {
                    width: parent.width * 0.05
                    height: parent.height
                }
                Label {
                    width: parent.width * 0.3
                    wrapMode: Text.Wrap
                    text: "Cloudinary API Key"
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField {
                    id: cloudinaryApiKeyInput
                    width: parent.width * 0.6
                    placeholderText: "123456789012123"
                    text: GlobalSettings.getCloudinaryApiKey();
                }
            }
            Row {
                width: parent.width
                Item {
                    width: parent.width * 0.05
                    height: parent.height
                }
                Label {
                    width: parent.width * 0.3
                    wrapMode: Text.Wrap
                    text: "Cloudinary API Secret"
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField {
                    id: cloudinaryApiSecretInput
                    width: parent.width * 0.6
                    placeholderText: "KNI5QG3uL2v8v846mqVycfD1LYg"
                    text: GlobalSettings.getCloudinaryApiSecret();
                }
            }
        }
        Row {
            id: bottomActionBar
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
    MessageDialog {
        id: formError
        title: "Error"
        text: "Something went wrong... Did you fill out all fields?"
    }

    property var submitForm: (function(){
        var validated = GlobalSettings.updateInBulk(cloudinaryCloudNameInput.text,cloudinaryApiKeyInput.text,cloudinaryApiSecretInput.text);
        if (validated){
            cancelAction();
        }
        else {
            formError.open();
        }
    })
}
