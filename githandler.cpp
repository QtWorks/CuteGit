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

    QMap<GitOid, GitBranch*> branchesSorted;
    CommitModel* model = nullptr;
//    CommitModel* model = CommitModel::fromBranch(branches.value("master"));
//    m_commits.insert(model->head(), model);
    foreach(GitBranch* branch, branches) {
        branchesSorted.insert(branch->oid(), branch);
//        if(m_commits.contains(branch->name())) {
//            continue;
//        }

//        model = CommitModel::fromBranch(branch);
//        m_commits.insert(model->head(), model);
    }

    foreach(GitBranch* branch, branchesSorted) {
        model = CommitModel::fromBranch(branch);
        m_commits.insert(model->head(), model);
    }


    CommitModel* main = new CommitModel("main");
    foreach (GitCommit* commitPtr, GitCommit::m_commitPool) {
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
