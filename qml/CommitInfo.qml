import QtQuick 2.0

Item {
    id: root
    state: "closed"
    clip: true
    states: [
        State {
            name: "opened"
            PropertyChanges {
                target: root
                height: childrenRect.height
            }
        },
        State {
            name: "closed"
            PropertyChanges {
                target: root
                height: shortInfo.height
            }
        }
    ]

    transitions: Transition {
        NumberAnimation {
            properties: "height"
            duration: 200
        }
    }
    Column {
        id: shortInfo
        anchors.top: parent.top
        width: parent.width
        CommitInfoLine {
            field: qsTr("Time")
            value: commit ? commit.time : ""
        }

        CommitInfoLine {
            field: qsTr("Autor")
            value: commit ? commit.author : ""
        }

        CommitInfoLine {
            field: qsTr("Email")
            value: commit ? commit.email : ""
        }
    }
    Column {
        anchors.top: shortInfo.bottom
        width: parent.width
        Text {
            wrapMode: Text.WordWrap
            width: contentWidth
            height: contentHeight
            text: qsTr("Message") + ": "
            font.pointSize: 10
            font.weight: Font.Bold
        }

        Rectangle {
            width: 10
            height: 10
        }

        Text {
            id: content
            wrapMode: Text.WordWrap
            width: parent.width
            height: contentHeight
            text: commit ? commit.message : ""
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 4
            height: 2
            radius: 2
            color: "#eeeeee"
        }
        Rectangle {
            width: 10
            height: 10
        }
    }
}
