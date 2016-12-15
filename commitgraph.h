#ifndef COMMITGRAPH_H
#define COMMITGRAPH_H

#include <QObject>

#include <gitoid.h>

#include <QString>
#include <QHash>
#include <QObjectList>

class GitCommit;
class GraphPoint;
class GitBranch;
class GitDiff;

class CommitGraph : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> points READ points CONSTANT)
    Q_PROPERTY(int branchesCount READ branchesCount NOTIFY branchesCountChanged)
public:
    CommitGraph();
    void addHead(GitBranch* branch);
    void addHead(const GitOid& oid);

    QList<QObject*> points() const {
        return m_sortedPoints;
    }

    int branchesCount() const
    {
        return m_branchesCount;
    }

signals:
    void branchesCountChanged(int branchesCount);

private:
    void findParents(GitCommit *commit);
    void addCommits(QList<GitOid> &reversList);

    QString m_color;

    QHash<GitOid, GraphPoint*> m_points;
    QObjectList m_sortedPoints;
    int m_branchesCount;
};

#endif // COMMITGRAPH_H
