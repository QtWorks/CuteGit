import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
Item {
    id: root
    anchors.right: parent.right
    anchors.left: parent.left
    height: childrenRect.height
    signal closeClicked()
    property alias closeVisible: closeButton.visible
    Row {
        spacing: 10
        height: 50
        Text {
            text: "Active repository" + repoOpenDialog.fileUrl
        }
        Button {
            text: "Choose..."
            onClicked: repoOpenDialog.open()
        }

        Text {
            text: _handler.activeRepo.name
        }

        FileDialog {
            id: repoOpenDialog
            folder: "."
            selectFolder: true
            selectMultiple: false
            onAccepted: {
    //TODO: repo open is not available
    //            _handler.open(repoOpenDialog.fileUrl)
            }
        }

    }

    Image {
        id: closeButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.rightMargin: 10
        source: closeControl.pressed ? "qrc:///images/x-mark-3-24_active.png" : "qrc:///images/x-mark-3-24.png"
        MouseArea {
            id: closeControl
            anchors.fill: parent
            onClicked: {
                root.closeClicked()
            }
        }
    }
}

