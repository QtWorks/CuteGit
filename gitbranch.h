#ifndef GITBRANCH_H
#define GITBRANCH_H

#include <gitreference.h>
#include <git2/types.h>

struct git_oid;

class GitBranch : public GitReference
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString fullName READ fullName NOTIFY fullNameChanged)
    Q_PROPERTY(QString remote READ remote NOTIFY remoteChanged)
    Q_PROPERTY(BranchType type READ type CONSTANT)
    Q_PROPERTY(GitOid oid READ oid CONSTANT)
    Q_ENUMS(BranchType PullStrategy)

public:
    enum BranchType {
        Invalid = 0,
        Local = GIT_BRANCH_LOCAL,
        Remote = GIT_BRANCH_REMOTE
    };

    enum PullStrategy {
        Rebase,
        Merge
    };

    GitBranch(git_reference *ref, git_branch_t type, GitRepository *parent);
    GitBranch(GitBranch &&other);
    GitBranch &operator=(GitBranch &&other);

    virtual ~GitBranch();

    BranchType type() const;

    QString name() const {
        return m_name;
    }

    QString fullName() const {
        return m_fullName;
    }

    git_annotated_commit *annotatedCommit()
    {
        return m_commit;
    }

    QString remote() const
    {
        return m_remote;
    }

    GitBranch upstream() const;

    void setUpstream(const GitBranch &branch);

    void pull(PullStrategy strategy);

public slots:
    void setName(QString name) {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(name);
    }

signals:
    void nameChanged(QString name);

    void fullNameChanged(QString fullName);

    void remoteChanged(QString remote);

private:
    void free();
    void fastForward();

    GitBranch();
    Q_DISABLE_COPY(GitBranch)

    git_annotated_commit *m_commit;
    BranchType m_type;
    QString m_name;
    QString m_fullName;
    QString m_remote;
};

#endif // GITBRANCH_H
