#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <QObject>

#include <repositorymodel.h>
#include <QFutureWatcher>

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
    Q_PROPERTY(GitRepository* activeRepo READ activeRepo NOTIFY activeRepoChanged)
    Q_PROPERTY(GitDiff* activeDiff READ activeDiff WRITE setActiveDiff NOTIFY activeDiffChanged)
    Q_PROPERTY(CommitModel* commits READ commits WRITE setCommits NOTIFY commitsChanged)
    Q_PROPERTY(BranchListModel* branchList READ branchList CONSTANT)
    Q_PROPERTY(TagListModel* tagList READ tagList CONSTANT)
    Q_PROPERTY(GitConsole* console READ console CONSTANT)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)

public:
    GitHandler();
    virtual ~GitHandler();
    Q_INVOKABLE void open(const QString &path, bool activate = false);
    Q_INVOKABLE void open(const QUrl &url, bool activate = false);
    Q_INVOKABLE void activateRepository(int i);

    Q_INVOKABLE void diff(GitCommit* a, GitCommit* b);

    Q_INVOKABLE void diff(); //Dif of workdir
    Q_INVOKABLE void diffReset();
    void onDiffReady();

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

    bool isBusy() const
    {
        return !m_graphTask.isCanceled() && !m_graphTask.isFinished();
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

    void setActiveRepo(GitRepository* repo);

signals:
    void activeRepoChanged(GitRepository* activeRepo);
    void repositoriesChanged(RepositoryModel* repositories);
    void graphChanged(CommitGraph* graph);
    void activeDiffChanged(GitDiff* activeDiff);
    void commitsChanged(CommitModel* commits);
    void isBusyChanged();

protected:
    QString lastError() const;

private:
    void updateModels();

    static CommitGraph* updateGraph(const GitOid& head, const BranchContainer &branches);
    void onGraphReady();
    void loadCachedRepos();

    RepositoryModel* m_repositories;
    CommitModel* m_commits;
    CommitGraph* m_graph;
    GitRepository* m_activeRepo;
    QPointer<GitDiff> m_activeDiff;
    BranchListModel* m_branchList;
    TagListModel* m_tagList;
    QFileSystemWatcher* m_activeRepoWatcher;
    GitConsole* m_console;
    QFutureWatcher<GitDiff*> m_diffTask;
    QFutureWatcher<CommitGraph*> m_graphTask;
    bool m_isBusy;

    //    GitOid m_constantHead; //TODO:
};

#endif // GITHANDLER_H
