import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

FocusScope {
    anchors.fill: parent
    Rectangle {
        anchors.fill: parent
    }

    Text {
        anchors.centerIn: parent
        textFormat: Text.RichText
        color: "#bbbbbb"
        font.pointSize: 14
        text: qsTr("Hi!<\br> Seems you started CuteGit for the first time. So lets <a href=\"#open\"><font color=\"#666666\">open</font></a> one of your repository.");
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
