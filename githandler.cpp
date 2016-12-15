#include "githandler.h"

#include <QDebug>
#include <QUrl>
#include <qqml.h>

#include <gitrepository.h>
#include <gitbranch.h>
#include <gitdiff.h>
#include <commitmodel.h>
#include <tagmodel.h>
#include <git2.h>

#include <commitgraph.h>
#include <graphpoint.h>

GitHandler::GitHandler() : QObject()
  ,m_repositories(new RepositoryModel(this))
  ,m_activeDiff(nullptr)
{
    git_libgit2_init();
}

GitHandler::~GitHandler()
{
    git_libgit2_shutdown();
}

void GitHandler::open(const QUrl &url)
{
    if(url.isLocalFile()) {
        open(url.toLocalFile());
    }
}

void GitHandler::open(const QString &path)
{
    git_buf root = {0,0,0};
    if(git_repository_discover(&root, path.toUtf8().data(), 0, NULL) != 0) {
        qDebug() << lastError();
        return;
    }

    m_activeRepo = new GitRepository(QString::fromUtf8(root.ptr, root.size));
    if(!m_activeRepo->isValid()) {
        qDebug() << lastError();
        return;
    }

    BranchContainer &branches = m_activeRepo->branches();

    CommitGraph* graph = new CommitGraph();
    graph->addHead(branches.value("master").data()->oid());
    foreach(GitBranch* branch, branches) {
        qDebug() << "Next head " << branch->name();
        graph->addHead(branch);
    }

    setGraph(graph);
    m_commits = CommitModel::fromGraph(graph);
    m_repositories->add(m_activeRepo);
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

GitDiff* GitHandler::diff(GitCommit* a, GitCommit* b)
{
    if(!m_activeDiff.isNull()) {
        m_activeDiff->deleteLater();
    }

    Q_ASSERT_X(a->repository() == b->repository(), "GitHandler", "Cross repository diff requested");

    m_activeDiff = new GitDiff(a->raw(), b->raw(), a->repository());
    return m_activeDiff.data();
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
