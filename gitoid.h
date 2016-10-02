#ifndef GITOID_H
#define GITOID_H

#include <QObject>
#include <git2/oid.h>

class GitRepository;

class GitOid : public QObject
{
    Q_OBJECT
public:
    GitOid(const git_oid* oid, GitRepository *parent);
    GitOid(const GitOid& other);


    bool operator ==(const GitOid& other) const;
    GitOid& operator=(const GitOid& other);

    const git_oid* raw() const {
        return &m_oid;
    }

    GitRepository* repository() const {
        return m_repository;
    }

    const QLatin1String &toString() const;

    bool isValid() const;

private:
    void updateOid(const git_oid* oid);

    git_oid m_oid;
    QLatin1String m_string;
    GitRepository *m_repository;
};

#endif // GITOID_H
