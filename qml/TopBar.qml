import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
Item {
    id: root
    anchors.right: parent.right
    anchors.left: parent.left
    height: childrenRect.height
    signal closeClicked()
    property alias closeVisible: closeControl.visible
    Row {
        anchors.left: parent.left
        anchors.leftMargin: 10
        spacing: 10
        height: 40
        Text {
            font.weight: Font.Bold
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("Active projects") + ":"
        }

        ComboBox {
            id: repositories
            anchors.verticalCenter: parent.verticalCenter
            width: 400
            model: _handler.repositories
            textRole: "name"
            onActivated: {
                _handler.activateRepository(index)
            }
            currentIndex: _handler.repositories.activeRepositoryIndex
            style: ComboBoxStyle {
                font.weight:Font.Bold
            }
        }

        Separator {
            anchors.verticalCenter: parent.verticalCenter
        }

        Button {
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("Add...")
            onClicked: repoOpenDialog.open()
        }
    }

    Row {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.rightMargin: 10
        spacing: 10
        Button {
            id: closeControl
            style: ButtonStyle {
                background: Image {
                    source: control.pressed ? "qrc:///images/x-mark-3-24_active.png" : "qrc:///images/x-mark-3-24.png"
                }
            }
            onClicked: {
                root.closeClicked()
            }
        }
        Button {
            id: help
            style: ButtonStyle {
                background: Image {
                    source: control.pressed ? "qrc:///images/question-mark-4-24_active.png" : "qrc:///images/question-mark-4-24.png"
                }
            }
            onClicked: {
                helpDialog.open()
            }
        }
    }

    FileDialog {
        id: repoOpenDialog
        folder: "."
        selectFolder: true
        selectMultiple: false
        onAccepted: {
            _handler.open(repoOpenDialog.fileUrl, true)
        }
    }

    Dialog {
        id: helpDialog
        title: qsTr("About")
        modality: Qt.NonModal
        contentItem: Item {
            implicitWidth: 400
            implicitHeight: 200
            Text {
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.horizontalCenter: parent
                wrapMode: Text.WordWrap
                font.pointSize: 12
                textFormat: Text.RichText
                text:"<p><b>CuteGit</b> is free opensource software. You're free to use and modify it in terms of GPLv3 license.<br/>" +
                     "You can support project by contributing your ideas and code to CuteGit repository.</p>" +
                     "<p><b>Author:</b> Alexey Edelev aka semlanik</p>"
            }
        }
    }
}

