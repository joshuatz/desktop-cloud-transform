import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "."

Item {
    id: root
    property alias background: dropAreaBackground.background
    DashedRectangle {
        id: dropAreaBackground
        background: "white"
        anchors.fill: parent
        DropArea {
            anchors.fill: parent
            Rectangle {
                anchors.centerIn: parent
                Text {
                    text: qsTr("Drop to Upload")
                    anchors.centerIn: parent
                    opacity: 0.5
                    color: "white"
                }
            }
            onDropped: {
                var dropEvent = drop;
                if (dropEvent.hasText && dropEvent.proposedAction == Qt.CopyAction){
                    var filepath = dropEvent.text;
                    filepath = filepath.replace("file:///","");
                    console.log(filepath);
                    // not much info gets passed in about the file itself in the dragevent. Pass to another function to test for valid image and process
                    if (Helpers.checkValidImageFilePath(filepath)){
                        console.log("Valid image dropped! :)");
                    }
                    else {
                        // @TODO popup message
                        console.log("Non image dropped into droparea");
                    }
                }
            }
        }
    }
}
