import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "./qml/components"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("DCT")

    // Application theming
    Material.theme: Material.dark

    // Top bar - has global buttons
    TopBar {
        width: parent.width
    }

    // Scrollable list of transformation sources
    TransformationList {}

    // Bottom Bar - area to drop file to upload
    BottomBar {}
}
