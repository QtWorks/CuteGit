#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <QObject>

#include <repositorymodel.h>

class CommitModel;
class CommitGraph;

typedef QHash<QString, QPointer<CommitModel>> CommitModelContainer;

class GitHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RepositoryModel* repositories READ repositories NOTIFY repositoriesChanged)
    Q_PROPERTY(CommitGraph* graph READ graph WRITE setGraph NOTIFY graphChanged)

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

    CommitGraph* graph() const
    {
        return m_graph;
    }

public slots:
    void setGraph(CommitGraph* graph)
    {
        if (m_graph == graph)
            return;

        m_graph = graph;
        emit graphChanged(graph);
    }

signals:
    void repositoriesChanged(RepositoryModel* repositories);

    void graphChanged(CommitGraph* graph);

protected:
    QString lastError() const;

private:
    RepositoryModel* m_repositories;
    CommitModelContainer m_commits;
    CommitGraph* m_graph;
};

#endif // GITHANDLER_H
