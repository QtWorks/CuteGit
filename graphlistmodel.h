#ifndef GRAPHLISTMODEL_H
#define GRAPHLISTMODEL_H

#include <universallistmodel.h>
#include <graphpoint.h>
#include <QPointer>

class GraphListModel : public UniversalListModel<GraphPoint>
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    GraphListModel(QObject* parent = 0);

    QList<QPointer<GraphPoint> > container() const {
        return m_container;
    }

    Q_INVOKABLE GraphPoint* at(int i) const {
        return m_container.at(i).data();
    }

    int count() const
    {
        return m_container.count();
    }
signals:
    void countChanged(int count);
};

#endif // GRAPHLISTMODEL_H
