import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import org.semlanik.nicegit 1.0

Item {
    id: root
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

    Flickable {
        width: root.width/2
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        contentWidth: innerItem.width
        contentHeight: innerItem.height
        Canvas {
            id: innerItem
            width: root.width/2
            property int elementWidth: 20
            property int elementHeight: 20
            height: _handler.graph.points.length*(elementWidth + 20)
            onPaint: {
                var ctx = getContext("2d")
                for(var i = 0; i < _handler.graph.points.length; i++) {
                    var point = _handler.graph.points[i]

                    ctx.beginPath()
                    ctx.fillStyle = "#"+point.color
                    ctx.roundedRect(point.x*(elementWidth + 20), point.y*(elementHeight + 20), elementWidth, elementHeight, elementWidth, elementHeight)
                    ctx.fill()
                    ctx.closePath()

                    var childPoints = point.childPoints

                    for(var j = 0; j < childPoints.length; j++) {
                        var childPoint = childPoints[j]
                        ctx.beginPath()
                        ctx.strokeStyle = "#" + childPoint.color
                        ctx.lineWidth = 2
                        if(point.x !== childPoint.x) {
                            if(point.x < childPoint.x) {
                                ctx.moveTo(point.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight + 20)
                                ctx.bezierCurveTo(
                                                  point.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight,
                                                  childPoint.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight + 20 + elementHeight/2,
                                                  childPoint.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight/2)
                            } else {
                                ctx.moveTo(point.x*(elementWidth + 20) + elementWidth/2, point.y*(elementHeight + 20) + elementHeight/2)
                                ctx.lineTo(point.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight + 20 + elementHeight)
                                ctx.bezierCurveTo(
                                                  point.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight/2,
                                                  childPoint.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight + 20  + elementHeight/2,
                                                  childPoint.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight/2)
//                                ctx.lineTo(childPoint.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight/2)
                            }
                        } else {
                            ctx.moveTo(point.x*(elementWidth + 20) + elementWidth/2, point.y*(elementHeight + 20) + elementHeight/2)
                            ctx.lineTo(childPoint.x*(elementWidth + 20) + elementWidth/2, childPoint.y*(elementHeight + 20) + elementHeight/2)
                        }
                        ctx.stroke()
                        ctx.closePath()
                    }
                }
            }
        }
        Column {
            width: parent.width
            spacing: 20
            Repeater {
                model: _handler.graph.points.length
                Rectangle {
                    width: parent.width
                    height: innerItem.elementHeight
                    color: textSelector.containsMouse ? "#9999ff" : "#009999ff"
                    property QtObject graphPointData: _handler.graph.points[_handler.graph.points.length - model.index - 1]
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        text: graphPointData.sha1
                        color: "e95f8cf36d49d7f5c084d8c17bc791170739917e" === graphPointData.sha1 ?
                                   "red" : "black"
                    }
                    Row {
                        anchors.left: parent.left
                        Text {
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            text: graphPointData.branch
                        }
                        Text {
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            text: graphPointData.tag
                        }
                    }

                    MouseArea {
                        id: textSelector
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            commitBody.visible = true
                            commitBodyText.text = _handler.graph.commitData(graphPointData)
                        }
                    }
                }
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

    Rectangle {
        id: commitBody
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width/2
        color: "white"
        visible: false
        MouseArea {
            anchors.fill: parent
            onClicked: {
                commitBody.visible = false;
            }
        }
        Text {
            id: commitBodyText
            anchors.fill: parent
        }
    }
}
