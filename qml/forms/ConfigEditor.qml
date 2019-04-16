import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "../"
import "../components"
import "../animated"
import QtQuick.Layouts 1.3

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
        Item {
            id: scrollableContainer
            width: parent.width
            height: parent.height - bottomActionBar.height
            ScrollView {
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                anchors.fill: parent
                width: scrollableContainer.width
                height: scrollableContainer.height
                clip: true
                Item {
                    id: frmColumnWrapper
                    height: frmColumn.height
                    implicitHeight: frmColumn.height
                    width: root.width
                    implicitWidth: root.width
                    anchors.top: parent.top
                    anchors.left: parent.left
                    clip: false
                    Column {
                        id: frmColumn
                        width: root.width
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        clip: false
                        // There seems to be a bug in column and its own computed height - changing visibility of a row is not changing the computation automatically
                        property real computedHeight: frmRow1.height + frmRow2.height + localSaveOptionsSection.height + frmRow3.height + frmRow4.height + frmRow5.height + frmRow6.height + heightBuffer
                        property real heightBuffer: 300
//                        height: frmColumn.computedHeight
                        onHeightChanged: {
                            console.log("frmColumn height = " + frmColumn.height);
                        }
                        move: Transition {
                            NumberAnimation { properties: "x,y"; duration: 200 }
                        }

                        Row {
                            id: frmRow1
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
                            id: frmRow2
                            width: parent.width
                            HelpButton {
                                id: saveLocallyHelp
                                helpText: qsTr("If checked, after transforming on Cloudinary, this program will download the final result to your hard drive")
                            }
                            CheckBox {
                                id: saveLocallyCheckbox
                                text: qsTr("Save Locally?")
                                onCheckStateChanged: {
                                    root.comboValidator();
                                }
                            }
                        }
                        // Dynamic section - Local Save options
                        DashedRectangle {
                            id: localSaveOptionsSection
                            width: parent.width * 0.90
                            visible: saveLocallyCheckbox.checked
                            onVisibleChanged: {
                                console.log("localSaveOptionsSection visible = " + visible)
                            }
                            onHeightChanged: {
                                console.log(localSaveOptionsSection.height)
                            }
                            property real computedHeight: overwriteLocalOptionRow.height + overwriteWarning.height + heightPadding
                            property real heightPadding: 8
                            height: localSaveOptionsSection.visible ? localSaveOptionsSection.computedHeight : 0
                            background: Qt.rgba(0,0,0,0)
                            clip: true
                            Column {
                                anchors.top: parent.top
                                width: parent.width
                                height: parent.heigth
                                Row {
                                    id: overwriteLocalOptionRow
                                    height: 40
                                    width: parent.width
                                    HelpButton {
                                        id: overwriteLocalFileHelp
                                        helpText: qsTr("If checked, this program will overwrite the image you upload with the resulting generated image from Cloudinary! USE WITH CAUTION!")
                                    }
                                    CheckBox {
                                        id: overwriteLocalFileCheckbox
                                        text: qsTr("Overwrite Local File?")
                                        onCheckStateChanged: {
                                            root.comboValidator();
                                        }
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
                                Row {
                                    id: overwriteWarning
                                    width: parent.width
                                    height: 50
                                    visible: overwriteLocalFileCheckbox.checked
                                    Item {
                                        width: (parent.width - overwriteWarningRect.width) /2
                                        height: parent.height
                                    }
                                    Rectangle {
                                        id: overwriteWarningRect
                                        width: parent.width - 10
                                        height: parent.height
                                        color: Material.color(Material.Red)
                                        Text {
                                            width: parent.width - 10
                                            height: parent.height
                                            anchors.centerIn: parent
                                            color: "white"
                                            wrapMode: Text.Wrap
                                            text: qsTr("WARNING! Turning on the overwrite feature means that any local image file you pick to upload with will get 'overwritten' by the generated result. There is no undo option once a file has been overwritten")
                                        }
                                    }
                                }
                            }
                        } // </DashedRectangle,localSaveOptionsSection>
                        Row {
                            id: frmRow3
                            width: parent.width
                            HelpButton {
                                id: presetHelpButton
                                helpText: qsTr("Upload presets are defined through the Cloudinary Admin settings")
                            }
                            CheckBox {
                                id: usesPresetCheckbox
                                text: qsTr("Uses Named Preset")
                                onCheckStateChanged: {
                                    root.comboValidator();
                                }
                            }
                            TextField {
                                id: presetNameInput
                                width: (parent.width - presetHelpButton.width - usesPresetCheckbox.width) * 0.85
                                placeholderText: "Preset Name"
                                visible: usesPresetCheckbox.checked
                            }
                        }
                        Row {
                            id: frmRow4
                            width: parent.width
                            HelpButton {
                                id: namedTransHelpButton
                                helpText: qsTr("Named transformations are set through the Cloudinary Console.")
                            }
                            CheckBox {
                                id: usesNamedTransCheckbox
                                text: qsTr("Uses Named Transformation")
                                onCheckStateChanged: {
                                    root.comboValidator();
                                }
                            }
                            TextField {
                                id: namedTransInput
                                width: (parent.width - namedTransHelpButton.width - usesNamedTransCheckbox.width) * 0.85
                                placeholderText: "Transformation Name"
                                visible: usesNamedTransCheckbox.checked
                            }
                        }
                        Row {
                            id: frmRow5
                            width: parent.width
                            HelpButton {
                                id: rawTransHelpButton
                                helpText: qsTr("Transformation strings look something like 'w_300,h_300/e_colorize'")
                            }
                            CheckBox {
                                id: usesRawTransCheckbox
                                text: qsTr("Uses a Raw Transformation String")
                                onCheckStateChanged: {
                                    root.comboValidator();
                                }
                            }
                            TextField {
                                id: rawTransInput
                                width: (parent.width - rawTransHelpButton.width - usesRawTransCheckbox.width) * 0.85
                                placeholderText: "Raw Transformation String"
                                visible: usesRawTransCheckbox.checked
                            }
                        }
                        Row {
                            id: frmRow6
                            width: parent.width
                            HelpButton {
                                id: storageHelpButton
                                helpText: qsTr("'Store Original' forces this program to upload the raw image file before it is transformed and leave it on Cloudinary.")
                            }
                            CheckBox {
                                id: storeOriginalCheckbox
                                text: qsTr("Store the original image file on Cloudinary servers?")
                                onCheckStateChanged: {
                                    root.comboValidator();
                                }
                            }
                        }

                        // I shouldn't need this... something funky going on with column computed height
                        Row {
                            width: parent.width
                            height: 100
                        }
                    } // </Column>
                }
            } // </ScrollView>
        }
        Row {
            id: bottomActionBar
            width: parent.width
            anchors.bottom: parent.bottom
            leftPadding: (parent.width / divisor) / divisor
            spacing: (parent.width / divisor) / divisor
            anchors.horizontalCenter: parent.horizontalCenter
            property int numButtons: deleteButton.visible ? 3 : 2
            property int divisor: bottomActionBar.numButtons + 1
            Button {
                id: deleteButton
                icon.source: "qrc:/assets/baseline-delete_forever-24px.svg"
                Material.background: Material.color(Material.Red,Material.Shade400)
                visible: !root.isNewConfig
                onClicked: {
                    confirmDelete.open();
                }
            }

            Button {
                id: cancelButton
                text: qsTr("Cancel")
                width: parent.width / bottomActionBar.divisor
                onClicked: {
                    closeAction();
                }
                Material.background: ThemeColors.darkPrimary
            }
            Button {
                id: saveButton
                text: qsTr("Save")
                width: parent.width / bottomActionBar.divisor
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

    MessageDialog {
        id: deleteError
        title: "Error"
        text: "Failed to delete config."
    }

    MessageDialog {
        id: confirmDelete
        icon: StandardIcon.Warning
        title: qsTr("Are you sure?")
        text: qsTr("Once a configuration is deleted, it can not be recovered without recreating from scratch")
        standardButtons: StandardButton.No | StandardButton.Yes
        onYes: {
            console.log("Going to delete...");
            root.deleteAction();
        }
    }

    property var submitForm: (function(){
        var validated = true;
        var configObj;
        if (root.isNewConfig) {
            configObj = UploadConfigsList.getBlankTransformationConfig();
        }
        else {
            configObj = root.attachedConfig;
        }

        // Map all form inputs to config values...





        console.log(JSON.stringify(configObj));

        validated = GlobalSettings.updateInBulk(cloudinaryCloudNameInput.text,cloudinaryApiKeyInput.text,cloudinaryApiSecretInput.text);
        if (validated){
            cancelAction();
        }
        else {
            formError.open();
        }
    })

    property var deleteAction: (function(){
        if (root.isNewConfig==false){
            var deleteSuccess = UploadConfigsList.deleteConfigByid(root.attachedConfig.id);
            if (deleteSuccess){
                globalToastManager.show("Config Deleted!")
                root.closeAction();
            }
            else {
                deleteError.open();
            }
        }
    })

    property var comboValidator: (function(){
        // @TODO - call this fn every time a checkbox is toggled, and this will make sure only valid combos of settings are live
        // @TODO? Prompt warning if invalid combo is set before unsetting it
        if (usesPresetCheckbox.checked){
            // If using a preset, don't allow named transformation or raw trans string
            usesNamedTransCheckbox.checked = false;
            usesRawTransCheckbox.checked = false;
        }
    })
}
