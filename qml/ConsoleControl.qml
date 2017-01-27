import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    id: root
    Rectangle {
        anchors.fill: parent
        color: "#839496"

    }

    states: [
        State {
            name: "opened"
            when: consoleInput.focus === true
            PropertyChanges {
                target: root
                height: 250
            }
            PropertyChanges {
                target: consoleInput
                focus: true
            }
        },
        State {
            name: "closed"
            PropertyChanges {
                target: root
                height: consoleInput.height
            }
            PropertyChanges {
                target: consoleInput
                focus: false
            }
        }
    ]

    state: "closed"

    FlickPager {
        id: flick
        clip: true
        anchors.top: parent.top
        anchors.bottom:consoleInput.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        color: "#839496"
        content: Text {
            id: consoleLog
            width: root.width - 10
            height: contentHeight
            textFormat: Text.RichText
            color: "#002b36"
            Connections {
                target: _handler.console
                onCommandLog: {
                    consoleLog.text = consoleLog.text + data
                    if(flick.flickable.contentHeight > flick.flickable.height) {
                        flick.flickable.contentY = flick.flickable.contentHeight - flick.flickable.height
                    }
                }
                onCommandError: {
                    fadeIn.start()
                }
            }
        }
    }

    Rectangle {
        id: errorRect
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: consoleInput.top
        anchors.bottom: consoleInput.bottom
        color: "#ff0000"
        opacity: 0.0
        visible: opacity > 0

        NumberAnimation {
            id: fadeIn
            target: errorRect
            property: "opacity"
            duration: 350
            from: 0.0
            to: 1.0
            onStopped: {
                fadeOut.start()
            }
        }

        NumberAnimation {
            id: fadeOut
            target: errorRect
            property: "opacity"
            duration: 350
            from: 1.0
            to: 0.0
        }
    }

    TextInput {
        id: consoleInput
        anchors.bottom: parent.bottom
        height: 30
        font.weight: Font.Bold
        color: "#002b36"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        enabled: !_handler.console.busy
        onAccepted: {
            _handler.console.exec(consoleInput.text);
            consoleInput.text = ""
        }
        Keys.onPressed: {
            switch(event.key) {
            case Qt.Key_Up:
                event.accepted = true
                _handler.console.recentUp();
                break;
            case Qt.Key_Down:
                event.accepted = true
                _handler.console.recentDown();
                break;
            }
        }
    }

    Connections {
        target: _handler.console
        onRecentChanged: {
            consoleInput.text = _handler.console.recent
        }
    }
}
