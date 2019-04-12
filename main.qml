import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "./qml/components"
import "./qml/forms"
import "./qml"

ApplicationWindow {
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
        width: parent.width
        anchors.bottom: parent.bottom
        background: ThemeColors.darkPrimary
    }

    // Hidden elements
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
}
