#include "commitgraph.h"

#include <git2/revwalk.h>
#include <git2/commit.h>

#include <gitcommit.h>
#include <QDateTime>

CommitGraph::CommitGraph() : QObject()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

void CommitGraph::addHead(const GitOid &oid)
{
    int red = qrand()%205 + 50;
    int green = qrand()%205 + 50;
    int blue = qrand()%205 + 50;

    m_color = QString::number(red, 16) + QString::number(green, 16) + QString::number(blue, 16);
    qDebug() << m_color;

    git_revwalk* walk;
    git_revwalk_new(&walk, oid.repository()->raw());

    git_revwalk_push(walk, oid.raw());
    git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL);

    git_oid newOid;
    while(git_revwalk_next(&newOid, walk) == 0) {
        qDebug() << "Next commit parents";
        GitOid commitOid(&newOid, oid.repository());
        GitCommit *commit = GitCommit::fromOid(commitOid);
        findParents(commit);
    }


    git_revwalk_free(walk);
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
        qDebug() << "Add commit to reverselist" << parentOid.toString();
        commitRaw = nullptr;
        git_commit_parent(&commitRaw, commit->raw(), 0);
    }

    if(reverseList.isEmpty()) {
        return;
    }
    addCommits(reverseList);
}

void CommitGraph::addCommits(QList<GitOid>& reversList)
{
    GitCommit* parent;
    GitCommit* commit;

    for(int i = 0; i < (reversList.count() - 1); i++) {
        parent = m_commits.value(reversList[i], nullptr);
        if(parent == nullptr) {
            //Ony in case if i==0
            parent = GitCommit::fromOid(reversList[i]);
            m_commits.insert(parent->oid(), parent);
            parent->m_color = m_color;
            m_fullList.push_back(parent);
        }

        commit = m_commits.value(reversList[i+1], nullptr);
        if(commit == nullptr) {
            commit = GitCommit::fromOid(reversList[i+1]);
            commit->m_x = parent->m_childrenCounter++;
            commit->m_childrenCounter = commit->m_x;
            m_commits.insert(commit->oid(), commit);
            commit->m_color = m_color;
            if(commit->m_x == parent->m_x) { //TODO: Too dirty hack seems will not work with amount of commits more than 1
                parent->m_color = commit->m_color;
            }

            int parentPosition = m_fullList.indexOf(parent);
            if(parentPosition >= 0) {
                m_fullList.insert(parentPosition + 1, commit);
            }
            qDebug() << "New commit: " << commit->sha1() << commit->x() << commit->y();
            qDebug() << "Parent commit: " << parent->sha1() << parent->x() << parent->y();
        }
    }
}
