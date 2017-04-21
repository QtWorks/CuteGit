import QtQuick 2.0
import org.semlanik.cutegit 1.0

Rectangle {
    id: root
    x: TooltipViewModel.x
    y: TooltipViewModel.y
    visible: opacity > 0
    opacity: TooltipViewModel.visible ? 1.0 : 0.0
    width: childrenRect.width + 10
    height: 20
    color: "#eeeeee"
    border.color: "#000000"
    border.width: 1

    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
    }

    Text {
        id: textArea
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        width: textArea.contentWidth
        text: TooltipViewModel.text
    }
    Component.onCompleted: {
        TooltipViewModel.viewport = root.parent
    }
}
