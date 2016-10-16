#ifndef COMMITGRAPH_H
#define COMMITGRAPH_H

#include <QObject>

#include <gitoid.h>

#include <QString>
#include <QHash>
#include <QObjectList>

class GitCommit;
class GraphPoint;

class CommitGraph : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> points READ points CONSTANT)
public:
    CommitGraph();
    void addHead(const GitOid& oid);

    QList<QObject*> points() const {
        return m_sortedPoints;
    }

private:
    void findParents(GitCommit *commit);
    void addCommits(QList<GitOid> &reversList);

    QString m_color;

    QHash<GitOid, GraphPoint*> m_points;
    QObjectList m_sortedPoints;
};

#endif // COMMITGRAPH_H
