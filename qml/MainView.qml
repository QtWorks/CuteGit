import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import org.semlanik.nicegit 1.0

Item {
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


    ListView {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 200
        anchors.right: parent.right
        model: _handler.modelByHead("master")
        delegate: Rectangle {
            id: idRect
            width: parent.width
            height: 80
            color: "#cccccc"
            states: [
                State {
                    name:"full"
                    PropertyChanges {
                        target: sha1Lable
                        text: model.sha1
                    }
                    PropertyChanges {
                        target: idRect
                        color: "#dddddd"
                    }
                },
                State {
                    name:"short"
                    PropertyChanges {
                        target: sha1Lable
                        text: model.shortSha1
                    }
                    PropertyChanges {
                        target: idRect
                        color: "#cccccc"
                    }
                }]

            state: "short"

            Text {
                id: sha1Lable
                maximumLineCount: 8
                text: model.shortSha1
            }
            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onEntered: {
                    parent.state = "full"
                }
                onExited: {
                    parent.state = "short"
                }
            }
        }
    }

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
