import QtQuick 2.0
import QtGraphicalEffects 1.0

import org.semlanik.nicegit 1.0

ListView {
    id: root
    property QtObject diff: null
    signal openDiff(var file)
    spacing: 10
    model: diff ? diff.files : 0
    clip: true
    delegate: Item {
        property QtObject diffModel: root.diff.model(modelData)
        width: commitBodyText.width + action.width + 10
        height: commitBodyText.height + 10
        //        Rectangle {
        //            color: "red"
        //            anchors.right: action.left
        //            height: parent.height
        //            width: parent.width*diffModel.similarity/100
        //        }
        Text {
            id: commitBodyText
            anchors.bottom: parent.bottom
            font.pointSize: 10
            width: root.width - action.width - 10
            text: modelData
            elide: Text.ElideLeft
            horizontalAlignment: Text.AlignRight
            color:control.containsMouse ? "#aaaaaa" : "#000000"
            MouseArea {
                id: control
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.RightButton|Qt.LeftButton
                onClicked: {
                    if(mouse.button === Qt.RightButton) {
                        var coord = commitBodyText.mapToItem(TooltipViewModel.viewport, 0, 0)
                        TooltipViewModel.x = coord.x
                        TooltipViewModel.y = coord.y
                        TooltipViewModel.text = qsTr("Filename copied ") + modelData
                        TooltipViewModel.visible = true
                        _handler.copy(modelData)
                        tttimer.restart()
                    } else {
                        root.openDiff(modelData)
                    }
                }
            }
        }
        Image {
            id: action
            anchors.right: parent.right
            anchors.verticalCenter: commitBodyText.verticalCenter
            source: {
                if(root.diff) {
                    switch(diffModel.type) {
                    case DiffModel.Added:
                        return "qrc:///images/diff-added.png"
                    case DiffModel.Deleted:
                        return "qrc:///images/diff-removed.png"
                    case DiffModel.Modified:
                        return "qrc:///images/diff-modified.png"
                    case DiffModel.Renamed:
                        return "qrc:///images/diff-renamed.png"
                    default:
                        return "qrc:///images/diff-ignored.png"
                    }
                }
            }
        }
    }

    Timer {
        id: tttimer
        interval: 1000
        repeat: false
        onTriggered: {
            TooltipViewModel.visible = false
        }
        Component.onDestruction: {
            TooltipViewModel.visible = false
        }
    }
}
