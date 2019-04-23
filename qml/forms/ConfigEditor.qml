import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "../components"
import "../animated"
import "../"
import QtQuick.Dialogs 1.2

Item {
    id: root
    property bool isNewConfig: true
    property var attachedConfig
    onAttachedConfigChanged: {
        root.mapConfigToFields();
    }
    onIsNewConfigChanged: {
        root.mapConfigToFields();
    }

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
                                        id: createdFileSuffixInput
                                        width: (parent.width - overwriteLocalFileCheckbox.width) * 0.8
                                        placeholderText: qsTr("Suffix for created file")
                                        visible: overwriteLocalFileCheckbox.checked === false
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
                            width: parent.width
                            HelpButton {
                                id: outgoingRawTransHelpButton
                                helpText: qsTr("Outoing transformation strings are applied AFTER the image is uploaded. Transformation strings look something like 'w_300,h_300/e_colorize'")
                            }
                            CheckBox {
                                id: usesOutgoingRawTransCheckbox
                                text: qsTr("Uses an OUTGOING Transformation String")
                                onCheckStateChanged: {
                                    root.comboValidator();
                                }
                            }
                            TextField {
                                id: outgoingRawTransInput
                                width: (parent.width - outgoingRawTransHelpButton.width - usesOutgoingRawTransCheckbox.width) * 0.85
                                placeholderText: "Raw Transformation String"
                                visible: usesOutgoingRawTransCheckbox.checked
                                selectByMouse: true
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
                        Row {
                            id: frmRow7
                            width: parent.width
                            visible: saveLocallyCheckbox.checked
                            HelpButton {
                                id: deleteCloudCopyAfterDownHelp
                                helpText: qsTr("To help stay under quota limits, you can force this program to delete the generated asset off Cloudinary once it has been downloaded. Note that this prevents the URL from working")
                            }
                            CheckBox {
                                id: deleteCloudCopyAfterDownCheckbox
                                text: qsTr("Delete the copy off Cloudinary's servers after downloaded to disc?")
                                onCheckStateChanged: {
                                    root.comboValidator();
                                }
                            }
                        }

                        // I shouldn't need this... something funky going on with column computed height
                        Row {
                            id: bufferRow
                            width: parent.width
                            height: 200
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
        var submitSuccess = false;
        var configObj;
        if (root.isNewConfig) {
            configObj = UploadConfigsList.getBlankTransformationConfig();
        }
        else {
            configObj = root.attachedConfig;
        }

        // Map all form inputs to config values...
        configObj = root.mapFieldsToConfig(configObj);

        console.log(JSON.stringify(configObj));

        if (validated){
            var newConfigId = UploadConfigsList.insertOrUpdateInStorage(configObj,!root.isNewConfig);
            submitSuccess = newConfigId >= 0;
            if (submitSuccess){
                if (root.isNewConfig){
                    globalToastManager.show("Config Created!");
                }
                else {
                    globalToastManager.show("Config Updated!");
                }
                root.closeAction();
            }
            else {
                formError.open();
            }
        }
        else {
            formError.open();
        }
    })

    property var configMapping: {
        "userDefinedName" : {
            "field" : configNameInput,
            "type" : "TextField"
        },
        "saveLocally" : {
            "field" : saveLocallyCheckbox,
            "type" : "CheckBox"
        },
        "overwriteLocalFile" : {
            "field" : overwriteLocalFileCheckbox,
            "type" : "CheckBox"
        },
        "createdFileSuffix" : {
            "field" : createdFileSuffixInput,
            "type" : "TextField"
        },
        "usesPreset" : {
            "field" : usesPresetCheckbox,
            "type" : "CheckBox"
        },
        "presetName" : {
            "field" : presetNameInput,
            "type" : "TextField"
        },
        "usesNamedTransformation" : {
            "field" : usesNamedTransCheckbox,
            "type" : "CheckBox"
        },
        "namedTransformation" : {
            "field" : namedTransInput,
            "type" : "TextField"
        },
        "usesTransformationRawString" : {
            "field" : usesRawTransCheckbox,
            "type" : "CheckBox"
        },
        "transformationRawString" : {
            "field" : rawTransInput,
            "type" : "TextField"
        },
        "usesOutgoingTransformationRawString" : {
            "field" : usesOutgoingRawTransCheckbox,
            "type" : "CheckBox"
        },
        "outoingTransformationRawString" : {
            "field" : outgoingRawTransInput,
            "type" : "TextField"
        },
        "storeOriginal" : {
            "field" : storeOriginalCheckbox,
            "type" : "CheckBox"
        },
        "deleteCloudCopyAfterDownload" : {
            "field" : deleteCloudCopyAfterDownCheckbox,
            "type" : "CheckBox"
        }
    }

    property var mapConfigToFields: (function(force){
        if (root.isNewConfig == false || force===true){
        var mapping = root.configMapping;
            for (var x=0; x<Object.keys(mapping).length; x++){
                var qPropName = Object.keys(mapping)[x];
                var fieldSet = mapping[qPropName];
                if (fieldSet.type === "CheckBox"){
                    fieldSet.field.checked = root.attachedConfig[qPropName];
                }
                else if (fieldSet.type === "TextField"){
                    fieldSet.field.text = root.attachedConfig[qPropName].toString();
                }
            }
        }
    })

    property var mapFieldsToConfig: (function(config){
        var mapping = root.configMapping;
        for (var x=0; x<Object.keys(mapping).length; x++){
            var qPropName = Object.keys(mapping)[x];
            var fieldSet = mapping[qPropName];
            if (fieldSet.type === "CheckBox"){
                config[qPropName] = fieldSet.field.checked;
            }
            else if (fieldSet.type === "TextField"){
                config[qPropName] = fieldSet.field.text;
            }
        }
        return config;
    })

    property var deleteAction: (function(){
        if (root.isNewConfig==false){
            var deleteSuccess = UploadConfigsList.deleteConfigByid(root.attachedConfig.id);
            console.log("Deleting config #" + root.attachedConfig.id);
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
        if (usesNamedTransCheckbox.checked){
            usesPresetCheckbox.checked = false;
            usesRawTransCheckbox.checked = false;
        }
    })

    property var resetForm: (function(){
        root.attachedConfig = UploadConfigsList.getBlankTransformationConfig();
        root.mapConfigToFields(true);
    })

    Component.onCompleted: {
        root.mapConfigToFields();
    }
}
