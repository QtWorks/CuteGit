#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <QObject>

#include <repositorymodel.h>

class CommitModel;
typedef QHash<QString, QPointer<CommitModel>> CommitModelContainer;

class GitHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RepositoryModel* repositories READ repositories NOTIFY repositoriesChanged)

public:
    GitHandler();
    virtual ~GitHandler();
    Q_INVOKABLE void open(const QString &path);
    Q_INVOKABLE void open(const QUrl &url);

    RepositoryModel* repositories() const
    {
        return m_repositories;
    }

    Q_INVOKABLE CommitModel* modelByHead(const QString& head);

signals:
    void repositoriesChanged(RepositoryModel* repositories);

protected:
    QString lastError() const;

private:
    RepositoryModel* m_repositories;
    CommitModelContainer m_commits;
};

#endif // GITHANDLER_H
