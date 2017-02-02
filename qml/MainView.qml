import QtQuick 2.0
import QtQuick.Controls 1.4
import org.semlanik.nicegit 1.0

FocusScope {
    id: root
    property var commitsForDiff: null
    property bool controlActive: false
    focus: true
    TopBar {
        id: topBar
        onCloseClicked: {
            commitPlane.diff = null
            commitPlane.commit = null
            commitList.state = "full"
            commitList.activeCommit = null
        }
        closeVisible: commitPlane.diff != null
    }

    Rectangle {
        id: bg
        color: "#eeeeee"
        anchors.fill: commitList
    }

    CommitList {
        id: commitList
        anchors.top: topBar.bottom
        anchors.bottom: consoleContol.top
        anchors.left: parent.left

        commitsModel: _handler.commits
        graphModel: _handler.graph
        onCommitClicked: {
            if(commit.diff === null) {
                commitPlane.commit = null
                commitPlane.diff = null
                commitList.state = "full"
                return
            }

            commitList.state = "commitsOnly"
            if(!root.controlActive) {
                commitPlane.commit = commit
                root.commitsForDiff=[]
            } else {
                console.log("root.controlActive: " + root.controlActive)
                if(root.commitsForDiff === null) {
                    root.commitsForDiff = new Array(0);
                }

                console.log("Length" + root.commitsForDiff.length)
                root.commitsForDiff.push(commit)
                if(root.commitsForDiff.length === 2) {
                    commitPlane.commit = root.commitsForDiff[1]
                    commitPlane.diff = _handler.diff(root.commitsForDiff[0], root.commitsForDiff[1])
                    root.commitsForDiff=[]
                }
            }
        }
    }

    CommitPlane {
        id: commitPlane
        anchors.left: commitList.right
        anchors.right: parent.right
        anchors.top: topBar.bottom
        anchors.bottom: consoleContol.top
//        visible: diff != null
//        opacity: diff != null ? 1.0 : 0.0
    }

    ConsoleControl {
        id: consoleContol
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Keys.onPressed: {
        event.accepted = true
        switch(event.key) {
        case Qt.Key_Control:
            root.controlActive = true
            console.log("control pressed")
            break
        case Qt.Key_F4:
            consoleContol.state = consoleContol.state === "closed" ? "opened" : "closed"
            break
        case Qt.Key_G://Found the G point. Let's open console using this action.
            //TODO: Later has to be part of Console C++ class to read settings
            consoleContol.state = "opened"
            _handler.console.requestAutocomplete("g");
            break
        default:
            event.accepted = false
        }
    }

    Keys.onReleased: {
        if(event.key === Qt.Key_Control) {
            root.controlActive = false
            console.log("control released");
        }
    }

    onActiveFocusChanged: {
        root.controlActive = false
        console.log("control released");
    }

    Tooltip {
    }
}
