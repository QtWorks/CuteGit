#ifndef REPOSITORYMODEL_H
#define REPOSITORYMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QString>
#include <QPointer>

class GitRepository;

class RepositoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        RepoName,
        RepoRoot,
        RepoBranches
    };

    RepositoryModel(QObject *parent = 0);
    ~RepositoryModel();

    bool addRepository(GitRepository* repo);
    void removeRepository(GitRepository* repo);
    void removeRepository(const QString& root);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames() const;

private:
    QHash<QString, QPointer<GitRepository>> m_repositories;
    static QHash<int, QByteArray> m_roles;
};

#endif // REPOSITORYMODEL_H
