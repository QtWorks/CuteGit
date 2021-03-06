#include "commitmodel.h"

#include <gitbranch.h>
#include <git2/revwalk.h>

#include <commitgraph.h>
#include <graphpoint.h>
#include <graphlistmodel.h>

CommitModel::CommitModel(const QString &head, QObject* parent) : UniversalListModel(parent)
  ,m_head(head)
{
}

CommitModel* CommitModel::fromBranch(GitBranch* branch)
{
    CommitModel* tmpModel = new CommitModel(branch->fullName(), branch);
    git_revwalk* walk;
    git_revwalk_new(&walk, branch->repository()->raw());

    git_revwalk_push(walk, branch->oid().raw());
    git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL);

    git_oid newOid;
    while(git_revwalk_next(&newOid, walk) == 0) {
        GitOid commitOid(&newOid, branch->repository());
        GitCommit *commit = GitCommit::fromOid(commitOid);
        if(commit != nullptr) {
            tmpModel->append(commit);
        } else {
            qDebug() << "Commit is null";
        }
    }

    git_revwalk_free(walk);
    return tmpModel;
}

CommitModel* CommitModel::fromGraph(CommitGraph *graph)
{
    CommitModel* model = new CommitModel("HEAD");
    QList<QPointer<GraphPoint> > points = graph->points()->container();
    for(int i = 0; i < points.count(); i++) {
        GraphPoint* point = points.at(i).data();
        if(!point->oid().isValid()) {
            qDebug() << "prepend empty";
            model->m_container.prepend(nullptr);
        } else {
            model->m_container.prepend(GitCommit::fromOid(point->oid()));
        }
//        QPointer<GitTag> tag = commit->repository()->tags().value(commit->oid());
//        if(!tag.isNull()) {
//            point->setTag(tag.data()->name());
//        }
    }

    return model;
}
