#include "repositorymodel.h"

#include <QDebug>
#include <QSettings>

RepositoryModel::RepositoryModel(QObject* parent) : UniversalListModel(parent)
{
}

RepositoryModel::~RepositoryModel()
{
}

void RepositoryModel::addRepository(GitRepository *repository)
{
    if(m_repolist.contains(repository->name())) {
        qDebug() << "Repository is already opened";
        return;
    }

    m_repolist.append(repository->name());
    add(repository);
    setActiveRepositoryIndex(m_repolist.count() - 1);
}

void RepositoryModel::removeRepository(GitRepository* repository)
{
    m_repolist.removeAll(repository->name());
    remove(repository);
}
