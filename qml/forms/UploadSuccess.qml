import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import "../"
import "../animated"

Item {
    property var closeFunction
    property var uploadResult
    property var globalToastManager
    property bool hasConfig: root.uploadResult.hasAttachedConfig
    property var attachedConfig: root.uploadResult.attachedConfig
    property string title: root.hasConfig ? "Tranformation Complete!" : "Upload Complete!";
    property string subTitle: ((root.hasConfig && root.attachedConfig.saveLocally) ? "File saved and " : "") + "URL copied to clipboard. Details below:"
    id: root
    Rectangle {
        anchors.fill: parent
        color: ThemeColors.darkAccentLight
        Item {
            id: topBar
            width: parent.width
            height: 50
            anchors.top: parent.top
            anchors.topMargin: 20
            Rectangle {
                id: titleBar
                width: parent.width * 0.7
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.15
                height: parent.height
                radius: 20
                color: Qt.rgba(255/255,255/255,255/255,0.8)
                Text {
                    anchors.centerIn: parent
                    text: qsTr(root.title)
                    color: ThemeColors.darkPrimary
                    font.pixelSize: 30
                }
            }
            Item {
                anchors.left: titleBar.right
                width: parent.width * 0.15
                height: parent.height
                RoundButton {
                    anchors.centerIn: parent
                    width: height
                    height: parent.height * 0.9
                    padding: 8
                    text: "X"
                    onClicked: {
                        root.closeFunction();
                    }
                    flat: true
                    radius: width * 0.5
                    Material.background: Material.color(Material.Red)
                }
            }
        }
        Rectangle {
            id: mainMessage
            width: parent.width * 0.6
            x: parent.width * 0.2
            anchors.top: topBar.bottom
            anchors.topMargin: 20
            Text {
                anchors.centerIn: parent
                text: qsTr(root.subTitle)
                color: ThemeColors.darkPrimary
                font.pixelSize: 20
            }
        }
        Loader {
            id: imagePreviewAreaLoader
            property string imagePath: ""
            anchors.top: mainMessage.bottom
            anchors.topMargin: 14
            width: parent.width
            height: imagePreviewAreaLoader.visible ? 100 : 0
            visible: false
        }

        Item {
            id: copyableFieldsContainer
            width: parent.width * 0.9
            height: root.height - topBar.height - mainMessage.height - imagePreviewAreaLoader.height
            x: parent.width * 0.05
            anchors.top: imagePreviewAreaLoader.bottom
            anchors.topMargin: 10
            ListView {
                id: copyableFieldsListview
                anchors.fill: parent
                anchors.topMargin: 10
                model: stringCopyFields
                spacing: 20
                delegate: Component {
                    Item {
                        id: copyableFieldsDelegate
                        width: copyableFieldsListview.width
                        height: 50
                        Row {
                            width: parent.width
                            height: parent.height
                            Rectangle {
                                width: parent.width * 0.2
                                height: parent.height
                                radius: 6
                                Text {
                                    id: fieldValName
                                    text: fieldName
                                    anchors.centerIn: parent
                                }
                            }
                            Item {
                                width: parent.width * 0.05
                                height: parent.height
                            }
                            Rectangle {
                                width: parent.width * 0.6
                                height: parent.height
                                radius: 6
                                TextInput {
                                    id: fieldValField
                                    text: fieldVal
                                    readOnly: false
                                    anchors.fill: parent
                                    anchors.margins: 6
                                    wrapMode: Text.WordWrap
                                    selectByMouse: true
                                    layer.enabled: true
                                    onTextChanged: {
                                        // Reset to true value
                                        fieldValField.text = fieldVal;
                                    }
                                }
                            }
                            Item {
                                width: parent.width * 0.05
                                height: parent.height
                            }
                            Item {
                                width: 60
                                height: parent.height
                                Button {
                                    anchors.top: parent.top
                                    anchors.topMargin: -6
                                    width: parent.width + 0
                                    height: parent.height + 12
                                    Material.background: ThemeColors.darkAccent
                                    icon.source: "qrc:/assets/iconfinder_icon-33-clipboard-add_314289_black.svg"
                                    onClicked: {
                                        // Copy the attached field to clipboard
                                        Helpers.copyToClipboard(fieldVal);
                                        localToastManager.show("Copied to clipboard!");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        root.resetModal();
    }

    /**
    * Hidden Elements
    */
    ToastManager {
        id: localToastManager
        verticalLayoutDirection: ListView.TopToBottom
        spacing: 20
        anchors.topMargin: root.height * 0.4
    }

    // @TODO use listview and delegate to provide nice fields to copy paste out of
    ListModel {
        id: stringCopyFields
    }

    Component {
        id: inlinePreviewImageComponent
        Rectangle {
            width: imagePreviewAreaLoader.width
            height: imagePreviewAreaLoader.height
            color: Qt.rgba(0,0,0,0)
            Image {
                height: parent.height - 10
                width: parent.width * 0.5
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                source: imagePreviewAreaLoader.imagePath
            }
        }
    }

    property var resetModal: (function(){
        var suppressUrlField = false;
        imagePreviewAreaLoader.visible = false;
        stringCopyFields.clear();
        stringCopyFields.append([
            {
                "fieldName" : "id",
                "fieldVal" : uploadResult.id
            }
        ]);
        if (root.hasConfig){
            var imageToDisplayPath = "";
            if (root.attachedConfig.saveLocally){
                imageToDisplayPath = Helpers.formatFilePathForQml(root.uploadResult.localSavePath);
                stringCopyFields.append([
                    {
                        "fieldName" : "Local File Path",
                        "fieldVal" : root.uploadResult.localSavePath
                    }
                ]);
            }
            if (root.attachedConfig.deleteCloudCopyAfterDownload){
                suppressUrlField = true;
            }
            else {
                imageToDisplayPath = root.uploadResult.url;
            }
            imagePreviewAreaLoader.visible = true;
            imagePreviewAreaLoader.imagePath = imageToDisplayPath;
            imagePreviewAreaLoader.sourceComponent = inlinePreviewImageComponent;
        }
        if (suppressUrlField === false){
            stringCopyFields.append([
                {
                    "fieldName" : "URL",
                    "fieldVal" : uploadResult.url
                }
            ]);
        }
    })
}
