import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "../"

import QtQuick.Dialogs 1.2

Item {
    id: root
    height: parent.height < 40 ? parent.height : 40
    width: 40
    property string helpText: ""
    RoundButton {
        anchors.fill: parent
        radius: 18
        icon.source: "qrc:/assets/baseline-help_outline-24px.svg"
        ToolTip.text: root.helpText
        ToolTip.visible: buttonHoverArea.containsMouse
        MouseArea {
            id: buttonHoverArea
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                helpDialog.open();
            }
        }
    }

    MessageDialog {
        id: helpDialog
        title: "Info"
        text: root.helpText
    }
}
