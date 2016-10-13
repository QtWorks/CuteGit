import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import org.semlanik.nicegit 1.0

Item {
    id: root
    Row {
        id: selector
        Text {
            text: "Active repository" + repoOpenDialog.fileUrl
        }
        Button {
            text: "Choose..."
            onClicked: repoOpenDialog.open()
        }
    }

    Column {
        anchors.top: selector.bottom
        Repeater {
            model: _handler.repositories

            Text {
                text: model.name
            }
        }
    }

    Item {
    width:  childrenRect.width
    height: childrenRect.height
    anchors.right: parent.right
    Repeater {
        model: _handler.modelByHead("main")
        Rectangle {
            radius: 10
            x: model.x*(width+20)
            y: model.y*(height+10)
            width: 100
            height: 30
            color: {
                var red = ((model.x+10)*5).toString()
                var green = (128 + (model.x + 10)*5).toString()
                var blue = (128 + (model.x + 10)*5).toString()

                red = red.length < 2 ? "0" + red : red
                blue = blue.length < 2 ? "0" + blue : blue
                green = green.length < 2 ? "0" + green : green

                return "#"+red+green+blue
            }
            Text {
                text: model.shortSha1
            }
        }
    }
}
//    Row {
//        anchors.right: parent.right
//        anchors.top: parent.top
//        anchors.bottom: parent.bottom
//        Repeater {
//            model: ListModel {
//                ListElement {
//                    branch: "b3"
//                }
//                ListElement {
//                    branch: "master"
//                }
//                ListElement {
//                    branch: "b1"
//                }
//                ListElement {
//                    branch: "b2"
//                }
//                ListElement {
//                    branch: "b4"
//                }
//            }

//            Column {
//                width: 200
//                Text {
//                    id: branchName
//                    text: "Branch:" + branch
//                }
//                ListView {
//                    id: branchList
//                    height: root.height
//                    width: 200
//                    model: _handler.modelByHead(branch)
//                    delegate: Rectangle {
//                        id: idRect
//                        width: parent.width
//                        height: 80
//                        color: "#cccccc"
//                        states: [
//                            State {
//                                name:"full"
//                                PropertyChanges {
//                                    target: sha1Lable
//                                    text: model.sha1
//                                }
//                                PropertyChanges {
//                                    target: idRect
//                                    color: !model.isMerge ? "#dddddd" : "#aa6666"
//                                }
//                            },
//                            State {
//                                name:"short"
//                                PropertyChanges {
//                                    target: sha1Lable
//                                    text: model.shortSha1
//                                }
//                                PropertyChanges {
//                                    target: idRect
//                                    color: "#cccccc"
//                                }
//                            }]

//                        state: "short"

//                        Column {
//                            width: branchList.width
//                            Text {
//                                id: sha1Lable
//                                width: branchList.width
//                                maximumLineCount: 8
//                                text: model.shortSha1
//                            }
//                            Text {
//                                text: model.message
//                            }
//                        }
//                        MouseArea {
//                            hoverEnabled: true
//                            anchors.fill: parent
//                            onEntered: {
//                                parent.state = "full"
//                            }
//                            onExited: {
//                                parent.state = "short"
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
    FileDialog {
        id: repoOpenDialog
        folder: "."
        selectFolder: true
        selectMultiple: false
        onAccepted: {
            _handler.open(repoOpenDialog.fileUrl)
        }
    }
}
