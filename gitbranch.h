#ifndef GITBRANCH_H
#define GITBRANCH_H

#include <gitreference.h>

struct git_oid;

class GitBranch : public GitReference
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    GitBranch(git_reference* ref, GitRepository* parent);

    QString name() const
    {
        return m_name;
    }

    virtual ~GitBranch();

public slots:
    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(name);
    }

signals:
    void nameChanged(QString name);

private:
    void free();
    GitBranch();
    Q_DISABLE_COPY(GitBranch)
    QString m_name;

    git_annotated_commit* m_commit;
    git_oid* m_oid;
};

#endif // GITBRANCH_H
