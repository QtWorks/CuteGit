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
    Q_PROPERTY(GitRepository* activeRepo READ activeRepo CONSTANT)
    Q_PROPERTY(GitDiff* activeDiff READ activeDiff WRITE setActiveDiff NOTIFY activeDiffChanged)
    Q_PROPERTY(CommitModel* commits READ commits WRITE setCommits NOTIFY commitsChanged)

public:
    GitHandler();
    virtual ~GitHandler();
    Q_INVOKABLE void open(const QString &path);
    Q_INVOKABLE void open(const QUrl &url);

    Q_INVOKABLE GitDiff* diff(GitCommit* a, GitCommit* b);

    RepositoryModel* repositories() const
    {
        return m_repositories;
    }

    CommitGraph* graph() const
    {
        return m_graph;
    }

    GitRepository* activeRepo() const
    {
        return m_activeRepo;
    }

    GitDiff* activeDiff() const;

    CommitModel* commits() const
    {
        return m_commits;
    }

    void pull() const;

public slots:
    void setGraph(CommitGraph* graph)
    {
        if (m_graph == graph)
            return;

        m_graph = graph;
        emit graphChanged(graph);
    }

    void setActiveDiff(GitDiff* activeDiff);

    void setCommits(CommitModel* commits)
    {
        if (m_commits == commits)
            return;

        m_commits = commits;
        emit commitsChanged(commits);
    }

signals:
    void repositoriesChanged(RepositoryModel* repositories);

    void graphChanged(CommitGraph* graph);

    void activeDiffChanged(GitDiff* activeDiff);

    void commitsChanged(CommitModel* commits);

protected:
    QString lastError() const;

private:
    RepositoryModel* m_repositories;
    CommitModel* m_commits;
    CommitGraph* m_graph;
    GitRepository* m_activeRepo;
    QPointer<GitDiff> m_activeDiff;
};

#endif // GITHANDLER_H
