#include "graphpoint.h"

GraphPoint::GraphPoint(const GitOid &commitOid, QObject* parent) : QObject(parent)
  ,m_commitOid(commitOid)
  ,m_x(0)
  ,m_y(0)
{

}

GraphPoint::GraphPoint(const GitOid &commitOid, int x, int y, const QString &color, QObject *parent) : QObject(parent)
  ,m_commitOid(commitOid)
  ,m_x(x)
  ,m_y(y)
  ,m_color(color)
{
}

GraphPoint::~GraphPoint()
{
}

void GraphPoint::setX(int x)
{
    if (m_x == x) {
        return;
    }

    m_x = x;
    emit xChanged(x);
}

void GraphPoint::setY(int y)
{
    if (m_y == y) {
        return;
    }

    m_y = y;
    emit yChanged(y);
}

void GraphPoint::setColor(const QString& color)
{
    if (m_color == color) {
        return;
    }

    m_color = color;
    emit colorChanged(color);
}

bool GraphPoint::addChildPoint(GraphPoint* point)
{
    bool orderChanged = false;
    if(m_childPoints.indexOf(point) < 0) {
        if(point->x() < x()) {
            for(int i = 0; i < m_childPoints.count(); i++) {
                GraphPoint* child = static_cast<GraphPoint*>(m_childPoints.at(i));
                child->setX(child->x() + 1);
            }
            orderChanged = true;
        }
        m_childPoints.append(point);
    }

    return orderChanged;
}

