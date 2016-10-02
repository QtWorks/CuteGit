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
        height: 200
        width: 100
        anchors.right: parent.right
        model: _handler.modelByHead("master")
        delegate: Rectangle {
            color: "#cccccc"
            width: 200
            height: 100
            Text {
                anchors.centerIn: parent
                text: model.sha1
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
