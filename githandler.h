#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <QObject>

#include <repositorymodel.h>

class CommitModel;
class CommitGraph;
class BranchListModel;
class TagListModel;
class QFileSystemWatcher;
class GitConsole;

typedef QHash<QString, QPointer<CommitModel>> CommitModelContainer;

class GitHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RepositoryModel* repositories READ repositories NOTIFY repositoriesChanged)
    Q_PROPERTY(CommitGraph* graph READ graph NOTIFY graphChanged)
    Q_PROPERTY(GitRepository* activeRepo READ activeRepo CONSTANT)
    Q_PROPERTY(GitDiff* activeDiff READ activeDiff WRITE setActiveDiff NOTIFY activeDiffChanged)
    Q_PROPERTY(CommitModel* commits READ commits WRITE setCommits NOTIFY commitsChanged)
    Q_PROPERTY(BranchListModel* branchList READ branchList CONSTANT)
    Q_PROPERTY(TagListModel* tagList READ tagList CONSTANT)
    Q_PROPERTY(GitConsole* console READ console CONSTANT)

public:
    GitHandler();
    virtual ~GitHandler();
    Q_INVOKABLE void open(const QString &path);
    Q_INVOKABLE void open(const QUrl &url);

    Q_INVOKABLE GitDiff* diff(GitCommit* a, GitCommit* b);

    Q_INVOKABLE void copy(const QString& sha1);

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

    BranchListModel* branchList() const
    {
        return m_branchList;
    }

    TagListModel* tagList() const
    {
        return m_tagList;
    }

    GitConsole* console() const
    {
        return m_console;
    }

public slots:
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
    void updateModels();

    RepositoryModel* m_repositories;
    CommitModel* m_commits;
    CommitGraph* m_graph;
    GitRepository* m_activeRepo;
    QPointer<GitDiff> m_activeDiff;
    BranchListModel* m_branchList;
    TagListModel* m_tagList;
    QFileSystemWatcher* m_activeRepoWatcher;
    GitConsole* m_console;
    GitOid m_constantHead;
};

#endif // GITHANDLER_H
