import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
Item {
    id: root
    property bool logEmpty: true
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
            PropertyChanges {
                target: controlBar
                opacity: 1.0
            }
        },
        State {
            name: "closed"
            PropertyChanges {
                target: root
                height: consoleInput.height + consoleInput.anchors.bottomMargin*2
            }
            PropertyChanges {
                target: consoleInput
                focus: false
            }
            PropertyChanges {
                target: controlBar
                opacity: 0.0
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
            textFormat: TextEdit.RichText
            font.family: "Inconsolata"
            font.pointSize: 11
            color: "#002b36"
            onTextChanged: root.logEmpty = consoleLog.text.length <= 0
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
                onResetLog: {
                    consoleLog.text = ""
                }
            }
        }
    }

    Rectangle {
        id: errorRect
        anchors.fill: consoleInput
        anchors.rightMargin: -2
        anchors.leftMargin: -2
        opacity: 0.6
        border.width: 1
        radius: 2

        ColorAnimation on color {
            id: fadeIn
            from: "#ffffff"
            to: "#ffaeae"
            running: false
            duration: 350
            onStopped: {
                fadeOut.start()
            }
        }

        ColorAnimation on color {
            id: fadeOut
            from: "#ffaeae"
            to: "#ffffff"
            running: false
            duration: 350
        }
    }

    TextInput {
        id: consoleInput
        anchors.bottom: parent.bottom
        height: 20
        font.weight: Font.Bold
        color: "#002b36"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.leftMargin: 3
        anchors.bottomMargin: 2
        enabled: !_handler.console.busy
        font.family: "Inconsolata"
        font.pointSize: 11
        onAccepted: {
            _handler.console.exec(consoleInput.text);
            consoleInput.text = ""
        }
        Keys.onPressed: {
            switch(event.key) {
            case Qt.Key_Up:
                event.accepted = true
                _handler.console.recentUp()
                break
            case Qt.Key_Down:
                event.accepted = true
                _handler.console.recentDown()
                break
            case Qt.Key_Tab:
                event.accepted = true
                _handler.console.requestAutocomplete(consoleInput.text)
                break
            case Qt.Key_C:
                if(event.modifiers === Qt.ControlModifier) {
                    event.accepted = true
                    consoleInput.text = ""
                    if(flick.flickable.contentHeight > flick.flickable.height) {
                        flick.flickable.contentY = flick.flickable.contentHeight - flick.flickable.height
                    }
                }
                break
            case Qt.Key_Escape:
                event.accepted = true
                consoleInput.text = ""
                break
                //TODO: choose from prediction menu
                //            case Qt.Key_Right:
                //                if(event.modifiers === Qt.ControlModifier) {
                //                    event.accepted = true
                //                    predictionsRepeater.activeIndex++
                //                    if(predictionsRepeater.activeIndex >= predictionsRepeater.model) {
                //                        predictionsRepeater.activeIndex = 0;
                //                    }
                //                }
                //                break
                //            case Qt.Key_Left:
                //                if(event.modifiers === Qt.ControlModifier) {
                //                    event.accepted = true
                //                    predictionsRepeater.activeIndex--
                //                    if(predictionsRepeater.activeIndex < 0) {
                //                        predictionsRepeater.activeIndex = predictionsRepeater.model - 1;
                //                    }
                //                }
                //                break
            }
        }

        onTextChanged: {
            predictionsRepeater.activeIndex = -1
            _handler.console.requestPredictions(consoleInput.text);
        }

        Row {
            anchors.bottom: parent.top
            spacing: 3
            Repeater {
                id: predictionsRepeater
                property var values: null
                property int activeIndex: -1
                model: values ? values.length : 0
                Rectangle {
                    opacity: 0.6
                    width: prediction.width + 10
                    height: prediction.height + 10
                    border.width: 1
                    radius: 2
                    color: predictionsRepeater.activeIndex === model.index ? "#e5f1f7" : "#ffffff"
                    Text {
                        id: prediction
                        anchors.centerIn: parent
                        text: predictionsRepeater.values[model.index]
                    }
                }
            }
        }
    }

    Row {
        id: controlBar
        anchors.right: parent.right
        anchors.rightMargin: 2
        anchors.top: parent.top
        anchors.topMargin: 2
        spacing: 10
        visible: opacity > 0

        Behavior on opacity {
            NumberAnimation {
                duration: 200
                from: 0.0
            }
        }

        Button {
            visible: !root.logEmpty
            style: ButtonStyle {
                background: Image {
                    source: control.pressed ? "qrc:///images/erase-24_active.png" : "qrc:///images/erase-24.png"
                }
            }
            onClicked: _handler.console.resetLog()
        }
        Button {
            style: ButtonStyle {
                background: Image {
                    source: control.pressed ? "qrc:///images/gear-2-24_active.png" : "qrc:///images/gear-2-24.png"
                }
            }
            onClicked: consoleSettingsMenu.popup()
        }
    }

    Menu {
        id: consoleSettingsMenu
        MenuItem {
            checkable: true
            text: "Show predictions"
            onTriggered: {
                //TODO: Once settigns are implemented need to make console more or less configurable
            }
        }
    }

    Connections {
        target: _handler.console
        onRecentChanged: {
            consoleInput.text = _handler.console.recent
        }
        onAutocomplete: {
            consoleInput.text = value
            consoleInput.cursorPosition = consoleInput.length
        }
        onPredict: {
            predictionsRepeater.values = predictions
        }
    }
}
