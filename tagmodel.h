#ifndef TAGMODEL_H
#define TAGMODEL_H

#include <gittag.h>
#include <universallistmodel.h>

class TagModel : UniversalListModel<GitTag>
{
public:
    TagModel(QObject* parent = 0);
};

#endif // TAGMODEL_H
