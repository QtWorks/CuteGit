#include "githandler.h"

#include <QDebug>
#include <QUrl>
#include <qqml.h>

#include <gitrepository.h>
#include <gitbranch.h>
#include <commitmodel.h>
#include <git2.h>

#include <commitgraph.h>

GitHandler::GitHandler() : QObject()
  ,m_repositories(new RepositoryModel(this))
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

    GitRepository* repo = new GitRepository(QString::fromUtf8(root.ptr, root.size));
    if(!repo->isValid()) {
        qDebug() << lastError();
        return;
    }

    BranchContainer &branches = repo->branches();

    CommitGraph* graph = new CommitGraph();
    graph->addHead(branches.value("master").data()->oid());
    foreach(GitBranch* branch, branches) {
        qDebug() << "Next head " << branch->name();
        graph->addHead(branch->oid());
    }

    foreach (GitCommit* commit, graph->m_fullList) {
        qDebug() << commit->sha1();
        commit->m_y = graph->m_fullList.indexOf(commit);
    }

    CommitModel* main = new CommitModel("main");
    foreach (GitCommit* commitPtr, graph->m_commits) {
        main->add(commitPtr);
    }
    m_commits.insert("main", main);
    m_repositories->add(repo);
}

CommitModel* GitHandler::modelByHead(const QString& head)
{
    return m_commits.value(head).data();
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
