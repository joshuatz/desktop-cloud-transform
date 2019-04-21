import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "."
import "../animated"

Item {
    id: root
    property alias fileDropCallback: root.fileDropCallback
    property alias imageFilepathCallback: root.imageFilepathCallback
    property alias background: dropAreaBackground.background
    property alias outerBorderColor: dropAreaBackground.outerBorderColor
    property alias dashedBorderColor: dropAreaBackground.dashedBorderColor
    property var globalProgressBarModal
    property var globalToastManager
    property string backgroundText: "Drop to Upload"
    property alias backgroundTextColor: backgoundText.color
    DashedRectangle {
        id: dropAreaBackground
        background: "white"
        outerBorderColor: "black"
        dashedBorderColor: "black"
        anchors.fill: parent
        DropArea {
            anchors.fill: parent
            Rectangle {
                anchors.centerIn: parent
                Text {
                    id: backgoundText
                    text: qsTr(root.backgroundText)
                    anchors.centerIn: parent
                    opacity: 0.5
                    color: "black"
                }
            }
            onDropped: {
                var dropEvent = drop;
                root.fileDropCallback(dropEvent);
            }
        }
    }

    property var fileDropCallback: (function(dropEvent){
        if (dropEvent.hasText && dropEvent.proposedAction === Qt.CopyAction){
            var filepath = dropEvent.text;
            filepath = filepath.replace("file:///","");
            console.log(filepath);
            // not much info gets passed in about the file itself in the dragevent. Pass to another function to test for valid image and process
            if (Helpers.checkValidImageFilePath(filepath)){
                console.log("Valid image dropped! :)");
                root.imageFilepathCallback(filepath);
            }
            else {
                // @TODO popup message / nice toast
                console.log("Non image dropped into droparea");
                root.globalToastManager.show("Non image dropped into droparea");
            }
        }
    })

    property var imageFilepathCallback: (function(filepath){
        Uploader.uploadImageFromLocalPath(filepath);
    })
}
