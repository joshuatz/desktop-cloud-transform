import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "../"

Item {
    property alias background: background.color
    property var configList: UploadConfigsList.configList
    property var editCallbackByConfig;
    width: parent.width
    height: 200
    id: root
    Rectangle {
        id: background
        color: Material.primary
        anchors.fill: parent
        ListView {
            id: mainListView
            anchors.fill: parent
            model: root.configList
            clip: true
            delegate: Component {
                Item {
                    id: configDelegate
                    width: root.width
                    height: 80
                    property var currConfig: root.configList[index]
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 10
                        id: configWrapper
                        radius: 30
                        color: ThemeColors.darkAccentLight
                        // Title of config
                        Item {
                            id: titleWrapper
                            width: parent.width * 0.3
                            height: parent.height
                            anchors.left: parent.left
                            Rectangle {
                                anchors.fill: parent
                                anchors.margins: 15
                                color: ThemeColors.darkPrimary
                                radius: 25
                                Text {
                                    anchors.centerIn: parent
                                    text: currConfig.userDefinedName
                                    color: "white"
                                }
                            }
                        }
                        // Drop area
                        Item {
                            id: fileDropAreaWrapper
                            anchors.left: titleWrapper.right
                            width: parent.width * 0.3
                            height: parent.height
                            FileDropArea {
                                id: fileDropArea
                                anchors.fill: parent
                                anchors.margins: 5
                                background: ThemeColors.darkPrimary
                                dashedBorderColor: "white"
                                backgroundTextColor: "white"
                                imageFilepathCallback: (function(imageFilepath){
                                    var configId = configDelegate.currConfig.id;
                                    Uploader.uploadImageWithConfigId(imageFilepath,configId);
                                })
                            }
                        }

                        // Buttons
                        Row {
                            anchors.left: fileDropAreaWrapper.right
                            anchors.leftMargin: 10
                            anchors.top: parent.top
                            anchors.topMargin: parent.height * 0.1
                            width: (parent.width * 0.4) - anchors.leftMargin
                            height: parent.height * 0.8
                            leftPadding: spacing
                            spacing: (width - filepickerButton.width - editButton.width) / 3
                            // Filepicker button
                            Button {
                                id: filepickerButton
                                icon.source: "qrc:/assets/baseline-folder_open-24px.svg"
                                Material.background: ThemeColors.darkAccent
                                onClicked: {
                                    imageFilePicker.selectedConfigId = configDelegate.currConfig.id;
                                    imageFilePicker.open();
                                }
                            }
                            // Edit button
                            Button {
                                id: editButton
                                icon.source: "qrc:/assets/baseline-edit-24px.svg"
                                Material.background: ThemeColors.darkAccent
                                onClicked: {
                                    editCallbackByConfig(configDelegate.currConfig);
                                }
                            }
                        }

                    }
                }
            }
        }
    }

    // Hidden - file picker for image
    ImageFilePicker {
        property int selectedConfigId: -1
        id: imageFilePicker
        imageFilepathCallback: (function(imageFilepath){
            var configId = imageFilePicker.selectedConfigId
            Uploader.uploadImageWithConfigId(imageFilepath,configId);
        })
    }
}
