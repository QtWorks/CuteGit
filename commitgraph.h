#ifndef COMMITGRAPH_H
#define COMMITGRAPH_H

#include <QObject>

#include <QString>
#include <QHash>
#include <gitoid.h>

class GitCommit;

class CommitGraph : public QObject
{
public:
    CommitGraph();
    void addHead(const GitOid& oid);

    QHash<GitOid, GitCommit*> m_commits;
    QList<GitCommit*> m_fullList;

private:
    void findParents(GitCommit *commit);

    void addCommits(QList<GitOid> &reversList);

    QString m_color;
};

#endif // COMMITGRAPH_H
