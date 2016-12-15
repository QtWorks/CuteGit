import QtQuick 2.0

FlickPager {
    id: root
    property QtObject graphModel: null
    property QtObject commitsModel: null
    signal commitClicked(var commit)

    QtObject {
        id: d
        property int commitsWidth: 0
        property int graphWidth: 0
        property int fullMessageWidth: 600
    }

    width: 120
    clip: true
    state: "full"
    states: [
        State {
            name: "full"
            PropertyChanges {
                target: root
                width: d.fullMessageWidth + d.commitsWidth + d.graphWidth
            }
        },
        State {
            name: "commitsOnly"
            PropertyChanges {
                target: root
                width: d.commitsWidth + d.graphWidth
            }
        },
        State {
            name: "treeOnly"
            PropertyChanges {
                target: root
                width: 120
            }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation {
                properties: "width"
                duration: 200
            }
        }
    ]

    content: Item {
        id: innerItem
        width: root.width
        height: graph.height

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            clip: true
            spacing: graph.spacingV
            Repeater {
                model: root.commitsModel
                Rectangle {
                    width: parent.width
                    height: graph.elementHeight
                    color: textSelector.containsMouse ? "#bbbbbb" : "#00bbbbbb"
                    Item {
                        width: root.width - graph.width
                        height: sha1.height
                        anchors.right: parent.right
                        Text {
                            id: sha1
                            font.family: "Inconsolata"
                            font.pointSize: 12
                            anchors.left: parent.left
                            text: "[" + model.shortSha1 + "]"
                            Component.onCompleted: {
                                d.commitsWidth = sha1.width  + 10
                            }
                        }

                        Text {
                            anchors.left: sha1.right
                            anchors.right: parent.right
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            verticalAlignment: Text.AlignVCenter
                            text: model.summary
                            elide: Text.ElideRight
                        }
                    }
                    //TODO: Make as part of graph
                    //                    Row {
                    //                        anchors.left: parent.left
                    //                        Text {
                    //                            verticalAlignment: Text.AlignVCenter
                    //                            horizontalAlignment: Text.AlignLeft
                    //                            text: graphPointData.branch
                    //                        }
                    //                        Text {
                    //                            verticalAlignment: Text.AlignVCenter
                    //                            horizontalAlignment: Text.AlignLeft
                    //                            text: graphPointData.tag
                    //                        }
                    //                    }

                    MouseArea {
                        id: textSelector
                        anchors.fill: parent
                        hoverEnabled: true
                        focus: true
                        onClicked: {
                            root.commitClicked(model.modelData)
                        }
                    }
                }
            }
        }

        Graph {
            id: graph
            model: root.graphModel
            onWidthChanged: {
                d.graphWidth = graph.width
            }
        }
    }
}
