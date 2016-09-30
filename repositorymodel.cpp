#include "repositorymodel.h"
#include <gitrepository.h>

#include <QDebug>

QHash<int, QByteArray> RepositoryModel::m_roles;

RepositoryModel::RepositoryModel(QObject* parent) : QAbstractListModel(parent)
{
    if(m_roles.isEmpty()) {
        m_roles[RepoName] = "repoName";
        m_roles[RepoRoot] = "repoRoot";
        m_roles[RepoBranches] = "repoBranches";
    }
}

RepositoryModel::~RepositoryModel()
{
    foreach (QPointer<GitRepository> repo, m_repositories) {
        delete repo.data();
    }
    m_repositories.clear();
}

bool RepositoryModel::addRepository(GitRepository* repo)
{
    if(repo == nullptr)
    {
        qDebug() << "Repo is null";
        return false;
    }

    if(m_repositories.contains(repo->root())) {
        qDebug() << "Repository" << repo->root() << "already exists";
        return false;
    }
    beginInsertRows(QModelIndex(),0,0);
    m_repositories.insert(repo->root(), QPointer<GitRepository>(repo));
    endInsertRows();
    return true;
}

void RepositoryModel::removeRepository(GitRepository* repo)
{
    if(repo == nullptr) {
        return;
    }

    if(m_repositories.contains(repo->root())) {
        beginRemoveRows(QModelIndex(), m_repositories.values().indexOf(repo), m_repositories.values().indexOf(repo));
        m_repositories.remove(repo->root());
        endRemoveRows();
    }
}

void RepositoryModel::removeRepository(const QString& root)
{
    m_repositories.remove(root);
}

QVariant RepositoryModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if(row < 0 || row >= m_repositories.count()) {
        return QVariant();
    }

    GitRepository* repo = m_repositories.values().at(row).data();

    switch (role) {
    case RepoName:
        return QVariant(repo->name());
    case RepoRoot:
        return QVariant(repo->root());
    default:
        break;
    }
    return QVariant();
}

int RepositoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_repositories.count();
}

QHash<int, QByteArray> RepositoryModel::roleNames() const
{
    return m_roles;
}

