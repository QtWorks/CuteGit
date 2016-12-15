import QtQuick 2.0

Item {
    id: root
    property string field: ""
    property string value: ""

    width: parent.width
    height:childrenRect.height + 10
    clip: true
    Row {
        id: info

        height: content.height
        Text {
            wrapMode: Text.WordWrap
            width: contentWidth
            height: contentHeight
            text: qsTr(root.field) + ": "
            font.pointSize: 10
            font.weight: Font.Bold
        }
        Text {
            id: content
            width: contentWidth
            height: contentHeight
            font.pointSize: 10
            text: root.value
        }
    }

    Rectangle {
        anchors.top: info.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 4
        height: 2
        radius: 2
        color: "#eeeeee"
    }
}
