#include "commitgraph.h"

#include <gitcommit.h>
#include <gitbranch.h>
#include <gittag.h>

#include <colorhandler.h>

#include <graphpoint.h>
#include <graphlistmodel.h>

#include <QDateTime>
#include <QScopedPointer>

#include <git2/revwalk.h>
#include <git2/commit.h>

CommitGraph::CommitGraph() : QObject()
  ,m_pointsModel(new GraphListModel(this))
  ,m_branchesCount(0)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

void CommitGraph::addHead(GitBranch* branch)
{
    const GitOid& oid = branch->oid();
    addHead(oid);
}

void CommitGraph::addHead(const GitOid &oid)
{
    //Read color for this head
    m_color = ColorHandler::instance().color(oid);

    //Random color generation to be replaced with resets
    git_revwalk* walk;
    git_revwalk_new(&walk, oid.repository()->raw());

    git_revwalk_push(walk, oid.raw());
    git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL);

    git_oid newOid;
    while(git_revwalk_next(&newOid, walk) == 0) {
        GitOid commitOid(&newOid, oid.repository());
        GitCommit *commit = GitCommit::fromOid(commitOid);
        findParents(commit);
        delete commit;
    }

    git_revwalk_free(walk);

//    qDebug() << "Update Y coordinate after head added";
    QList<int> branchStarted;
    for(int i = 0; i < m_sortedPoints.count(); i++) {
        GraphPoint* point = m_sortedPoints.at(i);
        point->setY(m_sortedPoints.count() - i - 1);
        GitCommit *commit = GitCommit::fromOid(point->oid());
        git_commit* commitRaw = nullptr;
        int parentCount = git_commit_parentcount(commit->raw());
//        qDebug() << "New commit: " << point->oid().toString() << point->x() << point->y();
        for(int j = 0; j < parentCount; j++) {//Add connection to parent in case if count of parents > 1
            git_commit_parent(&commitRaw, commit->raw(), j);
            GitOid oidParent(git_commit_id(commitRaw), oid.repository());
            GraphPoint* parentPoint = m_points.value(oidParent);
            bool rearrangmentRequired = parentPoint->addChildPoint(point);
            Q_UNUSED(rearrangmentRequired)
//            if(rearrangmentRequired) { //TODO: need to investigate how to avoid croses on branches
                                         //in case of while addChildPoint operations we have conflict
                                         //with logic bellow
//                i = m_sortedPoints.indexOf(parentPoint) - 1;
//                break;
//            }

            //This logic denie to produce branches that are on the same 'x' line with other branches
            //that are active at this time. The loop bellow finds free 'x' line to use it for branch
            //allocation.
            if(parentPoint->x() < point->x()) {
                bool contains = false;
                while(branchStarted.contains(point->x())) {
                    contains = true;
                    point->setX(point->x() + 1);
                }
                if(!contains) {
                    branchStarted.append(point->x());
                }
            //Once branch is merged to another branch or ends "in the air" this logic releases branch
            //line and provides possibility to use freed 'x' for other branches.
            } else if(parentPoint->x() > point->x()) {
                branchStarted.removeAll(parentPoint->x());
            }

            if( point->childPoints().count() <= 0) {
                branchStarted.removeAll(point->x());
            }
        }
        delete commit;
        m_branchesCount = m_branchesCount < (point->x() + 1) ? (point->x() + 1) : m_branchesCount;
    }
    m_pointsModel->reset(m_sortedPoints);
    emit branchesCountChanged(m_branchesCount);
}

void CommitGraph::findParents(GitCommit* commit)
{
    QList<GitOid> reverseList;

    git_commit* commitRaw = commit->raw();
    while(commitRaw != nullptr)
    {
        GitOid parentOid(git_commit_id(commitRaw), commit->repository());
        commit = GitCommit::fromOid(parentOid);
        reverseList.push_front(parentOid);
        if(m_points.contains(parentOid)) { //Finish parents lookup once parent found in tree. We will see nothing new in this branch
            break;
        }

//        qDebug() << "Add commit to reverselist" << parentOid.toString();
        commitRaw = nullptr;
        git_commit_parent(&commitRaw, commit->raw(), 0);
    }

    if(reverseList.count() < 2) { //In case if only original commit in list, we didn't find anything new
        return;
    }
    addCommits(reverseList);
}

void CommitGraph::addCommits(QList<GitOid>& reversList)
{
    GraphPoint* point = nullptr;
    GraphPoint* parentPoint = nullptr;

    for(int i = 0; i < (reversList.count() - 1); i++) {
        GitOid& parentIter = reversList[i];
        GitOid& childIter = reversList[i + 1];
        parentPoint = m_points.value(parentIter, nullptr);
        if(parentPoint == nullptr) {
            parentPoint = new GraphPoint(parentIter, this);
            parentPoint->setColor(m_color);
            m_sortedPoints.prepend(QPointer<GraphPoint>(parentPoint));
            m_points.insert(parentPoint->oid(), parentPoint);
        }

        point = m_points.value(childIter, nullptr);
        if(point == nullptr) {
            int parentPosition = m_sortedPoints.indexOf(parentPoint);
            int x = parentPoint->x() + parentPoint->childPointsCount();
            point = new GraphPoint(childIter, x, 0, m_color, this);
            m_points.insert(point->oid(), point);
            //Ordered commits
            if(parentPosition >= 0) {
                m_sortedPoints.insert(parentPosition + 1, point);
            }
//            qDebug() << "New commit: " << point->oid().toString() << point->x() << point->y();
//            qDebug() << "New commit parent: " << parentPoint->oid().toString() << parentPoint->x() << parentPoint->y();
        }

        if(point) { //add child point in any case
            parentPoint->addChildPoint(point);
        }
    }
}

GraphPoint* CommitGraph::point(const GitOid& oid)
{
    GraphPoint* point = m_points.value(oid, nullptr);
    return point;
}

GraphPoint* CommitGraph::point(int i)
{
    return m_sortedPoints.at(i);
}
