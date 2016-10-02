#include "githandler.h"

#include <QDebug>
#include <QUrl>
#include <qqml.h>

#include <gitrepository.h>
#include <gitbranch.h>
#include <commitmodel.h>
#include <git2.h>

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

//    foreach(GitBranch* branch, branches) {
//        qDebug() << "Branch: " << branch->name();
//        CommitModel* model = CommitModel::fromBranch(branch);
//        m_commits.insert(model->head(), model);
//    }

    CommitModel* model = CommitModel::fromBranch(branches.value("master"));
    m_commits.insert(model->head(), model);
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
