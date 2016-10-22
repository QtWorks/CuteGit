#include "commitgraph.h"

#include <gitcommit.h>
#include <graphpoint.h>

#include <QDateTime>

#include <git2/revwalk.h>
#include <git2/commit.h>

CommitGraph::CommitGraph() : QObject()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

void CommitGraph::addHead(const GitOid &oid)
{
    //Random color generation to be replaced with resets
    int red = qrand() % 205 + 50;
    int green = qrand() % 205 + 50;
    int blue = qrand() % 205 + 50;
    m_color = QString::number(red, 16) + QString::number(green, 16) + QString::number(blue, 16);

    git_revwalk* walk;
    git_revwalk_new(&walk, oid.repository()->raw());

    git_revwalk_push(walk, oid.raw());
    git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL);

    git_oid newOid;
    while(git_revwalk_next(&newOid, walk) == 0) {
        GitOid commitOid(&newOid, oid.repository());
        GitCommit *commit = GitCommit::fromOid(commitOid);
        findParents(commit);
    }

    git_revwalk_free(walk);

    QList<GitOid> openedBranches;
    qDebug() << "Update Y coordinate after head added";
    for(int i = 0; i < m_sortedPoints.count(); i++) {
        GraphPoint* point = static_cast<GraphPoint*>(m_sortedPoints.at(i));
        point->setY(m_sortedPoints.count() - i - 1);
        GitCommit *commit = GitCommit::fromOid(point->oid());
        git_commit* commitRaw = nullptr;
        int parentCount = git_commit_parentcount(commit->raw());
        int minX = point->x();
        for(int j = 0; j < parentCount; j++) {
            git_commit_parent(&commitRaw, commit->raw(), j);
            GitOid oidParent(git_commit_id(commitRaw), oid.repository());
            GraphPoint* parentPoint = m_points.value(oidParent);

            int x = parentPoint->x() + parentPoint->childPointsCount();

            qDebug() << "MinX = " << minX << " x:" << x;
            if(parentPoint->childPoints().indexOf(point) < 0) {
                minX = x;
//                if(x > minX) {
//                }
                point->setX(minX);
            }
            parentPoint->addChildPoint(point);
        }
        qDebug() << "New commit: " << point->oid().toString() << point->x() << point->y();
        point->setX(minX);
    }
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
            m_sortedPoints.append(parentPoint);
            m_points.insert(parentPoint->oid(), parentPoint);
        }

        point = m_points.value(childIter, nullptr);
        if(point == nullptr) {
            int parentPosition = m_sortedPoints.indexOf(parentPoint);
//            int x = parentPoint->x() + parentPoint->childPointsCount();
//            point = new GraphPoint(childIter, x, 0, m_color, this);
            point = new GraphPoint(childIter, 0, 0, m_color, this);
            m_points.insert(point->oid(), point);
//            parentPoint->addChildPoint(point);

            //Ordered commits
            if(parentPosition >= 0) {
                m_sortedPoints.insert(parentPosition + 1, point);
            }
//            qDebug() << "New commit: " << point->oid().toString() << point->x() << point->y();
//            qDebug() << "New commit parent: " << parentPoint->oid().toString() << parentPoint->x() << parentPoint->y();
        }
    }
}
