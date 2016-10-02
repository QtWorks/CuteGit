#ifndef GITBRANCH_H
#define GITBRANCH_H

#include <gitreference.h>
#include <git2/types.h>

struct git_oid;

class CommitModel;

class GitBranch : public GitReference
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(BranchType type READ type CONSTANT)
    Q_ENUMS(BranchType)
public:
    enum BranchType {
        Local = GIT_BRANCH_LOCAL,
        Remote = GIT_BRANCH_REMOTE
    };

    GitBranch(git_reference* ref, git_branch_t type, GitRepository* parent);
    virtual ~GitBranch();


    QString name() const {
        return m_name;
    }
    BranchType type() const;

public slots:
    void setName(QString name) {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(name);
    }

signals:
    void nameChanged(QString name);
    void commitsChanged(CommitModel* commits);

private:
    void free();
    GitBranch();
    Q_DISABLE_COPY(GitBranch)
    QString m_name;

    git_annotated_commit* m_commit;
    BranchType m_type;
};

#endif // GITBRANCH_H
