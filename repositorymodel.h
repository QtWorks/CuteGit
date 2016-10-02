#ifndef REPOSITORYMODEL_H
#define REPOSITORYMODEL_H

#include <universallistmodel.h>
#include <gitrepository.h>
#include <QString>
#include <QPointer>

class RepositoryModel : public UniversalListModel<GitRepository>
{
    Q_OBJECT
public:
    RepositoryModel(QObject *parent = 0);
    ~RepositoryModel();
};


#endif // REPOSITORYMODEL_H
