import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

FocusScope {
    Rectangle {
        anchors.fill: parent
    }

    Text {
        textFormat: Text.RichText
        font.pointSize: 10
        text: qsTr("Hi!<\br> Seems you started CuteGit for the first time. So lets <a href=\"#open\">open</a> one of your repository.");
        onLinkActivated: {
            if(link == "#open") {
                repoOpenDialog.open()
            }
        }
    }

    FileDialog {
        id: repoOpenDialog
        folder: _handler.homePath
        selectFolder: true
        selectMultiple: false
        onAccepted: {
            _handler.open(repoOpenDialog.fileUrl, true)
        }
    }
}
