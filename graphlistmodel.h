#ifndef GRAPHLISTMODEL_H
#define GRAPHLISTMODEL_H

#include <universallistmodel.h>
#include <graphpoint.h>
#include <QPointer>

class GraphListModel : public UniversalListModel<GraphPoint>
{
    Q_OBJECT
public:
    GraphListModel(QObject* parent = 0);

    Q_INVOKABLE GraphPoint* at(int i) const {
        return UniversalListModel::at(i);
    }
};

#endif // GRAPHLISTMODEL_H
