import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "../"
import "../components"

import QtQuick.Dialogs 1.2

Item {
    id: root
    property bool isNewConfig: true
    property var attachedConfig
    property var closeAction
    anchors.fill: parent
    anchors.margins: 20
    Rectangle {
        anchors.fill: parent
        color: ThemeColors.darkAccent
        Column {
            id: frmColumn
            width: parent.width
            height: parent.height - bottomActionBar.height
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 10
            Row {
                width: parent.width
                Label {
                    width: parent.width * 0.3
                    wrapMode: Text.Wrap
                    text: "Config Name"
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField {
                    id: configNameInput
                    width: parent.width * 0.6
                    placeholderText: "My Transformation Config"
                    text: root.isNewConfig ? "" : root.attachedConfig.userDefinedName
                }
            }
            Row {
                width: parent.width
                CheckBox {
                    id: saveLocallyCheckbox
                    text: qsTr("Save Locally?")
                }
            }
            // Dynamic section - Local Save options
            DashedRectangle {
                id: localSaveOptionsSection
                width: parent.width * 0.95
                visible: saveLocallyCheckbox.checked
                height: localSaveOptionsSection.visible ? 100 : 0
                background: Qt.rgba(0,0,0,0)
                Row {
                    anchors.top: parent.top
                    width: parent.width
                    CheckBox {
                        id: overwriteLocalFileCheckbox
                        text: qsTr("Overwrite Local File?")
                    }
                    Item {
                        width: (parent.width - overwriteLocalFileCheckbox.width) * 0.2
                    }
                    TextField {
                        id: createdFilePrefixInput
                        width: (parent.width - overwriteLocalFileCheckbox.width) * 0.8
                        placeholderText: qsTr("Suffix for created file")
                        visible: overwriteLocalFileCheckbox.checked == false
                    }
                }
            }
            Row {
                width: parent.width
                HelpButton {
                    id: presetHelpButton
                    helpText: qsTr("Upload presets are defined through the Cloudinary Admin settings")
                }
                CheckBox {
                    id: usesPresetCheckbox
                    text: qsTr("Uses Named Preset")
                }
                TextField {
                    id: presetNameInput
                    width: (parent.width - presetHelpButton.width - usesPresetCheckbox.width) * 0.85
                    placeholderText: "Preset Name"
                    visible: usesPresetCheckbox.checked
                }
            }
            Row {
                width: parent.width
                HelpButton {
                    id: namedTransHelpButton
                    helpText: qsTr("Named transformations are set through the Cloudinary Console.")
                }
                CheckBox {
                    id: usesNamedTransCheckbox
                    text: qsTr("Uses Named Transformation")
                }
                TextField {
                    id: namedTransInput
                    width: (parent.width - namedTransHelpButton.width - usesNamedTransCheckbox.width) * 0.85
                    placeholderText: "Transformation Name"
                    visible: usesNamedTransCheckbox.checked
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
                    closeAction();
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
        var validated = false;
        validated = GlobalSettings.updateInBulk(cloudinaryCloudNameInput.text,cloudinaryApiKeyInput.text,cloudinaryApiSecretInput.text);
        if (validated){
            cancelAction();
        }
        else {
            formError.open();
        }
    })
}
