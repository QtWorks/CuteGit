import QtQuick 2.0
import QtQuick.Controls 1.4

FlickPager {
    id: root
    property QtObject graphModel: null
    property QtObject commitsModel: null
    property QtObject activeCommit: null
    signal commitClicked(var commit)

    QtObject {
        id: d
        property int commitsWidth: 0
        property int graphWidth: 0
        property int fullMessageWidth: 600
        property int annotationWidth: 200
        property int summaryAdditionalSpace: 95 //Applicable for commitsOnly state
    }

    width: 120
    clip: true
    state: "full"
    states: [
        State {
            name: "full"
            PropertyChanges {
                target: root
                width: d.fullMessageWidth + d.commitsWidth + d.graphWidth + d.annotationWidth
            }
        },
        State {
            name: "commitsOnly"
            PropertyChanges {
                target: root
                width: d.commitsWidth + d.graphWidth + d.annotationWidth + d.summaryAdditionalSpace
            }
        },
        State {
            name: "treeOnly"
            PropertyChanges {
                target: root
                width: d.graphWidth + d.annotationWidth
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
                    color: {
                        if(activeCommit && activeCommit.sha1 === model.sha1){
                            return "#bbeebb"
                        }

                        return textSelector.containsMouse ? "#bbbbbb" : "#00bbbbbb"
                    }
                    Item {
                        width: root.width - graph.width - graphAnnotation.width
                        height: sha1.height
                        anchors.right: parent.right

                        Image {
                            id: marker
                            visible: model.isMerge
                            source: "qrc:///images/flow-merge-16.png"
                        }

                        Text {
                            id: sha1
                            font.family: "Inconsolata"
                            font.pointSize: 12
                            anchors.left: marker.right
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
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onClicked: {
                            if(mouse.button === Qt.RightButton) {
                                commitMenu.popup()
                            } else {
                                root.commitClicked(model.modelData)
                                //TODO: Because active commit is used in multiple places need to make it part of some model (e.g. git handler)
                                activeCommit = model.modelData;
                            }
                        }
                    }
                    Menu {
                        id: commitMenu
                        MenuItem {
                            text: "Copy sha-id"
                            onTriggered: {
                                _handler.copy(model.sha1)
                            }
                        }
                        MenuItem {
                            text: "Checkout commit"
                            onTriggered: {
                                _handler.activeRepo.checkout(model.modelData)
                            }
                        }
                    }
                }
            }
        }

        GraphAnnotation {
            id: graphAnnotation
            elementHeight: graph.elementHeight
            spacing: graph.spacingV
            width: d.annotationWidth
        }

        Graph {
            id: graph
            anchors.left: graphAnnotation.right
            model: root.graphModel
            onWidthChanged: {
                d.graphWidth = graph.width
            }
        }
    }
}
