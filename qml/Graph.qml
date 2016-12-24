import QtQuick 2.0

Canvas {
    id: root
    property int elementWidth: 20
    property int elementHeight: 20
    property int spacingH: 12
    property int spacingV: 20
    property int lineWidth: 1
    property QtObject model: null
    height: model.points.count*(elementWidth + spacingV)
    width: (elementWidth + spacingH)*model.branchesCount
    property int headY: 0
    QtObject {
        id: d
        property int halfElementWidth: elementWidth/2
        property int halfElementHeight: elementHeight/2
        Component.onCompleted: {
            root.loadImage("qrc:///images/aim.svg")
            _handler.activeRepo.headChanged.connect(update)
        }
        function update() {
            root.requestPaint()
        }
    }

    onPaint: {
        var ctx = getContext("2d")
        ctx.clearRect(region.x, region.y ,region.width, region.height)
        for(var i = 0; i < model.points.count; i++) {

            var point = model.points.at(i)
            var pointAbsX = point.x*(elementWidth + spacingH)
            var pointAbsY = point.y*(elementHeight + spacingV)
            var childPoints = point.childPoints

            if(!_handler.activeRepo.isHead(point.oid)) {
                ctx.beginPath()
                ctx.lineWidth = root.lineWidth
                ctx.strokeStyle = "#"+point.color
                ctx.roundedRect(pointAbsX, pointAbsY, elementWidth, elementHeight, elementWidth, elementHeight)
                ctx.stroke()
                ctx.closePath()
            } else {
                ctx.beginPath()
                root.headY = pointAbsY;
//                ctx.lineWidth = root.lineWidth
                ctx.fillStyle = "#"+point.color
                ctx.roundedRect(pointAbsX, pointAbsY, elementWidth, elementHeight, elementWidth, elementHeight)
                ctx.fill()
                ctx.closePath()
                ctx.drawImage("qrc:///images/aim.svg", pointAbsX + 1, pointAbsY + 1, 18, 18)
            }

            for(var j = 0; j < childPoints.length; j++) {
                var childPoint = childPoints[j]
                var childPointAbsX = childPoint.x*(elementWidth + spacingH)
                var childPointAbsY = childPoint.y*(elementHeight + spacingV)
                ctx.beginPath()
                ctx.strokeStyle = "#" + childPoint.color
                ctx.lineWidth = root.lineWidth
                if(point.x !== childPoint.x) {
                    if(point.x < childPoint.x) {
                        ctx.moveTo(pointAbsX + d.halfElementWidth, childPointAbsY + elementHeight + spacingV)
                        ctx.bezierCurveTo(
                                    pointAbsX + d.halfElementWidth, childPointAbsY + elementHeight,
                                    childPointAbsX + d.halfElementWidth, childPointAbsY + elementHeight + spacingV/* + d.halfElementHeight*/,
                                    childPointAbsX + d.halfElementWidth, childPointAbsY + elementHeight/*d.halfElementHeight*/)
                    } else {
                        ctx.moveTo(pointAbsX + d.halfElementWidth, pointAbsY/* + d.halfElementHeight*/)
                        ctx.lineTo(pointAbsX + d.halfElementWidth, childPointAbsY + elementHeight + spacingV + elementHeight)
                        ctx.bezierCurveTo(
                                    pointAbsX + d.halfElementWidth, childPointAbsY + d.halfElementHeight,
                                    childPointAbsX + d.halfElementWidth, childPointAbsY + elementHeight + spacingV/*  + d.halfElementHeight*/,
                                    childPointAbsX + d.halfElementWidth, childPointAbsY + elementHeight/*d.halfElementHeight*/)
                        //For debuggin bazier curve migh be replaced buy straight line
                        //                                ctx.lineTo(childPointAbsX + d.halfElementWidth, childPointAbsY + d.halfElementHeight)
                    }
                } else {
                    ctx.moveTo(pointAbsX + d.halfElementWidth, pointAbsY/* + d.halfElementHeight*/)
                    ctx.lineTo(childPointAbsX + d.halfElementWidth, childPointAbsY + elementHeight)
                }
                ctx.stroke()
                ctx.closePath()
            }
        }
    }
}
