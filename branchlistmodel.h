#ifndef BRANCHLISTMODEL_H
#define BRANCHLISTMODEL_H

#include <universallistmodel.h>
#include <gitbranch.h>

class BranchListModel : public UniversalListModel<GitBranch>
{
    Q_OBJECT
public:
    BranchListModel(QObject *parent = 0);
};

#endif // BRANCHLISTMODEL_H
