import QtQuick 2.0

Canvas {
    id: root
    property int elementWidth: 20
    property int elementHeight: 20
    property int spacingH: 12
    property int spacingV: 20
    property int lineWidth: 1
    property QtObject model: null
    height: model.points.length*(elementWidth + spacingV)
    width: (elementWidth + spacingH)*model.branchesCount
    QtObject {
        id: d
        property int halfElementWidth: elementWidth/2
        property int halfElementHeight: elementHeight/2
    }

    onPaint: {
        var ctx = getContext("2d")
        for(var i = 0; i < model.points.length; i++) {
            var point = model.points[i]
            var pointAbsX = point.x*(elementWidth + spacingH)
            var pointAbsY = point.y*(elementHeight + spacingV)
            var childPoints = point.childPoints

            ctx.beginPath()
            ctx.lineWidth = root.lineWidth
            ctx.strokeStyle = "#"+point.color
            ctx.roundedRect(pointAbsX, pointAbsY, elementWidth, elementHeight, elementWidth, elementHeight)
            ctx.stroke()
            ctx.closePath()

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
