import QtQuick 2.0

ListView {
    id: root
    property var files: null
    signal openDiff(var file)
    spacing: 10
    model: files ? files : 0
    clip: true
    delegate: Item {
        width: commitBodyText.width
        height: commitBodyText.height + 10
        Text {
            id: commitBodyText
            anchors.bottom: parent.bottom
            font.pointSize: 10
            width: root.width
            text: modelData
            elide: Text.ElideLeft
            horizontalAlignment: Text.AlignRight
            color:control.containsMouse ? "#aaaaaa" : "#000000"
            MouseArea {
                id: control
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    root.openDiff(modelData)
                }
            }
        }
    }
}
