import QtQuick 2.0

Item {
    id: root
    property Component content: null
    property color color: "#ffffffff"
    property alias flickable: flick
    QtObject {
        id: d
        property Item flickItem: null
    }
    Flickable {
        id: flick
        anchors.top: arrowUp.bottom
        anchors.bottom:arrowDown.top
        anchors.left: parent.left
        anchors.right: parent.right
        contentWidth: d.flickItem.width
        contentHeight: d.flickItem.height
    }

    FlickPagerArrow {
        id: arrowUp
        anchors.top: parent.top
        source: "arrow-141-16"
        active: flick.contentY > 0 && root.height > 0
        onClicked: {
            flick.contentY -= (flick.contentY - flick.height) < 0 ? flick.contentY : flick.height
        }
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.color }
            GradientStop { position: 0.8; color: root.color }
            GradientStop { position: 1.0; color: "#00ffffff" }
        }
    }

    FlickPagerArrow {
        id: arrowDown
        anchors.bottom: parent.bottom
        source: "arrow-203-16"
        active: flick.contentY < (flick.contentHeight - flick.height) && root.height > 0
        onClicked: {
            flick.contentY += (flick.contentY + flick.height*2) >= flick.contentHeight ? flick.contentHeight - flick.contentY - flick.height : flick.height
        }
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#00ffffff" }
            GradientStop { position: 0.2; color: root.color }
            GradientStop { position: 1.0; color: root.color }
        }
    }

    onContentChanged: {
        if(d.flickItem != null) {
            d.flickItem.destroy()
        }

        if(content != null) {
            d.flickItem = content.createObject(flick.contentItem)
        }
    }
}
