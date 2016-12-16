#ifndef COMMITGRAPH_H
#define COMMITGRAPH_H

#include <QObject>

#include <gitoid.h>

#include <QString>
#include <QHash>
#include <QList>
#include <QPointer>

class GitCommit;
class GraphPoint;
class GraphListModel;
class GitBranch;
class GitDiff;

class CommitGraph : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QList<QObject*> points READ points CONSTANT)
    Q_PROPERTY(int branchesCount READ branchesCount NOTIFY branchesCountChanged)
    Q_PROPERTY(GraphListModel* points READ points NOTIFY pointsChanged)
public:
    CommitGraph();
    void addHead(GitBranch* branch);
    void addHead(const GitOid& oid);

    GraphListModel* points() const {
        return m_pointsModel;
    }

    int branchesCount() const
    {
        return m_branchesCount;
    }

signals:
    void branchesCountChanged(int branchesCount);

    void pointsChanged(GraphListModel* points);

private:
    void findParents(GitCommit *commit);
    void addCommits(QList<GitOid> &reversList);

    QString m_color;

    QHash<GitOid, GraphPoint*> m_points;
    QList<QPointer<GraphPoint> > m_sortedPoints;
    GraphListModel* m_pointsModel;
    int m_branchesCount;
};

#endif // COMMITGRAPH_H
