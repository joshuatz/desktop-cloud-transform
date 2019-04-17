import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2
import "../"

Item {
    id: root
    property alias imageFilepathCallback: root.imageFilepathCallback
    property alias open: root.open
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
            root.imageFilepathCallback(imagePath);
        }
    }
    property var imageFilepathCallback: (function(imagePath){
        // Pass image path to uploader to start process off
        Uploader.uploadImageFromLocalPath(imagePath);
    })
    property var open: (function(){
        imageFilePicker.open();
    })
}
