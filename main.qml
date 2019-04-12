import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Controls.impl 2.0
import "./qml/animated"
import "./qml/components"
import "./qml/forms"
import "./qml"

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("DCT")

    // Application theming
    Material.theme: Material.Dark

    // Top bar - has global buttons
    TopBar {
        id: topBar
        width: parent.width
        settingsPopup: globalSettingsPopup
        background: ThemeColors.darkPrimary
    }

    // Scrollable list of transformation sources
    TransformationList {
        anchors.top: topBar.bottom
        height: parent.height - topBar.height - bottomBar.height
        width: parent.width
        background: ThemeColors.darkSecondary
    }

    // Bottom Bar - area to drop file to upload
    BottomBar {
        id: bottomBar
        globalToastManager: globalToastManager
        width: parent.width
        anchors.bottom: parent.bottom
        background: ThemeColors.darkPrimary
    }

    /**
    * Hidden elements
    */
    // Global settings popup
    Popup {
        id: globalSettingsPopup
        anchors.centerIn: parent
        width: parent.width - 50
        height: parent.height - 50
        modal: true
        dim: true
        background: Rectangle {
            color: ThemeColors.darkPrimary
        }
        contentItem: SettingsEditor{
            cancelAction: (function(){
                globalSettingsPopup.close();
            })
        }
    }
    // Progress bar modal
    Popup {
        id: mainProgressBarModal
        property bool indeterminate : true
        property alias text: progressBarModalText.text
        anchors.centerIn: parent
        width: parent.width * 0.5
        height: parent.height * 0.35
        modal: true
        dim: true
        closePolicy: Popup.NoAutoClose
        background: Rectangle {
            color: ThemeColors.darkPrimary
        }
        contentItem: Rectangle {
            anchors.fill: parent
            Rectangle {
                id: mainProgressBarMessageArea
                height: parent.height * 0.5
                width: parent.width
                color: ThemeColors.darkPrimary
                Text {
                    id: progressBarModalText
                    text: "Uploading..."
                    anchors.centerIn: parent
                    color: "white"
                }
            }

            ProgressBar {
                id: mainProgressBar
                anchors.top: mainProgressBarMessageArea.bottom
                width: parent.width
                height: parent.height * 0.5
                indeterminate: mainProgressBarModal.indeterminate
                background: Rectangle {
                    id: mainProgressBarBackground
                    implicitWidth: 200
                    implicitHeight: 4
                    y: 0
                    height: parent.height
                    color: Qt.rgba(mainProgressBar.Material.accentColor.r, mainProgressBar.Material.accentColor.g, mainProgressBar.Material.accentColor.b, 0.25)
                }
                contentItem: ProgressBarImpl {
                    implicitWidth: 200
                    width: mainProgressBarModal.width
                    implicitHeight: 80
//                    height: parent.height
                    indeterminate: mainProgressBarModal.indeterminate
                    progress: mainProgressBar.position
                    scale: mainProgressBar.mirrored ? -1 : 1
                    color: mainProgressBar.Material.accentColor
                }
            }
        }
    }
    // Success modal - popup with fields to copy to clipboard, info, etc.
    Popup {
        id: uploadSuccessModal
        property var uploadResult: Uploader.lastUploadActionResult
        anchors.centerIn: parent
        width: parent.width - 50
        height: parent.height - 50
        modal: true
        dim: true
        background: Rectangle {
            color: ThemeColors.darkPrimary
        }
        contentItem: UploadSuccess {
            uploadResult: uploadSuccessModal.uploadResult
            anchors.fill: parent
            closeFunction: (function(){
                uploadSuccessModal.close();
            })
        }
        visible: true
    }

    // Global toast manager
    ToastManager {
        id: globalToastManager
        verticalLayoutDirection: ListView.TopToBottom
        spacing: 20
        anchors.topMargin: root.height * 0.4
    }

    // Attaching some signals to the top level app window
    property bool uploadInProgress: Uploader.uploadInProgress
    onUploadInProgressChanged: {
        mainProgressBarModal.visible = root.uploadInProgress
        console.log("uploading changed");
    }
    property var lastUploadResult: Uploader.lastUploadActionResult
    onLastUploadResultChanged: {
        console.log(JSON.stringify(root.lastUploadResult));
        if (root.lastUploadResult.success){
            uploadSuccessModal.uploadResult = root.lastUploadResult;
            uploadSuccessModal.open();
            delay(5000,function(){
               uploadSuccessModal.close();
            });

        }
        else {
            // @TODO replace with animated toast
            globalToastManager.show("Upload failed!");
        }
    }

    Timer {
        id: timer
    }

    function delay(delayTime, cb) {
        timer.interval = delayTime;
        timer.repeat = false;
        timer.triggered.connect(cb);
        timer.start();
    }
}
