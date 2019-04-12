import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "./qml/components"
import "./qml/forms"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("DCT")

    // Application theming
    Material.theme: Material.Dark

    // Top bar - has global buttons
    TopBar {
        width: parent.width
        settingsPopup: globalSettingsPopup
    }

    // Scrollable list of transformation sources
    TransformationList {}

    // Bottom Bar - area to drop file to upload
    BottomBar {
        width: parent.width
        anchors.bottom: parent.bottom
    }

    // Hidden elements
    Popup {
        id: globalSettingsPopup
        anchors.centerIn: parent
        width: parent.width - 50
        height: parent.height - 50
        modal: true
        contentItem: SettingsEditor{}
    }
}
