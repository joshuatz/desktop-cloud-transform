import QtQuick 2.12
import QtQuick.Shapes 1.12

Item {
    id: root
    property alias customInset: shapeRoot.customInset
    property alias background: rect.color
    property alias outerBorderColor: root.outerBorderColor
    property alias dashedBorderColor: root.dashedBorderColor
    property alias rect: rect
    property color outerBorderColor: "black"
    property color dashedBorderColor: "black"
    width: 400
    height: 400
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "transparent"
        radius: 5
        border.color: root.outerBorderColor
        Shape {
            id: shapeRoot
            property int customInset: 0
            property int realInset: shapeRoot.customInset + borderShape.strokeWidth
            width: parent.width - (shapeRoot.realInset * 2)
            height: parent.height - (shapeRoot.realInset * 2)
            x: realInset
            y: realInset
            ShapePath {
                id: borderShape
                property int offset: borderShape.strokeWidth
                strokeColor: root.dashedBorderColor
                strokeWidth: 2
                strokeStyle: ShapePath.DashLine
                fillColor: "transparent"
                dashPattern: [1, 2]
                capStyle: ShapePath.RoundCap
                joinStyle: ShapePath.MiterJoin
                startX: 0 + borderShape.offset
                startY: 0 + borderShape.offset
                PathLine {
                    x: 0 + borderShape.offset
                    y: shapeRoot.height - borderShape.offset
                }
                // Draw right
                PathLine {
                    x: shapeRoot.width - borderShape.offset
                    y: shapeRoot.height - borderShape.offset
                }
                // Draw top
                PathLine {
                    x: shapeRoot.width - borderShape.offset
                    y: 0 + borderShape.offset
                }
                PathLine {
                    x: 0 + borderShape.offset
                    y: 0 + borderShape.offset
                }
            }
        }
    }
}
