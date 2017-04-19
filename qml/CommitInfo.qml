import QtQuick 2.0

Item {
    id: root
    property QtObject model: null
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
        },
        State {
            name: "hidden"
            when: !root.visible || model == null
            PropertyChanges {
                target: root
                height: 0
            }
        }
    ]

    onModelChanged:{
        if(model != null) {
            root.state = "closed"
        }
    }

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
            value: model ? model.time : ""
        }

        CommitInfoLine {
            field: qsTr("Author")
            value: model ? model.author : ""
        }

        CommitInfoLine {
            field: qsTr("Email")
            value: model ? model.email : ""
        }

        CommitInfoLine {
            field: qsTr("Summary")
            value: model ? model.summary : ""
        }
    }
    Image {
        id: merge
        visible: model ? model.isMerge : false
        source: "qrc:///images/flow-merge.png"
        anchors.right: shortInfo.right
        anchors.rightMargin: 5
        anchors.top: shortInfo.top
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
            text: model ? model.message : ""
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
