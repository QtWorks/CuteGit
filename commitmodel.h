#ifndef COMMITMODEL_H
#define COMMITMODEL_H

#include <universallistmodel.h>
#include <gitcommit.h>

//GitBranch;

class CommitGraph;

class CommitModel : public UniversalListModel<GitCommit>
{
    Q_OBJECT
    Q_PROPERTY(QString head READ head CONSTANT)
public:
    CommitModel(const QString& head, QObject* parent = 0);

    static CommitModel* fromBranch(GitBranch* branch);
    static CommitModel* fromGraph(CommitGraph* graph);
    QString head() const
    {
        return m_head;
    }

private:
    QString m_head;
};

#endif // COMMITMODEL_H
