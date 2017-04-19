#include "githandler.h"

#include <QUrl>
#include <QFileSystemWatcher>
#include <QGuiApplication>
#include <QClipboard>
#include <QtConcurrentRun>
#include <QDebug>
#include <qqml.h>

#include <gitrepository.h>
#include <gitbranch.h>
#include <gitdiff.h>
#include <commitmodel.h>
#include <tagmodel.h>
#include <git2.h>

#include <colorhandler.h>
#include <commitgraph.h>
#include <graphpoint.h>
#include <branchlistmodel.h>
#include <taglistmodel.h>
#include <gitconsole.h>
#include <graphpoint.h>
#include <settings.h>

GitHandler::GitHandler() : QObject()
  ,m_repositories(new RepositoryModel(this))
  ,m_commits(nullptr)
  ,m_graph(nullptr)
  ,m_activeRepo(nullptr)
  ,m_activeDiff(nullptr)
  ,m_branchList(new BranchListModel(this))
  ,m_tagList(new TagListModel(this))
  ,m_activeRepoWatcher(new QFileSystemWatcher(this))
  ,m_console(new GitConsole(this))
{
    git_libgit2_init();
    connect(&m_diffTask, &QFutureWatcher<GitDiff*>::finished, this, &GitHandler::onDiffReady);
    connect(&m_graphTask, &QFutureWatcher<CommitGraph*>::finished, this, &GitHandler::onGraphReady);

    connect(&m_graphTask, &QFutureWatcher<CommitGraph*>::started, this, &GitHandler::isBusyChanged);
    connect(&m_graphTask, &QFutureWatcher<CommitGraph*>::finished, this, &GitHandler::isBusyChanged);
    connect(&m_graphTask, &QFutureWatcher<CommitGraph*>::canceled, this, &GitHandler::isBusyChanged);
    connect(&m_graphTask, &QFutureWatcher<CommitGraph*>::paused, this, &GitHandler::isBusyChanged);
    connect(&m_graphTask, &QFutureWatcher<CommitGraph*>::resumed, this, &GitHandler::isBusyChanged);

    loadCachedRepos();
}

GitHandler::~GitHandler()
{
    git_libgit2_shutdown();
}

GitRepository *GitHandler::open(const QUrl &url)
{
    if(url.isLocalFile()) {
        return open(url.toLocalFile());
    }
    return nullptr;
}

GitRepository *GitHandler::open(const QString &path)
{
    qDebug() << "path" << path;
    git_buf root = {0,0,0};
    if(git_repository_discover(&root, path.toUtf8().data(), 0, NULL) != 0) {
        qDebug() << lastError();
        return nullptr;
    }

    GitRepository* repo = new GitRepository(QString::fromUtf8(root.ptr, root.size));
    Settings::instance()->add(repo);
    m_repositories->addRepository(repo);
    return repo;
}

void GitHandler::activateRepository(int i)
{
    GitRepository* repo = m_repositories->at(i);
    setActiveRepo(repo);
}

void GitHandler::setActiveRepo(GitRepository* repo)
{
    if(repo == nullptr || !repo->isValid()) {
        qDebug() << lastError();
        return;
    }

    if(m_activeRepo) {
        disconnect(m_activeRepoWatcher, &QFileSystemWatcher::directoryChanged, m_activeRepo, &GitRepository::readBranches);
        disconnect(m_activeRepoWatcher, &QFileSystemWatcher::directoryChanged, m_activeRepo, &GitRepository::readRemotes);
        disconnect(m_activeRepoWatcher, &QFileSystemWatcher::directoryChanged, m_activeRepo, &GitRepository::readTags);
        disconnect(m_activeRepoWatcher, &QFileSystemWatcher::directoryChanged, this, &GitHandler::updateModels);
        m_activeRepoWatcher->removePath(m_activeRepo->root());

        disconnect(m_activeRepo, &GitRepository::branchesChanged, this, &GitHandler::updateModels);
    }

    m_activeRepo = repo;

    connect(m_activeRepoWatcher, &QFileSystemWatcher::directoryChanged, m_activeRepo, &GitRepository::readBranches);
    connect(m_activeRepoWatcher, &QFileSystemWatcher::directoryChanged, m_activeRepo, &GitRepository::readRemotes);
    connect(m_activeRepoWatcher, &QFileSystemWatcher::directoryChanged, m_activeRepo, &GitRepository::readTags);
    connect(m_activeRepoWatcher, &QFileSystemWatcher::directoryChanged, this, &GitHandler::updateModels);
    connect(m_activeRepo, &GitRepository::branchesChanged, this, &GitHandler::updateModels);


    ColorHandler::instance().updateColors(m_activeRepo);
    m_constantHead = m_activeRepo->head();
    m_console->setRepository(m_activeRepo);
    connect(m_activeRepo, &GitRepository::branchesChanged, this, &GitHandler::updateModels);
    updateModels();

    m_activeRepoWatcher->addPath(m_activeRepo->root());

    qDebug() << "Active repo index: " << m_repositories->indexOf(m_activeRepo);
    m_repositories->setActiveRepositoryIndex(m_repositories->indexOf(m_activeRepo));
    activeRepoChanged(m_activeRepo);

    Settings::instance()->saveLastRepo(m_activeRepo);
}


