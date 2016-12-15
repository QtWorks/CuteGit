import QtQuick 2.0

Rectangle {
    id: arrow
    property string source: icon.source
    property bool active: false
    signal clicked()

    anchors.right: parent.right
    anchors.left: parent.left
    height: 27
    Image {
        id: icon
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
        visible: opacity > 0.0
        opacity: active ? 1.0 : 0.0
        anchors.centerIn: parent
        source: control.pressed ? "qrc:///images/" + arrow.source + "_active.png" : "qrc:///images/" + arrow.source + ".png"
    }

    MouseArea {
        id: control
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            arrow.clicked()
        }
    }
}
