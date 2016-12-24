#ifndef TAGLISTMODEL_H
#define TAGLISTMODEL_H

#include <universallistmodel.h>
#include <gittag.h>

class TagListModel : public UniversalListModel<GitTag>
{
    Q_OBJECT
public:
    TagListModel(QObject* parent = 0);
};

#endif // TAGLISTMODEL_H