QString GitHandler::lastError() const
{
    const git_error *e = giterr_last();
    if(e) {
        return QString("(%1): %2").arg(e->klass).arg(e->message);
        giterr_clear();
    }
    giterr_clear();
    return QString();
}

void GitHandler::diff(GitCommit* a, GitCommit* b)
{
    if(!m_activeDiff.isNull()) {
        m_activeDiff->deleteLater();
    }

    Q_ASSERT_X(a->repository() == b->repository(), "GitHandler", "Cross repository diff requested");

    if(m_diffTask.isRunning()) {
        m_diffTask.cancel();
    }

    qDebug() << "Diff start thread: " << QThread::currentThreadId();
    QFuture<GitDiff*> future = QtConcurrent::run(&GitDiff::diff, a, b);
    m_diffTask.setFuture(future);
}

void GitHandler::diff()
{
    if(!m_activeDiff.isNull()) {
        m_activeDiff->deleteLater();
    }

    GitCommit* commit = GitCommit::fromOid(activeRepo()->head());

    QFuture<GitDiff*> future = QtConcurrent::run(&GitDiff::diff, commit);
    m_diffTask.setFuture(future);
}

void GitHandler::diffReset()
{
    delete m_activeDiff;
}

void GitHandler::onDiffReady()
{
    setActiveDiff(m_diffTask.result());
}


void GitHandler::setActiveDiff(GitDiff* activeDiff)
{
    if (m_activeDiff == activeDiff)
        return;

    m_activeDiff = activeDiff;
    emit activeDiffChanged(activeDiff);
}

GitDiff* GitHandler::activeDiff() const
{
    return m_activeDiff.data();
}

void GitHandler::pull() const
{
    //    git_remote_fetch(m_activeRepo->remote)
}

void GitHandler::updateModels()
{
    if(!m_activeRepo) {
        return;
    }
    BranchContainer &branches = m_activeRepo->branches();
    m_activeRepo->updateHead();

    m_graphTask.cancel();
    m_graphTask.setFuture(QtConcurrent::run(&GitHandler::updateGraph, m_activeRepo->head(), branches));

    m_branchList->reset(branches.values());
    m_tagList->reset(m_activeRepo->tags().values());
}

void GitHandler::copy(const QString& sha1)
{
    QGuiApplication::clipboard()->setText(sha1);
}

CommitGraph* GitHandler::updateGraph(const GitOid &head, const BranchContainer &branches)
{
    CommitGraph* graph = new CommitGraph();

    bool headIsBranch = false;
    //TODO: Need to think about constant head more deeply
//    foreach(GitBranch* branch, branches) {
//        if(branch->oid() == m_constantHead) {
//            graph->addHead(branch);
//            headIsBranch = true;
//            break;
//        }
//    }

    if(!headIsBranch) {
        graph->addHead(head);
    }

    foreach(GitBranch* branch, branches) {
        qDebug() << "Next head " << branch->fullName();
        graph->addHead(branch);
    }
    graph->addWorkdir();
    graph->moveToThread(head.repository()->thread());
    return graph;
}

void GitHandler::onGraphReady()
{
    m_graph->deleteLater();
    m_graph = m_graphTask.result();
    emit graphChanged(m_graph);

    m_commits = CommitModel::fromGraph(m_graph);
    emit commitsChanged(m_commits);
}

void GitHandler::loadCachedRepos()
{
    QString activeRepo = Settings::instance()->loadLastRepo();
    GitRepository* lastRepo = nullptr;

    QStringList cachedRepos;
    Settings::instance()->load(cachedRepos);

    foreach (QString repoPath, cachedRepos) {
        GitRepository* repo = open(repoPath);
        if(repo->id() == activeRepo) {
            lastRepo = repo;
        }
    }

    setActiveRepo(lastRepo);
}
