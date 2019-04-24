import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "."
import "../"
import "../animated"

Item {
    id: root
    property var closeAction
    anchors.fill: parent
    anchors.margins: 20
    Rectangle {
        id: mainContainer
        anchors.fill: parent
        color: ThemeColors.darkAccent
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
                    text: "About DCT - v" + Helpers.getVersionString()
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
                        root.closeAction();
                    }
                    flat: true
                    radius: width * 0.5
                    Material.background: Material.color(Material.Red)
                }
            }
        }

        Column {
            id: mainColumn
            anchors.top: topBar.bottom
            anchors.topMargin: 10
            width: parent.width
            spacing: 10
            Row {
                width: parent.width
                Item {
                    width: 10
                    height: parent.height
                }
                Rectangle {
                    width: parent.width - (10*2)
                    color: Qt.rgba(255/255,255/255,255/255,0.8)
                    height: mainAboutText.height
                    Text {
                        id: mainAboutText
                        width: parent.width
                        padding: 10
                        font.pixelSize: 12
                        wrapMode: Text.Wrap
                        text: "Desktop Cloud Transform (DCT) was created by Joshua Tzucker. For more information, visit my website at joshuatz.com, or by clicking the link below"
                    }
                }
            }
            Row {
                width: parent.width
                Item {
                    height: parent.height
                    width: (parent.width - linkButton.width) / 2
                }
                Button {
                    id: linkButton
                    Material.background: ThemeColors.darkAccentLight
                    icon.source: "qrc:/assets/baseline-link-24px.svg"
                    text: "joshuatz.com Project Page"
                    onClicked: {
                        Qt.openUrlExternally("https://joshuatz.com/");
                    }
                }
            }
            Row {
                width: parent.width
                Item {
                    height: parent.height
                    width: (parent.width - attributionHeading.width) / 2
                }
                Text {
                    id: attributionHeading
                    text: qsTr("Attributions and Licenses:")
                    font.pixelSize: 18
                    color: "white"
                }
            }
        } // </column>

        ScrollView {
            id: attributionWrapperArea
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            anchors.top: mainColumn.bottom
            anchors.topMargin: 10
            height: parent.height - topBar.height -  mainColumn.height - 50
            width: parent.width * 0.9
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.05
            clip: true

            TextArea {
                id: attributionTextArea
                readOnly: true
                wrapMode: Text.Wrap
                text: "Loading text..."
                MouseArea {
                    anchors.fill: parent
                    enabled: false
                }
                color: "white"
                clip: false
                activeFocusOnPress: false
            }
        }
    }
    Component.onCompleted: {
        // Load local text file into textarea
        var localTextFilepath= "qrc:/assets/AttributionsAndLicenses.txt";
        var xhr = new XMLHttpRequest;
        xhr.open("GET", localTextFilepath);
        xhr.onreadystatechange = function () {
            if(xhr.readyState === XMLHttpRequest.DONE){
                var response = xhr.responseText;
                attributionTextArea.text = response
            }
        }
        xhr.send();
    }
}
