import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2
import "../"

Item {
    id: root
    width: parent.width
    height: 50
    property alias background : container.color
    property var globalToastManager
    Rectangle {
        id: container
        anchors.fill: parent
        color: Material.primary
        Button {
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.1
            width: 60
            id: uploadFileSelectButton
            text: qsTr("Upload")
            Material.background: ThemeColors.darkAccent
            onClicked: {
                imageFilePicker.visible = true;
            }
        }
        FileDropArea {
            globalToastManager: root.globalToastManager
            anchors.left: uploadFileSelectButton.right
            anchors.leftMargin: parent.width * 0.1
            width: parent.width - uploadFileSelectButton.width - (parent.width * 0.3)
            height: parent.height * 0.8
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.1
            background: ThemeColors.darkAccentLight
        }

    }

    // Hidden - file picker for image
    FileDialog {
        id: imageFilePicker
        title: "Select an image to upload"
        folder: shortcuts.home
        nameFilters: "Images (*.png *.jpg *jpeg *gif *ico *bmp)"
        // @TODO?
        selectMultiple: false
        onAccepted: {
            var imagePath = imageFilePicker.fileUrl + "";
            imagePath = imagePath.replace(/file:[\/]*/,'');
            // Pass image path to uploader to start process off
            Uploader.uploadImageFromLocalPath(imagePath);
        }
        visible: false
    }
}
