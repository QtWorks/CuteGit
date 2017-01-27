import QtQuick 2.0
import QtQuick.Controls 1.4
import org.semlanik.nicegit 1.0

Item {
    id: root
    property int elementHeight: 20
    property int spacing: 10

    QtObject {
        id: d
        property int fullHeight: elementHeight + spacing
    }

    Repeater {
        id: branches
        model: _handler.branchList
        Item {
            anchors.left: parent.left
            height: d.fullHeight
            width: root.width / 2
            y: _handler.graph.point(model.oid).y*d.fullHeight - spacing/2
            clip: true
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                height: 25
                color:"#77ccff"
                Rectangle {
                    id:branchIconRect
                    width: 20
                    height: parent.height
                    color:"#55aaff"
                    Image {
                        id: branchIcon
                        anchors.centerIn: parent
                        height: 16
                        width: 10
                        source: "qrc:///images/branch.svg"
                    }
                }
                Text {
                    id: branchNameText
                    anchors.left: branchIconRect.right
                    anchors.leftMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                    text: model.fullName
                    elide: Text.ElideMiddle
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onContainsMouseChanged: {
                    if(containsMouse) {
                        if(branchNameText.implicitWidth <= branchNameText.width) {
                            return
                        }
                        var coord = parent.mapToItem(TooltipViewModel.viewport, 0, 0)
                        TooltipViewModel.x = coord.x
                        TooltipViewModel.y = coord.y + spacing/2
                        TooltipViewModel.text = model.fullName
                        TooltipViewModel.visible = true
                    } else {
                        TooltipViewModel.visible = false
                    }
                }
                onClicked: {
                    if(mouse.button === Qt.RightButton) {
                        branchMenu.popup()
                    } else {
                        root.commitClicked(model.modelData)
                    }
                }
            }
            Menu {
                id: branchMenu
                MenuItem {
                    text: "Checkout branch"
                    onTriggered: {
                        _handler.activeRepo.checkout(model.modelData)
                    }
                }
            }
        }
    }

    Repeater {
        id: tags
        model: _handler.tagList
        Item {
            id: tagContainer
            anchors.right: parent.right
            height: d.fullHeight
            width: root.width / 2
            y: _handler.graph.point(model.targetId).y*d.fullHeight - spacing/2
            clip: true
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                height: 25
                color:"#ccff77"
                Rectangle {
                    id: tagIconRect
                    width: 20
                    height: parent.height
                    color:"#aaff55"
                    Image {
                        id: tagIcon
                        anchors.centerIn: parent
                        height: 14
                        width: height
                        source: "qrc:///images/tag.svg"
                    }
                }
                Text {
                    id: tagNameText
                    anchors.left: tagIconRect.right
                    anchors.leftMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    text: model.name
                    font.pixelSize: 12
                    elide: Text.ElideRight
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onContainsMouseChanged: {
                    if(tagNameText.implicitWidth <= tagNameText.width) {
                        return
                    }

                    if(containsMouse) {
                        var coord = parent.mapToItem(TooltipViewModel.viewport, 0, 0)
                        TooltipViewModel.x = coord.x
                        TooltipViewModel.y = coord.y + spacing/2
                        TooltipViewModel.text = model.name
                        TooltipViewModel.visible = true
                    } else {
                        TooltipViewModel.visible = false
                    }
                }
            }
        }
    }
}
