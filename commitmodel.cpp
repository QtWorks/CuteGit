#include "commitmodel.h"

#include <gitbranch.h>
#include <git2/revwalk.h>

CommitModel::CommitModel(const QString &head, QObject* parent) : UniversalListModel(parent)
  ,m_head(head)
{
}

CommitModel* CommitModel::fromBranch(GitBranch* branch)
{
    qDebug() << branch->name();
    CommitModel* tmpModel = new CommitModel(branch->name(), branch);
    git_revwalk* walk;
    git_revwalk_new(&walk, branch->repository()->raw());

    git_revwalk_push(walk, branch->oid().raw());
    git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL);

    git_oid newOid;
    while(git_revwalk_next(&newOid, walk) == 0) {
        GitOid commitOid(&newOid, branch->repository());
        GitCommit *commit = GitCommit::fromOid(commitOid);
        if(commit != nullptr) {
            tmpModel->add(commit);
        } else {
            qDebug() << "Commit is null";
        }
    }

    git_revwalk_free(walk);
    return tmpModel;
}
