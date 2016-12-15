import QtQuick 2.0

Rectangle {
    id: root
    property bool active: false
    property alias text: phText.text
    anchors.fill: parent
    opacity: active ? 1.0 : 0.0
    visible: opacity > 0
    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
    }
    Text {
        id: phText
        anchors.centerIn: parent
        color: "#bbbbbb"
        font.pointSize: 14
    }
}
